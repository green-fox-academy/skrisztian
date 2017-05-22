/*
Try to write a console program which is similar to the classic game "snake".
You will need console related special library functions.
On windows use conio.h
On linux use curses.h
You can use the getch() function to get the code of the button which was pressed
by the user.
At first try to make a snake which goes straight on the screen.
After that try to change the direction of the movement with the arrow keys.
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdlib.h>

void clrscr()
{
    system("@cls||clear");
}

int main()
{
    printf("Hello world!\n");
    clrscr();

    int num = kbhit();
    printf("%d\n", num);


    return 0;
}
