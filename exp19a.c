#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define PRODUCE_COUNT 5

// Shared buffer
int buffer[BUFFER_SIZE];
int count = 0; // Number of items in the buffer
int in = 0;    // Index for the producer
int out = 0;   // Index for the consumer

// Mutex and condition variables
pthread_mutex_t mutex;
pthread_cond_t not_full;
pthread_cond_t not_empty;

// Producer function
void* producer(void* arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &mutex);
        }

        buffer[in] = i; // Produce an item
        printf("Producer %ld produced %d\n", (long)arg, buffer[in]);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 2); // Simulate variable production time
    }
    return NULL; // Corrected return statement
}

// Consumer function
void* consumer(void* arg) {
    for (int i = 0; i < PRODUCE_COUNT; i++) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = buffer[out]; // Consume an item
        printf("Consumer %ld consumed %d\n", (long)arg, item);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 2); // Simulate variable consumption time
    }
    return NULL; // Corrected return statement
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    // Create producer and consumer threads
    for (long i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producers[i], NULL, producer, (void*)i);
    }
    for (long i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumers[i], NULL, consumer, (void*)i);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
    return 0; // Corrected return statement
}
