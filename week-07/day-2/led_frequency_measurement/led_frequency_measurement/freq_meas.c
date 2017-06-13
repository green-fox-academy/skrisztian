#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "freq_meas.h"

// Input capture interrupt
ISR(TIMER1_CAPT_vect)
{
	// Save ICR1 value
	capture[capture_index] = read_ICR1();
	capture_index = !capture_index;
	overflow_count = 0;
}

// Timer overflow interrupt
ISR(TIMER1_OVF_vect)
{
	overflow_count++;
}

void freq_meas_init()
{
	/**************
	 * TC1 CONFIG *
	 **************/
	capture[0] = 0;
	capture[1] = 0;
	capture_index = 0;
	overflow_count = 0;
	
	uint8_t clock_prescaler = PS_CLK_1;
	
	// Enable input capture interrupt and overflow interrupt
	TIMSK1 |= (1 << ICIE1) | (1 << TOIE1);
	
	// Set rising edge for input capture, set pre-scaler
	TCCR1B |= (1 << ICES1) | clock_prescaler;
	
	switch (clock_prescaler) {
	case PS_CLK_1:
		freq_divider = 1;
		break;
	case PS_CLK_8:
		freq_divider = 8;
		break;
	case PS_CLK_64:
		freq_divider = 64;
		break;
	case PS_CLK_256:
		freq_divider = 256;
		break;
	case PS_CLK_1024:
		freq_divider = 1024;
		break;
	default:
		freq_divider = 1;
	}
}

// TODO:
// Write this function. It returns the measured frequency in Hz
float get_freq()
{
	float step = overflow_count * COUNTER_MAX - capture[0] + capture[1];
	float t_period = (step / F_CPU) * freq_divider;
	
	return 1 / t_period;
}

uint16_t read_ICR1(void)
{
	unsigned char sreg;
	uint16_t i;
	
	// Save global interrupt flag
	sreg = SREG;
	
	// Disable interrupts
	cli();
	
	// Read ICR1 into i
	i = ICR1;
	
	// Restore global interrupt flag */
	SREG = sreg;

	return i;
}