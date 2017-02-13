typedef struct {
  pthread_mutex_t lock;
  pthread_cond_t cond;
  int count;
} sem_t;

void sem_init(sem_t *sem) {
  pthread_mutex_init(&sem->lock, NULL);
  pthread_cond_init(&sem->cond, NULL);
  sem->count = N; // the initial semaphore value
}
