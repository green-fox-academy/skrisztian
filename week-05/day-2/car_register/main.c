#include <string.h>
#include <stdio.h>

/* Write a car register!
 * You should store the following data in a structure, called "car":
 *  - the manufacturer's name (which is shorter than 256 characters)
 *  - the price of the car (in euros, stored as a floating point number)
 *  - the year of manufacture
 *  - the type of the transmission (as a custom type, see below)
 *
 * You should store the transmission type in an enumeration ("transmission"),
 * the valid types are:
 *  - manual
 *  - automatic
 *  - CVT
 *  - semi-automatic
 *  - dual-clutch
 *
 * The "car"-s are stored in an array.
 *
 * Write the following functions:
 *  - get_older_cars_than(struct car* array, int array_length, int age)
 *      - it returns the count of the older cars than "age"
 *  - get_transmission_count(struct car* array, int array_length, enum transmission trnsm)
 *      - it returns the count of cars which has "trnsm" transmission
 */

 enum transmission {MANUAL, AUTOMAIC, CVT, SEMI_AUTOMATIC, DUAL_CLUTCH};
 int this_year = 2017;

 struct car{
    char name[256];
    float price;
    int manufacture_year;
    enum transmission transmission_type;
 };

 int get_older_cars_than(struct car *array, int array_length, int age)
 {
     int count = 0;
     for (int i = 0; i < array_length; i++) {
        if (this_year - array[i].manufacture_year > age)
            count++;
     }
     return count;
 }

 int get_transmission_count(struct car *array, int array_length, enum transmission trnsm)
 {
     int count = 0;
     for (int i = 0; i < array_length; i++) {
        if (array[i].transmission_type == trnsm)
            count++;
     }
     return count;
 }

int main()
{

    struct car cars[] = {{"Toyota", 12000.0, 1998, MANUAL},
                         {"Lada", 2000.0, 1970, MANUAL},
                         {"Honda", 25000.0, 2010, CVT}};

    printf("older than 10: %d\n", get_older_cars_than(cars, sizeof(cars) / sizeof(cars[0]), 10));
    printf("CVT: %d\n", get_transmission_count(cars, sizeof(cars) / sizeof(cars[0]), CVT));

    return 0;
}



