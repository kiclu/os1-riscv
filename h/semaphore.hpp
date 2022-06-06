#ifndef _OS1_RISCV_SEMAPHORE_HPP
#define _OS1_RISCV_SEMAPHORE_HPP

#include"sem.h"

class Semaphore{
public:
    Semaphore(uint init = 1){ sem_open(&m_handle, init); }
    virtual ~Semaphore(){ sem_close(m_handle); }

    int wait(){ return sem_wait(m_handle); }
    int signal(){ return sem_signal(m_handle); }
private:
    sem_t m_handle;
};

#endif//_OS1_RISCV_SEMAPHORE_HPP