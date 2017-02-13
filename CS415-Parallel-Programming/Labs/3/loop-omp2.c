//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A simple double loop (OMP version 2).
//
#include <stdio.h>
#include <omp.h>

#define N 4

int main() {
  int i, j, total=0;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      total += i + j;
      printf("[%d,%d]\n", i, j);
    }
  }
  printf("Total = %d (should be 48)\n", total);

}
