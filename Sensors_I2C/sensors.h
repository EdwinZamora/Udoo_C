#define GYROSCOPE_ADDRESS 0x20

#define ACCELMAGNET_ADDRESS 0x1E

#define GYROSCOPE_WHO_AM_I_REGISTER_ADDRESS 0x0C

#define ACCELMAGNET_WHO_AM_I_REGISTER_ADDRESS 0x0D

#define GYROSCOPE_WHO_AM_I_REGISTER_VALUE 0xD7

#define ACCELMAGNET_WHO_AM_I_REGISTER_VALUE 0xC7

#define GYROSCOPE_CONTROL_REGISTER 0x15

#define GYROSCOPE_WRAP_TO_X_AXIS 0x08 /*For gyro control register 3*/

#define ACCELMAGNET_CONTROL_REGISTER 0x2B

#define ACCELMAGNET_LOW_NOISE_LOW_POWER 0x01 /*For accel/magnet control register 2*/

#define SENSOR_X_DATA_MSB_REGISTER 0x01

#define SENSOR_BUFFER_SIZE 6

#define I2C_DIR_ADD "/dev/i2c-3"


uint8_t bfnSensorInit(uint8_t baWriteBuffer[], uint8_t baReadBuffer[], const uint8_t kbSlaveAddress, const uint8_t kbWhoAmIRegisterAddress, const uint8_t kbWhoAmIRegisterValue); 

int16_t wfnSensorRead(const uint8_t kbSlaveAddress, const uint8_t kbAxis); 

