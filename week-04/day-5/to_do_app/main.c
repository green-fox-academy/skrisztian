#include <stdio.h>
#include <stdlib.h>
#include "to_do_app.h"





int main()
{
    char user_input[256];
    print_usage();
    gets(user_input);
    clrscr();

    puts("Task: ");
    gets(user_input);

    task_t *task1 = new_task(user_input, 256, 3);
    print_task(task1);


    return 0;
}
