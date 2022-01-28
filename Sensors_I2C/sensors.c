#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sensors.h"
#include "I2cWrapper.h"

uint8_t bfnSensorInit(uint8_t baWriteBuffer[], uint8_t baReadBuffer[], const uint8_t kbSlaveAddress, const uint8_t kbWhoAmIRegisterAddress, const uint8_t kbWhoAmIRegisterValue)
{
	i2c_status_t I2cStatus;
	i2c_handler_t I2cHandler;
	uint8_t bReturnValue = 0;
	
	if(kbWhoAmIRegisterAddress == ACCELMAGNET_WHO_AM_I_REGISTER_ADDRESS || kbWhoAmIRegisterAddress == GYROSCOPE_WHO_AM_I_REGISTER_ADDRESS)
	{
		baWriteBuffer[0] = kbWhoAmIRegisterAddress;
		if(kbSlaveAddress == GYROSCOPE_ADDRESS ||  kbSlaveAddress == ACCELMAGNET_ADDRESS)
		{			
			I2cStatus = I2c_Read(&I2cHandler, (uint8_t) kbSlaveAddress, &baWriteBuffer[0], 1, &baReadBuffer[0], 1); /*Write in order to search for device*/
			if(I2cStatus == I2C_OK) 
			{
				if(kbWhoAmIRegisterValue == ACCELMAGNET_WHO_AM_I_REGISTER_VALUE || kbWhoAmIRegisterValue == GYROSCOPE_WHO_AM_I_REGISTER_VALUE)
				{
					if(baReadBuffer[0] == kbWhoAmIRegisterValue) /*If device returned its who am I value properly*/
					{
						printf("\n\rDevice Found!\n\r");  /*Gyroscope ID: FXAS21002C. Accelerometer & Magnetometer ID: FXOS8700CQ*/

						if(kbSlaveAddress == ACCELMAGNET_ADDRESS)
						{
							baWriteBuffer[0] = ACCELMAGNET_CONTROL_REGISTER;
							baWriteBuffer[1] = ACCELMAGNET_LOW_NOISE_LOW_POWER;
						}
						else
						{
							baWriteBuffer[0] = GYROSCOPE_CONTROL_REGISTER;
							baWriteBuffer[1] = GYROSCOPE_WRAP_TO_X_AXIS;
						}
						
						I2cStatus = I2c_Write(&I2cHandler, (uint8_t) kbSlaveAddress, &baWriteBuffer[0],2); /*Writes to the control register to enable the sensor*/
						if(I2cStatus != I2C_OK)
						{
							bReturnValue = -1;
							printf("\n\rI2C Error\n\r"); 
						}
					}
					else
					{
						bReturnValue = -1;
						printf("\n\rError. No device found.\n\r"); 
					}
				}
				else
				{
					bReturnValue = -1;
					printf("\n\rError. Invalid who am I value\n\r"); 
				}
			}
			else
			{
				bReturnValue = -1;
				printf("\n\rI2C Error\n\r"); 
			}
		}
		else
		{
			bReturnValue = -1;
			printf("\n\rError. Invalid slave address\n\r"); 
		}
	}
	else
	{
		bReturnValue = -1;
		printf("\n\rError. Invalid who am I address\n\r"); 
	}
	return bReturnValue;
}

int16_t wfnSensorRead(const uint8_t kbSlaveAddress, const uint8_t kbAxis)
{
	i2c_status_t I2cStatus;
	i2c_handler_t I2cHandler;
	int16_t wAxisReadValue = 0;
	uint8_t baReadBuffer[SENSOR_BUFFER_SIZE];
	uint8_t bWriteValue = SENSOR_X_DATA_MSB_REGISTER;  /*Offset to start reading from where the axis' information is*/
	
	if(kbAxis < 3 && kbAxis >= 0)
	{
		if(kbSlaveAddress == GYROSCOPE_ADDRESS ||  kbSlaveAddress == ACCELMAGNET_ADDRESS)
		{
			I2cStatus = I2c_Read(&I2cHandler, (uint8_t)kbSlaveAddress, &bWriteValue, 1, &baReadBuffer[0], SENSOR_BUFFER_SIZE); /*Read sensor*/
			
			if(I2cStatus == I2C_OK) 
			{
				if(!kbAxis)
				{
					wAxisReadValue = (int16_t)((uint16_t)((uint16_t)baReadBuffer[0] << 8) | (uint16_t)baReadBuffer[1]) >> 2U; /*X axis*/
				}
				else if(kbAxis == 1)
				{
					wAxisReadValue = (int16_t)((uint16_t)((uint16_t)baReadBuffer[2] << 8) | (uint16_t)baReadBuffer[3]) >> 2U; /*Y axis*/
				}
				else
				{
					wAxisReadValue = (int16_t)((uint16_t)((uint16_t)baReadBuffer[4] << 8) | (uint16_t)baReadBuffer[5]) >> 2U; /*Z axis*/    
				}
			}
			else
			{
				printf("\n\rError reading sensor\n\r");
			}
		}
		else
		{
			printf("\n\rError. Invalid slave address\n\r"); 
		}
	}
	else
	{
		printf("\n\rError. Non-existing axis\n\r"); 
	}
	return wAxisReadValue;
}