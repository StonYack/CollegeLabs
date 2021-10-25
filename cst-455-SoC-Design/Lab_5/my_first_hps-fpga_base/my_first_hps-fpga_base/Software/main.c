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

int main()
{
	int fd = 0;
	void * virtual_base;
	void * h2p_lw_led_addr;

	PROT_READ;
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
						((unsigned long) (ALT_LWFPGASLVS_OFST + PIO_LED_BASE)
						& (unsigned long) (HW_REGS_MASK));

	int loop_count = 0;
	int led_mask = 0x01;
	int led_direction = 0;		// 0: left to right direction
	
	while(loop_count < 60)
	{
		// control LED, Add ~ because the LED is active-low
		*(uint32_t *) h2p_lw_led_addr = ~led_mask;
		
		// wait 100ms
		usleep(100 * 1000);
		
		// update LED mask
		if(led_direction == 0)
		{
			led_mask <<= 1;
			if(led_mask == (0x01 << (PIO_LED_DATA_WIDTH - 1)))
				led_direction = 1;
		}
		else
		{
			led_mask >>= 1;
			if(led_mask == 0x01)
			{
				led_direction = 0;
				loop_count++;
			}
		}
	}  // while				
}