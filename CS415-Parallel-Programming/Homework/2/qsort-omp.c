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
int *stats;

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
  for (i = low; i <= high; i++)
    for (j = i+1; j <= high; j++) 
      if (array[i] > array[j]) {
        swap(array, i, j);
      }

  // print 
  int t_num = omp_get_thread_num();
  printf("Thread %d sorted [%d, %d]\n", t_num, low, high);
  stats[t_num]++;
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
 
  // print and collect stats
  stats[omp_get_thread_num()]++;
  return middle;
}
 
// QuickSort an array range
// 
void quicksort(int *array, int low, int high) {
  if (high - low < MINSIZE) {
    bubblesort(array, low, high);
    return;
  }
  int middle = partition(array, low, high);
  printf("Thread %d found middle [%d]\n", omp_get_thread_num(), middle);
  #pragma omp task
  if (low < middle) {
    quicksort(array, low, middle-1);
  }
  #pragma omp task
  if (middle < high) {
    quicksort(array, middle+1, high);
  }
}
 
// Main routine for testing quicksort
// 
int main(int argc, char **argv) {
  int *array, N, num_t = 1;
  
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

  // initialize the stats array
  stats = malloc(sizeof(int) * num_t); 
  for (int i = 0; i < num_t; ++i)
    stats[i] = 0;

  array = init_array(N);
  
  printf("Sorting started ...|t|=%d\n", num_t);
#pragma omp parallel shared(array)
{
#pragma omp single
  quicksort(array, 0, N-1);
}
printf("... completed.\n\n");
  
  // print array to see where the sorting has gone bad
  for (int i = 0; i < N; ++i) {
    printf("%2d, ", array[i]);
    if (i%10==9) printf("\n");
  }
  printf("\n");

  verify_array(array, N);

  // print thread stats
  printf("\n");
  for (int i = 0; i < num_t; ++i) {
    printf("[%2d]: %d \t", i, stats[i]);
    if (i%5==4) printf("\n");
  }
  printf("\n");
  free(stats);
}
