#include <stdio.h>

int main()
{
    int numbers[] = {5, 6, 7, 8, 9};
    int* number_pointer;

    //TODO:
    // The "number_pointer" should point to the first element of the array called "numbers",
    // than please print its value with it.

    number_pointer = &numbers[0];

    // This is good too:
    // number_pointer = &numbers[0];

    printf("%p\t%d\n", number_pointer, *number_pointer);

    return 0;
}
