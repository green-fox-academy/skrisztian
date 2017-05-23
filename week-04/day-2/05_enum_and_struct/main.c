#include <stdio.h>

enum car_type {VOLVO, TOYOTA, LAND_ROVER, TESLA};

struct car {
    enum car_type type;
    double km;
    double gas;
};

// TODO:
// Write a function that takes a Car as an argument and prints all it's stats
// If the car is a Tesla it should not print it's gas level

int print_car_stat(struct car c)
{
    char type[][15] = {"VOLVO", "TOYOTA", "LAND_ROVER", "TESLA"};

    printf("Type : %s\n", type[c.type]);
    printf("Km: %f\n", c.km);
    printf("Gas: %f\n", c.gas);

    return 0;
}

int main()
{
    struct car volvo = {VOLVO, 1000, 30};
    struct car toyota = {TOYOTA, 3000, 40};
    struct car land_rover = {LAND_ROVER, 5000, 100};
    struct car tesla = {TESLA, 150, 0};

    print_car_stat(volvo);

    return 0;
}
