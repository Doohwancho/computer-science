#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#define BUF_SIZE 3

int buffer[BUF_SIZE]; // shared buffer
int add = 0; // index to add next element
int rem = 0; // index to remove next element
int num = 0; // number of elements in buffer
int number_of_production = 5;
int stop_iteration = 1; // this will stop iteration of consumer thread

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex lock for buffer
pthread_cond_t c_consumer = PTHREAD_COND_INITIALIZER; // consumer
pthread_cond_t c_producer = PTHREAD_COND_INITIALIZER; // producer

void *producer(void *parameter) {

    for (int i=1; i<=number_of_production; i++) {

        // locking mutex to prevent race conditions
        pthread_mutex_lock(&mutex);
        {
            // there should be no situations where number of elements are greater than buffer size
            if (num > BUF_SIZE) exit(1);

            // if the buffer is full, wait until some data is consumed
            while (num == BUF_SIZE) {

                // c_producer will wake up when receiving signel
                pthread_cond_wait(&c_producer, &mutex);
            }

            // c_producer producing elements
            buffer[add] = i;
            add = (add + 1) % BUF_SIZE;
            num++;
            sleep(1);
        }
        pthread_mutex_unlock(&mutex);

        // we can send signal to consumer that data has been produced
        pthread_cond_signal(&c_consumer);
        printf("Producer inserted %d, size of the buffer : %d\n", i, num);

    }

    printf("Producer quiting\n");
    fflush(stdout);
    stop_iteration = 0;
    return 0;

}

void *consumer(void* parameter) {

    while (1) {
        int i;
        // we also have to lock the mutex before consuming data
        pthread_mutex_lock(&mutex);
        {

            // there should be no case in which number of elements is below 0
            if (num < 0) {
                exit(1);
            }

            // wait for data produced signal if there are no data to be consumed
            while (num == 0) {
                pthread_cond_wait(&c_consumer, &mutex);
            }

            // consuming
            i = buffer[rem];
            rem = (rem + 1) % BUF_SIZE;
            num--;
            sleep(1);
        }
        pthread_mutex_unlock(&mutex);

        // Now we can send signal to c_producer if it was waiting for data to be consumed
        pthread_cond_signal(&c_producer);
        printf("Consumer consumed value of %d\n", i);
        fflush(stdout);

    }

}

int main(int argc, char* argv[]) {

    pthread_t producer_thread, consumer_thread;
    int i;

    // pthread_create(thread, attribute, function to execute, argument for the function)
    // pthread_create is used to create threads and returns 0 when succeeds
    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0) {
        fprintf(stderr, "Unable to create producer thread\n");
        exit(1);
    }
    else printf("Producer thread(%ld) created\n", syscall(SYS_gettid));

    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0) {
        fprintf(stderr, "Unable to create consumer thread");
    }
    else printf("Consumer thread(%ld) created\n", syscall(SYS_gettid));

    // This will block the parent thread to wait until producer_thread and consumter_thread ends
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);


    return 0;
}

/*
console.log

Producer thread(-1) created
Consumer thread(-1) created
Producer inserted 1, size of the buffer : 1
Consumer consumed value of 1
Producer inserted 2, size of the buffer : 1
Consumer consumed value of 2
Producer inserted 3, size of the buffer : 1
Consumer consumed value of 3
Producer inserted 4, size of the buffer : 1
Consumer consumed value of 4
Producer inserted 5, size of the buffer : 1
Producer quiting
Consumer consumed value of 5

*/
