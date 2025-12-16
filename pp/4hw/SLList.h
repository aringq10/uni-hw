#ifndef SLList_H
#define SLList_H

#include <stddef.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    int size;
} List;

typedef int (*CompareFunc)(void *a, void *b);
typedef void (*PrintFunc)(void *a);

// LIST
List *listCreate();
void listDestroy(List *list);

// CREATE
int listInsert(List *list, int index, void *data, size_t dataSize);
int listInsertStart(List *list, void *data, size_t dataSize);
int listInsertEnd(List *list, void *data, size_t dataSize);
int listInsertArray(List *list, void *array, size_t length, size_t elementSize, int index);

// READ
int listGet(List *list, int index, void **out);
Node *listGetNode(List *list, int index);

// UPDATE
int listSet(List *list, int index, void *data, size_t dataSize);

// DELETE
int listRemove(List *list, int index);
int listRemoveFirst(List *list);
int listRemoveLast(List *list);
int listCompress(List *list, CompareFunc cmp);

// OTHER
int listSize(List *list);
void printList(List *list, PrintFunc printCustom, char *separator, char *opener, char *closer, int indexing);

#endif
