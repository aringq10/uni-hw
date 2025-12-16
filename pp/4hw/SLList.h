// SLList.h
// Public interface for working with singly linked lists

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

// Initiate a `List` and return it, otherwise return NULL.
List *listCreate();

// Free all nodes in a list and their corresponding data.
void listDestroy(List *list);

// Inserts an element into a list at `index`.
// Returns the number of elements inserted.
int listInsert(List *list, int index, void *data, size_t dataSize);

// Adds an element to the start of the list.
// Returns the number of elements inserted.
int listInsertStart(List *list, void *data, size_t dataSize);

// Adds an element to the end of the list.
// Returns the number of elements inserted.
int listInsertEnd(List *list, void *data, size_t dataSize);

// Inserts an array of elements into a list at `index`.
// Returns 1 if all elements were inserted successfully, 0 otherwise.
int listInsertArray(
    List *list,
    void *array,
    size_t length,
    size_t elementSize,
    int index
);

// Get the node at `index` and store its data pointer into *out.
int listGet(List *list, int index, void **out);

// Return pointer to a node at `index`.
Node *listGetNode(List *list, int index);

// Update the data of the node at `index`.
int listSet(List *list, int index, void *data, size_t dataSize);

//
// DELETE FUNCTIONS
//

// Remove node at `index`.
int listRemove(List *list, int index);

// Remove first node.
int listRemoveFirst(List *list);

// Remove last node.
int listRemoveLast(List *list);

// Remove consecutive duplicate elements, leaving only one instance.
int listCompress(List *list, CompareFunc cmp);

//
// OTHER
//

// Return the list size.
int listSize(List *list);

// Print each of list's elements using `printCustom`.
// `indexing` is a flag that prints [idx] before every element.
void printList(
    List *list,
    PrintFunc printCustom,
    char *separator,
    char *opener,
    char *closer,
    int indexing
);

#endif
