#include <stdio.h>

/* Write a non void function which has a paramters (let's say for example x).
The funtion than returns the "xth" fibonaci number.
What the hack is a fibonacci number? Search it on wikipedia!
*/

long fibonacci(int number);

int main(){

    int x = 4;
    printf("%ld\n", fibonacci(x));

    return 0;
}

long fibonacci(int number){

    int a = 0;
    int b = 1;
    int i;
    long fibonacci_num;

    switch(number) {
        case 1:
            fibonacci_num = 0;
            break;
        case 2:
            fibonacci_num = 1;
            break;
        default:
            for(i=1; i<=number-2; i++){
                fibonacci_num = a + b;
                a = b;
                b = fibonacci_num;
            }
    }

     return fibonacci_num;
}
