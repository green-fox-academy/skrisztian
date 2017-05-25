#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "to_do_app.h"

// Global variables are defined in to_do_app.h

int main()
{
    // Setup variables
    const int max_task = 10;
    const int max_char = 256;

    int priority;
    char user_input[max_char];
    char command[4][256];

    // Create empty task list
    task_list_t *list_of_tasks;
    list_of_tasks = new_task_list(max_task);

    // Set start parameter
    strcpy(command[0], "");
    //command[0] = NULL;

    while(1) {
        // Start
        if (strcmp(command[0], "") == 0) {
            print_usage(NULL);
            gets(user_input);
            process_command(command, user_input);
            // debug_command(command);

        // Do we have a valid command formatting?
        } else if (command[0][0] != '-') {
            print_usage("This was not a valid or properly formatted command");
            gets(user_input);
            process_command(command, user_input);

        // Actions
        } else {

            // Add new task with priority
            if (strcmp(command[0], "-a") == 0)  {

                // If we have don't have 2 double quotes, the message cannot be interpreted
                if (atoi(command[3]) != 2) {
                    print_usage("No task created. Mind your double quotes!");
                    gets(user_input);
                    process_command(command, user_input);

                } else {

                    // Set priority to 0 if nothing given
                    if (strcmp(command[2], "") == 0) {
                        priority = 0;
                    } else {
                        priority = atoi(command[2]);
                    }

                    // Create the task
                    task_t *task = new_task(command[1], max_char, priority);

                    // Return error if task list is full
                    if (add_task_to_list(task, list_of_tasks) == 1) {
                        print_usage("No task created. Your task list is full.");
                        gets(user_input);
                        process_command(command, user_input);

                    // Add was successful
                    } else {
                        print_usage("Task added.");
                        gets(user_input);
                        process_command(command, user_input);
                    }
                }

            // Write list to file
            } else if (strcmp(command[0], "-wr") == 0) {
                // FIXME
                ;

            // Read list from file
            } else if (strcmp(command[0], "-rd") == 0) {
                // FIXME
                ;

            // List all the tasks
            } else if (strcmp(command[0], "-l") == 0) {
                list_tasks(list_of_tasks, ALL);
                gets(user_input);
                process_command(command, user_input);

            // Empty task list
            } else if (strcmp(command[0], "-e") == 0) {

                // Delete the list, initialize a new one
                destroy_task_list(list_of_tasks);
                list_of_tasks = new_task_list(max_task);

                print_usage("Task list emptied. No todos for today! :)");
                gets(user_input);
                process_command(command, user_input);

            } else if (strcmp(command[0], "-r") == 0) {

                // FIXME

            } else if (strcmp(command[0], "-c") == 0) {

                // FIXME

            } else if (strcmp(command[0], "-p") == 0) {

                // FIXME

            } else if (strcmp(command[0], "-lp") == 0) {

                // FIXME

            // Exit from application
            } else if (strcmp(command[0], "-x") == 0) {
                clrscr();
                break;

            // For every other comment give an error
            } else {
                print_usage("This was not a valid or properly formatted command");
                gets(user_input);
                process_command(command, user_input);
            }
        }
    }

    return 0;
}
