#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int x;
    int y;
} point_t;

// TODO:
// Create a function that constructs a Point (returns a pointer to a new point)
// It should take it's x and y coordinate as parameter

// TODO:
// Create a function that takes 2 Points as a pointer and returns the distance between them

point_t *new_point(int x, int y)
{
    point_t *new_point = (point_t*) malloc(sizeof(point_t));
    (*new_point).x = x;
    (*new_point).y = y;

    return new_point;
}

double distance(point_t *p1, point_t *p2)
{
    // Element of a pointer
    double x1 = (double) p1->x;
    double x2 = (double) p2->x;

    // Element of a dereferenced pointer (=element of value)
    double y1 = (double) (*p1).y;
    double y2 = (double) (*p2).y;

    double dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    return dist;
}

int main()
{

    point_t *point1 = new_point(1, 1);
    point_t *point2 = new_point(2, 2);

    printf("Distance: %f\n", distance(point1, point2));

    free(point1);
    free(point2);

    return 0;
}
