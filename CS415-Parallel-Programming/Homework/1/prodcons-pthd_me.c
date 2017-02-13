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

// default values
int numConsumers = 1;               // default number of consumers
int queue_cap    = 20;              // the maximum task buffer size
int num_tasks    = 100;             // number of tasks to complete
queue_t *queue;                     // the queue
pthread_mutex_t buffer;             // lock the queue (task buffer)
pthread_cond_t cond_task_available; // for signalling a task is available
pthread_cond_t cond_free_available; // for signalling a position is open

// a counting variable with a lock to maintain synchronization
struct count {
  pthread_mutex_t mutex;
  int num;
};
struct count free_count;           // keep track of the free count
struct count task_count;           // keep track of the task count

// function declarations
void consumer(void*);
void producer(void);
static void p_add_task(task_t*);         // just for reuse
static void c_init(struct count*, int);  // to initialize the counting struct
static void c_up(struct count*);         // to increase a count
static void c_down(struct count*);       // to decrease a count

int main(int argc, char **argv) {
  // check for input and change default consumer count if necessary
  if (argc > 1) {
    if ((numConsumers = atoi(argv[1])) < 1) {
      printf("<numCons> must be greater than 0\n");      
    }
  }
 
  // initialize the producer consumer queue
  queue = init_queue(queue_cap);

  // initialize the locks
  pthread_mutex_init(&buffer, NULL);
 
  // initialize the free and task counts
  c_init(&free_count, queue_cap);
  c_init(&task_count, 0);

  // create consumer threads
  pthread_t *threads = 
    (pthread_t *) malloc(sizeof(pthread_t) * numConsumers);
  for (long k = 0; k < numConsumers; k++)
    pthread_create(&threads[k], NULL, (void*)consumer, (void*)k);

  // execute the producer code leaving time for the consumers to start
  producer();

  // wait for consumer threads to terminate
  for (long k = 0; k < numConsumers; k++)
    pthread_join(threads[k], NULL);
  
  exit(0);
}

void 
producer(void) {
  printf("Producer starting on core %d\n", sched_getcpu());
  int t_num = 1;
  
  // keep adding tasks until the limit is reached
  while(t_num <= num_tasks) {
    task_t *t = create_task(t_num, t_num); 
    p_add_task(t);                             
    ++t_num;                               
  }
  
  // lock, send termination tasks, unlock
  for (int i = 0; i < numConsumers; i++) {
    task_t *terminate = create_task(-1, -1);
    p_add_task(terminate);
  }
}

static void
p_add_task(task_t *t) {
    // wait for a free space in the buffer
    pthread_mutex_lock(&free_count.mutex);
    while(free_count.num == 0)
      pthread_cond_wait(&cond_free_available, &free_count.mutex);
    pthread_mutex_unlock(&free_count.mutex);

    // lock the buffer, add the task, decrease the free count
    pthread_mutex_lock(&buffer);
    add_task(queue, t); 
    c_down(&free_count);
    pthread_mutex_unlock(&buffer);
    
    // increase task count and signal its available
    c_up(&task_count);
    pthread_cond_signal(&cond_task_available);
}

void 
consumer(void *p) {
  long k = (long)&p;
  printf("Consumer[%ld] starting on core %d\n", k, sched_getcpu());

  // infinite loop to keep consuming tasks
  int t_consumed = 0;
  while (1) {
    // wait for an available task
    pthread_mutex_lock(&task_count.mutex);
    while (task_count.num == 0) {
      pthread_cond_wait(&cond_task_available, &task_count.mutex);
    }
    pthread_mutex_unlock(&task_count.mutex);

    // lock the buffer, take a task, decrease the task count
    pthread_mutex_lock(&buffer);              
    task_t *task = remove_task(queue);        
    c_down(&task_count);
    pthread_mutex_unlock(&buffer);            

    // increase the free count and signal a free position is available
    c_up(&free_count);
    pthread_cond_signal(&cond_free_available);

    // check for termination task, print number of tasks, terminate
    if (!task || (task->low == -1 && task->high == -1)) {
      printf("C[%ld]: %d\n", k, t_consumed);
      return;
    }

    ++t_consumed;
  }
}

static void 
c_init(struct count* c, int init_num) {
  pthread_mutex_init(&c->mutex, NULL);
  c->num = init_num;
}

static void 
c_down(struct count *c) {
  pthread_mutex_lock(&c->mutex);
  c->num--;
  pthread_mutex_unlock(&c->mutex);
}

static void 
c_up(struct count *c) {
  pthread_mutex_lock(&c->mutex);
  c->num++;
  pthread_mutex_unlock(&c->mutex);
}
