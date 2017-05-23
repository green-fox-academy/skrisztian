#include <stdio.h>

struct rectangular_cuboid {
    double a;
    double b;
    double c;
};

// TODO:
// Write a function called "GetSurface" that takes a RectangularCuboid
// and returns it's surface

// TODO:
// Write a function called "GetVolume" that takes a RectangularCuboid
// and returns it's volume

double GetSurface(struct rectangular_cuboid body)
{
    double surface = 2 * (body.a * body.b + body.b * body.c + body.c * body.a);
    return surface;
}

double GetVolume(struct rectangular_cuboid body)
{
    double volume = body.a * body.b * body.c;
    return volume;
}


int main()
{
    struct rectangular_cuboid cube = {3, 4, 5};
    printf("Surface: %f\n", GetSurface(cube));
    printf("Volume: %f\n", GetVolume(cube));

    return 0;
}
