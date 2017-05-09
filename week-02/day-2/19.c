#include <stdio.h>
#include <stdint.h>

int main() {
	int a = 654;
	uint64_t b = 654987312;

	// Tell if the remainder of b devided by a is 3
    if (b % a == 3) {
        printf("remainder is 3");
    } else {
        printf("remainder is not 3");
    }

	return 0;
}
