#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Add two numbers using pointers
*/


int main()
{

    int a  = 20;
    int b = 17;


    int *ptr_a = &a;
    int *ptr_b = &b;

    int c = *ptr_a + *ptr_b;

    printf("%d\n", c);

    return 0;
}
