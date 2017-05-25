#ifndef TO_DO_APP_H_INCLUDED
#define TO_DO_APP_H_INCLUDED

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define NO 0
#define YES 1

typedef struct {
    char todo[256];
    short priority;
    short checked;
}task_t;

typedef struct {
    task_t **tasks;
    int task_count;
    int max_count;
}task_list_t;

// Creates a new task in memory
task_t *new_task(char *description, short priority, short checked);

// Deletes a task from the task list. Also removes task from memory.
void delete_task(task_list_t *task_list, int position);

// Prints a single task's type content for debugging
void print_task(task_t *task);

// Clears the screen
void clrscr(void);

// Creates task list structure in memory
task_list_t *new_task_list(int max_count);

// Deletes task list from memory
void delete_task_list(task_list_t *task_list);

// Prints usage info, with message
void print_usage(const char *message);

// Checks or un-checks a task
void check_task(task_t *task);

// Lists all tasks
void list_tasks(task_list_t *task_list);

// Creates task numbers array
int *new_numbers_array(int size);

// Deletes task numbers array from memory
void delete_numbers_array(int *numbers_array);

// Updates the priority of a task
void change_task_priority(task_t *task, short new_priority);

// Adds a task to a task list
int add_task_to_list(task_t *task, task_list_t *task_list);

// Deletes physically from the list and from memory deleted tasks
void cleanup_task_list(task_list_t *task_list);

//Turns user input into array with command and parameters
void process_command(char command[][256], char *user_input);

// Prints the content of the command buffer for debugging
void debug_command(char (*command)[256]);

// Checks if a given string consists of numbers only
int is_number(char *string);

// Writes to-do list into file
void write_file(char *file_name, task_list_t *task_list);

// Read to-do list from file
void read_file(char *file_name, task_list_t *task_list);

// Sorts the tasks in the task list in  order of priority
void sort_tasks_by_priority(task_list_t *task_list);


#endif // TO_DO_APP_H_INCLUDED
