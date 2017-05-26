#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char todo[256];
    short priority;
    short checked;
}task_t;

typedef struct {
    task_t **tasks;
    int task_count;
    int max_count;
}task_list_t_pointer;

typedef struct {
    task_t *tasks;
    int task_count;
    int max_count;
}task_list_t_normal;

task_t *new_task(char *description, short priority, short checked)
{
    // Create task in memory
    task_t* new_task = (task_t*) malloc(sizeof(task_t));

    // Set data
    new_task->priority = priority;
    new_task->checked = checked;
    strcpy(new_task->todo, description);

    return new_task;
}

task_list_t_pointer *new_task_list_pointer(int max_count)
{
    // Initialize the pointer to the task list object
    task_list_t_pointer *new_task_list;

    // Reserve memory for the structure
    new_task_list = (task_list_t_pointer*) malloc(sizeof(task_list_t_pointer));

    // Create array of task pointers
    new_task_list->tasks = (task_t**) calloc(max_count, sizeof(task_t*));

    // Set up parameters
    new_task_list->task_count = max_count;
    new_task_list->max_count = max_count;

    return new_task_list;
}

task_list_t_normal *new_task_list_normal(int max_count)
{
    // Initialize the pointer to the task list object
    task_list_t_normal *new_task_list;

    // Reserve memory for the structure
    new_task_list = (task_list_t_normal*) malloc(sizeof(task_list_t_normal));

    // Create array of task pointers
    new_task_list->tasks = (task_t*) calloc(max_count, sizeof(task_t));

    // Set up parameters
    new_task_list->task_count = max_count;
    new_task_list->max_count = max_count;

    return new_task_list;
}

void delete_task_pointer(task_list_t_pointer *task_list, int position)
{
    // Delete task at position from memory
    free(task_list->tasks[position]);

    // Move all pointers beyond position one cell ahead
    for (int i = position + 1; i < task_list->task_count; i++) {
        task_list->tasks[i - 1] = task_list->tasks[i];
    }

    // Decrease task count by one
    task_list->task_count--;

    return;
}

void delete_task_normal(task_list_t_normal *task_list, int position)
{
    // Move all pointers beyond position one cell ahead
    for (int i = position + 1; i < task_list->task_count; i++) {
        task_list->tasks[i - 1] = task_list->tasks[i];
    }

    // Decrease task count by one
    task_list->task_count--;

    return;
}


int main()
{
    const int count = 2000000;

    // Create two task lists with count elements
    task_list_t_normal *tl_normal = new_task_list_normal(count);
    task_list_t_pointer *tl_pointer = new_task_list_pointer(count);

    for (int i = 0; i < count; i++) {

        task_t *task1 = new_task("Walk the dog and then take down the rubbish too, and don't forget to cook dinner either and then study hard.", 1, 0);
        tl_pointer->tasks[i] = task1;

        task_t *task2 = new_task("Walk the dog and then take down the rubbish too, and don't forget to cook dinner either and then study hard.", 1, 0);
        tl_normal->tasks[i] = *task2;
    }

    printf("Number of tasks: %d\n", count);

    // Delete first task and measure time
    clock_t begin;
    clock_t end;
    double time_spent;

    begin = clock();
    delete_task_normal(tl_normal, 0);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time for normal delete: %f\n", time_spent);

    begin = clock();
    delete_task_pointer(tl_pointer, 0);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time for pointer delete: %f\n", time_spent);

    // FIXME: Garbage collection is not implemented

    return 0;
}

