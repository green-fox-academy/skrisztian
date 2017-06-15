#include "freq_meas.h"

// Input capture interrupt
ISR(TIMER1_CAPT_vect)
{
	previous_v = last_v;
	last_v = ICR1;
	overflow_v = overflow_count;
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
	previous_v = 0;
	overflow_count = 0;
	
	uint16_t clock_prescaler = PS_CLK_64;
	
	// Enable input capture interrupt and overflow interrupt
	TIMSK1 |= (1 << ICIE1) | (1 << TOIE1);
	
	// Set rising edge for input capture, set pre-scaler
	TCCR1B |= (1 << ICES1) | clock_prescaler; // (1 << CS10) | (1 << CS11)
	
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
float get_freq(void)
{
	cli();
	previous_c = previous_v;
	last_c = last_v;
	overflow_c = overflow_v;
	sei();
	
	uint32_t step = (uint32_t) overflow_v * (uint32_t) COUNTER_MAX - (uint32_t) previous_v + (uint32_t) last_v;
	float t_period = ((float) step / F_CPU) * freq_divider;
	float freq = 1 / t_period;
	
	if (isnan(freq) || isinf(freq))
		return -1;
	else
		return freq;
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