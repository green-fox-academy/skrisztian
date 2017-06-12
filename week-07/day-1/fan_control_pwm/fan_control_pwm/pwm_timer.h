#ifndef PWM_TIMER_H_INCLUDED
#define PWM_TIMER_H_INCLUDED

#include <stdint.h>

void init_fast_pwm(void);
void set_duty_cycle_a(uint8_t duty_cycle);
void set_duty_cycle_p(uint8_t duty_cycle);

#define PIN_LED			PIND6
#define PIN_PORT_LED	PIND
#define DDR_LED			DDRD
#define DDR_LED_PIN		DDRD6




#endif // PWM_TIMER_H_INCLUDED