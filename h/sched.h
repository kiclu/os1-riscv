#ifndef _OS1_RISCV_SCHED_H
#define _OS1_RISCV_SCHED_H

#include"thread.h"

#ifdef  __cplusplus
extern "C"{
#endif//__cplusplus

thread_t sched();

void sched_init();

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//_OS1_RISCV_SCHED_H