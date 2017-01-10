# Makefile for CS 415P/515 Lab 1 (J. Li)
#

all:	sum-pthd sum-omp sum-mpi sum1

sum-pthd:
	gcc -pthread -g -o sum-pthd sum-pthd.c	

sum-omp:
	gcc -fopenmp -g -o sum-omp sum-omp.c

sum-mpi:
	mpicc -g -o sum-mpi sum-mpi.c

sum1:
	chpl -g -o sum1 sum1.chpl

sum2:
	chpl -g -o sum2 sum2.chpl

clean:
	rm sum-pthd sum-omp sum-mpi sum1 sum2
