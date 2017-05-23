#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// TODO:
// Create a struct called CircularBuffer
// It should store:
// A pointer that points to the start of a dynamically allocated array of ints (could be null) (head pointer)
// A pointer that points to the end of the dynamically allocated array of ints (could be null) (tail pointer)
// A pointer that points to the currently readable buffer item (could be null) (read pointer))
// A pointer that points to the currently writable buffer item (could be null) (write pointer)
// The length of the buffer

struct circular_buffer {
    int *head_p;
    int *tail_p;
    int *read_p;
    int *write_p;
    int buffer_length;
};

// TODO:
// Create a function that returns a dynamically allocated CircularBuffer (the buffer itself should be allocated as well,
// not just the array inside)
// It should take a normal array and it's length as argument, and it should fill all the buffer values from the array

struct *circular_buffer new_buffer(int *array, int size)
{
    // Allocate memory to new buffer
    int *buffer = (int*) calloc(size, sizeof(int));

    // Allocate memory to the structure
    struct circular_buffer *new_buffer = (struct circular_buffer*) malloc(sizeof(struct circular_buffer));

    new_buffer.head_p = &buffer[0];
    new_buffer.tail_p = &buffer[size - 1];
    new_buffer.buffer_length = size;

    // Fill in new buffer with data
    for (int i = 0; i < size; i++) {
        buffer[i] = array[i];
    }

    // Set read and write pointers
    new_buffer.read_p = new_buffer.head_p;
    new_buffer.write_p = new_buffer.head_p;

    // Return pointer to the structure
    return new_buffer;
};

// TODO:
// Create a function that resizes a CircularBuffer, it should take a CircularBuffer
// as a pointer and change that buffers length and reallocate it's array as well. (copying it's values)

// TODO:
// Create a function that takes a CircularBuffer and returns a new CircularBuffer that only contains
// the even numbers from the old CircularBuffer

// TODO:
// Try all the functions in the main function, please deallocate any buffer and the array inside it after using them

int main()
{
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8};

    size = sizeof(array) / sizeof(array[0]);
    return 0;
}
