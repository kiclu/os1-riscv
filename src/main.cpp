#include"../h/mem.h"
#include"../h/sched.h"
#include"../h/thread.h"
#include"../h/trap.h"

extern void userMain();

void main(){
    mem_init();
    thread_init();
    sched_init();
    set_stvec();
    intr_enable();

    userMain();
}
