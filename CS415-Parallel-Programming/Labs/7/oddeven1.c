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
    printf("%2d ", a[i]);
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
  int sorted = 0;
  for (int i=0; i < n && !sorted; i++) {
    sorted = 1;
    if (i&1) {
      for (int j=2; j < n; j+=2) {
        if (a[j] < a[j-1]) {
          swap(a, j-1, j);
          sorted = 0;
        }
      }
    }
    else {
      for (int j=1; j<n; j+=2) {
        if (a[j] < a[j-1]) {
          swap(a, j-1, j);
          sorted = 0;
        }
      }
    }
    printf("[t= %2d] ", i+1);
    print_array(a, n);
    if (sorted) 
      printf("array sorted in %d steps\n", i);
  }
}

// Main routine
// - Read in the value of N from command-line, verify that it's 
//   an even number. If N is not provided, use the default value
// - Allocate and initialize array
// - Sort the array and verify the result
// 
int main(int argc, char **argv) {
  int *a, N = 16;

  if (argc > 1) {
    if ((N = atoi(argv[1])) < 2 || N&0) {
      N = 16;
      printf("N must be at least 2 and even\n");
    }
  }
  a = malloc(sizeof(int) * N);

  init_array(a, N);
  printf("[Init ] ");
  print_array(a, N);

  oddeven_sort(a, N);
  verify_array(a, N);
  free(a);
}

