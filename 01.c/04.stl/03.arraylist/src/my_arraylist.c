#include "my_arraylist.h"
#include "memory_allocator.h"
#include <string.h>
#include <stdio.h>

#define GROWTH_FACTOR 2
#define INITIAL_CAPACITY 16 


/**********************
 * Private functions
 */
static bool resize_if_necessary(KirbyArrayList* list, size_t required_size) {
    // Locks are already held by caller (arraylist_add)
    if (!list || list->capacity >= required_size) return true;
    
    printf("Resizing: current_capacity=%zu, required_size=%zu\n", 
           list->capacity, required_size);

    size_t new_capacity = list->capacity * GROWTH_FACTOR;
    if (new_capacity < required_size) {
        new_capacity = required_size;
    }
    
    // Allocate new array
    void** new_data = my_malloc(new_capacity * sizeof(void*));
    if (!new_data) {
        printf("Failed to allocate memory for resize\n");
        return false;
    }
    
    // Copy old data and free
    if (list->data) {
        memcpy(new_data, list->data, list->size * sizeof(void*));
        void** old_data = list->data;  // Keep reference to old data
        list->data = new_data;         // Update pointer atomically
        list->capacity = new_capacity;  // Update capacity
        my_free(old_data);             // Free old data after update
    } else {
        list->data = new_data;
        list->capacity = new_capacity;
    }
    
    printf("Resize complete: new_capacity=%zu\n", new_capacity);
    return true;
}


// static bool resize_if_necessary(KirbyArrayList* list, size_t required_size) {
//     if (!list) return false;

//     // CHANGE: Ensure the new capacity is sufficient
//     if (required_size > list->capacity) {
//         printf("Resizing: current_capacity=%zu, required_size=%zu\n", 
//                list->capacity, required_size);

//         size_t new_capacity = list->capacity * GROWTH_FACTOR;
//         if (new_capacity < required_size) {
//             new_capacity = required_size; // CHANGE: Ensure new_capacity >= required_size
//         }

//         void** new_data = my_malloc(new_capacity * sizeof(void*));
//         if (!new_data) {
//             printf("Failed to allocate memory for resize\n");
//             return false;
//         }

//         if (list->data) {
//             memcpy(new_data, list->data, list->size * sizeof(void*));
//             my_free(list->data);
//         }

//         list->data = new_data;
//         list->capacity = new_capacity;
//         printf("Resize complete: new_capacity=%zu\n", new_capacity);
//     }

//     return true;
// }

// static bool resize_if_necessary(KirbyArrayList* list, size_t required_size) {
//     //잠재적인 문제점 리스트 
//     // 1. data를 free하고 새로운 메모리를 할당하는 동안 다른 스레드가 old data에 접근할 수 있음
//     // 2. resize 중에 rwlock과 resize_lock의 순서가 다른 연산과 충돌할 수 있음
//     // 3. new_data로 교체하는 순간 atomic하지 않음
//     if (!list) return false;
    
//     mutex_lock(&list->resize_lock);
    
//     if (required_size > list->capacity) {
//         printf("Resizing: current_capacity=%zu, required_size=%zu\n", 
//                list->capacity, required_size);

//         size_t new_capacity = list->capacity * GROWTH_FACTOR;
//         if (new_capacity < required_size) {
//             new_capacity = required_size; //ensure new_capacity >= required_size
//         }
        
//         void** new_data = my_malloc(new_capacity * sizeof(void*));
//         if (!new_data) {
//             printf("Failed to allocate memory for resize\n");
//             mutex_unlock(&list->resize_lock);
//             return false;
//         }
        
//         if (list->data) {
//             memcpy(new_data, list->data, list->size * sizeof(void*));
//             my_free(list->data);
//         }
        
//         list->data = new_data;
//         list->capacity = new_capacity;
//         printf("Resize complete: new_capacity=%zu\n", new_capacity);
//     }
    
//     mutex_unlock(&list->resize_lock);
//     return true;
// }

/**********************
 * Public functions - core operations
 */
KirbyArrayList* arraylist_create(size_t initial_capacity, size_t element_size) {
    KirbyArrayList* list = my_malloc(sizeof(KirbyArrayList));
    if (!list) return NULL;
    
    if (initial_capacity == 0) {
        initial_capacity = INITIAL_CAPACITY;
    }
    
    list->data = my_malloc(initial_capacity * sizeof(void*));
    if (!list->data) {
        my_free(list);
        return NULL;
    }
    
    list->size = 0;
    list->capacity = initial_capacity;
    list->element_size = element_size;
    
    rwlock_init(&list->lock);
    mutex_init(&list->resize_lock);
    
    return list;
}

void arraylist_destroy(KirbyArrayList* list) {
    if (!list) return;
    
    rwlock_write_lock(&list->lock);
    
    for (size_t i = 0; i < list->size; i++) {
        if (list->data[i]) {
            my_free(list->data[i]);
        }
    }
    
    my_free(list->data);
    rwlock_write_unlock(&list->lock);

    rwlock_destroy(&list->lock);
    mutex_destroy(&list->resize_lock);
    my_free(list);
}

bool arraylist_is_empty(const KirbyArrayList* list) {
    return arraylist_size(list) == 0;
}

size_t arraylist_size(const KirbyArrayList* list) {
    if (!list) return 0;
    rwlock_read_lock((KirbyRWLock*)&list->lock);
    size_t size = list->size;
    rwlock_read_unlock((KirbyRWLock*)&list->lock);
    return size;
}



/**********************
 * Public functions - thread-safe element access
 */
void* arraylist_get(KirbyArrayList* list, size_t index) {
    if (!list) return NULL;
    
    rwlock_read_lock(&list->lock);
    
    // Double check after acquiring lock, 추가 안전장치 
    if (index >= list->size) {
        rwlock_read_unlock(&list->lock);
        return NULL;
    }
    
    void* element = list->data[index];
    rwlock_read_unlock(&list->lock);
    
    return element;
}

bool arraylist_set(KirbyArrayList* list, size_t index, void* element) {
    if (!list || !element || index >= list->size) return false;
    
    rwlock_write_lock(&list->lock);
    
    void* new_element = my_malloc(list->element_size);
    if (!new_element) {
        rwlock_write_unlock(&list->lock);
        return false;
    }
    
    memcpy(new_element, element, list->element_size);
    
    if (list->data[index]) {
        my_free(list->data[index]);
    }
    
    list->data[index] = new_element;
    
    rwlock_write_unlock(&list->lock);
    return true;
}




/**********************
 * Public functions - Thread-safe modifiers 
 */
bool arraylist_add(KirbyArrayList* list, void* element) {
    if (!list || !element) return false;
    
    mutex_lock(&list->resize_lock);
    rwlock_write_lock(&list->lock);
    
    printf("Before add: size=%zu, capacity=%zu\n", 
           list->size, list->capacity);
    
    bool success = false;
    
    // Handle resize if needed
    if (list->size >= list->capacity) {
        printf("Resizing required: size=%zu, capacity=%zu\n", 
               list->size, list->capacity);
        if (!resize_if_necessary(list, list->size + 1)) {
            goto cleanup;  // Use goto for clean error handling
        }
    }
    
    // Add new element
    void* new_element = my_malloc(list->element_size);
    if (!new_element) {
        goto cleanup;
    }
    
    memcpy(new_element, element, list->element_size);
    list->data[list->size++] = new_element;
    success = true;
    printf("Added element: size=%zu, capacity=%zu\n", 
           list->size, list->capacity);

cleanup:
    rwlock_write_unlock(&list->lock);
    mutex_unlock(&list->resize_lock);
    return success;
}

// bool arraylist_add(KirbyArrayList* list, void* element) {
//     if (!list || !element) return false;
    
//     // Acquire resize_lock first, then rwlock_write_lock
//     mutex_lock(&list->resize_lock);
//     rwlock_write_lock(&list->lock);
    
//     printf("Before add: size=%zu, capacity=%zu\n", 
//            list->size, list->capacity);
    
//     // First try to resize if needed
//     if (list->size >= list->capacity) {
//         printf("Resizing required: size=%zu, capacity=%zu\n", 
//                list->size, list->capacity); // Debug statement
//         if (!resize_if_necessary(list, list->size + 1)) {
//             rwlock_write_unlock(&list->lock);
//             mutex_unlock(&list->resize_lock);
//             return false;
//         }
//     }
    
//     bool success = false;
//     void* new_element = my_malloc(list->element_size);
//     if (new_element) {
//         memcpy(new_element, element, list->element_size);
//         size_t current_size = list->size;
//         list->data[current_size] = new_element;
//         list->size = current_size + 1;
//         success = true;
//         printf("Added element: size=%zu, capacity=%zu\n", 
//                list->size, list->capacity); // CHANGE: Debug statement
//     } else {
//         printf("Failed to allocate memory for new element\n"); // CHANGE: Debug statement
//     }
    
//     // unlock rwlock first, and then mutex lock next
//     rwlock_write_unlock(&list->lock);
//     mutex_unlock(&list->resize_lock);
//     return success;
// }

bool arraylist_remove(KirbyArrayList* list, size_t index) {
    if (!list || index >= list->size) return false;
    
    rwlock_write_lock(&list->lock);
    
    if (list->data[index]) {
        my_free(list->data[index]);
    }
    
    // Shift elements left
    for (size_t i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    
    list->size--;
    
    rwlock_write_unlock(&list->lock);
    return true;
}


bool arraylist_insert(KirbyArrayList* list, size_t index, void* element) {
    if (!list || !element || index > list->size) return false;
    
    rwlock_write_lock(&list->lock);
    
    // Resize if necessary
    if (!resize_if_necessary(list, list->size + 1)) {
        rwlock_write_unlock(&list->lock);
        return false;
    }
    
    // Create new element
    void* new_element = my_malloc(list->element_size);
    if (!new_element) {
        rwlock_write_unlock(&list->lock);
        return false;
    }
    memcpy(new_element, element, list->element_size);
    
    // Shift elements right
    for (size_t i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }
    
    list->data[index] = new_element;
    list->size++;
    
    rwlock_write_unlock(&list->lock);
    return true;
}

void arraylist_clear(KirbyArrayList* list) {
    if (!list) return;
    
    rwlock_write_lock(&list->lock);
    
    // Free all elements
    for (size_t i = 0; i < list->size; i++) {
        if (list->data[i]) {
            my_free(list->data[i]);
            list->data[i] = NULL;
        }
    }
    
    list->size = 0;
    
    rwlock_write_unlock(&list->lock);
}



/**********************
 * Public functions - Bulk operations with optimized locking
 */
size_t arraylist_add_all(KirbyArrayList* list, void** elements, size_t count) {
    if (!list || !elements || count == 0) return 0;
    
    rwlock_write_lock(&list->lock);
    
    // Ensure capacity for all elements
    if (!resize_if_necessary(list, list->size + count)) {
        rwlock_write_unlock(&list->lock);
        return 0;
    }
    
    size_t added_count = 0;
    for (size_t i = 0; i < count; i++) {
        void* new_element = my_malloc(list->element_size);
        if (!new_element) break;
        
        memcpy(new_element, elements[i], list->element_size);
        list->data[list->size++] = new_element;
        added_count++;
    }
    
    rwlock_write_unlock(&list->lock);
    return added_count;
}

size_t arraylist_remove_range(KirbyArrayList* list, size_t from_index, size_t to_index) {
    if (!list || from_index >= list->size || to_index > list->size || from_index >= to_index) {
        return 0;
    }
    
    rwlock_write_lock(&list->lock);
    
    // Free elements in range
    for (size_t i = from_index; i < to_index; i++) {
        if (list->data[i]) {
            my_free(list->data[i]);
        }
    }
    
    // Shift remaining elements left
    size_t remove_count = to_index - from_index;
    for (size_t i = from_index; i < list->size - remove_count; i++) {
        list->data[i] = list->data[i + remove_count];
    }
    
    list->size -= remove_count;
    
    rwlock_write_unlock(&list->lock);
    return remove_count;
}


/**********************
 * Public functions - capacity
 */
// bool arraylist_ensure_capacity(KirbyArrayList* list, size_t min_capacity) {
//     if (list->capacity >= min_capacity) return true;
    
//     mutex_lock(&list->resize_lock);
    
//     // Double-check after acquiring lock
//     if (list->capacity >= min_capacity) {
//         mutex_unlock(&list->resize_lock);
//         return true;
//     }
    
//     size_t new_capacity = list->capacity * GROWTH_FACTOR;
//     if (new_capacity < min_capacity) {
//         new_capacity = min_capacity;
//     }
    
//     void** new_data = my_malloc(new_capacity * sizeof(void*));
//     if (!new_data) {
//         mutex_unlock(&list->resize_lock);
//         return false;
//     }
    
//     memcpy(new_data, list->data, list->size * sizeof(void*));
//     my_free(list->data);
//     list->data = new_data;
//     list->capacity = new_capacity;
    
//     mutex_unlock(&list->resize_lock);
//     return true;
// }

// void arraylist_trim_to_size(KirbyArrayList* list) {
//     if (!list || list->size == list->capacity) return;
    
//     mutex_lock(&list->resize_lock);
//     rwlock_write_lock(&list->lock);  // Need both locks to prevent data corruption
    
//     if (list->size < list->capacity) {
//         void** new_data = my_malloc(list->size * sizeof(void*));
//         if (new_data) {
//             memcpy(new_data, list->data, list->size * sizeof(void*));
//             my_free(list->data);
//             list->data = new_data;
//             list->capacity = list->size;
//         }
//     }
    
//     rwlock_write_unlock(&list->lock);
//     mutex_unlock(&list->resize_lock);
// }
