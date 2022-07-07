#include"../h/console.h"

#include"../h/sem.h"
#include"../h/syscall_c.h"
#include"../h/trap.h"
#include"../h/sched.h"

extern void queue_waiting_push(thread_t);

typedef struct {
    thread_t front;
    thread_t back;
} _uart_blocked_queue;

static _uart_blocked_queue uart_blocked_queue;

static thread_t queue_pop(){
    if(uart_blocked_queue.front == NULL) return NULL;

    thread_t ret = uart_blocked_queue.front;
    uart_blocked_queue.front = ret->queue_next;
    if(uart_blocked_queue.front == NULL) uart_blocked_queue.back = NULL;

    ret->queue_next = NULL;
    return ret;
}

static void queue_push(thread_t thr){
    thr->state = BLOCKED;
    thr->queue_next = NULL;

    if(uart_blocked_queue.front == NULL){
        uart_blocked_queue.front = uart_blocked_queue.back = thr;
        return;
    }

    uart_blocked_queue.back = uart_blocked_queue.back->queue_next = thr;
}

typedef struct _uart_buffer{
    char data[UART_BUFFER_SIZE];
    size_t r;
    size_t w;
    size_t count;
} _uart_buffer;

static _uart_buffer input_buffer;
static _uart_buffer output_buffer;

static char uart_getc(){ return *(char*)CONSOLE_RX_DATA; }
static void uart_putc(char c){ *(char*)CONSOLE_TX_DATA = c; }

static void uart_input(){
    while(*(char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT){
        char c = uart_getc();
        thread_t blocked = queue_pop();
        if(blocked == NULL){
            input_buffer.data[input_buffer.w++] = c;
            input_buffer.w %= UART_BUFFER_SIZE;
            ++input_buffer.count;
        }
        else{
            queue_waiting_push(blocked);
            blocked->context.a0 = c;
        }
    }
}

static void uart_output(){
    while(*(char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT){
        if(output_buffer.count == 0) return;
        uart_putc(output_buffer.data[output_buffer.r++]);
        output_buffer.r %= UART_BUFFER_SIZE;
        --output_buffer.count;
    }
}

static void input_handler(void* arg){
    for(;;){ uart_input(); thread_dispatch(); }
}

static void output_handler(void* arg){
    for(;;){ uart_output(); thread_dispatch(); }
}

static void yield(thread_t new){
    running = new;
    running->state = RUNNING;
    running->cpu_time = 0;
}

char __getc(){
    char c = 0;
    if(input_buffer.count == 0){
        queue_push(running);
        yield(sched());
    }
    else{
        c = input_buffer.data[input_buffer.r++];
        input_buffer.r %= UART_BUFFER_SIZE;
        --input_buffer.count;
    }
    return c;
}

void __putc(char c){
    if(output_buffer.count == UART_BUFFER_SIZE) return;
    output_buffer.data[output_buffer.w++] = c;
    output_buffer.w %= UART_BUFFER_SIZE;
    ++output_buffer.count;
}

void __puts(const char* str){ while(*str) __putc(*str++); }

void console_handler(){}

void console_init(){
    // input buffer initialization
    input_buffer.r = input_buffer.w = 0;
    input_buffer.count = 0;
    thread_t uart_input_thread = NULL;
    thread_create(&uart_input_thread, input_handler, NULL);

    // output buffer initialization
    output_buffer.r = output_buffer.w = 0;
    output_buffer.count = 0;
    thread_t uart_output_thread = NULL;
    thread_create(&uart_output_thread, output_handler, NULL);
}

void console_shutdown(){
    while(output_buffer.count > 0){
        uart_putc(output_buffer.data[output_buffer.r++]);
        output_buffer.r %= UART_BUFFER_SIZE;
        --output_buffer.count;
    }
}