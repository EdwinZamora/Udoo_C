#include <stdint.h>

#define MAX_DATA_SIZE 9
#define COMMAND_FRAME_SIZE 3
#define RESPONSE_FRAME_SIZE 3

typedef struct
{
	uint8_t bStartOfFrame;
	uint8_t bSensor;
	uint8_t bAxis;
	int8_t bChecksum;
}CommandFrame_t;

CommandFrame_t tCommandFrame;

typedef struct
{
	uint8_t bStartOfFrame;
	uint8_t bSensor;
	uint8_t bDataSize;
	int16_t wData[MAX_DATA_SIZE];
	int8_t bChecksum;
}ResponseFrame_t;

ResponseFrame_t tResponseFrame;

uint8_t bfnCommandFrameRead(const CommandFrame_t * psCommandFrame, int16_t waData[]);

void vfnResponseFrameRead(const ResponseFrame_t * psResponseFrame);