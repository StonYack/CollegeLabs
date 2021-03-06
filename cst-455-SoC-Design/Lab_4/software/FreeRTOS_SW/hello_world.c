#include "sys/alt_stdio.h"
#include "system.h"
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"


void task1(void *p);
void task2(void *p);

int count(int counter);
void blink();
void cylon();
void NewPattern();

char message = 'N';

int main()
{
	// Thread tasks
	xTaskCreate(task1, "task1", 512, NULL, 2, NULL);
	xTaskCreate(task2, "task2", 512, NULL, 2, NULL);

	//Start the tasks
	vTaskStartScheduler();

  return 0;
}

void task1(void *p)
{
	int BUT = 0;

	while(1)
	{
		BUT = IORD(KEYS_BASE, 0);	// takes the buttons data
		// decides what to do with the LED's based on button input
		switch(BUT)
		{
			case 14:
				message = 'a';
				break;
			case 13:
				message = 'b';
				break;
			case 11:
				message = 'c';
				break;
			case 7:
				message = 'd';
				break;
		}
	}
}


void task2(void *p)
{
	int counter = 0;

	while(1)
	{
		//Receives the message and outputs the corresponding pattern
		switch(message)
		{
			case 'a':
				blink();
				counter = 0;
				break;
			case 'b':
				cylon();
				counter = 0;
				IOWR(LEDS_BASE, 0, 0);
				break;
			case 'c':
				counter = count(counter);
				break;
			case 'd':
				NewPattern();
				counter = 0;
				IOWR(LEDS_BASE, 0, 0);
				break;
		}
	}
}


void blink()
{
	IOWR(LEDS_BASE, 0, 0x3FF);
	vTaskDelay(500);
	IOWR(LEDS_BASE, 0, 0x000);
	vTaskDelay(500);
}

//increments through the LED's
int count(int counter)
{
	counter += 1;
	IOWR(LEDS_BASE, 0, counter);
	vTaskDelay(250);
	return counter;
}

//Creates the Cylon pattern on LED's
void cylon()
{
	//Initialization
	int cur = 0;
	int max = 0b10000000000;
	int min = 0;

	// stays in loop while either KEY1 is pressed or nothing is pressed
	while(message == 'b')
	{
		cur = 1;
		// Stay in loop while current doesnt equal max and Key1 is pressed or nothing is pressed
		while((cur != max) & (message == 'b'))
		{
			IOWR(LEDS_BASE, 0, cur);	// write current
			cur = cur << 1;	// shift left to perform pattern
			vTaskDelay(50);	// Don't make the LED's go too zoomy

		}
		// Stay in loop while current doesnt equal minimum and Key1 is pressed or nothing is pressed
		while((cur != min) & ((message == 'b')))
		{
			cur = cur >> 1; // shift right to perform pattern
			IOWR(LEDS_BASE, 0, cur); // write current
			vTaskDelay(50); // Don't make the LED's go too zoomy
		}
	}
}

/* Creates a different pattern for the LED's
			LED:			1000000001
							1100000011
							1110000111
							1111001111
							1111111111
							1111001111
							1110000111
							1100000011
							1000000001
							1100000011...
*/
void NewPattern()
{
	//initialization
	int cur = 0;
	int sec1 = 0;
	int sec2 = 0;

	// stays in loop while either KEY3 is pressed or nothing is pressed
	while(message == 'd')
	{
		// Set up start of pattern
		sec1 = 0b1000000000;
		sec2 = 0b00001;
		cur = sec1 + sec2;
		// Stay in loop while section 2 isn't greater than 16 and Key1 is pressed or nothing is pressed
		while((sec2 < 16) & (message == 'd'))
		{
			IOWR(LEDS_BASE, 0, cur);	// writes current data
			sec1 = (sec1 >> 1) + 512;	// Sets up section 1 of the pattern
			sec2 = (sec2 << 1) + 1;		// Sets up section 2 of the pattern
			cur = sec1 + sec2;			// Form output data
			vTaskDelay(50);				// Don't make the LED's go too zoomy
		}
		// Stay in loop while until section 2 equals 0 and Key1 is pressed or nothing is pressed
		while((sec2 >= 0) & (message == 'd'))
		{
			IOWR(LEDS_BASE, 0, cur);	// writes current data
			sec1 = (sec1 << 1) - 1024;	// Sets up section 1 of the pattern
			sec2 = (sec2 - 1) >> 1 ;	// Sets up section 2 of the pattern
			cur = sec1 + sec2;			// Form output data
			vTaskDelay(50);				// Don't make the LED's go too zoomy
		}
	}
}
