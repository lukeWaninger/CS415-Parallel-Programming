//------------------------------------------------------------------------- 
// This is supporting software for CS415P/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Matrix multiplication algorithm.  
//
#include <stdio.h>

#define N 8

int main() {
  int a[N][N], b[N][N], c[N][N];
  int i, j, k;

  // initialization
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = i + j;
      b[i][j] = 1;
    }
  }

  // multiplication
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
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
  printf("total = %d (should be 3584)\n", total);

}
