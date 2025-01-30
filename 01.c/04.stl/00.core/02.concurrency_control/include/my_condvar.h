#ifndef CONDVAR_H
#define CONDVAR_H

#include "my_mutex.h"
#include <stdatomic.h>

typedef struct {
    atomic_int waiters;      // Tracks the number of threads waiting on the condition variable. 
    atomic_flag lock;        // An atomic flag used to protect the internal state of the condition variable
    atomic_bool signal;      // A flag indicating whether a signal has been sent
    atomic_int generation;   // A counter used to prevent spurious wakeups (threads waking up without being signaled).
} CondVar;

void condvar_init(CondVar* cv);
void condvar_wait(CondVar* cv, KirbyMutex* mutex);
void condvar_signal(CondVar* cv);
void condvar_broadcast(CondVar* cv);
void condvar_destroy(CondVar* cv);


// typedef struct {
//     atomic_int waiters; //number of waiting threads
//     atomic_bool signal; //signal flag
// } KirbyCondVar;

// Q. What is condition variable?
// A. 쓰레드간 시그널링 메커니즘. 스레드간 이벤트 통신. producer - consumer pattern

// Example)
// 생산자-소비자 패턴 예시
// Mutex mutex;
// CondVar not_empty;
// Queue queue;

// 생산자
// mutex_lock(&mutex);
// queue_add(&queue, item);
// condvar_signal(&not_empty);  // 대기 중인 소비자에게 알림
// mutex_unlock(&mutex);

// 소비자
// mutex_lock(&mutex);
// while (queue_is_empty(&queue)) {
//     condvar_wait(&not_empty, &mutex);  // 큐가 비어있으면 대기
// }
// item = queue_get(&queue);
// mutex_unlock(&mutex);

// Q. 왜 mutex 안쓰고 이거 씀?
// A. 어떤 특정 조건 걸고 싶을 떄 씀.

// void condvar_init(KirbyCondVar* cv);
// void condvar_wait(KirbyCondVar* cv, KirbyMutex* mutex);
// void condvar_signal(KirbyCondVar* cv);
// void condvar_broadcast(KirbyCondVar* cv);
// void condvar_destroy(KirbyCondVar* cv);

#endif
