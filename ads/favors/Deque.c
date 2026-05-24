#include <stdio.h>
#include <stdlib.h>
#include "Deque.h"

//Create() sukuria tuščią deką ir grąžina jį
//Precondition: -
//Postcondition: front = NULL, rear = NULL, count == 0

Deque Create(){
    Deque d;
    d.front = NULL;
    d.rear = NULL;
    d.count = 0;
    return d;
}

//Done() - sunaikina deką ir atlaisvina visą atmintį
//Precondition: d != NULL;
//Postcondition: front == NULL, rear == NULL, count == 0

void Done(Deque *d){
    Node *current = d->front;
    Node *next = NULL;

    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
    d->front=NULL;
    d->rear=NULL;
    d->count=0;
}

//count() - grąžina elementų skaičių deke

int count(Deque *d){
    return d->count;
}

//isEmpty() - patikrina ar dekas tuščias
//Grąžina 1(true) jei count == 0, kitaip 0 (false)

int isEmpty(Deque *d){
    return d->count == 0;
}

//isFull() - patikrina ar dekas pilnas
//Visada grąžina 0

int isFull(Deque *d){
    return 0;
}

//toString() - atspausdina deko turinį nuo front iki rear

void toString(Deque *d){
    if(isEmpty(d)){
        printf("Deque is empty\n");
        return;
    }

    Node *current = d->front;
    printf("front -> ");
    while(current!=NULL){
        printf("%d ", current->data);
        current = current -> next;
    }
    printf("<- rear\n");
}

//peekFront() - grąžina pirmojo elemento reikšmę jo neišimant
//grąžina -1 jei dekas tušcias

int peekFront(Deque *d){
    if(isEmpty(d)) 
        return -1;
    return d->front->data;
}

//peekRear() - grąžina paskutiniojo elemento reikšmę jo neišimant
//Grąžina -1, jei dekas tuščias

int peekRear(Deque *d){
    if(isEmpty(d)) 
        return -1;
    return d->rear->data;
}

//pushFront() - įterpia naują elementą deko pradžioje
//peekFront() == value, count++

void pushFront(Deque *d, int value){
    Node *n = malloc(sizeof(Node));
    if(n==NULL) 
        return;

    n->data = value;
    n->prev = NULL;
    n->next = d->front;

    if(isEmpty(d))
        d->rear = n;
    else
        d->front->prev = n;
    
    d->front = n;
    d->count++;
}

//pushRear() - iterpia naują elementą deko gale
//peekRear() == value, count++

void pushRear(Deque *d, int value){
    Node *n = malloc(sizeof(Node));
    if(n==NULL) 
        return;

    n->data = value;
    n->prev = d->rear;
    n->next = NULL;

    if(isEmpty(d))
        d->front = n;
    else
        d->rear->next = n;
    
    d->rear = n;
    d->count++;
}

//popFront() - išima ir grąžina elementą iš deko pradžios
//grąžina -1, jei dekas tuščias

int popFront(Deque *d){
    if(isEmpty(d)) 
        return -1;

    Node *delete = d->front;
    int value = delete->data;

    d->front = d->front->next;
    if(d->front == NULL) 
        d->rear = NULL;
    else 
        d->front->prev = NULL;

    free(delete);
    d->count--;
    return value;
}

//popRear() - išima ir grąžina elementą iš deko galo
//Grąžina -1, jei dekas tuščias

int popRear(Deque *d){
    if(isEmpty(d)) 
        return -1;

    Node *delete = d->rear;
    int value = delete->data;

    d->rear = d->rear->prev;
    if(d->rear == NULL) 
        d->front = NULL;
    else
        d->rear->next = NULL;
    
    free(delete);
    d->count--;
    return value;
}

//makeEmpty() - išvalo visus deko elementus

void makeEmpty(Deque *d){
    Done(d);
}

//clone() - sukuria tikslią originalo kopiją

Deque clone(Deque * d){
    Deque copy = Create();
    Node *current = d->front;
    while(current!=NULL){
        pushRear(&copy, current->data);
        current = current->next;
    }
    return copy;
}