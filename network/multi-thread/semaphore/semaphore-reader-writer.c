#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

/*
    Q. Status Quo

    DB에서 같은 메모리에 동시에 여러 프로세스가 write && read 하는 경우 어떻게 해결?

    여러 프로세스가 동시에 read하는건 괜찮은데, write하는 경우에는 문제가 생긴다.
    writer가 write하는 동안 reader가 read하면 안된다. (write하고 commit하기 직전 read한다던가..)

    reader가 read하는 동안 writer가 write해도 안된다.



    Q. How to solve?

    read를 mutex lock 걸고,

    read 하는 동안 binary-semaphore에 -1해줘서 0으로 만들어, writer가 write할 수 없게 한다.


    Q. 실제 봤는데

    물론 read, write 자체는 mutex, semaphore lock 걸어서 해결했긴 했는데,

    뭔가 reader들이cnt가 1일 때, 먼저 다 read하고,
    다 끝나면 writer가 드가는 식으로 될 줄 알았는데,
    여러번 돌려보면 writer가 reader들 보다 먼저 실행되는 경우도 있다. (100% 완벽하지 않다)

    애초에 writer를 늦게 실행해서 reader 10개가 먼저 mutex실행하고 반환할 때 까지 writer들은 아직 initialized 안됬겠지? 라는 안일한 마인드로 짜진 코드 같다.



*/

sem_t wrt;
pthread_mutex_t mutex;
int cnt = 1; //both reader and writer want access to the same memory location
int numreader = 0; //use numreader to help mutex. 0 | 1

void *writer(void *wno)
{
    sem_wait(&wrt); //write 도중 lock
    cnt = cnt*2;
    printf("Writer %d modified cnt to %d\n",(*((int *)wno)),cnt);
    sem_post(&wrt); //write 끝나면 unlock
    return NULL;
}
void *reader(void *rno)
{
    pthread_mutex_lock(&mutex); //first thread goes in, other 9 threads will wait on that line until the first thread releases the lock. They will not be suspended into kernel space.
    numreader++;
    cnt++;
    if(numreader == 1) {
        sem_wait(&wrt); // If this id the first reader, then it will block the writer. wait is semaphore -1
    }
    pthread_mutex_unlock(&mutex);



    // Reading Section
    printf("Reader %d: read cnt as %d\n",*((int *)rno),cnt);




    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        sem_post(&wrt); // If this is the last reader, it will wake up the writer. post is seemaphore + 1
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{

    pthread_t read[10],write[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1); //binary semaphore. (semaphore obj, initial value, max value)

    int a[10] = {1,2,3,4,5,6,7,8,9,10}; //Just used for numbering the producer and consumer


    for(int i = 0; i < 10; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(read[i], NULL); //join waits for thread to finish
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;

}


// console.log 1

//cnt++ in reader 안하면 고르게 나오네?

/* Reader 1: read cnt as 1 */
/* Reader 2: read cnt as 1 */
/* Reader 4: read cnt as 1 */
/* Reader 3: read cnt as 1 */
/* Reader 6: read cnt as 1 */
/* Reader 5: read cnt as 1 */
/* Reader 7: read cnt as 1 */
/* Reader 8: read cnt as 1 */
/* Reader 9: read cnt as 1 */
/* Reader 10: read cnt as 1 */
/* Writer 1 modified cnt to 2 */
/* Writer 2 modified cnt to 4 */
/* Writer 3 modified cnt to 8 */
/* Writer 4 modified cnt to 16 */
/* Writer 5 modified cnt to 32 */



//console.log 2

//cnt++ in reader하면 reader가 좀 더 느려져서 후발주자인 writer가 침범하네?
//이 방법도 reader 먼저 read다 끝나고 write하게 보장하진 않는구나.

/* Reader 1: read cnt as 2 */
/* Reader 2: read cnt as 3 */
/* Reader 6: read cnt as 6 */
/* Reader 5: read cnt as 7 */
/* Reader 8: read cnt as 8 */
/* Reader 4: read cnt as 5 */
/* Reader 7: read cnt as 9 */
/* Reader 10: read cnt as 11 */
/* Writer 2 modified cnt to 44 */
/* Writer 4 modified cnt to 88 */
/* Writer 1 modified cnt to 22 */
/* Writer 3 modified cnt to 176 */
/* Reader 3: read cnt as 4 */
/* Writer 5 modified cnt to 352 */
/* Reader 9: read cnt as 10 */


//writer들이 write으로 값을 바꾸기 전에, reader가 공유자원 count의 값 1을 읽은걸 확인 가능하다.
//reader다 다 읽고 그제서야 writer가 쓰는걸 볼 수 있다.
