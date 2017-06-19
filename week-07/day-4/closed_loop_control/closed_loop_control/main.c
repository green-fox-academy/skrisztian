/*
 * closed_loop_control.c
 *
 * Created: 2017-06-15 12:00:19
 * Author : Krisztian Stancz
 */ 

#include <avr/io.h>
#include "freq_meas.h"
#include "UART_driver.h"
#include "AC_driver.h"
#include "p_utils.h"
#include "pwm_timer.h"
#include "ADC_driver.h"
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

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

#define OPEN_LOOP		0
#define CLOSED_LOOP_P	1
#define CLOSED_LOOP_PI	2
#define CLOSED_LOOP_PID	3

#define MAX_RPM			4300

void system_init()
{
	LED_DDR |= 1 << LED_DDR_POS;
	ADC_Init();
	init_fast_pwm();
	AC_driver_init();
	freq_meas_init();
	UART_init();
	
	sei();
}

int main(void)
{
	uint16_t measure_adc;
	uint8_t control_type = CLOSED_LOOP_PID;
	float err = 0;
	float P = 0;
	float I = 0;
	float D = 0;
	float ref = 0;
	float process_variable = 0;
	float ctrler_out = 0;
	float integral = 0;
	float dt = 0;
	float previous_err = 0;
	float derivative = 0;
	char buffer[100];
	uint8_t debug = 0;
	uint8_t ref_type = 0;
	
	P = 0.10;
	I = 0.001;
	D = 0.02;
	dt = 30;	// ms
	

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

	if (debug) {
		// Try printf
		printf("Startup...\r\n");
	
		// Try print_float
		const float pi = 3.14159265359;
		print_float(pi, 5);
		printf("\n");
	}

	// Infinite loop
	while (1) {

		// Set reference RPM with potmeter		
		if (ref_type == 0)
			ref = (MAX_RPM / 1023.0) * (float) ADC_read_avg(10);

		// Controller
		switch (control_type) {
		case OPEN_LOOP:
			
			// Flash LED so we see it's working
			LED_PIN |= 1 << LED_PIN_POS;
			_delay_ms(500);

			// Measure ADC from the potmeter
			measure_adc = ADC_read_avg(10) >> 2;
			
			// Set PWM duty cycle
			set_duty_cycle_a(measure_adc);

			// Print data on UART
			printf("PWM: %d\t", measure_adc);
			print_float(get_freq(), 3);
			printf(" Hz\t");
			print_float(get_rpm(), 3);
			printf(" RPM\n");
			
			break; // END OPEN_LOOP	
		
		case CLOSED_LOOP_PID:
			
			// Flash LED so we see it's working
			LED_PIN |= 1 << LED_PIN_POS;
			_delay_ms(dt);

			// Calculate error signal
			process_variable = get_rpm();
			err = ref - process_variable;
			integral = integral + err * dt;
			derivative = (err - previous_err) / dt;
			previous_err = err;
			
			// Set output control signal
			ctrler_out = P * err + I * integral + D * derivative;
			
			// Set saturation values
			if (ctrler_out < 0.0) {
				ctrler_out = 0;
				integral = integral - err * dt;
			} else if (ctrler_out > 255.0) {
				ctrler_out = 255;
				integral = integral - err * dt;
			}
			
			// Set PWM duty cycle
			set_duty_cycle_a((uint8_t) ctrler_out);
			
			break; // END CLOSED_LOOP_PID

		} // END switch
		
		// Print debug data on UART
		if (debug) {
			printf("P: ");
			print_float(P, 3);
			printf("\tI: ");
			print_float(I, 3);
			printf("\tD: ");
			print_float(D, 3);
			printf("\tRef: ");
			print_float(ref, 3);
			printf("\terr: ");
			print_float(err, 3);
			printf("\tintegral: ");
			print_float(integral, 3);
			printf("\tderivative: ");
			print_float(derivative, 3);
			printf("\tPWM: %d\t\t", (uint8_t) ctrler_out);
			print_float(process_variable, 3);
			printf(" RPM\n");
		} else {
			// Print charting data on UART
			// print_float(ref, 3);
			// printf(" ");
			// print_float(process_variable, 3);
			// printf("\n");
			printf("%d %d\n", (uint16_t) ref, (uint16_t) process_variable);
		}
		
		
		// Read data from UART
		if (!UART_is_buffer_empty()) {
			gets(buffer);
			char *c;
			char command;
			c = strtok(buffer, " =");
			command = c[0];
			c = strtok(NULL, " =");
			
			switch (command) {
			case 'p':
				P = (float) atof(c);
				break;
			case 'i':
				I = (float) atof(c);
				break;
			case 'd':
				D = (float) atof(c);
				break;
			case 't':
				ref_type = (uint8_t) atoi(c);
				break;
			case 'r':
				ref = (float) atof(c);
				break;	
			}

		}
		
		
	} // END while
}

