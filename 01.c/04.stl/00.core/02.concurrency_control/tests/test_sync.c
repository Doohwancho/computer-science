// tests/test_rwlock.c
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "my_rwlock.h"

#define NUM_READERS 4
#define NUM_WRITERS 2
#define NUM_ITERATIONS 1000

typedef struct {
    KirbyRWLock* rwlock;
    int* shared_data;
    int* read_count;
    int* write_count;
    int thread_id;
} ThreadArgs;

static void* reader_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    KirbyRWLock* rwlock = args->rwlock;
    int thread_id = args->thread_id;

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        rwlock_read_lock(rwlock);
        
        // Read operation
        int value = *(args->shared_data);
        // Simulate some work
        usleep(1);  // 1 microsecond
        assert(value >= 0);  // Verify data consistency
        
        // Increment read counter
        __atomic_fetch_add(args->read_count, 1, __ATOMIC_SEQ_CST);
        
        printf("Reader %d read value: %d\n", thread_id, value);
        
        rwlock_read_unlock(rwlock);
        
        // Small delay between operations
        usleep(rand() % 100);
    }
    return NULL;
}

static void* writer_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    KirbyRWLock* rwlock = args->rwlock;
    int thread_id = args->thread_id;

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        rwlock_write_lock(rwlock);
        
        // Write operation
        (*args->shared_data)++;
        // Simulate some work
        usleep(1);  // 1 microsecond
        
        // Increment write counter
        __atomic_fetch_add(args->write_count, 1, __ATOMIC_SEQ_CST);
        
        printf("Writer %d wrote value: %d\n", thread_id, *(args->shared_data));
        
        rwlock_write_unlock(rwlock);
        
        // Small delay between operations
        usleep(rand() % 100);
    }
    return NULL;
}

static void test_basic_operations(void) {
    printf("Testing basic KirbyRWLock operations...\n");
    
    KirbyRWLock rwlock;
    rwlock_init(&rwlock);
    
    // Test single reader
    rwlock_read_lock(&rwlock);
    assert(atomic_load(&rwlock.readers) == 1);
    rwlock_read_unlock(&rwlock);
    assert(atomic_load(&rwlock.readers) == 0);
    
    // Test single writer
    rwlock_write_lock(&rwlock);
    assert(atomic_load(&rwlock.writing));
    rwlock_write_unlock(&rwlock);
    assert(!atomic_load(&rwlock.writing));
    
    rwlock_destroy(&rwlock);
    printf("Basic operations test passed\n\n");
}

static void test_concurrent_access(void) {
    printf("Testing concurrent access...\n");
    
    KirbyRWLock rwlock;
    rwlock_init(&rwlock);
    
    int shared_data = 0;
    int read_count = 0;
    int write_count = 0;
    
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    ThreadArgs reader_args[NUM_READERS];
    ThreadArgs writer_args[NUM_WRITERS];
    
    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_args[i] = (ThreadArgs){
            .rwlock = &rwlock,
            .shared_data = &shared_data,
            .read_count = &read_count,
            .write_count = &write_count,
            .thread_id = i
        };
        pthread_create(&readers[i], NULL, reader_thread, &reader_args[i]);
    }
    
    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_args[i] = (ThreadArgs){
            .rwlock = &rwlock,
            .shared_data = &shared_data,
            .read_count = &read_count,
            .write_count = &write_count,
            .thread_id = i
        };
        pthread_create(&writers[i], NULL, writer_thread, &writer_args[i]);
    }
    
    // Join all threads
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    
    // Verify results
    printf("Final state:\n");
    printf("  Shared data: %d\n", shared_data);
    printf("  Read operations: %d\n", read_count);
    printf("  Write operations: %d\n", write_count);
    
    assert(write_count == NUM_WRITERS * NUM_ITERATIONS);
    assert(read_count == NUM_READERS * NUM_ITERATIONS);
    assert(shared_data == NUM_WRITERS * NUM_ITERATIONS);
    
    rwlock_destroy(&rwlock);
    printf("Concurrent access test passed\n\n");
}

static void* reader_func(void* arg) {
    KirbyRWLock* rwlock = (KirbyRWLock*)arg;
    rwlock_read_lock(rwlock);
    usleep(2000);  // Simulate some work
    rwlock_read_unlock(rwlock);
    return NULL;
}

static void* writer_func(void* arg) {
    KirbyRWLock* rwlock = (KirbyRWLock*)arg;
    rwlock_write_lock(rwlock);
    usleep(2000);  // Simulate some work
    rwlock_write_unlock(rwlock);
    return NULL;
}

static void test_writer_preference(void) {
    printf("Testing writer preference...\n");
    
    KirbyRWLock rwlock;
    rwlock_init(&rwlock);
    // atomic_store(&rwlock.debug, true);  // 디버깅 활성화

    
    // Start with a reader
    pthread_t reader1, reader2, writer;
    
    // First reader gets the lock
    pthread_create(&reader1, NULL, reader_func, &rwlock);
    usleep(100);  // Let reader1 get the lock
    
    // Create a writer - will be waiting
    pthread_create(&writer, NULL, writer_func, &rwlock);
    usleep(100);  // Let writer start waiting
    
    // Try to create another reader - should be blocked because of waiting writer
    pthread_create(&reader2, NULL, reader_func, &rwlock);
    
    // Give threads time to run
    usleep(1000);
    
    // Check state
    bool has_waiting_readers = (atomic_load(&rwlock.waiting_readers) > 0);
    bool has_waiting_writers = (atomic_load(&rwlock.waiting_writers) > 0);

    // printf("State: waiting_writers=%d, waiting_readers=%d, active_readers=%d, writing=%d\n",
    //     atomic_load(&rwlock.waiting_writers),
    //     atomic_load(&rwlock.waiting_readers),
    //     atomic_load(&rwlock.readers),
    //     atomic_load(&rwlock.writing));
    
    // Cleanup
    pthread_join(reader1, NULL);
    pthread_join(writer, NULL);
    pthread_join(reader2, NULL);

    // Verify writer preference
    assert(has_waiting_writers);  // Writer should have been waiting
    assert(has_waiting_readers);  // Reader2 should have been blocked by writer
    
    rwlock_destroy(&rwlock);
    printf("Writer preference test passed\n");
}

int main(void) {
    printf("Starting KirbyRWLock tests...\n\n");
    
    // Seed random number generator
    srand(time(NULL));
    
    test_basic_operations();
    test_concurrent_access();
    test_writer_preference();
    
    printf("All KirbyRWLock tests passed successfully!\n");
    return 0;
}
