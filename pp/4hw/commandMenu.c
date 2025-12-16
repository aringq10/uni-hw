#include "commandMenu.h"
#include "SLList.h"
#include "studentList.h"
#include <stdio.h>
#include <string.h>

static void printMenu();
static int readInt(int *value);
static int readString(char *dest, size_t maxLen);
static int readStudentInfo(Student *student);

Student students[] = {
    {"John",  "Doe",    1001, 1, 101},
    {"Jane",  "Doe",    1001, 1, 102},

    {"Alice", "Smith",  1002, 1, 103},
    {"Bob",   "Brown",  1003, 2, 201},
    {"Eve",   "White",  1004, 2, 202},

    {"Tom",   "Black",  2001, 3, 301},
    {"Anna",  "Black",  2001, 3, 302},
    {"Mike",  "Black",  2001, 3, 303}
};

void startPromptLoop() {
    StudentList *list = studentListCreate();
    if (!list) {
        printf("Failed to create student list\n");
        return;
    }
    studentListInsertArray(list, students, 8, 0);
    printStudentList(list);

    int running = 1;
    while (running) {
        printMenu();
        int cmd;
        if (readInt(&cmd) != 1) {
            printf("\nInvalid input, try again\n");
            continue;
        }

        switch (cmd) {
            case 1: {
                int index;
                Student value;
                printf("Index: ");
                if (readInt(&index) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Student Details:\n");
                if (readStudentInfo(&value) != 1) {
                    printf("Invalid input\n");
                    break;
                }

                if (studentListInsert(list, index, &value)) {
                    printf("Inserted student at index %d:\n", index);
                    printStudent(&value, index);
                    printStudentList(list);
                }
                else {
                    printf("Insert failed (invalid index)\n");
                }
                break;

            }
            case 2: {
                int index;
                printf("Index: ");
                if (readInt(&index) != 1) {
                    printf("Invalid input\n");
                    break;
                }

                if (studentListRemove(list, index)) {
                    printf("Removed element at index %d\n", index);
                    printStudentList(list);
                }
                else {
                    printf("Remove failed (invalid index)\n");
                }
                break;
            }
            case 3: {
                int index;
                Student value;
                printf("Index: ");
                if (readInt(&index) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Student Details:\n");
                if (readStudentInfo(&value) != 1) {
                    printf("Invalid input\n");
                    break;
                }
 
                if (studentListSet(list, index, &value)) {
                    printf("Set element at index %d to:\n", index);
                    printStudent(&value, index);
                    printStudentList(list);
                }
                else {
                    printf("Set failed (invalid index)\n");
                }
                break;
            }
            case 4: {
                printStudentList(list);
                break;
            }
            case 5: {
                int index;
                printf("Index: ");
                if (readInt(&index) != 1) {
                    printf("Invalid input\n");
                    break;
                }

                Student *value;
                if (studentListGet(list, index, &value)) {
                    printf("Element at index %d:\n", index);
                    printStudent(value, index);
                } else {
                    printf("Get failed (invalid index)\n");
                }
                break;
            }
            case 6: {
                int removed = listCompress(list->list, compareStudentId);
                printf("Removed %d elements\n", removed);
                printStudentList(list);
                break;
            }
            case 0: {
                running = 0;
                break;
            }
            default: {
                printf("Unknown command\n");
            }
        }
    }

    studentListDestroy(list);
    printf("List destroyed, exiting\n");
    return;
}

static void printMenu() {
    printf("\n-----Command Menu-----\n");
    printf("1 - Insert at index\n");
    printf("2 - Remove at index\n");
    printf("3 - Set value at index\n");
    printf("4 - Print list\n");
    printf("5 - Get value at index\n");
    printf("6 - Compress list\n");
    printf("0 - Exit\n");
    printf("Enter command: ");
}

static int readInt(int *value) {
    if (!value) return 0;

    int result = 0;
    char buffer[100];

    if (fgets(buffer, sizeof(buffer), stdin)) {
        result = sscanf(buffer, "%d", value);
    }

    return result;
}

static int readString(char *dest, size_t maxLen) {
    if (!dest || maxLen == 0) return 0;

    char buffer[1024];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 0;
    }

    size_t len = strcspn(buffer, "\n");
    buffer[len] = '\0';

    if (len == 0) return 0;

    if (len >= maxLen) {
        len = maxLen - 1;
    }
    strncpy(dest, buffer, len);
    dest[len] = '\0';

    return 1;
}

static int readStudentInfo(Student *student) {
    if (!student) return 0;

    printf("Enter first name: ");
    if (readString(student->firstName, MAX_NAME_LEN) != 1) {
        return 0;
    }

    printf("Enter last name: ");
    if (readString(student->lastName, MAX_NAME_LEN) != 1) {
        return 0;
    }

    printf("Enter student ID: ");
    if (readInt(&student->studentId) != 1) {
        return 0;
    }

    printf("Enter course number: ");
    if (readInt(&student->courseNo) != 1) {
        return 0;
    }

    printf("Enter group number: ");
    if (readInt(&student->groupNo) != 1) {
        return 0;
    }

    return 1;
}


