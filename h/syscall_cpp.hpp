#ifndef _OS1_RISCV_SYSCALL_CPP_HPP
#define _OS1_RISCV_SYSCALL_CPP_HPP

#include"syscall_c.h"

#include"console.hpp"
#include"semaphore.hpp"
#include"thread.hpp"

void* operator new(size_t);
void operator delete(void*);

#endif//_OS1_RISCV_SYSCALL_CPP_HPP