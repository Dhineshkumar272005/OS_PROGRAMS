#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2
#define ITERATIONS 1000000

int shared_variable = 0; // Shared variable

pthread_mutex_t mutex; // Mutex for synchronization

void* increment_without_sync(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        shared_variable++; // Increment shared variable without synchronization
    }
    return NULL;
}

void* increment_with_sync(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex); // Lock the mutex
        shared_variable++; // Increment shared variable with synchronization
        pthread_mutex_unlock(&mutex); // Unlock the mutex
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // 1. Without Synchronization
    shared_variable = 0;
    printf("Without Synchronization:\n");

    // Create threads that modify the shared variable without synchronization
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_without_sync, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the value of the shared variable
    printf("Final value of shared variable (without sync): %d\n", shared_variable);

    // 2. With Synchronization
    shared_variable = 0;
    pthread_mutex_init(&mutex, NULL); // Initialize the mutex
    printf("\nWith Synchronization:\n");

    // Create threads that modify the shared variable with synchronization
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_with_sync, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the value of the shared variable
    printf("Final value of shared variable (with sync): %d\n", shared_variable);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
