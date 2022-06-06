#ifndef _OS1_RISCV_SEM_H
#define _OS1_RISCV_SEM_H

#include"defs.h"
#include"thread.h"

#ifdef  __cplusplus
extern "C"{
#endif//__cplusplus

typedef struct _sem{
    uint value;

    thread_t blocked_queue_front;
    thread_t blocked_queue_back;
} _sem;

typedef _sem* sem_t;

int __sem_open(sem_t*, uint);
int __sem_close(sem_t);
int __sem_wait(sem_t);
int __sem_signal(sem_t);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//_OS1_RISCV_SEM_H