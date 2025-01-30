#ifndef RWLOCK_H
#define RWLOCK_H

#include <stdatomic.h>
#include "my_mutex.h"
#include "my_condvar.h"


typedef struct {
    KirbyMutex lock;              // protects internal state
    CondVar readers_cv;      // for reader threads
    CondVar writers_cv;      // for writer threads
    atomic_int readers;      // active readers count
    atomic_bool writing;     // is writer active?
    atomic_int waiting_writers;  // waiting writers count
    atomic_int waiting_readers;  // waiting readers count
    // 디버깅용 추가
    // atomic_bool debug;
} KirbyRWLock;

// Q. what is KirbyRWLock?
// A. read/write 작업 특화 concurrency control
//    왜? 
//    1. read는 여러 쓰레드가 동시 접근 가능. 
//    2. write는 한번에 하나의 쓰레드만 접근가능.

// 사용 예시
// KirbyRWLock rwlock;

// 읽기 쓰레드 (여러 개 동시 실행 가능)
// rwlock_read_lock(&rwlock);
// read_data();         // 여러 쓰레드가 동시에 읽기 가능
// rwlock_read_unlock(&rwlock);

// 쓰기 쓰레드 (exclusive access)
// rwlock_write_lock(&rwlock);
// write_data();        // 다른 모든 쓰레드 block
// rwlock_write_unlock(&rwlock);

void rwlock_init(KirbyRWLock* rw);
void rwlock_read_lock(KirbyRWLock* rw);
void rwlock_read_unlock(KirbyRWLock* rw);
void rwlock_write_lock(KirbyRWLock* rw);
void rwlock_write_unlock(KirbyRWLock* rw);
void rwlock_destroy(KirbyRWLock* rw);


#endif // RWLOCK_H
