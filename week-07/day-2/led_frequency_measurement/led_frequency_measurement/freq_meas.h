#ifndef _FREQ_MEAS_H
#define _FREQ_MEAS_H

#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define NO_CLOCK		0x00
#define PS_CLK_1		0x01
#define PS_CLK_8		0x02
#define PS_CLK_64		0x03
#define PS_CLK_256		0x04
#define PS_CLK_1024		0x05
#define EXT_CLK_FALL	0x06
#define EXT_CLK_RISE	0x07

#define COUNTER_MAX 65535

volatile uint16_t capture[2];
volatile uint8_t capture_index;
volatile uint16_t overflow_count;
uint16_t freq_divider;

void freq_meas_init(void);
float get_freq(void);
uint16_t read_ICR1(void);

#endif // _FREQ_MEAS_H


