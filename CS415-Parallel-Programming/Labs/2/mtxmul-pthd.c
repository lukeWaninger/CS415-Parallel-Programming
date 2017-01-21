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
  init_array();

  pthread_t thread[N];
  int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
  cpu_set_t cpuset;
  int cid, k = 0;

  // create the threads
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
		struct v* param = (struct v*)malloc(sizeof(struct v));
		param->i = i;
		param->j = j;
    
		// strat a thread
		pthread_t tid;
		pthread_attr_init(&attr); // set thread attributes
		pthread_create(&tid, &attr, (void*)slave, (void*)param); // create the thread
		pthread_join(tid, NULL); // wait for the thread to finish
    }
  }

  print_array();
}
