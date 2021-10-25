/*
 * mytasks.c
 *
 * Created: 1/4/2021 12:58:14 PM
 *  Author: Ston Yackamouih
 */ 

#include <asf.h>
#include <time.h>
#include <string.h>
#include "mytasks.h"
#include "OITExpansionBoard.h"
#include "swDriver.h"
#include "ledDriver.h"
#include "Uartdrv.h"


extern QueueHandle_t xQueueLED;
extern QueueHandle_t xQueueRX;
extern QueueHandle_t xQueueTX;
extern int Button;

void ButtonTask(void * pvParameters)
{
	while(true)
	{
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY ); 
		if(Button == 1)
		{
			xQueueSendToBack(xQueueTX, "\r\nHello FreeRTOS World\r\n", ((TickType_t)10));
		}
		if(Button == 2)
		{
			xQueueSendToBack(xQueueTX, "\r\nCST 347 – RTOS\r\n", ((TickType_t)10));
		}
		if(Button == 3)
		{
			xQueueSendToBack(xQueueTX, "\r\nLab 05 – Interrupts in FreeRTOS\r\n", ((TickType_t)10));
		}
	}
}

void LEDTask(void * pvParameters)
{
	uint8_t control = 0;
	char xMessage = '\0';
	while(true)
	{
		if(uxQueueMessagesWaiting(xQueueLED) > 0)
		{
			xQueueReceive( xQueueLED,
							&xMessage,
							( TickType_t ) 10 );
			if(xMessage == '1')
			{
				control = !readLED(1);
				setLED(1, control);
			}
			if(xMessage == '2')
			{
				control = !readLED(2);
				setLED(2, control);
			}
			if(xMessage == '3')
			{
				control = !readLED(3);
				setLED(3, control);
			}
		}
	}
}

void HeartbeatTask(void * pvParameters)
{
	Bool LED = 0;
	
	while (true)
	{
		ioport_set_pin_level(LED_0_PIN, LED);
		LED = ~LED;
		vTaskDelay(1000);
	}
}

void TXTask(void * pvParameters)
{
	char xUARTTransmit[50];
	while (true)
	{
		xQueueReceive( xQueueTX,
						&xUARTTransmit,
						(portMAX_DELAY));
		UARTPutStr(EDBG_UART, &xUARTTransmit, strlen(xUARTTransmit));
	}
}

void RXTask(void * pvParameters)
{
	char xUARTReceive[50];
	while (true)
	{
		memset(xUARTReceive, 0, 50);
		xQueueReceive( xQueueRX,
						&xUARTReceive,
						(portMAX_DELAY));
		if(xUARTReceive[0] == '1'|| xUARTReceive[0] == '2' || xUARTReceive[0] == '3')
		{
			xQueueSendToBack(xQueueLED, (void*) &xUARTReceive[0], ((TickType_t)10));
			xQueueSendToBack(xQueueTX,  (void*)&xUARTReceive, ((TickType_t)10));
		}
		else if(xUARTReceive[0] == 'u')
		{
			strcpy(xUARTReceive, "u Ston\n\r");
			xQueueSendToBack(xQueueTX,  (void*)&xUARTReceive, ((TickType_t)10));
		}
		else
		{
			xQueueSendToBack(xQueueTX,  (void*)&xUARTReceive, ((TickType_t)10));
		}
	}
}
