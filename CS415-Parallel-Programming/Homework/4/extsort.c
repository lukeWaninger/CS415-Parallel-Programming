#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define TAG 1001

int main(int argc, char*argv[]) {
  int rank, P, N;
  MPI_File infile, outfile;
  MPI_Status status;

  if (argc != 3) {
    printf("command must contain 2 arguments\n");
    exit(0);
  }
 
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &P);

  // if P0, open input file, find and  broadcast N
  if (rank == 0) {
    MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &infile);
    N = MPI_File_get_size(infile, NULL);
    printf("file size: %d\n", N);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }

  // all processes
  int p = 0;
  while (p++ < P) {
    if (p != rank) continue;

    MPI_Recv(&N, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
    printf("P%d recieved %d\n", rank, N);

    // check if P evenly divides N and P^2 <= N
    if (!N&P || P*P > N) {
      printf("%d does not divide %d or %d^2>%d. terminating\n", P, N, P, N);
      exit(0);
    }

    // allocate a buffer of size N/P
    int* data = malloc(sizeof(int)*N/P);
    
    // open the input file to a specific view for each process
    // what to do about p(0) who already has the file open....
    MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &infile);
    // set the file view for each process
    MPI_Offset disp = p*sizeof(int);
    MPI_File_set_view(infile, disp, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);

    // read the ints
    // TODO: VERIFY READ BOUNDS ARE CORRECT AND NO INT IS 
    // TODO: MISSED OR READ TWICE BY DIFFERENT THREADS
    MPI_File_read(infile, &data, N/P, MPI_INT, &status);
    MPI_File_close(&infile);
    
    // print the data assigned to each thread for verification
    for (int i=0; i<sizeof(data)/sizeof(int); i++) {
      printf("[p:%d] %d, ", p, data[i]);
    }
    printf("\n");
    // every process sorts its data
     

    free(data);
  }


  MPI_Finalize();
}
