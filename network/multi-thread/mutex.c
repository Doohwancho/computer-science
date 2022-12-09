#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BIG 1000000000UL
#define HALF_BIG (BIG / 2)

uint32_t counter = 0;
pthread_mutex_t mutex;

void * add_1(void *arg)
{
    pthread_mutex_lock(&mutex); // case3) only
    for(uint32_t i = 0; i < HALF_BIG; i++) { //BIG -> case1,2 / HALF_BIG -> case3
        counter += 1;
    }
    pthread_mutex_unlock(&mutex); //case3) only
    return NULL;
}

int main()
{
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    //case1) single thread
    //counter = 1000000000Time taken: 2.105982
    /* add_1(NULL); */

    //case2) multi-thread
    //counter = 1003840364Time taken: 4.384060
    //wrong answer, slower time. even though it is multi-thread.

    //Q1. why wrong answer?

    //i++ is not atomic operation.
    //atomic class쓰거나 lock 써서 해결하는게 좋은데, atomic class는 프로세스랑 컴파일러마나 달라서 불안정하다고 비추한다네.
    //이번 예제에서는 mutex를 써서 해결

    //Q2. why slower time?

    //쓰레드 생성 기간 + 쓰레드간의 context switching 기간 때문에 느림.

    /* pthread_t t; */
    /* pthread_create(&t, NULL, add_1, NULL); */
    /* add_1(NULL); */
    /* pthread_join(t, NULL); */


    //case3) mutex
    //counter = 1000000000Time taken: 2.132229
    //correct answer, faster time. even though it is multi-thread.
    pthread_t thread1, thread2;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread1, NULL, add_1, NULL);
    pthread_create(&thread2, NULL, add_1, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("counter = %d", counter);
    printf("Time taken: %f\n", cpu_time_used);

    pthread_mutex_destroy(&mutex);

    return 0;
}
