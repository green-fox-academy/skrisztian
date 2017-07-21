#ifndef __LCD_LOGGER_H_INCLUDED
#define __LCD_LOGGER_H_INCLUDED

#include "lcd_log.h"
#include <stdint.h>

#define NONE	0
#define ERROR	1
#define USER	2
#define DEBUG	3

#define LOG_LEVEL	USER

void log_msg(uint8_t log_type, char* message);




#endif // __LCD_LOGGER_H_INCLUDED
