/*
 * i2c_temperature_sensor.c
 *
 * Created: 2017-06-07 10:55:56
 * Author : Krisztian Stancz
 */ 

#include "TC74_driver.h"
#include "UART_driver.h"
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

#define LED_DDR			DDRB
#define LED_DDR_POS		DDRB5
#define LED_PIN			PINB
#define LED_PIN_POS		PINB5
#define LED_PORT		PORTB
#define LED_PORT_POS	PORTB5

void system_init()
{
	// Call the TWI driver init function
	TWI_init();

	//Init the uart
	UART_init();
	
	// Set LED out
	LED_DDR |= 1 << LED_DDR_POS;
}






int main(void)
{

	// Don't forget to call the init function :)
	system_init();

	// Setting up STDIO input and output buffer
	// You don't have to understand this!
	//----- START OF STDIO IO BUFFER SETUP
	FILE UART_output = FDEV_SETUP_STREAM(UART_send_character, NULL, _FDEV_SETUP_WRITE);
	stdout = &UART_output;
	FILE UART_input = FDEV_SETUP_STREAM(NULL, UART_get_character, _FDEV_SETUP_READ);
	stdin = &UART_input;
	//----- END OF STDIO IO BUFFER SETUP

	// Try printf
	printf("Startup...\r\n");

	sei();

	// Infinite loop
	while (1) {
		//TODO
		//Write the temperature frequently.
		printf("temperature: %d\n", read_temperature(TC_ADDR));
		
		_delay_ms(500);

		//TODO
		//Advanced: Don't use delay, use timer.

		//TODO
		//Blink the led to make sure the code is running
		LED_PIN |= 1 << LED_PIN_POS;

	}
}
