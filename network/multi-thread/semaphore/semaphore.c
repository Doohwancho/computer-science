#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

/*

   Q. Status Quo
   1. The producer and consumer share a fixed-size buffer used as a queue.
   2. The producer’s job is to generate data and put this in the buffer.
   3. The consumer’s job is to consume the data from this buffer, one at a time.


   Q. Problem

   1. How do you make sure that producer doesn’t try to put data in buffer when the buffer is full
   2. consumer doesn’t try to consumer data when the buffer is empty?


   Q. Why?

   A. When producer tries to put data into the buffer when it is full, it wastes cpu cycles. The same is true for consumer it tries to consumer from an empty buffer. It’s better that they go on sleep in these cases so that the scheduler can schedule another process.


   Q. How to solve problem?

   A. use mutex & semaphore


   Q.

*/

#define MaxItems 5 // Maximum items a producer can produce or a consumer can consume
#define BufferSize 5 // Size of the buffer

sem_t empty;
sem_t full;
int in = 0; //index at which producer will put the next data
int out = 0; // index from which the consumer will consume next data
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno)
{
    int item;
    for(int i = 0; i < MaxItems; i++) {
        item = rand(); // Produce an random item

        //critical section using mutex
        sem_wait(&empty); //empty semaphore에서 -1
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in);
        in = (in+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        //critical section using mutex

        sem_post(&full); //일 다 끝나면 full semaphore에 +1. 이제 consumer가 깨어남
    }
}
void *consumer(void *cno)
{
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&full); //full semaphore에서 -1

        //critical section using mutex
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n",*((int *)cno),item, out);
        out = (out+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        //critical section using mutex

        sem_post(&empty); //일 다 끝나면 empty semaphore에 +1. 이제 producer가 깨어남
    }
}

int main()
{

    pthread_t pro[5],con[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize); //producer가 쓸 semiphore은 초기 사이즈를 buffer size로만듬.
    sem_init(&full,0,0); //consumer가 쓸 semiphore은 producer가 반납하면 쓸꺼니까 초기값 0

    int a[5] = {1,2,3,4,5}; //Just used for numbering the producer and consumer

    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL); //wait for thread termination
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;

}
