//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// An OMP lock ownership demo.
//
#include <stdio.h>
#include <omp.h>

int main() {
  omp_lock_t lck;
  omp_init_lock(&lck);
  #pragma omp parallel num_threads(2)
  {
    if ((omp_get_thread_num()) == 0) {
      omp_set_lock(&lck);
      printf("Lock set by thread 0\n");
    } else {
      omp_unset_lock(&lck);  // !!!
      omp_set_lock(&lck);
      printf("Lock re-set by thread 1\n");
    }
  }
}
