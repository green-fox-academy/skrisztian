// Multiple inclusion guard
#ifndef _TC74_DRIVER_H
#define _TC74_DRIVER_H

#include <stdint.h>

#define TC_WRITE	0
#define TC_READ		1
//TODO

//Define the TC74A0 address
#define TC_ADDR	0b01001000

void TWI_init(void);
uint8_t TWI_status(void);
void TWI_start(void);
void TWI_write(uint8_t u8data);
void TWI_stop(void);
uint8_t TWI_read_nack(void);
uint8_t TWI_read_ack(void);
int8_t read_temperature(uint8_t device_address);

#endif // _TC74_DRIVER_H