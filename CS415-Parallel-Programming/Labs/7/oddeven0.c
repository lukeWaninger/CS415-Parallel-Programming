// A sequential odd-even sorting algorithm in C. 
//
// Usage: ./oddeven-seq [N]
// 
// (This is an incomplete version.)
//
//
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Swap two array elements 
//
void swap(int *a, int i, int j) {
  if (i == j) return;
  int tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

// Initialize array.
// - First initialize array to [1,2,...,N],
//   then randomly permute the elements
//
void init_array(int *a, int n) {
  for (int i = 0; i < n; i++) {
    a[i] = i + 1;
  }
  srand(time(NULL));
  for (int i = 0; i < n; i++) {
    int j = rand() % (n-1) + 1;
    swap(a, i, j);
  }
}

// Print array 
//
void print_array(int *a, int n) {
  for (int i = 0; i < n; i++)
    printf("%d ", a[i]);
  printf("\n");
}

// Verify that array is sorted
//
void verify_array(int *a, int n) {
  for (int i = 0; i < n-1; i++)
    if (a[i] > a[i+1]) {
      printf("FAILED: a[%d]=%d, a[%d]=%d\n", 
	     i, a[i], i+1, a[i+1]);
      return;
    }
  printf("Result verified!\n");
}

// Odd-even sort (sequential simulation)
// - Print out the array after each iteration (together with 
//   the iteration number)
//
void oddeven_sort(int *a, int n) {

  // ... need code

}

// Main routine
// - Read in the value of N from command-line, verify that it's 
//   an even number. If N is not provided, use the default value
// - Allocate and initialize array
// - Sort the array and verify the result
// 
int main(int argc, char **argv) {
  int *a, N = 16;

  // ... need code

  init_array(a, N);
  printf("[Init] ");
  print_array(a, N);

  oddeven_sort(a, N);
  verify_array(a, N);
}

