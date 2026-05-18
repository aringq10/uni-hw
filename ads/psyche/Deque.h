#ifndef DEQUE_H
#define DEQUE_H

typedef struct {
    int *data;
    int size;
    int front;
    int rear;
    int capacity;
} Deque;

void initDeque(Deque *d, int capacity);
int pushBack(Deque *d, int value);
int pushFront(Deque *d, int value);
int popBack(Deque *d);
int popFront(Deque *d);
void freeDeque(Deque *d);

#endif