#include <stdio.h>

int main()
{
    char char_array[] = {'a', 'b', 'c', 'd'};
    char char_array_the_same[] = "abcd";

    //TODO:
    // Print out the two arrays character-by-character

    for (int i = 0; i < sizeof(char_array); i++) {
        printf("%c\t", char_array[i]);
    }
    printf("<-\n");

    for (int i = 0; i < sizeof(char_array_the_same); i++) {
        printf("%c\t", char_array_the_same[i]);
    }
    printf("<-\n");


    //TODO:
    // What is the difference between the arrays?

    // char_array_the_same has null string termination

    return 0;
}
