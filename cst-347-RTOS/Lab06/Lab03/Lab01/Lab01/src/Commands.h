/*
 * Commands.h
 *
 * Created: 2/16/2021 10:51:59 PM
 *  Author: Ston
 */ 


struct LEDData
{
	uint8_t LEDNum;
	uint8_t Delay;
};

void vRegisterCommands(void);

portBASE_TYPE prvTaskStatsCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString);
portBASE_TYPE prvStartLEDCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString);
portBASE_TYPE prvStopLEDCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString);
portBASE_TYPE prvChangeLEDCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString);