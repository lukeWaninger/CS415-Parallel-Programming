//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A simple recursive routine (OMP version 1).
//
#include <stdio.h>
#include <omp.h>

const int N = 8;
int array[8];

// array a[]'s size n is assumped to be a power of 2
void rec(int a[], int n) {
  if (n == 1) {
#pragma omp taskwait
    printf("%d by thread %d \t%d\n", a[0], omp_get_thread_num(), omp_get_team_size(omp_get_level()));
  } else {
//#pragma omp sections
    {
//#pragma omp section
#pragma omp task 
    rec(a,n/2);
//#pragma omp section
#pragma omp task
    rec(a+n/2, n/2);
    }
  }
//#pragma omp taskwait
}

int main() {
  for (int i = 0; i < N; ++i)
    array[i] = i;
  int n = N;
#pragma omp parallel num_threads(8)
#pragma omp single
  rec(array, n);
}
