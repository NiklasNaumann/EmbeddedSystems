.section .init
.globl _start
.extern _premain

_start:
// setup gp and sp
.option push
.option norelax
    la    gp, __global_pointer$
.option pop
    la    sp, 0x10000

// clear all register
    mv    tp,  zero

    mv    t0,  zero
    mv    t1,  zero
    mv    t2,  zero
    mv    t3,  zero
    mv    t4,  zero
    mv    t5,  zero
    mv    t6,  zero

    mv    a0,  zero
    mv    a1,  zero
    mv    a2,  zero
    mv    a3,  zero
    mv    a4,  zero
    mv    a5,  zero
    mv    a6,  zero
    mv    a7,  zero

    mv    s0,  zero
    mv    s1,  zero
    mv    s2,  zero
    mv    s3,  zero
    mv    s4,  zero
    mv    s5,  zero
    mv    s6,  zero
    mv    s7,  zero
    mv    s8,  zero
    mv    s9,  zero
    mv    s10, zero
    mv    s11, zero

// setup the stack
    auipc ra, 0
    addi  sp, sp, -16
    sw    ra, 8(sp)

// jump to start
    j     init
