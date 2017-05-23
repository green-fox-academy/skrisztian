#include <stdio.h>
#include <stdlib.h>

/** TODO:
 * Create a function called "range" that creates a new array and returns a pointer to it.
 * It should fill the array with numbers based on it's parameters.
 * It should take 3 parameters:
 * int from: the first number in the array
 * int to: it will fill the array till it would reach this number (so this number is not in the array)
 * int step: the step between the numbers
 *
 * Examples:
 * range(0, 10, 2) -> {0, 2, 4, 6, 8}
 * range(1, -8, -3) -> {1, -2, -5}
 *
 * It should delete any dynamically allocated resource before the program exits.
 */

 int *range(int from, int to, int step)
 {
    int size = 0;
    if (to >= from) {
        for (int i = from; i < to; i += step) {
            size++;
        }
    } else {
        for (int i = from; i > to; i += step) {
            size++;
        }
    }

     // Create and fill array
     int *array = (int*) calloc(size, sizeof(int));

     for (int i = 0; i < size; i++) {
        array[i] = from + i * step;
     }

     // Print array
     for (int i = 0; i < size; i++) {
        printf("%d\t", array[i]);
     }

     return array;
 }



int main()
{
    int *pointer = range(0, 10, 2);
    free(pointer);
    return 0;
}
