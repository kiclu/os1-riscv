#include"stdio.h"

#include"../h/syscall_c.h"

#include"stdint.h"

static char lc_digits[] = "0123456789abcdef";
static char uc_digits[] = "0123456789ABCDEF";

void print_uint(uint64_t x, uint8_t base, _Bool caps){
    if(x == 0){ putc('0'); return; }
    char buffer[1024];
    size_t i = 0;
    char* digits = caps ? uc_digits : lc_digits;
    while(x > 0){ buffer[i++] = digits[x % base]; x /= base;}
    while(i > 0){ putc(buffer[--i]); }
}

void print_int(int64_t x){
    if(x == 0){ putc('0'); return; }
    int sign = 1;
    if(x < 0){ sign = -1; x = -x; }
    char buffer[1024];
    size_t i = 0;
    if(sign == -1) putc('-');
    while(x > 0){ buffer[i++] = x % 10 + '0'; x /= 10; }
    while(i > 0){ putc(buffer[--i]); }
}

void print_str(char* str){ while(*str) putc(*str++); }

void print_ptr(void* ptr){ print_str("0x"); print_uint((uint64_t)ptr, 16, 0); }

#include"stdarg.h"

void printf(const char* fmt, ...){
    va_list vl;
    va_start(vl, 1);

    while(*fmt){
        if(*fmt == '%'){
            ++fmt;
            switch(*fmt){
                case 'd': case 'i': { print_int(va_arg(vl, int32_t)); } break;
                case 'u': { print_uint(va_arg(vl, uint32_t), 10, 0); } break;
                case 'o': { print_uint(va_arg(vl, uint32_t), 8, 0); } break;
                case 'x': { print_uint(va_arg(vl, uint32_t), 16, 0); } break;
                case 'X': { print_uint(va_arg(vl, uint32_t), 16, 1); } break;
                case 'c': { putc(va_arg(vl, char)); } break;
                case 's': { print_str(va_arg(vl, char*)); } break;
                case 'p': { print_ptr(va_arg(vl, void*)); } break;
                case '%': { putc('%'); } break;

                // length specifier
                case 'h': {
                    ++fmt;
                    switch(*fmt){
                        case 'd': case 'i': { print_int(va_arg(vl, int16_t)); } break;
                        case 'u': { print_uint(va_arg(vl, uint16_t), 10, 0); } break;
                        case 'o': { print_uint(va_arg(vl, uint16_t), 8, 0); } break;
                        case 'x': { print_uint(va_arg(vl, uint16_t), 16, 0); } break;
                        case 'X': { print_uint(va_arg(vl, uint16_t), 16, 1); } break;

                        case 'h':{
                            ++fmt;
                            switch(*fmt){
                                case 'd': case 'i': { print_int(va_arg(vl, int8_t)); } break;
                                case 'u': { print_uint(va_arg(vl, uint8_t), 10, 0); } break;
                                case 'o': { print_uint(va_arg(vl, uint8_t), 8, 0); } break;
                                case 'x': { print_uint(va_arg(vl, uint8_t), 16, 0); } break;
                                case 'X': { print_uint(va_arg(vl, uint8_t), 16, 1); } break;
                                default: break;
                            }
                        } break;

                        default: break;
                    }
                } break;

                case 'l': {
                    ++fmt;
                    switch(*fmt){
                        case 'd': case 'i': { print_int(va_arg(vl, int64_t)); } break;
                        case 'u': { print_uint(va_arg(vl, uint64_t), 10, 0); } break;
                        case 'o': { print_uint(va_arg(vl, uint64_t), 8, 0); } break;
                        case 'x': { print_uint(va_arg(vl, uint64_t), 16, 0); } break;
                        case 'X': { print_uint(va_arg(vl, uint64_t), 16, 1); } break;

                        default: break;
                    }
                } break;

                default: break;
            }
        }
        else putc(*fmt);
        ++fmt;
    }
}