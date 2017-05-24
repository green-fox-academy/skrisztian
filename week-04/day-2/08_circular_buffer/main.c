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

struct circular_buffer *new_buffer(int *array, int size)
{
    // Allocate memory to new buffer
    int *buffer = (int*) calloc(size, sizeof(int));

    // Allocate memory to the structure
    struct circular_buffer *new_buffer = (struct circular_buffer*) malloc(sizeof(struct circular_buffer));

    new_buffer->head_p = &buffer[0];
    new_buffer->tail_p = &buffer[size - 1];
    new_buffer->buffer_length = size;

    // Fill in new buffer with data
    for (int i = 0; i < size; i++) {
        buffer[i] = array[i];
    }

    // Set read and write pointers
    new_buffer->read_p = new_buffer->head_p;
    new_buffer->write_p = new_buffer->head_p;

    // Return pointer to the structure
    return new_buffer;
}

// TODO:
// Create a function that resizes a CircularBuffer, it should take a CircularBuffer
// as a pointer and change that buffers length and reallocate it's array as well. (copying it's values)
int resize_buffer(struct circular_buffer *buffer, int new_size)
{
    int return_value = 0;

    // Need to remember where the read an write pointers are
    int read_pos = buffer->read_p - buffer->head_p;
    int write_pos = buffer->write_p - buffer->head_p;

    // Resize buffer
    int *resized_buffer = (int*) realloc(buffer->head_p, new_size * sizeof(int));
    buffer->buffer_length = new_size;

    // Setup the pointers
    buffer->head_p = &resized_buffer[0];
    buffer->tail_p = &resized_buffer[new_size-1];
    buffer->read_p = buffer->head_p + read_pos;
    buffer->write_p = buffer->head_p + write_pos;

    // If we decrease the size the read/write pointers may fall out of range
    // In that case reset them to the tail position and let the user know
    if (buffer->read_p > buffer->tail_p) {
        buffer->read_p = buffer->tail_p;
        return_value += 1;
    }

    if (buffer->write_p > buffer->tail_p) {
        buffer->write_p = buffer->tail_p;
        return_value += 2;
    }

    return return_value;
}

// TODO:
// Create a function that takes a CircularBuffer and returns a new CircularBuffer that only contains
// the even numbers from the old CircularBuffer
int create_even_buffer(struct circular_buffer *buffer)
{
    // Save even numbers in incoming buffer into a temporary array,
    // which has the same size as the buffer
    int *array = (int*) calloc(buffer->buffer_length, sizeof(int));
    int i = 0;
    int *j;
    for (j = buffer->head_p; j <= buffer->tail_p; j++) {
        if (*j % 2 == 0) {
            array[i] = *j;
            i++;    // i will return size of array too
        }
    }

    // Cut away the empty space from array
    int *shortend_array = (int*) realloc(array, i * sizeof(int));

    // Create new circular buffer with the even numbers
    struct circular_buffer *even_buffer = new_buffer(shortend_array, i);

    // Cleanup garbage
    free(shortend_array);

    return even_buffer;
}

// TODO:
// Try all the functions in the main function, please deallocate any buffer and the array inside it after using them

int main()
{
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8};

    int size = sizeof(array) / sizeof(array[0]);

    struct circular_buffer *c_buffer = new_buffer(array, size);

    printf("buffer->head_p: %d\n", c_buffer->head_p);
    printf("buffer->tail_p: %d\n", c_buffer->tail_p);
    printf("buffer->read_p: %d\n", c_buffer->read_p);
    printf("buffer->write_p: %d\n", c_buffer->write_p);
    printf("buffer->buffer_length: %d\n", c_buffer->buffer_length);

    for (int i = 0; i < c_buffer->buffer_length; i++) {
        printf("%d\t%d\n", i, *(c_buffer->head_p + i));
    //    printf("%d\t%d\n", i, c_buffer->head_p[i]);
    }

    resize_buffer(c_buffer, 7);

    printf("buffer->head_p: %d\n", c_buffer->head_p);
    printf("buffer->tail_p: %d\n", c_buffer->tail_p);
    printf("buffer->read_p: %d\n", c_buffer->read_p);
    printf("buffer->write_p: %d\n", c_buffer->write_p);
    printf("buffer->buffer_length: %d\n", c_buffer->buffer_length);

    for (int i = 0; i < c_buffer->buffer_length; i++) {
        printf("%d\t%d\n", i, *(c_buffer->head_p + i));
    //    printf("%d\t%d\n", i, c_buffer->head_p[i]);
    }

    struct circular_buffer *e_buffer = create_even_buffer(c_buffer);

    printf("buffer->head_p: %d\n", e_buffer->head_p);
    printf("buffer->tail_p: %d\n", e_buffer->tail_p);
    printf("buffer->read_p: %d\n", e_buffer->read_p);
    printf("buffer->write_p: %d\n", e_buffer->write_p);
    printf("buffer->buffer_length: %d\n", e_buffer->buffer_length);

    for (int i = 0; i < e_buffer->buffer_length; i++) {
        printf("%d\t%d\n", i, *(e_buffer->head_p + i));
    //    printf("%d\t%d\n", i, c_buffer->head_p[i]);
    }



    return 0;
}
