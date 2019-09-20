#ifndef I2C_H_
#define I2C_H_

#ifndef ADI_UART_H
#include <drivers/i2c/adi_i2c.h>
#endif

#define CLOCK_ADDRESS 0x68

/*#ifdef ADI_I2C_CFG_MCTL_STRETCHSCL
#undef ADI_I2C_CFG_MCTL_STRETCHSCL
#define ADI_I2C_CFG_MCTL_STRETCHSCL (1)
#endif

#ifdef ADI_I2C_CFG_ASTRETCH_MST
#undef ADI_I2C_CFG_ASTRETCH_MST
#define ADI_I2C_CFG_ASTRETCH_MST (0b0010)
#endif*/

extern uint8_t i2c_mem[ADI_I2C_MEMORY_SIZE];
extern ADI_I2C_HANDLE i2c_device;
extern uint32_t i2c_hwErrors;

//setups i2c driver
//requestedBitRate32: transmission bitrate (100000 - 400000)
ADI_I2C_RESULT i2cSetup(uint32_t const requestedBitRate32);

//requests bytes from I2C device
//SlaveAddress: device address
//bytes: number of bytes requested
//data: pointer to where the data received is going to saved
ADI_I2C_RESULT i2cRead(uint16_t const SlaveAddress, uint8_t bytes, uint8_t* data);

//writes bytes to I2C device
//SlaveAddress: device address
//bytes: number of bytes to be written
//data: pointer to data that's going to be written
ADI_I2C_RESULT i2cWrite(uint16_t const SlaveAddress, uint8_t bytes, uint8_t* data);

ADI_I2C_RESULT i2cWriteDS3231(uint8_t bytes, uint8_t* data);
ADI_I2C_RESULT i2cReadDS3231(uint8_t bytes, uint8_t* data);
#endif /* I2C_H_ */
