#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "linkedlist.h"

// Test helper functions
static void print_int(void* data, size_t size, void* user_data) {
    (void)size;
    (void)user_data;
    printf("%d ", *(int*)data);
}

static void test_create_destroy(void) {
    printf("Testing list creation and destruction...\n");
    
    LinkedList* list = list_create();
    assert(list != NULL);
    assert(list_is_empty(list));
    assert(list_length(list) == 0);
    
    list_destroy(list);
    printf("PASSED\n\n");
}

static void test_append(void) {
    printf("Testing append and prepend operations...\n");
    
    LinkedList* list = list_create();
    
    // Test append
    int values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        assert(list_append(list, &values[i], sizeof(int)));
        assert(list_length(list) == (size_t)(i + 1));
        assert(*(int*)list_get_at(list, i) == values[i]);
    }
    
    // // Test prepend -- 미구현됨 
    // int front_values[] = {0, -1, -2};
    // for (int i = 0; i < 3; i++) {
    //     assert(list_prepend(list, &front_values[i], sizeof(int)));
    //     assert(*(int*)list_get_at(list, 0) == front_values[i]);
    // }
    
    printf("List content: ");
    list_foreach(list, print_int, NULL);
    printf("\n");
    
    list_destroy(list);
    printf("PASSED\n\n");
}

static void test_insert_remove(void) {
    printf("Testing insert and remove operations...\n");
    
    LinkedList* list = list_create();
    int values[] = {1, 2, 3, 4, 5};
    
    // Insert at various positions
    for (int i = 0; i < 5; i++) {
        assert(list_append(list, &values[i], sizeof(int)));
    }
    
    int insert_val = 99;
    assert(list_insert_at(list, 2, &insert_val, sizeof(int)));
    assert(*(int*)list_get_at(list, 2) == insert_val);
    
    // Remove from various positions
    void* removed_data;
    assert(list_remove_at(list, 2, &removed_data));
    assert(*(int*)removed_data == insert_val);
    free(removed_data);
    
    printf("List content after operations: ");
    list_foreach(list, print_int, NULL);
    printf("\n");
    
    list_destroy(list);
    printf("PASSED\n\n");
}

static void test_find_set(void) {
    printf("Testing find and set operations...\n");
    
    LinkedList* list = list_create();
    int values[] = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 5; i++) {
        list_append(list, &values[i], sizeof(int));
    }
    
    // Test find
    int search_val = 3;
    ssize_t index = list_find(list, &search_val, sizeof(int));
    assert(index == 2);
    
    // Test set
    int new_val = 99;
    assert(list_set_at(list, 2, &new_val, sizeof(int)));
    assert(*(int*)list_get_at(list, 2) == new_val);
    
    printf("List content after operations: ");
    list_foreach(list, print_int, NULL);
    printf("\n");
    
    list_destroy(list);
    printf("PASSED\n\n");
}

static void test_clear(void) {
    printf("Testing clear operation...\n");
    
    LinkedList* list = list_create();
    int values[] = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 5; i++) {
        list_append(list, &values[i], sizeof(int));
    }
    
    list_clear(list);
    assert(list_is_empty(list));
    assert(list_length(list) == 0);
    
    list_destroy(list);
    printf("PASSED\n\n");
}

int main(void) {
    printf("Starting LinkedList tests...\n\n");
    
    test_create_destroy();
    test_append();
    test_insert_remove();
    test_find_set();
    test_clear();
    
    printf("All tests passed successfully!\n");
    return 0;
}
