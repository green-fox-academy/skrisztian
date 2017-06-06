#include <stdio.h>

void int_to_bin_digit(int in, int count)
{
	int mask;
	int i;
	for (i = count-1; i >= 0; i--) {
        mask = (1 << i);
		int result = (in & mask) ? 1 : 0;
		printf("%d", result);
	}
}

int main()
{
	int_to_bin_digit(16, 32);
	return 0;
}
