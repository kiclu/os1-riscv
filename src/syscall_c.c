#include"../h/syscall_c.h"

#define STACK_ALLOC() (__kmem_alloc(DEFAULT_STACK_SIZE))

extern size_t block_count(size_t);

static uint64 syscall(uint code, ...){
    uint64 ret = -1;
    uint64* fp = NULL;
    asm("mv %0, fp" : "=r"(fp));
    asm("ld a1, 0x08(fp)");
    asm("ld a2, 0x10(fp)");
    asm("ld a3, 0x18(fp)");
    asm("ld a4, 0x20(fp)");
    asm("ecall");
    asm("mv %0, a0" : "=r"(ret));
    return ret;
}

void* mem_alloc(size_t size){ return (void*)syscall(0x01, size); }

int mem_free(void* addr){ return syscall(0x02, addr); }

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){ return syscall(0x11, handle, start_routine, arg, STACK_ALLOC()); }

int thread_exit(){ return syscall(0x12); }

void thread_dispatch(){ syscall(0x13); }

int sem_open(sem_t* handle, uint value){ return syscall(0x21, handle, value); }

int sem_close(sem_t handle){ return syscall(0x22, handle); }

int sem_wait(sem_t id){ return syscall(0x23, id); }

int sem_signal(sem_t id){ return syscall(0x24, id); }

int time_sleep(time_t time){ return syscall(0x31, time); }

char getc(){ return syscall(0x41); }

void putc(char c){ syscall(0x42, c); }

void user_mode(){ syscall(0x51); }
