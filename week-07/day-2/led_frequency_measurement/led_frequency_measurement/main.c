/*
 * led_frequency_measurement.c
 *
 * Created: 2017-06-13 12:26:30
 * Author : Krisztian Stancz
 */ 

#include "freq_meas.h"
#include "UART_driver.h"
#include "p_utils.h"
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_DDR			DDRB
#define LED_DDR_POS		DDRB5
#define LED_PIN			PINB
#define LED_PIN_POS		PINB5
#define LED_PORT		PORTB
#define LED_PORT_POS	PORTB5

void system_init()
{
	LED_DDR |= 1 << LED_DDR_POS;
	freq_meas_init();
	UART_init();
	sei();
}

int main(void)
{

	// Don't forget to call the init function :)
	system_init();

	// Setting up STDIO input and output buffer
	// You don't have to understand this!
	//----- START OF STDIO IO BUFFER SETUP
	FILE UART_output = FDEV_SETUP_STREAM((void *)UART_send_character, NULL, _FDEV_SETUP_WRITE);
	stdout = &UART_output;
	FILE UART_input = FDEV_SETUP_STREAM(NULL, (void *)UART_get_character, _FDEV_SETUP_READ);
	stdin = &UART_input;
	//----- END OF STDIO IO BUFFER SETUP

	// Try printf
	printf("Startup...\r\n");
	const float pi = 3.14159265358979;
	
	printf("pi: ");
	print_float(pi, 5);
	printf("\n");

	// Infinite loop
	while (1) {
		// Generating an about 1Hz signal on the LED pin.
		// The printf call will also take some time, so this won't be exactly 1Hz.
		LED_PORT |= 1 << LED_PORT_POS;
		_delay_ms(500);
		LED_PORT &= ~(1 << LED_PORT_POS);
		_delay_ms(500);
		
		printf("freq: ");
		print_float(get_freq(), 3);
		printf(" Hz\n");
		
		// printf("%f Hz\n", get_freq());
	}
}