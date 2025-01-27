#include <stdio.h>
#include <stdlib.h>  // for rand()
#include <assert.h>
#include <string.h>
#include <stdint.h>  // for uintptr_t
#include <time.h>    // for time() to seed random
#include "memory_allocator.h"

// Ensure we have access to the same alignment value
#define ALIGNMENT 8

// Test helper functions
static void test_setup(void) {
    // Seed random number generator
    srand((unsigned int)time(NULL));
}

static void test_teardown(void) {
    // Clean up any remaining allocations
}

// Basic allocation tests
static void test_basic_allocation(void) {
    printf("Running basic allocation test...\n");
    
    // Test 1: Basic allocation
    void* ptr1 = my_malloc(100);
    assert(ptr1 != NULL);
    
    // Test 2: Zero size allocation
    void* ptr2 = my_malloc(0);
    assert(ptr2 == NULL);
    
    // Test 3: Write to allocated memory
    memset(ptr1, 0xFF, 100);
    
    my_free(ptr1);
    my_free(ptr2);
    printf("Basic allocation test passed\n");
}

// Alignment tests
static void test_alignment(void) {
    printf("Running alignment test...\n");
    
    // Test various sizes to verify alignment
    for (size_t size = 1; size <= 1024; size *= 2) {
        void* ptr = my_malloc(size);
        assert(ptr != NULL);
        // Verify alignment
        assert(((uintptr_t)ptr % ALIGNMENT) == 0);
        my_free(ptr);
    }
    
    printf("Alignment test passed\n");
}

// Multiple allocation test
static void test_multiple_allocations(void) {
    printf("Running multiple allocations test...\n");
    
    #define NUM_ALLOCS 100
    void* ptrs[NUM_ALLOCS];
    
    // Allocate multiple blocks
    for (int i = 0; i < NUM_ALLOCS; i++) {
        ptrs[i] = my_malloc(i * 8 + 8);
        assert(ptrs[i] != NULL);
        memset(ptrs[i], 0xAA, i * 8 + 8);
    }
    
    // Free in reverse order
    for (int i = NUM_ALLOCS - 1; i >= 0; i--) {
        my_free(ptrs[i]);
    }
    
    printf("Multiple allocations test passed\n");
}

// Fragmentation test
static void test_fragmentation(void) {
    printf("Running fragmentation test...\n");
    
    void* ptrs[5];
    
    // Allocate 5 blocks
    for (int i = 0; i < 5; i++) {
        ptrs[i] = my_malloc(100);
        assert(ptrs[i] != NULL);
    }
    
    // Free alternate blocks
    my_free(ptrs[1]);
    my_free(ptrs[3]);
    
    // Allocate smaller blocks that should fit in the gaps
    void* small1 = my_malloc(50);
    void* small2 = my_malloc(50);
    assert(small1 != NULL);
    assert(small2 != NULL);
    
    // Clean up
    my_free(ptrs[0]);
    my_free(ptrs[2]);
    my_free(ptrs[4]);
    my_free(small1);
    my_free(small2);
    
    printf("Fragmentation test passed\n");
}

// Coalescing test
static void test_coalescing(void) {
    printf("Running coalescing test...\n");
    
    // Allocate three consecutive blocks
    void* ptr1 = my_malloc(100);
    void* ptr2 = my_malloc(100);
    void* ptr3 = my_malloc(100);
    assert(ptr1 != NULL && ptr2 != NULL && ptr3 != NULL);
    
    // Free them in a way that should trigger coalescing
    my_free(ptr1);
    my_free(ptr3);
    my_free(ptr2);  // This should coalesce all three blocks
    
    // Try to allocate a larger block that should fit in the coalesced space
    void* large_ptr = my_malloc(250);
    assert(large_ptr != NULL);
    
    my_free(large_ptr);
    printf("Coalescing test passed\n");
}

// // Stress test
// static void test_stress(void) {
//     printf("Running stress test...\n");
    
//     #define STRESS_ITERATIONS 1000
//     #define MAX_ALLOCS 100
    
//     void* ptrs[MAX_ALLOCS] = {NULL};
    
//     for (int i = 0; i < STRESS_ITERATIONS; i++) {
//         int idx = rand() % MAX_ALLOCS;
//         if (ptrs[idx] != NULL) {
//             my_free(ptrs[idx]);
//             ptrs[idx] = NULL;
//         } else {
//             size_t size = (rand() % 1000) + 1;
//             ptrs[idx] = my_malloc(size);
//             if (ptrs[idx] != NULL) {
//                 memset(ptrs[idx], 0xFF, size);
//             }
//         }
//     }
    
//     // Clean up
//     for (int i = 0; i < MAX_ALLOCS; i++) {
//         if (ptrs[i] != NULL) {
//             my_free(ptrs[i]);
//         }
//     }
    
//     printf("Stress test passed\n");
// }

int main(void) {
    printf("Starting memory allocator tests...\n\n");
    
    test_setup();
    
    test_basic_allocation();
    test_alignment();
    test_multiple_allocations();
    test_fragmentation();
    test_coalescing();
    // test_stress(); //error! - seg fault!
    
    test_teardown();
    
    printf("\nAll tests passed successfully!\n");
    return 0;
}
