#include <stdio.h>
#include <stdint.h>

int main() {
    int16_t high_number = 6655;
    int16_t low_number = 2;

    int16_t* high_number_pointer = &low_number;
    int16_t* low_number_pointer = &high_number;

    //TODO:
    // Please fix the problem and swap where the pointers point,
    // without using the "&" operator.
    int16_t* storage;

    storage = high_number_pointer;
    high_number_pointer = low_number_pointer;
    low_number_pointer = storage;

    printf("high number: %x\t%d\n", high_number_pointer, *high_number_pointer);
    printf("low number: %x\t%d\n", low_number_pointer, *low_number_pointer);

    return 0;
}
