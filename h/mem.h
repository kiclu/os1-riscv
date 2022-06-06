#ifndef _OS1_RISCV_MEM_H
#define _OS1_RISCV_MEM_H

// defines which algorithm will be used for memory allocation
// 0 - best fit
// 1 - first fit
#define FIT_ALGORITHM 1

#include"defs.h"

#ifdef  __cplusplus
extern "C"{
#endif//__cplusplus

void* __mem_alloc(size_t);
int __mem_free(void*);

void* __kmem_alloc(size_t);
int __kmem_free(void*);

void mem_init();

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//_OS1_RISCV_MEM_H