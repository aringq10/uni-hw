#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stddef.h>

/* ADT: orientuotas grafas, realizuotas kaimynystės sąrašais.
   Briauna u -> v reiškia: u padarė paslaugą v.                  */
typedef struct Graph Graph;

Graph *graphCreate(void);
void   graphDestroy(Graph *g);

/* Prideda žmogų jei dar nėra. Grąžina indeksą arba -1. */
int graphAddPerson(Graph *g, const char *name);

/* -1 jei tokio žmogaus nėra. */
int graphFindPerson(const Graph *g, const char *name);

/* „from" padarė paslaugą „to" — abu vardai prireikus pridedami.
   Abipusės paslaugos ir savicikai leidžiami.                    */
bool graphAddFavor(Graph *g, const char *from, const char *to);

/* DFS pasiekiamumas: ar „asker" gali tikėtis, kad „target"
   paskolins? Jei target nepasiekiamas iš asker — false.          */
bool graphCanExpectLoan(const Graph *g, const char *asker,
                        const char *target);

size_t      graphPersonCount(const Graph *g);
const char *graphPersonName (const Graph *g, int index);

#endif
