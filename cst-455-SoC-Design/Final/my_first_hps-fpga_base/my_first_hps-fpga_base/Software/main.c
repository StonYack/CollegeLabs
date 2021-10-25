#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "hwlib.h"
#include "hps_0.h"

#define HW_REGS_BASE (ALT_STM_OFST)
#define HW_REGS_SPAN (0x04000000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)
#define KeyRead (*(uint32_t *) h2p_lw_KEY_addr)

void KeyControl(int Val);
void SWControl(int Val);
int HexDecode(int Val);
void cylon();
void NewPattern();

void * h2p_lw_led_addr;		// LED
void * h2p_lw_SW_addr;		// SW
void * h2p_lw_HEX0_addr;	// Hex
void * h2p_lw_HEX1_addr;
void * h2p_lw_HEX2_addr;
void * h2p_lw_KEY_addr;		// Key
void * h2p_lw_FIB_addr;		// Fib

int main()
{
	int fd = 0;
	void * virtual_base;

	if((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
	{
		printf("ERROR: could not open \"/dev/mem\"...\n");
		return (1);
	}

	virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), 
						MAP_SHARED, fd, HW_REGS_BASE);

	if(virtual_base == MAP_FAILED)
	{
		printf("ERROR: mmap() failed...\n");
		close(fd);
		return(1);
	}

	h2p_lw_led_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + LED_0_BASE)
						& (unsigned long) (HW_REGS_MASK));

	h2p_lw_SW_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + SW_0_BASE)
						& (unsigned long) (HW_REGS_MASK));
					
	h2p_lw_HEX0_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_0_BASE)
						& (unsigned long) (HW_REGS_MASK));
	
	h2p_lw_HEX1_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_1_BASE)
						& (unsigned long) (HW_REGS_MASK));
	
	h2p_lw_HEX2_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_2_BASE)
						& (unsigned long) (HW_REGS_MASK));
	
	h2p_lw_KEY_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + KEY_0_BASE)
						& (unsigned long) (HW_REGS_MASK));

	h2p_lw_FIB_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + FIB_0_BASE)
						& (unsigned long) (HW_REGS_MASK));
	
	int Key = 0;

	while(1)
	{
		//Read Buttons
		Key = *(uint32_t *) h2p_lw_KEY_addr;
		
		//*(uint32_t *) h2p_lw_HEX0_addr = HexDecode(0);
		//*(uint32_t *) h2p_lw_HEX1_addr = HexDecode(0);
		//*(uint32_t *) h2p_lw_HEX2_addr = HexDecode(0);
		//printf("%d\n", Key );

		KeyControl(Key);
	} 			
}

void SWControl(int Val)
{	
	// Variables
	int FUNC = 0;
	int Num0 = 0;
	int Num1 = 0;

	int HEX0 = 0;
	int HEX1 = 0;
	int HEX2 = 0;
	int HEX3 = 0;
	int HEX4 = 0;
	int HEX5 = 0;

	int Result1 = 0;

	FUNC = (Val & 0x300) >> 8;		// take the info needed for the function to perform

	Num0 = (Val & 0x00F);			// take the info for number 1

	Num1 = (Val & 0x0F0) >> 4;		// take the info for number 2


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
			Result1 = Num1 - Num0;
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
	HEX0 = HexDecode(HEX0);	
	HEX1 = HexDecode(HEX1);
	HEX2 = HexDecode(HEX2);
	HEX3 = HexDecode(HEX3);
	HEX4 = HexDecode(HEX4);
	HEX5 = HexDecode(HEX5);

	//Combine values to be sent to the displays
	HEX0 = (HEX1 << 7) | HEX0;
	HEX1 = (HEX3 << 7) | HEX2;
	HEX2 = (HEX5 << 7) | HEX4;

	// write to each to display
	*(uint32_t *) h2p_lw_HEX0_addr = HEX0;	
	*(uint32_t *) h2p_lw_HEX1_addr = HEX2;// Ended up swapping HEX2 and HEX1 values
	*(uint32_t *) h2p_lw_HEX2_addr = HEX1;// Made more sense visually 
}

int HexDecode(int Val)	// Decodes the input to be legible for Hex display
{
	int conv = 0;
	switch(Val)
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

void KeyControl(int Val)
{
	int FibNum = 0;
	int SW = 0;

	int HEX1 = 0;
	int HEX0 = 0;

	int HexZero = HexDecode(0);

	switch(Val)	
	{
		case 14:	// Key 0 pressed  = Run the Switch calculations
			//Read Switches
			SW = *(uint32_t *) h2p_lw_SW_addr;
			SWControl(SW);	//Control the Hex displays based on switches
			break;
		case 13:	// Key 1 pressed = Run the Fibonacci Sequence based on SW[3:0]
			SW = *(uint32_t *) h2p_lw_SW_addr;
			SW = SW & 0xF;

			*(uint32_t *) h2p_lw_FIB_addr = SW;	// Write then Read Fibonacci Sequence Module
			FibNum = *(uint32_t *) h2p_lw_FIB_addr;

			if(FibNum > 9)
			{
				HEX1 = FibNum / 10;
			}
			else
			{
				HEX1 = 0;
			}
			HEX0 = FibNum % 10;

			HEX0 = HexDecode(HEX0);	//Decode Hex values for Display
			HEX1 = HexDecode(HEX1);

			*(uint32_t *) h2p_lw_HEX0_addr = (HEX1 << 7) | HEX0;	// Send Data from Fibonacci sequence
			*(uint32_t *) h2p_lw_HEX1_addr = (HexZero << 7) | HexZero;	// 0 out rest of the Hex Display
			*(uint32_t *) h2p_lw_HEX2_addr = (HexZero << 7) | HexZero;
			break;
		case 11:
			cylon();	// Run Cylon Pattern
			break;
		case 7:
			NewPattern();	// Run New Pattern 
			break;
	}
}

void cylon()
{
	//Initialization
	int cur = 0;
	int max = 0b10000000000;
	int min = 0;
	int Zeros = HexDecode(0);
	Zeros = (Zeros << 7) | Zeros;

	*(uint32_t *) h2p_lw_HEX0_addr = Zeros;
	*(uint32_t *) h2p_lw_HEX1_addr = Zeros;
	*(uint32_t *) h2p_lw_HEX2_addr = Zeros;

	// stays in loop while either KEY1 is pressed or nothing is pressed
	while((KeyRead == 13) | (KeyRead == 15))
	{
		cur = 1;
		// Stay in loop while current doesnt equal max and Key1 is pressed or nothing is pressed
		while((cur != max) & ((KeyRead == 13) | (KeyRead == 15)))
		{
			*(uint32_t *)h2p_lw_led_addr = cur;	// write current
			cur = cur << 1;	// shift left to perform pattern
			usleep(100 * 500);	// Don't make the LED's go too zoomy
		}
		// Stay in loop while current doesnt equal minimum and Key1 is pressed or nothing is pressed
		while((cur != min) & ((KeyRead == 13) | (KeyRead == 15)))
		{
			cur = cur >> 1; // shift right to perform pattern
			*(uint32_t *)h2p_lw_led_addr = cur; // write current
			usleep(100 * 500); // Don't make the LED's go too zoomy
		}
	}
}

void NewPattern()
{
	//initialization
	int cur = 0;
	int sec1 = 0;
	int sec2 = 0;
	int Display = 0;

	// stays in loop while either KEY3 is pressed or nothing is pressed
	while((KeyRead == 7) | (KeyRead == 15))
	{
		// Set up start of pattern
		sec1 = 0b1000000000;
		sec2 = 0b00001;
		cur = sec1 + sec2;
		Display = 0;
		// Stay in loop while section 2 isn't greater than 16 and Key1 is pressed or nothing is pressed
		while((sec2 < 16) & ((KeyRead == 7) | (KeyRead == 15)))
		{
			*(uint32_t *)h2p_lw_led_addr = cur;	// writes current data
			sec1 = (sec1 >> 1) + 512;	// Sets up section 1 of the pattern
			sec2 = (sec2 << 1) + 1;		// Sets up section 2 of the pattern
			cur = sec1 + sec2;			// Form output data
			Display++;
			*(uint32_t *) h2p_lw_HEX0_addr = (HexDecode(Display) << 7) | HexDecode(Display);
			*(uint32_t *) h2p_lw_HEX1_addr = (0x3FF << 7) | 0x3FF;
			*(uint32_t *) h2p_lw_HEX2_addr = (HexDecode(Display) << 7) | HexDecode(Display);
			usleep(100 * 500);				// Don't make the LED's go too zoomy
		}
		Display = 0;
		// Stay in loop while until section 2 equals 0 and Key1 is pressed or nothing is pressed
		while((sec2 >= 0) & ((KeyRead == 7) | (KeyRead == 15)))
		{
			*(uint32_t *)h2p_lw_led_addr = cur;	// writes current data
			sec1 = (sec1 << 1) - 1024;	// Sets up section 1 of the pattern
			sec2 = (sec2 - 1) >> 1 ;	// Sets up section 2 of the pattern
			cur = sec1 + sec2;			// Form output data
			Display++;
			*(uint32_t *) h2p_lw_HEX0_addr = (0x3FF << 7) | 0x3FF;
			*(uint32_t *) h2p_lw_HEX1_addr = (HexDecode(Display) << 7) | HexDecode(Display);
			*(uint32_t *) h2p_lw_HEX2_addr = (0x3FF << 7) | 0x3FF;
			usleep(100 * 500);			// Don't make the LED's go too zoomy
		}
	}
}
