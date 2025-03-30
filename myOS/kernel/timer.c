#include "os.h"
#include "user_api.h"

extern void schedule(void);

/* interval ~= 1s */
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

/* software timer */
#define MAX_TIMER 10
static uint32_t _tick = 0;
static timer timer_list[MAX_TIMER];
static timer *timer_head=NULL;
static timer *timer_rear=NULL;
//static struct timer timer_list[MAX_TIMER];

/* load timer interval(in ticks) for next timer interrupt.*/
void timer_load(int interval) {
    /* each CPU has a separate source of timer interrupts. */
    int id = r_mhartid();

    *(uint64_t *)CLINT_MTIMECMP(id) = *(uint64_t *)CLINT_MTIME + interval;
}

void timer_init() {;
    struct timer *t = &(timer_list[0]);
    for (int i = 0; i < MAX_TIMER; i++) {
        t->func = NULL;
        t->arg = NULL;
        t->next = NULL;
        t++;
    }
    /*
     * On reset, mtime is cleared to zero, but the mtimecmp registers
     * are not reset. So we have to init the mtimecmp manually.
     */
    timer_load(TIMER_INTERVAL);

    /* enable machine-mode timer interrupts. */
    w_mie(r_mie() | MIE_MTIE);

    /* enable machine-mode global interrupts. */
    w_mstatus(r_mstatus() | MSTATUS_MIE);
    // printf("Timer init ready\n");
}

/* software timer */
struct timer *timer_create(void (*handler)(void *arg), void *arg,
                           uint32_t timeout) {
    /* TBD: params should be checked more, but now we just simplify this */
    if (NULL == handler || 0 == timeout) {
        return NULL;
    }

    /* use lock to protect the shared timer_list between multiple tasks */
    basic_lock();

    struct timer *t = &(timer_list[0]);
    for (int i = 0; i < MAX_TIMER; i++) {
        if (NULL == t->func) {
            break;
        }
        t++;
    }
    if (NULL != t->func) {
        basic_unlock();
        return NULL;
    }
    struct timer **idx = &timer_head;
    while(*idx){    
        if (timeout - (*idx)->timeout_tick <= 0){
            break;
        }
        timeout -= (*idx)->timeout_tick;
        idx = &((*idx)->next);
    }
    t->func = handler;
    t->arg = arg;
    t->timeout_tick = timeout;
    t->next = NULL;
    
    struct timer *tmp = *idx;
    *idx = t;
    if (tmp != NULL){
        tmp->timeout_tick -= timeout;
        (*idx)->next = tmp;
    }

    basic_unlock();

    return t;
}

void timer_delete(struct timer *timer) {
    basic_lock();

    timer->next->timeout_tick -= timer->timeout_tick;
    timer->func = NULL;
    timer->arg = NULL;
    timer->timeout_tick = 0;

    basic_unlock();
}

/* this routine should be called in interrupt context (interrupt is disabled) */
static void timer_check() {
    if (timer_head == NULL || timer_head->func == NULL){
        return;
    }
    timer_head->timeout_tick -= 1;
    // printf("timer_head: %d\n", *(int* )timer_head->arg);
    while (timer_head && timer_head->timeout_tick <= 0){
        if (timer_head->func){
            timer_head->func(timer_head->arg);
        }
        timer_head->func = NULL;
        timer_head->arg = NULL;
        timer_head->timeout_tick = 0;
        timer_head = timer_head->next;
    }
    return;
}

void timer_handler() {
    _tick++;
    printf("tick: %d\n", _tick);
    // printf("\e[8D");
    // printf("%2d:%2d:%2d", 0, _tick / 60, _tick % 60);
	timer_check();
    timer_load(TIMER_INTERVAL);
    schedule();
}

inline uint32_t get_tick(){
    return _tick;
} 