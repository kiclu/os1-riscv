#include"../h/syscall_c.h"

extern size_t block_count(size_t);

void* mem_alloc(size_t size){
    void* ret = NULL;
    asm volatile("mv a1, %0" :: "r"(block_count(size)));
    asm volatile("mv a0, %0" :: "r"(0x01));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int mem_free(void* addr){
    int ret = 0;
    asm volatile("mv a1, %0" :: "r"(addr));
    asm volatile("mv a0, %0" :: "r"(0x02));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    if(handle == NULL || start_routine == NULL) return -1;

    int ret = 0;
    asm volatile("mv a4, %0" :: "r"(__kmem_alloc(DEFAULT_STACK_SIZE)));
    asm volatile("mv a3, %0" :: "r"(arg));
    asm volatile("mv a2, %0" :: "r"(start_routine));
    asm volatile("mv a1, %0" :: "r"(handle));
    asm volatile("mv a0, %0" :: "r"(0x11));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int thread_exit(){
    int ret = 0;
    asm volatile("mv a0, %0" :: "r"(0x12));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

void thread_dispatch(){
    asm volatile("mv a0, %0" :: "r"(0x13));
    asm volatile("ecall");
}

int sem_open(sem_t* handle, uint value){
    int ret = 0;
    asm volatile("mv a2, %0" :: "r"(value));
    asm volatile("mv a1, %0" :: "r"(handle));
    asm volatile("mv a0, %0" :: "r"(0x21));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_close(sem_t handle){
    int ret = 0;
    asm volatile("mv a1, %0" :: "r"(handle));
    asm volatile("mv a0, %0" :: "r"(0x22));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_wait(sem_t id){
    int ret = 0;
    asm volatile("mv a1, %0" :: "r"(id));
    asm volatile("mv a0, %0" :: "r"(0x23));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_signal(sem_t id){
    int ret = 0;
    asm volatile("mv a1, %0" :: "r"(id));
    asm volatile("mv a0, %0" :: "r"(0x24));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int time_sleep(time_t time){
    int ret = 0;
    asm volatile("mv a1, %0" :: "r"(time));
    asm volatile("mv a0, %0" :: "r"(0x31));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

char getc(){
    char ret = -1;
    asm volatile("mv a0, %0" :: "r"(0x41));
    asm volatile("ecall");
    asm volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

void putc(char c){
    asm volatile("mv a1, %0" :: "r"(c));
    asm volatile("mv a0, %0" :: "r"(0x42));
    asm volatile("ecall");
}

void user_mode(){
    asm volatile("mv a0, %0" :: "r"(0x51));
    asm volatile("ecall");
}