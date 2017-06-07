#include <avr/io.h>
#include <string.h>
#include <stdint.h>
#include "TC74_driver.h"

void TWI_init(void)
{
	// Set Prescaler to 4
	TWSR &= ~(1 << TWPS1);
	TWSR |= 1 << TWPS0;

	// Set SCL frequency = 16000000 / (16 + 2 * 48 * 4) = 40.000 Khz
	//So set the correct register to 0x30
	TWBR = 0x30;

	//Enable TWI
	TWCR |= 1 << TWEN;
}

uint8_t TWI_status(void)
{
	// Read top 5 bits of TWI Status Register
	uint8_t status = TWSR;

	// Mask bottom 3 bits
	status &= ~(0x07);
	
	return status;	
}

void TWI_start(void)
{
	//Send start signal
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// Wait for TWINT Flag set. This indicates that
	//the START condition has been transmitted.
	while (!(TWCR & (1<<TWINT)));
}

void TWI_stop(void)
{
	//Send stop signal
	TWCR = (1<<TWSTO) | (1<<TWEN); 
}

uint8_t TWI_read_ack(void)
{
	//Read byte with ACK
	TWCR |= (1 << TWINT) | (1 << TWEA);
	
	//Wait for TWINT Flag set. This indicates that
	//the DATA has been received.
	while (!(TWCR & (1<<TWINT)));
		
	// Return TWI data register content
	return TWDR;
}


uint8_t TWI_read_nack(void)
{
	//Read byte with NACK
	TWCR |= 1 << TWINT;

	//Wait for TWINT Flag set. This indicates that
	//the DATA has been received.
	while (!(TWCR & (1<<TWINT)));
	
	// Do not send ACK, just read the TWI data register
	return TWDR;
}

void TWI_write(uint8_t u8data)
{
	//Load DATA into TWDR Register. 
	TWDR = u8data;
	
	// Clear TWINT in TWCR to start transmission of data.
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	//Wait for TWINT Flag set. This indicates that
	//the DATA has been transmitted, and ACK/
	//NACK has been received.
	while (!(TWCR & (1<<TWINT)));
}

//TODO
//Write a function that communicates with the TC74A0.
//The function need to take the address of the ic as a parameter.
//datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/21462D.pdf
//And it returns with the temperature.
uint8_t read_temperature(uint8_t device_address)
{
	// Set start condition
	TWI_start();
	
	// Send address with WR (0) bit
	TWI_write(device_address << 1);
	
	// Send command with temp register
	TWI_write(0x00);
	
	// Set start condition before read
	TWI_start();
	
	// Send address with RD (0) bit
	TWI_write((device_address << 1) | 0x01);
	
	// Read temp data sent by sensor
	uint8_t temperature = TWI_read_nack();
	
	// Set Stop condition
	TWI_stop();
	
	// Negative numbers are stored as 2 complementers
	if (temperature > 127) {
		temperature--;
		temperature = ~temperature;
	}
	
	return temperature;
}

//Calculate the average of the last 16 data, and returns with that.
float avg_temperature(uint8_t device_address)
{
	uint16_t total = 0;
	for (uint8_t i = 0; i < 16; i++) {
		total += read_temperature(device_address);
	}
	return ((float) total / 16.0);
}

//TODO Advanced+:
//Select the outstanding (false data) before average it.
//These data isn't needed, it messes up your data, get rid of it.