#include <stdio.h>

struct pirate {
    char name[256];
    int has_wooden_leg;
    short int gold_count;
};

// TODO:
// Create a function that takes an array of pirates (and it's length) then returns the sum of the golds of all pirates

int sum_gold(struct pirate *pirates, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += pirates[i].gold_count;
    }
    return sum;
}

// TODO:
// Create a function that takes an array of pirates (and it's length) then returns the average of the gold / pirate

float avg_gold(struct pirate *pirates, int size)
{
    int sum = sum_gold(pirates, size);
    return (float) sum / (float) size;
}

// TODO:
// Create a function that takes an array of pirates (and it's length) then returns the name of the
// richest that has wooden leg

char *richest(struct pirate *pirates, int size)
{
    int max_gold = 0;
    int max_id = 0;
    for (int i = 0; i < size; i++) {
        if ((pirates[i].has_wooden_leg) && (pirates[i].gold_count > max_gold)) {
            max_gold = pirates[i].gold_count;
            max_id = i;
        }
    }

    return pirates[max_id].name;
}

int main()
{
    struct pirate pirates[] = {
        {"Jack", 0, 18},
        {"Uwe", 1, 8},
        {"Hook", 1, 12},
        {"Halloween kid", 0, 0},
        {"Sea Wolf", 1, 14},
        {"Morgan", 0, 1}
    };

    int size = sizeof(pirates) / sizeof(struct pirate);

    printf("Sum of golds: %d\n", sum_gold(pirates, size));
    printf("Avg of golds: %f\n", avg_gold(pirates, size));
    printf("Richest: %s\n", richest(pirates, size));

    return 0;
}
