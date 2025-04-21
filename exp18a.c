#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>

typedef struct {
    atomic_int lock; // Atomic variable to represent the lock
} Spinlock;

// Initialize the spinlock
void spinlock_init(Spinlock *spinlock) {
    atomic_store(&spinlock->lock, 0); // Set lock to 0 (unlocked)
}

// Acquire the spinlock
void spinlock_lock(Spinlock *spinlock) {
    // Busy wait (spin) until the lock becomes available
    while (atomic_exchange(&spinlock->lock, 1) != 0) {
        // Spin until the lock is released
        while (atomic_load(&spinlock->lock) != 0);
    }
}

// Release the spinlock
void spinlock_unlock(Spinlock *spinlock) {
    atomic_store(&spinlock->lock, 0); // Set lock to 0 (unlocked)
}

// Shared resource
int shared_resource = 0;
Spinlock spinlock;

// Thread function to increment the shared resource
void *increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        spinlock_lock(&spinlock); // Acquire the lock
        shared_resource++; // Critical section
        spinlock_unlock(&spinlock); // Release the lock
    }
    return NULL;
}

int main() {
    pthread_t threads[4];

    // Initialize the spinlock
    spinlock_init(&spinlock);

    // Create multiple threads to increment the shared resource
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the final value of the shared resource
    printf("Final value of shared resource: %d\n", shared_resource);
    return 0;
}
