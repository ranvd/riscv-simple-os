#include "syscall.h"

#include "os.h"

int sys_gethid(unsigned int *ptr_hid) {
    printf("--> sys_gethid, arg0 = 0x%x\n", ptr_hid);
    if (ptr_hid == NULL) {
        return -1;
    } else {
        *ptr_hid = r_mhartid();
        return 0;
    }
}
void sys_itr_disable(){
    printf("Interrupt Disable\n");
    w_mstatus(r_mstatus() & ~MSTATUS_MIE);
    return;
}
void sys_itr_enable(){
    printf("Interrupt Enable\n");
    w_mstatus(r_mstatus() | MSTATUS_MIE);
    return;
}

void do_syscall(struct context *cxt) {
    uint32_t syscall_num = cxt->a7;

    switch (syscall_num) {
        case SYS_gethid:
            cxt->a0 = sys_gethid((unsigned int *)(cxt->a0));
            break;
        case SYS_itr_disable:
            sys_itr_disable();
            break;
        case SYS_itr_enable:
            sys_itr_enable();
            break;
        default:
            printf("Unknown syscall no: %d\n", syscall_num);
            cxt->a0 = -1;
    }

    return;
}