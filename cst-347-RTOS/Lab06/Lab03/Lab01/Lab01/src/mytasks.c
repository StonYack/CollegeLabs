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
#include "Commands.h"
#include "FreeRTOS_CLI.h"

extern QueueHandle_t xQueueLED;
extern QueueHandle_t xQueueRX;
extern QueueHandle_t xQueueTX;
extern TaskHandle_t xLEDHandle1;
extern TaskHandle_t xLEDHandle2;
extern TaskHandle_t xLEDHandle3;

const char Message1[] = "This is a test\r\n";
const char Message2[] = "This is a aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\r\n";
const char Message3[] = "This is ahhhhhhhhhhhhhhhhhhhhhhhh\r\n";

void LEDTask(void * pvParameters)
{
	uint8_t control = 0;
	struct LEDData * Data = (struct LEDData *)pvParameters;

	while(true)
	{
		control = toggleLED(control);
		setLED(Data->LEDNum, control);
		vTaskDelay(Data->Delay);
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
						xUARTTransmit,
						(portMAX_DELAY));
		UARTPutStr(EDBG_UART, xUARTTransmit, strlen(xUARTTransmit));
		memset(xUARTTransmit, 0, 50);
	}
}

void RXTask(void * pvParameters)
{
	char xUARTReceive[2];
	char message[50];
	char outBuffer[50];
	int count = 0;
	BaseType_t xReturn;
	
	while (true)
	{
		memset(xUARTReceive, 0, 2);
		//xReturn = pdFALSE;
		xQueueReceive( xQueueRX,
						xUARTReceive,
						(portMAX_DELAY));
						
		message[count] = xUARTReceive[0];
		count++;
		
		xQueueSendToBack(xQueueTX, (void*)xUARTReceive, ((TickType_t)10));
		
		if(message[count-1] == '\r')
		{
			xQueueSendToBack(xQueueTX, (void*)"\n", ((TickType_t)10));
			message[count - 1] = '\0';
			do 
			{
				memset(outBuffer, 0, 50);
				xReturn = FreeRTOS_CLIProcessCommand(message, outBuffer, 49);
				//xQueueSendToBack(xQueueTX,  (void*)Message1, ((TickType_t)10)); // For some reason these get put in the queue but never get transmitted
				//xQueueSendToBack(xQueueTX,  (void*)Message2, ((TickType_t)10)); // For some reason these get put in the queue but never get transmitted
				//xQueueSendToBack(xQueueTX,  (void*)Message3, ((TickType_t)10)); // For some reason these get put in the queue but never get transmitted
				xQueueSendToBack(xQueueTX,  (void*)outBuffer, ((TickType_t)10)); // For some reason these get put in the queue but never get transmitted
			} while (xReturn == pdTRUE);
			
			memset(message, 0, 50);
			count = 0;
		}
		//else
		//{
			//xQueueSendToBack(xQueueTX, (void*)xUARTReceive, ((TickType_t)10));
		//}
	}
}
