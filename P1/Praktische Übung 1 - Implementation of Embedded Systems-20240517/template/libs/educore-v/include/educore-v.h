#ifndef __EDUCORE_V_H__
#define __EDUCORE_V_H__

#include <stdint.h>
#include <risc-v.h>


#define S1(x, y) x ## y
#define S2(x, y) S1(x, y)
#define UNUSED(num) const uint8_t S2(__unused_, __LINE__)[num]


// --------------- UART ---------------

#define UART_CTRL_ENABLE   (1 << 0)
#define UART_CTRL_BUSY     (1 << 1)
#define UART_CTRL_VALID    (1 << 2)
#define UART_CTRL_BREAK    (1 << 3)
#define UART_CTRL_ACK_IRQ  (1 << 4)

typedef struct {
    // 0, 0, 0, ack_irq (rw), break (ro), ready (ro), busy (ro), enable (rw)
    volatile uint8_t ctrl; 
    volatile uint8_t wdata;
    volatile uint8_t rdata;
    UNUSED(1);
} uart_t;


// --------------- I2C ----------------

#define I2C_CTRL_ENABLE        (1 << 0)
#define I2C_CTRL_BUSY          (1 << 1)
#define I2C_CTRL_ACTIVE        (1 << 2)
#define I2C_CTRL_RCV_ACK       (1 << 3)
#define I2C_CTRL_CMD_NOP       (0 << 5)
#define I2C_CTRL_CMD_START     (1 << 5)
#define I2C_CTRL_CMD_STOP      (2 << 5)
#define I2C_CTRL_CMD_WRITE     (5 << 5)
#define I2C_CTRL_CMD_READ_ACK  (6 << 5)
#define I2C_CTRL_CMD_READ_NACK (7 << 5)

typedef struct {
    // command[3] (rw), 0, ack (ro), active (ro), busy (ro), enable (rw)
    volatile uint8_t ctrl;
    volatile uint8_t wdata;
    volatile const uint8_t rdata;
    UNUSED(1);
} i2c_t;


// --------------- PWM ----------------

typedef struct {
    // Between 0 and 65535
    volatile uint16_t duty_cycle;
    UNUSED(2);
} pwm_t;


// --------------- Port ---------------

#define IO_MUX_PIN   0 
#define IO_MUX_SCL   1 
#define IO_MUX_SDA   2 
#define IO_MUX_PWM   3 

typedef struct {
    volatile uint8_t dir;
    volatile uint8_t wdata;
    volatile const uint8_t rdata;

    UNUSED(1);

    union {
        uint32_t raw;
        struct {
            uint32_t pin0: 4;
            uint32_t pin1: 4;
            uint32_t pin2: 4;
            uint32_t pin3: 4;
            uint32_t pin4: 4;
            uint32_t pin5: 4;
            uint32_t pin6: 4;
            uint32_t pin7: 4;
        }; 
    } io_mux;
    
    i2c_t i2c;
    pwm_t pwm;
} port_t;


// --------------- MMIO ---------------

typedef struct {
    volatile uint16_t led;
    UNUSED(2);
    volatile const uint16_t sw;
    UNUSED(2);
    volatile const uint8_t btn;
    volatile uint8_t btn_latch;
    volatile uint8_t btn_conf_p;
    volatile uint8_t btn_conf_n;
    union {
        volatile uint32_t seg_w;
        volatile uint8_t  seg[4];
    };
    port_t  port_ja;
    port_t  port_jb;
    port_t  port_jc;
    uart_t  uart;
    UNUSED(24);
    volatile const uint32_t mtime_l;
    volatile const uint32_t mtime_h;
    volatile uint32_t mtimecmp_l;
    volatile uint32_t mtimecmp_h;
    volatile uint32_t software_irq;
} * const mmio_t; 


// --------------- VGA ----------------

typedef struct  {
    uint8_t tiles[0x800];

    uint8_t bg_layer[0x300];
    UNUSED(0x100);

    uint8_t bg_color[0x300];
    UNUSED(0xFF);
    
    // 0: off, 1: on
    uint8_t bitmap_mode;
    uint8_t bitmap[0x3000];
} * const vga_t;


// ----------- Declaration ------------

extern mmio_t mmio;
extern vga_t  vga;

#undef S1
#undef S2
#undef UNUSED

#endif // __EDUCORE_V_H__
