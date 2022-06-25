#include"../h/sem.h"

#include"../h/mem.h"
#include"../h/sched.h"
#include"../h/thread.h"
#include"../h/trap.h"
#include"../h/syscall_c.h"

#define SEM_SIZE sizeof(_sem)

static void panic(){ for(;;); }

extern void queue_waiting_push(thread_t);

static void queue_sem_push(thread_t thr, sem_t id){
    thr->state = BLOCKED;
    thr->queue_next = NULL;
    thr->lock = id;

    if(id->blocked_queue_front == NULL){
        id->blocked_queue_front = id->blocked_queue_back = thr;
        return;
    }

    id->blocked_queue_back = id->blocked_queue_back->queue_next = thr;
}

static thread_t queue_sem_pop(sem_t id){
    if(id->blocked_queue_front == NULL) return NULL;

    thread_t ret = id->blocked_queue_front;
    id->blocked_queue_front = ret->queue_next;
    if(id->blocked_queue_front == NULL) id->blocked_queue_back = NULL;

    ret->queue_next = NULL;
    ret->lock = NULL;

    return ret;
}

static void yield_sem(thread_t new, sem_t id){
    queue_sem_push(running, id);

    running = new;
    running->state = RUNNING;
    running->cpu_time = 0;
}

int __sem_open(sem_t* handle, uint value){
    if(handle == NULL) panic();

    *handle = __kmem_alloc(SEM_SIZE);
    if(*handle == NULL) return -1;

    (*handle)->value = (int)value;
    (*handle)->blocked_queue_front = (*handle)->blocked_queue_back = NULL;
    return 0;
}

int __sem_close(sem_t handle){
    if(handle == NULL) return -1;

    // unblock any threads waiting on this semaphore
    thread_t blocked = NULL;
    while((blocked = queue_sem_pop(handle)) != NULL){
        queue_waiting_push(blocked);
    }

    // no threads are waiting, delete semaphore
    __kmem_free(handle);
    handle = NULL;
    return 0;
}

int __sem_wait(sem_t id){
    if(id == NULL) return -1;

    // if value greater than 0, decrement and return immediately
    if(id->value > 0){ --id->value; return 0; }

    // else block thread and yield
    thread_t new = NULL;
    while((new = sched()) == NULL);
    yield_sem(new, id);

    return 0;
}

int __sem_signal(sem_t id){
    if(id == NULL) return -1;
    
    // if there are threads waiting, unblock one and return
    // else increment value and return
    thread_t blocked = queue_sem_pop(id);
    if(blocked == NULL) ++id->value;
    else queue_waiting_push(blocked);

    return 0;
}
