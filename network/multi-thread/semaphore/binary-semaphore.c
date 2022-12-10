#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>


/*
 * binary semaphore works like a mutex
 * it can be locked or unlocked (0 or 1)
 * it is used to protect a critical section
 * it is used to synchronize threads
 *
 * sem_init() initializes the semaphore (1)
 * sem_wait() locks the semaphore (1-1 = 0)
 * sem_post() unlocks the semaphore (0+1 = 1)
 *
 * sem_wait() and sem_post() are atomic **
 *
 * Q. if it works as a mutex, why not use a mutex?
 *
 * 1. lightweight than mutex -> suspended thread에 wakeup call 보내는건 user space -> kernel space 전환해야하는데, 이 때, 비용이 크다.
 * 2. more flexible than mutex
 * -> as they can be used for multiple threads and multiple processes, whereas a mutex is generally used for only one thread or process.
 *
 *
 * Q. why we should not use semaphore?
 *
 * 1. sem_wait() and sem_post() are atomic -> atomic operation is slow
 * 2.
 */


/************************************************/
/*                  Semaphore                   */
/************************************************/

int count = 0;


// Declaring semaphore
sem_t sem;

// Function to Initialize the Semaphore
void sem_init_1()
{
	sem_init(&sem, 0, 1); //binary semaphore!
                          //0 means semaphore is local
                          //1 means semaphore is initialized to 1
                          //semaphore is initialized to 1
                          //so that the first process can enter the critical section
                          //and the second process has to wait until the first process leaves the critical section
                          //and the semaphore is set to 1
                          //so that the second process can enter the critical section
                          //and the first process has to wait until the second process leaves the critical section
}

// Function to acquire the semaphore
void sem_wait_1()
{
	// Acquiring the Semaphore
	sem_wait(&sem);
}

// Function to release the semaphore
void sem_post_1()
{
	// Release the semaphore
	sem_post(&sem);
}

// Function to destroy the semaphore
void sem_destroy_1()
{
	// Destroy the semaphore
	sem_destroy(&sem);
}


/************************************************/
/*                  Therads                     */
/************************************************/


void* thread_func1(void* arg)
{
	// Acquire the semaphore
	sem_wait_1();

	// Do some task
    count++;
	printf("\nTask 1 is done\n");

	// Release the semaphore
	sem_post_1();
}

void* thread_func2(void* arg)
{
	// Acquire the semaphore
	sem_wait_1();

	// Do some task
    count++;
	printf("\nTask 2 is done\n");

	// Release the semaphore
	sem_post_1();
}

void* thread_func3(void* arg)
{
	// Acquire the semaphore
	sem_wait_1();

	// Do some task
	count++;
    printf("\nTask 3 is done\n");

	// Release the semaphore
	sem_post_1();
}



/************************************************/
/*                  Main                        */
/************************************************/

int main()
{
	// Initialize the Semaphore
	sem_init_1();

	// Creating three threads
	pthread_t thread1, thread2, thread3;

	// Creating three threads
	pthread_create(&thread1, NULL, thread_func1, NULL);
	pthread_create(&thread2, NULL, thread_func2, NULL);
	pthread_create(&thread3, NULL, thread_func3, NULL);

	// Wait for the threads to finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	// Destroy the semaphore
	sem_destroy_1();

    printf("\nCount = %d\n", count);

	return 0;
}

