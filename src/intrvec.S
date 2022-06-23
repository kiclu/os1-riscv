.globl intrvec
.globl intr_handler
.globl running
.globl kernel_thread
.align 4
intrvec:
    # save t0 to sscratch
    csrw sscratch, t0

    # load running->context to t0
    ld t0, running

    # save context
    sd ra,  0x00(t0)
    sd sp,  0x08(t0)
    sd gp,  0x10(t0)
    #sd tp,  0x18(t0)
    # save t0 later
    sd t1,  0x28(t0)
    sd t2,  0x30(t0)
    sd s0,  0x38(t0)
    sd s1,  0x40(t0)
    sd a0,  0x48(t0)
    sd a1,  0x50(t0)
    sd a2,  0x58(t0)
    sd a3,  0x60(t0)
    sd a4,  0x68(t0)
    sd a5,  0x70(t0)
    sd a6,  0x78(t0)
    sd a7,  0x80(t0)
    sd s2,  0x88(t0)
    sd s3,  0x90(t0)
    sd s4,  0x98(t0)
    sd s5,  0xA0(t0)
    sd s6,  0xA8(t0)
    sd s7,  0xB0(t0)
    sd s8,  0xB8(t0)
    sd s9,  0xC0(t0)
    sd s10, 0xC8(t0)
    sd s11, 0xD0(t0)
    sd t3,  0xD8(t0)
    sd t4,  0xE0(t0)
    sd t5,  0xE8(t0)
    sd t6,  0xF0(t0)

    # save t0
    ld t1, running
    csrr t0, sscratch
    sd t0, 0x20(t1)

    # restore t1
    ld t1, 0x28(t1)

    ld sp, kernel_thread
    ld sp, 0x08(sp)
    
    call intr_handler

    ld a0, kernel_thread
    sd sp, 0x08(a0)

    # load running->context to t0
    ld t0, running

    # load context
    ld ra,  0x00(t0)
    ld sp,  0x08(t0)
    ld gp,  0x10(t0)
    #ld tp,  0x18(t0)
    # load t0 later
    ld t1,  0x28(t0)
    ld t2,  0x30(t0)
    ld s0,  0x38(t0)
    ld s1,  0x40(t0)
    ld a0,  0x48(t0)
    ld a1,  0x50(t0)
    ld a2,  0x58(t0)
    ld a3,  0x60(t0)
    ld a4,  0x68(t0)
    ld a5,  0x70(t0)
    ld a6,  0x78(t0)
    ld a7,  0x80(t0)
    ld s2,  0x88(t0)
    ld s3,  0x90(t0)
    ld s4,  0x98(t0)
    ld s5,  0xA0(t0)
    ld s6,  0xA8(t0)
    ld s7,  0xB0(t0)
    ld s8,  0xB8(t0)
    ld s9,  0xC0(t0)
    ld s10, 0xC8(t0)
    ld s11, 0xD0(t0)
    ld t3,  0xD8(t0)
    ld t4,  0xE0(t0)
    ld t5,  0xE8(t0)
    ld t6,  0xF0(t0)

    ld t0, 0x20(t0)
    sret
