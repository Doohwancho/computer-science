#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "memory_allocator.h"


#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define BLOCK_SIZE sizeof(block_t)

// Debug macro - comment out to disable debug prints
#define DEBUG_ALLOC 1
#ifdef DEBUG_ALLOC
    #define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__)
#else
    #define DEBUG_PRINT(...) ((void)0)
#endif


typedef struct block {
    size_t size;         // Size of the block
    bool free;           // Is block free?
    struct block *next;  // Next block in list
    struct block *prev;  // Previous block in list
} block_t;

static block_t *head = NULL;

// Round up to nearest multiple of page size
static size_t align_to_page(size_t size) {
    size_t page_size = getpagesize();
    return (size + page_size - 1) & ~(page_size - 1);
}

// Find a free block that fits size
static block_t *find_free_block(size_t size) {
    block_t *current = head;
    while (current) {
        if (current->free && current->size >= size)
            DEBUG_PRINT("Found free block of size %zu for request of %zu\n", 
                    current->size, size);
            return current;
        current = current->next;
    }
    DEBUG_PRINT("No free block found for size %zu\n", size);
    return NULL;
}

// Split block if remaining size is large enough
static void split_block(block_t *block, size_t size) {
    if (block->size >= size + BLOCK_SIZE + ALIGNMENT) {
        block_t *new_block = (void*)((char*)block + BLOCK_SIZE + size);
        new_block->size = block->size - size - BLOCK_SIZE;
        new_block->free = true;
        new_block->next = block->next;
        new_block->prev = block;
        
        if (block->next)
            block->next->prev = new_block;
        
        block->size = size;
        block->next = new_block;

        DEBUG_PRINT("Split block: original=%zu, new=%zu\n", size, new_block->size);
    }
}

// Merge adjacent free blocks
static void coalesce(block_t *block) {
    if (block->next && block->next->free) {
        DEBUG_PRINT("Coalescing blocks: %zu + %zu\n", 
                   block->size, block->next->size);
        block->size += BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
        if (block->next)
            block->next->prev = block;
    }
}

void *my_malloc(size_t size) {
    if (size == 0) return NULL;
    
    DEBUG_PRINT("\nAllocating %zu bytes\n", size);
    
    size = ALIGN(size);
    block_t *block = find_free_block(size);
    
    if (!block) {
        // No free block found, allocate new memory
        size_t alloc_size = align_to_page(size + BLOCK_SIZE);
        block = mmap(NULL, alloc_size, 
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS,
                    -1, 0);
                    
        if (block == MAP_FAILED) {
            DEBUG_PRINT("mmap failed!\n");
            return NULL;
        }
            
        block->size = alloc_size - BLOCK_SIZE;
        block->free = true;
        block->prev = NULL;
        block->next = head;
        
        if (head)
            head->prev = block;
        head = block;
        
        DEBUG_PRINT("Created new block of size %zu\n", block->size);
    }
    
    split_block(block, size);
    block->free = false;
    
    DEBUG_PRINT("Allocated block at %p, size=%zu\n", 
               (void*)(block + 1), block->size);
               
    return (void*)(block + 1);
}

void my_free(void *ptr) {
    if (!ptr) return;
    
    block_t *block = ((block_t*)ptr) - 1;
    DEBUG_PRINT("\nFreeing block at %p, size=%zu\n", ptr, block->size);
    
    block->free = true;
    
    // Try to coalesce with neighboring blocks
    if (block->prev && block->prev->free) {
        coalesce(block->prev);
    }
    if (block->next && block->next->free) {
        coalesce(block);
    }
}


void* my_realloc(void* ptr, size_t size) {
    if (!ptr) return my_malloc(size);
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }
    
    block_t* block = ((block_t*)ptr) - 1;
    size = ALIGN(size);
    
    // If current block is big enough, we can reuse it
    if (block->size >= size) {
        split_block(block, size);
        return ptr;
    }
    
    // Try to coalesce with next block if it's free
    if (block->next && block->next->free && 
        (block->size + BLOCK_SIZE + block->next->size) >= size) {
        coalesce(block);
        split_block(block, size);
        return ptr;
    }
    
    // Need to allocate new block
    void* new_ptr = my_malloc(size);
    if (!new_ptr) return NULL;
    
    // Copy old data and free old block
    memcpy(new_ptr, ptr, block->size);
    my_free(ptr);
    
    DEBUG_PRINT("Reallocated block from %p to %p, new size=%zu\n", 
               ptr, new_ptr, size);
               
    return new_ptr;
}

// Optional: memory info function
void print_memory_list(void) {
    block_t *current = head;
    printf("\nMemory List:\n");
    while (current) {
        printf("Block at %p: size=%zu, free=%d\n",
               (void*)current, current->size, current->free);
        current = current->next;
    }
    printf("\n");
}