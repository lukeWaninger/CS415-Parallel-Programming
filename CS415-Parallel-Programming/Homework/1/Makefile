#----------------------------------------------------------------------
# Makefile for CS415/515 Assignment 1
#----------------------------------------------------------------------

FLAGS = -std=c99 -pthread -g
CC = gcc

%: task-queue.h task-queue.c %.c 
	$(CC) $(FLAGS) -o $@ task-queue.c $@.c

all: prodcons-pthd qsort-pthd

clean:
	rm prodcons-pthd qsort-pthd
