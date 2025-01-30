#include "my_rwlock.h"
#include <stdio.h>
#include <unistd.h> //for usleep();

void rwlock_init(KirbyRWLock* rw) {
    mutex_init(&rw->lock);
    condvar_init(&rw->readers_cv);
    condvar_init(&rw->writers_cv);
    atomic_init(&rw->readers, 0);
    atomic_init(&rw->writing, false);
    atomic_init(&rw->waiting_writers, 0);
    atomic_init(&rw->waiting_readers, 0);
}

//Q. 굳금한게 있는데, read면 여러 쓰레드가 읽는걸 허용한다면, mutex_lock()은 왜 거는거야? 그냥 읽게 하면 되잖아?
//A. 아마 해당 리소스를 다른 쓰레드가 write하는 상태일 때 read 하려고 접근하는 여러 쓰레드를 막기 위함인 듯?
void rwlock_read_lock(KirbyRWLock* rw) {
    mutex_lock(&rw->lock);
    
    // Increment waiting readers
    atomic_fetch_add(&rw->waiting_readers, 1);

    // if (rw->debug) printf("Reader waiting. Writers: %d\n", 
    //     atomic_load(&rw->waiting_writers));
    
    // Wait if there's a writer or waiting writers (writer preference)
    while (atomic_load(&rw->writing) || atomic_load(&rw->waiting_writers) > 0) {
        condvar_wait(&rw->readers_cv, &rw->lock);
    }
    
    // Got the lock - decrease waiting and increase active readers
    atomic_fetch_sub(&rw->waiting_readers, 1);
    atomic_fetch_add(&rw->readers, 1);

    // if (rw->debug) printf("Reader acquired lock. Active readers: %d\n", 
    //     atomic_load(&rw->readers));

    mutex_unlock(&rw->lock);
}

void rwlock_read_unlock(KirbyRWLock* rw) {
    mutex_lock(&rw->lock);
    
    // Decrease active readers
    int remaining = atomic_fetch_sub(&rw->readers, 1) - 1;
    
    // If last reader out and writers waiting, signal one writer
    if (remaining == 0 && atomic_load(&rw->waiting_writers) > 0) {
        condvar_signal(&rw->writers_cv);
    }
    
    mutex_unlock(&rw->lock);
}

void rwlock_write_lock(KirbyRWLock* rw) {
   mutex_lock(&rw->lock);
   
   // Increment waiting writers
   atomic_fetch_add(&rw->waiting_writers, 1);

    // if (rw->debug) printf("Writer waiting. Readers: %d, Writing: %d\n", 
    // atomic_load(&rw->readers), atomic_load(&rw->writing));

   
   // Wait if there are active readers or another writer
   while (atomic_load(&rw->readers) > 0 || atomic_load(&rw->writing)) {
       condvar_wait(&rw->writers_cv, &rw->lock);
   }
   
   // Got the lock - decrease waiting writers and mark as writing
   atomic_fetch_sub(&rw->waiting_writers, 1);
   atomic_store(&rw->writing, true);

    // if (rw->debug) printf("Writer acquired lock\n");

   
   mutex_unlock(&rw->lock);
}

void rwlock_write_unlock(KirbyRWLock* rw) {
   mutex_lock(&rw->lock);
   
   // No longer writing
   atomic_store(&rw->writing, false);
   
   // Signal based on waiting threads:
   // - If writers waiting, signal one writer (writer preference)
   // - Otherwise, broadcast to all waiting readers
   if (atomic_load(&rw->waiting_writers) > 0) {
       condvar_signal(&rw->writers_cv);
   } else if (atomic_load(&rw->waiting_readers) > 0) {
       condvar_broadcast(&rw->readers_cv);
   }
   
   mutex_unlock(&rw->lock);
}

void rwlock_destroy(KirbyRWLock* rw) {
   mutex_destroy(&rw->lock);
   condvar_destroy(&rw->readers_cv);
   condvar_destroy(&rw->writers_cv);
}
