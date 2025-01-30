#include "my_mutex.h"


void mutex_init(KirbyMutex* mutex) {
    atomic_flag_clear(&mutex->lock);
    mutex->owner = 0;
    mutex->count = 0;
}

// 다른 쓰레드는 여기서 block
bool mutex_lock(KirbyMutex* mutex) {
    pthread_t self = pthread_self(); //get the current thread ID

    //재귀적 락 처리 
    if(mutex->owner == self) {
        mutex->count++; //if self, count + 1
        return true;
    }

    //스핀락 구현 (busy-wait spinlock)
    while(atomic_flag_test_and_set(&mutex -> lock)) { //atomic_flag가 ensure that the lock is acquired and released atomically, preventing race conditions.
        //백오프 로직 
        for(int i = 0; i < 1000; i++) {
            __asm__ volatile("" : : : "memory"); //Q. what is this part of code? A. prevent compiler optimization, intending that the spinlock behaves as intended.
            //__asm__: Inline assembly. 
            //volatile: Ensures the compiler does not optimize the instruction. 
            //"": Empty assembly instruction (no operation).
            //: : : "memory": Tells the compiler that memory may be modified, preventing reordering of instructions around the barrier.
        }
    }

    mutex->owner = self; //이 스레드가 먹었다. 
    mutex->count = 1; //reset count to 1 (1 is taken, 0 is not taken)
    
    return true;
}

bool mutex_unlock(KirbyMutex* mutex) {
    if(mutex->owner != pthread_self()) {
        return false; //only the owner can unlock the mutex. if another thread try to unlock, return false.
    }

    mutex->count--; //decrement recursion count. if mutex->count reaches zero, it means it is fully unlocked.

    if(mutex->count == 0) {
        mutex->owner = 0;      //owner is '0', indicating owned by no one
        atomic_flag_clear(&mutex->lock); //release the lock, so other threads can enter.
    }

    return true;
}


void mutex_destroy(KirbyMutex* mutex) {
    mutex->count = 0; //reset recursion count
    mutex->owner = 0; //reset owner
    atomic_flag_clear(&mutex->lock); //clear the lock flag
}
