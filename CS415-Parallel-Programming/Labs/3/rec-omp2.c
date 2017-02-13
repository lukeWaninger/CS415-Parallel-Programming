//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A simple recursive routine (OMP version 2).
//
#include <stdio.h>
#include <omp.h>

const int N = 8
int array[N] = {1,2,3,4,5,6,7,8};

// array a[]'s size n is assumped to be a power of 2
void rec(int a[], int n) {
  if (n == 1) {
    printf("%d by thread %d\n", a[0], omp_get_thread_num());
  } else {
#pragma omp parallel 
    {
#pragma omp task
    rec(a,n/2);
#pragma omp task
    rec(a+n/2, n/2);
    }
  }
}

int main() {
  rec(array, N);
}
