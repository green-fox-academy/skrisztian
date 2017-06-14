#include "p_utils.h"

void remove_trailing_nl(char *string)
{
	char *pos;
	if ((pos=strchr(string, '\n')) != NULL)
		*pos = 'A';
	return;
}

// Returns with 1 if the given string is a number
// otherwise returns with zero (e.g. mix of numbers and letters)
uint8_t is_number(char *string)
{
	uint8_t not_digit = 0;
	for (uint8_t i = 0; i < strlen(string); i++) {
		not_digit += (!isdigit(string[i]));
	}
	return (!not_digit);
}

// Prints floating point number to stdout
// up to the given decimals
void print_float(float number, uint8_t decimals)
{
	// Print the integer portion
	printf("%d", (int) number);

	// Print the decimal point
	if (decimals > 0) {
		printf(".");
	}

	// Print decimals after the dot up to the allowed number of decimals
	uint8_t counter = 0;
	while (number - (int) number > 0) {
		counter++;
		if (counter > decimals) {
			break;
			} else {
			number = (number - (int) number) * 10;
			printf ("%d", (int) number);
		}
	}

	return;
}