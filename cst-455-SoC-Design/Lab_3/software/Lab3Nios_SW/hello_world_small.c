#include "sys/alt_stdio.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"

int Decode(int conv);

int main()
{ 
	  // variable initializations
	  int Result = 0x0;
	  int HEX0 = 0x0;
	  int HEX1 = 0x0;
	  int HEX2 = 0x0;

	  /* Event loop never exits. */
	  while (1)
	  {
		  //Reads data from the switches
		  Result = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);

		  //Grabs collected data from the switches
	  	  HEX0 = Result & 0xF;
	  	  HEX1 = (Result & 0xF0) >> 4;
	  	  HEX2 = (Result & 0xF00) >> 8;

	  	  //Decodes data to be used for hex display
	  	  HEX0 = Decode(HEX0);
	  	  HEX1 = Decode(HEX1);
	  	  HEX2 = Decode(HEX2);

	  	  //Combines the decoded data to be used on the display
	  	  Result = (HEX2 << 14) + (HEX1 << 7) + HEX0;

	  	  //Writes data to the hex display
	  	  IOWR_ALTERA_AVALON_PIO_DATA(HEX_BASE, Result);
	  }

  return 0;
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
		case 10:
			conv = 0b0001000;
			break;
		case 11:
			conv = 0b0000011;
			break;
		case 12:
			conv = 0b1000110;
			break;
		case 13:
			conv = 0b0100001;
			break;
		case 14:
			conv = 0b0000110;
			break;
		case 15:
			conv = 0b0001110;
			break;
		default:
			conv = 0b1000000;
			break;
	}
	return conv;
}
