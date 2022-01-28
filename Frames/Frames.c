#include <stdint.h>
#include "Frames.h"
#include "sensors.h"

uint8_t bfnCommandFrameRead(const CommandFrame_t * psCommandFrame, int16_t waData[])
{
	uint8_t bSize = COMMAND_FRAME_SIZE ;
	uint8_t bDataBufferSize = MAX_DATA_SIZE;
	int8_t bChecksum = 0;
	uint8_t bAxisCounter = 2;
	int8_t bDataSize = 0;

    if(psCommandFrame-> bStartOfFrame == 0xAA)
	{
		while(bSize--)
		{
			bChecksum += psCommandFrame-> bStartOfFrame + psCommandFrame-> bSensor + psCommandFrame-> bAxis;
		}
		bChecksum = ~bChecksum;
		
		if(psCommandFrame-> bChecksum == bChecksum)
		{ 
			waData[9] = bChecksum;
			while(bDataBufferSize--) /*Memset buffer to 0*/
			{
				waData[bDataBufferSize] = 0;
			}
			bDataBufferSize = MAX_DATA_SIZE;
			if(psCommandFrame-> bSensor == 0x01)
			{
				if(psCommandFrame-> bAxis == 0x01)
				{
					waData[0] = wfnSensorRead(ACCELMAGNET_ADDRESS, 0);
				}
				else if(psCommandFrame-> bAxis == 0x02)
				{
					waData[1] = wfnSensorRead(ACCELMAGNET_ADDRESS, 1);
				}
				else if(psCommandFrame-> bAxis == 0x03)
				{
					waData[2] = wfnSensorRead(ACCELMAGNET_ADDRESS, 2);
				}
				else if(psCommandFrame-> bAxis == 0x04)
				{
					while(bAxisCounter)
					{
						waData[bAxisCounter] = wfnSensorRead(ACCELMAGNET_ADDRESS, bAxisCounter);
						bAxisCounter--;
					}
				}
				else
				{
					bReturnValue = -1;  /*Axis error*/
				}
			}
			else if(psCommandFrame-> bSensor == 0x02)
			{
				if(psCommandFrame-> bAxis == 0x01)
				{
					waData[3] = wfnSensorRead(ACCELMAGNET_ADDRESS, 0);
				}
				else if(psCommandFrame-> bAxis == 0x02)
				{
					waData[4] = wfnSensorRead(ACCELMAGNET_ADDRESS, 1);
				}
				else if(psCommandFrame-> bAxis == 0x03)
				{
					waData[5] = wfnSensorRead(ACCELMAGNET_ADDRESS, 2));
				}
				else if(psCommandFrame-> bAxis == 0x04)
				{
					while(bAxisCounter)
					{
						waData[bAxisCounter + 3] = wfnSensorRead(ACCELMAGNET_ADDRESS, bAxisCounter);
						bAxisCounter--;
					}
				}
				else
				{
					bReturnValue = -1; /*Axis error*/
				}
			}
			else if(psCommandFrame-> bSensor == 0x03)
			{
				if(psCommandFrame-> bAxis == 0x01)
				{
					waData[6] = wfnSensorRead(GYROSCOPE_ADDRESS, 0);
				}
				else if(psCommandFrame-> bAxis == 0x02)
				{
					waData[7] = wfnSensorRead(GYROSCOPE_ADDRESS, 1);
				}
				else if(psCommandFrame-> bAxis == 0x03)
				{
					waData[8] = wfnSensorRead(GYROSCOPE_ADDRESS, 2);
				}
				else if(psCommandFrame-> bAxis == 0x04)
				{
					while(bAxisCounter)
					{
						wData[bAxisCounter + 6] = wfnSensorRead(GYROSCOPE_ADDRESS, bAxisCounter);
						bAxisCounter--;
					}
				}
				else
				{
					bReturnValue = -1; /*Axis error*/
				}
			}
			else if(psCommandFrame-> bSensor == 0xFF)
			{
				if(psCommandFrame-> bAxis == 0x01)
				{
					waData[0] = wfnSensorRead(ACCELMAGNET_ADDRESS, 0);
					waData[3] = wfnSensorRead(ACCELMAGNET_ADDRESS, 0);
					waData[6] = wfnSensorRead(GYROSCOPE_ADDRESS, 0);
				}
				else if(psCommandFrame-> bAxis == 0x02)
				{
					waData[1] = wfnSensorRead(ACCELMAGNET_ADDRESS, 1);
					waData[4] = wfnSensorRead(ACCELMAGNET_ADDRESS, 1);
					waData[7] = wfnSensorRead(GYROSCOPE_ADDRESS, 1);
				}
				else if(psCommandFrame-> bAxis == 0x03)
				{
					waData[2] = wfnSensorRead(ACCELMAGNET_ADDRESS, 2);
					waData[5] = wfnSensorRead(ACCELMAGNET_ADDRESS, 2);
					waData[8] = wfnSensorRead(GYROSCOPE_ADDRESS, 2);
				}
				else if(psCommandFrame-> bAxis == 0x04)
				{
					while(bAxisCounter)
					{
						waData[bAxisCounter]= wfnSensorRead(ACCELMAGNET_ADDRESS, bAxisCounter);
						waData[bAxisCounter + 3] = wfnSensorRead(ACCELMAGNET_ADDRESS, bAxisCounter);
						waData[bAxisCounter + 6] = wfnSensorRead(GYROSCOPE_ADDRESS, bAxisCounter);
						bAxisCounter--;
					}
				}
				else
				{
					bReturnValue = -1; /*Axis error*/
				}
			}
			else
			{
				bReturnValue = -1; /*Sensor error*/
			}
			while(bDataBufferSize--)
			{
				if(waData[bDataBufferSize] == -1)
				{
					bReturnValue = -1; /*Sensor read error*/
					break;
				}
				else if(waData[bDataBufferSize] != 0)
				{
					bReturnValue++;
				}			
			}
		}
		else
		{
			bReturnValue = -1; /*Checksum error*/
		}
	}
	else
	{
		bReturnValue = -1; /*SOF error*/
	}
	return bReturnValue;
}

void vfnResponseFrameRead(const ResponseFrame_t * psResponseFrame)
{
	uint8_t bSize = RESPONSE_FRAME_SIZE;
	int8_t bChecksum
	int16_t wReadValue = 0;

	while(bSize--)
	{
		bChecksum += psResponseFrame->bStartOfFrame + psResponseFrame->bSensor + psResponseFrame->bDataSize + psResponseFrame->wData[0] + psResponseFrame->wData[1] + psResponseFrame->wData[2] + psResponseFrame->wData[3] + psResponseFrame->wData[4] + psResponseFrame->wData[5] + psResponseFrame->wData[6] + psResponseFrame->wData[7] + psResponseFrame->wData[8];
	}
	bChecksum = ~bChecksum;

	if(psResponseFrame->bChecksum == bChecksum)
	{
		if(psResponseFrame->bDataHigh[0] || psResponseFrame->bDataLow[0])
		{
			printf("\n\rAccelerometer X axis = %i\n\r", psResponseFrame->wData[0]);
		}
	    if(psResponseFrame->wData[1])
		{
			printf("\n\rAccelerometer Y axis = %i\n\r", psResponseFrame->wData[1]);
		}
	    if(psResponseFrame->wData[2])
		{
			printf("\n\rAccelerometer Z axis = %i\n\r", psResponseFrame->wData[2]);
		}
		if(psResponseFrame->wData[3])
		{
			printf("\n\rMagnetometer X axis = %i\n\r", psResponseFrame->wData[3]);
		}
		if(psResponseFrame->wData[4])
		{
			printf("\n\rMagnetometer Y axis = %i\n\r", psResponseFrame->wData[4]);
		}
		if(psResponseFrame->wData[5])
		{
			printf("\n\rMagnetometer Z axis = %i\n\r", psResponseFrame->wData[5]);
		}
		if(psResponseFrame->wData[6])
		{
			printf("\n\rGyroscope X axis = %i\n\r", psResponseFrame->wData[6]);
		}
		if(psResponseFrame->wData[7])
		{
			printf("\n\rGyroscope Y axis = %i\n\r", psResponseFrame->wData[7]);
		}
		if(psResponseFrame->wData[8])
		{
			printf("\n\rGyroscope Y axis = %i\n\r", psResponseFrame->wData[8]);
		}
	}
	else
	{
		printf("\n\rChecksum error, corrupted data\n\r");
	}
	return;
}