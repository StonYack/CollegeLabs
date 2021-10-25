/*
 * swDriver.c
 *
 * Created: 1/12/2021 3:01:54 PM
 *  Author: Ston
 */ 
#include <asf.h>
#include <time.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "swDriver.h"
#include "ledDriver.h"
#include "OITExpansionBoard.h"
#include "OITExpansionBoardDefines.h"

extern TaskHandle_t xMain1;
extern TaskHandle_t xMain2;
extern TaskHandle_t xMain3;
extern QueueHandle_t xUARTQueue;

enum ACommand xMessage;

void initializeSWDriver(void)
{
}

uint8_t readSW(void)
{
	uint8_t read = 0;
	
	if(!ioport_get_pin_level(EXT1_PUSH_BUTTON_6))
	{
		vTaskDelay(10);
		if(!ioport_get_pin_level(EXT1_PUSH_BUTTON_6))
		{
			read = 1;
		}
	}
	if(!ioport_get_pin_level(EXT1_PUSH_BUTTON_7))
	{
		vTaskDelay(10);
		if(!ioport_get_pin_level(EXT1_PUSH_BUTTON_7))
		{
			read = 2;
		}
	}
	if (!ioport_get_pin_level(EXT1_PUSH_BUTTON_8))
	{
		vTaskDelay(10);
		if(!ioport_get_pin_level(EXT1_PUSH_BUTTON_8))
		{
			read = 3;
		}
	}
	return read;
}

void Button1(uint8_t Cur)
{
	if((xQueue1 != 0) && (Cur == 1))
	{
		xMessage = increase;
		xQueueSendToBack(xUARTQueue, "Increasing Led 1 flash speed\r\n", ((TickType_t)10) != pdPASS);
		xQueueSendToBack(xQueue1, (void *)&xMessage, ((TickType_t)10) != pdPASS);
	}
	if((xQueue2 != 0) && (Cur == 2))
	{
		xMessage = increase;
		xQueueSendToBack(xUARTQueue, "Increasing Led 2 flash speed\r\n", ((TickType_t)10) != pdPASS);
		xQueueSendToBack(xQueue2, (void *)&xMessage, ((TickType_t)10) != pdPASS);
	}
	if((xQueue3 != 0) && (Cur == 3))
	{
		xMessage = increase;
		xQueueSendToBack(xUARTQueue, "Increasing Led 3 flash speed\r\n", ((TickType_t)10) != pdPASS);
		xQueueSendToBack(xQueue3, (void *)&xMessage, ((TickType_t)10) != pdPASS);
	}
	
}

void Button2(uint8_t Cur)
{
	if((xQueue1 != 0) && (Cur == 1))
	{
		xQueueSendToBack(xUARTQueue, "Decreasing Led 1 flash speed\r\n", ((TickType_t)10) != pdPASS);
		xQueueSendToBack(xQueue1, (void *)&xMessage, ((TickType_t)10) != pdPASS);
	}
	if((xQueue2 != 0) && (Cur == 2))
	{
		xMessage = decrease;
		xQueueSendToBack(xUARTQueue, "Decreasing Led 2 flash speed\r\n", ((TickType_t)10) != pdPASS);
		xQueueSendToBack(xQueue2, (void *)&xMessage, ((TickType_t)10) != pdPASS);
	}
	if((xQueue3 != 0) && (Cur == 3))
	{
		xMessage = decrease;	
		xQueueSendToBack(xUARTQueue, "Decreasing Led 3 flash speed\r\n", ((TickType_t)10) != pdPASS);
		xQueueSendToBack(xQueue3, (void *)&xMessage, ((TickType_t)10) != pdPASS);
	}
}

void Button3(uint8_t Cur)
{
	if(Cur == 1)
	{
		vTaskResume(xMain2);
		xQueueSendToBack(xUARTQueue, "LED 2 is Now Active\r\n", ((TickType_t)10) != pdPASS);
		vTaskSuspend(NULL);
	}
	else if(Cur == 2)
	{
		vTaskResume(xMain3);
		xQueueSendToBack(xUARTQueue, "LED 3 is Now Active\r\n", ((TickType_t)10) != pdPASS);
		vTaskSuspend(NULL);
	}
	else if(Cur == 3)
	{
		vTaskResume(xMain1);
		xQueueSendToBack(xUARTQueue, "LED 1 is Now Active\r\n", ((TickType_t)10) != pdPASS);
		vTaskSuspend(NULL);
	}
}
