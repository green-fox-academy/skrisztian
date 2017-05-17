#include <stdio.h>
#include <stdlib.h>

// TODO:
/**
 * Create a function called "resize" that takes a pointer to an int array, it's size and it's new size,
 * then it should return a new pointer that points to the new resized array.
 * It should copy all the elements to the new array, if the array is bigger it should fill all the new slots with zeros.
 * It should delete the old array.
 */

 int* resize(int* array_ptr, int size, int new_size) {

    // Resize arrat
    int* new_array_ptr = (int*) realloc(array_ptr, new_size*sizeof(int));

    // Fill in extra space with zeros
    for (int i=new_size-size; i<new_size; i++) {
        new_array_ptr[i] = 0;
    }

    // Print result
    for (int i=0; i<new_size; i++) {
        printf("%d\t", new_array_ptr[i]);
    }
    printf("\n");

    return new_array_ptr;
 }

int main() {

    // Starting array
    int* array = (int*) calloc(4, sizeof(int));
    for (int i=0; i<4; i++) {
            array[i] = i;
    }

    // Resized array
    int* new_array = resize(array, 4, 8);

    // Collect garbage
    free(new_array);

    // Check garbage collection
    printf("array address: %d\n", array);
    printf("new_array address: %d\n", new_array);

    return 0;
}
