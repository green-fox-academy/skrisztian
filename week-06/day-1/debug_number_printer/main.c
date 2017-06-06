#include <stdio.h>
#include <stdint.h>
#include <limits.h>

void printf_numbers(int from, int to)
{
	for (int i = from; i < to; i++) {
        printf("%d,", i);
	}
}

int main()
{
	printf_numbers(2147483600, 2147483647);
	printf("\n%d\n", INT_MAX);
	return 0;
}
