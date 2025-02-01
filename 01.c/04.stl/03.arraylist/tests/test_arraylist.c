#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h> // for usleep();
#include <pthread.h>
#include "my_arraylist.h"
#include "memory_allocator.h"

#define NUM_THREADS 4
#define NUM_OPERATIONS 700 //Q. 왜 숫자가 늘어나면 segfault가 터지는걸까?

typedef struct {
    KirbyArrayList* list;
    int thread_id;
    // int* values;
    size_t* success_count;
} ThreadArgs;

static void* mixed_operations(void* arg);

static void test_basic_operations(void) {
    printf("Testing basic operations...\n");
    
    KirbyArrayList* list = arraylist_create(4, sizeof(int));
    assert(list != NULL);
    assert(arraylist_is_empty(list));
    assert(arraylist_size(list) == 0);
    
    // Test adding elements
    for (int i = 0; i < 5; i++) {
        bool success = arraylist_add(list, &i);
        assert(success);
        assert(arraylist_size(list) == (size_t)(i + 1));
        
        int* value = arraylist_get(list, i);
        assert(value != NULL);
        assert(*value == i);
    }
    
    // Test setting elements
    int new_value = 42;
    bool success = arraylist_set(list, 2, &new_value);
    assert(success);
    
    int* value = arraylist_get(list, 2);
    assert(value != NULL);
    assert(*value == 42);
    
    arraylist_destroy(list);
    printf("Basic operations test passed\n\n");
}

static void* concurrent_add(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    KirbyArrayList* list = args->list;
    int thread_id = args->thread_id;
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        int value = thread_id * NUM_OPERATIONS + i;
        printf("Writer %d: trying to add value %d\n", thread_id, value);
        
        bool success = arraylist_add(list, &value);
        if (success) {
            printf("Writer %d: successfully added value %d\n", thread_id, value);
            __atomic_fetch_add(args->success_count, 1, __ATOMIC_SEQ_CST);
        } else {
            printf("Writer %d: failed to add value %d\n", thread_id, value);
        }
        usleep(1000);  // 1ms delay
    }
    return NULL;
}

static void* concurrent_read(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    KirbyArrayList* list = args->list;
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        size_t current_size = arraylist_size(list);
        if (current_size > 0) {
            size_t index = current_size > 1 ? rand() % (current_size - 1) : 0;

            printf("Reader %d: trying to read index %zu of %zu\n", 
                   args->thread_id, index, current_size);
            
            int* value = arraylist_get(list, index);
            if (value != NULL) {
                printf("Reader %d: successfully read value %d from index %zu\n", 
                       args->thread_id, *value, index);
            } else {
                printf("Reader %d: got NULL from index %zu\n", 
                       args->thread_id, index);
            }
        } else {
            printf("Reader %d: list is empty\n", args->thread_id);
        }
        usleep(1000);  // 1ms delay
    }
    return NULL;
}

static void* mixed_operations(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    KirbyArrayList* list = args->list;
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        usleep(rand() % 100);  // 랜덤 지연
        
        switch (rand() % 3) {
            case 0: {  // Add
                int value = rand() % 1000;
                arraylist_add(list, &value);
                break;
            }
            case 1: {  // Get
                size_t current_size = arraylist_size(list);
                if (current_size > 0) {
                    size_t index = rand() % current_size;
                    int* value = arraylist_get(list, index);
                    if (value != NULL) {
                        assert(*value >= 0);
                    }
                }
                break;
            }
            case 2: {  // Remove
                size_t current_size = arraylist_size(list);
                if (current_size > 0) {
                    size_t index = rand() % current_size;
                    arraylist_remove(list, index);
                }
                break;
            }
        }
    }
    return NULL;
}

static void test_concurrent_operations(void) {
    printf("Testing concurrent operations...\n");
    
    // Step 1: Create list
    printf("Creating list...\n");
    KirbyArrayList* list = arraylist_create(1024, sizeof(int)); //16
    assert(list != NULL);
    printf("List created successfully.\n");
    
    printf("Initial list size: %zu\n", arraylist_size(list));
    
    // Step 2: Initialize counters
    size_t success_count = 0;
    pthread_t writers[NUM_THREADS];
    pthread_t readers[NUM_THREADS];
    ThreadArgs writer_args[NUM_THREADS];
    ThreadArgs reader_args[NUM_THREADS];
    
    // Step 3: Create writer threads
    printf("Creating writer threads...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Creating writer thread %d\n", i);
        writer_args[i] = (ThreadArgs){
            .list = list,
            .thread_id = i,
            .success_count = &success_count
        };
        if (pthread_create(&writers[i], NULL, concurrent_add, &writer_args[i]) != 0) {
            printf("Failed to create writer thread %d\n", i);
            exit(1);
        }
        printf("Writer thread %d created successfully\n", i);
    }
    
    // Step 4: Small delay
    printf("Waiting before creating reader threads...\n");
    usleep(10000);  // 10ms delay
    
    // Step 5: Create reader threads
    printf("Creating reader threads...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Creating reader thread %d\n", i);
        reader_args[i] = (ThreadArgs){
            .list = list,
            .thread_id = i,
            .success_count = &success_count
        };
        if (pthread_create(&readers[i], NULL, concurrent_read, &reader_args[i]) != 0) {
            printf("Failed to create reader thread %d\n", i);
            exit(1);
        }
        printf("Reader thread %d created successfully\n", i);
    }
    
    // Step 6: Join writer threads
    printf("Waiting for writer threads to finish...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Joining writer thread %d\n", i);
        pthread_join(writers[i], NULL);
        printf("Writer thread %d joined\n", i);
    }
    
    // Step 7: Join reader threads
    printf("Waiting for reader threads to finish...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Joining reader thread %d\n", i);
        pthread_join(readers[i], NULL);
        printf("Reader thread %d joined\n", i);
    }
    
    // Step 8: Check final state
    printf("Checking final state...\n");
    size_t final_size = arraylist_size(list);
    printf("Final list size: %zu, Successful adds: %zu\n", final_size, success_count);
    
    // Step 9: Verify integrity
    assert(final_size == success_count);
    
    // Step 10: Cleanup
    printf("Destroying list...\n");
    arraylist_destroy(list);
    printf("Concurrent operations test passed\n\n");
}

static void test_concurrent_mixed_operations(void) {
    printf("Testing concurrent mixed operations...\n");
    
    KirbyArrayList* list = arraylist_create(16, sizeof(int));
    assert(list != NULL);
    
    // Pre-fill list with some data
    for (int i = 0; i < 20; i++) {
        arraylist_add(list, &i);
    }
    
    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];
    
    // Start mixed operation threads
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i] = (ThreadArgs){
            .list = list,
            .thread_id = i,
            .success_count = NULL
        };
        pthread_create(&threads[i], NULL, mixed_operations, &args[i]);
    }
    
    // Wait for all threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final list size after mixed operations: %zu\n", 
           arraylist_size(list));
    
    arraylist_destroy(list);
    printf("Mixed operations test passed\n\n");
}

int main(void) {
    printf("Starting KirbyArrayList tests...\n\n");
    
    // Seed random number generator
    srand(time(NULL));
    
    test_basic_operations();
    test_concurrent_operations();
    test_concurrent_mixed_operations();
    
    printf("All KirbyArrayList tests passed successfully!\n");
    return 0;
}
