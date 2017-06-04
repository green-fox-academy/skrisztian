#include <stdio.h>

typedef struct {
    double a;
    double b;
    double c;
}rectangular_cuboid;

// TODO:
// Write a function called "GetSurface" that takes a RectangularCuboid
// and returns it's surface

// TODO:
// Write a function called "GetVolume" that takes a RectangularCuboid
// and returns it's volume

long get_surface(rectangular_cuboid c)
{
    return 2* (c.a * c.b + c.a * c.c + c.b * c.c);
}

long get_volume(rectangular_cuboid c)
{
    return c.a * c.b * c.c;
}



int main()
{
    rectangular_cuboid cube = {1, 2, 3};

    printf("Surface: %d\n", get_surface(cube));
    printf("Volume: %d", get_volume(cube));

    return 0;
}
