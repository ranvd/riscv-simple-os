#include "os.h"
#include "user_api.h"

extern int atomic_swap(lock_t *);

int m_basic_lock(){
    w_mstatus(r_mstatus() & ~MSTATUS_MIE);
    return 0;
}
int m_basic_unlock(){
    w_mstatus(r_mstatus() | MSTATUS_MIE);
    return 0;
}

int basic_lock() {
    //disable interrupt
    itr_disable();
    return 0;
}

int basic_unlock() {
    // enable interrupt
    itr_enable();
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