#include <stdio.h>
#include <stdlib.h>

// Prints floating point number to stdout
void print_float(float number, int decimals)
{
    // Print the integer portion
    printf("%d", (int) number);

    // Print the decimal point
    if (decimals > 0) {
        printf(".");
    }

    // Print decimals after the dot up to the allowed number of decimals
    int counter = 0;
    while (number - (int) number > 0) {
        counter++;
        if (counter > decimals) {
            break;
        } else {
            number = (number - (int) number) * 10;
            printf ("%d", (int) number);
        }
    }

    return;
}




int main()
{

    float float_num = 1234.5677;

    printf("Hello world!\n");
    printf("Number: ");
    print_float(float_num, 4);
    printf("\n");
    printf("Float: %f\n", float_num);


    return 0;
}
