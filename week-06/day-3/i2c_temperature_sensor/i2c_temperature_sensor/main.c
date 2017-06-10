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
#include <string.h>

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

volatile uint8_t cntr =0;
volatile uint8_t measure = 0;
volatile uint8_t measure_cnt = 0;
volatile uint8_t measure_cnt_max = 0;

ISR(TIMER0_OVF_vect)
{
	// LED is on for about 0.2 sec
	// Increase counter
	if (cntr < 10) {
		cntr++;
		
	// Turn off led, reset counter
	// disable timer interrupt
	} else {
		cntr = 0;
		LED_PIN |= 1 << LED_PIN_POS;
		TIMSK0 &= ~(1 << TOIE0);
	}
}

ISR(TIMER1_COMPA_vect)
{
	// Increase counter
	if (measure_cnt < measure_cnt_max) {
		measure_cnt++;

	// Measure once
	} else {
		measure_cnt = 0;
		measure = 1;
	}
}

void timer_init(void)
{
	// *** Timer B0 for flashing the LED
	
	// Set prescaler to 1024
	TCCR0B |= (1 << CS02) | (1 << CS00);

	// Interrupt will be turned on from the blink_led function	
	
	// *** Timer B1 for polling temperature
	
	// Set prescaler to 1024
	TCCR1B |= (1<<CS10) | (1<<CS12);
	
	// Set CTC mode
	TCCR1B |= 1 << WGM12;
	
	// Initialize counter
	TCNT1 = 0;
	
	// Set compare value for 3 sec at 16 MHz / 1024
	OCR1A = 46874;
}


void system_init()
{
	// Init I2C
	TWI_init();

	// Init uart
	UART_init();
	
	// Init the timers
	timer_init();
	
	// Set LED to output
	LED_DDR |= 1 << LED_DDR_POS;
	
	// Enable global interrupts
	sei();
}

void blink_led(void)
{
	// Turn on led
	LED_PIN |= 1 << LED_PIN_POS;
	
	// Enable TC0 timer interrupt
	TIMSK0 |= 1 << TOIE0;
}

void disable_measurement(void)
{
	// Turn of timer interrupt
	TIMSK1 &= ~(1 << OCIE1A);
	
	// Re-initiate counters
	measure_cnt_max = 0;
	measure_cnt = 0;
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

	// Infinite loop
	while (1) {

		/* The PC can send commands to the MCU
		 * If we receive as a char
		 * '1' - send back a single measurement
		 * '2' - send back measurements every 3 seconds
		 * '3' - send back measurements every 30 seconds
		 * '0' - stop sending back continuous measurements
		 */
		
		// Check for command
		if (read_buffer) {
			
			// Return a single measurement
			if (char_buffer == '1') {
				read_buffer = 0;
				disable_measurement();
				measure = 1;

			// Return measurement every 3 seconds
			} else if (char_buffer == '2') {
				read_buffer = 0;
				disable_measurement();
				
				// Enable timer
				TIMSK1 |= (1 << OCIE1A);				
				
			// Return measurement every 30 seconds
			} else if (char_buffer == '3') {
				read_buffer = 0;
				disable_measurement();
				
				// Set the counter max value
				measure_cnt_max = 10;
				
				// Enable timer
				TIMSK1 |= (1 << OCIE1A);
			
			// Stop measurements
			} else {
				read_buffer = 0;
				disable_measurement();
			}
		}
		
		// Do a single measurement
		if (measure) {
			
			// Send measurement
			printf("%d\n", read_temperature(TC_ADDR));
			
			// Blink led once
			blink_led();
				
			// Reset measure flag
			measure = 0;
		}
	}
}
