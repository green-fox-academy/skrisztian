/*
 * DC_function_generator.c
 *
 * Created: 2017-05-18 16:54:50
 * Author : Krisztian Stancz
 */ 

#include "MCP4821_driver.h"
#include "UART_driver.h"
#include "ADC_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/delay.h>

#define LED_DDR			DDRB
#define LED_DDR_POS		DDRB5
#define LED_PIN			PINB
#define LED_PIN_POS		PINB5
#define LED_PORT		PORTB
#define LED_PORT_POS	PORTB5

#define MAX_VOLTAGE		4.096
#define MIN_VOLTAGE		0

MCP4821_Data_t DAC_data;

/*
The DC function generator should be controlled via UART
It receives a floating point number in Volts
This received voltage then appears on the output of the DAC
if the user gives too low voltage put out the minimal voltage
if the user gives too high voltage put out the maximal voltage
Output voltage minimum is 0V
Output voltage maximum is 4.096V
*/

int SystemInit() {

	// Call the DAC driver init function
	MCP4821_Init();
	
	// Set main parameters for DAC data
	DAC_data.start_zero = 0;
	DAC_data.dont_care = 0;
	DAC_data.gain = 0;
	DAC_data.shutdown = 1;
	DAC_data.data = 0x0;
	
	// Initialize ADC
	ADC_Init();
	
	// Call the UART driver init function
	// Keep in mind that the UART will run at 115200 baud/sec
	UART_Init();

	// Enable interrupts globally, UART uses interrupts
	sei();
	
	return 0;
}

int main(void) {
	
	// Don't forget to call the init function :)
	SystemInit();

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
	
	// Variable declarations
	char voltage_string[25];
	float voltage;
	uint16_t adc_data;
	
    while (1) {
		
		// Read user input
		puts("Enter the required voltage between 0 and 4.096");
		gets(voltage_string);
		
		// Convert string to float
		voltage = atof(voltage_string);
		
		// Set the voltage level on DAC
		if (voltage >= 4.096) {
			DAC_data.data = 0xFFF;
		} else if (voltage < 0) {
			DAC_data.data = 0x000;
		} else {
			DAC_data.data = (uint16_t) (voltage * 1000);	
		}
		
		// Send data to DAC
		MCP4821_SendData(&DAC_data);

		// Feedback to user
		printf("* Required voltage: ");
		puts(voltage_string);
		printf("* Output voltage is set to 0x%x\r\n", DAC_data.data);		
		
		adc_data = ADC_Read();
		printf("* ADC data: %d\r\n", adc_data);
		
	    }
}

