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


extern QueueHandle_t xQueue1;// = xQueueCreate(4, sizeof(char[50]));
extern QueueHandle_t xQueue2;// = xQueueCreate(4, sizeof(char[50]));
extern QueueHandle_t xQueue3;// = xQueueCreate(4, sizeof(char[50]));
extern QueueHandle_t xUARTQueue;

void taskSystemControl(void * pvParameters)
{
	uint8_t SwPress = 0;
	
	while (true)
	{
		SwPress = readSW();
		//if SW1 is pressed, LED1 will flash, if pressed again, LED2 will flash, if pressed again, LED3 will flash
		if(SwPress == 1)	//decrease delay time
		{
			Button1(pvParameters);
		}
		
		if(SwPress == 2)	//increase delay time
		{
			Button2(pvParameters);
		}
		
		if(SwPress == 3)	// change control
		{
			Button3(pvParameters);
		}
		SwPress = 0;
		vTaskDelay(100);
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

void UARTTask(void * pvParameters)
{
	char xUARTReceive[50];
	int len = 0;
	while (true)
	{
		if(uxQueueMessagesWaiting(xUARTQueue) > 0)
		{
			xQueueReceive( xUARTQueue,
			&xUARTReceive,
			(portMAX_DELAY) == pdPASS);
			UARTPutStr(EDBG_UART, &xUARTReceive, strlen(xUARTReceive));
		}
	}
}
