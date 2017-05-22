//make it work

#include <stdio.h>
#include <time.h>

int main(){

    srand(time(NULL));        // should only be called once
    int number = rand();      // returns a pseudo-random integer between 0 and RAND_MAX
    int choice;

    printf("\t**********\n\tHello User\n\t**********\nYou have an integer named 'number'\n");
    printf("- for it's memory address please press 1\n");
    printf("- for it's current value please press 2\n");
    printf("- to double it's value please press 3\n");

    while(1) {
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Memory address: %d\n", &number);
                break;
            case 2:
                printf("Value: %d\n", number);
                break;
            case 3:
                printf("Double value: %d\n", number*2);
                break;
            }
        }

    return 0;
}
