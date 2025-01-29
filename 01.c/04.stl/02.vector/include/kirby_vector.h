#ifndef VECTOR_H //Q. whats this ifndef? and endif?
#define VECTOR_H //Q. why define VECTOR_H?

#include <stddef.h>  //iterator function type 정의할 때 쓰임 
#include <stdbool.h> //bool 쓸 때 필요함 
#include <sys/types.h> //size_t 쓸 때 필요함 

typedef struct {
   void** data; // 동적 배열 
   size_t size; // 현재 요소 갯수
   size_t capacity; // 할당된 공간 
   size_t element_size; //각 요소의 크기 
} KirbyVector;

// Core operations
KirbyVector* vector_create(size_t initial_capacity, size_t element_size);
void vector_destroy(KirbyVector* vec);
bool vector_is_empty(const KirbyVector* vec);
size_t vector_size(const KirbyVector* vec);
size_t vector_capacity(const KirbyVector* vec);

// Element Access
void* vector_get(const KirbyVector* vec, size_t index);
bool vector_set(KirbyVector* vec, size_t index, void* element);


// Modifier 
bool vector_push_back(KirbyVector* vec, void* element);
bool vector_insert(KirbyVector* vec, size_t index, void* element);
bool vector_remove(KirbyVector* vec, size_t index);
void vector_clear(KirbyVector* vec);

// Capacity Handling 
bool vector_reserve(KirbyVector* vec, size_t new_capacity);
bool vector_shrink_to_fit(KirbyVector* vec);

// Iterator function type 
typedef void (*KirbyVectorIterFn)(void* element, void* user_data);
void vector_foreach(const KirbyVector* vec, KirbyVectorIterFn fn, void* user_data);

#endif
