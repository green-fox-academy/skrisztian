#include <avr/io.h>

#define RTC_SCLK	4
#define RTC_RS		5
#define RTC_IO		3

#define RTC_DDR		DDRC
#define RTC_PORT	PORTC
#define RTC_PIN		PINC

void initialize_DS1302(void);
void reset_3w(void);
void wbyte_3w(unsigned char W_Byte); //write into DS1302
unsigned char rbyte_3w(void);	//read from DS1302
void init_3w(void);