extern void uart_init(void);
extern void uart_puts(char *s);
extern char uart_getc();
extern int uart_putc(char);

void start_kernel(void) {
    uart_init();
    uart_puts("Hello, RVOS!\n");
    while (1) {
		char a = uart_getc();
        uart_puts(&a);
    }; // stop here!
}