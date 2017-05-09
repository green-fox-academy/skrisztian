#include <stdio.h>
#include <stdint.h>

int main() {
	uint16_t v = 426;

	// print 'Yeah!' if dividable by 4 but print 'End of program' after regardless
	if (v % 4 == 0) {
        printf("Yeah!\n");
	}

	printf("End of program\n");

	return 0;
}
