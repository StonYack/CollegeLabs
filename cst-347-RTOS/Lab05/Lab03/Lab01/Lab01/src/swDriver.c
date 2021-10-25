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

extern QueueHandle_t xQueueTX;
extern TaskHandle_t xHandle;

int Button = 0;

void button_handler(uint32_t id, uint32_t mask)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if((PIN_PUSHBUTTON_6_ID == id) && (PIN_PUSHBUTTON_6_MASK == mask))
	{
		// Set a global variable to signify which button (IE onboard in this case)
		Button = 1;
		// Then wake up the button Task to process.
		vTaskNotifyGiveFromISR(xHandle, &xHigherPriorityTaskWoken);
	}
	else if((PIN_PUSHBUTTON_7_ID == id) && (PIN_PUSHBUTTON_7_MASK == mask))
	{
		// Set a global variable to signify which button (IE onboard in this case)
		Button = 2;
		// Then wake up the button Task to process.
		vTaskNotifyGiveFromISR(xHandle, &xHigherPriorityTaskWoken);
	}
	else if((PIN_PUSHBUTTON_8_ID == id) && (PIN_PUSHBUTTON_8_MASK == mask))
	{
		// Set a global variable to signify which button (IE onboard in this case)
		Button = 3;
		// Then wake up the button Task to process.
		vTaskNotifyGiveFromISR(xHandle, &xHigherPriorityTaskWoken);
	}
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void initializeSWDriver(void)
{
	pio_configure(PIN_PUSHBUTTON_6_PIO, PIN_PUSHBUTTON_6_TYPE, PIN_PUSHBUTTON_6_MASK, PIN_PUSHBUTTON_6_ATTR);
	pio_set_debounce_filter(PIN_PUSHBUTTON_6_PIO, PIN_PUSHBUTTON_6_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_6_PIO, PIN_PUSHBUTTON_6_ID, PIN_PUSHBUTTON_6_MASK, PIN_PUSHBUTTON_6_ATTR, button_handler);
	pio_handler_set_priority(PIN_PUSHBUTTON_6_PIO, PIN_PUSHBUTTON_6_IRQn, 5);
	pio_enable_interrupt(PIN_PUSHBUTTON_6_PIO, PIN_PUSHBUTTON_6_MASK);
	
	pio_configure(PIN_PUSHBUTTON_7_PIO, PIN_PUSHBUTTON_7_TYPE, PIN_PUSHBUTTON_7_MASK, PIN_PUSHBUTTON_7_ATTR);
	pio_set_debounce_filter(PIN_PUSHBUTTON_7_PIO, PIN_PUSHBUTTON_7_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_7_PIO, PIN_PUSHBUTTON_7_ID, PIN_PUSHBUTTON_7_MASK, PIN_PUSHBUTTON_7_ATTR, button_handler);
	pio_handler_set_priority(PIN_PUSHBUTTON_7_PIO, PIN_PUSHBUTTON_7_IRQn, 5);
	pio_enable_interrupt(PIN_PUSHBUTTON_7_PIO, PIN_PUSHBUTTON_7_MASK);
	
	pio_configure(PIN_PUSHBUTTON_8_PIO, PIN_PUSHBUTTON_8_TYPE, PIN_PUSHBUTTON_8_MASK, PIN_PUSHBUTTON_8_ATTR);
	pio_set_debounce_filter(PIN_PUSHBUTTON_8_PIO, PIN_PUSHBUTTON_8_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_8_PIO, PIN_PUSHBUTTON_8_ID, PIN_PUSHBUTTON_8_MASK, PIN_PUSHBUTTON_8_ATTR, button_handler);
	pio_handler_set_priority(PIN_PUSHBUTTON_8_PIO, PIN_PUSHBUTTON_8_IRQn, 5);
	pio_enable_interrupt(PIN_PUSHBUTTON_8_PIO, PIN_PUSHBUTTON_8_MASK);
}

