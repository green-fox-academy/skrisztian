#include <stdio.h>

int main() {
    int numbers[] = {5, 6, 7, 8, 9};
    int* number_pointer = &numbers[0];

    printf("%x\t%d\n", number_pointer, *number_pointer);

    //TODO:
    // The "number_pointer" should point to the third element of the array called "numbers",
    // than please print its value with it. Solve the problem without reusing the "&" operator
    // for getting the address of the third element.
    number_pointer += 2;
    printf("%x\t%d\n", number_pointer, *number_pointer);

    return 0;
}
