#include <stdio.h>
#include <stdint.h>

int main() {
    uint8_t number = 254;
	uint8_t result;

	result = number + 23;
	// Check the result with printf
	printf("result: %d\n", result);

 	// What happened?!
 	// Overflow

	// Try again with different datatype
	uint16_t result_16 = number + 23;

	// Check the result with printf
	printf("result_16: %d\n", result_16);

	// What happened and why?
	// it fits fine into 16 bits

	//----------------------------------------
	int8_t number_negative = number;

	// Check the result with printf
	printf("number_negative: %d", number_negative);

	// What happened and why?
	// int8_t is signed
	
    return 0;
}
