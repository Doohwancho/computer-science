#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

// Public interface
// Core allocation functions
void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);

// Debug/utility functions
void print_memory_list(void);

// for string
void* my_memcpy(void* dest, const void* src, size_t n);
void* my_memmove(void* dest, const void* src, size_t n);
void* my_memset(void* ptr, int value, size_t n);

#endif // ALLOCATOR_H
