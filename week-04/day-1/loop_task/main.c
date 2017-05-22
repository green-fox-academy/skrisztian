/*
Create a program which writes down the following sentence 100 times:
"I won't cheat on the test!"
Do it with:
    - for loop
    - while loop
    - do while loop
Also create a function which does the same.
*/

#include <stdio.h>

void repeat_with_for(void) {
    for (int i=0; i<100; i++) {
        printf("%3d: I won't cheat on the test!\n", i+1);
    }
}

void repeat_with_while(void) {
    int i = 0;
    while (i<100) {
        printf("%3d: I won't cheat on the test!\n", i+1);
        i++;
    }
}

void repeat_with_do_while(void) {
    int i = 0;
    do {
        // Notice the ++i. It increments i before printing it
        printf("%3d: I won't cheat on the test!\n", ++i);
    } while (i<100);
}



int main() {

    repeat_with_for();
    repeat_with_while();
    repeat_with_do_while();

    return 0;
}
