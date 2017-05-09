#include <stdio.h>
#include <stdint.h>

int main() {

	// Write a program that prints the numbers from 1 to 100.
	// But for multiples of three print "Fizz" instead of the number
	// and for the multiples of five print "Buzz".
	// For numbers which are multiples of both three and five print "FizzBuzz".

	int8_t number;
    int8_t divisible;

	for (number=1; number<100; number++) {
        divisible = 0;
        if (number % 3 == 0) {
            printf("Fizz");
            divisible = 1;
        }
        if (number % 5 == 0) {
            printf("Buzz");
            divisible = 1;
        }
        if (divisible == 0) {
            printf("%d", number);
        }
        printf("\n");
	}

	return 0;
}