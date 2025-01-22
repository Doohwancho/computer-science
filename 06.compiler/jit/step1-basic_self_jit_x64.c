#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>


/*

Q. what is this code?

this file is implementation of this simple function below

long add4(long num) {
  return num + 4;
}


얘를 heap에 mmap()으로 할당하고, 그 주소를 실행시키는 코드입니다.

포인트는 mmap으로 0부터 size(2^10)까지 메모리 받은다음, 거기에 code[]인 기계어를 memcpy로 넣는데,
권한을 rwx로 받고 넣는 것임.

근데 c가 heap 지맘대로 건들이면 안되니까, os선에서 컷함
그러니 step2에선 rw로 받고, 그 다음에 rwx로 바꾸는 코드를 넣음.

이 코드는 segmentation error 뜸.
확실하진 않지만 메모리를 받아야 하는 주소가 다른애가 쓰고있어서 허락이 안됬나봄.


 */


// Allocates RWX memory of given size and returns a pointer to it. On failure,
// prints out the error and returns NULL.
void* alloc_executable_memory(size_t size) {
  void* ptr = mmap(0, size,
                   PROT_READ | PROT_WRITE | PROT_EXEC,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); //step1) Use mmap to allocate a readable, writable and executable chunk of memory on the heap. why? you can't directly use heap memory without permission of os due to 'protected mode'
  if (ptr == (void*)-1) { //error! segmentation fault. mmap으로 주소 0번지에 size만큼 쓰려고 하는데, os가 컷해서 메모리를 할당받지 못했을 경우
    perror("mmap!!!");
    return NULL;
  }
  return ptr;
}

void emit_code_into_memory(unsigned char* m) {
  unsigned char code[] = {
    0x48, 0x89, 0xf8,                   // mov %rdi, %rax
    0x48, 0x83, 0xc0, 0x04,             // add $4, %rax
    0xc3                                // ret
  };
  memcpy(m, code, sizeof(code));
}

const size_t SIZE = 1024;
typedef long (*JittedFunc)(long);

// Allocates RWX memory directly.
void run_from_rwx() {
  void* m = alloc_executable_memory(SIZE); //allocate 2^10 size of memory
  emit_code_into_memory(m); //step2) copy the machine code implementing add4 into this chunk

  JittedFunc func = m; //step3) execute code from this chunk by casting it to a function pointer and calling through it
  int result = func(2); //note this is executable only because it got rwx permission from alloc_executable_memory()
  printf("result = %d\n", result); //result 6 expected
}

// create main function
int main() {
  run_from_rwx();
  return 0;
}

// Compile and run
// $ gcc -o basic_self_jit_x64 basic_self_jit_x64.c
