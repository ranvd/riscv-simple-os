#ifndef __USER_API_H__
#define __USER_API_H__

/* user mode syscall APIs */
extern int gethid(unsigned int *hid);
extern void itr_disable();
extern void itr_enable();

#endif /* __USER_API_H__ */
