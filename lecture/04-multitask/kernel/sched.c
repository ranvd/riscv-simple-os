#include "os.h"
#include "heap.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

#define MAX_TASKS 10
#define STACK_SIZE 1024
uint8_t task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];
struct Arr_Heap prior_q;
struct context *curr_ctx = NULL;

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 */
static int _top = 0;

static void w_mscratch(reg_t x)
{
	/* 把 reg_t 的值寫到 mscratch 暫存器裡面 */
	asm volatile("csrw mscratch, %0" : : "r" (x));
}

static int _compare(void *a, void *b){
	return ((context*)a)->priority > ((context*)b)->priority;
}

void sched_init()
{
	w_mscratch(0);
	for (int i= 0;i < MAX_TASKS; i++){
		ctx_tasks[i].inused = 0;
	}
	arr_heap_init(&prior_q, ctx_tasks, sizeof(context), _top, MAX_TASKS, _compare);
}

/*
 * implment a simple cycle FIFO schedular
 */
void schedule()
{
	if (prior_q.blk_top <= 0 && !curr_ctx->inused ){
		panic("Num of task should be greater than zero");
		return;
	}
	// printf("priority: %d\n", next->priority);
	if(curr_ctx && curr_ctx->inused)
		heap_push(&prior_q, curr_ctx);
	curr_ctx = (context*)heap_get(prior_q);
	printf("id: %d\n", curr_ctx->id);
	heap_pop(&prior_q);
	switch_to(curr_ctx);
}

int task_create(void (*start_routin)(void), void *param, uint8_t priority){
	for (int i = 0; i<MAX_TASKS; i++){
		if (!ctx_tasks[i].inused){
			ctx_tasks[i].inused=1;
			_top = i;
			break;
		}
	}
	if (_top < MAX_TASKS) {
		ctx_tasks[_top].sp = (reg_t) &task_stack[_top][STACK_SIZE - 1];
		ctx_tasks[_top].ra = (reg_t) start_routin;
		ctx_tasks[_top].param = param;
		ctx_tasks[_top].priority = priority;
		ctx_tasks[_top].id = _top;
		heap_push(&prior_q, &ctx_tasks[_top]);
		return 0;
	} else {
		return -1;
	}
}

void task_exit(){
	ctx_tasks[curr_ctx->id].inused = 0;
	printf("Exitting...\n");
	schedule();
	return;
}

void task_yield(){
	schedule();
}
/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}
