/*
 * single_led_with_pwm.c
 *
 * Created: 2017-06-12 08:02:32
 * Author : Krisztian Stancz
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
# include "pwm_timer.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>




#include <avr/io.h>

void init_led(void)
{
	// Set LED as output
	DDR_LED |= 1 << DDR_LED_PIN;
	
	return;
}



int main(void)
{
	// Init functions
	init_led();
	init_fast_pwm();
	
    PIN_PORT_LED |= 1 << PIN_LED;
	set_duty_cycle(100);
	
	while(1) {
		for (int i = 0; i <= 100; i++) {
			set_duty_cycle(i);
			_delay_ms(10);
		}
	
		for (int i = 100; i >= 0; i--) {
			set_duty_cycle(i);
			_delay_ms(10);
		}
	}

	return 0;
}

