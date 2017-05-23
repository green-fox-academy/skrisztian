#include <stdio.h>
#include <stdlib.h>

// TODO:
// Create a function that takes a double array as a parameter, and the length of the array.
// This function should calculate the average of the elements.

// TODO:
// Create an other function that takes a double array as a parameter, and the length of the array.
// It calculates how many elements are under the average, and returns with that.

// TODO:
// Create an other function that takes a double array as a parameter, and the length of the array.
// It allocates a new array, and loads into it the elements which are under the average.
// It returns a pointer to the new array.

float calculate_average(double *array, int size)
{
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }

    return (sum / size);
}

int count_elements_under_average(double *array, int size)
{
    double average = calculate_average(array, size);
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] < average)
            count++;
    }

    return count;
}

double *elements_under_average(double *array, int size)
{
    double average = calculate_average(array, size);
    int count = count_elements_under_average(array, size);

    double *new_array = (double*) calloc(count, sizeof(double));

    int j = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] < average) {
            new_array[j] = array[i];
            j++;
        }
    }

   return new_array;
}


int main()
{

    double list[] = {12.5, 34.5, 43.2, 100.0, 762.3, 457.5};

    double average = calculate_average(list, sizeof(list) / sizeof(double));
    int element_count = count_elements_under_average(list, sizeof(list) / sizeof(double));

    printf("average: %f\n", average);
    printf("elements under average: %d\n", element_count);

    for (int i = 0; i < element_count; i++) {
        printf("%d\t%f\n", i, elements_under_average(list, sizeof(list) / sizeof(double))[i]);
    }

    return 0;
}
