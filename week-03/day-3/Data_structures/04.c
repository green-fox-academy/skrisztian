#include <stdio.h>
#include <string.h>

struct Pirate {
    char name[256];
    int has_wooden_leg;
    short int gold_count;
};

// TODO:
// Create a function that takes an array of pirates (and it's length) then returns the sum of the golds of all pirates

// TODO:
// Create a function that takes an array of pirates (and it's length) then returns the average of the gold / pirate

// TODO:
// Create a function that takes an array of pirates (and it's length) then returns the name of the
// richest that has wooden leg

int sumGold(struct Pirate* array, int size) {

    // Sum the gold
    int sum = 0;
    for (int i=0; i<size; i++) {
        sum +=  array[i].gold_count;
    }

    return sum;
}

float avgGold(struct Pirate* array, int size) {

    // Average the gold
    float avg = 0;
    int sum = sumGold(array, size);
    avg = (float) sumGold(array, size) / (float) size;

    return avg;
}

char* richestPirate(struct Pirate* array, int size) {
    int max_gold = 0;
    int max_id = 0;

    for (int i=0; i<size; i++) {
        if (array[i].has_wooden_leg) {
            if (array[i].gold_count > max_gold) {
                max_gold = array[i].gold_count;
                max_id = i;
            }
        }
    }

    return array[max_id].name;

}

int main() {
    struct Pirate pirates[] = {
        {"Jack", 0, 18},
        {"Uwe", 1, 8},
        {"Hook", 1, 12},
        {"Halloween kid", 0, 0},
        {"Sea Wolf", 1, 14},
        {"Morgan", 0, 1}
    };

    printf("All gold:\t%d\n", sumGold(pirates, 6));
    printf("Average gold:\t%f\n", avgGold(pirates, 6));
    printf("Richest pirate:\t%s\n", richestPirate(pirates, 6));

    return 0;
}
