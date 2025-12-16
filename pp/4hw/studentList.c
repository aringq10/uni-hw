// studentList.c
// Implementation of functions in studentList.h

#include "studentList.h"
#include "SLList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

StudentList *studentListCreate() {
    StudentList *slist = malloc(sizeof(StudentList));
    if (!slist) return NULL;
    slist->list = listCreate();
    if (!slist->list) {
        free(slist);
        return NULL;
    }
    return slist;
}

void studentListDestroy(StudentList *slist) {
    if (!slist) return;

    listDestroy(slist->list);
    free(slist);
}

int studentListInsert(StudentList *slist, int index, Student *student) {
    if (!slist || !student) return 0;
    return listInsert(slist->list, index, student, sizeof(Student));
}

int studentListInsertArray(
    StudentList *slist,
    void *array,
    size_t length,
    int index
) {
    if (!slist) return 0;
    return listInsertArray(slist->list, array, length, sizeof(Student), index);
}

int studentListGet(StudentList *slist, int index, Student **outStudent) {
    if (!slist) return 0;
    return listGet(slist->list, index, (void **)outStudent);
}

int studentListSet(StudentList *slist, int index, Student *student) {
    if (!slist || !student) return 0;
    return listSet(slist->list, index, student, sizeof(Student));
}

int studentListRemove(StudentList *slist, int index) {
    if (!slist) return 0;
    return listRemove(slist->list, index);
}

int studentListSize(StudentList *slist) {
    if (!slist) return 0;
    return listSize(slist->list);
}

void printStudentList(StudentList *slist) {
    if (!slist) return;
    printf("Student list:\n");
    printList(slist->list, printStudentHandler, "\n", "", "", 1);
}

void printStudentHandler(void *a) {
    Student *s = (Student *)a;
    if (!s) return;
    printf("%s %s | ID: %d | Course: %d | Group: %d",
        s->firstName, s->lastName, s->studentId, s->courseNo, s->groupNo);

}

void printStudent(Student *s, int idx) {
    if (!s) return;
    printf("[%d] ", idx);
    printStudentHandler((void *)s);
    printf("\n");
}

int compareStudentId(void *a, void *b) {
    return (((Student*)a)->studentId - ((Student*)b)->studentId);
}
