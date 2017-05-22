/*
Write a program that reads a number form the standard input,
than prints "Odd" if the number is odd, or "Even" it it is even.
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int number;

    printf("Enter number: ");
    scanf("%d", &number);

    // True if the remainder is 1 -> Odd
    if (number % 2) {
        printf("This number is odd\n");
    } else {
        printf("This number is even\n");
    }

    return 0;
}
