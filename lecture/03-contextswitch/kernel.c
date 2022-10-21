#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */
extern void uart_init(void);
extern void page_init(void);
extern void sched_init(void);
extern void schedule(void);

void start_kernel(void) {
    uart_init();
    uart_puts("Hello, RVOS!\n");

    page_init();

    sched_init();
    schedule();

    printf("do not show here\n");
    int *a = malloc(10);
    printf("malloc addr: %x\n", a);
    
    while (1) {
		char a = uart_getc();
        uart_puts(&a);
    }; // stop here!
}