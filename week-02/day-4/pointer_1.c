#include <stdio.h>

int main() {
    unsigned int number = 1234;
    //TODO:
    // print the memory address of the variable
    printf("%x\n", &number);

    unsigned int* pointer = &number;
    printf("%x\n", &number);

    return 0;
}
