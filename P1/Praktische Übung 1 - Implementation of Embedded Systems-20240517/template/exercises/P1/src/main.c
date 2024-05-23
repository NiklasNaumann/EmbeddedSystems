#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <educore-v.h>
#include <utils/delay.h>

#include "tcs34725.h"

void port_setup(void);

void i2c_start();
uint8_t i2c_read(bool ack);
bool i2c_write(uint8_t value);
void i2c_stop();

bool pmod_init(void);
uint8_t pmod_read(uint8_t reg_address);
void pmod_write(uint8_t reg_address, uint8_t value);

void port_setup(void)
{
    // TODO: Configure the PWM-Controller and the I²C-Controller to use the correct pins.
    // Make sure to also set the PWM output to 0 and top any I2C transactions that could still be open.
    mmio->port_jb.io_mux.pin1 = IO_MUX_PWM;
    mmio->port_jb.io_mux.pin2 = IO_MUX_SCL;
    mmio->port_jb.io_mux.pin3 = IO_MUX_SDA;

    mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_STOP;
    mmio->port_jb.pwm.duty_cycle = 0;
}

void i2c_start()
{
    // TODO: Set the correct command bit and start an operation. Then wait for the controller to finish.
    mmio->port_jb.i2c.ctrl = I2C_CTRL_ENABLE | I2C_CTRL_CMD_START;
    while (mmio->port_jb.i2c.ctrl & I2C_CTRL_BUSY)
    {
    }
}

void i2c_stop()
{
    // TODO: Set the correct command bit and start an operation. Then wait for the controller to finish.
    mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_STOP;
    while (mmio->port_jb.i2c.ctrl & I2C_CTRL_BUSY)
    {
    }
}

uint8_t i2c_read(bool ack)
{
    // TODO: Read and send the (N)ACK, wait for the operation to finish and return the value that was read.
    if (ack)
    {
        mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_READ_ACK;
    }
    else
    {
        mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_READ_NACK;
    }
    while (mmio->port_jb.i2c.ctrl & I2C_CTRL_BUSY)
    {
    }
    return mmio->port_jb.i2c.rdata;
}

bool i2c_write(uint8_t value)
{
    // TODO: Write "value" into the designated register. After that, the operation can be started and waited for its completion. A Boolean must be returned, indicating whether an ACK was received.
    mmio->port_jb.i2c.wdata = value;
    mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_WRITE;
    while (mmio->port_jb.i2c.ctrl & I2C_CTRL_BUSY)
    {
    }
    return mmio->port_jb.i2c.ctrl & I2C_CTRL_RCV_ACK;
}

uint8_t pmod_read(uint8_t reg_address)
{
    // TODO: Read a value from the register at reg_address of the TCS34725. The device address can be hard-coded into the function but the register is passed as a parameter. Don’t forget to start and stop the transactions.
    uint8_t data;

    // I2C Read Protocol - Combined Format
    // Send start condition
    i2c_start();

    // Write device address
    data = TCS34725_ADDRESS;
    if (!i2c_write(data))
        return 0;

    // Write target register
    data = TCS34725_COMMAND_BIT | reg_address;
    if (!i2c_write(data))
        return 0;

    // Repeat start condition
    i2c_start();

    // Read data
    data = i2c_read(true);

    // Send stop condition and return data
    i2c_stop();
    return data;
}

void pmod_write(uint8_t reg_address, uint8_t value)
{
    // TODO: Write a value to the register at reg_address of the TCS34725. The device address can be hard-coded into the function but the register is passed along with the value to be written. Don’t forget to start and stop the transactions.
    uint8_t data;

    // I2C Write Protocol
    // Send start condition
    i2c_start();

    // Write device address
    data = TCS34725_ADDRESS;
    if (!i2c_write(data))
        return;

    // Write target register
    data = TCS34725_COMMAND_BIT | reg_address;
    if (!i2c_write(data))
        return;

    // Write data
    i2c_write(value);

    // Send stop condition
    i2c_stop();
}

bool pmod_init(void)
{
    /* TODO: Implement the initialization routine of the sensor. First call the previously implemented port_setup() function.
    Then read the ID register of the sensor and print it to the console using printf. Also check if the ID equals 0x44 and exit the function returning false if that is not the case.
    Then set the integration time and gain (ATIME, CONTROL). Use the lowest settings first and experiment with different values later.
    Then set the PON flag in the ENABLE register to power on the sensor and wait 3ms using the _delay_ms function. Then also set the AEN flag in the ENABLE register to enable the Analog-Digital-Converter inside the sensor and again wait 3ms.
    Use the previously implemented pmod_read() and pmod_write() functions and the constants in the 'tcs34725.h' file to communicate with the sensor.
    */
    uint8_t data;
    port_setup();

    // Read and print device ID
    data = pmod_read(TCS34725_ID);
    printf("TCS34725 ID: %u", data);
    if (data != 0x44)
    {
        return false;
    }

    // Integration time
    pmod_write(TCS34725_ATIME, TCS34725_ATIME_2_4MS);

    // Gain
    pmod_write(TCS34725_CONTROL, TCS34725_CONTROL_GAIN_1X);

    // PON and AEN
    pmod_write(TCS34725_ENABLE, TCS34725_ENABLE_PON);
    _delay_ms(3);
    pmod_write(TCS34725_ENABLE, TCS34725_ENABLE_AEN);
    _delay_ms(3);

    return true;
}

int main(void)
{
    _delay_ms(500);
    uint32_t sum = 0;
    for (int i = 0; i < 128 * 96; i++)
    {
        vga->bitmap[i] = i;
    }
    printf("Done!!! %08x\n", sum);
    // initialize the sensor
    pmod_init();

    while (1)
    {
        // set the switch state as the duty cycle of the pwm pin
        mmio->port_jb.pwm.duty_cycle = mmio->sw >> 8;

        // set the max brightness depending on the switch states
        uint16_t maxBrightness = mmio->sw & 0xFF;

        // read in the values from the sensor
        uint8_t value_l = pmod_read(TCS34725_CDATAL);
        uint8_t value_h = pmod_read(TCS34725_CDATAH);
        // calculate the combined sensor value
        uint16_t value = ((uint16_t)value_h) << 8 | value_l;

        // print the value to the 7-seg display
        uint8_t digits[] = {
            0b00111111, 0b00000110, 0b01011011, 0b01001111,
            0b01100110, 0b01101101, 0b01111101, 0b00000111,
            0b01111111, 0b01101111, 0b01110111, 0b01111100,
            0b00111001, 0b01011110, 0b01111001, 0b01110001};
        mmio->seg[0] = digits[value & 0xF];
        mmio->seg[1] = digits[(value >> 4) & 0xF];
        mmio->seg[2] = digits[(value >> 8) & 0xF];
        mmio->seg[3] = digits[(value >> 12) & 0xF];

        // map value from [0, maxBrightness] to [0, 15] ...
        uint8_t mappedValue = value * 15 / maxBrightness;
        // ... and turn on the corresponding LED
        if (mappedValue > 0)
        {
            mmio->led = 1 << (mappedValue - 1);
        }
        else
        {
            mmio->led = 0;
        }

        // check if the read value is greater that maxBrightness
        // and change the LEDs and 7-seg display accordingly
        if (value > maxBrightness) {
            mmio->led = 0xFFFF;
            mmio->seg[0] = 0b01000000;
            mmio->seg[1] = 0b01000000;
            mmio->seg[2] = 0b01000000;
            mmio->seg[3] = 0b01000000;
        }

        // wait a little bit before reading in the next values
        _delay_ms(30);
    }
}
