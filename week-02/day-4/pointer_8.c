#include <stdio.h>

int main() {
    int high_number = 2;
    int low_number = 6655;

    int* high_number_pointer = &high_number;
    int* low_number_pointer = &low_number;
    //TODO:
    // Please fix the problem and swap the value of the variables,
    // without using the "high_number" and the "low_number" variables.

    printf("high number: %x\t%d\n", high_number_pointer, *high_number_pointer);
    printf("low number: %x\t%d\n", low_number_pointer, *low_number_pointer);


    int storage = *high_number_pointer;
    *high_number_pointer = *low_number_pointer;
    *low_number_pointer = storage;

    printf("high number: %x\t%d\n", high_number_pointer, *high_number_pointer);
    printf("low number: %x\t%d\n", low_number_pointer, *low_number_pointer);

    return 0;
}
