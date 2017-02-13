#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void g() { printf("G\n"); }

void f(int *ip) {
  pthread_t t3;
  pthread_create(&t3, NULL, (void*)g, NULL);
  printf("f %d\n", *ip);
  pthread_join(t3, NULL);
}

void sleep(unsigned long nsec) {
  int d = 0;
  while (d++ < nsec);
}

int main() {
  while (1) {
    int i = 1, j = 2;
    pthread_t t1, t2;
    pthread_create(&t1, NULL, (void*)f, &i);
    pthread_create(&t2, NULL, (void*)f, &j);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Work done\n");
    sleep(90000000);
  }
}
