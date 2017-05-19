#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846

int main() {
    double pi = M_PI;
    double x;
    double y;

    for (int i = 0; i<256; i++) {

        x = 2*pi / 256 * i;
        y = round(sin(x)*2000+2000);
        printf("%d, ", (int) y);
    }
}
