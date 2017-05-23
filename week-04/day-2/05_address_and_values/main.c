#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Print out the addresses with and without pointer
print out the values with and without pointer
 */


int main()
{
    int number = 2017;
    float Pi = 3.14;
    char letter = 't';

    int *ptr_number = &number;
    float *ptr_pi = &Pi;
    char *ptr_letter = &letter;

    printf("%p\t%d\t%p\t%d\n", &number, number, ptr_number, *ptr_number);
    printf("%p\t%f\t%p\t%f\n", &Pi, Pi, ptr_pi, *ptr_pi);
    printf("%p\t%c\t%p\t%c\n", &letter, letter, ptr_letter, *ptr_letter);

    return 0;
}
