#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

/* Thread pool implementation */

/* Define task structure */
typedef struct task {
    void (*function)(void *);  /* Function to execute */
    void *argument;            /* Function argument */
    struct task *next;         /* Next task in queue */
    int is_heavy;              /* Flag for heavy tasks */
} task_t;

/* Define thread pool structure */
typedef struct threadpool {
    pthread_mutex_t lock;          /* Mutex for task queue access */
    pthread_cond_t notify;         /* Condition variable for signaling */
    pthread_t *threads;            /* Array of worker threads */
    task_t *queue_head;            /* Head of task queue */
    task_t *queue_tail;            /* Tail of task queue */
    int thread_count;              /* Number of threads */
    int queue_size;                /* Number of pending tasks */
    atomic_int tasks_processed;    /* Total tasks processed */
    int shutdown;                  /* Shutdown flag */
} threadpool_t;

/* Forward declarations */
threadpool_t *threadpool_create(int thread_count);
int threadpool_add(threadpool_t *pool, void (*function)(void *), void *argument, int is_heavy);
int threadpool_destroy(threadpool_t *pool);
static void *threadpool_worker(void *threadpool);

/* Create a new thread pool */
threadpool_t *threadpool_create(int thread_count) {
    if (thread_count <= 0) thread_count = 4; /* Default to 4 threads */
    
    /* Allocate pool structure */
    threadpool_t *pool = (threadpool_t *)malloc(sizeof(threadpool_t));
    if (pool == NULL) {
        perror("malloc");
        return NULL;
    }
    
    /* Initialize pool attributes */
    pool->thread_count = thread_count;
    pool->queue_size = 0;
    atomic_init(&pool->tasks_processed, 0);
    pool->queue_head = NULL;
    pool->queue_tail = NULL;
    pool->shutdown = 0;
    
    /* Initialize mutex and condition variable */
    if (pthread_mutex_init(&pool->lock, NULL) != 0 ||
        pthread_cond_init(&pool->notify, NULL) != 0) {
        free(pool);
        return NULL;
    }
    
    /* Allocate and create threads */
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    if (pool->threads == NULL) {
        pthread_mutex_destroy(&pool->lock);
        pthread_cond_destroy(&pool->notify);
        free(pool);
        return NULL;
    }
    
    /* Create worker threads */
    for (int i = 0; i < thread_count; i++) {
        if (pthread_create(&pool->threads[i], NULL, threadpool_worker, (void *)pool) != 0) {
            /* If thread creation fails, destroy all created threads */
            threadpool_destroy(pool);
            return NULL;
        }
    }
    
    return pool;
}

/* Add a task to the threadpool */
int threadpool_add(threadpool_t *pool, void (*function)(void *), void *argument, int is_heavy) {
    if (pool == NULL || function == NULL) {
        return -1;
    }
    
    /* Acquire lock */
    if (pthread_mutex_lock(&pool->lock) != 0) {
        return -1;
    }
    
    /* Check if shutdown */
    if (pool->shutdown) {
        pthread_mutex_unlock(&pool->lock);
        return -1;
    }
    
    /* Create task */
    task_t *task = (task_t *)malloc(sizeof(task_t));
    if (task == NULL) {
        pthread_mutex_unlock(&pool->lock);
        return -1;
    }
    
    task->function = function;
    task->argument = argument;
    task->is_heavy = is_heavy;
    task->next = NULL;
    
    /* Add task to queue */
    if (pool->queue_head == NULL) {
        pool->queue_head = task;
    } else {
        pool->queue_tail->next = task;
    }
    pool->queue_tail = task;
    pool->queue_size++;
    
    /* Signal a worker thread */
    if (pthread_cond_signal(&pool->notify) != 0) {
        pthread_mutex_unlock(&pool->lock);
        return -1;
    }
    
    /* Release lock */
    pthread_mutex_unlock(&pool->lock);
    
    return 0;
}

/* Destroy the threadpool */
int threadpool_destroy(threadpool_t *pool) {
    if (pool == NULL) {
        return -1;
    }
    
    /* Acquire lock */
    if (pthread_mutex_lock(&pool->lock) != 0) {
        return -1;
    }
    
    /* Check if already shutting down */
    if (pool->shutdown) {
        pthread_mutex_unlock(&pool->lock);
        return -1;
    }
    
    /* Set shutdown flag */
    pool->shutdown = 1;
    
    /* Wake up all worker threads */
    if (pthread_cond_broadcast(&pool->notify) != 0 ||
        pthread_mutex_unlock(&pool->lock) != 0) {
        return -1;
    }
    
    /* Join all worker threads */
    for (int i = 0; i < pool->thread_count; i++) {
        if (pthread_join(pool->threads[i], NULL) != 0) {
            return -1;
        }
    }
    
    /* Free the task queue */
    task_t *task;
    while (pool->queue_head != NULL) {
        task = pool->queue_head;
        pool->queue_head = pool->queue_head->next;
        free(task);
    }
    
    /* Free resources */
    free(pool->threads);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notify);
    free(pool);
    
    return 0;
}

/* Worker thread function */
static void *threadpool_worker(void *threadpool) {
    threadpool_t *pool = (threadpool_t *)threadpool;
    task_t *task;
    
    while (1) {
        /* Acquire lock */
        pthread_mutex_lock(&pool->lock);
        
        /* Wait for a task if queue is empty and not shutting down */
        while (pool->queue_size == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->notify, &pool->lock);
        }
        
        /* Check for shutdown */
        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }
        
        /* Get a task from the queue */
        task = pool->queue_head;
        if (task == NULL) {
            pthread_mutex_unlock(&pool->lock);
            continue;
        }
        
        /* Remove task from queue */
        pool->queue_head = task->next;
        if (pool->queue_head == NULL) {
            pool->queue_tail = NULL;
        }
        pool->queue_size--;
        
        /* Release lock */
        pthread_mutex_unlock(&pool->lock);
        
        /* Execute task */
        (*(task->function))(task->argument);
        
        /* Log if it's a heavy task */
        if (task->is_heavy) {
            printf("Completed heavy task in thread %lu\n", (unsigned long)pthread_self());
        }
        
        /* Increment counter of processed tasks */
        atomic_fetch_add(&pool->tasks_processed, 1);
        
        /* Free task structure */
        free(task);
    }
    
    return NULL;
}

/* Get number of tasks processed */
int threadpool_get_tasks_processed(threadpool_t *pool) {
    if (pool == NULL) return 0;
    return atomic_load(&pool->tasks_processed);
}

/* Get current queue size */
int threadpool_get_queue_size(threadpool_t *pool) {
    if (pool == NULL) return 0;
    
    pthread_mutex_lock(&pool->lock);
    int size = pool->queue_size;
    pthread_mutex_unlock(&pool->lock);
    
    return size;
}
