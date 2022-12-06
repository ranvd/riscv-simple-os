#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "riscv.h"
#include "platform.h"

#include <stddef.h>
#include <stdarg.h>

/* uart */
extern int uart_putc(char ch);
extern void uart_puts(char *s);
extern int uart_getc();

/* printf */
extern int  printf(const char* s, ...);
extern void panic(char *s);

/* memory management */
extern void *page_alloc(int npages);
extern void page_free(void *p);
extern void *malloc(size_t size);

/* task management */
typedef struct context {
	/* ignore x0 */
	reg_t ra;
	reg_t sp;
	reg_t gp;
	reg_t tp;
	reg_t t0;
	reg_t t1;
	reg_t t2;
	reg_t s0;
	reg_t s1;
	reg_t a0;
	reg_t a1;
	reg_t a2;
	reg_t a3;
	reg_t a4;
	reg_t a5;
	reg_t a6;
	reg_t a7;
	reg_t s2;
	reg_t s3;
	reg_t s4;
	reg_t s5;
	reg_t s6;
	reg_t s7;
	reg_t s8;
	reg_t s9;
	reg_t s10;
	reg_t s11;
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;

	reg_t pc;
	
	void *param;
	uint8_t priority;
	uint8_t id;
	uint8_t inused;
} context;

extern int  task_create(void (*task)(void), void *param, uint8_t priority);
// extern void task_exit(void);
extern void task_delay(volatile int count);
// extern void task_yield();

/* plic */
extern int plic_claim(void);
extern void plic_complete(int irq);

/* software interrupt */
extern void software_interrupt();

/* lock */
typedef struct lock
{
  volatile int locked;
} lock_t;

extern int basic_lock();
extern int basic_unlock();
extern void lock_init(lock_t *lock);
extern void lock_acquire(lock_t *lock);
extern void lock_free(lock_t *lock);

/* software timer */
struct timer{
	void (*func) (void *arg);
	void *arg;
	uint32_t timeout_tick; 
};
extern struct timer *timer_create(void (*handler)(void *arg), void *arg, uint32_t timeout);
extern void timer_delete(struct timer *timer);
extern inline uint32_t get_tick();

#endif /* __OS_H__ */
