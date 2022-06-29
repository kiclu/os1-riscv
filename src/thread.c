#include"../h/thread.h"

#include"../h/mem.h"
#include"../h/sched.h"

#define THREAD_MB_COUNT block_count(sizeof(_thread))
#define THREAD_SIZE sizeof(_thread)

static void panic(){ for(;;); }

thread_t running = NULL;
thread_t kernel_thread = NULL;

extern void queue_new_push(thread_t);
extern void queue_waiting_push(thread_t);
extern void queue_sleeping_push(thread_t, time_t);

static pid_t pid_counter = 2000;

static void yield(thread_t new){
    queue_waiting_push(running);

    running = new;
    running->state = RUNNING;
    running->cpu_time = 0;
}

int __thread_create(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space){
    if(handle == NULL || start_routine == NULL) panic();
    if(stack_space == NULL) return -1;
    
    *handle = __kmem_alloc(THREAD_SIZE);
    if((*handle) == NULL){
        __kmem_free(stack_space);
        return -1;
    }

    (*handle)->context.sp = (uint64)((*handle)->stack_space = stack_space) + DEFAULT_STACK_SIZE;
    (*handle)->context.a0 = (uint64)arg;
    (*handle)->pc = (uint64)start_routine;

    (*handle)->pid = pid_counter++;
    (*handle)->state = NEW;
    (*handle)->lock = 0;

    (*handle)->cpu_time = 0;
    (*handle)->wake_time = 0;

    queue_new_push(*handle);

    if(DISPATCH_ON_CREATE) __thread_dispatch();
    return (*handle)->pid;
}

int __thread_exit(){
    if(running == kernel_thread) return -1;
    thread_t new = sched();
    if(new != NULL){
        running->state = EXITED;
        yield(new);
        if(__kmem_free(running->stack_space) != 0){ return -2; }
        return 0;
    }
    return -3;
}

void __thread_dispatch(){
    thread_t new = sched();
    if(new != NULL) yield(new);
}

// insert running thread into sleeping queue
int __time_sleep(time_t time){
    // if(running == kernel_thread) return;
    queue_sleeping_push(running, time);
    running = sched();
    return 0;
}

void thread_init(){
    kernel_thread = running = (thread_t)__kmem_alloc(THREAD_SIZE);
    if(kernel_thread == NULL) panic();
    
    kernel_thread->stack_space = __kmem_alloc(DEFAULT_STACK_SIZE);
    kernel_thread->context.sp = (uint64)kernel_thread->stack_space + DEFAULT_STACK_SIZE;
    if(kernel_thread->stack_space == NULL) panic();

    kernel_thread->pid = 0;
    kernel_thread->state = RUNNING;
    kernel_thread->lock = NULL;
}