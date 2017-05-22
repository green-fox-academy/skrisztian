/* Write a function which is called "get_fibonacci_number". It returns the "N"th
 * Fibonacci number, where "N" is received as a paramter.
 * For example:
 *  - get_fibonacci_number(1) returns 0
 *  - get_fibonacci_number(2) returns 1
 *  - get_fibonacci_number(3) returns 1
 *  - get_fibonacci_number(4) returns 2
 *  - get_fibonacci_number(5) returns 3
 *  - get_fibonacci_number(6) returns 5
 *
 *  If the function gets invalid parameter it returns -1.
 */

/*
Using that function create a program, which gets a number from the user, and
prints out the "N"th Fibonacci number, where "N" is the user given number.
Alert the user if he/she entered invalid value.
*/

#include <stdio.h>
#include <stdlib.h>

int get_fibonacci_number(int n) {
    int fibonacci_number_minus_2 = 0;
    int fibonacci_number_minus_1 = 1;
    int fibonacci_number;

    if (n < 1) {
        fibonacci_number = -1;
    } else if (n == 1) {
        fibonacci_number = 0;
    } else if (n == 2) {
        fibonacci_number = 1;
    } else {
        for (int i=2; i<n; i++) {
            fibonacci_number = fibonacci_number_minus_1 + fibonacci_number_minus_2;
            fibonacci_number_minus_2 = fibonacci_number_minus_1;
            fibonacci_number_minus_1 = fibonacci_number;
        }
    }
    return fibonacci_number;
}

int main()
{
    for (int i=0; i<10; i++) {
        printf("%d\t%d\n", i, get_fibonacci_number(i));
    }

    return 0;
}
