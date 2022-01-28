#define GYROSCOPE_ADDRESS 			0x20

#define GYROSCOPE_WHO_AM_I_REGISTER_ADDRESS    0x0C

#define GYROSCOPE_WHO_AM_I_REGISTER_VALUE    0xD7

#define GYROSCOPE_BUFFER_SIZE		6

#define GYROSCOPE_CONTROL_REGISTER		0x13

#define GYROSCOPE_ENABLE               0x16

#define GYROSCOPE_X_DATA_MSB_REGISTER	0x01

#define I2C_DIR_ADD "/dev/i2c-3"


void vfnSensorInit(uint8_t *bpWriteBuffer, uint8_t *bpReadBuffer, uint8_t bSlaveAddress, uint8_t bWhoAmIRegisterAddress, uint8_t bWhoAmIRegisterValue, uint8_t bControlRegisterAddress, uint8_t bEnableValue); 

void vfnSensorRead(uint8_t *bpReadBuffer, uint8_t bAxis); 