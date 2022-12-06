//This module handles general I2C communications between the beagle bone and peripherals 

#ifndef I2C
#define I2C

//Module for all I2C functionality (read, write, init)

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x08

#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

int I2C_initBus(char* bus, int address);
void I2C_writeReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);

#endif