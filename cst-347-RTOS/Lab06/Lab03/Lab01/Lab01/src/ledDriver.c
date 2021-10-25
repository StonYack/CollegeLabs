/*
 * ledDriver.c
 *
 * Created: 1/12/2021 3:01:43 PM
 *  Author: Ston
 */ 
#include <FreeRTOS.h>
#include <queue.h>
#include "ledDriver.h"
#include "OITExpansionBoard.h"
#include "OITExpansionBoardDefines.h"
#include "swDriver.h"

void initializeLEDDriver(void)
{
}

uint8_t readLED(uint8_t uiLedNum)
{
	uint8_t ret = 0;
	if(uiLedNum == 1)
	{
		ret = ioport_get_pin_level(EXT1_LED1_GPIO); 
	}
	
	if(uiLedNum == 2)
	{
		ret = ioport_get_pin_level(EXT1_LED2_GPIO);
	}
	
	if(uiLedNum == 3)
	{
		ret = ioport_get_pin_level(EXT1_LED3_GPIO);
	}
	return ret;
}

void setLED(uint8_t uiLedNum, uint8_t uiLEDValue)
{
	if(uiLedNum == 1)
	{
		ioport_set_pin_level(EXT1_LED1_GPIO, uiLEDValue);
	}
	if(uiLedNum == 2)
	{
		ioport_set_pin_level(EXT1_LED2_GPIO, uiLEDValue);
	}
	if(uiLedNum == 3)
	{
		ioport_set_pin_level(EXT1_LED3_GPIO, uiLEDValue);
	}
}
	
uint8_t toggleLED(uint8_t uiLedNum)
{
	uiLedNum = ~uiLedNum;
	return uiLedNum;
}