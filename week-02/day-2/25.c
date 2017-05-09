#include <stdio.h>
#include <stdint.h>

int main() {

	// print the even numbers till 20
    int8_t i;

    for (i=0; i<=20; i++) {
        if (i % 2 == 0) {
            printf("%d\n", i);
        }
    }

	return 0;
}
