#ifndef _OS1_RISCV_CONSOLE_HPP
#define _OS1_RISCV_CONSOLE_HPP

#include"syscall_c.h"

class Console{
public:
    static char getc(){ return getc(); }
    static void putc(char c){ putc(c); }
};

#endif//_OS1_RISCV_CONSOLE_HPP