#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stddef.h>
#include <stdbool.h>
#include "my_rwlock.h"
#include "my_mutex.h"

typedef struct {
    void** data;          // 동적 배열
    size_t size;          // 현재 요소 개수
    size_t capacity;      // 할당된 공간
    size_t element_size;  // 각 요소의 크기
    KirbyRWLock lock;         // read-write lock for thread safety
    KirbyMutex resize_lock;   // separate lock for resizing operations
} KirbyArrayList;

// Core operations
KirbyArrayList* arraylist_create(size_t initial_capacity, size_t element_size);
void arraylist_destroy(KirbyArrayList* list);
size_t arraylist_size(const KirbyArrayList* list);
bool arraylist_is_empty(const KirbyArrayList* list);

// Thread-safe element access
void* arraylist_get(KirbyArrayList* list, size_t index);
bool arraylist_set(KirbyArrayList* list, size_t index, void* element);

// Thread-safe modifiers
bool arraylist_add(KirbyArrayList* list, void* element);
bool arraylist_remove(KirbyArrayList* list, size_t index);
bool arraylist_insert(KirbyArrayList* list, size_t index, void* element);
void arraylist_clear(KirbyArrayList* list);

// Bulk operations with optimized locking
size_t arraylist_add_all(KirbyArrayList* list, void** elements, size_t count);
size_t arraylist_remove_range(KirbyArrayList* list, size_t from_index, size_t to_index);

// Capacity
// bool arraylist_ensure_capacity(KirbyArrayList* list, size_t min_capacity);
// void arraylist_trim_to_size(KirbyArrayList* list);

#endif // ARRAYLIST_H
