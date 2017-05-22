/*
Create a program which ask for a name and an age.
From the age it tells the user when he or she will be 100 years old.
You should only take care of the years (don't bother with the months or days).
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int age;
    char name[256];

    printf("Your name: ");
    scanf("%s", &name);
    printf("Your age: ");
    scanf("%d", &age);

    int now = 2017;
    int date = 100 - age + 2017;

    printf("%s, you will be 100 years old in %d\n", name, date);

    return 0;
}
