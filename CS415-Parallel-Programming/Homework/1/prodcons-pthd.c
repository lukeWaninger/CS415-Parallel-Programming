#include "task-queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex;
pthread_cond_t cond_produce, cond_consume;

int total_tasks = 100;
int last_task = 0;
int task_count=1; 
const int max_queue = 20;
const int Q_count =1;
queue_t* task_Q;

void producer()
{

	while(total_tasks != 0)		//continue to produce tasks until 100 habe 
	{
		printf("thread number %d \n",task_count);
		
		pthread_mutex_lock(&mutex);

		if(task_Q -> length == max_queue)
			pthread_cond_wait(&cond_consume,&mutex);
		
		task_t * new_task = create_task(task_count, task_count);
		
		add_task(task_Q,new_task);
		pthread_mutex_unlock(&mutex);
	
		pthread_cond_signal(&cond_consume);
		task_count++;		//increment number of created tasks
		total_tasks--;		//decrement total task by one with each iteration 
	}


}

void consumer()
{

	while(1)
	{
	
		pthread_mutex_lock(&mutex);

		if(task_Q->length == 0)
			pthread_cond_wait(&cond_produce,&mutex);
		
		task_t * finished =remove_task(task_Q);	
		pthread_mutex_unlock(&mutex);
		
		printf("thread %d consumed\n",finished->high);
		pthread_cond_signal(&cond_produce);
		
		if(finished -> high  == 100){					//if all tasks have been produced and consumer
			printf("last task reached\n");
			last_task = 1;
			return;
		}
	
		if(last_task)
			return;
	}
	
}

int main(int argc, char * argv[])
{
	
	long numConsumers =0;
	
	if(argc < 2)		//if no argument provided 
	{
		printf("no consumer thread number provided, default to 1 consumer thread\n");
		numConsumers = 1; 
	}

	else if(argc > 2)	//cannot have more than one argument
	{
		printf("Must not have more than two arguments, default to 1 consumer thread\n");
		numConsumers = 1; 
	}

	else			//case of negate values or 0
	{
		
	 	int val =  atol(argv[1]);

		if(val <=0)
		{
			printf("The number of consumers must be greater than 1, default to 1 consumer thread\n");
			numConsumers =1;
		}
		
		else
	       		numConsumers = val;
	}
	
	printf("The number of consumer threads is %ld\n",numConsumers);
	long i;


	task_Q = init_queue(max_queue);						//create task queue passing it max number of tasks (20)
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond_consume,NULL);
	pthread_cond_init(&cond_produce,NULL);

	pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t)* numConsumers);

	for(i =0; i < numConsumers; ++i)
	{
		pthread_create(&threads[i],NULL,(void *)consumer, (void*)i);
	}

	producer();
	
	for(i = 0; i < numConsumers; ++i);
		pthread_join(threads[i],NULL);


	return 0;
}