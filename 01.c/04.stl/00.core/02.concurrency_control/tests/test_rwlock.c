// tests/test_rwlock.c
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>  // for rand()
#include <time.h>    // for time()
#include "my_rwlock.h"

#define NUM_READERS 4
#define NUM_WRITERS 2
#define NUM_ITERATIONS 1000

static KirbyRWLock rwlock;
static int shared_data = 0;
static int read_count = 0;
static int write_count = 0;
static atomic_int concurrent_readers = 0;  // 동시 reader 수 추적

static void* reader(void* arg) {
    int thread_id = *(int*)arg;
    int local_reads = 0;
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        rwlock_read_lock(&rwlock);
        
        // 동시 reader 수 증가 및 체크
        int current_readers = atomic_fetch_add(&concurrent_readers, 1) + 1;
        assert(current_readers > 0);  // 최소 1명
        assert(current_readers <= NUM_READERS);  // 최대 NUM_READERS명
        
        // 읽기 작업
        int value = shared_data;
        assert(value >= 0);  // 데이터 일관성 체크
        local_reads++;
        read_count++;
        
        usleep(rand() % 100);  // 임의 지연
        
        // 동시 reader 수 감소
        atomic_fetch_sub(&concurrent_readers, 1);
        
        rwlock_read_unlock(&rwlock);
        usleep(rand() % 1000);  // 작업 간 지연
    }
    
    printf("Reader %d completed %d reads\n", thread_id, local_reads);
    return NULL;
}

static void* writer(void* arg) {
    int thread_id = *(int*)arg;
    int local_writes = 0;
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        rwlock_write_lock(&rwlock);
        
        // writer는 exclusive access를 가져야 함
        assert(atomic_load(&concurrent_readers) == 0);
        
        // 쓰기 작업
        shared_data++;
        local_writes++;
        write_count++;
        
        usleep(rand() % 100);  // 임의 지연
        
        rwlock_write_unlock(&rwlock);
        usleep(rand() % 1000);  // 작업 간 지연
    }
    
    printf("Writer %d completed %d writes\n", thread_id, local_writes);
    return NULL;
}

static void test_writer_preference(void) {
    printf("Testing writer preference...\n");
    
    KirbyRWLock test_rwlock;
    rwlock_init(&test_rwlock);
    
    // Create two readers and a writer
    pthread_t readers[2];
    pthread_t writer;
    // int reader_active = 0;
    
    // Start first reader
    pthread_create(&readers[0], NULL, (void*(*)(void*))rwlock_read_lock, &test_rwlock);
    usleep(1000);  // Give reader time to acquire lock
    // reader_active = 1;
    
    // Start writer (should wait)
    pthread_create(&writer, NULL, (void*(*)(void*))rwlock_write_lock, &test_rwlock);
    usleep(1000);  // Give writer time to start waiting
    
    // Try starting second reader (should be blocked due to writer preference)
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    pthread_create(&readers[1], NULL, (void*(*)(void*))rwlock_read_lock, &test_rwlock);
    usleep(1000);  // Give second reader time to try acquiring lock
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    // Calculate wait time
    long wait_time = (end.tv_sec - start.tv_sec) * 1000000000L + 
                    (end.tv_nsec - start.tv_nsec);
    bool reader_blocked = wait_time >= 900000;  // 0.9ms 이상 대기했는지
    
    // Cleanup
    rwlock_read_unlock(&test_rwlock);  // Release first reader's lock
    
    // Join all threads
    pthread_join(writer, NULL);
    pthread_join(readers[0], NULL);
    pthread_join(readers[1], NULL);
    
    assert(reader_blocked);
    printf("Writer preference test passed\n\n");
}

static void test_concurrent_access(void) {
    printf("Testing concurrent access...\n");
    
    rwlock_init(&rwlock);
    shared_data = 0;
    read_count = 0;
    write_count = 0;
    atomic_store(&concurrent_readers, 0);
    
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int thread_ids[NUM_READERS + NUM_WRITERS];
    
    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        thread_ids[i] = i;
        pthread_create(&readers[i], NULL, reader, &thread_ids[i]);
    }
    
    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        thread_ids[NUM_READERS + i] = i;
        pthread_create(&writers[i], NULL, writer, &thread_ids[NUM_READERS + i]);
    }
    
    // Join all threads
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    
    printf("Final state: shared_data=%d, reads=%d, writes=%d\n",
           shared_data, read_count, write_count);
           
    assert(shared_data == NUM_WRITERS * NUM_ITERATIONS);
    printf("Concurrent access test passed\n\n");
}

int main(void) {
    printf("Starting RWLock tests...\n\n");
    
    srand(time(NULL));
    
    test_writer_preference();
    test_concurrent_access();
    
    printf("All RWLock tests passed!\n");
    return 0;
}
