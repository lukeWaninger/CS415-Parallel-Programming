//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A simple recursive routine.
//
#include <stdio.h>
#include <omp.h>

int array[8] = {1,2,3,4,5,6,7,8};

// array a[]'s size n is assumped to be a power of 2
void rec(int a[], int n) {
  if (n == 1) {
    printf("thread %d\n", a[0]);
  } else {
    rec(a,n/2);
    rec(a+n/2, n/2);
  }
}

int main() {
  rec(array, 8);
}
