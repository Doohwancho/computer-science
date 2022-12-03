#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>


/*
 * step1와 차이점?
 *
 * rwx에서 rw로 변경함. 일단 쓰고
 * rw ->rx 로 변경함.
 * 보안상 좀 더 좋아짐.
 *
 * heap 건드리는 rx이 malloc임.
 * 거즘 malloc을 구현했다고 봐도 될 듯.
 *
 */


// Allocates RW memory of given size and returns a pointer to it. On failure,
// prints out the error and returns NULL. Unlike malloc, the memory is allocated
// on a page boundary so it's suitable for calling mprotect.
void* alloc_writable_memory(size_t size) {
  void* ptr = mmap(0, size,
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (ptr == (void*)-1) {
    perror("mmap");
    return NULL;
  }
  return ptr;
}

// Sets a RX permission on the given memory, which must be page-aligned. Returns
// 0 on success. On failure, prints out the error and returns -1.
int make_memory_executable(void* m, size_t size) {
  if (mprotect(m, size, PROT_READ | PROT_EXEC) == -1) {
    perror("mprotect");
    return -1;
  }
  return 0;
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


// Allocates RW memory, emits the code into it and sets it to RX before
// executing.
void emit_to_rw_run_from_rx() {
  void* m = alloc_writable_memory(SIZE);
  emit_code_into_memory(m);
  make_memory_executable(m, SIZE);

  JittedFunc func = m;
  int result = func(2);
  printf("result = %d\n", result);
}

int main(int argc, char** argv) {
  emit_to_rw_run_from_rx(); //error! - illegal hardware instruction. arch x86_64 말고 윈도우에서 하면 돌아갈 듯?
  return 0;
}
