#include"../h/trap.h"

#include"../h/riscv.h"
#include"../h/sched.h"

#include"../h/console.h"
#include"../h/mem.h"
#include"../h/thread.h"
#include"../h/sem.h"

#include"../h/printf.h"

#include"../lib/console.h"

// scause interrupt flag
#define SC_INTERRUPT (1UL << 63)

// software interrupt handling
time_t tick_count = 0;

static int async_ctx_switched = 0;
static void async_ctx_switch(){
    ++running->cpu_time;
    if(running->cpu_time >= DEFAULT_TIME_SLICE){
        __thread_dispatch();
    }
}

static void software_interrupt(){
    sip_disable(SI_SOFTWARE);
    ++tick_count;
    if(!async_ctx_switched) async_ctx_switch();
}

// timer interrupt handling
static void timer_interrupt(){
    sip_disable(SI_TIMER);
}

// hardware interrupt handling
static void hardware_interrupt(){
    sip_disable(SI_EXTERNAL);
    int irq = plic_claim();
    switch(irq){
        case CONSOLE_IRQ: console_handler(); break;
    }
    if(irq) plic_complete(irq);
}

// syscall handler
typedef enum { MODE_USER, MODE_SUPERVISOR } _mode;

static int kernel_initialized = 0;

inline static void user_mode(){
    asm volatile("csrc sstatus, %0" :: "r" (1UL << 8));
    kernel_initialized = 1;
}

static void syscall(_mode mode){
    if(mode == MODE_SUPERVISOR && kernel_initialized) write_sepc(read_sepc() + 4);
    else running->pc += 4;

    uint64* args = (uint64*)(&(running->context.a0));
    switch(args[0]){
        // void* mem_alloc(size_t);
        case 0x01: { args[0] = (uint64)__mem_alloc(args[1]); } break;

        // int mem_free(void*);
        case 0x02: { args[0] = __mem_free((void*)args[1]); } break;

        // int thread_create(thread_t*, void(*)(void*), void*);
        case 0x11: {
            args[0] = -1;
            args[0] = __thread_create(
                (thread_t*)args[1],
                (void(*)(void*))args[2],
                (void*)args[3],
                (void*)args[4]
            );
        } break;

        // int thread_exit();
        case 0x12: { args[0] = __thread_exit(); } break;

        // int thread_dispatch();
        case 0x13: { __thread_dispatch(); } break;

        // int sem_open(sem_t*, uint);
        case 0x21: { args[0] = __sem_open((sem_t*)args[1], (uint)args[2]); } break;

        // int sem_close(sem_t);
        case 0x22: { args[0] = __sem_close((sem_t)args[1]); } break;

        // int sem_wait(sem_t);
        case 0x23: { args[0] = __sem_wait((sem_t)args[1]); } break;

        // int sem_signal(sem_t);
        case 0x24: { args[0] = __sem_signal((sem_t)args[1]); } break;

        // int time_sleep(time_t);
        case 0x31: { args[0] = __time_sleep(args[1]); } break;

        // char getc();
        case 0x41: { args[0] = __getc(); } break;

        // void putc(char);
        case 0x42: { __putc(args[1]); } break;

        // void user_mode();
        case 0x51: { user_mode(); } break;

        // unsupported syscall
        default: {} break;
    }
}

// trap handler
extern void kintrvec();

void intr_handler(){
    uint64 scause = read_scause();
    running->pc = read_sepc();
    async_ctx_switched = 0;
    set_stvec(kintrvec);

    // interrupt
    if(scause & SC_INTERRUPT){
        switch(scause & 0xFF){
            // supervisor software interrupt
            case 1: { software_interrupt(); } break;

            // supervisor timer interrupt
            case 5: { timer_interrupt(); } break;

            // supervisor external interrupt
            case 9: { hardware_interrupt(); } break;
        }
    }
    // exception
    else{
        switch(scause){
            // instruction address misaligned
            case 0x00: {} break;

            // instruction access fault
            case 0x01: { __thread_exit(); } break;

            // illegal instruction
            case 0x02: {} break;

            // breakpoint
            case 0x03: {} break;

            // load address misaligned
            case 0x04: {} break;

            // load access fault
            case 0x05: {} break;

            // store/AMO address misaligned
            case 0x06: {} break;

            // store/AMO access fault
            case 0x07: {} break;

            // environment call from U mode
            case 0x08: { syscall(MODE_USER); } break;

            // environment call from S mode
            case 0x09: { syscall(MODE_SUPERVISOR); } break;

            // environment call from M mode
            case 0x0B: {} break;

            // instruction page fault
            case 0x0C: {} break;

            // load page fault
            case 0x0D: {} break;

            // store/AMO page fault
            case 0x0F: {} break;

            // unknown exception
            default: {} break;
        }
    }

    set_stvec(intrvec);
    write_sepc(running->pc);
}

void kintr_handler(){
    uint64 sstatus = read_sstatus();
    uint64 scause = read_scause();

    // interrupt
    if(scause & SC_INTERRUPT){
        switch(scause & 0xFF){
            // supervisor software interrupt
            case 1: { software_interrupt(); } break;

            // supervisor timer interrupt
            case 5: { timer_interrupt(); } break;

            // supervisor external interrupt
            case 9: { hardware_interrupt(); } break;
        }
    }
    else{
        // environment call from S mode
        if((scause & 0xFF) == 0x09){ syscall(MODE_SUPERVISOR); }
    }

    write_sstatus(sstatus);
}