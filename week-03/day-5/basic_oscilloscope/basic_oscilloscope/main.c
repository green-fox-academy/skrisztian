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


/*
Set up the UART interface of the ATmega168PB
Set up the ADC peripheral of the ATmega168PB
Send data to the PC via serial port
Send the data as a number (not as a string!) in tha main while loop
Open the Data Visualizer Extension
Set up the serial port
Set up an Oscilloscope object with the serial port as the input
*/

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
		adc_data /= 4;
		
		UART_SendCharacter(adc_data);
		//UART_SendCharacter('v');
    }
}
