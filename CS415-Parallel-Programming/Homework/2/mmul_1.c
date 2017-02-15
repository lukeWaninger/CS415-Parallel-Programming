//------------------------------------------------------------------------- 
// This is supporting software for CS415P/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Matrix multiplication algorithm.  
//
#include <stdio.h>
#include <omp.h>

#define N 8

int main() {
  int a[N][N], b[N][N], c[N][N];
  int i, j, k;
  
  // for stats gathering
  int t_limit = omp_get_max_threads();
  int stats[t_limit];
  for (i = 0; i < t_limit; ++i)
    stats[i] = 0;

  // initialization
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = i + j;
      b[i][j] = 1;
    }
  }

  // multiplication
#pragma omp parallel for private(i,j,k)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      int tid = omp_get_thread_num();
      printf("[%d, %d] by thread %d\n", i, j, tid);
      stats[tid]++;
  	  c[i][j] = 0.;
  
      for (k = 0; k < N; k++) {
  	    c[i][j] += a[i][k] * b[k][j];
    	}
    }
  }

  // validation
  int total = 0;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      total += c[i][j];
    }
  }

  // print stats
  printf("\n");
  for (i = 0; i < t_limit; ++i) {
    printf("[%2d]: %d\t", i, stats[i]);
    if (i%4 == 3) printf("\n");
  }
  
  printf("\ntotal = %d (should be 3584)\n", total);
}
