#include"../h/sched.h"

#include"../h/mem.h"
#include"../h/thread.h"

typedef struct{
    thread_t front;
    thread_t back;
} _thread_queue;

_thread_queue queue_new;
_thread_queue queue_waiting;
_thread_queue queue_sleeping;
_thread_queue queue_exited;

void queue_new_push(thread_t thr){
    thr->state = NEW;
    thr->queue_next = NULL;

    if(queue_new.front == NULL){
        queue_new.front = queue_new.back = thr;
        return;
    }

    queue_new.back->queue_next = thr;
    queue_new.back = thr;
}

static thread_t queue_new_pop(){
    if(queue_new.front == NULL) return NULL;
    thread_t res = queue_new.front;
    queue_new.front = queue_new.front->queue_next;
    if(queue_new.front == NULL) queue_new.back = NULL;
    res->queue_next = NULL;
    return res;
}

void queue_waiting_push(thread_t thr){
    thr->state = WAITING;
    thr->queue_next = NULL;

    if(queue_waiting.front == NULL){
        queue_waiting.front = queue_waiting.back = thr;
        return;
    }

    queue_waiting.back->queue_next = thr;
    queue_waiting.back = thr;
}

static thread_t queue_waiting_pop(){
    if(queue_waiting.front == NULL) return NULL;
    thread_t res = queue_waiting.front;
    queue_waiting.front = queue_waiting.front->queue_next;
    if(queue_waiting.front == NULL) queue_waiting.back = NULL;
    res->queue_next = NULL;
    return res;
}

extern time_t tick_count;

void queue_sleeping_push(thread_t thr, time_t wake_time){
    thr->state = SLEEPING;
    thr->queue_next = NULL;
    thr->wake_time = wake_time;

    if(queue_sleeping.front == NULL){
        queue_sleeping.front = queue_sleeping.back = thr;
        return;
    }

    if(wake_time < queue_sleeping.front->wake_time){
        thr->queue_next = queue_sleeping.front;
        queue_sleeping.front = thr;
        return;
    }

    for(thread_t i = queue_sleeping.front; i; i = i->queue_next){
        if(i->wake_time <= wake_time && wake_time < i->queue_next->wake_time){
            thr->queue_next = i->queue_next;
            if(i->queue_next) i->queue_next = thr;
        }
    }
}

static thread_t queue_sleeping_pop(){
    if(queue_sleeping.front == NULL) return NULL;
    if(queue_sleeping.front->wake_time < tick_count) return NULL;
    thread_t res = queue_sleeping.front;
    queue_sleeping.front = queue_sleeping.front->queue_next;
    res->queue_next = NULL;
    return res;
}

thread_t sched(){
    thread_t new = NULL;
    if(new == NULL) new = queue_new_pop();
    if(new == NULL) new = queue_sleeping_pop();
    if(new == NULL) new = queue_waiting_pop();
    return new;
}

void sched_init(){
    queue_new.front = queue_new.back = NULL;
    queue_waiting.front = queue_waiting.back = NULL;
    queue_sleeping.front = queue_sleeping.back = NULL;
    queue_exited.front = queue_exited.back = NULL;
}
