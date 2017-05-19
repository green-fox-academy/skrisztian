/*
 * basic_oscilloscope.c
 *
 * Created: 2017-05-18 14:44:18
 * Author : Krisztian Stancz
 */ 

#include "ADC_driver.h"
#include "UART_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/delay.h>

int SystemInit(void) {
	ADC_Init();
	UART_Init();
	sei();
	
	return 0;
}

int main(void) {

	// Don't forget to call the init function :)
	SystemInit();

	uint16_t adc_data;

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

	while (1) {
		
		// Read input voltage
		adc_data = ADC_Read();
		
		// This scales up to 1023
		// the other side can only receive up to 255

		// Divide by 4
		adc_data = adc_data >> 2;
		
		UART_SendCharacter(adc_data);
    }
}
