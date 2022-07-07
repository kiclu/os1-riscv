#ifndef _OS1_RISCV_STD_STDARG_H
#define _OS1_RISCV_STD_STDARG_H

#include"../lib/hw.h"

#ifdef  __cplusplus
extern "C"{
#endif//__cplusplus

#define va_start(vl, idx) asm("mv %0, fp" : "=r"(vl.fp)); vl.fp += idx;

#define va_arg(vl, type) ((type)(*(vl.fp++)))

typedef struct{ uint64* fp; } va_list;

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//_OS1_RISCV_STD_STDARG_H
