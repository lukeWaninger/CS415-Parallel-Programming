#define _BSD_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "qsort.c"

#define TAG 1001

int main(int argc, char*argv[]) {
  int rank, P, N;
  char host[20];
  MPI_File infile, outfile;
  MPI_Status status;

  if (argc != 3) {
    printf("command must contain 2 arguments\n");
    exit(0);
  }
  gethostname(host, 20);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  printf("rank %3d starting on %s \n", rank, host);

  // if P0, open input file, find and  broadcast N
  if (rank == 0) {
    MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &infile);
    MPI_Offset n; MPI_File_get_size(infile, &n); N = (int)n/4;
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }
  else {
    MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &infile);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }

  // check if P evenly divides N and P^2 <= N, terminate if it does not
  if (N%P || P*P >= N) {
    printf("%d does not divide %d or %d>=%d. rank %d terminating\n", P, N, P*P, N, rank);
    MPI_Finalize();
    exit(0);
  }

  // allocate a buffer of size N/P
  int range = N/P;
  int data[range];

  // set the file view for each process
  MPI_File_set_view(infile, rank*range*sizeof(int), MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  
  // perform a collective file read
  MPI_File_read(infile, data, range, MPI_INT, &status);
  MPI_File_close(&infile);

  // every process sorts its data
  quicksort(data, 0, range-1); 
 
  // P0 partitions its sorted data into P equal-sized sections
  // then broadcasts the pivots to all other processes
  int pivots[P-1];
  if (rank == 0) {
    int stride = range/P;
    for (int i=stride-1, j=0; i<range && j<P-1; i+=stride, j++) 
      pivots[j] = data[i];
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&pivots, P-1, MPI_INT, 0, MPI_COMM_WORLD);
  }
  else {
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&pivots, P-1, MPI_INT, 0, MPI_COMM_WORLD);
  } 
  
    // find counts and displacements for a scatterv operation
  int scounts[P], sdispls[P];
  for(int i=0;i<P;i++) scounts[i]=sdispls[i]=0;

  int i=0,j=0,s=0,t=0;
  for (i,t; i<range && j<P-1; i++,t++) {
    if (data[i] <= pivots[j]) continue;
    else {
      sdispls[j] = s; s = i;
      scounts[j] = t; t = 0;
      j++;
    }
  }
  sdispls[j] = s;
  scounts[j] = range-i+1;
 
  //printf("[%d] pass 88\n", rank);

  // scatter all items from ith bucket to ith process
  int l = 2*N/P;
  int mybucket[l]; for(int i=0;i<l;i++) mybucket[i]=-1;
  int *b = mybucket;

  //printf("[%d] pass 93\n", rank);
  //for (int i=0;i<range;i++) printf("%2d ", data[i]); printf("\n");
  //for (int i=0;i<P;i++) printf("(%d->%d) ", sdispls[i], scounts[i]); printf("\n");

  for (int i=0;i<P;i++) {
    MPI_Scatterv(data, scounts, sdispls, MPI_INT, b, l, MPI_INT, i, MPI_COMM_WORLD);
    while(*b++!=-1);b--;
  }

  //printf("[%d] pass 100\n", rank);

  // each process sorts its bucket
  quicksort(mybucket, 0, l-1);

  // each process writes its bucket, one at a time in proc order, to the output file
  int c=1; if (P>1) c=0;
  b = mybucket; while(*b++==-1) { c++; } b--;
  for (int i=0;i<P;i++) {
    if (i == rank) {
      MPI_File_open(MPI_COMM_SELF, argv[2], MPI_MODE_CREATE|MPI_MODE_APPEND|MPI_MODE_RDWR, MPI_INFO_NULL, &outfile);
      MPI_File_write(outfile, b, l-c, MPI_INT, &status);
      MPI_File_close(&outfile);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  
  printf("rank %3d finished with %s\n", rank, host);
  MPI_Finalize();
}
