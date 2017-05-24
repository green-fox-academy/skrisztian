#include <string.h>
#include <stdint.h>
#include "to_do_app.h"

task_t *new_task(char *description, int desc_size, short priority){

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
    task->active = NO;

    return;
}

void print_task(task_t *task)
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
    task_list_t *new_task_list;

    new_task_list->tasks = (int*) calloc(max_count, sizeof(task_t));
    new_task_list->task_count = 0;
    new_task_list->max_count = max_count;

    return new_task_list;
}

void delete_task_list(task_list_t *task_list)
{
    // Delete all tasks from memory
    for (int i = 0; i < task_list->task_count; i++) {
        free(task_list->tasks[i]);
    }

    // Delete task lists
    free(task_list->tasks);
    free(task_list);

    return;
}

void print_usage(void)
{
    printf("Todo application\n");
    printf("====================\n");
    printf("Commands:\n");
    printf(" -a   Adds a new task\n");
    printf(" -ap  Adds a new task with priority\n");
    printf(" -wr  Write current todos to file\n");
    printf(" -rd  Read todos from a file\n");
    printf(" -l   Lists all the tasks\n");
    printf(" -e   Empty the list\n");
    printf(" -r   Removes a task\n");
    printf(" -c   Completes a task\n");
    printf(" -p   Change priority of a task\n");
    printf(" -lp  Lists all the tasks by priority\n");

    return;
}

/*copy to h */

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
    printf(" Num\tCheck\tPri\tTo do\n\n");

    for (int i = 0; i < task_list->task_count; i++) {
       if (task_list->tasks[i]->active &&
            (task_list->tasks[i].checked >= status)){

            printf("%3d\t[", i);

            if (task_list->tasks[i].checked)
                printf("x");
            else
                printf(" ");

            printf("]\t");

            if (task_list->tasks[i].priority)
                printf("%d", task_list->tasks[i].priority);
            else
                printf(" ");

            printf("\t%s\n", task_list->tasks[i].todo);
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
    // Check if we have space. We might have inactive tasks
    // in the list. Remove them.
    if (task_list->task_count == task_list->max_count) {
        cleanup_task_list(task_list);

        // Check space again. If still no space,
        // return with error
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
    task_list_t *new_task_list = new_task_list(task_list->max_count);

    // Copy the active tasks into the new task list
    // Skip and delete from memory inactive tasks
    int j = 0;
    for (int i = 0; task_list->task_count; i++) {
        if(task_list->tasks[i].active) {
            new_task_list->tasks[j] = task_list->tasks[i];
            j++;
        } else {
            free(task_list->tasks[i]);
        }
    }
    // Set task count appropriately in new list
    new_task_list->task_count = j;

    // Delete old task list from memory
    free(task_list);

    // Make new task list the actual
    task_list = new_task_list;

    return;
}



/**************************************
TODO:

+error handling


Write to file
Read from file




list tasks by priority

**************************/

