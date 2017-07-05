#include "userfunctions.h"

void clrscr(void)
{
    system("@cls||clear");

    return;
}

void print_help(void)
{
    clrscr();
    printf("TOTORO CHAT\n");
    printf("====================\n");
    printf("Commands:\n");
    printf(" h   Display this help info\n");
    printf(" x   Exit\n");
    printf(" n   Set user name\n");
    printf(" l   List known users\n");
    printf(" d   Send discovery request\n");
    printf(" m   Send message\n");
    printf(" c   Clear screen\n");

    return;
}


