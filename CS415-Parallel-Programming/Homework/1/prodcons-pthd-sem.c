//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University
//------------------------------------------------------------------------- 

// Producer Consumer Program using Pthreads
// Luke Waninger
//
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include "task-queue.h"

// function declarations
void consumer(void*);
void producer(void);

// default values
int numConsumers = 1;   // default number of consumers
int queue_cap    = 20;  // maximum number of items in the queue
int num_tasks    = 100; // number of tasks to complete
queue_t *queue;         // the queue
sem_t sem_task_count;   // semaphore to count waiting tasks
sem_t sem_free_count;   // semaphore to count free tasks in buffer
pthread_mutex_t mutex;  // lock for task load/unload

int main(int argc, char **argv) {
  if (argc > 1) {
    if ((numConsumers = atoi(argv[1])) < 1) {
      printf("<numCons> must be greater than 0\n");      
    }
  }
 
  // initialize the producer consumer queue
  queue = init_queue(queue_cap);

  // initialize the semaphores
  // exit with an error code if the semaphore fails to init
  if (sem_init(&sem_task_count, 0, 0) != 0 ||
      sem_init(&sem_free_count, 0, queue_cap) != 0)
    exit(EAGAIN);
  
  // initialize the lock
  pthread_mutex_init(&mutex, NULL);

  // create consumer threads
  pthread_t *threads = 
    (pthread_t *) malloc(sizeof(pthread_t) * numConsumers);
  for (long k = 0; k < numConsumers; k++)
    pthread_create(&threads[k], NULL, (void*)consumer, (void*)k);

  // execute the producer code leaving time for the consumers to start
  // sleep(1);
  producer();

  // wait for consumer threads to terminate
  for (long k = 0; k < numConsumers; k++)
    pthread_join(threads[k], NULL);
  
  exit(0);
}

void producer(void) {
  printf("Producer starting on core %d\n", sched_getcpu());

  int t_num = 1;
  while(t_num <= num_tasks) {
    task_t *t = create_task(t_num, t_num);   // create task
    sem_wait(&sem_free_count);               // wait and decrease sem
    pthread_mutex_lock(&mutex);              // lock the queue
    add_task(queue, t);                      // add the task
    pthread_mutex_unlock(&mutex);            // unlock the queue
    sem_post(&sem_task_count);               // increase sem count
    ++t_num;
  }
  
  // lock, send termination tasks, unlock
  for (int i = 0; i < numConsumers; i++) {
    task_t *terminate = create_task(-1, -1); // create task
    sem_wait(&sem_free_count);               // wait for an opening
    pthread_mutex_lock(&mutex);              // lock the queue
    add_task(queue, terminate);              // add to queue
    pthread_mutex_unlock(&mutex);            // unlock the queue
    sem_post(&sem_task_count);               // increase sem count
  }
}

void consumer(void *p) {
  long k = (long)&p;
  printf("Consumer[%ld] starting on core %d\n", k, sched_getcpu());

  int t_consumed = 0;
  while (1) {
    sem_wait(&sem_task_count);               // wait for a task
    pthread_mutex_lock(&mutex);              // lock the queue
    task_t *task = remove_task(queue);       // take a task
    pthread_mutex_unlock(&mutex);            // unlock
    sem_post(&sem_free_count);               // increase the free sem
 
    // check for termination task
    if (task->low == -1 && task->high == -1) {
      printf("C[%ld]: %d\n", k, t_consumed);
      return;
    }

    ++t_consumed;
  }
}
