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
#include "swDriver.h"
#include "ledDriver.h"
#include "OITExpansionBoard.h"
#include "OITExpansionBoardDefines.h"

static uint8_t CurLED = 0;
static uint8_t FreezeState = 0;
static TaskHandle_t xHandle1 = NULL;
static TaskHandle_t xHandle2 = NULL;
static TaskHandle_t xHandle3 = NULL;

//static volatile uint8_t button_down1 = 0;
//static volatile uint8_t button_down2 = 0;
//static volatile uint8_t button_down3 = 0;

void initializeSWDriver(void)
{
}

uint8_t readSW()
{
	uint8_t read = 0;
	
	if(!ioport_get_pin_level(EXT1_PUSH_BUTTON_6))
	{
		read = 1;
	}
	if(!ioport_get_pin_level(EXT1_PUSH_BUTTON_7))
	{
		read = 2;
	}
	if (!ioport_get_pin_level(EXT1_PUSH_BUTTON_8))
	{
		read = 3;
	}
	return read;
}

void Button1()
{
	if(CurLED < 3)
	{
		CurLED++;
		
		if(CurLED == 1)
		{
			xTaskCreate(ledTask,				// Function Called by task
						"LED Task",			// Task Name
						configMINIMAL_STACK_SIZE,	// Task Stack Size
						CurLED,						// Any Parameters Passed to Task
						1,							// Task Priority
						&xHandle1);						// Place to store Task Handle
		}
		if(CurLED == 2)
		{
			xTaskCreate(ledTask,				// Function Called by task
						"LED Task",			// Task Name
						configMINIMAL_STACK_SIZE,	// Task Stack Size
						CurLED,						// Any Parameters Passed to Task
						1,							// Task Priority
						&xHandle2);						// Place to store Task Handle
		}
		if(CurLED == 3)
		{
			xTaskCreate(ledTask,				// Function Called by task
						"LED Task",			// Task Name
						configMINIMAL_STACK_SIZE,	// Task Stack Size
						CurLED,						// Any Parameters Passed to Task
						1,							// Task Priority
						&xHandle3);						// Place to store Task Handle
		}
	}
	vTaskDelay(250);
}

void Button2()
{
	if(CurLED == 3)
	{
		vTaskDelete(xHandle3);
		setLED(3, 0);
		CurLED--;
	}
	else if(CurLED == 2)
	{
		vTaskDelete(xHandle2);
		setLED(2, 0);
		CurLED--;
	}
	else if(CurLED == 1)
	{
		vTaskDelete(xHandle1);
		setLED(1, 0);
		CurLED--;
	}
}

void Button3()
{
	if(FreezeState > 0)
	{

		vTaskResume(xHandle1);

		if(FreezeState > 1)
		{
			vTaskResume(xHandle2);
		}
		if(FreezeState > 2)
		{
			vTaskResume(xHandle3);
		}
		FreezeState = 0;
	}
	else
	{
		if(CurLED > 0)
		{
			vTaskSuspend(xHandle1);
			FreezeState = 1;
		}
		if(CurLED > 1)
		{
			vTaskSuspend(xHandle2);
			FreezeState = 2;
		}
		if(CurLED > 2)
		{
			vTaskSuspend(xHandle3);
			FreezeState = 3;
		}
	}
}
