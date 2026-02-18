#ifndef STACK_H
#define STACK_H

/*  APRAŠYMAS

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Stack Stack;

/*  KONSTRUKTORIUS
    Sukuria tuščia naują int tipo steką
*/
Stack* Create();

/*  DESTRUKTORIUS
    Atlaisvina (free) steką
*/ 
void Done(Stack* s);

// --------------- UŽKLAUSOS --------------

// Grąžina steko elementų skaičių
int count(Stack* s);

// Grąžina steko talpą
int capacity(Stack* s);

// Grąžina True jeigu stekas yra tuščias
int isEmpty(Stack* s);

// Grąžina True jeigu elementų skaičius lygus alokuotos vietos skaičiui
int isFull(Stack* s);

// Grąžina steko elementus paverstus į char'o masyvą (būtina atlaisvinti masyva kai nebereikės (free))
char* toString(Stack* s);

// --------------- KOMANDOS ---------------

//   Įterpia elementą į steko viršų
void push(Stack* s, int element);

//   Grąžina elementą nuo steko viršaus
int* pop(Stack* s);

//   Sukuria pilną steko kopiją
Stack* clone(Stack* s);

//   Išvalo steko subkomponentus
void makeEmpty(Stack* s);

#endif

