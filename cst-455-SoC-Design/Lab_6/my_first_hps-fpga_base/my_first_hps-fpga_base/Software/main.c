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

int Decode(int conv);

int main()
{
	int fd = 0;
	void * virtual_base;
	void * h2p_lw_SW_addr;
	void * h2p_lw_HEX0_addr;
	void * h2p_lw_HEX1_addr;
	void * h2p_lw_HEX2_addr;
	void * h2p_lw_HEX3_addr;
	void * h2p_lw_HEX4_addr;
	void * h2p_lw_HEX5_addr;
	
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

	h2p_lw_SW_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + SWITCHES_BASE)
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

	h2p_lw_HEX3_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_3_BASE)
						& (unsigned long) (HW_REGS_MASK));

	h2p_lw_HEX4_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_4_BASE)
						& (unsigned long) (HW_REGS_MASK));

	h2p_lw_HEX5_addr = virtual_base +
						((unsigned long) (ALT_LWFPGASLVS_OFST + HEX_5_BASE)
						& (unsigned long) (HW_REGS_MASK));

	//int loop_count = 0;

	int SW = 0;
	int Num0 = 0;
	int Num1 = 0;
	int Num2 = 0;

	int HEX0 = 0;
	int HEX1 = 0;
	int HEX2 = 0;
	int HEX3 = 0;
	int HEX4 = 0;
	int HEX5 = 0;

	while(1)
	{
		// control LED, Add ~ because the LED is active-low
		//*(uint32_t *) h2p_lw_led_addr = ~led_mask;
		
		//Read Switches
		SW = *(uint32_t *) h2p_lw_SW_addr;

		Num2 = (SW & 0x300) >> 8;		// take the info for number 2

		Num0 = (SW & 0x00F);			// take the info for number 1

		Num1 = (SW & 0x0F0) >> 4;		// take the info for number 0

		if(Num0 > 9)
		{
			HEX1 = Num0 / 10;
		}
		else
		{
			HEX1 = 0;
		}
		HEX0 = Num0 % 10;

		if(Num1 > 9)
		{
			HEX3 = Num1 / 10;
		}
		else
		{
			HEX3 = 0;
		}
		HEX2 = Num1 % 10;

		if(Num2 > 9)
		{
			HEX5 = Num2 / 10;
		}
		else
		{
			HEX5 = 0;
		}
		HEX4 = Num2 % 10;

		// decode and write data
		*(uint32_t *) h2p_lw_HEX0_addr = Decode(HEX0);
		*(uint32_t *) h2p_lw_HEX1_addr = Decode(HEX1);
		*(uint32_t *) h2p_lw_HEX2_addr = Decode(HEX2);
		*(uint32_t *) h2p_lw_HEX3_addr = Decode(HEX3);
		*(uint32_t *) h2p_lw_HEX4_addr = Decode(HEX4);
		*(uint32_t *) h2p_lw_HEX5_addr = Decode(HEX5);
	}  // while				
}

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