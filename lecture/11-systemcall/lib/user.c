#include "os.h"
#include "user_api.h"
#define DELAY 2000

extern void trap_test(void);

void time_fun(void *arg){
	uint32_t tick = get_tick();
	printf("%2d:%2d:%2d\n", 0, tick / 60, tick % 60);
	return;
}

void user_task0(void)
{
	uart_puts("Task 0: Created!\n");

	unsigned int hid = -1;
#ifdef CONFIG_SYSCALL
	int ret = -1;
	ret = gethid(&hid);
	//ret = gethid(NULL);
	if (!ret){
		printf("system call returned!, hart id is %d\n", hid);
	} else {
		printf("gethid() failed, return: %d\n", ret);
	}
#endif
	while (1) {
		uart_puts("Task 0: Running...\n");
		task_delay(DELAY);
	}
}

void user_task1(void)
{
	// int c = 0;
	if (timer_create(time_fun, NULL, 3)){
		printf("timer_create() faile: timer 3\n");
	}
	if (timer_create(time_fun, NULL, 4)){
		printf("timer_create() faile: timer 4\n");
	}
	if (timer_create(time_fun, NULL, 5)){
		printf("timer_create() faile: timer 5\n");
	}
	uart_puts("Task 1: Created!\n");
	while (1) {
		uart_puts("Task 1: Running...\n");
		
		// software_interrupt();
		task_delay(DELAY);
	}
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{
	task_create(user_task0, NULL, 101);
	task_create(user_task1, NULL, 100);
}

