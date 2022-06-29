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

// insert thread into sorted sleeping queue
void queue_sleeping_push(thread_t thr, time_t time){
    time_t wake_time = tick_count + time;

    thr->state = SLEEPING;
    thr->wake_time = wake_time;
    thr->queue_next = NULL;

    if(queue_sleeping.front == NULL){
        queue_sleeping.front = queue_sleeping.back = thr;
        return;
    }

    thread_t cur = queue_sleeping.front;
    thread_t prev = NULL;
    while(cur != NULL){
        if(cur->wake_time > wake_time){
            if(prev == NULL){
                queue_sleeping.front = thr;
                thr->queue_next = cur;
                return;
            }
            prev->queue_next = thr;
            thr->queue_next = cur;
            return;
        }
        prev = cur;
        cur = cur->queue_next;
    }
    queue_sleeping.back->queue_next = thr;
    queue_sleeping.back = thr;
}

// pop first elem from sleeping queue if wake time is greater than tick count
static thread_t queue_sleeping_pop(){
    if(queue_sleeping.front == NULL) return NULL;
    if(queue_sleeping.front->wake_time > tick_count) return NULL;
    thread_t res = queue_sleeping.front;
    queue_sleeping.front = queue_sleeping.front->queue_next;
    if(queue_sleeping.front == NULL) queue_sleeping.back = NULL;
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


