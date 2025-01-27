#ifndef BLOCK_H
#define BLOCK_H

#include <stdbool.h>
#include <stddef.h>

#define ALIGNMENT 8
//Q. what is this code? whats alignment for? 그냥 size_t size 그대로 쓰지, 왜 ALIGN(size) 함?
//A. 8의 배수로 만들어줌. 
//ex. size=7일 때: ((7 + 7) & ~7) = 8
// size=9일 때: ((9 + 7) & ~7) = 16
// 8의 배수로 정렬함 -> cpu는 메모리 정렬된 메모리 주소로 더 효율적으로 접근 가능하다.
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1)) 

typedef struct block {
    size_t size;         // Size of the block
    bool free;           // Is block free?
    struct block *next;  // Next block in list
    struct block *prev;  // Previous block in list
} block_t;

#define BLOCK_SIZE sizeof(block_t)

#endif // BLOCK_H
