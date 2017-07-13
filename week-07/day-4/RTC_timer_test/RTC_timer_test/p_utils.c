#include "p_utils.h"

void remove_trailing_nl(char *string)
{
	char *pos;
	if ((pos=strchr(string, '\n')) != NULL)
		*pos = 'A';
	return;
}

// Returns with 1 if the given string is a number (integer or float)
// otherwise returns with 0 (i.e. string is a mix of numbers and letters).
// Also allows leading plus or minus signs and one decimal point
uint8_t is_number(char *string)
{
	uint8_t not_digit = 0;
	uint8_t point_count = 0;

	for (uint8_t i = 0; i < strlen(string); i++) {

		// Skip leading + or - sign
		if ((i == 0) && ((string[i] == '-') || (string[i] == '+')))
			i++;

		// Skip first (decimal) point
		if ((string[i] == '.') && (point_count == 0)) {
			point_count++;
			i++;
		}

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
	
	// Print a zero after the decimal if number is an integer
	if (number - (int) number == 0) {
		printf("0");
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