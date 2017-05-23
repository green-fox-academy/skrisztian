#include <stdio.h>
#include <stdlib.h>

// TODO:
/**
 * Create a function called "resize" that takes a pointer to an int array, it's size and it's new size,
 * then it should return a new pointer that points to the new resized array.
 * It should copy all the elements to the new array, if the array is bigger it should fill all the new slots with zeros.
 * It should delete the old array.
 */

int *resize(int *array, int size, int new_size)
{
    int *new_array = (int*) realloc(array, new_size * sizeof(int));

    return new_array;
}


int main()
{
    int list[] = {1, 2, 3};
    int *long_list = resize(list, 3, 10);
    free(long_list);

    return 0;
}
