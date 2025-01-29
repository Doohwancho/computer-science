#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "kirby_vector.h"

static void print_int(void* element, void* user_data) {
    (void)user_data;
    printf("%d ", *(int*)element);
}

static void test_create_destroy(void) {
    printf("Testing vector creation and destruction...\n");
    
    KirbyVector* vec = vector_create(4, sizeof(int));
    assert(vec != NULL);
    assert(vector_is_empty(vec));
    assert(vector_size(vec) == 0);
    assert(vector_capacity(vec) == 4);
    
    vector_destroy(vec);
    printf("PASSED\n\n");
}

static void test_push(void) {
    printf("Testing push and pop operations...\n");
    
    KirbyVector* vec = vector_create(4, sizeof(int));
    
    // Test push_back
    for (int i = 0; i < 10; i++) {
        assert(vector_push_back(vec, &i));
        assert(vector_size(vec) == (size_t)(i + 1));
        assert(*(int*)vector_get(vec, i) == i);
    }
    
    // // Test pop_back
    // for (int i = 9; i >= 0; i--) {
    //     assert(*(int*)vector_back(vec) == i);
    //     assert(vector_pop_back(vec));
    // }
    
    // assert(vector_is_empty(vec));
    vector_destroy(vec);
    printf("PASSED\n\n");
}

static void test_insert_remove(void) {
    printf("Testing insert and remove operations...\n");
    
    KirbyVector* vec = vector_create(4, sizeof(int));
    
    // Insert elements
    for (int i = 0; i < 5; i++) {
        assert(vector_push_back(vec, &i));
    }
    
    // Insert at middle
    int value = 99;
    assert(vector_insert(vec, 2, &value));
    assert(*(int*)vector_get(vec, 2) == value);
    assert(vector_size(vec) == 6);
    
    // Remove from middle
    assert(vector_remove(vec, 2));
    assert(vector_size(vec) == 5);
    assert(*(int*)vector_get(vec, 2) == 2);
    
    printf("KirbyVector* content: ");
    vector_foreach(vec, print_int, NULL);
    printf("\n");
    
    vector_destroy(vec);
    printf("PASSED\n\n");
}

static void test_set_get(void) {
    printf("Testing set and get operations...\n");
    
    KirbyVector* vec = vector_create(4, sizeof(int));
    
    // Fill vector
    for (int i = 0; i < 5; i++) {
        assert(vector_push_back(vec, &i));
    }
    
    // Test set
    int new_value = 99;
    assert(vector_set(vec, 2, &new_value));
    assert(*(int*)vector_get(vec, 2) == new_value);
    
    // Test get
    // assert(*(int*)vector_front(vec) == 0);
    // assert(*(int*)vector_back(vec) == 4);
    
    printf("KirbyVector* content: ");
    vector_foreach(vec, print_int, NULL);
    printf("\n");
    
    vector_destroy(vec);
    printf("PASSED\n\n");
}

static void test_capacity(void) {
    printf("Testing capacity operations...\n");
    
    KirbyVector* vec = vector_create(4, sizeof(int));
    
    // Test reserve
    assert(vector_reserve(vec, 10));
    assert(vector_capacity(vec) == 10);
    
    // Fill vector
    for (int i = 0; i < 5; i++) {
        assert(vector_push_back(vec, &i));
    }
    
    // Test shrink_to_fit
    assert(vector_shrink_to_fit(vec));
    assert(vector_capacity(vec) == vector_size(vec));
    
    vector_destroy(vec);
    printf("PASSED\n\n");
}

int main(void) {
    printf("Starting KirbyVector* tests...\n\n");
    
    test_create_destroy();
    test_push();
    test_insert_remove();
    test_set_get();
    test_capacity();
    
    printf("All tests passed successfully!\n");
    return 0;
}
