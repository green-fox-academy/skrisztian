#include <stdio.h>
#include <stdint.h>

struct Computer {
    float cpu_speed_GHz;
    int ram_size_GB;
    int bits;
};

int main() {
    struct Computer computer = {3.2, 8, 32};

    //TODO: Change the bits to 64
    computer.bits = 64;

    //TODO: print out the structure members
    printf("computer cpu speed [GHz]: %f\n", computer.cpu_speed_GHz);
    printf("computer ram size [GB]: %d\n", computer.ram_size_GB);
    printf("computer bits: %d\n", computer.bits);

    return 0;
}