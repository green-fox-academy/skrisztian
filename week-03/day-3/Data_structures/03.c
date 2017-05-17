#include <stdio.h>

enum CarType {VOLVO, TOYOTA, LAND_ROVER, TESLA};

struct Car {
  enum CarType type;
  double km;
  double gas;
};

// TODO:
// Write a function that takes a Car as an argument and prints all it's stats
// If the car is a Tesla it should not print it's gas level

int printStats(struct Car car) {

    switch (car.type) {
        case VOLVO:
            printf("type: VOLVO\n");
            break;
        case TOYOTA:
            printf("type: TOYOTA\n");
            break;
        case LAND_ROVER:
            printf("type: LAND_ROVER\n");
            break;
        case TESLA:
            printf("type: TESLA\n");
            break;
    }

    printf("km: %f\n", car.km);
    if (car.type != TESLA) {
        printf("gas: %f\n", car.gas);
    }

    return 0;
}


int main() {

    struct Car volvo = {VOLVO, 1000, 30};
    struct Car toyota = {TOYOTA, 3000, 40};
    struct Car land_rover = {LAND_ROVER, 5000, 100};
    struct Car tesla = {TESLA, 150, 0};

    printStats(toyota);

    return 0;
}
