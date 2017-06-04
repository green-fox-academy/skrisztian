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
// The lenght of the buffer

typedef struct {
    int *head;
    int *tail;
    int *read;
    int *write;
    int length;
}circular_buffer_t;

// TODO:
// Create a function that returns a dynamically allocated CircularBuffer (the buffer itself should be allocated as well, not just the array inside)
// It should take a normal array and it's length as argument, and it should fill all the buffer values from the array

circular_buffer_t *create_circular_buffer(int *array, int length)
{
    // Create buffer type
    circular_buffer_t *new_buffer = (circular_buffer_t*) malloc(sizeof(circular_buffer_t));

    // Create buffer array
    new_buffer->head = (int*) calloc(length, sizeof(int));

    // Setup parameters
    new_buffer->tail = new_buffer->head + length;
    new_buffer->read = new_buffer->head;
    new_buffer->write = new_buffer->head;
    new_buffer->length = length;

    // Copy values
    memcpy(new_buffer->head, array, length * sizeof(int));

    return new_buffer;
}

// TODO:
// Create a function that resizes a CircularBuffer, it should take a CircularBuffer as a pointer and change that buffers length and reallocate it's array as well. (copying it's values)
void resize_buffer(circular_buffer_t *buffer, int new_length)
{
    buffer->head = (int*) realloc(buffer->head, new_length * sizeof(int));
    buffer->tail = buffer->head + new_length;
    if (buffer->read > buffer->tail)
        buffer->read = buffer->head;
    if (buffer->write > buffer->tail)
        buffer->write = buffer->head;
    buffer->length = new_length;

    return;
}


// TODO:
// Create a function that takes a CircularBuffer and returns a new CircularBuffer that only contains the even numbers from the old CircularBuffer
circular_buffer_t *create_even_circular_buffer(circular_buffer_t *buffer)
{
    // Pick out even numbers into temp_array
    int *temp_array = (int*) calloc(buffer->length, sizeof(int));
    int even_count = 0;
    for (int i = 0; i < buffer->length; i++) {
        if (buffer->head[i] % 2 == 0) {
            temp_array[even_count] = buffer->head[i];
            even_count++;
        }
    }

    // Cut off empty part of temp_array
    temp_array = (int*) realloc(temp_array, even_count * sizeof(int));

    // Create new buffer
    circular_buffer_t *even_buffer = create_circular_buffer(temp_array, even_count);

    free(temp_array);

    return even_buffer;
}

void deconstruct_circular_buffer(circular_buffer_t *buffer)
{
    free(buffer->head);
    free(buffer);

    return;
}


// TODO:
// Try all the functions in the main function, plase deallocate any buffer and the array inside it after using them

int main()
{
    int array[] = {11, 12, 13, 14, 15};

    circular_buffer_t *c_buffer = create_circular_buffer(array, sizeof(array) / sizeof(array[0]));

    printf("buffer->head_p: %d\n", c_buffer->head);
    printf("buffer->tail_p: %d\n", c_buffer->tail);
    printf("buffer->read_p: %d\n", c_buffer->read);
    printf("buffer->write_p: %d\n", c_buffer->write);
    printf("buffer->buffer_length: %d\n", c_buffer->length);

    for (int i = 0; i < c_buffer->length; i++) {
        printf("%d\t%d\n", i, *(c_buffer->head + i));
    }

    resize_buffer(c_buffer, 7);

    printf("buffer->head_p: %d\n", c_buffer->head);
    printf("buffer->tail_p: %d\n", c_buffer->tail);
    printf("buffer->read_p: %d\n", c_buffer->read);
    printf("buffer->write_p: %d\n", c_buffer->write);
    printf("buffer->buffer_length: %d\n", c_buffer->length);

    for (int i = 0; i < c_buffer->length; i++) {
        printf("%d\t%d\n", i, *(c_buffer->head + i));
    }

    circular_buffer_t *e_buffer = create_even_circular_buffer(c_buffer);

    printf("buffer->head_p: %d\n", e_buffer->head);
    printf("buffer->tail_p: %d\n", e_buffer->tail);
    printf("buffer->read_p: %d\n", e_buffer->read);
    printf("buffer->write_p: %d\n", e_buffer->write);
    printf("buffer->buffer_length: %d\n", e_buffer->length);

    for (int i = 0; i < e_buffer->length; i++) {
        printf("%d\t%d\n", i, *(e_buffer->head + i));
    }



    deconstruct_circular_buffer(c_buffer);
    deconstruct_circular_buffer(e_buffer);


    return 0;
}
