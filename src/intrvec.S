.globl intrvec
.globl intrret
.globl intr_handler

.globl kintrvec
.globl kintr_handler

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
    sd tp,  0x18(t0)
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

    jal intr_handler

intrret:
    ld a0, kernel_thread
    sd sp, 0x08(a0)

    # load running->context to t0
    ld t0, running

    # load context
    ld ra,  0x00(t0)
    ld sp,  0x08(t0)
    ld gp,  0x10(t0)
    ld tp,  0x18(t0)
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


kintrvec:
    addi sp, sp, -256
    sd ra,  0x00(sp)
    sd sp,  0x08(sp)
    sd gp,  0x10(sp)
    sd tp,  0x18(sp)
    sd t0,  0x20(sp)
    sd t1,  0x28(sp)
    sd t2,  0x30(sp)
    sd s0,  0x38(sp)
    sd s1,  0x40(sp)
    sd a0,  0x48(sp)
    sd a1,  0x50(sp)
    sd a2,  0x58(sp)
    sd a3,  0x60(sp)
    sd a4,  0x68(sp)
    sd a5,  0x70(sp)
    sd a6,  0x78(sp)
    sd a7,  0x80(sp)
    sd s2,  0x88(sp)
    sd s3,  0x90(sp)
    sd s4,  0x98(sp)
    sd s5,  0xA0(sp)
    sd s6,  0xA8(sp)
    sd s7,  0xB0(sp)
    sd s8,  0xB8(sp)
    sd s9,  0xC0(sp)
    sd s10, 0xC8(sp)
    sd s11, 0xD0(sp)
    sd t3,  0xD8(sp)
    sd t4,  0xE0(sp)
    sd t5,  0xE8(sp)
    sd t6,  0xF0(sp)

    call kintr_handler

    ld ra,  0x00(sp)
    ld sp,  0x08(sp)
    ld gp,  0x10(sp)
    ld tp,  0x18(sp)
    ld t0,  0x20(sp)
    ld t1,  0x28(sp)
    ld t2,  0x30(sp)
    ld s0,  0x38(sp)
    ld s1,  0x40(sp)
    ld a0,  0x48(sp)
    ld a1,  0x50(sp)
    ld a2,  0x58(sp)
    ld a3,  0x60(sp)
    ld a4,  0x68(sp)
    ld a5,  0x70(sp)
    ld a6,  0x78(sp)
    ld a7,  0x80(sp)
    ld s2,  0x88(sp)
    ld s3,  0x90(sp)
    ld s4,  0x98(sp)
    ld s5,  0xA0(sp)
    ld s6,  0xA8(sp)
    ld s7,  0xB0(sp)
    ld s8,  0xB8(sp)
    ld s9,  0xC0(sp)
    ld s10, 0xC8(sp)
    ld s11, 0xD0(sp)
    ld t3,  0xD8(sp)
    ld t4,  0xE0(sp)
    ld t5,  0xE8(sp)
    ld t6,  0xF0(sp)

    addi sp, sp, 256

    sret