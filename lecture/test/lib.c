#include <stdio.h>
#include <unistd.h>

extern void fun1(void (*f_ptr) (int, int)){
    //printf("%x\n", f_ptr);
    f_ptr(2,2);
}