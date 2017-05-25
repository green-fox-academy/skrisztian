#include "to_do_app.h"

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

        // Do we have a valid command formatting?
        } else if (command[0][0] != '-') {
            print_usage("This was not a valid or properly formatted command.");
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

                    // Set priority to 32767 if nothing given
                    if (strcmp(command[2], "") == 0) {
                        priority = 32767;
                    } else {
                        priority = atoi(command[2]);
                    }

                    // Create the task
                    task_t *task = new_task(command[1], priority, 0);

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

                // Return error if filename is missing
                if (strcmp(command[1], "") == 0) {
                    print_usage("Unable to write file. Missing file name.");
                    gets(user_input);
                    process_command(command, user_input);

                // If we have don't have 2 double quotes, the message cannot be interpreted
                } else if (atoi(command[3]) != 2) {
                    print_usage("Unable to write file. Mind your double quotes!");
                    gets(user_input);
                    process_command(command, user_input);

                // Write to file
                } else {
                    write_file(command[1], list_of_tasks);
                    print_usage("File writing done.");
                    gets(user_input);
                    process_command(command, user_input);
                }

            // Read list from file
            } else if (strcmp(command[0], "-rd") == 0) {

                // Return error if filename is missing
                if (strcmp(command[1], "") == 0) {
                    print_usage("Unable to read file. Missing file name.");
                    gets(user_input);
                    process_command(command, user_input);

                // If we have don't have 2 double quotes, the message cannot be interpreted
                } else if (atoi(command[3]) != 2) {
                    print_usage("Unable to read file. Mind your double quotes!");
                    gets(user_input);
                    process_command(command, user_input);

                // Read from file
                } else {
                    read_file(command[1], list_of_tasks);
                    print_usage("File reading done. Content added to task list.");
                    gets(user_input);
                    process_command(command, user_input);
                }

            // List all the tasks
            } else if (strcmp(command[0], "-l") == 0) {
                list_tasks(list_of_tasks);
                gets(user_input);
                process_command(command, user_input);

            // Empty task list
            } else if (strcmp(command[0], "-e") == 0) {

                // Delete the list, initialize a new one
                delete_task_list(list_of_tasks);
                list_of_tasks = new_task_list(max_task);

                print_usage("Task list emptied. No todos for today! :)");
                gets(user_input);
                process_command(command, user_input);

            // Remove task
            } else if (strcmp(command[0], "-r") == 0) {

                // If command parameter is empty return error
                if (strcmp(command[1], "") == 0) {
                    print_usage("Unable to remove: No index is provided.");
                    gets(user_input);
                    process_command(command, user_input);

                // If command parameter is not a number, return error
                } else if (!(is_number(command[1]))) {
                    print_usage("Unable to remove. Index is not a number.");
                    gets(user_input);
                    process_command(command, user_input);

                } else {
                    int del_position = atoi(command[1]) - 1;

                    // If position is out of range return error
                    if ((del_position < 0) || (del_position > list_of_tasks->task_count - 1)) {
                        print_usage("Unable to remove. Index is out of bound.");
                        gets(user_input);
                        process_command(command, user_input);

                     // Delete task
                    } else {
                        delete_task(list_of_tasks, del_position);
                        print_usage("Task removed.");
                        gets(user_input);
                        process_command(command, user_input);
                    }
                }

            // Check or uncheck task
            } else if (strcmp(command[0], "-c") == 0) {
                // If command parameter is empty return error
                if (strcmp(command[1], "") == 0) {
                    print_usage("Unable to check: No index is provided.");
                    gets(user_input);
                    process_command(command, user_input);

                // If command parameter is not a number, return error
                } else if (!(is_number(command[1]))) {
                    print_usage("Unable to check. Index is not a number.");
                    gets(user_input);
                    process_command(command, user_input);

                } else {
                    int check_position = atoi(command[1]) - 1;

                    // If position is out of range return error
                    if ((check_position < 0) || (check_position > list_of_tasks->task_count - 1)) {
                        print_usage("Unable to check. Index is out of bound.");
                        gets(user_input);
                        process_command(command, user_input);

                     // Check task
                    } else {
                        short is_checked = list_of_tasks->tasks[check_position]->checked;
                        check_task(list_of_tasks->tasks[check_position]);
                        if (is_checked)
                            print_usage("Task unchecked.");
                        else
                            print_usage("Task checked.");
                        gets(user_input);
                        process_command(command, user_input);
                    }
                }

            // Change priority
            } else if (strcmp(command[0], "-p") == 0) {

                // If command's first parameter is empty return error
                if (strcmp(command[1], "") == 0) {
                    print_usage("Unable to change priority: No index is provided.");
                    gets(user_input);
                    process_command(command, user_input);

                // If command parameter is not a number, return error
                } else if (!(is_number(command[1]))) {
                    print_usage("Unable to change priority. Index is not a number.");
                    gets(user_input);
                    process_command(command, user_input);

                } else {
                    int pri_position = atoi(command[1]) - 1;

                    // If position is out of range return error
                    if ((pri_position < 0) || (pri_position > list_of_tasks->task_count - 1)) {
                        print_usage("Unable to change priority. Index is out of bound.");
                        gets(user_input);
                        process_command(command, user_input);

                     // Check second parameter
                    } else {
                        // If priority parameter is not a number, return error
                        if (!(is_number(command[2]))) {
                            print_usage("Unable to change priority. The given priority is not a number.");
                            gets(user_input);
                            process_command(command, user_input);

                        // If priority parameter is empty, set it to default
                        } if (strcmp(command[2], "") == 0) {
                            change_task_priority(list_of_tasks->tasks[pri_position], 32767);
                            print_usage("Removed task priority.");
                            gets(user_input);
                            process_command(command, user_input);


                        } else {
                            int new_priority = atoi(command[2]);

                            // If priority out of range give error
                            if ((new_priority < 1) || (new_priority > 32766)) {
                                print_usage("Unable to change priority. New priority is out of bound.");
                                gets(user_input);
                                process_command(command, user_input);
                            } else {
                                change_task_priority(list_of_tasks->tasks[pri_position], new_priority);
                                print_usage("Priority updated.");
                                gets(user_input);
                                process_command(command, user_input);
                            }
                        }
                    }
                }

            } else if (strcmp(command[0], "-lp") == 0) {
                sort_tasks_by_priority(list_of_tasks);
                list_tasks(list_of_tasks);
                gets(user_input);
                process_command(command, user_input);

            // Exit from application
            } else if (strcmp(command[0], "-x") == 0) {
                clrscr();
                break;

            // For every other comment give an error
            } else {
                print_usage("This was not a valid or properly formatted command.");
                gets(user_input);
                process_command(command, user_input);
            }
        }
    }

    return 0;
}
