#include "pwm_timer.h"

void init_fast_pwm(void)
{
	// set PB3 as output
	PWM_DDR |= 1 << PWM_DDR_PIN;
	
	// Set fast PWM output on OC2A, clear at bottom (non-inverting mode)
	// TOV set at MAX = OCR2A
	TCCR2A |=  (1 << WGM21) | (1 << WGM20) | (1 << COM2A1);
	TCCR2B |=  (1 << CS20) | (1 << CS22);
	
	return;
}

void set_duty_cycle_p(uint8_t duty_cycle)
{
	OCR2A = (duty_cycle * 255) / 100;
	
	return;
}

void set_duty_cycle_a(uint8_t duty_cycle)
{
	OCR2A = duty_cycle;
	
	return;
}