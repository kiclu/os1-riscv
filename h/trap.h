#ifndef _OS1_RISCV_TRAP_H
#define _OS1_RISCV_TRAP_H

// sie/sip software interrupt flag
#define SI_SOFTWARE (1UL << 1)

// sie/sip timer interrupt flag
#define SI_TIMER (1UL << 5)

// sie/sip external interrupt flag
#define SI_EXTERNAL (1UL << 9)

#include"../lib/hw.h"

#include"../h/riscv.h"

#ifdef __cplusplus
extern "C"{
#endif//__cplusplus

extern time_t tick_count;

extern void intrvec();

inline void intr_enable(){ write_sstatus(read_sstatus() | (1UL << 1)); }
inline void intr_disable(){ write_sstatus(read_sstatus() & ~(1UL << 1)); }

inline void sip_enable(uint64 sip_flag){ write_sip(read_sip() | sip_flag); }
inline void sip_disable(uint64 sip_flag){ write_sip(read_sip() & ~sip_flag); }

inline void sie_enable(uint64 sie_flag){ write_sie(read_sie() | sie_flag); }
inline void sie_disable(uint64 sie_flag){ write_sie(read_sie() & ~sie_flag); }

inline void set_stvec(void(*stvec)()){ write_stvec((uint64)stvec); }

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_OS1_RISCV_TRAP_H
