#ifndef P_UTILS_H_INCLUDED
#define P_UTILS_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

uint8_t is_number(char *string);
void print_float(float number, uint8_t decimals);
void remove_trailing_nl(char *string);




#endif // P_UTILS_H_INCLUDED