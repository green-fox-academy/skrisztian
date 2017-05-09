#include <stdio.h>

int main() {
	int k = 1521;

	// tell if k is dividable by 3 or 5

	if ((k%3 == 0) || (k%5 == 0)) {
        printf("%d is divisible by 3 or 5", k);
	} else {
        printf("%d is not divisible by 3 or 5\n", k);
	}

	return 0;
}
