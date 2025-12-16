#include "SLList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

List *listCreate() {
    List *list = malloc(sizeof(List));
    if (!list) return NULL;
    list->head = NULL;
    list->size = 0;
    return list;
}

void listDestroy(List *list) {
    if (!list) return;

    Node *cur = list->head;
    while (cur) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp->data);
        free(tmp);
    }
    free(list);
}

int listInsert(List *list, int index, void *data, size_t dataSize) {
    if (!list || index < 0 || index > list->size || !data || dataSize == 0) return 0;

    Node *node = malloc(sizeof(Node));
    if (!node) return 0;

    void *newData = malloc(dataSize);
    if (!newData) {
        free(node);
        return 0;
    }
    memcpy(newData, data, dataSize);
    node->data = newData;

    if (index == 0) {
        node->next = list->head;
        list->head = node;
    }
    else {
        Node *prev = listGetNode(list, index - 1);
        node->next = prev->next;
        prev->next = node;
    }

    list->size++;
    return 1;
}

int listInsertStart(List *list, void *data, size_t dataSize) {
    return listInsert(list, 0, data, dataSize);
}

int listInsertEnd(List *list, void *data, size_t dataSize) {
    return listInsert(list, listSize(list), data, dataSize);
}

int listInsertArray(List *list, void *array, size_t length, size_t elementSize, int index) {
    if (!list || !array || length == 0 || elementSize == 0) return 0;
    if (index < 0 || index > listSize(list)) return 0;

    for (size_t i = 0; i < length; i++) {
        if (!listInsert(list, index + i, array + i * elementSize, elementSize)) {
            return 0;
        }
    }
    return 1;
}

int listGet(List *list, int index, void **data) {
    Node *node = listGetNode(list, index);
    if (!node || !data) return 0;

    *data = node->data;
    return 1;
}

Node *listGetNode(List *list, int index) {
    if (!list || index < 0 || index >= list->size) return NULL;

    Node *cur = list->head;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }

    return cur;
}

int listSet(List *list, int index, void *data, size_t dataSize) {
    if (!list || index < 0 || index >= list->size || !data || dataSize == 0) return 0;

    Node *node = listGetNode(list, index);
    if (!node) return 0;

    void *newData = malloc(dataSize);
    if (!newData) return 0;
    memcpy(newData, data, dataSize);

    free(node->data);
    node->data = newData;

    return 1;
}

int listRemove(List *list, int index) {
    if (!list || index < 0 || index >= list->size) return 0;

    Node *toDelete;
    if (index == 0) {
        toDelete = list->head;
        list->head = toDelete->next;
    } else {
        Node *prev = listGetNode(list, index - 1);
        toDelete = prev->next;
        prev->next = toDelete->next;
    }

    free(toDelete->data);
    free(toDelete);
    list->size--;

    return 1;
}

int listRemoveFirst(List *list) {
    return listRemove(list, 0);
}

int listRemoveLast(List *list) {
    return listRemove(list, listSize(list) - 1);
}

int listCompress(List *list, CompareFunc cmp) {
    if (!list || !cmp) return 0;

    Node *current = list->head;
    int compressed = 0;

    while (current) {
        Node *runner = current;
        while (runner->next) {
            if (cmp(current->data, runner->next->data) == 0) {
                Node *toDelete = runner->next;
                runner->next = runner->next->next;
                free(toDelete->data);
                free(toDelete);
                list->size--;
                compressed++;
            } else {
                break;
            }
        }
        current = current->next;
    }

    return compressed;
}

int listSize(List *list) {
    if (!list) return 0;
    return list->size;
}

void printList(List *list, PrintFunc printCustom, char *separator, char *opener, char *closer, int indexing) {
    if (!list) {
        printf("List is NULL\n");
        return;
    }
    
    printf("%s", opener);
    Node *cur = list->head;
    int idx = 0;
    while (cur) {
        if (indexing) printf("[%d]", idx);
        printCustom(cur->data);
        if (cur->next) printf("%s", separator);
        cur = cur->next;
        idx++;
    }   
    printf("%s", closer);
    printf("\n");
}
