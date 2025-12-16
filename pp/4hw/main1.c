/*
    Program for unit testing the SSList.h library
*/

#include <assert.h>
#include <stdio.h>
#include "SLList.h"

int intCompare(void *a, void *b) {
    return (*(int*)a - *(int*)b);
}

void printInt(void *a) {
    printf("%d", *(int *)a);
}

void printIntList(List *list) {
    printList(list, printInt, " -> ", "[", "]", 0);
}

int main() {
    List *list = listCreate();
    assert(list != NULL);
    assert(listSize(list) == 0);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    printf("Inserting values 1, 2 and 3 at indices 0, 1 and 2\n");
    assert(listInsert(list, 0, &a, sizeof(int)) == 1);
    assert(listInsert(list, 1, &b, sizeof(int)) == 1);
    assert(listInsert(list, 2, &c, sizeof(int)) == 1);
    assert(listSize(list) == 3);
    printIntList(list);

    printf("Removing first element\n");
    assert(listRemoveFirst(list) == 1);
    assert(listSize(list) == 2);
    printIntList(list);

    int x = 69;
    int y = 68;

    printf("Inserting values 69, 68 at start\n");
    assert(listInsertStart(list, &x, sizeof(int)) == 1);
    assert(listInsertStart(list, &y, sizeof(int)) == 1);
    assert(listSize(list) == 4);
    printIntList(list);

    printf("Removing last element\n");
    assert(listRemoveLast(list) == 1);
    assert(listSize(list) == 3);
    printIntList(list);

    void *data = NULL;

    assert(listGet(list, 0, &data) == 1);
    assert(*(int *)data == 68);

    assert(listGet(list, 1, &data) == 1);
    assert(*(int *)data == 69);

    assert(listGet(list, 2, &data) == 1);
    assert(*(int *)data == 2);

    printf("Inserting 4 at index 1\n");
    assert(listInsert(list, 1, &d, sizeof(int)) == 1);
    assert(listSize(list) == 4);
    printIntList(list);

    assert(listGet(list, 1, &data) == 1);
    assert(*(int *)data == 4);

    printf("Removing element at index 2\n");
    assert(listRemove(list, 2) == 1);
    assert(listSize(list) == 3);
    printIntList(list);

    assert(listGet(list, 2, &data) == 1);
    assert(*(int *)data == 2);

    printf("Setting element at index 1 to 99\n");
    int e = 99;
    assert(listSet(list, 1, &e, sizeof(int)) == 1);
    assert(listGet(list, 1, &data) == 1);
    assert(*(int *)data == 99);
    printIntList(list);

    //
    // Passing invalid ranges and arguments
    //

    printf("Trying to pass invalid ranges and arguments\n");
    assert(listInsert(list, -1, &a, sizeof(int)) == 0);
    assert(listInsert(list, 10, &a, sizeof(int)) == 0);
    assert(listGet(list, -1, &data) == 0);
    assert(listGet(list, 10, &data) == 0);
    assert(listSet(list, -1, &a, sizeof(int)) == 0);
    assert(listSet(list, 10, &a, sizeof(int)) == 0);
    assert(listRemove(list, -1) == 0);
    assert(listRemove(list, 10) == 0);
    assert(listGet(NULL, 0, &data) == 0);
    assert(listInsert(NULL, 0, &a, sizeof(int)) == 0);
    assert(listRemove(NULL, 0) == 0);
    assert(listSet(NULL, 0, &a, sizeof(int)) == 0);
    assert(listSize(NULL) == 0);
    printIntList(list);

    // 
    // Compressing list
    //

    int array[10] = {3, 3, 3, 4, 4, 5, 6, 7, 2, 2};

    printf("Inserting values 3, 3, 3, 4, 4, 5, 6, 7, 2, 2 at end\n");
    listInsertArray(list, array, 10, sizeof(int), listSize(list));
    assert(listSize(list) == 13);
    printIntList(list);

    printf("Compressing list\n");
    assert(listCompress(list, intCompare));
    assert(listSize(list) == 9);
    printIntList(list);

    listDestroy(list);

    printf("\nAll unit tests passed successfully\n");

    return 0;
}

