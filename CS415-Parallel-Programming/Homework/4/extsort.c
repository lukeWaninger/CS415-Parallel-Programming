#define _BSD_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define TAG 1001

int main(int argc, char*argv[]) {
  int rank, P, N;
  char host[20];
  MPI_File infile;
  MPI_Status status;

  if (argc != 3) {
    printf("command must contain 2 arguments\n");
    exit(0);
  }
  gethostname(host, 20);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &P);

  //printf("%s starting rank %d of %d\n", host, rank, P);
  // if P0, open input file, find and  broadcast N
  if (rank == 0) {
    //printf("%s reading %s\n", host, argv[1]);
    MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &infile);
    // read in rile size
    MPI_Offset n; MPI_File_get_size(infile, &n); N = (int)n/4;
    // printf("file size: %d. broadcasting from %s\n", N, host);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }
  else {
    MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &infile);
    //printf("%s[%d] waiting for broadcast...\n", host, p);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //printf("P%d received %d\n", rank, N);
  }

  // check if P evenly divides N and P^2 <= N, terminate if it does not
  if (!N&P || P*P > N) {
    printf("%d does not divide %d or %d^2>%d. terminating\n", P, N, P, N);
    MPI_Finalize();
    exit(0);
  }

  // allocate a buffer of size N/P
  int range = N/P;
  int data[range];
  for (int i=0; i<range; i++) data[i] = 0;

  // set the file view for each process
  MPI_File_set_view(infile, rank*range*sizeof(int), MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  
  // read the ints
  MPI_File_read(infile, data, range, MPI_INT, &status);
  MPI_File_close(&infile);

  // print the data assigned to each thread for verification
  for (int i=0; i<range; i++) {
    printf("%3d ", data[i]);
  }
  printf("  [%d]\n",rank);
  
  // every process sorts its data
  
  
  MPI_Finalize();
}
