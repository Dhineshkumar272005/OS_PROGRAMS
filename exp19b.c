#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2
#define READ_COUNT 5
#define WRITE_COUNT 3

// Shared resource and synchronization primitives
int data = 0;
int read_count = 0;
pthread_mutex_t mutex;
pthread_mutex_t read_mutex;
pthread_cond_t write_cond;

// Reader function
void* reader(void* arg) {
    for (int i = 0; i < READ_COUNT; i++) {
        pthread_mutex_lock(&read_mutex);
        read_count++;
        if (read_count == 1) {
            pthread_mutex_lock(&mutex); // Lock the resource for reading
        }
        pthread_mutex_unlock(&read_mutex);

        printf("Reader %ld reading data: %d\n", (long)arg, data);

        pthread_mutex_lock(&read_mutex);
        read_count--;
        if (read_count == 0) {
            pthread_mutex_unlock(&mutex); // Unlock the resource if no readers
        }
        pthread_mutex_unlock(&read_mutex);

        sleep(rand() % 2); // Simulate variable read time
    }
    return NULL; // Corrected return statement
}

// Writer function
void* writer(void* arg) {
    for (int i = 0; i < WRITE_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        data = rand() % 100; // Write data
        printf("Writer %ld writing data: %d\n", (long)arg, data);
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&write_cond);
        sleep(rand() % 2); // Simulate variable write time
    }
    return NULL; // Corrected return statement
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&read_mutex, NULL);
    pthread_cond_init(&write_cond, NULL);

    // Create reader and writer threads
    for (long i = 0; i < NUM_READERS; i++) {
        pthread_create(&readers[i], NULL, reader, (void*)i);
    }
    for (long i = 0; i < NUM_WRITERS; i++) {
        pthread_create(&writers[i], NULL, writer, (void*)i);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&read_mutex);
    pthread_cond_destroy(&write_cond);
    return 0; // Corrected return statement
}
