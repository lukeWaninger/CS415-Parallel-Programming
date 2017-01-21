//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// Matrix multiplication algorithm.  
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#define N 16
int idx = 0;
int numThreads = 4;
int a[N][N], b[N][N], c[N][N];
struct v {
  int i;
  int j;
};

void init_array()  {
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i][j] = i + j;
      b[i][j] = N - j;
    }
  }
}

void print_array()  {
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++)
      printf("%4d ", c[i][j]);
    printf("\n");
  }
}

void slave(void* param) {
  struct v* data = param;
  int n, sum = 0;

  for (n = 0; n < N; ++n) {
    sum += a[data->i][n] * b[n][data->j]; 
  }

  c[data->i][data->j] = sum;
  pthread_exit(0);
}

int main(int argc, char **argv) {
  if (argc > 2) {
    if ((numThreads = atoi(argv[2])) < 1) {
    	printf("<numThreads> must be greater than 0");
    }
  }
  init_array();

  pthread_t thread[numThreads];
  int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
  cpu_set_t cpuset;
  
  if (numThreads > N) numThreads = N;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N;) {
      struct v* param = (struct v*)malloc(sizeof(struct v));
      param->i = i;
      
      // create threads and pass parameters
      for (long k = 0; k < numThreads; k++) { 
	param->j = j;
	pthread_attr_t attr;       // set of thread attributes
	pthread_attr_init(&attr);  // give them defaults
        pthread_create(&thread[k], &attr, (void*)slave, (void*)param);
	pthread_join(thread[k], NULL);
	j++;
      }
      free(param);
    }
  }
  
  print_array();
}
