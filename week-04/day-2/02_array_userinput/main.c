#include <stdio.h>
#include <stdlib.h>

// TODO:
// Please create a program that asks for a count when it starts.
// It should ask for a number count times, then it should print the average of the numbers.
// It should delete any dynamically allocated resource before the program exits.

int main()
{
    int count;

    printf("Count: ");
    scanf("%d", &count);

    int *array = calloc(count, sizeof(int));

    for (int i = 0; i < count; i++) {
        printf("Enter number %d: ", i + 1);
        scanf("%d", &array[i]);
    }

    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += (double) array[i];
    }

    printf("Average: %f", sum / (double) count);

    free(array);

    return 0;
}
