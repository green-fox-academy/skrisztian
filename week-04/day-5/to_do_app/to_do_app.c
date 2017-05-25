#include "to_do_app.h"

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

void delete_task(task_list_t *task_list, int position)
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

void debug_task(task_t *task)
{
    printf("Task content: %s\n", task->todo);
    printf("Priority: %d\n", task->priority);
    printf("Checked: %d\n", task->checked);

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
    // Delete all tasks from memory
    for (int i = 0; i < task_list->task_count; i++) {
        free(task_list->tasks[i]);
    }

    // Delete the array of task pointers from memory
    free(task_list->tasks);

    // Delete task lists object from memory
    free(task_list);

    return;
}

void print_usage(const char *message)
{
    clrscr();
    printf("                       Todo application\n");
    printf(" =================================================================\n");
    printf("\n");
    printf(" Commands:\n");
    printf("\n");
    printf("  -a \"task name\" <priority>    Adds a new task\n");
    printf("  -wr \"file name\"              Write current todos to file\n");
    printf("  -rd \"file name\"              Read todos from a file\n");
    printf("  -l                           List all the tasks\n");
    printf("  -e                           Empty the list\n");
    printf("  -r <task num>                Remove a task\n");
    printf("  -c <task num>                Complete a task\n");
    printf("  -p <task num> <priority>     Change priority of a task\n");
    printf("  -lp                          List all the tasks by priority\n");
    printf("\n");
    printf("  -x                           Exit from this application\n");
    printf("\n");

    if (message != NULL)
        printf("%s\n", message);

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

void list_tasks(task_list_t *task_list)
{
    clrscr();

    // Header
    printf("\n");
    printf(" Num\tCheck\tPri\tTo do\n");
    printf("\n");
    printf(" ------------------------------------\n");
    printf("\n");

    for (int i = 0; i < task_list->task_count; i++) {

        // Task number
        printf("%3d\t[", i +1);

        // [x] or [ ]
        if (task_list->tasks[i]->checked)
            printf("x");
        else
            printf(" ");

        printf("]\t");

        // Priority
        if (task_list->tasks[i]->priority == 32767)
            printf(" ");
        else
            printf("%d", task_list->tasks[i]->priority);

        // To-do
        printf("\t- %s\n", task_list->tasks[i]->todo);
    }
    printf("\n");
    printf("Enter command: ");
    return;
}

void change_task_priority(task_t *task, short new_priority)
{
    task->priority = new_priority;

    return;
}

int add_task_to_list(task_t *task, task_list_t *task_list)
{
    //If no space left return with error
    if (task_list->task_count == task_list->max_count) {
        free(task);
        return 1;
    }

    // Add task to list
    task_list->tasks[task_list->task_count] = task;
    task_list->task_count++;

    return 0;
}

void process_command(char (*command)[256], char *user_input)
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
    if (token != NULL)
        strcpy(command[0], token);
    else
        strcpy(command[0], "");

    // First parameter
    if (strcmp(command[0], "-p") == 0)
        token = strtok(NULL, " ");
    else
        token = strtok(NULL, "\"");

    if (token != NULL)
        strcpy(command[1], token);
    else
        strcpy(command[1], "");

    // Second parameter
    token = strtok(NULL, " ");
    if (token != NULL)
        strcpy(command[2], token);
    else
        strcpy(command[2], "");

#ifdef DEBUG
    gets(NULL);
    printf("command[0]: %s\n", command[0]);
    printf("command[1]: %s\n", command[1]);
    printf("command[2]: %s\n", command[2]);
    printf("command[3]: %s\n", command[3]);
    gets(NULL);
#endif // DEBUG

    return;
}

void debug_command(char (*command)[256])
{
    printf("\n");
    printf("* 0 - command: %s\n", command[0]);
    printf("* 1 - param 1: %s\n", command[1]);
    printf("* 2 - param 2: %s\n", command[2]);
    printf("* 3 - quotes: %s\n", command[3]);
    printf("\n");

    return;
}

int is_number(char *string)
{
    int not_digit = 0;
    for (int i = 0; i < strlen(string); i++) {
        not_digit += (!isdigit(string[i]));
    }
    return (!not_digit);
}

void write_file(char *file_name, task_list_t *task_list)
{
    FILE *file_p;
    file_p = fopen(file_name, "w");
    for (int i = 0; i < task_list->task_count; i++) {
        fprintf(file_p, "%d", task_list->tasks[i]->checked);
        fprintf(file_p, ";");
        fprintf(file_p, "%d", task_list->tasks[i]->priority);
        fprintf(file_p, ";");
        fprintf(file_p, "%s",task_list->tasks[i]->todo);
        fprintf(file_p, "\n");
    }
    fclose(file_p);

    return;
}

void read_file(char *file_name, task_list_t *task_list)
{
    // TODO: error handling at file operation

    FILE *file_p;
    file_p = fopen(file_name, "r");

    char buffer[256];

    while (fgets(buffer, 256, file_p)) {

        // Remove trailing \n
        strtok(buffer, "\n");

        char *token;
        char todo[256];

        token = strtok(buffer, ";");
        int checked = atoi(token);

        token = strtok(NULL, ";");
        int priority = atoi(token);

        token = strtok(NULL, ";");
        strcpy(todo, token);

        // TODO: add_task_to_list returns 1 if task list is full
        // That should be handled here.
        task_t *task = new_task(todo, priority, checked);
        add_task_to_list(task, task_list);
    }

    fclose(file_p);
    return;
}

void sort_tasks_by_priority(task_list_t *task_list)
{
    // Bubble sort
    for (int i = 0; i < task_list->task_count; i++) {
        for (int j = 0; j < task_list->task_count - i - 1; j++) {
            if (task_list->tasks[j]->priority > task_list->tasks[j + 1]->priority) {
                task_t *temp = task_list->tasks[j];
                task_list->tasks[j] = task_list->tasks[j + 1];
                task_list->tasks[j + 1] = temp;
            }
        }
    }
    return;
}
