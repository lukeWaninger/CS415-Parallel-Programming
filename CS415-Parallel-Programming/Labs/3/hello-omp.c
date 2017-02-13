//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Hello world program (OMP version).
//
#include <stdio.h>
#include <omp.h>

int main() {
#pragma omp parallel num_threads(100)
  printf("Hello world! -- thread %d\n", omp_get_thread_num());
}
