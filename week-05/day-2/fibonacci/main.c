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

#include <stdio.h>

int get_fibonacci_number(int n)
{
    if (n < 1)
        return -1;
    else if (n == 1)
        return 0;
    else if (n == 2)
        return 1;
    else
        return get_fibonacci_number(n-1) + get_fibonacci_number(n-2);
 }

 int main(void)
 {
     for (int i = 0; i <= 6; i++) {
        printf("i: %d\tfibonacci: %d\n", i, get_fibonacci_number(i));
     }
     return 0;
 }
