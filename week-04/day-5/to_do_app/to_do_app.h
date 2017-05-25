#ifndef TO_DO_APP_H_INCLUDED
#define TO_DO_APP_H_INCLUDED

#define NO 0
#define YES 1

#define ALL 0
#define CHECKED 1

typedef struct {
    char todo[256];
    short priority;
    short checked;
    short active;
}task_t;

typedef struct {
    task_t **tasks;
    int task_count;
    int max_count;
}task_list_t;

// Creates a new task in memory
task_t *new_task(char *description, int desc_size, short priority);

// Flags a task as deleted. Does not remove it from memory.
void delete_task(task_t *task);

// Physically deletes a task from memory
void destroy_task(task_t *task);

// Prints a single task's type content for debugging
void print_task(task_t *task);

// Clears the screen
void clrscr(void);

// Creates task list structure in memory
task_list_t *new_task_list(int max_count);

// Deletes task list from memory
void delete_task_list(task_list_t *task_list);

// Deletes task list and tasks from memory
void destroy_task_list(task_list_t *task_list);

// Prints usage info, with message
void print_usage(const char *message);

// Checks or un-checks a task
void check_task(task_t *task);

// Lists all tasks or checked only tasks
void list_tasks(task_list_t *task_list, short status);

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
void debug_command(char *command);


#endif // TO_DO_APP_H_INCLUDED
