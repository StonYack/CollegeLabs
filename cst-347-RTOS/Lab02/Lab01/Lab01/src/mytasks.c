/*
 * mytasks.c
 *
 * Created: 1/4/2021 12:58:14 PM
 *  Author: Ston Yackamouih
 */ 

#include <asf.h>
#include <time.h>
#include "mytasks.h"
#include "OITExpansionBoard.h"
#include "swDriver.h"
#include "ledDriver.h"

void taskSystemControl(void * pvParameters)
{
	uint8_t SwPress = 0;
	
	while (true)
	{
		SwPress = readSW();
		//if SW1 is pressed, LED1 will flash, if pressed again, LED2 will flash, if pressed again, LED3 will flash
		if(SwPress == 1)
		{
			Button1();
		}
		
		if(SwPress == 2)
		{
			Button2();
		}
		
		if(SwPress == 3)
		{
			Button3();
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
