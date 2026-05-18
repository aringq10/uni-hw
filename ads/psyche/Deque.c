#include <stdio.h>
#include <stdlib.h>
#include "Deque.h"

void initDeque(Deque *d, int capacity){
    d->data = (int*)malloc(capacity * sizeof(int));
    d->capacity = capacity;
    d->size = 0;
    d->front = 0;
    d->rear = -1;
}

int pushBack(Deque *d, int value){
    if(d->size == d->capacity) return 0;

    d->rear = (d->rear + 1) % d->capacity;
    d->data[d->rear] = value;
    d->size++;
    return 1;
}

int pushFront(Deque *d, int value){
    if(d->size == d->capacity) return 0;

    d->front = (d->front - 1 + d->capacity) % d->capacity;
    d->data[d->front] = value;
    d->size++;
    return 1;
}

int popBack(Deque *d){
    if (d->size == 0) return 0;

    int value = d->data[d->rear];
    d->rear = (d->rear - 1 + d->capacity) % d->capacity;
    d->size--;
    return value;
}

int popFront(Deque *d){
    if (d->size == 0) return 0;

    int value = d->data[d->front];
    d->front = (d->front + 1) % d->capacity;
    d->size--;
    return value;
}

void freeDeque(Deque *d){
    free(d->data);
}