#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t unnamed_sem;
int shard_num = 0;

void *plusOne(void *argv) {
    sem_wait(&unnamed_sem);
    int tmp = shard_num + 1;
    shard_num = tmp;
    sem_post(&unnamed_sem);
}

int main() {
    sem_init(&unnamed_sem, 0, 1);

    pthread_t tid[10000];
    for (int i = 0; i < 10000; i++) {
        pthread_create(tid + i, NULL, plusOne, NULL);
    }

    for (int i = 0; i < 10000; i++) {
        pthread_join(tid[i], NULL);
    }
    
    printf("shard_num is %d\n", shard_num);

    sem_destroy(&unnamed_sem);

    return 0;
}
