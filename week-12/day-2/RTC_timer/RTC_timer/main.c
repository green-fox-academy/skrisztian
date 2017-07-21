/*
 * RTC_timer.c
 *
 * Created: 2017-07-21 00:54:09
 * Author : Krisztian Stancz
 */ 

#include <string.h>
#include "UART_driver.h"
#include "ds1302.h"

int main(void)
{
	UART_Init();
	sei();
	
	// Setting up STDIO input and output buffer
	// You don't have to understand this!
	//----- START OF STDIO IO BUFFER SETUP
	FILE UART_output = FDEV_SETUP_STREAM(UART_SendCharacter, NULL, _FDEV_SETUP_WRITE);
	stdout = &UART_output;
	FILE UART_input = FDEV_SETUP_STREAM(NULL, UART_GetCharacter, _FDEV_SETUP_READ);
	stdin = &UART_input;
	//----- END OF STDIO IO BUFFER SETUP

	// Try printf
	printf("Startup...\r\n");
	
	rtc_time_t t;
	
	while(1) {
		
		rtc_get_time(&t);
		printf("20%d-%02d-%02d %02d:%02d:%02d\n", t.year, t.month, t.date, t.hour, t.min, t.sec);
		_delay_ms(1000);

	}
	
}

