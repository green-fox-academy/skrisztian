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
	uint8_t control_type = CLOSED_LOOP_PI;
	float err = 0;
	float P = 0;
	float I = 0;
	float ref = 0;
	float process_variable = 0;
	float ctrler_out = 0;
	float integral = 0;

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
	
	// Try print_float
	const float pi = 3.14159265359;
	print_float(pi, 5);
	printf("\n");

	// Infinite loop
	while (1) {

		// Set reference RPM with potmeter		
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
			
		case CLOSED_LOOP_P:

			/* Pseudo-code
			 * err = ref - process_variable;
			 * ctrler_out = P * err;
			 * 
			 * if (ctrler_out < ctrler_out_min)
			 * ctrler_out = ctrler_out_min;
			 * else if (ctrler_out > ctrler_out_max)
			 * ctrler_out = ctrler_out_max;
			 */
			
			P = 60;
			
			// Flash LED so we see it's working
			LED_PIN |= 1 << LED_PIN_POS;
			_delay_ms(10);

			// Calculate error signal
			process_variable = get_rpm();
			err = ref - process_variable;
			
			// Set output control signal
			ctrler_out = P * err;
			
			// Set saturation values
			if (ctrler_out < 0.0)
				ctrler_out = 0;
			else if (ctrler_out > 255.0)
				ctrler_out = 255;
			
			// Set PWM duty cycle
			set_duty_cycle_a((uint8_t) ctrler_out);
		
			break; // END CLOSED_LOOP_P
			
		case CLOSED_LOOP_PI:
			
			/* Pseudo code
			 * err = ref - process_variable;
			 * integral = integral + err;
			 * ctrler_out = P * err + I * integral;
			 *
			 * if (ctrler_out < ctrler_out_min) {
			 *	ctrler_out = ctrler_out_min;
			 *	integral = integral - err;
			 * }
			 * else if (ctrler_out > ctrler_out_max) {
			 * 	ctrler_out = ctrler_out_max;
			 *	integral = integral - err;
			 * }
			 */

			P = 0.1;
			I = 0.01;
			
			// Flash LED so we see it's working
			LED_PIN |= 1 << LED_PIN_POS;
			_delay_ms(10);

			// Calculate error signal
			process_variable = get_rpm();
			err = ref - process_variable;
			integral = integral + err;
			
			// Set output control signal
			ctrler_out = P * err + I * integral;
			
			// Set saturation values
			if (ctrler_out < 0.0) {
				ctrler_out = 0;
				integral = integral - err;
			} else if (ctrler_out > 255.0) {
				ctrler_out = 255;
				integral = integral - err;
			}
			
			// Set PWM duty cycle
			set_duty_cycle_a((uint8_t) ctrler_out);
			
			break; // END CLOSED_LOOP_PI

		} // END switch
		
		// Print data on UART
		// printf("P: ");
		// print_float(P, 3);
		// printf("\tI: ");
		// print_float(I, 3);
		printf("Ref: ");
		print_float(ref, 3);
		// printf("\terr: ");
		// print_float(err, 3);
		// printf("\tintegral: ");
		// print_float(integral, 3);
		// printf("\tPWM: %d\t", (uint8_t) ctrler_out);
		printf("\t");
		print_float(process_variable, 3);
		printf(" RPM\n");
		
	} // END while
}

