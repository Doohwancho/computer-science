#include "kirby_vector.h"
#include "memory_allocator.h"
#include <string.h>
#include <assert.h>

#define VECTOR_GROWTH_FACTOR 2
#define VECTOR_INITIAL_CAPACITY 4
 
/************************8
 * Private
 */
static bool vector_resize(KirbyVector* vec, size_t new_capacity) {
    if(!vec) return false;

    void** new_data = my_malloc(new_capacity * sizeof(void*));
    if(!new_data) return false;

    // Copy existing elements 
    if(vec->data) {
        memcpy(new_data, vec->data, vec->size * sizeof(void*));
        my_free(vec->data);
    }

    vec->data = new_data;
    vec->capacity = new_capacity;
    
    return true;
}


/************************8
 * Public 
 */
// Core operations
KirbyVector* vector_create(size_t initial_capacity, size_t element_size) {  
    KirbyVector* vec = my_malloc(sizeof(KirbyVector));
    if(!vec) return NULL;

    if(initial_capacity == 0) {
        initial_capacity = VECTOR_INITIAL_CAPACITY;
    }

    // vec->data = NULL;
    vec->data = my_malloc(initial_capacity * sizeof(void*));
    if(!vec->data) {
        my_free(vec);
        return NULL;
    }

    vec->size = 0;
    vec->capacity = initial_capacity;
    vec->element_size = element_size;

    return vec; 
}

void vector_destroy(KirbyVector* vec) {
    if(!vec) return;
    
    // free all elements
    for(size_t i = 0; i < vec->size; i++) {
        if(vec->data[i]) {
            my_free(vec->data[i]);
        }
    } 

    // free vector
    my_free(vec->data);
    my_free(vec);
}

bool vector_is_empty(const KirbyVector* vec) {
    return vec ? vec->size == 0 : true;
}

size_t vector_size(const KirbyVector* vec) {
    return vec ? vec->size : 0;
}

size_t vector_capacity(const KirbyVector* vec) { // Q. 이걸 public method로 하는게 맞는건가?
    return vec ? vec->capacity : 0;
}

// Element Access
void* vector_get(const KirbyVector* vec, size_t index) {
    if(!vec || index >= vec->size) return NULL;
    return vec->data[index];
}

bool vector_set(KirbyVector* vec, size_t index, void* element) {
    if(!vec || !element || index >= vec->size) return false;

    void* new_element = my_malloc(vec->element_size);
    if(!new_element) return false;

    memcpy(new_element, element, vec->element_size); //from string.h
    
    // Free old element if it exists
    if(vec->data[index]) {
        my_free(vec->data[index]);
    }

    vec->data[index] = new_element;
    return true;
}


// Modifier 
bool vector_push_back(KirbyVector* vec, void* element) {
    if(!vec || !element) return false;

    // Resize if necessary
    if(vec->size == vec->capacity) {
        size_t new_capacity = vec->capacity * VECTOR_GROWTH_FACTOR;
        if(!vector_resize(vec, new_capacity)) {
            return false;
        }
    }

    // Allocate and copy new element
    void* new_element = my_malloc(vec->element_size);
    if(!new_element) return false;

    memcpy(new_element, element, vec->element_size);
    vec->data[vec->size++] = new_element;

    return true;
}

bool vector_insert(KirbyVector* vec, size_t index, void* element) {
    if(!vec || !element || index > vec->size) return false;

    // Resize if necessary
    if(vec->size == vec->capacity) {
        size_t new_capacity = vec->capacity * VECTOR_GROWTH_FACTOR;
        if(!vector_resize(vec, new_capacity)) {
            return false;
        }
    }
    
    // Shift Element to right 
    for(size_t i = vec->size; i > index; i--) {
        vec->data[i] = vec->data[i-1];
    }

    // Allocate and copy new element
    void* new_element = my_malloc(vec->element_size);
    if(!new_element) return false;

    memcpy(new_element, element, vec->element_size);
    vec->data[index] = new_element;
    vec->size++;

    return true;
}

bool vector_remove(KirbyVector* vec, size_t index) {
    if(!vec || index >= vec->size) return false;

    // Free element at index 
    if(vec->data[index]) {
        my_free(vec->data[index]);
    }

    // Shift element left 
    for(size_t i = index; i < vec->size -1; i++) {
        vec->data[i] = vec->data[i+1];
    }

    vec->size--;
    return true;
}

void vector_clear(KirbyVector* vec) {
    if(!vec) return;

    // Free all elements
    for(size_t i = 0; i < vec->size; i++) {
        if(vec->data[i]) {
            my_free(vec->data[i]);
        }
    }
    
    vec->size = 0;
}

// Capacity Handling 
bool vector_reserve(KirbyVector* vec, size_t new_capacity) {
    if(!vec || new_capacity <= vec->capacity) return false;
    return vector_resize(vec, new_capacity);
}

bool vector_shrink_to_fit(KirbyVector* vec) {
    if(!vec || vec->size == vec->capacity) return false;
    return vector_resize(vec, vec->size);
}

// Iterator function type 
typedef void (*KirbyVectorIterFn)(void* element, void* user_data);
void vector_foreach(const KirbyVector* vec, KirbyVectorIterFn fn, void* user_data) {
    if(!vec || !fn) return;

    for(size_t i = 0; i < vec->size; i++) {
        fn(vec->data[i], user_data);
    }
}
