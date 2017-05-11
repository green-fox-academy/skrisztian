#include <stdio.h>
#include <stdint.h>

int main() {
    char char_array[] = {'a', 'b', 'c', 'd'};
    char char_array_the_same[] = "abcd";

	//TODO:
    // Print out the two arrays character-by-character
    for (uint8_t i=0; i<sizeof(char_array); i++){
        printf("%c\t", char_array[i]);
    }
    printf("*\n");

    for (uint8_t i=0; i<sizeof(char_array_the_same); i++){
        printf("%c\t", char_array_the_same[i]);
    }
    printf("*\n");

	//TODO:
    // What is the difference between the arrays?

    /* In the first one, there is no "zero string terminator" */

    return 0;
}
