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
      printf("%d does not divide %d or %d^2 > %d. terminating\n", P, N, P, N);
      exit(0);
    }
  }


  // every process allocates a buffer with size of N/P
  MPI_Finalize();
}
