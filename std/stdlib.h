#ifndef _OS1_RISCV_STD_STDLIB_H
#define _OS1_RISCV_STD_STDLIB_H

#define NULL 0

#include"../h/syscall_c.h"

#include"../lib/hw.h"

#ifdef  __cplusplus
extern "C"{
#endif//__cplusplus

// double atof(const char*);
int atoi(const char*);
long int atol(const char*);

// double strtod(const char*, char**);
// long int strol(const char*, char**, int);
// unsigned long int strtoul(const char*, char**, int);

inline void free(void* ptr){ mem_free(ptr); }
inline void* malloc(size_t size){ return mem_alloc(size); }
void* calloc(size_t, size_t);
void* realloc(void* , size_t);

// void abort(void);
// int atexit(void (*)(void));
// void exit(int);

// char* getenv(const char*);
// int system(const char*);

// void* bsearch(const void*, const void*, size_t, size_t, int (*)(const void*, const void*));
// void qsort(void*, size_t, size_t, int (*)(const void*, const void*));

inline int abs(int x){ return x < 0 ? -x : x; }
// div_t div(int, int);

// long int labs(long int);
// ldiv_t ldiv(long int, long int);

// int rand(void);
// void srand(unsigned int);

// int mblen(const char*, size_t);
// size_t mbtowc(wchar_t*, const char*, size_t);
// size_t wcstombs(char*, const wchar_t*, size_t);
// int wctomb(char*, wchar_t);

#ifdef  __cplusplus
}
#endif//__cplusplus

#endif//_OS1_RISCV_STD_STDLIB_H