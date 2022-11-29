#include "os.h"

int ioff_spin_lock() {
    w_mstatus(r_mstatus() & ~MSTATUS_MIE);
    return 0;
}

int ioff_spin_unlock() {
    w_mstatus(r_mstatus() | MSTATUS_MIE);
    return 0;
}

int spin_lock() { return ioff_spin_lock(); }

int spin_unlock() { return ioff_spin_unlock(); }