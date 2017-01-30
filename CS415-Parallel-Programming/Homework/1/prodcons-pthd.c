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
#include "task-queue.h"

// function declarations
void consumer(void*);
void producer(void);

// default values
int numConsumers = 1;    // default number of consumers
int queue_cap    = 20;   // maximum number of items in the queue
int num_tasks    = 100;  // number of tasks to complete
queue_t *queue;          // the queue
pthread_mutex_t buffer;  // lock for task load/unload
pthread_cond_t cond_task_available;
pthread_cond_t cond_free_available;

struct count {
  pthread_mutex_t mutex;
  int num;
};
struct count free_count;
struct count task_count;

static void p_add_task(task_t*);
static void c_init(struct count*, int);
static void c_up(struct count*);
static void c_down(struct count*);

int main(int argc, char **argv) {
  if (argc > 1) {
    if ((numConsumers = atoi(argv[1])) < 1) {
      printf("<numCons> must be greater than 0\n");      
    }
  }
 
  // initialize the producer consumer queue
  queue = init_queue(queue_cap);

  // initialize the locks
  pthread_mutex_init(&buffer, NULL);
 
  // initialize the task count
  c_init(&free_count, queue_cap);
  c_init(&task_count, 0);

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
    p_add_task(t);
    ++t_num;
  }
  
  // lock, send termination tasks, unlock
  for (int i = 0; i < numConsumers; i++) {
    task_t *terminate = create_task(-1, -1); // create task
    p_add_task(terminate);
  }
}

static void p_add_task(task_t *t) {
    // check for free space in the buffer
    pthread_mutex_lock(&free_count.mutex);
    while(free_count.num == 0)
      pthread_cond_wait(&cond_free_available, &free_count.mutex);
    pthread_mutex_unlock(&free_count.mutex);

    pthread_mutex_lock(&buffer);
    add_task(queue, t); 
    c_down(&free_count);
    pthread_mutex_unlock(&buffer);
    
    // increase task count 
    c_up(&task_count);
    pthread_cond_signal(&cond_task_available);
}

void consumer(void *p) {
  long k = (long)&p;
  printf("Consumer[%ld] starting on core %d\n", k, sched_getcpu());

  int t_consumed = 0;
  while (1) {
    // check for available task
    pthread_mutex_lock(&task_count.mutex);
    while (task_count.num == 0) {
      pthread_cond_wait(&cond_task_available, &task_count.mutex);
    }
    pthread_mutex_unlock(&task_count.mutex);

    pthread_mutex_lock(&buffer);              // lock the queue
    task_t *task = remove_task(queue);        // take a task
    c_down(&task_count);
    pthread_mutex_unlock(&buffer);            // unlock

    // increase the free count
    c_up(&free_count);
    pthread_cond_signal(&cond_free_available);

    // check for termination task
    if (task->low == -1 && task->high == -1) {
      printf("C[%ld]: %d\n", k, t_consumed);
      return;
    }

    ++t_consumed;
  }
}

static void c_init(struct count* c, int init_num) {
  pthread_mutex_init(&c->mutex, NULL);
  c->num = init_num;
}

static void c_down(struct count *c) {
  pthread_mutex_lock(&c->mutex);
  c->num--;
  pthread_mutex_unlock(&c->mutex);
}

static void c_up(struct count *c) {
  pthread_mutex_lock(&c->mutex);
  c->num++;
  pthread_mutex_unlock(&c->mutex);
}
