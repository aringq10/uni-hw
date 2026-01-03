// commandMenu.h
// Public interface for running an interactive command menu and
// modifying a StudentList through STDIN

#ifndef COMMANDMENU_H
#define COMMANDMENU_H

#include "studentList.h"

void startPromptLoop();

// Print command selection menu.
static void printMenu();

// Read INT into `value`, return 1 on success.
static int readInt(int *value);

// Read a string into `dest`, return 1 on success.
static int readString(char *dest, size_t maxLen);

// Read a `Student struct` into `student`, return 1 on success.
static int readStudentInfo(Student *student);

#endif
