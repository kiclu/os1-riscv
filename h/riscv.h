#ifndef _OS1_RISCV_RISCV_H
#define _OS1_RISCV_RISCV_H

#include"defs.h"

#ifdef __cplusplus
extern "C"{
#endif//__cplusplus

inline uint64 read_sstatus(){
    uint64 sstatus;
    asm volatile("csrr %0, sstatus" : "=r" (sstatus));
    return sstatus;
}

inline void write_sstatus(uint64 sstatus){
    asm volatile("csrw sstatus, %0" :: "r" (sstatus));
}

inline uint64 read_sedeleg(){
    uint64 sedeleg;
    asm volatile("csrr %0, sedeleg" : "=r" (sedeleg));
    return sedeleg;
}

inline void write_sedeleg(uint64 sedeleg){
    asm volatile("csrw sedeleg, %0" :: "r" (sedeleg));
}

inline uint64 read_sideleg(){
    uint64 sideleg;
    asm volatile("csrr %0, sideleg" : "=r" (sideleg));
    return sideleg;
}

inline void write_sideleg(uint64 sideleg){
    asm volatile("csrw sideleg, %0" :: "r" (sideleg));
}

inline uint64 read_sie(){
    uint64 sie;
    asm volatile("csrr %0, sie" : "=r" (sie));
    return sie;
}

inline void write_sie(uint64 sie){
    asm volatile("csrw sie, %0" :: "r" (sie));
}

inline uint64 read_stvec(){
    uint64 stvec;
    asm volatile("csrr %0, stvec" : "=r" (stvec));
    return stvec;
}

inline void write_stvec(uint64 stvec){
    asm volatile("csrw stvec, %0" :: "r" (stvec));
}

inline uint64 read_scounteren(){
    uint64 scounteren;
    asm volatile("csrr %0, scounteren" : "=r" (scounteren));
    return scounteren;
}

inline void write_scounteren(uint64 scounteren){
    asm volatile("csrw scounteren, %0" :: "r" (scounteren));
}

inline uint64 read_sscratch(){
    uint64 sscratch;
    asm volatile("csrr %0, sscratch" : "=r" (sscratch));
    return sscratch;
}

inline void write_sscratch(uint64 sscratch){
    asm volatile("csrw sscratch, %0" :: "r" (sscratch));
}

inline uint64 read_sepc(){
    uint64 sepc;
    asm volatile("csrr %0, sepc" : "=r" (sepc));
    return sepc;
}

inline void write_sepc(uint64 sepc){
    asm volatile("csrw sepc, %0" :: "r" (sepc));
}

inline uint64 read_scause(){
    uint64 scause;
    asm volatile("csrr %0, scause" : "=r" (scause));
    return scause;
}

inline void write_scause(uint64 scause){
    asm volatile("csrw scause, %0" :: "r" (scause));
}

inline uint64 read_stval(){
    uint64 stval;
    asm volatile("csrr %0, stval" : "=r" (stval));
    return stval;
}

inline void write_stval(uint64 stval){
    asm volatile("csrw stval, %0" :: "r" (stval));
}

inline uint64 read_sip(){
    uint64 sip;
    asm volatile("csrr %0, sip" : "=r" (sip));
    return sip;
}

inline void write_sip(uint64 sip){
    asm volatile("csrw sip, %0" :: "r" (sip));
}

inline uint64 read_satp(){
    uint64 satp;
    asm volatile("csrr %0, satp" : "=r" (satp));
    return satp;
}

inline void write_satp(uint64 satp){
    asm volatile("csrw satp, %0" :: "r" (satp));
}

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_OS1_RISCV_RISCV_H