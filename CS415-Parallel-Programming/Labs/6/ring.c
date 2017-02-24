//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// A simple demo program of MPI send-receive routines.
//
// - Process 0 sends an integer to process 1.
// - Process 1 decreases the integer's value by one, and sends it back
//
// Usage: 
//   linux> mpirun -hostflie <hostfile> -n <#processes> simple [<N>]
// 
// 
#define _BSD_SOURCE
#include <unistd.h>	// for gethostname()
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define TAG 1001

int main(int argc, char *argv[])
{
  int nprocs, rank;
  char host[20];
  MPI_Status status;
  int N = 10;		// default integer value
  
  if (argc == 2) {
    N = atoi(argv[1]);	// overwirte the value
  }
  gethostname(host, 20);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  if (nprocs < 2) {
    printf("Need at least 2 processes.\n");
    MPI_Finalize();
    return(1);
  }
  printf("P%d/%d started on %s ...\n", rank, nprocs, host);

  if (rank == 0) {
    MPI_Send(&N, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
    printf("P%d/%d starting message passing on %s...  [N=%d]\n", rank, nprocs, host, N);
  }
  int i = 0;
  while (i++ < nprocs) {
    if (i != rank) continue;
    
    MPI_Recv(&N, 1, MPI_INT, i-1, TAG, MPI_COMM_WORLD, &status);
    printf("P%d recieved %d\n", rank, N);
    N++;
    MPI_Send(&N, 1, MPI_INT, (i+1)%nprocs, TAG, MPI_COMM_WORLD);
    printf("P%d sent %d to P%d\n", rank, N, (i+1)%nprocs);
  } 

  MPI_Finalize();
}
