#ifndef BB_THREAD_POOL
#define BB_THREAD_POOL

#define BB_MAX_THREADS 8 //mac에서는 cpu core 갯수 확인을 `sysctl hw.ncpu` 명령어로 한다. 
#define BB_MAX_TASKS   1000

typedef struct threadPool_T *threadPool;

int  createThreadPool    (threadPool *pool);
void addTaskToThreadPool (threadPool pool, 
                          void (*function)(void *), 
                          void *argument);
void destroyThreadPool   (threadPool *pool); 


#endif
