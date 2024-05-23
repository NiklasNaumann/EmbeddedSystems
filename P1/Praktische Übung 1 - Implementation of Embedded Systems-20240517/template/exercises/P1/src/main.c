#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <educore-v.h>
#include <utils/delay.h>

#include "tcs34725.h"


void    port_setup(void);

void    i2c_start();
uint8_t i2c_read(bool ack);
bool    i2c_write(uint8_t value);
void    i2c_stop();

bool    pmod_init(void);
uint8_t pmod_read(uint8_t reg_address);
void    pmod_write(uint8_t reg_address, uint8_t value);



void port_setup(void) {
	// TODO: Configure the PWM-Controller and the I²C-Controller to use the correct pins.
    // Make sure to also set the PWM output to 0 and top any I2C transactions that could still be open.
    mmio->port_jb.io_mux.pin1 = IO_MUX_PWM;
    mmio->port_jb.io_mux.pin2 = IO_MUX_SCL;
    mmio->port_jb.io_mux.pin3 = IO_MUX_SDA;

    mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_STOP;
    mmio->port_jb.pwm.duty_cycle = 0;
    
}

void i2c_start() {
    // TODO: Set the correct command bit and start an operation. Then wait for the controller to finish.
    mmio->port_jb.i2c.ctrl = I2C_CTRL_ENABLE;
    mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_START;
    while(mmio->port_jb.i2c.ctrl & I2C_CTRL_BUSY) {}
}

void i2c_stop() {
    // TODO: Set the correct command bit and start an operation. Then wait for the controller to finish.
    mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_STOP;
    while(mmio->port_jb.i2c.ctrl & I2C_CTRL_BUSY) {}
}

uint8_t i2c_read(bool ack) {
    // TODO: Read and send the (N)ACK, wait for the operation to finish and return the value that was read.
	if(ack) {
        mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_READ_ACK;
    }
    else {
        mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_READ_NACK;
    }
    while(mmio->port_jb.i2c.ctrl & I2C_CTRL_BUSY) {}
    return mmio->port_jb.i2c.rdata;
}

bool i2c_write(uint8_t value) {
    // TODO: Write "value" into the designated register. After that, the operation can be started and waited for its completion. A Boolean must be returned, indicating whether an ACK was received.
	mmio->port_jb.i2c.wdata = value;
    mmio->port_jb.i2c.ctrl = I2C_CTRL_CMD_WRITE;
    while(mmio->port_jb.i2c.ctrl & I2C_CTRL_BUSY) {}
    return mmio->port_jb.i2c.ctrl & I2C_CTRL_RCV_ACK;
}

uint8_t pmod_read(uint8_t reg_address) {
	// TODO: Read a value from the register at reg_address of the TCS34725. The device address can be hard-coded into the function but the register is passed as a parameter. Don’t forget to start and stop the transactions.

}

void pmod_write(uint8_t reg_address, uint8_t value) {
	// TODO: Write a value to the register at reg_address of the TCS34725. The device address can be hard-coded into the function but the register is passed along with the value to be written. Don’t forget to start and stop the transactions.

}

bool pmod_init(void) {
    /* TODO: Implement the initialization routine of the sensor. First call the previously implemented port_setup() function. 
	Then read the ID register of the sensor and print it to the console using printf. Also check if the ID equals 0x44 and exit the function returning false if that is not the case. 
	Then set the integration time and gain (ATIME, CONTROL). Use the lowest settings first and experiment with different values later. 
	Then set the PON flag in the ENABLE register to power on the sensor and wait 3ms using the _delay_ms function. Then also set the AEN flag in the ENABLE register to enable the Analog-Digital-Converter inside the sensor and again wait 3ms. 
	Use the previously implemented pmod_read() and pmod_write() functions and the constants in the 'tcs34725.h' file to communicate with the sensor.
	*/
    
    return true;
}

int main(void)
{
    _delay_ms(500);
    uint32_t sum = 0;
    for (int i=0; i<128*96; i++) {
        vga->bitmap[i] = i;
    }
    printf("Done!!! %08x\n",sum);
    // initialize the sensor
    pmod_init();

    while (1)
    {
        // set the switch state as the duty cycle of the pwm pin
        mmio->port_jb.pwm.duty_cycle = //TODO

        // set the max brightness depending on the switch states
        uint16_t maxBrightness = //TODO

        // read in the values from the sensor
        uint8_t value_l = //TODO
        uint8_t value_h = //TODO
        // calculate the combined sensor value
        uint16_t value = //TODO

        // print the value to the 7-seg display
        uint8_t digits[] = {
            0b00111111, 0b00000110, 0b01011011, 0b01001111,
            0b01100110, 0b01101101, 0b01111101, 0b00000111,
            0b01111111, 0b01101111, 0b01110111, 0b01111100,
            0b00111001, 0b01011110, 0b01111001, 0b01110001
        };
       //TODO

        // map value from [0, maxBrightness] to [0, 15] ...
        uint8_t mappedValue = //TODO
        // ... and turn on the corresponding LED
        //TODO

        // check if the read value is greater that maxBrightness
        // and change the LEDs and 7-seg display accordingly
        //TODO

        // wait a little bit before reading in the next values
        _delay_ms(30);
    }
}

