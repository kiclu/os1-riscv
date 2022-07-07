#ifndef _OS1_RISCV_THREAD_HPP
#define _OS1_RISCV_THREAD_HPP

#include"syscall_c.h"
#include"thread.h"

void wrapper(void*);

class Thread{
public:
    Thread(void(*start_routine)(void*), void* arg) :
    m_start_routine(start_routine), m_arg(arg){}
    virtual ~Thread(){}

    int start(){
        if(m_handle) return m_handle->pid;
        return thread_create(&m_handle, wrapper, this);
    }

    static void dispatch(){ thread_dispatch(); }
    static int sleep(time_t time){ return time_sleep(time); }
protected:
    Thread(){}
    virtual void run(){ m_start_routine(m_arg); }
private:
    thread_t m_handle = NULL;
    void(*m_start_routine)(void*) = NULL;
    void* m_arg = NULL;
    
    friend void wrapper(void* thr){ if(thr) ((Thread*)thr)->run(); }
};

class PeriodicThread : public Thread{
protected:
    PeriodicThread(time_t period) : m_period(period){}
    virtual void periodicActivation(){}

    virtual void run() final { do{ periodicActivation(); sleep(m_period); }while(1); }
private:
    time_t m_period;
};

#endif//_OS1_RISCV_THREAD_HPP