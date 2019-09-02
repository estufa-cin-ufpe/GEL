#include "i2c.h"

uint8_t i2c_mem[ADI_I2C_MEMORY_SIZE];
ADI_I2C_HANDLE i2c_device;
uint32_t i2c_hwErrors;

ADI_I2C_RESULT i2cSetup(uint32_t const requestedBitRate32)
{
	ADI_I2C_RESULT eResult;

	if(ADI_I2C_SUCCESS != (eResult=adi_i2c_Open(0, i2c_mem, ADI_I2C_MEMORY_SIZE, &i2c_device)))
	{
		return eResult;
	}

	if(ADI_I2C_SUCCESS != (eResult=adi_i2c_Reset(i2c_device)))
	{
		return eResult;
	}

	if(ADI_I2C_SUCCESS != (eResult=adi_i2c_SetBitRate(i2c_device, requestedBitRate32)))
	{
		return eResult;
	}

	return eResult;
}

ADI_I2C_RESULT i2cRead(uint16_t const SlaveAddress, uint8_t bytes, uint8_t* data)
{
	ADI_I2C_RESULT result;
	ADI_I2C_TRANSACTION transaction;

	//set slave address
	result = adi_i2c_SetSlaveAddress(i2c_device, SlaveAddress);
	if(result)
		return result;

	//build transaction
	transaction.nPrologueSize = 0;
	transaction.nDataSize = bytes;
	transaction.pData = data;
	transaction.bReadNotWrite = true;
	transaction.bRepeatStart = true;

	result = adi_i2c_ReadWrite(i2c_device, &transaction, &i2c_hwErrors);
	if(result)
		return result;
	return 0;
}

ADI_I2C_RESULT i2cWrite(uint16_t const SlaveAddress, uint8_t bytes, uint8_t* data)
{
	ADI_I2C_RESULT result;
	ADI_I2C_TRANSACTION transaction;

	//set slave address
	result = adi_i2c_SetSlaveAddress(i2c_device, SlaveAddress);
	if(result)
		return result;

	//build transaction
	transaction.nPrologueSize = 0;
	transaction.nDataSize = bytes;
	transaction.pData = data;
	transaction.bReadNotWrite = false;
	transaction.bRepeatStart = true;

	result = adi_i2c_ReadWrite(i2c_device, &transaction, &i2c_hwErrors);
	if(result)
		return result;
	return 0;
}
