#include <io.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "sys/alt_irq.h"
#include "system.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sys/alt_stdio.h"
#include "altera_avalon_timer_regs.h"


void task1(void *p);
void task2(void *p);

int Switch();
void HexDecode(char decode);
int TranslateDecode(char decode);
void ServoControl();

char Display = 'X';
int Overflows = 0;
int Go = 0;
int Translate;

void timer_0_ISR(void *context, alt_32 id)
{
	if(Overflows == 10)
	{
		Overflows = 0;
		IOWR(SERVO_0_BASE, 0, Translate); // Unable to run all 6 Servo's due to power limitations
		// Adding more Servo's currently would hurt performance.
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE) & (0xFFFD)); // stop timer
	}
	else
	{
		IOWR(SERVO_0_BASE, 0, 0x0);
		Overflows++;
	}

   // clear irq
   IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
}

void timer_1_ISR(void *context, alt_32 id)
{
	if (Overflows == 13)
	{
		IOWR(SERVO_0_BASE, 0, 0x7);
		Overflows = 0;
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_1_BASE, IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_1_BASE) & (0xFFFD)); // stop timer
	}
	else
	{
		IOWR(SERVO_0_BASE, 0, 0);
		Overflows++;
	}

   // clear irq
   IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_1_BASE, 0);
}

void timer_2_ISR(void *context, alt_32 id)
{
	if (Overflows == 1)
	{
		IOWR(PIEZO_0_BASE, 0, 0x0);	// SERVO_0_BASE
		Overflows = 0;
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_2_BASE, IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_2_BASE) & (0xFFFD)); // stop timer
	}
	else
	{
		IOWR(PIEZO_0_BASE, 0, 1);	// SERVO_0_BASE
		Overflows++;
	}

   // clear irq
   IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_2_BASE, 0);
}

int main()
{
  //Timer Setup
  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, ALTERA_AVALON_TIMER_CONTROL_CONT_MSK | ALTERA_AVALON_TIMER_CONTROL_START_MSK | ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);

  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);    // Clear IRQ status
  IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE, 0x5F00); //0x28
  IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, 0x1); //0x8

  alt_irq_register(TIMER_0_IRQ, 0, timer_0_ISR);	//Set up ISR

  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_1_BASE, ALTERA_AVALON_TIMER_CONTROL_CONT_MSK | ALTERA_AVALON_TIMER_CONTROL_START_MSK | ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);

  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_1_BASE, 0);    // Clear IRQ status
  IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_1_BASE, 0x20C0); //0x28
  IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_1_BASE, 0x1); //0x8

  alt_irq_register(TIMER_1_IRQ, 0, timer_1_ISR);	//Set up ISR

  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_2_BASE, ALTERA_AVALON_TIMER_CONTROL_CONT_MSK | ALTERA_AVALON_TIMER_CONTROL_START_MSK | ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);

  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_2_BASE, 0);    // Clear IRQ status
  IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_2_BASE, 0x28); //0x28
  IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_2_BASE, 0x0); //0x8

  alt_irq_register(TIMER_2_IRQ, 0, timer_2_ISR);	//Set up ISR

  xTaskCreate(task1, "task1", 512, NULL, 2, NULL);	// Create task to handle Switches and other core components
  xTaskCreate(task2, "task2", 512, NULL, 2, NULL);	// Create task to handle Hex display
  vTaskStartScheduler();	// Start Tasks

  return 0;
}

void task1(void *p)
{
	while(1)
	{
		Translate = Switch();	// Receive Switches translation
		if (Go == 1)
		{
			ServoControl(Translate);	// Send the data to the Servo's
		}
	}
}

int Switch()
{
	int SW = IORD(SW_0_BASE, 0);	// Read switches
	char Character;	// Character being translated
	int Result;	// End result

	if((SW & 0x8) > 0)	// SW[3] is the control switch
	{
		switch(SW & 0x7)	//Encode into ASCII character
		{
			case 0:
				Character = 'A';
				break;
			case 1:
				Character = 'B';
				break;
			case 2:
				Character = 'C';
				break;
			case 3:
				Character = 'D';
				break;
			case 4:
				Character = 'E';
				break;
			case 5:
				Character = 'F';
				break;
			default:
				Character = 'X';
				break;
		}
		Go = 1;
	}
	else
	{
		Go = 0;
	}
	HexDecode(Character);	// Decode character for the Hex Display
	Result = TranslateDecode(Character); // Decode data for the Servo's

	return Result;
}

void HexDecode(char decode)
{
	switch(decode)	// Decode for Hex display
		{
			case 'A':
				Display = 0b0001000;
				break;
			case 'B':
				Display = 0b0000011;
				break;
			case 'C':
				Display = 0b1000110;
				break;
			case 'D':
				Display = 0b0100001;
				break;
			case 'E':
				Display = 0b0000110;
				break;
			case 'F':
				Display = 0b0001110;
				break;
			default:
				Display = 0b1000000;
				break;
		}
}

int TranslateDecode(char decode)
{
	int num;
	switch(decode)	// Decode for Servo's
	{
		case 'A':
			num = 0b000001;
			break;
		case 'B':
			num = 0b000101;
			break;
		case 'C':
			num = 0b000011;
			break;
		case 'D':
			num = 0b001011;
			break;
		case 'E':
			num = 0b001001;
			break;
		case 'F':
			num = 0b000111;
			break;
		default:
			num = 0b000000;
			break;
	}
	return num;
}
/*************************************************************************
Explanation: Servo actuates on the Go signal given by SW[3], while that
is active the Servo's will take the translation values from the switches.
in this function we run the timer_0 5 different times to get the longest
swing possible on the servo's (like 50 degrees). After a few seconds we
ring the buzzer to signal the user that the generation will be resetting
now. The servo's then begin resetting. After the reset we will begin
waiting for the next go signal.
Note: Once a translation has started it can't be stopped
*************************************************************************/
void ServoControl()
{
	int Ninety = 0;
	while(Ninety != 5) // Continuously try to turn the Servo's (takes 5 for max turn that is supposed to be 90 degrees)
	{
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x7 | IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE));	// Start timer for PWM to turn 90 degrees
		vTaskDelay(1000);
		Ninety++;
	}
	vTaskDelay(5000);
	Ninety = 0;
	while (Ninety != 100000)	// Make an awful noise on the piezo
	{
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_2_BASE, 0x7 | IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_2_BASE));	// Start timer for Piezo to buzz
		Ninety++;
	}
	Ninety = 0;
	while(Ninety != 5)	// Continuously try to turn the Servo's (takes 5 for max turn that is supposed to be 90 degrees)
	{
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_1_BASE, 0x7 | IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_1_BASE));	// Start timer for PWM to turn back 90 degrees
		vTaskDelay(1000);																						// Servo's only actually turn about 30 degrees
		Ninety++;
	}
}

void task2(void *p)
{
	while(1)
	{
		IOWR(HEX_0_BASE, 0, Display);	// Write to Hex Display
	}
}
