#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Stack {
    int size;
    int capacity;
    int* data;
};

Stack* Create() {
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (s == NULL) return NULL;

    s->size = 0;
    s->capacity= 0;
    s->data = NULL;
    return s;
}

void Done(Stack* s) {
    free(s->data);
    free(s);
};

int count(Stack* s) {
    return s->size;
};

int capacity(Stack* s) {
    return s->capacity;
};

int isEmpty(Stack* s) {
    return s->size == 0;
};

int isFull(Stack* s) {
    return s->size == s->capacity;
};

char* toString(Stack* s) {
    if (s->data == NULL || s->size == 0) {
        char* z = malloc(sizeof(char));
        if (z == NULL) return NULL;
        *z = '\0';
        return z;
    }

    char* buffer = malloc(1024 * sizeof(char));
    if (buffer == NULL) return NULL;
    int length = 0;
    for (int i = 0 ; i < s->size ; i++) {
        if (length >= 1024 - 1) {
            break;
        }
        length += sprintf(buffer + length, "%d ", s->data[i]);
    }
    buffer[length] = '\0';

    return buffer;
};

void push(Stack* s, int element) {
    if (isFull(s)) {
        s->capacity = (s->capacity == 0) ? 4 : s->capacity * 2;

        int* tmp = realloc(s->data, s->capacity* sizeof(int));
        if (tmp == NULL) return;
        s->data = tmp;
    }
    s->data[s->size] = element;
    s->size++;
};

int* pop(Stack* s) {
    if (s->size == 0) return NULL;
    s->size--;
    return &(s->data[s->size]);
};

Stack* clone(Stack* s) {
    Stack* tmpStack = Create();
    if (tmpStack == NULL) return NULL;

    tmpStack->data = malloc(sizeof(int) * s->capacity);
    if (tmpStack->data == NULL) return NULL;

    tmpStack->size = s->size;
    tmpStack->capacity = s->capacity;

    memcpy(tmpStack->data, s->data, s->capacity * sizeof(int));
    return tmpStack;
};

void makeEmpty(Stack* s) {
    s->size = 0;
};
