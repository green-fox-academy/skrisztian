#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "pwm_timer.h"

void init_fast_pwm(void)
{
	// Set fast PWM output on OC0A, clear at bottom (non-inverting mode)
	// TOV set at MAX = OCR0A
	TCCR0A |=  (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
	TCCR0B |=  (1 << CS00);
	
	return;
}

void set_duty_cycle(uint8_t duty_cycle)
{
	OCR0A = duty_cycle * (255 / 100);
	
	return;
}
