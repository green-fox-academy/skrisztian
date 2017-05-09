#include <stdio.h>

int main() {
	int f = 16;

	// please square root f's value

	int test = 1;

	while (f != test*test) {
        test++;
	}

	printf("%d", test);

	return 0;
}
