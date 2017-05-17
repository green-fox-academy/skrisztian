#include <stdio.h>
#include <stdlib.h>

// TODO:
// Please create a program that asks for a count when it starts.
// It should ask for a number count times, then it should print the average of the numbers.
// It should delete any dynamically allocated resource before the program exits.

int main() {

    int count;
    float average = 0;
    int* array;    // This is actually a pointer

    printf("Hello! How many numbers do you want to average? ");
    scanf("%d", &count);
    printf("Ok, give me the %d numbers:\n", count);

    // Reserve memory
    array = (int) calloc(count, sizeof(int));

    // Ask for numbers to average
    for (int i=0; i<count; i++) {
        printf("Number %d: ", i+1);
        scanf("%d", &array[i]);
    }

    // Calculate average
    for (int i=0; i<count; i++) {
        average += array[i];
    }
    average = average / count;

    // Print result
    printf("The average is: %f\n", average);

    // Collect garbage
    free(array);
    
    return 0;
}
