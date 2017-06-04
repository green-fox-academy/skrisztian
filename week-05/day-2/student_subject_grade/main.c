#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {MATH, HISTORY, ENGLISH, SCIENCE} subject_t;

typedef struct {
    subject_t subject;
    int value;
} grade_t;

typedef struct {
    char name[128];
    int age;
    grade_t* grades;
    int grade_count;
} student_t;

// TODO:
// Create a function the constructs a Student (returns a pointer to a new student)
// It should take it's name and it's age as parameter
// It should allocate 5 grades
// It should set the grade_count to 0

student_t *create_student(char name[128], int age)
{
    // Create student
    student_t *new_student = (student_t*) malloc(sizeof(student_t));

    // Create array for grades
    new_student->grades = (grade_t*) calloc(5, sizeof(grade_t));

    // Fill in data
    strcpy(new_student->name, name);
    new_student->age = age;
    new_student->grade_count = 0;

    return new_student;
}

// TODO:
// Create a function that takes 3 parameters:
// A pointer to a Student
// A Subject
// An int (1-5)
// It should set the last grade's values with the given arguments

void set_grade(student_t *student, subject_t subject, int grade)
{
    student->grades[student->grade_count].subject = subject;
    student->grades[student->grade_count].value = grade;
    student->grade_count++;

    return;
}

// TODO:
// Create a function that takes a Student as a pointer and returns the subject
// that the student has worst grade in

subject_t worst_grade(student_t *student)
{
    subject_t worst_subject;
    int worst_mark = 5;

    for (int i = 0; i < student->grade_count; i++) {
        if (student->grades[i].value < worst_mark) {
            worst_mark = student->grades[i].value;
            worst_subject = student->grades[i].subject;
        }
    }

    return worst_subject;
}

// TODO:
// Create a function that deconstructs a Student
// It should take a pointer that points to the student
// It should free each Grade and the Student itself

void deconstruct_student(student_t *student)
{
    free(student->grades);
    free(student);

    return;
}

int main(void)
{
    char subjects[][10] = {"MATH", "HISTORY", "ENGLISH", "SCIENCE"};


    student_t *John = create_student("John Smith", 15);
    set_grade(John, MATH, 4);
    set_grade(John, HISTORY, 3);
    set_grade(John, ENGLISH, 5);

    printf("John is worst in %s\n", subjects[worst_grade(John)]);

    deconstruct_student(John);

    return 0;
}
