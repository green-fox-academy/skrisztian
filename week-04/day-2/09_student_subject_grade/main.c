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

student_t *new_student(const char *s_name, int s_age)
{
    // Create new student structure and grade array
    student_t *new_student = (student_t*) malloc(sizeof(student_t));

    // Setup structure values
    new_student->grades = (grade_t*) calloc(5, sizeof(grade_t));
    new_student->age = s_age;
    new_student->grade_count = 0;

    // Copy student name
    for (int i = 0; i < 128; i++) {
        new_student->name[i] = s_name[i];
    }

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

    // If we have more than 5 grades we turn it into a circular buffer
    if (student->grade_count > 4)
        student->grade_count = 0;

    return;
}

// TODO:
// Create a function that takes a Student as a pointer and returns the subject
// that the student has worst grade in
subject_t worst_subject(student_t *student)
{
    int min_pos = 0;
    int min_grade = 5;
    for (int i = 0; i < student->grade_count - 1; i++) {
        if (student->grades[i].value < min_grade) {
            min_grade = student->grades[i].value;
            min_pos = i;
        }
    }
    return student->grades[min_pos].subject;
}

// TODO:
// Create a function that deconstructs a Student
// It should take a pointer that points to the student
// It should free each Grade and the Student itself
void delete_student(student_t *student)
{
    free(student->grades);
    free(student);

    return;
}

int main()
{
    char subjects[][10] = {"MATH", "HISTORY", "ENGLISH", "SCIENCE"};

    student_t *paul = new_student("Paul Smith", 16);
    set_grade(paul, MATH, 3);
    set_grade(paul, HISTORY, 1);
    set_grade(paul, ENGLISH, 3);
    set_grade(paul, SCIENCE, 2);

    printf("Worst subject of %s: %s", paul->name, subjects[worst_subject(paul)]);

    delete_student(paul);

    return 0;
}
