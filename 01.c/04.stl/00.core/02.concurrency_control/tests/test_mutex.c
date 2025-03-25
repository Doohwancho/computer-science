#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "my_mutex.h"

#define NUM_THREADS 4
#define NUM_OPERATIONS 100

static KirbyMutex mutex;
static int shared_counter = 0;

static void* increment_counter(void* arg) {
    int thread_id = *(int*)arg;
    int local_count = 0;
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        mutex_lock(&mutex);
        shared_counter++;
        local_count++;
        mutex_unlock(&mutex);
    }
    
    printf("Thread %d completed %d operations\n", thread_id, local_count);
    return NULL;
}

static void test_basic_operations(void) {
    printf("Testing basic mutex operations...\n");
    
    KirbyMutex test_mutex;
    mutex_init(&test_mutex);
    
    assert(mutex_lock(&test_mutex));
    assert(!mutex_unlock(&test_mutex));  // Should fail as different thread
    assert(mutex_unlock(&test_mutex));
    
    printf("Basic operations test passed\n\n");
}

static void test_recursive_locking(void) {
    printf("Testing recursive locking...\n");
    
    KirbyMutex test_mutex;
    mutex_init(&test_mutex);
    
    assert(mutex_lock(&test_mutex));
    assert(mutex_lock(&test_mutex));  // Should succeed (recursive)
    assert(test_mutex.count == 2);
    
    assert(mutex_unlock(&test_mutex));
    assert(test_mutex.count == 1);
    assert(mutex_unlock(&test_mutex));
    assert(test_mutex.count == 0);
    
    printf("Recursive locking test passed\n\n");
}

static void test_concurrent_operations(void) {
    printf("Testing concurrent operations...\n");
    
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    shared_counter = 0;
    
    mutex_init(&mutex);
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, increment_counter, &thread_ids[i]);
    }
    
    // Wait for all threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Verify result
    printf("Final counter value: %d\n", shared_counter);
    assert(shared_counter == NUM_THREADS * NUM_OPERATIONS);
    
    printf("Concurrent operations test passed\n\n");
}

static void* try_lock_test(void* arg) {
    KirbyMutex* test_mutex = (KirbyMutex*)arg;
    mutex_lock(test_mutex);
    usleep(100000);  // Hold lock for 100ms
    mutex_unlock(test_mutex);
    return NULL;
}

static void test_contention(void) {
    printf("Testing lock contention...\n");
    
    KirbyMutex test_mutex;
    mutex_init(&test_mutex);
    
    pthread_t thread;
    pthread_create(&thread, NULL, try_lock_test, &test_mutex);
    
    // Give the other thread time to acquire the lock
    usleep(10000);
    
    // Try to lock - should block
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    mutex_lock(&test_mutex);
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long wait_time = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
    printf("Lock wait time: %ld ns\n", wait_time);
    
    mutex_unlock(&test_mutex);
    pthread_join(thread, NULL);
    
    assert(wait_time >= 80000000);  // Should have waited at least 80ms
    
    printf("Contention test passed\n\n");
}

int main(void) {
    printf("Starting mutex tests...\n\n");
    
    test_basic_operations();
    test_recursive_locking();
    test_concurrent_operations();
    test_contention();
    
    printf("All mutex tests passed!\n");
    return 0;
}
