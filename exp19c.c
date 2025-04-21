#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;

void* critical_section(void* arg) {
    sem_wait(&semaphore); // P operation (wait)
    printf("Thread %ld entered critical section\n", (long)arg);
    
    // Critical section code
    // Simulate some work in the critical section
    sleep(1); // Optional: simulate work being done in the critical section

    printf("Thread %ld exiting critical section\n", (long)arg);
    sem_post(&semaphore); // V operation (signal)
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    // Initialize semaphore with value 1 (binary semaphore)
    sem_init(&semaphore, 0, 1);

    // Create threads
    pthread_create(&t1, NULL, critical_section, (void*)1);
    pthread_create(&t2, NULL, critical_section, (void*)2);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destroy semaphore
    sem_destroy(&semaphore);
    return 0;
}
