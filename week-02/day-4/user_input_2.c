#include <stdio.h>

int main() {
    char buffer[255];   // Buffer variable for user input

    //TODO:
    // Get the user's name with gets
    printf("Name : ");
    gets(buffer);

    //TODO:
    // Print it out with puts
    printf("Your name is: ");
    puts(buffer);

    //TODO:
    // Get the user's pet name with getch

    // Init and zero out buffer2
    // Buffer variable for user input
    char buffer2[255];
    for (int i=0; i<sizeof(buffer2); i++){
        buffer2[i] = 0;
    }

    printf("Pet name : ");

    // Get user input
    int letter = 0;
    int j = 0;
    while (letter != 10) {
        letter = getchar();
        buffer2[j] = letter;
        j++;
    }

    //TODO:
    // Print it out
    printf("Your pet's name is %s.\n", buffer2);

    return 0;
}
