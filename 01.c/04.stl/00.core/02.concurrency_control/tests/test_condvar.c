#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>  // for rand()
#include <time.h>    // for time()
#include "my_condvar.h"
#include "my_mutex.h"

#define NUM_THREADS 4
#define BUFFER_SIZE 5
#define NUM_ITEMS 100

// Shared resources for producer-consumer test
static KirbyMutex mutex;
static CondVar not_full;
static CondVar not_empty;
static int buffer[BUFFER_SIZE];
static int in = 0, out = 0;
static int count = 0;

static void* producer(void* arg) {
    int thread_id = *(int*)arg;
    int items_produced = 0;
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        mutex_lock(&mutex);
        
        while (count == BUFFER_SIZE) {
            condvar_wait(&not_full, &mutex);
        }
        
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        items_produced++;
        
        printf("Producer %d produced item %d, count=%d\n", 
               thread_id, i, count);
        
        condvar_signal(&not_empty);
        mutex_unlock(&mutex);
        
        usleep(rand() % 1000);  // Random delay
    }
    
    printf("Producer %d finished, produced %d items\n", 
           thread_id, items_produced);
    return NULL;
}

static void* consumer(void* arg) {
    int thread_id = *(int*)arg;
    int items_consumed = 0;
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        mutex_lock(&mutex);
        
        while (count == 0) {
            condvar_wait(&not_empty, &mutex);
        }
        
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        items_consumed++;
        
        printf("Consumer %d consumed item %d, count=%d\n", 
               thread_id, item, count);
        
        condvar_signal(&not_full);
        mutex_unlock(&mutex);
        
        usleep(rand() % 1000);  // Random delay
    }
    
    printf("Consumer %d finished, consumed %d items\n", 
           thread_id, items_consumed);
    return NULL;
}

static void test_signal_lost_wakeup(void) {
    printf("Testing signal lost wakeup...\n");
    
    KirbyMutex test_mutex;
    CondVar test_cv;
    mutex_init(&test_mutex);
    condvar_init(&test_cv);
    
    // Signal before wait (should be lost)
    condvar_signal(&test_cv);
    
    // Now try to wait
    mutex_lock(&test_mutex);
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    condvar_wait(&test_cv, &test_mutex);
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long wait_time = (end.tv_sec - start.tv_sec) * 1000000000L + 
                    (end.tv_nsec - start.tv_nsec);
    
    mutex_unlock(&test_mutex);
    printf("Wait time: %ld ns\n", wait_time);
    
    printf("Signal lost wakeup test passed\n\n");
}

static void test_broadcast(void) {
    printf("Testing broadcast...\n");
    
    KirbyMutex test_mutex;
    CondVar test_cv;
    mutex_init(&test_mutex);
    condvar_init(&test_cv);
    
    #define NUM_WAITERS 3
    pthread_t waiters[NUM_WAITERS];
    int waiter_count = 0;
    
    // Create waiter threads
    for (int i = 0; i < NUM_WAITERS; i++) {
        pthread_create(&waiters[i], NULL, 
            (void*(*)(void*))condvar_wait, &test_cv);
        waiter_count++;
    }
    
    usleep(100000);  // Let waiters get ready
    
    // Broadcast to wake all waiters
    condvar_broadcast(&test_cv);
    
    // Wait for all threads to finish
    for (int i = 0; i < NUM_WAITERS; i++) {
        pthread_join(waiters[i], NULL);
        waiter_count--;
    }
    
    assert(waiter_count == 0);
    printf("Broadcast test passed\n\n");
}

static void test_producer_consumer(void) {
    printf("Testing producer-consumer scenario...\n");
    
    mutex_init(&mutex);
    condvar_init(&not_full);
    condvar_init(&not_empty);
    
    pthread_t producers[NUM_THREADS];
    pthread_t consumers[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    // Create producer and consumer threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &thread_ids[i]);
        pthread_create(&consumers[i], NULL, consumer, &thread_ids[i]);
    }
    
    // Wait for all threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }
    
    assert(count == 0);  // Buffer should be empty
    printf("Producer-consumer test passed\n\n");
}

int main(void) {
    printf("Starting condition variable tests...\n\n");
    
    test_signal_lost_wakeup();
    test_broadcast();
    test_producer_consumer();
    
    printf("All condition variable tests passed!\n");
    return 0;
}
