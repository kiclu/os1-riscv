#include"../h/console.h"

#include"../h/sem.h"
#include"../h/syscall_c.h"
#include"../h/trap.h"

typedef struct _uart_buffer{
    char data[UART_BUFFER_SIZE];
    size_t r;
    size_t w;
    size_t count;

    sem_t empty;
    sem_t full;
    sem_t mutex;
} _uart_buffer;

static _uart_buffer output_buffer;
static _uart_buffer input_buffer;

static void uart_buffer_push(_uart_buffer* buf, char c){
    __sem_wait(buf->full);
    __sem_wait(buf->mutex);

    __putc('+');

    buf->data[buf->w++] = c;
    buf->w %= UART_BUFFER_SIZE;

    __sem_signal(buf->mutex);
    __sem_signal(buf->empty);
}

static char uart_buffer_pop(_uart_buffer* buf){
    __sem_wait(buf->empty);
    __sem_wait(buf->mutex);

    __putc('-');

    char c = buf->data[buf->r++];
    buf->r %= UART_BUFFER_SIZE;

    __sem_signal(buf->mutex);
    __sem_signal(buf->full);

    return c;
}

static void uart_putc(char c){ *(char*)CONSOLE_TX_DATA = c; }
static char uart_getc(){ return *(char*)CONSOLE_RX_DATA; }

static void uart_output(){
    while(*(char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT && output_buffer.count){
        uart_putc(output_buffer.data[output_buffer.r++]);
        output_buffer.r %= UART_BUFFER_SIZE;
        --output_buffer.count;
    }
}

static void uart_input(){
    while(*(char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT){
        uart_buffer_push(&input_buffer, uart_getc());
    }
}

static void output_handler(void* arg){
    for(;;){ uart_output(); thread_dispatch(); }
}

void console_handler(){ uart_input(); }

void __putc(char c){
    if(output_buffer.count < UART_BUFFER_SIZE){
        output_buffer.data[output_buffer.w++] = c;
        output_buffer.w %= UART_BUFFER_SIZE;
        ++output_buffer.count;
    }
}

char __getc(){ return uart_buffer_pop(&input_buffer); }

void console_init(){
    output_buffer.r = output_buffer.w = 0;
    output_buffer.count = 0;

    thread_t uart_output_thread = NULL;
    thread_create(&uart_output_thread, output_handler, NULL);

    input_buffer.r = 0;
    input_buffer.w = 0;
    sem_open(&input_buffer.full, UART_BUFFER_SIZE - 1);
    sem_open(&input_buffer.empty, 0);
    sem_open(&input_buffer.mutex, 1);
}

void console_shutdown(){
    while(output_buffer.count > 0){
        uart_putc(output_buffer.data[output_buffer.r++]);
        output_buffer.r %= UART_BUFFER_SIZE;
        --output_buffer.count;
    }
}