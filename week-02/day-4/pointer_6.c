#include <stdio.h>

int main() {
    int high_number = 6655;
    int low_number = 2;

    int* high_number_pointer = &high_number;
    int* other_high_number_pointer;

    //TODO:
    // The "other_high_number_pointer" should point to the same memory address
    // without using the "&" operator.
    other_high_number_pointer = high_number_pointer;
    printf("%x\t%d\n", high_number_pointer, *high_number_pointer);
    printf("%x\t%d\n", other_high_number_pointer, *other_high_number_pointer);


    return 0;
}
