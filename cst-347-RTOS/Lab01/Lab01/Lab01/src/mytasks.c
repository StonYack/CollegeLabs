/*
 * mytasks.c
 *
 * Created: 1/4/2021 12:58:14 PM
 *  Author: Ston Yackamouih
 */ 

#include <asf.h>
#include <time.h>
#include "mytasks.h"

extern void vTaskDelay();

void myButtonTask(void * pvParamaters)
{
	while (true)
	{
		/* Is button pressed? */
		if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE)
		{
			/* Yes, so turn LED on. */
			ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
		}
		else
		{
			/* No, so turn LED off. */
			ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		}
	}
}

void myLEDTask(void * pvParamaters)
{
	Bool LED = 0;
	
	while (true)
	{
		ioport_set_pin_level(LED_0_PIN, LED);
		LED = ~LED;
		vTaskDelay(500);
	}
}