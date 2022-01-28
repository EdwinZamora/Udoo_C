///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "gyro.h"
#include "I2cWrapper.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section
///////////////////////////////////////////////////////////////////////////////////////////////////

static void Sensor_PrintSignedNumber(int16_t DataToDisplay);

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Constants Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Constants Section
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Variables Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Variables Section
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section
///////////////////////////////////////////////////////////////////////////////////////////////////

int main (int argc, char * argv[])
{
	i2c_status_t I2cStatus;
	i2c_handler_t I2cHandler;	
	uint8_t * GyroscopeReadBuffer;
	uint8_t * GyroscopeWriteBuffer;
	int16_t AxisX;
	int16_t AxisY;
	int16_t AxisZ;

	I2cStatus = I2c_Init((uint8_t*)I2C_DIR_ADD,&I2cHandler);
	if(I2cStatus == I2C_OK)
	{
		/*Sensor init*/
		GyroscopeReadBuffer = (uint8_t*)malloc(GYROSCOPE_BUFFER_SIZE);
		GyroscopeWriteBuffer = (uint8_t*)malloc(GYROSCOPE_BUFFER_SIZE);
		
		if((GyroscopeReadBuffer != NULL) && (GyroscopeWriteBuffer != NULL))
		{
			/* Assume the buffers were allocated */
			GyroscopeWriteBuffer[0] = GYROSCOPE_WHO_AM_I_REGISTER_ADDRESS;
			
			I2cStatus = I2c_Read(&I2cHandler, GYROSCOPE_ADDRESS, GyroscopeWriteBuffer, 1, GyroscopeReadBuffer, 1);
			if((I2cStatus == I2C_OK) && (GyroscopeReadBuffer[0] == GYROSCOPE_WHO_AM_I_REGISTER_VALUE)) /*If device returned its who am I value properly*/
			{
				printf("FXAS21002C Found!\n\r");  /*Gyroscope ID: FXAS21002C */
				
				/*Enable gyroscope in low noise low power mode*/
				GyroscopeWriteBuffer[0] = GYROSCOPE_CONTROL_REGISTER;
				GyroscopeWriteBuffer[1] = SENSOR_LOW_NOISE_LOW_POWER;

				I2cStatus = I2c_Write(&I2cHandler, GYROSCOPE_ADDRESS, GyroscopeWriteBuffer,2); /*Writes to the control register to enable the sensor*/
				if(I2cStatus == I2C_OK)
				{
					GyroscopeWriteBuffer[0] = GYROSCOPE_X_DATA_MSB_REGISTER; /*Offset to start reading from where the axis' information is*/
					/*Now we can start reading the sensors*/
					
					while(1)
					{
						
						/* Accelerometer data is 14-bit 2's Complement left justified */

						AxisX = (int16_t)((uint16_t)((uint16_t)GyroscopeReadBuffer[0] << 8) | (uint16_t)GyroscopeReadBuffer[1]) >> 2U;
						
						AxisY = (int16_t)((uint16_t)((uint16_t)GyroscopeReadBuffer[2] << 8) | (uint16_t)GyroscopeReadBuffer[3]) >> 2U;
						
						AxisZ = (int16_t)((uint16_t)((uint16_t)GyroscopeReadBuffer[4] << 8) | (uint16_t)GyroscopeReadBuffer[5]) >> 2U;    
						
						
						printf("\n\rX Axis: ");
						Sensor_PrintSignedNumber(AxisX);
						printf("\n\rY Axis: ");
						Sensor_PrintSignedNumber(AxisY);
						printf("\n\rZ Axis: ");
						Sensor_PrintSignedNumber(AxisZ);                        					
					}

				}
				else
				{
					printf("\n\rError writing to I2C Device: %d\n\r",I2cStatus);
				}
			
			}
		}
		else
		{
			printf("Error while allocating buffers\n\r");
		}
	}
	else
	{
		printf("\n\rError opening I2C Device: %d\n\r",I2cStatus);
	}

	return 0;

}

static void Sensor_PrintSignedNumber(int16_t DataToDisplay)
{
	uint16_t Data = (uint16_t)DataToDisplay;

	/* verify if data is positive or negative */
	/* note this function will work only with 14-bit numbers */
	if(Data < 0x1FFF)
	{
		printf("+");
	}
	else
	{
		printf("-");
		/* convert 2's complement back*/
		Data = ((~Data) + 1);
	}

	printf("%d",Data);
}
/* EOF */