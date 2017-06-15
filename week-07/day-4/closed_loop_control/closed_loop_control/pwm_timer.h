#ifndef PWM_TIMER_H_INCLUDED
#define PWM_TIMER_H_INCLUDED

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void init_fast_pwm(void);
void set_duty_cycle_a(uint8_t duty_cycle);
void set_duty_cycle_p(uint8_t duty_cycle);

#define PWM_DDR		DDRB
#define PWM_DDR_PIN	DDRB3				




#endif // PWM_TIMER_H_INCLUDED