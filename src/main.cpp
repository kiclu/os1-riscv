#include"../h/mem.h"
#include"../h/sched.h"
#include"../h/syscall_c.h"
#include"../h/thread.h"
#include"../h/trap.h"

#include"../h/printf.h"

extern void userMain();

void main(){
    mem_init();
    thread_init();
    sched_init();
    set_stvec(uintrvec);
    intr_enable();
    user_mode();

    printf("Hello world!\n");
}
