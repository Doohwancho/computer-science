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

#endif // ALLOCATOR_H
