// TODO:
// Create a function the constructs a Point (returns a pointer to a new point)
// It should take it's x and y coordinate as parameter

// TODO:
// Create a function that takes 2 Points as a pointer and returns the distance between them

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} point;

point *create_point(int x, int y)
{
    // Allocate memory
    point *new_point = (point*) malloc(sizeof(point));

    // Fill in with data
    new_point->x = x;
    new_point->y = y;

    return new_point;
}

float distance(point *a, point *b)
{
    return sqrt(pow((a->x - b->x), 2) + pow((a->y - b->y), 2));
}

int main(void)
{
    point *a = create_point(1, 1);
    point *b = create_point(2, 2);

    printf("distance: %f\n", distance(a, b));

    free(a);
    free(b);

    return 0;
}
