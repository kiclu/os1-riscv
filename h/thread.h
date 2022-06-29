#ifndef _OS1_RISCV_THREAD_H
#define _OS1_RISCV_THREAD_H

// defines thread_create behaviour
// 0 - newly created thread will be added to the waiting queue
// 1 - execution continues on the newly created thread
#define DISPATCH_ON_CREATE 0

#include"../lib/hw.h"

#ifdef  __cplusplus
extern "C"{
#endif//__cplusplus

typedef struct _thread_context{
    uint64 ra;  // return address
    uint64 sp;  // stack pointer
    uint64 gp;  // global pointer
    uint64 tp;  // thread pointer
    // temporary registers
    uint64 t0;
    uint64 t1;
    uint64 t2;
    // saved registers
    uint64 s0;
    uint64 s1;
    // function arguments
    uint64 a0;
    uint64 a1;
    uint64 a2;
    uint64 a3;
    uint64 a4;
    uint64 a5;
    uint64 a6;
    uint64 a7;
    // saved registers
    uint64 s2;
    uint64 s3;
    uint64 s4;
    uint64 s5;
    uint64 s6;
    uint64 s7;
    uint64 s8;
    uint64 s9;
    uint64 s10;
    uint64 s11;
    // temporary registers
    uint64 t3;
    uint64 t4;
    uint64 t5;
    uint64 t6;
} _thread_context;

typedef uint64 pid_t;

typedef enum _thread_state{ NEW, RUNNING, WAITING, BLOCKED, SLEEPING, EXITED } _thread_state;

typedef struct _thread{
    _thread_context context;
    uint64 pc;

    pid_t pid;
    _thread_state state;
    time_t cpu_time;
    time_t wake_time;

    void* stack_space;
    struct _thread* queue_next;
} _thread;

typedef _thread* thread_t;
extern thread_t running;

int __thread_create(thread_t*, void(*)(void*), void*, void*);
int __thread_exit();
void __thread_dispatch();

int __time_sleep(time_t);

void thread_init();

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//_OS1_RISCV_THREAD_H