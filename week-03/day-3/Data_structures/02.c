#include <stdio.h>

struct RectangularCuboid {
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

int GetSurface(struct RectangularCuboid body) {

    int surface = 2 * (body.a * body.b + body.b * body.c + body.a * body.c);
    return surface;
}

int GetVolume(struct RectangularCuboid body) {

    int volume = body.a * body.b * body.c;
    return volume;
}


int main() {

    struct RectangularCuboid new_body = {3, 5, 8};

    printf("Surface: %d\n", GetSurface(new_body));
    printf("Volume: %d\n", GetVolume(new_body));

    return 0;
}
