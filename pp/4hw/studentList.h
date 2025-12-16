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

// LIST
StudentList *studentListCreate();
void studentListDestroy(StudentList *slist);

// CREATE
int studentListInsert(StudentList *slist, int index, Student *student);
int studentListInsertArray(StudentList *slist, void *array, size_t length, int index);

// READ
int studentListGet(StudentList *slist, int index, Student **outStudent);

// UPDATE
int studentListSet(StudentList *slist, int index, Student *student);

// DELETE
int studentListRemove(StudentList *slist, int index);

// OTHER
int studentListSize(StudentList *slist);
void printStudentList(StudentList *slist);
void printStudentHandler(void *a);
void printStudent(Student *student, int idx);
int compareStudentId(void *a, void *b);

#endif
