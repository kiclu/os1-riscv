#include"../h/trap.h"

#include"../h/riscv.h"
#include"../h/sched.h"

#include"../h/console.h"
#include"../h/mem.h"
#include"../h/syscall_c.h"
#include"../h/thread.h"
#include"../h/sem.h"

#include"../h/printf.h"

#include"../lib/console.h"

// scause interrupt flag
#define SC_INTERRUPT (1UL << 63)

time_t tick_count = 0;

void sync_ctx_switch(){
    ++running->cpu_time;
    if(running->cpu_time >= DEFAULT_TIME_SLICE) __thread_dispatch();
}

void intr_handler(){
    running->pc = read_sepc();
    uint64 sstatus = read_sstatus();
    uint64 scause = read_scause();

    set_stvec(kintrvec);
    //intr_enable();

    // interrupt
    if(scause & SC_INTERRUPT){
        // supervisor software interrupt
        if((scause & 0xFF) == 1){
            sip_disable(SI_SOFTWARE);

            ++tick_count;
            sync_ctx_switch();
        }

        // supervisor timer interrupt
        if((scause & 0xFF) == 5){
            sip_disable(SI_TIMER);
        }

        // supervisor external interrupt
        if((scause & 0xFF) == 9){
            sip_disable(SI_EXTERNAL);

            int irq = plic_claim();
            switch(irq){
                case CONSOLE_IRQ: console_handler(); break;
            }
            if(irq) plic_complete(irq);
        }
    }
    // exception
    else{
        // instruction address misaligned
        if((scause & 0xFF) == 0x00){}

        // instruction access fault
        if((scause & 0xFF) == 0x01){
            // forcefully exits the thread if it has reached the end
            // but has not been terminated properly with thread_exit();
            __thread_exit();
        }

        // illegal instruction
        if((scause & 0xFF) == 0x02){}

        // breakpoint
        if((scause & 0xFF) == 0x03){}

        // load address misaligned
        if((scause & 0xFF) == 0x04){}

        // load access fault
        if((scause & 0xFF) == 0x05){}

        // store/AMO address misaligned
        if((scause & 0xFF) == 0x06){}

        // store/AMO access fault
        if((scause & 0xFF) == 0x07){}

        // ecnvironment all from U mode
        if((scause & 0xFF) == 0x08){
            running->pc += 4;

            // load ecall arguments to a0-a5 registers
            asm volatile("mv a4, %0" :: "r"(running->context.a4));
            asm volatile("mv a3, %0" :: "r"(running->context.a3));
            asm volatile("mv a2, %0" :: "r"(running->context.a2));
            asm volatile("mv a1, %0" :: "r"(running->context.a1));
            asm volatile("mv a0, %0" :: "r"(running->context.a0));

            // execute ecall
            asm volatile("ecall");
        }
        // environment call from S mode
        if((scause & 0xFF) == 0x09){
            running->pc += 4;
            uint64* args = (uint64*)(&(running->context.a0));
            switch(args[0]){
                // void* mem_alloc(size_t);
                case 0x01: { args[0] = (uint64)__mem_alloc(args[1]); } break;

                // int mem_free(void*);
                case 0x02: { args[0] = __mem_free((void*)args[1]); } break;

                // int thread_create(thread_t*, void(*)(void*), void*);
                case 0x11: {
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

                // switch to user mode
                case 0x51: {
                    //printf("Switching to user mode...\n");
                    sstatus &= ~(1UL << 8);
                } break;
            }
        }

        // environment call from M mode
        if((scause & 0xFF) == 0x0B){}

        // instruction page fault
        if((scause & 0xFF) == 0x0C){}

        // load page fault
        if((scause & 0xFF) == 0x0D){}

        // store/AMO page fault
        if((scause & 0xFF) == 0x0F){}
    }

    //intr_disable();
    set_stvec(intrvec);

    write_sepc(running->pc);
    write_sstatus(sstatus);

    intrret();
}

void kintr_handler(){
    uint64 sepc = read_sepc();
    uint64 sstatus = read_sstatus();
    uint64 scause = read_scause();

    if(scause & SC_INTERRUPT){
        if((scause & 0xFF) == 1){
            sip_disable(SI_SOFTWARE);

            ++tick_count;
        }
        if((scause & 0xFF) == 5){
            sip_disable(SI_TIMER);
        }
        if((scause & 0xFF) == 9){
            sip_disable(SI_EXTERNAL);
            
            //console_handler();
        }
    }
    else{
        // environment call from S mode
        if((scause & 0xFF) == 0x09){
            sepc += 4;
            uint64* args = (uint64*)(&(running->context.a0));
            switch(args[0]){
                // void* mem_alloc(size_t);
                case 0x01: { args[0] = (uint64)__mem_alloc(args[1]); } break;

                // int mem_free(void*);
                case 0x02: { args[0] = __mem_free((void*)args[1]); } break;

                // int thread_create(thread_t*, void(*)(void*), void*);
                case 0x11: {
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

                // switch to user mode
                case 0x51: { sstatus &= ~(1UL << 8); } break;
            }
        }
    }

    write_sepc(sepc);
    write_sstatus(sstatus);
}