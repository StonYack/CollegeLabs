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

void ledTask(void* uiLedNum)
{
	uint8_t control = 0;
	uint16_t CurDel = 500;
	enum ACommand xMessage;
	
	// Set -> Toggle
	while(1)
	{
		setLED(uiLedNum, control);
		control = toggleLED(control);
		vTaskDelay(CurDel/portTICK_PERIOD_MS);
		
		if(uiLedNum == 1)
		{
			if(uxQueueMessagesWaiting(xQueue1) > 0)
			{
					xQueueReceive( xQueue1,
									&xMessage,
									(( TickType_t ) 10 ) == pdPASS);
					if(xMessage == decrease)
					{
						if(CurDel > 0)
						{
							CurDel = CurDel - 200;
						}
						
					}
					if(xMessage == increase)
					{
						if(CurDel < 1000)
						{
							CurDel = CurDel + 200;
						}
					}
			}	
		}
		if(uiLedNum == 2)
		{
			if(uxQueueMessagesWaiting(xQueue2) > 0)
			{
				xQueueReceive( xQueue2,
								&xMessage,
								(( TickType_t ) 10 ) == pdPASS);
				if(xMessage == decrease)
				{
					if(CurDel > 0)
					{
						CurDel = CurDel - 100;
					}
					
				}
				if(xMessage == increase)
				{
					if(CurDel < 1000)
					{
						CurDel = CurDel + 100;
					}
				}
			}
		}
		if(uiLedNum == 3)
		{
			if(uxQueueMessagesWaiting(xQueue3) > 0)
			{
				xQueueReceive( xQueue3,
								&xMessage,
								(( TickType_t ) 10 ) == pdPASS);
				if(xMessage == decrease)
				{
					if(CurDel > 0)
					{
						CurDel = CurDel + 100;
					}
					
				}
				if(xMessage == increase)
				{
					if(CurDel < 1000)
					{
						CurDel = CurDel - 100;
					}
				}
			}
		}
		}	
}