/*
 * RTC_timer_test.c
 *
 * Created: 2017-07-06 16:03:32
 * Author : Krisztian Stancz
 */ 

// sensor d2
// output d6

#include <avr/io.h>
#include <avr/interrupt.h>
#include "rtc2.h"
#include "p_utils.h"
#include "UART_driver.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

void init_led(void)
{
	// Set to input
	DDRD &= ~(1 << DDRD2); 
	
	// Set to output
	DDRD |= 1 << DDRD6;
	
	// Turn off pind6
	PORTB &= ~(1 << DDRD6);
	
	return;
}

void init_pin_change(void)
{
	// Enable interrupts on port
	// PCICR |= 1 << PCIE0;
	
	// Enable interrupt on pin
	EIMSK |= 1 << INT0;
	
	// PCMSK0 |= 1 << PCINT4;
	
	// rising edge
	EICRA |= (1 << ISC01) | (1 << ISC00);
	
	return;
}

void init_timer0(void)
{
	 //timer0 prescaler 256 -> we need 69.44 steps
	 OCR0A = 7;
	 TCCR0A |= 1<< WGM01;
	 TIMSK0 |= 1 << OCIE0A;		// enable CTC interrupt
	 
	 return;
}

void init(void)
{
	init_led();
	init_pin_change();
	init_timer0();
	UART_init();
	init_3w();
	
	// Enable global interrupts
	sei();
}

ISR(INT0_vect)
{
	PORTD |= (1 << PORTD6);	// turn D6 LED pin on
	TCCR0B |= (1<< CS02);		// turn on timer
	
}

ISR(TIMER0_COMPA_vect)
{
	PORTD &= ~(1 << PORTD6);			// turn led pin off
	TCCR0B &= ~(1 << CS02);			// turn off timer
	TCNT0 = 0;
	
}

int main(void)
{
	init();
	
	
	
	// Setting up STDIO input and output buffer
	// You don't have to understand this!
	//----- START OF STDIO IO BUFFER SETUP
	FILE UART_output = FDEV_SETUP_STREAM((void *)UART_send_character, NULL, _FDEV_SETUP_WRITE);
	stdout = &UART_output;
	FILE UART_input = FDEV_SETUP_STREAM(NULL, (void *)UART_get_character, _FDEV_SETUP_READ);
	stdin = &UART_input;
	//----- END OF STDIO IO BUFFER SETUP
	
	
	
	// unsigned char sec = 0;
	// unsigned char temp;
	// struct rtc_time ds1302;
	// struct rtc_time* rtc;
	// char temp_buffer[10];
	// rtc = &ds1302;


	// ds1302_update(rtc);				// update all fields in the struct
	// printf("seconds: %d\n", rtc->second);	
	// printf("seconds: %c\n", rtc->second);	
	
/*	
	rtc->second = 0;
	rtc->minute = 47;
	rtc->hour = 9;
	rtc->day = 5;
	rtc->date = 7;
	rtc->month = 7;
	rtc->year = 17;
	rtc->hour_format = 12;
*/
	/*
	ds1302_set_time(rtc, MIN, 48);	//set the seconds to 31
	
	ds1302_update_time(rtc, SEC);	//read the seconds
	printf("seconds: %d\n", rtc->second);	
	*/
	
	printf("Startup...\n");
	
	initialize_DS1302();
	
	
	while (1)   // read seconds and send it via uart continuously
	{
		// display_write();
		/*
		gets(temp_buffer);
		printf("seconds: %d\n", rtc->second);	
		printf("minute: %d\n", rtc->minute);
		printf("hour: %d\n", rtc->hour);
		printf("day: %d\n", rtc->day);
		printf("date: %d\n", rtc->date);
		printf("month: %d\n", rtc->month);
		printf("year: %d\n", rtc->year);
		printf("hour_format: %d\n", rtc->hour_format);
		*/
		
		
		//uart_putc(rtc->second); // send byte to serial line , you may not send it via uart,
		//uart_puts_P( "\r"); // LF	// I just want to mention how to use the data with this
		//_delay_ms(100);
	}
}

void display_write(void)
{
	unsigned char yr1, mn1, date1, dy1, hr1, min1, sec1, line1[16];

	reset_3w();
	wbyte_3w(0xBF);	/* clock burst */
	sec1 = rbyte_3w();
	min1 = rbyte_3w();
	hr1 = rbyte_3w();
	dy1 = rbyte_3w();
	date1 = rbyte_3w();
	mn1 = rbyte_3w();
	yr1 = rbyte_3w();
	reset_3w();
	
	printf("sec: %d\n", sec1);
	//printf("min: %d\n", min1);
	//printf("year: %d\n", yr1);
}
