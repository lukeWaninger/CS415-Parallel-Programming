// The sum program (OpenMP version).
//
//
#include <stdio.h>
#include <omp.h>

#define N 1000 	/* problem domain size */

int sum = 0;

int compute(int i) {
  return i*i;
}

int main(int argc, char **argv) {
  int i; 
#ifdef DEBUG
  int unique_ids[50] = {0};
  for (int i = 0; i < 50; ++i)
	  unique_ids[0] = 0;
#endif

#pragma omp parallel for reduction(+:sum)
  for (i = 0; i < N; i++) {
    sum += compute(i);
#ifdef DEBUG
    printf("thread id: %d\n", omp_get_thread_num());
    unique_ids[omp_get_thread_num()] = 1;
#endif
  }
  printf("The result sum is %d\n", sum);
#ifdef DEBUG
  int thread_count = 0;
  printf("threads used: ");
  for (int i = 0; i < 50; ++i) {
	if (unique_ids[i] == 1) {
		printf("%d, ", i);
		thread_count++;
	}
  }
  printf("\nthread count: %d\n", sum);
#endif
}
