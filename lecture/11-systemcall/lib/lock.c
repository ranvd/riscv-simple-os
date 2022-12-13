#include "os.h"

extern int atomic_swap(lock_t *);

int basic_lock() {
    //disable interrupt
    w_mstatus(r_mstatus() & ~MSTATUS_MIE);
    return 0;
}

int basic_unlock() {
    // enable interrupt
    w_mstatus(r_mstatus() | MSTATUS_MIE);
    return 0;
}

void lock_init(lock_t *lock) { lock->locked = 0; }

void lock_acquire(lock_t *lock) {
    for (;;) {
        if (!atomic_swap(lock)) {
            break;
        }
    }
}

void lock_free(lock_t *lock) { lock->locked = 0; }