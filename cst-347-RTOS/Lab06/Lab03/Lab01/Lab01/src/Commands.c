/*
 * Commands.c
 *
 * Created: 2/16/2021 10:52:14 PM
 *  Author: Ston
 */ 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <FreeRTOS_CLI.h>

#include "Commands.h"
#include "mytasks.h"

extern QueueHandle_t xQueueTX;

static const char taskListHdr[] = "Name\t\t\tStat\tPri\tS/Space\tTCB\r\n";
static TaskHandle_t xLEDHandle[3];

static const CLI_Command_Definition_t xTaskStatsCommand = 
{
	"task-stats",
    "task-stats: Displays a table of task state information\r\n",
    prvTaskStatsCommand,
    0
};

static const CLI_Command_Definition_t xStartLED =
{
	"Start-LED",
	"start_led <LED_int> <DELAY_ms>  Start <LED_int> blinking using a toggle delay time of <DELAY_ms>\r\n",
	prvStartLEDCommand,
	2
};

static const CLI_Command_Definition_t xStopLED =
{
	"Stop-LED",
	"Stop_LED <LED_int>  Stop <LED_int> from blinking.\r\n",
	prvStopLEDCommand,
	1
};
	
static const CLI_Command_Definition_t xChangeLED =
{
	"Change-LED",
	"Change_LED <LED_int> <DELAY_ms>  Change <LED_int>s blink delay time to <DELAY_ms>\r\n",
	prvChangeLEDCommand,
	2
};
			
void vRegisterCommands(void)
{
	FreeRTOS_CLIRegisterCommand( &xTaskStatsCommand );
	FreeRTOS_CLIRegisterCommand( &xStartLED );
	FreeRTOS_CLIRegisterCommand( &xStopLED );
	FreeRTOS_CLIRegisterCommand( &xChangeLED );
}		

// Note: You should check to make sure everything fits in the buffer you are 
// sending back. This example does not...
portBASE_TYPE prvTaskStatsCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString)
{
	BaseType_t xReturn = pdTRUE;
	static int count;
	static char buffer[300];
	if (buffer[0] == 0)
	{
		count = 0;
		memset(buffer, 0, 300);
		sprintf(pcWriteBuffer, taskListHdr);
		pcWriteBuffer += strlen(taskListHdr);
		vTaskList(buffer);
		memcpy(pcWriteBuffer, buffer[strlen(taskListHdr)], 49 - strlen(taskListHdr));
	}
	else if(count != 6)
	{
		memcpy(pcWriteBuffer, buffer[count*49], 49);
		count++;
	}
	else
	{
		xReturn = pdFALSE;
		count = 0;
	}
	
    return xReturn;
}

portBASE_TYPE prvStartLEDCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString)
{
	int8_t *pcParameter1, *pcParameter2;
	BaseType_t xParameter1StringLength,xParameter2StringLength;
	struct LEDData Data;
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	/* Obtain the parameter string. */
	pcParameter1 = FreeRTOS_CLIGetParameter(
						pcCommandString,		/* The command string itself. */
						1,		/* Return the next parameter. */
						&xParameter1StringLength	/* Store the parameter string length. */
					);
					
	pcParameter2 = FreeRTOS_CLIGetParameter(
					pcCommandString,		/* The command string itself. */
					2,		/* Return the next parameter. */
					&xParameter2StringLength	/* Store the parameter string length. */
					);
					
	pcParameter1[xParameter1StringLength] = 0x00;
	pcParameter2[xParameter2StringLength] = 0x00;
	
	/* Sanity check something was returned. */
	configASSERT( pcParameter1 );
	configASSERT( pcParameter2 );
	
	Data.LEDNum = &pcParameter1;
	Data.Delay = &pcParameter2;
	
	if(xLEDHandle[Data.LEDNum] == NULL)
	{
		xTaskCreate(LEDTask,					// Function Called by task
					"LED Task",					// Task Name
					configMINIMAL_STACK_SIZE,	// Task Stack Size
					&Data,						// Any Parameters Passed to Task
					2,							// Task Priority
					&xLEDHandle[Data.LEDNum - 1]);// Place to store Task Handle
	}
	else
	{
		xQueueSendToBack(xQueueTX, "LED already started", (TickType_t)10);
	}

	return pdFALSE;
}

portBASE_TYPE prvStopLEDCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString)
{
	int8_t *pcParameter1;
	BaseType_t xParameter1StringLength;
	struct LEDData Data;
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	/* Obtain the parameter string. */
	pcParameter1 = FreeRTOS_CLIGetParameter(
						pcCommandString,		/* The command string itself. */
						1,		/* Return the next parameter. */
						&xParameter1StringLength	/* Store the parameter string length. */
					);
				
					
	pcParameter1[xParameter1StringLength] = 0x00;
	
	/* Sanity check something was returned. */
	configASSERT( pcParameter1 );
	
	Data.LEDNum = &pcParameter1;
	
	if(xLEDHandle[Data.LEDNum] != NULL)
	{
		vTaskDelete(&xLEDHandle[Data.LEDNum - 1]);						// Place to store Task Handle
	}
	else
	{
		xQueueSendToBack(xQueueTX, "LED already Stopped", (TickType_t)10);
	}

	return pdFALSE;
}

portBASE_TYPE prvChangeLEDCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString)
{
	int8_t *pcParameter1, *pcParameter2;
	BaseType_t xParameter1StringLength,xParameter2StringLength;
	struct LEDData Data;
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	/* Obtain the parameter string. */
	pcParameter1 = FreeRTOS_CLIGetParameter(
						pcCommandString,		/* The command string itself. */
						1,		/* Return the next parameter. */
						&xParameter1StringLength	/* Store the parameter string length. */
					);
					
	pcParameter2 = FreeRTOS_CLIGetParameter(
					pcCommandString,		/* The command string itself. */
					2,		/* Return the next parameter. */
					&xParameter2StringLength	/* Store the parameter string length. */
					);
					
	pcParameter1[xParameter1StringLength] = 0x00;
	pcParameter2[xParameter2StringLength] = 0x00;
	
	/* Sanity check something was returned. */
	configASSERT( pcParameter1 );
	configASSERT( pcParameter2 );
	
	Data.LEDNum = &pcParameter1;
	Data.Delay = &pcParameter2;
	
	if(xLEDHandle[Data.LEDNum] != NULL)
	{
		vTaskDelete(xLEDHandle[Data.LEDNum - 1]);
		xTaskCreate(LEDTask,					// Function Called by task
					"LED Task",					// Task Name
					configMINIMAL_STACK_SIZE,	// Task Stack Size
					&Data,						// Any Parameters Passed to Task
					2,							// Task Priority
					&xLEDHandle[Data.LEDNum - 1]);						// Place to store Task Handle
	}
	else
	{
		xQueueSendToBack(xQueueTX, "LED is stopped", (TickType_t)10);
	}

	return pdFALSE;
}