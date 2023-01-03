#include "syscall.h"

.global gethid
gethid:
    li  a7, SYS_gethid
    ecall
    ret

.global itr_disable
itr_disable:
    li  a7, SYS_itr_disable
    ecall
    ret

.global itr_enable
itr_enable:
    li  a7, SYS_itr_enable
    ecall
    ret