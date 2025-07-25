#include "os.h"

extern void trap_vector(void);
extern void uart_isr(void);
extern void timer_handler(void);
extern void do_syscall(struct context *cxt);

void trap_init() {
    /*
     * set the trap-vector base-address for machine-mode
     */
    w_mtvec((reg_t)trap_vector);
}

void software_interrupt_init() {
    /* enable machine-mode software interrupts. */
    w_mie(r_mie() | MIE_MSIE);
}

void external_interrupt_handler() {
    int irq = plic_claim();

    if (irq == UART0_IRQ) {
        uart_isr();
    } else if (irq) {
        printf("unexpected interrupt irq = %d\n", irq);
    }

    if (irq) {
        plic_complete(irq);
    }
}

void software_interrupt_handler() {
    /*
     * acknowledge the software interrupt by clearing
     * the MSIP bit in mip.
     */
    int id = r_mhartid();
    *(uint32_t *)CLINT_MSIP(id) = 0;
}

void software_interrupt() {
    /* trigger a machine-level software interrupt */
    int id = r_mhartid();
    *(uint32_t *)CLINT_MSIP(id) = 1;
}

reg_t trap_handler(reg_t epc, reg_t cause, struct context *cxt, reg_t mtval) {
    reg_t return_pc = epc;
    reg_t cause_code = cause & 0xfff;
    if (cause & 0x80000000) {
        /* Asynchronous trap - interrupt */
        switch (cause_code) {
            case 3:
                uart_puts("software interruption!\n");
                software_interrupt_handler();
                break;
            case 7:
                uart_puts("timer interruption!\n");
                timer_handler();
                break;
            case 11:
                uart_puts("external interruption!\n");
                external_interrupt_handler();
                break;
            default:
                uart_puts("unknown async exception!\n");
                break;
        }
    } else {
        /* Synchronous trap - exception */
        printf("Sync exceptions!, code = %d, mtval = %d\n", cause_code, mtval);
        switch (cause_code) {
            case 8:
                printf("System call from U-mode!: system call no: %d\n",cxt->a7);
                do_syscall(cxt);
                return_pc += 4;
                break;

            default:
                panic("OOPS! What can I do!");
                break;
        }
        // return_pc += 4;
    }

    return return_pc;
}

void trap_test() {
    /*
     * Synchronous exception code = 7
     * Store/AMO access fault
     */
    *(int *)0x00000000 = 100;

    /*
     * Synchronous exception code = 5
     * Load access fault
     */
    // int a = *(int *)0x00000000;

    uart_puts("Yeah! I'm return back from trap!\n");
}
