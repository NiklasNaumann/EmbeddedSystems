#ifndef __RISC_V_H__
#define __RISC_V_H__


#define MSTATUS_MIE  (1 <<  3)

#define MIE_MSIE     (1 <<  3)
#define MIE_MTIE     (1 <<  7)
#define MIE_MEIE     (1 << 11)
#define MIE_MLIE_0   (1 << 16)
#define MIE_MBIE     MIE_MLIE_0
#define MIE_MLIE_1   (1 << 17)
#define MIE_MUartIE  MIE_MLIE_1
#define MIE_MLIE_2   (1 << 18)
#define MIE_MLIE_3   (1 << 19)
#define MIE_MLIE_4   (1 << 20)
#define MIE_MLIE_5   (1 << 21)
#define MIE_MLIE_6   (1 << 22)
#define MIE_MLIE_7   (1 << 23)
#define MIE_MLIE_8   (1 << 24)
#define MIE_MLIE_9   (1 << 25)
#define MIE_MLIE_10  (1 << 26)
#define MIE_MLIE_11  (1 << 27)
#define MIE_MLIE_12  (1 << 28)
#define MIE_MLIE_13  (1 << 29)
#define MIE_MLIE_14  (1 << 30)
#define MIE_MLIE_15  (1 << 31)

#define MIP_MSIP     MIE_MSIE
#define MIP_MTIP     MIE_MTIE
#define MIP_MEIP     MIE_MEIE
#define MIP_MLIP_0   MIE_MLIE_0
#define MIP_MBIP     MIE_MBIE
#define MIP_MLIP_1   MIE_MLIE_1
#define MIP_MUartIP  MIE_MUartIE
#define MIP_MLIP_2   MIE_MLIE_2
#define MIP_MLIP_3   MIE_MLIE_3
#define MIP_MLIP_4   MIE_MLIE_4
#define MIP_MLIP_5   MIE_MLIE_5
#define MIP_MLIP_6   MIE_MLIE_6
#define MIP_MLIP_7   MIE_MLIE_7
#define MIP_MLIP_8   MIE_MLIE_8
#define MIP_MLIP_9   MIE_MLIE_9
#define MIP_MLIP_10  MIE_MLIE_10
#define MIP_MLIP_11  MIE_MLIE_11
#define MIP_MLIP_12  MIE_MLIE_12
#define MIP_MLIP_13  MIE_MLIE_13
#define MIP_MLIP_14  MIE_MLIE_14
#define MIP_MLIP_15  MIE_MLIE_15

extern void default_exception_isr();

#define nop     __asm__ __volatile__("nop")
#define wfi     __asm__ __volatile__("wfi")
#define ecall   __asm__ __volatile__("ecall")
#define ebreak  __asm__ __volatile__("ebreak")

#define csr_swap(csr, val)                                        \
({                                                                \
        unsigned long __v = (unsigned long)(val);                 \
        __asm__ __volatile__ ("csrrw %0, " #csr ", %1"            \
                              : "=r" (__v) : "rK" (__v));         \
        __v;                                                      \
})

#define csr_read(csr)                                             \
({                                                                \
        register unsigned long __v;                               \
        __asm__ __volatile__ ("csrr %0, " #csr                    \
                              : "=r" (__v));                      \
        __v;                                                      \
})

#define csr_write(csr, val)                                       \
({                                                                \
        unsigned long __v = (unsigned long)(val);                 \
        __asm__ __volatile__ ("csrw " #csr ", %0"                 \
                              : : "rK" (__v));                    \
})

#define csr_read_set(csr, val)                                    \
({                                                                \
        unsigned long __v = (unsigned long)(val);                 \
        __asm__ __volatile__ ("csrrs %0, " #csr ", %1"            \
                              : "=r" (__v) : "rK" (__v));         \
        __v;                                                      \
})

#define csr_set(csr, val)                                         \
({                                                                \
        unsigned long __v = (unsigned long)(val);                 \
        __asm__ __volatile__ ("csrs " #csr ", %0"                 \
                              : : "rK" (__v));                    \
})

#define csr_read_clear(csr, val)                                  \
({                                                                \
        unsigned long __v = (unsigned long)(val);                 \
        __asm__ __volatile__ ("csrrc %0, " #csr ", %1"            \
                              : "=r" (__v) : "rK" (__v));         \
        __v;                                                      \
})

#define csr_clear(csr, val)                                       \
({                                                                \
        unsigned long __v = (unsigned long)(val);                 \
        __asm__ __volatile__ ("csrc " #csr ", %0"                 \
                              : : "rK" (__v));                    \
})

#endif // __RISC_V_H__
