/*
 * potmeter_timer_set.c
 *
 * Created: 2017-07-07 14:41:29
 * Author : Krisztian Stancz
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "ADC_driver.h"
#include "UART_driver.h"

#ifndef F_CPU
#define F_CPU 16000000
#endif // F_CPU


#define BUTTON_PIN				PINB7
#define BUTTON_PIN_REG 			PINB
#define BUTTON_PIN_INT			PCINT7
#define BUTTON_PIN_INT_GROUP	PCIE0

#define ON		1
#define OFF		0

volatile uint8_t state = OFF;
uint8_t hours;
uint8_t minutes;


init_pins(void)
{
	// Inputs
	// PD2, PD6, PD7, PB7, PC4, PC5
	DDRD = 0;
	DDRB = 0;
	DDRC = 0;

	// Outputs
	DDRD |= (1 << DDRD6) | (1 << DDRD7);
	DDRB |= (1 << DDRB5);
	
	return;
}


// Start button settings
uint8_t button_init(void)
{
	// Enable interrupts on button pin
	PCMSK0 |= 1 << BUTTON_PIN_INT;

	// Enable interrupts on button pin group
	PCICR |=  1 << BUTTON_PIN_INT_GROUP;

	return 0;
}

init(void)
{
	init_pins();
	ADC_init();
	button_init();
	UART_init();
	
	sei();
}

ISR(PCINT0_vect)
{
	// Start/reset on button press (0->1 transition)
	if (BUTTON_PIN_REG & (1 << BUTTON_PIN)) {
		state = ON;
		PORTB |= 1 << PORTB5;
	}
}

void set_time(void)
{
	if (state == OFF) {
		hours = (uint8_t) (ADC_read(ADC_CH_HOUR) / 93);
		if (hours > 11)
			hours = 11;
		
		minutes = (uint8_t) (ADC_read(ADC_CH_MIN) / 17);
		if (minutes > 59)
			minutes = 59;
	}
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
	
	printf("Startup...\n");
		
	
    /* Replace with your application code */
    while (1) 
    {
		set_time();
			
		printf("h: %d\tm: %d\n", hours, minutes);
			
			
		
		
		
    }
}

