#ifndef _OS1_RISCV_SYSCALL_C_H
#define _OS1_RISCV_SYSCALL_C_H

#include"../lib/hw.h"

#include"console.h"
#include"mem.h"
#include"sem.h"
#include"thread.h"

#ifdef  __cplusplus
extern "C"{
#endif//__cplusplus

void* mem_alloc(size_t);
int mem_free(void*);

int thread_create(thread_t*, void(*)(void*), void*);
int thread_exit();
void thread_dispatch();

int sem_open(sem_t*, uint);
int sem_close(sem_t);
int sem_wait(sem_t);
int sem_signal(sem_t);

int time_sleep(time_t);

char getc();
void putc(char);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//_OS1_RISCV_SYSCALL_C_H