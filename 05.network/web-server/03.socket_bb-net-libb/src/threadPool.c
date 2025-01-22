#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "threadPool.h"

// Task structure
typedef struct {
    void (*function)(void *);
    void *argument;
} task_T;

// Thread pool structure
typedef struct threadPool_T {
    pthread_t       threads [BB_MAX_THREADS]; //max thread 갯수:8개 만큼 pthread_t의 들의 공간을 마련한다. 
    task_T          tasks   [BB_MAX_TASKS]; //tasks는 총 1000개 공간 만큼 수행 가능하다. task 'queue'다. 이 task 'queue'를 공유자원 삼아 lock & notify 하는 것. 
    pthread_mutex_t lock;   //mutex lock으로 락건다. 
    pthread_cond_t  notify; //lock 풀 때 notify로 알림주는 듯?
    int             task_count;
    int             task_head;
    int             task_tail;
    int             shutdown;
} threadPool_T;

static void *executeTask(void *arg);

int createThreadPool(threadPool_T **pool) 
{
    *pool = (threadPool_T*)calloc(1, sizeof(threadPool_T)); //threadPool_T 구조체의 사이즈 만큼 메모리의 공간을 마련한다.
    if ((*pool) == NULL) {
        return -1;
    }

    pthread_mutex_init (&(*pool)->lock, NULL);
    pthread_cond_init  (&(*pool)->notify, NULL);
    (*pool)->task_count = 0;
    (*pool)->task_head  = 0;
    (*pool)->task_tail  = 0;
    (*pool)->shutdown   = 0;

    for (int i = 0; i < BB_MAX_THREADS; i++) { //thread pool의 8개 만큼 쓰레드 생성한다. 
        pthread_create(&(*pool)->threads[i], NULL, executeTask, (*pool));
    }
    return 0;
}

// Execute the task
static void *executeTask(void *arg) 
{
    threadPool_T *pool = (threadPool_T *)arg; //해당 threadpool을 불러와서 

    while (1) { //exit될 때까지 계속 돈다. 
        pthread_mutex_lock(&pool->lock); //mutex lock 걸고, 

        while (pool->task_count == 0 && !pool->shutdown) {  //task가 쌓인게 없거나 pool이 shutdown 상태면 wait 처리 
            pthread_cond_wait(&pool->notify, &pool->lock);
        }

        if (pool->shutdown) { //에러 핸들링 
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }

        task_T task = pool->tasks[pool->task_head]; //task_T 구조체는 function & argument로 구성되어 있다. 
        pool->task_head = (pool->task_head + 1) % BB_MAX_TASKS; //% 1000을 해줘서 index out of error 피하려는 듯?
        pool->task_count--;

        pthread_mutex_unlock(&pool->lock); //lock 해제 

        // Execute the task
        task.function(task.argument);
    }

    return NULL;
}

// Add a task to the thread pool
void addTaskToThreadPool(threadPool_T *pool, void (*function)(void *), void *argument) 
{
    pthread_mutex_lock  (&pool->lock);

    if (pool->task_count == BB_MAX_TASKS) { //task queue 1000개가 이미 꽉 찬 경우 -> print error & return 
        fprintf              (stderr, "Task queue is full!\n");
        pthread_mutex_unlock (&pool->lock);
        return;
    }

    pool->tasks[pool->task_tail].function = function; //task_tail 은 queue의 마지막 index. 여기에 function & argument 넣는 것 
    pool->tasks[pool->task_tail].argument = argument;

    pool->task_tail = (pool->task_tail + 1) % BB_MAX_TASKS; // % 1000
    pool->task_count++;

    pthread_cond_signal  (&pool->notify);
    pthread_mutex_unlock (&pool->lock);
}

// Shutdown the thread pool
void destroyThreadPool(threadPool_T **pool) 
{
    if ((*pool) == NULL) {
        return;
    }
    pthread_mutex_lock     (&(*pool)->lock);
    (*pool)->shutdown = 1;
    pthread_mutex_unlock   (&(*pool)->lock);
    pthread_cond_broadcast (&(*pool)->notify);

    for (int i = 0; i < BB_MAX_THREADS; i++) {
        pthread_join((*pool)->threads[i], NULL);
    }

    pthread_mutex_destroy (&(*pool)->lock);
    pthread_cond_destroy  (&(*pool)->notify);
    free                  (*pool);
    *pool = NULL;
}
