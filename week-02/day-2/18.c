#include <stdio.h>
#include <stdint.h>

int main() {
	uint16_t number = 254;

	// if number can't be represented with 8bits
	// print 'longer than 8bits' otherwise print 'ok'
    uint8_t compare = number;

    printf("number: %d\n", number);
    printf("compare: %d\n", compare);

    if (number != compare) {
        printf("longer than 8bits\n");
    } else {
        printf("ok\n");
    }

	return 0;
}
