//------------------------------------------------------------------------- 
// This is supporting software for CS415P/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A sequential quicksort program.
//
// Usage: ./qsort N
// 
//
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define MINSIZE   10 		// threshold for switching to bubblesort

// Swap two array elements 
//
void swap(int *array, int i, int j) {
  if (i == j) return;
  int tmp = array[i];
  array[i] = array[j];
  array[j] = tmp;
}

// Initialize array.
// - first generate [1,2,...,N]
// - then perform a random permutation
//
int *init_array(int N)  {
  int *array = (int *) malloc(sizeof(int) * N);
  for (int i = 0; i < N; i++) {
    array[i] = i + 1;
  }
  srand(time(NULL));
  for (int i = 0; i < N; i++) {
    int j = (rand()*1./RAND_MAX) * (N-1);
    swap(array, i, j);
  }
  printf("Initialized array to a random permutation of [1..%d]\n", N);
  return array;
}

// Verify that array is sorted
//
void verify_array(int *array, int N) {
  for (int i = 0; i < N-1; i++)
    if (array[i]>array[i+1]) {
      printf("FAILED: array[%d]=%d, array[%d]=%d\n", 
	     i, array[i], i+1, array[i+1]);
      return;
    }
  printf("Result verified!\n");
}

// Bubble sort for the base cases
//
void bubblesort(int *array, int low, int high) {
  if (low >= high)
    return;
  int i,j;
#pragma omp parallel for shared(i,j) collapse(2)
  for (i = low; i <= high; i++)
    for (j = i+1; j <= high; j++) 
      if (array[i] > array[j])
        swap(array, i, j);
  printf("Thread %d sorted [%d, %d]\n", omp_get_thread_num(), low, high);
}      

// Pick an arbitrary element as pivot. Rearrange array 
// elements into [smaller one, pivot, larger ones].
// Return pivot's index.
//
int partition(int *array, int low, int high) {
  int pivot = array[high]; 	// use highest element as pivot 
  int middle = low;
  for(int i = low; i < high; i++)
    if(array[i] < pivot) {
      swap(array, i, middle);
      middle++;
    }
  swap(array, high, middle);
  return middle;
}
 
// QuickSort an array range
// 
void quicksort(int *array, int low, int high) {
  if (high - low < MINSIZE) {
    #pragma omp single
    bubblesort(array, low, high);
    return;
  }
  int middle = partition(array, low, high);
  printf("Thread %d found middle [%d]\n", omp_get_thread_num(), middle);
  if (low < middle)
  #pragma omp sections
  {
    #pragma omp section
    quicksort(array, low, middle-1);
  if (middle < high)
    #pragma omp section
    quicksort(array, middle+1, high);
  }
}
 
// Main routine for testing quicksort
// 
int main(int argc, char **argv) {
  int *array, N, num_t;
  
  // check command line first 
  if (argc < 2) {
    printf ("Usage: ./qsort N\n");
    exit(0);
  }
  if ((N = atoi(argv[1])) < 2) {
    printf ("N must be greater than 2\n");
    exit(0);
  }
  if (argc == 3) {
    if ((num_t = atoi(argv[2])) < 1) {
      printf("num threads setting to default of 1");
      num_t = 1;
    }
  }
  omp_set_num_threads(num_t);

  array = init_array(N);
  printf("Sorting started ...|t|=%d\n", num_t);
  quicksort(array, 0, N-1);
  printf("... completed.\n");
  verify_array(array, N);
}

