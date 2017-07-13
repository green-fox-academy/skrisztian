# include "rtc2.h"

void initialize_DS1302(void)
{
	unsigned char sec, min, hr, dy, date, mn, yr;
	sec = 0;
	min = 3;
	hr = 12;
	dy = 5;
	date = 7;
	mn = 7;
	yr = 17;
	
	reset_3w();
	wbyte_3w(0x8e);	// control register
	wbyte_3w(0);	// disable write protect
	reset_3w();
	// wbyte_3w(0x90);	// trickle charger register
	// wbyte_3w(0xab);	// enable, 2 diodes, 8K resistor
	// reset_3w();
	wbyte_3w(0xbe);	// clock burst write (eight registers)
	wbyte_3w(sec);
	wbyte_3w(min);
	wbyte_3w(hr);
	wbyte_3w(dy);
	wbyte_3w(date);
	wbyte_3w(mn);
	wbyte_3w(yr);
	wbyte_3w(0); // must write control register in burst mode
	reset_3w();

}

void reset_3w(void)
{
	RTC_PORT &= ~(1 << RTC_SCLK);	// 0
	RTC_PORT &= ~(1 << RTC_RS);		// 0
	RTC_PORT |= 1<< RTC_RS;			// 1
}

void init_3w(void)
{
	// set all pins as output
	RTC_DDR |= (1 << RTC_IO) | (1 << RTC_RS) | (1 << RTC_SCLK);
	
	// set all pins as 0
	RTC_PORT &= ~(1 << RTC_IO);
	RTC_PORT &= ~(1 << RTC_RS);
	RTC_PORT &= ~(1 << RTC_SCLK);
}

void wbyte_3w(unsigned char W_Byte) //write into DS1302
{
	unsigned char i;

	//TRISB = 0b00000011; //RB3 as o/p
	RTC_DDR |= 1 << RTC_IO;
	for(i = 0; i < 8; i++)
	{
		// RTC_IO = 0;
		RTC_PORT &= ~(1 << RTC_IO);	// 0
		if(W_Byte & (1 << i))
		{
			// RTC_IO = 1; /* set port pin high to read data */
			RTC_PORT |= 1 << RTC_IO;	// 1
		}
		// RTC_SCLK = 0;
		RTC_PORT &= ~(1 << RTC_SCLK);	// 0
		
		// RTC_SCLK = 1;
		RTC_PORT |= 1 << RTC_SCLK;	// 1
		W_Byte >>= 1;
	}
}

unsigned char rbyte_3w(void)	//read from DS1302
{
	unsigned char i;
	unsigned char R_Byte;
	unsigned char TmpByte;
	
	// TRISB = 0b00001011; //RB3 as i/p
	RTC_DDR &= ~(1 << RTC_IO);

	R_Byte = 0x00;
	// RTC_IO = 1;
	RTC_PORT |= 1 << RTC_IO;
	
	for(i = 0; i < 8; ++i)
	{
		// RTC_SCLK = 1;
		RTC_PORT |= 1 << RTC_SCLK;
		
		// RTC_SCLK = 0;
		RTC_PORT &= ~(1 << RTC_SCLK);
		
		// TmpByte = (uchar)RTC_IO;
		TmpByte = (unsigned char) (RTC_PIN & (1 << RTC_IO));
		TmpByte <<= (7 - RTC_IO);
		
		R_Byte >>= 1;
		R_Byte |= TmpByte;
	}

	// TRISB = 0b00000011; //RB3 as o/p
	RTC_DDR |= 1 << RTC_IO;
	return R_Byte;
}
