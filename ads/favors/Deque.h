#ifndef DEQUE_H
#define DEQUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
    int data;
    struct Node *prev; //adresas mazgo kuris yra kaireje
    struct Node *next;    
}Node;

//saugo du adresus ir skaiciu
typedef struct{ //nebereikia pradzioje Deque nes viduje nenaudoja rodykles i save pacia
    Node *front;
    Node *rear;
    int count;
}Deque;

Deque Create(); //kuriamas tuscias dekas
void Done(Deque *d); //free() visi mazgai, dekas tampa tuscias

int count(Deque *d);
int isEmpty(Deque *d);
int isFull(Deque *d);
void toString(Deque *d);
int peekFront(Deque *d);
int peekRear(Deque *d);

void pushFront(Deque *d, int val);
void pushRear(Deque *d, int val);
int popFront(Deque *d);
int popRear(Deque *d);
void makeEmpty(Deque *d);
Deque clone(Deque *d);

#endif
