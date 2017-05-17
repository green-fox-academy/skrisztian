#include <stdio.h>
#include <stdlib.h>

/** TODO:
 * Create a function called "range" that creates a new array and returns a pointer to it.
 * It should fill the array with numbers based on it's paramters.
 * It should take 3 paramters:
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


 int* range(int from, int to, int step) {

    if (from > to) {
        int temp = from;
        from = to;
        to = temp;
    }

    int element = from;
    int size = 0;

    while (element < to) {
        size++;
        element += step;

        printf("Size: %d\n", size);
        printf("Element: %d\n", element);
    }

    // Create array
    int* array = (int) calloc(size, sizeof(int));
    for (int i=0; i<size; i++) {
            array[i] = from + step * i;
    }

    // Print result
    printf("The range is:\n");
    for (int i=0; i<size; i++) {
        printf("%d\t", array[i]);
    }

    // Return the array pointer
    return array;
 }

int main() {
    int range_from;
    int range_to;
    int range_step;
    int* array_ptr;

    // Get data from user
    printf("Range from: ");
    scanf("%d", &range_from);
    printf("Range to: ");
    scanf("%d", &range_to);
    printf("Steps: ");
    scanf("%d", &range_step);

    // Create array
    array_ptr = range(range_from, range_to, range_step);

    // Collect garbage
    free(array_ptr);

    return 0;
}
