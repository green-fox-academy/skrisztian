/*
 * led_matrix.c
 *
 * Created: 2017-05-27 20:56:49
 * Author : Krisztian Stancz
 */ 

// GTM2088RGB - 8x8 common cathode (-)

// SN74HC595N
// 14 SER - PB0 - DATA_PIN
// 12 RCLK - PB1 - LATCH_PIN
// 11 SRCLK - PB2 - CLOCK_PIN

// Pin layout
// 1-8 BLUE
// 9-16 RED
// 28-21 GREEN
// 17-20 ROW 1-4
// 29-32 ROW 5-8
// LED (1,1): upper left
// bit order R->G->B->rows
// row 1-8 : PD0-PD7


#ifndef F_CPU
#define F_CPU 16000000
#endif // F_CPU

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define SHIFT_REG_PORT	PORTB
#define DDR_SHIFT_REG	DDRB
#define DATA_PIN	PORTB0
#define LATCH_PIN	PORTB1
#define CLOCK_PIN	PORTB2

uint8_t init(void)
{
	// Set all port D to output	
	DDRD = 0xff;
	
	// Turn off all port D
	PORTD = 0x00;
	
	// Set shift register port to output
	DDR_SHIFT_REG |= (1 << DATA_PIN);
	DDR_SHIFT_REG |= (1 << LATCH_PIN);
	DDR_SHIFT_REG |= (1 << CLOCK_PIN);
	
	// Set shift register pins to low
	SHIFT_REG_PORT &= ~(1 << DATA_PIN);
	SHIFT_REG_PORT &= ~(1 << LATCH_PIN);
	SHIFT_REG_PORT &= ~(1 << CLOCK_PIN);
	
	return 0;
}

uint8_t shift_out(uint8_t byte_out)
{
	
	// Set latch low while serial shift goes in
	SHIFT_REG_PORT &= ~(1 << LATCH_PIN);
	
	// For start, set clock to low
	SHIFT_REG_PORT &= ~(1 << CLOCK_PIN);

	// Send out one bit for each rising clock pulse
	// Set data bit
	for (int i = 0; i <= 7; i++) {
		if (byte_out & (1 << i)) {
			SHIFT_REG_PORT |= 1 << DATA_PIN;
		} else {
			SHIFT_REG_PORT &= ~(1 << DATA_PIN);
		}
		
		// Set clock pulse high
		SHIFT_REG_PORT |= 1 << CLOCK_PIN;
	
		// Set clock pulse low
		SHIFT_REG_PORT &= ~(1 << CLOCK_PIN);
		
	}
	
	// Set latch high to parallel write
	SHIFT_REG_PORT |= 1 << LATCH_PIN;
	
	// Set latch low
	SHIFT_REG_PORT &= ~(1 << LATCH_PIN);
	
	return 0;
}


int main(void)
{
    init();


		
	// Rows
		
	

	// Columns
	while (1) {
		for (int j = 0; j <= 7; j++) {
			PORTD |=  1 << j;
			for (int i = 7; i >= 0; i--) {
				int byte = 1 << i;
				shift_out(~byte);	// R
				shift_out(~byte);	// G
				shift_out(~byte);	// B 
				shift_out(~byte);	// row                                                       
				_delay_ms(100);
			}
			PORTD &=  ~(1 << j);
		}
	}
			
	
			


			
		


	return 0;
}

