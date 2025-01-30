#include "my_condvar.h"
#include <unistd.h> //for usleep();


void condvar_init(CondVar* cv) {
    atomic_init(&cv->waiters, 0);
    atomic_flag_clear(&cv->lock);
    atomic_init(&cv->signal, false);
    atomic_init(&cv->generation, 0);
}

void condvar_wait(CondVar* cv, KirbyMutex* mutex) {
    atomic_fetch_add(&cv->waiters, 1); //increment waiter's count
    
    // Get current generation
    int my_generation = atomic_load(&cv->generation);
    
    // Release mutex and wait
    mutex_unlock(mutex);
    
    bool keep_waiting = true;
    
    //wait loop, until it gets signal 
    while (keep_waiting) { 
        // Check if signaled, if not, keep wait.
        if (atomic_load(&cv->signal)) {
            // Try to acquire the internal lock
            while (atomic_flag_test_and_set(&cv->lock)) {
                for (int i = 0; i < 100; i++) {
                    __asm__ volatile("" : : : "memory"); //Q. ? 이 코드가 뭐야? A. compiler barrier
                }
            }
            
            // Check if this signal was for us
            if (atomic_load(&cv->generation) != my_generation) {
                keep_waiting = false;
            }
            
            atomic_flag_clear(&cv->lock);
        }
        
        if (keep_waiting) {
            // Exponential backoff
            for (int i = 0; i < 1000; i++) {
                __asm__ volatile("" : : : "memory");
            }
        }
    }
    
    // Reacquire mutex before returning
    mutex_lock(mutex);
    
    atomic_fetch_sub(&cv->waiters, 1); //decrement waiter's count
}


//condvar_wait() 에서 waiting하고 있는 thread한테 signal 보내는 함수. 받으면 들어감.
void condvar_signal(CondVar* cv) {
    // No waiters? Don't bother
    if (atomic_load(&cv->waiters) == 0) {
        return;
    }
    
    // Acquire internal lock
    while (atomic_flag_test_and_set(&cv->lock)) {
        for (int i = 0; i < 100; i++) {
            __asm__ volatile("" : : : "memory"); //block compiler optimization
        }
    }
    
    // Signal and increment generation
    atomic_store(&cv->signal, true);
    atomic_fetch_add(&cv->generation, 1);
    
    atomic_flag_clear(&cv->lock);
}

//condvar_signal은 waiting하는 쓰레드 를 하나만 시그널 보내는거라면, 얘는 전체한테 다 보냄 
void condvar_broadcast(CondVar* cv) {
    // No waiters? Don't bother
    if (atomic_load(&cv->waiters) == 0) {
        return;
    }
    
    // Acquire internal lock
    while (atomic_flag_test_and_set(&cv->lock)) {
        for (int i = 0; i < 100; i++) {
            __asm__ volatile("" : : : "memory");
        }
    }
    
    // Signal and increment generation
    atomic_store(&cv->signal, true);
    atomic_fetch_add(&cv->generation, atomic_load(&cv->waiters)); // Q. Increment by number of waiters??? why?
    
    atomic_flag_clear(&cv->lock);
}

void condvar_destroy(CondVar* cv) {
    atomic_store(&cv->waiters, 0);
    atomic_store(&cv->signal, false);
}



// void condvar_init(KirbyCondVar* cv) {
//     atomic_init(&cv->waiters, 0);
//     atomic_init(&cv->signal, false);
// }

// void condvar_wait(KirbyCondVar* cv, KirbyMutex* mutex) {
//     atomic_fetch_add(&cv->waiters, 1);
    
//     mutex_unlock(mutex);
    
//     while (!atomic_load(&cv->signal)) {
//         usleep(1000);  // 1ms 대기, from #include <unistd.h>
//     }
    
//     mutex_lock(mutex);
//     atomic_fetch_sub(&cv->waiters, 1);
// }

// void condvar_signal(KirbyCondVar* cv) {
//     if (atomic_load(&cv->waiters) > 0) {
//         atomic_store(&cv->signal, true);
//         usleep(1000);  // waiter가 깨어날 시간 제공
//         atomic_store(&cv->signal, false);
//     }
// }

// void condvar_broadcast(KirbyCondVar* cv) {
//     if (atomic_load(&cv->waiters) > 0) {
//         atomic_store(&cv->signal, true);
//         while (atomic_load(&cv->waiters) > 0) {
//             usleep(1000); // 1ms 대기, from #include <unistd.h>
//         }
//         atomic_store(&cv->signal, false);
//     }
// }

// void condvar_destroy(KirbyCondVar* cv) {
//     atomic_store(&cv->waiters, 0);
//     atomic_store(&cv->signal, false);
// }
