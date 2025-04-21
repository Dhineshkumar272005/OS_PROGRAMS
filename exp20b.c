#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5

sem_t semaphore;

void* thread_function(void* arg) {
    int thread_num = *((int*)arg);
    sem_wait(&semaphore); // Wait (P operation)
    printf("Thread %d is in the critical section\n", thread_num);
    sleep(1); // Simulate some work
    printf("Thread %d is leaving the critical section\n", thread_num);
    sem_post(&semaphore); // Signal (V operation)
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_nums[NUM_THREADS];

    // Initialize the semaphore with a value of 2
    sem_init(&semaphore, 0, 2);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_nums[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_nums[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the semaphore
    sem_destroy(&semaphore);
    return 0;
}
