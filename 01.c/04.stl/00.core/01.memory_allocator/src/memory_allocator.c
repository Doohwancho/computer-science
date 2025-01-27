#include "memory_allocator.h"
#include "block.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


// Debug macro - comment out to disable debug prints
#define DEBUG_ALLOC 1
#ifdef DEBUG_ALLOC
    #define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__)
#else
    #define DEBUG_PRINT(...) ((void)0)
#endif


static block_t *head = NULL;

// Round up to nearest multiple of page size
static size_t align_to_page(size_t size) {
    size_t page_size = getpagesize();
    return (size + page_size - 1) & ~(page_size - 1); //Q. 이게 뭐야? 페이지 사이즈?
}

// 기존에 free block 중에서, 메모리 요구치보다 더 큰 block이 있으면(작으면 못넣잖아?) return that free block
static block_t* find_free_block(size_t size) {
    block_t *current = head;
    while (current) {
        if (current->free && current->size >= size) { //해당 free block이, 요구치 size보다 같거나 커야 해당 block을 반환한다.
            DEBUG_PRINT("Found free block of size %zu for request of %zu\n", 
                    current->size, size);
            return current;
        }
        current = current->next;
    }
    DEBUG_PRINT("No free block found for size %zu\n", size);
    return NULL;
}

// Q. 혹시 free block들이 linkedlist 구조가 아니라 tree 구조야? 정확히는 heap? what is this split algorithm?
// Split block if remaining size is large enough
static void split_block(block_t *block, size_t size) {
    if(!block) return;
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

// coalesce = 합치다 
// Merge adjacent free blocks
// Q. my_free()에서 특정 block을 메모리 해제하고, 그 block 앞,뒤를 붙이는 함수인데, 왜 free() 안함? 그냥 노드만 이어 붙이면 메모리 해제는 안한거잖아?
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

void* my_malloc(size_t size) {
    if (size == 0) return NULL;
    
    DEBUG_PRINT("\nAllocating %zu bytes\n", size);
    
    size = ALIGN(size);
    block_t *block = find_free_block(size);
    
    //만약 기존 heap에서 free block이 없다면, allocate new memory space
    if (!block) {
        // No free block found, allocate new memory
        size_t alloc_size = align_to_page(size + BLOCK_SIZE);  //Q. 왜 기존 메모리 요구 사이즈에서 block_size를 더하는거지? 
                                                               //A. 실제 데이터 외에 메타데이터 저장할 사이즈도 필요하기 때문. BLOCK_SIZE가 메타데이터 사이즈, size가 actual data's size
        block = mmap(NULL, alloc_size,  //syscall to get memory from os (data size + metadata size)
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS,
                    -1, 0);
                    
        //error handling
        if (block == MAP_FAILED) {
            DEBUG_PRINT("mmap failed!\n");
            return NULL;
        }

        // 할당된 크기가 충분한지 확인
        if (alloc_size < size + BLOCK_SIZE) {
            munmap(block, alloc_size);
            return NULL;
        }
            
        block->size = alloc_size - BLOCK_SIZE; //Q. 이번엔 또 빼네? A. 메타데이터 공간 빼고 actual data size
        block->free = true; //Q. 이게 왜 false가 아니야? 메모리 할당했는데? A. 할당받은 직후라 아직 사용자에게 할당되기 전이기 때문
        block->prev = NULL; 
        block->next = head; 
        
        if (head)
            head->prev = block;
        head = block; //Q. 최신에 할당한 노드를 root node로 넣네? 왜지? A. 단순 구현 편의성 때문. 맨 끝에 붙여도 상관없다.
        
        DEBUG_PRINT("Created new block of size %zu\n", block->size);
    }
    
    split_block(block, size);
    block->free = false;
    
    DEBUG_PRINT("Allocated block at %p, size=%zu\n", 
               (void*)(block + 1), block->size);

    //메모리 레이아웃:
    //
    // [block_t 구조체][실제 데이터 공간........................]
    //       ↑          ↑
    //     block       block + 1
      
    return (void*)(block + 1); //Q. 왜 +1하지?  A. 메타데이터 말고 실제 데이터가 저장된 공간 반환하려고.
}

void my_free(void *ptr) {
    if (!ptr) return;
    
    //ptr-1 가서 메타데이터 찾기 
    block_t *block = ((block_t*)ptr) - 1;
    DEBUG_PRINT("\nFreeing block at %p, size=%zu\n", ptr, block->size);
    
    block->free = true;
    
    // Try to coalesce(합치다) with neighboring blocks
    if (block->prev && block->prev->free) {
        coalesce(block->prev);
    }
    if (block->next && block->next->free) {
        coalesce(block);
    }
}


//Q. what is realloc for?
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
