#ifndef STACK_H
#define STACK_H

/*  APRAŠYMAS
    Biblioteka skirta IntStack duomenų tipui sukurti, keisti,
    daryti užklausas ir panaikinti.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct IntStack IntStack;

// KONSTRUKTORIUS
IntStack* Create();

// DESTRUKTORIUS
void Done(IntStack* s);

// Grąžina steko elementų skaičių
int count(IntStack* s);

// Grąžina steko talpą
int capacity(IntStack* s);

// Grąžina True jeigu stekas yra tuščias
int isEmpty(IntStack* s);

// Grąžina True jeigu elementų skaičius lygus alokuotos vietos skaičiui
int isFull(IntStack* s);

// Grąžina steko elementus paverstus į char'o masyvą (būtina atlaisvinti masyva kai nebereikės (free))
char* toString(IntStack* s);

// Įterpia elementą į steko viršų
int push(IntStack* s, int element);

// Grąžina elementą nuo steko viršaus
int pop(IntStack* s, int* p);

// Sukuria pilną steko kopiją
IntStack* clone(IntStack* s);

// Išvalo steko subkomponentus
void makeEmpty(IntStack* s);

#endif

