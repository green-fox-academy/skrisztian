/*
 * presentation_timer.c
 *
 * Created: 2017-05-26 14:15:43
 * Author : Krisztian Stancz
 *
 * Testing timer interrups
 */ 

// 100 ms blink

#include <avr/io.h>
#include <avr/interrupt.h>


ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	PORTD ^= (1 << PORTD4);
	TCNT1 = 63974;   // for 1 sec at 16 MHz
}

int timer_init(void)
{
	DDRD = 1 << DDRD4;     //Configure the PORTD4 as output
	
	TCNT1 = 63974;   // for 1 sec at 16 MHz

	TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);;  // Timer mode with 1024 prescaler
	TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
}


int main(void)
{
	timer_init();
    
    while (1);

}

