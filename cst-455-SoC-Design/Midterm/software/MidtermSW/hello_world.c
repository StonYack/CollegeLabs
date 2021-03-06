#include "sys/alt_stdio.h"
#include "system.h"
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"


void task1(void *p);
void task2(void *p);

int Decode(int conv);
void cylon();
void NewPattern();

int main()
{
	// Thread tasks
	xTaskCreate(task1, "task1", 512, NULL, 2, NULL);
	xTaskCreate(task2, "task2", 512, NULL, 2, NULL);

	//Start the tasks
	vTaskStartScheduler();

  return 0;
}

//task 1 takes one of the threads and controls the Hex display plus the switches
void task1(void *p)
{
	int SW = 0;
	int Result1 = 0;
	int FUNC = 0;
	int Num0 = 0;
	int Num1 = 0;
	int HEX0 = 0;
	int HEX1 = 0;
	int HEX2 = 0;
	int HEX3 = 0;
	int HEX4 = 0;
	int HEX5 = 0;

	while(1)
	{
		SW = IORD(SWITCHES_BASE, 0);	// takes switches reading

		FUNC = (SW & 0x300) >> 8;		// take the info needed for the function to perform

		Num0 = (SW & 0x00F);			// take the info for number 1

		Num1 = (SW & 0x0F0) >> 4;		// take the info for number 2


		//Decides which function to perform
		switch(FUNC)
		{
			case 0:
				Result1 = 0;
				Num0 = 0;
				Num1 = 0;
				break;
			case 1:
				Result1 = Num0 + Num1;
				break;
			case 2:
				Result1 = Num0 - Num1;
				break;
			case 3:
				Result1 = Num0 * Num1;
				break;
			default:
				Result1 = 0;
				Num0 = 0;
				Num1 = 0;
		}

		// Sets up base 10 format for the Hex display
		if(Num0 > 9)
		{
			HEX5 = Num0 / 10;
		}
		else
		{
			HEX5 = 0;
		}
		HEX4 = Num0 % 10;

		if(Num1 > 9)
		{
			HEX3 = Num1 / 10;
		}
		else
		{
			HEX3 = 0;
		}
		HEX2 = Num1 % 10;

		if(Result1 > 9)
		{
			HEX1 = Result1 / 10;
		}
		else
		{
			HEX1 = 0;
		}
		HEX0 = Result1 % 10;

		// decode data
		HEX0 = Decode(HEX0);
	  	HEX1 = Decode(HEX1);
	  	HEX2 = Decode(HEX2);
		HEX3 = Decode(HEX3);
	  	HEX4 = Decode(HEX4);
	  	HEX5 = Decode(HEX5);

	  	// write to each to display
		IOWR(HEX_0_BASE, 0, HEX0);
		IOWR(HEX_1_BASE, 0, HEX1);
		IOWR(HEX_2_BASE, 0, HEX2);
		IOWR(HEX_3_BASE, 0, HEX3);
		IOWR(HEX_4_BASE, 0, HEX4);
		IOWR(HEX_5_BASE, 0, HEX5);
	}

}

//task 2 takes the other thread and controls the LED's plus the buttons
void task2(void *p)
{
	//Initializations
	int BUT = 0;
	int counter = 0;

	while(1)
	{
		BUT = IORD(KEYS_BASE, 0);	// takes the buttons data

		// decides what to do with the LED's based on button input
		switch(BUT)
		{
			case 14:
				IOWR(LEDS_BASE, 0, 0x0);	// clears LED's
				counter = 0;
				break;
			case 13:
				cylon();
				counter = 0;
				IOWR(LEDS_BASE, 0, 0);
				break;
			case 11:
				counter += 1;
				IOWR(LEDS_BASE, 0, counter);
				vTaskDelay(250);
				break;
			case 7:
				NewPattern();
				counter = 0;
				IOWR(LEDS_BASE, 0, 0);
				break;
		}
	}
}

//Decodes Hex data
int Decode(int conv)
{
	switch(conv)
	{
		case 0:
			conv = 0b1000000;
			break;
		case 1:
			conv = 0b1111001;
			break;
		case 2:
			conv = 0b0100100;
			break;
		case 3:
			conv = 0b0110000;
			break;
		case 4:
			conv = 0b0011001;
			break;
		case 5:
			conv = 0b0010010;
			break;
		case 6:
			conv = 0b0000010;
			break;
		case 7:
			conv = 0b1111000;
			break;
		case 8:
			conv = 0b0000000;
			break;
		case 9:
			conv = 0b0011000;
			break;
		default:
			conv = 0b1000000;
	}
	return conv;
}

//Creates the Cylon pattern on LED's
void cylon()
{
	//Initialization
	int cur = 0;
	int max = 0b10000000000;
	int min = 0;

	// stays in loop while either KEY1 is pressed or nothing is pressed
	while((IORD(KEYS_BASE, 0) == 13) | (IORD(KEYS_BASE, 0) == 15))
	{
		cur = 1;
		// Stay in loop while current doesnt equal max and Key1 is pressed or nothing is pressed
		while((cur != max) & ((IORD(KEYS_BASE, 0) == 13) | (IORD(KEYS_BASE, 0) == 15)))
		{
			IOWR(LEDS_BASE, 0, cur);	// write current
			cur = cur << 1;	// shift left to perform pattern
			vTaskDelay(50);	// Don't make the LED's go too zoomy

		}
		// Stay in loop while current doesnt equal minimum and Key1 is pressed or nothing is pressed
		while((cur != min) & ((IORD(KEYS_BASE, 0) == 13) | (IORD(KEYS_BASE, 0) == 15)))
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
	while((IORD(KEYS_BASE, 0) == 7) | (IORD(KEYS_BASE, 0) == 15))
	{
		// Set up start of pattern
		sec1 = 0b1000000000;
		sec2 = 0b00001;
		cur = sec1 + sec2;
		// Stay in loop while section 2 isn't greater than 16 and Key1 is pressed or nothing is pressed
		while((sec2 < 16) & ((IORD(KEYS_BASE, 0) == 7) | (IORD(KEYS_BASE, 0) == 15)))
		{
			IOWR(LEDS_BASE, 0, cur);	// writes current data
			sec1 = (sec1 >> 1) + 512;	// Sets up section 1 of the pattern
			sec2 = (sec2 << 1) + 1;		// Sets up section 2 of the pattern
			cur = sec1 + sec2;			// Form output data
			vTaskDelay(50);				// Don't make the LED's go too zoomy
		}
		// Stay in loop while until section 2 equals 0 and Key1 is pressed or nothing is pressed
		while((sec2 >= 0) & ((IORD(KEYS_BASE, 0) == 7) | (IORD(KEYS_BASE, 0) == 15)))
		{
			IOWR(LEDS_BASE, 0, cur);	// writes current data
			sec1 = (sec1 << 1) - 1024;	// Sets up section 1 of the pattern
			sec2 = (sec2 - 1) >> 1 ;	// Sets up section 2 of the pattern
			cur = sec1 + sec2;			// Form output data
			vTaskDelay(50);				// Don't make the LED's go too zoomy
		}
	}
}
