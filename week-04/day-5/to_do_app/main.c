#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "to_do_app.h"

int main()
{
    // Setup variables
    const int max_tasks = 10;
    const int max_char = 256;
    int priority;
    char user_input[max_char];
    char user_input_cpy[max_char];
    char *command[4];

    // Create empty task list
    task_list_t *list_of_tasks = new_task_list(max_tasks);

    // Start
    print_usage();
    gets(user_input);
    strcpy(user_input_cpy, user_input);
    process_command(command, user_input);

    // Do action based on command

    // Do we have a valid command formatting?
    if (command[0][0] != '-') {
        // FIXME
        printf("\"%s\" is a not a valid, properly formatted command.\n", user_input_cpy);
    } else {

        // Add new task with priority
        if (strcmp(command[0], "-a") == 0)  {

            // If we have don't have 2 double quotes, the message cannot be interpreted
            if (atoi(command[3]) != 2) {
                // FIXME
                printf("\"%s\" is a not a valid, properly formatted command. Mind your double quotes!\n", user_input_cpy);
            } else {

                // Set priority to 0 if nothing given
                if (command[2] == NULL) {
                    priority = 0;
                } else {
                    priority = atoi(command[3]);
                }

                // Create the task
                task_t *task = new_task(command[1], max_char, priority);

                // Add task to task list. Return error if task list is full
                if (add_task_to_list(task, list_of_tasks) == 1) {
                    // FIXME
                    printf("Cannot add new task, your task list is full.\n");
                }

            }
        } else if (strcmp(command[0], "-wr") == 0) {
            // FIXME
            ;
        } else if (strcmp(command[0], "-rd") == 0) {
            // FIXME
            ;
        } else if (strcmp(command[0], "-l") == 0) {
            // FIXME
            ;
        } else if (strcmp(command[0], "-e") == 0) {
            // FIXME
            ;
        }
    }


/*


    printf("command: %s\n", command[0]);
    printf("param 1: %s\n", command[1]);
    printf("param 2: %s\n", command[2]);
    printf("quotes: %s\n", command[3]);

*/

    return 0;
}
