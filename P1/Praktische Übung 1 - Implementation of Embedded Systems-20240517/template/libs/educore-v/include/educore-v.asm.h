#ifndef __EDUCORE_V_ASM_H__
#define __EDUCORE_V_ASM_H__


#define MMIO_BASE_ADDR              (0x10000000)

// #define MMIO_LED_OFFSET             (0x00000000)
// #define MMIO_LED_L_OFFSET           (MMIO_LED_OFFSET + 0)
// #define MMIO_LED_H_OFFSET           (MMIO_LED_OFFSET + 1)

// #define MMIO_SWITCH_OFFSET          (0x00000004)
// #define MMIO_SWITCH_L_OFFSET        (MMIO_SWITCH_OFFSET + 0)
// #define MMIO_SWITCH_H_OFFSET        (MMIO_SWITCH_OFFSET + 1)

// #define MMIO_SEG_OFFSET             (0x00000008)
// #define MMIO_SEG_LENGTH             (4)
// #define MMIO_SEG_0_OFFSET           (MMIO_SEG_OFFSET + 0)
// #define MMIO_SEG_1_OFFSET           (MMIO_SEG_OFFSET + 1)
// #define MMIO_SEG_2_OFFSET           (MMIO_SEG_OFFSET + 2)
// #define MMIO_SEG_3_OFFSET           (MMIO_SEG_OFFSET + 3)

// #define MMIO_BUTTON_OFFSET          (0x0000000C)
// #define MMIO_BUTTON_LATCH_OFFSET    (MMIO_BUTTON_OFFSET + 1)
// #define MMIO_BUTTON_P_CONF_OFFSET   (MMIO_BUTTON_OFFSET + 2)
// #define MMIO_BUTTON_N_CONF_OFFSET   (MMIO_BUTTON_OFFSET + 3)


// #define MMIO_JA_DDR_OFFSET             (0x00000010)
// #define MMIO_JA_PORT_OFFSET            (0x00000011)
// #define MMIO_JA_PIN_OFFSET             (0x00000012)
// #define MMIO_JA_PWM_ENABLE_OFFSET      (0x00000014)
// #define MMIO_JA_PWM_DUTY_CYCLE_OFFSET  (0x00000015)
// #define MMIO_JA_I2C_CTRL_OFFSET        (0x00000018)
// #define MMIO_JA_I2C_ADDR_OFFSET        (0x00000019)
// #define MMIO_JA_I2C_WDATA_OFFSET       (0x0000001A)
// #define MMIO_JA_I2C_RDATA_OFFSET       (0x0000001B)

// #define MMIO_JB_DDR_OFFSET             (0x00000020)
// #define MMIO_JB_PORT_OFFSET            (0x00000021)
// #define MMIO_JB_PIN_OFFSET             (0x00000022)
// #define MMIO_JB_PWM_ENABLE_OFFSET      (0x00000024)
// #define MMIO_JB_PWM_DUTY_CYCLE_OFFSET  (0x00000025)
// #define MMIO_JB_I2C_CTRL_OFFSET        (0x00000028)
// #define MMIO_JB_I2C_ADDR_OFFSET        (0x00000029)
// #define MMIO_JB_I2C_WDATA_OFFSET       (0x0000002A)
// #define MMIO_JB_I2C_RDATA_OFFSET       (0x0000002B)

// #define MMIO_JC_DDR_OFFSET             (0x00000030)
// #define MMIO_JC_PORT_OFFSET            (0x00000031)
// #define MMIO_JC_PIN_OFFSET             (0x00000032)
// #define MMIO_JC_PWM_ENABLE_OFFSET      (0x00000034)
// #define MMIO_JC_PWM_DUTY_CYCLE_OFFSET  (0x00000035)
// #define MMIO_JC_I2C_CTRL_OFFSET        (0x00000038)
// #define MMIO_JC_I2C_ADDR_OFFSET        (0x00000039)
// #define MMIO_JC_I2C_WDATA_OFFSET       (0x0000003A)
// #define MMIO_JC_I2C_RDATA_OFFSET       (0x0000003B)


// #define MMIO_UART_CTRL_OFFSET       (0x00000040)
// #define MMIO_UART_WDATA_OFFSET      (0x00000041)
// #define MMIO_UART_RDATA_OFFSET      (0x00000042)


#define MMIO_MTIME_L_OFFSET         (0x00000050)
#define MMIO_MTIME_H_OFFSET         (MMIO_MTIME_L_OFFSET + 4)
#define MMIO_MTIMECMP_L_OFFSET      (0x00000058)
#define MMIO_MTIMECMP_H_OFFSET      (MMIO_MTIMECMP_L_OFFSET + 4)

#define MMIO_MSIP_OFFSET            (0x00000060)


#endif // __EDUCORE_V_ASM_H__
