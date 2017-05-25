#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "to_do_app.h"

task_t *new_task(char *description, int desc_size, short priority)
{

    // Create task in memory
    task_t* new_task = (task_t*) malloc(sizeof(task_t));

    // Set data
    new_task->priority = priority;
    new_task->checked = NO;
    new_task->active = YES;

    // Fill in task description
    for (int i = 0; i < desc_size; i++) {
        new_task->todo[i] = description[i];
        if (description[i] == '\0')
            break;
    }
    return new_task;
}

void delete_task(task_t *task)
{
    // Set flag to deleted
    task->active = NO;

    return;
}

void destroy_task(task_t *task)
{
    // Remove task data from memory
    free(task);
}

void debug_task(task_t *task)
{
    printf("Task content: %s\n", task->todo);
    printf("Priority: %d\n", task->priority);
    printf("Checked: %d\n", task->checked);
    printf("Active: %d\n", task->active);

    return;
}

void clrscr(void)
{
    system("@cls||clear");

    return;
}

task_list_t *new_task_list(int max_count)
{
    // Initialize the pointer to the task list object
    task_list_t *new_task_list;

    // Reserve memory for the structure
    new_task_list = (task_list_t*) malloc(sizeof(task_list_t));

    // Create array of task pointers
    new_task_list->tasks = (task_t**) calloc(max_count, sizeof(task_t*));

    // Set up parameters
    new_task_list->task_count = 0;
    new_task_list->max_count = max_count;

    return new_task_list;
}

void delete_task_list(task_list_t *task_list)
{
    // Delete the array of task pointers from memory
    free(task_list->tasks);

    // Delete task lists object from memory
    free(task_list);

    return;
}

void destroy_task_list(task_list_t *task_list)
{
    // Delete all tasks from memory
    for (int i = 0; i < task_list->task_count; i++) {
        destroy_task(task_list->tasks[i]);
    }

    // Delete task list
    delete_task_list(task_list);

    return;
}

void print_usage(void)
{
    printf("                       Todo application\n");
    printf(" =================================================================\n");
    printf(" Commands:\n");
    printf("  -a \"task name\" <priority>    Adds a new task\n");
    printf("  -wr                          Write current todos to file\n");
    printf("  -rd                          Read todos from a file\n");
    printf("  -l                           Lists all the tasks\n");
    printf("  -e                           Empty the list\n");
    printf("  -r <task num>                Removes a task\n");
    printf("  -c <task num>                Completes a task\n");
    printf("  -p <priority>                Changes priority of a task\n");
    printf("  -lp                          Lists all the tasks by priority\n");
    printf("\n");
    printf("Enter command: ");

    return;
}

void check_task(task_t *task)
{
    if (task->checked)
        task->checked = NO;
    else
        task->checked = YES;

    return;
}

void list_tasks(task_list_t *task_list, short status)
{
    // Header
    printf(" Num\tCheck\tPri\tTo do\n");
    printf("\n");

    for (int i = 0; i < task_list->task_count; i++) {
       if (task_list->tasks[i]->active &&
            (task_list->tasks[i]->checked >= status)){

            // Task number
            printf("%3d\t[", i);

            // [x] or [ ]
            if (task_list->tasks[i]->checked)
                printf("x");
            else
                printf(" ");

            printf("]\t");

            // Priority
            if (task_list->tasks[i]->priority)
                printf("%d", task_list->tasks[i]->priority);
            else
                printf(" ");

            // To-do
            printf("\t%s\n", task_list->tasks[i]->todo);
        }
    }
    return;
}

int *new_numbers_array(int size)
{
    int *new_numbers = (int*) calloc(size, sizeof(int));

    return new_numbers;
}

void delete_numbers_array(int *numbers_array)
{
    free(numbers_array);

    return;
}

void change_task_priority(task_t *task, short new_priority)
{
    task->priority = new_priority;

    return;
}

int add_task_to_list(task_t *task, task_list_t *task_list)
{
    // The list might be full because of tasks flagged
    // for deletion. Remove them.
    if (task_list->task_count == task_list->max_count) {
        cleanup_task_list(task_list);

        //If still no space, return with error
        if (task_list->task_count == task_list->max_count)
            return 1;
    }

    // Add task to list
    task_list->tasks[task_list->task_count] = task;
    task_list->task_count++;

    return 0;
}

void cleanup_task_list(task_list_t *task_list)
{
    // Create a new empty task list
    task_list_t *clean_task_list = new_task_list(task_list->max_count);

    // Copy the active tasks into the new task list.
    // Skip and delete from memory inactive tasks.
    int j = 0;
    for (int i = 0; task_list->task_count; i++) {
        if(task_list->tasks[i]->active) {
            clean_task_list->tasks[j] = task_list->tasks[i];
            j++;
        } else {
            destroy_task(task_list->tasks[i]);
        }
    }

    // Set task count appropriately in new list
    clean_task_list->task_count = j;

    // Delete old task list from memory
    delete_task_list(task_list);

    // Make new task list the actual
    task_list = clean_task_list;

    return;
}

void process_command(char **command, char *user_input)
{
    // Count double quotes
    int quotes = 0;
    for (int i = 0; i < strlen(user_input); i++) {
        if (user_input[i] == 34) // double quotes
            quotes++;
    }

    // Convert to string
    char q_string[3];
    sprintf(q_string, "%d", quotes);
    strcpy(command[3], q_string);

    // Get command letter
    char *token;
    token = strtok(user_input, " ");
    command[0] = token;

    // First parameter
    token = strtok(NULL, "\"");
    command[1] = token;

    // Second parameter
    token = strtok(NULL, " ");
    command[2] = token;

    return;
}

/**************************************
TODO:

+error handling


Write to file
Read from file




list tasks by priority

**************************/

