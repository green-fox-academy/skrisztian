#include <stdio.h>
#include <stdint.h>

int main() {
	uint8_t ad = 6;

	// print the numbers till ad from 0
    uint8_t i;
    for (i=0; i<=ad; i++) {
        printf("%d\n", i);
    }

    return 0;
}