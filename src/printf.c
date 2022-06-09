#include"../h/printf.h"

#include"../lib/hw.h"

#include"../h/syscall_c.h"

static const char* digits = "0123456789abcdef";
static const char* capital_digits = "0123456789ABCDEF";

void print_str(const char* arg){
    for(int i = 0; arg[i] != '\0'; ++i) __putc(arg[i]);
}

void print_uint8(uint8 arg, uint8 base, char uppercase){
    char buffer[20];
    int i = 0;
    do{
        buffer[i++] = uppercase ? capital_digits[arg % base] : digits[arg % base];
        arg /= base;
    }while(arg);
    for(int j = i - 1; j >= 0; --j) __putc(buffer[j]);
}

void print_uint16(uint16 arg, uint8 base, char uppercase){
    char buffer[20];
    int i = 0;
    do{
        buffer[i++] = uppercase ? capital_digits[arg % base] : digits[arg % base];
        arg /= base;
    }while(arg);
    for(int j = i - 1; j >= 0; --j) __putc(buffer[j]);
}

void print_uint32(uint32 arg, uint8 base, char uppercase){
    char buffer[20];
    int i = 0;
    do{
        buffer[i++] = uppercase ? capital_digits[arg % base] : digits[arg % base];
        arg /= base;
    }while(arg);
    for(int j = i - 1; j >= 0; --j) __putc(buffer[j]);
}

void print_uint64(uint64 arg, uint8 base, char uppercase){
    char buffer[20];
    int i = 0;
    do{
        buffer[i++] = digits[arg % base];
        arg /= base;
    }while(arg);
    for(int j = i - 1; j >= 0; --j) __putc(buffer[j]);
}

void print_schar(signed char arg){
    if(arg < 0){
        __putc('-');
        arg *= -1;
    }
    print_uint8((uint8)arg, 10, 0);
}

void print_short(short arg){
    if(arg < 0){
        __putc('-');
        arg *= -1;
    }
    print_uint16((uint16)arg, 10, 0);
}

void print_int(int arg){
    if(arg < 0){
        __putc('-');
        arg *= -1;
    }
    print_uint32((uint32)arg, 10, 0);
}

void print_long(long arg){
    if(arg < 0){
        __putc('-');
        arg *= -1;
    }
    print_uint64((uint64)arg, 10, 0);
}

void print_ptr(void* arg){
    print_str("0x");
    print_uint64((uint64)arg, 16, 0);
}

void printf(const char* fmt, ...){
    uint64 arg[7];
    asm volatile("mv %0, a1" : "=r"(arg[0]));
    asm volatile("mv %0, a2" : "=r"(arg[1]));
    asm volatile("mv %0, a3" : "=r"(arg[2]));
    asm volatile("mv %0, a4" : "=r"(arg[3]));
    asm volatile("mv %0, a5" : "=r"(arg[4]));
    asm volatile("mv %0, a6" : "=r"(arg[5]));
    asm volatile("mv %0, a7" : "=r"(arg[6]));
    int k = 0;

    for(int i = 0; fmt[i] != '\0'; ++i){
        if(fmt[i] != '%'){
            __putc(fmt[i]);
            continue;
        }

        ++i;
        switch(fmt[i]){
            case '%': __putc('%'); break;

            // specifiers
            case 'c': __putc((char)arg[k++]); break;                // char
            case 's': print_str((const char*)arg[k++]); break;      // string
            case 'd': case 'i': print_int((int)arg[k++]); break;    // int
            case 'o': print_uint32((uint)arg[k++], 8, 0); break;    // octal
            case 'x': print_uint32((uint)arg[k++], 16, 0); break;   // int hex
            case 'X': print_uint32((uint)arg[k++], 16, 1); break;   // int hex uppercase
            case 'u': print_uint32((uint)arg[k++], 10, 0); break;   // unsigned
            case 'p': print_ptr((void*)arg[k++]); break;            // pointer

            // length
            case 'h': {
                ++i;
                switch(fmt[i]){
                    case 'd': case 'i': print_short((short)arg[k++]); break;
                    case 'o': print_uint16((uint16)arg[k++], 8, 0); break;
                    case 'x': print_uint16((uint16)arg[k++], 16, 0); break;
                    case 'X': print_uint16((uint16)arg[k++], 16, 1); break;
                    case 'u': print_uint16((uint16)arg[k++], 10, 0); break;
                    case 'h': {
                        ++i;
                        switch(fmt[i]){
                        case 'd': case 'i': print_int((signed char)arg[k++]); break;
                        case 'u': print_uint16((uint8)arg[k++], 10, 0); break;
                        default: continue;
                        }
                    }
                    default: continue;
                }
            } break;
            case 'l': {
                ++i;
                switch(fmt[i]){
                    case 'd': case 'i': print_long((long)arg[k++]); break;
                    case 'o': print_uint64((uint64)arg[k++], 8, 0); break;
                    case 'x': print_uint64((uint64)arg[k++], 16, 0); break;
                    case 'X': print_uint64((uint64)arg[k++], 16, 1); break;
                    case 'u': print_uint64((uint64)arg[k++], 10, 0); break;
                    case 'b': print_uint64((uint64)arg[k++], 2, 0); break;      // testing
                    default: continue;
                }
            } break;

            default: continue;
        }
    }
}
