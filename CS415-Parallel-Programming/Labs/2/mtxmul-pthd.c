//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// Matrix multiplication algorithm.  
//
#define _GNU_SOURCE
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
  long tid;
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
  printf("Thread %ld started on %d\n", data->tid, sched_getcpu()); 
   
  int n, sum = 0; // is this sum in shared memory space
  for (n = 0; n < N; ++n) {
    sum += a[data->i][n] * b[n][data->j]; 
  }

  c[data->i][data->j] = sum;
  pthread_exit(0);
}

int main(int argc, char **argv) {
  if (argc > 1) {
    if ((numThreads = atoi(argv[1])) < 1) {
      printf("<numThreads> must be greater than 0");
    }
    printf("Threads to start: %d\n", numThreads);
  }
  init_array(N);

  if (numThreads > N) numThreads = N;
  pthread_t thread[numThreads];
  int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
  cpu_set_t cpuset;
  int cid = 0;

  int k = numThreads - 1;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      struct v* param = (struct v*)malloc(sizeof(struct v));
      param->tid = k;
      param->i   = i;
      param->j   = j;

      // setup a new thread
      pthread_create(&thread[k], NULL, (void*)slave, (void*)param);
      CPU_ZERO(&cpuset);
      CPU_SET(cid++ % nprocs, &cpuset);
      pthread_setaffinity_np(thread[k], sizeof(cpu_set_t), &cpuset);
      
      k--;
      if (k == -1) {
	  for (k = numThreads - 1; k >= 0; k--) {
	  pthread_join(thread[k], NULL);
	}
	k = numThreads - 1;
      }
    }
  }
  print_array();
}
