#include <string.h>
#include <stdint.h>
#include <educore-v.h>


void uart_write(char c) {
    mmio->uart.wdata = c;
    mmio->uart.ctrl |= UART_CTRL_ENABLE;
    while ((mmio->uart.ctrl & UART_CTRL_BUSY) != 0);
}

static void _uart_write_string(char *str) {
    while (*str) uart_write(*str++);
}

static void _uart_write_int8(uint8_t num) {
    uart_write("0123456789ABCDEF"[(num >> 4) & 0xF]);
    uart_write("0123456789ABCDEF"[(num >> 0) & 0xF]);
}

static void _uart_write_int16(uint16_t num) {
    _uart_write_int8((num >> 8) & 0xFF);
    _uart_write_int8((num >> 0) & 0xFF);
}

static void _uart_write_int32(uint32_t num) {
    _uart_write_int16((num >> 16) & 0xFFFF);
    _uart_write_int16((num >>  0) & 0xFFFF);
}

void default_exception_isr() {
    csr_clear(mstatus, MSTATUS_MIE);

    _uart_write_string("Exception occured:\n");
    _uart_write_string("  mcause: ");
    _uart_write_int32 (csr_read(mcause));
    _uart_write_string("\n");
    _uart_write_string("  mepc: ");
    _uart_write_int32 (csr_read(mepc));
    _uart_write_string("\n");
    _uart_write_string("  mtval: ");
    _uart_write_int32 (csr_read(mtval));
    _uart_write_string("\n");

    while (1);
}

mmio_t mmio = (mmio_t)((uint8_t *) 0x10000000);
vga_t  vga  = (vga_t )((uint8_t *) 0x10010000);

void init() {
    extern void main();

    csr_clear(mstatus, MSTATUS_MIE);
    csr_write(mie, 0);
    csr_write(mtvec, &default_exception_isr);

    mmio->led = 0;
    mmio->seg_w = 0;
    
    mmio->btn_conf_n = 0;
    mmio->btn_conf_p = 0;
    mmio->btn_latch = 0xFF;

    mmio->uart.ctrl = UART_CTRL_ACK_IRQ;

    mmio->port_ja.io_mux.raw = 0;
    mmio->port_ja.dir = 0;
    mmio->port_ja.pwm.duty_cycle = 0;
    mmio->port_ja.i2c.ctrl = I2C_CTRL_CMD_STOP | I2C_CTRL_ENABLE;
    
    mmio->port_jb.io_mux.raw = 0;
    mmio->port_jb.dir = 0;
    mmio->port_jb.pwm.duty_cycle = 0;
    mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_STOP | I2C_CTRL_ENABLE;
    
    mmio->port_jc.io_mux.raw = 0;
    mmio->port_jc.dir = 0;
    mmio->port_jc.pwm.duty_cycle = 0;
    mmio->port_jc.i2c.ctrl = I2C_CTRL_CMD_STOP | I2C_CTRL_ENABLE;
    
    mmio->mtimecmp_h = -1;
    mmio->mtimecmp_l = -1;

    mmio->software_irq = 0;
    
    vga->bitmap_mode = 0;
    memset(vga->tiles, 0x00, 32*8);
    memset(vga->bg_layer, 0x00, 32*24);
    memset(vga->bg_color, 0xF0, 32*24);
    memset(vga->bitmap, 0x00, 128*96);

    main();    
}