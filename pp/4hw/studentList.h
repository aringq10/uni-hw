// studentList.h
// Public interface for singly linked lists that store data of type Student,
// extends the SLList.h library

#ifndef STUDENTLIST_H
#define STUDENTLIST_H

#include "SLList.h"

#define MAX_NAME_LEN 50

typedef struct {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int studentId;
    int courseNo;
    int groupNo;
} Student;

typedef struct {
    List *list;
} StudentList;

//
// LIST
//

// Create a `StudentList`, return NULL on error.
StudentList *studentListCreate();

// Free `StudentList` and its nodes.
void studentListDestroy(StudentList *slist);

//
// CREATE
//

// Insert a node at `index` with value `student`.
// Returns 1 on success.
int studentListInsert(StudentList *slist, int index, Student *student);

// Insert an array at `index` with elements of type `Student`.
// Returns 1 on success.
int studentListInsertArray(
    StudentList *slist,
    void *array,
    size_t length,
    int index
);

//
// READ
//

// Get node at `index` and store its value into `*outStudent`.
// Returns 1 on success.
int studentListGet(StudentList *slist, int index, Student **outStudent);

//
// UPDATE
//

// Update the value of node at `index` to `student`.
// Returns 1 on success.
int studentListSet(StudentList *slist, int index, Student *student);

//
// DELETE
//

// Remove node at `index`.
// Returns 1 on success.
int studentListRemove(StudentList *slist, int index);

//
// OTHER
//

// Returns size of `slist`.
int studentListSize(StudentList *slist);

// Print each student's data separated by \n
void printStudentList(StudentList *slist);

// Handler function for printing student info.
void printStudentHandler(void *a);

// Print information for a single student with their `idx` in the list.
void printStudent(Student *student, int idx);

// Returns 0 if both students have the same Id.
int compareStudentId(void *a, void *b);

#endif
