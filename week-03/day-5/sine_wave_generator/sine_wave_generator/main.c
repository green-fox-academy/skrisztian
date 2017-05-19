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
#include <math.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/delay.h>

#define MAX_VOLTAGE		4.096
#define MIN_VOLTAGE		0

enum CalcType {CALCULATE, LOOKUP};


uint8_t sinx_254[] =
	{0x7f, 0x82, 0x85, 0x88, 0x8b, 0x8f, 0x92, 0x95, 0x98, 0x9b, 0x9e, 0xa1, 0xa4, 0xa7, 0xaa, 0xad,
	 0xb0, 0xb2, 0xb5, 0xb8, 0xbb, 0xbe, 0xc0, 0xc3, 0xc6, 0xc8, 0xcb, 0xcd, 0xd0, 0xd2, 0xd4, 0xd7,
	 0xd9, 0xdb, 0xdd, 0xdf, 0xe1, 0xe3, 0xe5, 0xe7, 0xe9, 0xea, 0xec, 0xee, 0xef, 0xf0, 0xf2, 0xf3,
	 0xf4, 0xf5, 0xf7, 0xf8, 0xf9, 0xf9, 0xfa, 0xfb, 0xfc, 0xfc, 0xfd, 0xfd, 0xfd, 0xfe, 0xfe, 0xfe,
	 0xfe, 0xfe, 0xfe, 0xfe, 0xfd, 0xfd, 0xfd, 0xfc, 0xfc, 0xfb, 0xfa, 0xf9, 0xf9, 0xf8, 0xf7, 0xf5,
	 0xf4, 0xf3, 0xf2, 0xf0, 0xef, 0xee, 0xec, 0xea, 0xe9, 0xe7, 0xe5, 0xe3, 0xe1, 0xdf, 0xdd, 0xdb,
	 0xd9, 0xd7, 0xd4, 0xd2, 0xd0, 0xcd, 0xcb, 0xc8, 0xc6, 0xc3, 0xc0, 0xbe, 0xbb, 0xb8, 0xb5, 0xb2,
	 0xb0, 0xad, 0xaa, 0xa7, 0xa4, 0xa1, 0x9e, 0x9b, 0x98, 0x95, 0x92, 0x8f, 0x8b, 0x88, 0x85, 0x82,
	 0x7f, 0x7c, 0x79, 0x76, 0x73, 0x6f, 0x6c, 0x69, 0x66, 0x63, 0x60, 0x5d, 0x5a, 0x57, 0x54, 0x51,
	 0x4e, 0x4c, 0x49, 0x46, 0x43, 0x40, 0x3e, 0x3b, 0x38, 0x36, 0x33, 0x31, 0x2e, 0x2c, 0x2a, 0x27,
	 0x25, 0x23, 0x21, 0x1f, 0x1d, 0x1b, 0x19, 0x17, 0x15, 0x14, 0x12, 0x10, 0x0f, 0x0e, 0x0c, 0x0b,
	 0x0a, 0x09, 0x07, 0x06, 0x05, 0x05, 0x04, 0x03, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 
	 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x03, 0x04, 0x05, 0x05, 0x06, 0x07, 0x09,
	 0x0a, 0x0b, 0x0c, 0x0e, 0x0f, 0x10, 0x12, 0x14, 0x15, 0x17, 0x19, 0x1b, 0x1d, 0x1f, 0x21, 0x23,
	 0x25, 0x27, 0x2a, 0x2c, 0x2e, 0x31, 0x33, 0x36, 0x38, 0x3b, 0x3e, 0x40, 0x43, 0x46, 0x49, 0x4c,
	 0x4e, 0x51, 0x54, 0x57, 0x5a, 0x5d, 0x60, 0x63, 0x66, 0x69, 0x6c, 0x6f, 0x73, 0x76, 0x79, 0x7c};

uint16_t sinx_4000[] =
	{2000, 2049, 2098, 2147, 2196, 2245, 2293, 2342, 2390, 2438, 2486, 2533, 2581, 2627, 2674, 2720,
	 2765, 2810, 2855, 2899, 2943, 2986, 3028, 3070, 3111, 3152, 3191, 3230, 3269, 3306, 3343, 3379,
	 3414, 3448, 3482, 3514, 3546, 3577, 3606, 3635, 3663, 3690, 3715, 3740, 3764, 3786, 3808, 3828,
	 3848, 3866, 3883, 3899, 3914, 3928, 3940, 3951, 3962, 3971, 3978, 3985, 3990, 3995, 3998, 3999,
	 4000, 3999, 3998, 3995, 3990, 3985, 3978, 3971, 3962, 3951, 3940, 3928, 3914, 3899, 3883, 3866,
	 3848, 3828, 3808, 3786, 3764, 3740, 3715, 3690, 3663, 3635, 3606, 3577, 3546, 3514, 3482, 3448,
	 3414, 3379, 3343, 3306, 3269, 3230, 3191, 3152, 3111, 3070, 3028, 2986, 2943, 2899, 2855, 2810,
	 2765, 2720, 2674, 2627, 2581, 2533, 2486, 2438, 2390, 2342, 2293, 2245, 2196, 2147, 2098, 2049,
	 2000, 1951, 1902, 1853, 1804, 1755, 1707, 1658, 1610, 1562, 1514, 1467, 1419, 1373, 1326, 1280,
	 1235, 1190, 1145, 1101, 1057, 1014, 972, 930, 889, 848, 809, 770, 731, 694, 657, 621, 586, 552,
	 518, 486, 454, 423, 394, 365, 337, 310, 285, 260, 236, 214, 192, 172, 152, 134, 117, 101, 86,
	 72, 60, 49, 38, 29, 22, 15, 10, 5, 2, 1, 0, 1, 2, 5, 10, 15, 22, 29, 38, 49, 60, 72, 86, 101,
	 117, 134, 152, 172, 192, 214, 236, 260, 285, 310, 337, 365, 394, 423, 454, 486, 518, 552, 586,
	 621, 657, 694, 731, 770, 809, 848, 889, 930, 972, 1014, 1057, 1101, 1145, 1190, 1235, 1280,
	 1326, 1373, 1419, 1467, 1514, 1562, 1610, 1658, 1707, 1755, 1804, 1853, 1902, 1951};


MCP4821_Data_t DAC_data;

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
	// ADC_Init();
	
	// Call the UART driver init function
	// Keep in mind that the UART will run at 115200 baud/sec
	// UART_Init();

	// Enable interrupts globally, UART uses interrupts
	// sei();
	
	return 0;
}

int setOutputVoltage(float voltage) {
	if (voltage >= 4.096) {
		DAC_data.data = 0xFFF;
	} else if (voltage <= 0) {
		DAC_data.data = 0x000;
	} else {
		DAC_data.data = (uint16_t) (voltage * 1000);
	}
	
	// Send data to DAC
	MCP4821_SendData(&DAC_data);
	
	return 0;
}

int main(void) {
	
	// Don't forget to call the init function :)
	SystemInit();

	/*
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
	*/
	
	// Variable declarations
	// char voltage_string[25];
	uint16_t adc_data;
	float peak_voltage = 2;
	float x;
	float y;
	float pi = M_PI;
	
	uint16_t sample_size = 62;
	uint8_t counter = 0;
		
	enum CalcType type = CALCULATE;
		
	while (1) {
		
		if (type == CALCULATE) {
			for (int i = 0; i<sample_size; i++) {
				x = i * (2 * pi / sample_size);
				y = sin(x) * peak_voltage + peak_voltage;

				DAC_data.data = (uint16_t) (y * 1000);
				MCP4821_SendData(&DAC_data);
			}
			
		} else if (type == LOOKUP) {
			
				DAC_data.data = sinx_4000[counter];
				MCP4821_SendData(&DAC_data);
				counter++;
			
		}
	}
}

