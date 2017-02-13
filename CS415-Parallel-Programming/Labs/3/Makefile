#----------------------------------------------------------------------
# Makefile for CS415/515 OpenMP Lab
#----------------------------------------------------------------------

FLAGS = -std=c99 -fopenmp -g
CC = gcc

loop:
	$(CC) $(FLAGS) -o loop loop.c
	$(CC) $(FLAGS) -o loop-omp1 loop-omp1.c
	$(CC) $(FLAGS) -o loop-omp2 loop-omp2.c

rec:
	$(CC) $(FLAGS) -o rec rec-omp1.c
 
%: %.c
	$(CC) -o $@ $< $(FLAGS)

clean:
	rm rec rec-omp1 rec-omp2 loop hello-omp loop loop-omp1 loop-omp2
