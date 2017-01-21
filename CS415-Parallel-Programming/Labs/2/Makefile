# Makefile for CS 415P/515 Lab 1 (J. Li)
#
condvar:
	gcc -pthread -g -o condvar condvar-pthd.c

race-nolock:
	gcc -pthread -g -o race-nolock arraysum-pthd.c

race:
	gcc -pthread -DLOCK -g -o race arraysum-pthd.c

mtx:
	gcc -pthread -g -o mtx mtxmul.c

mtx-p:
	gcc -pthread -g -o mtx-p mtxmul-pthd.c

clean:
	rm condvar race race-nolock mtx
