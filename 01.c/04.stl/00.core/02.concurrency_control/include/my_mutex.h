#ifndef MUTEX_H
#define MUTEX_H

#include <stdbool.h>
#include <stdatomic.h> //atomic_flag type을 위해 쓴다.
#include <pthread.h>

typedef struct {
    atomic_flag lock; //atomic flag for spinlock
    pthread_t owner;  //thread_id for current owner thread
    int count;        //recursion count for recursive mutex
} KirbyMutex;

// 사용 예시
// Mutex lock;
// mutex_lock(&lock);    // 다른 쓰레드는 여기서 block
// critial_section();    // 한 번에 하나의 쓰레드만 실행 가능
// mutex_unlock(&lock);  // 다른 쓰레드가 진입 가능

void mutex_init(KirbyMutex* mutex);
bool mutex_lock(KirbyMutex* mutex); // 다른 쓰레드는 여기서 block
bool mutex_unlock(KirbyMutex* mutex);
void mutex_destroy(KirbyMutex* mutex);

#endif
