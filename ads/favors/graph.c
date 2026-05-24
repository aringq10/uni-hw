#include "graph.h"
#include "Deque.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Orientuotas grafas, kaimynystės sąrašais.
   adj[u] – Deque su visų v indeksais, kuriems u padarė paslaugą. */
struct Graph {
    char  **names;
    Deque  *adj;
    size_t  count;
    size_t  capacity;
};

static char *strDup(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy != NULL) {
        memcpy(copy, s, len);
    }
    return copy;
}

static bool graphReserve(Graph *g, size_t newCapacity) {
    if (newCapacity <= g->capacity) {
        return true;
    }

    char **newNames = realloc(g->names, newCapacity * sizeof(*newNames));
    if (newNames == NULL) {
        return false;
    }
    g->names = newNames;

    Deque *newAdj = realloc(g->adj, newCapacity * sizeof(*newAdj));
    if (newAdj == NULL) {
        return false;
    }
    g->adj = newAdj;

    for (size_t i = g->capacity; i < newCapacity; ++i) {
        g->names[i] = NULL;
        g->adj[i]   = Create();   /* tuščias dekas: front=rear=NULL, count=0 */
    }

    g->capacity = newCapacity;
    return true;
}

Graph *graphCreate(void) {
    Graph *g = calloc(1, sizeof(*g));
    if (g == NULL) {
        return NULL;
    }
    if (!graphReserve(g, 8)) {
        graphDestroy(g);
        return NULL;
    }
    return g;
}

void graphDestroy(Graph *g) {
    if (g == NULL) {
        return;
    }
    for (size_t i = 0; i < g->count; ++i) {
        free(g->names[i]);
        Done(&g->adj[i]);
    }
    free(g->names);
    free(g->adj);
    free(g);
}

int graphFindPerson(const Graph *g, const char *name) {
    if (g == NULL || name == NULL) {
        return -1;
    }
    for (size_t i = 0; i < g->count; ++i) {
        if (strcmp(g->names[i], name) == 0) {
            return (int)i;
        }
    }
    return -1;
}

int graphAddPerson(Graph *g, const char *name) {
    if (g == NULL || name == NULL || name[0] == '\0') {
        return -1;
    }
    int existing = graphFindPerson(g, name);
    if (existing >= 0) {
        return existing;
    }
    if (g->count == g->capacity) {
        size_t newCap = g->capacity == 0 ? 8 : g->capacity * 2;
        if (!graphReserve(g, newCap)) {
            return -1;
        }
    }
    char *copy = strDup(name);
    if (copy == NULL) {
        return -1;
    }
    int index = (int)g->count;
    g->names[g->count] = copy;
    /* adj[index] jau inicializuotas tuščiu deku graphReserve metu. */
    g->count++;
    return index;
}

bool graphAddFavor(Graph *g, const char *from, const char *to) {
    if (g == NULL || from == NULL || to == NULL) {
        return false;
    }
    int u = graphAddPerson(g, from);
    int v = graphAddPerson(g, to);
    if (u < 0 || v < 0) {
        return false;
    }
    pushRear(&g->adj[u], v);
    return true;
}

static bool dfsVisit(const Graph *g, int cur, int target, bool *visited) {
    if (cur == target) {
        return true;
    }
    visited[cur] = true;
    for (Node *n = g->adj[cur].front; n != NULL; n = n->next) {
        int nbr = n->data;
        if (!visited[nbr] && dfsVisit(g, nbr, target, visited)) {
            return true;
        }
    }
    return false;
}

bool graphCanExpectLoan(const Graph *g, const char *asker,
                        const char *target) {
    if (g == NULL || asker == NULL || target == NULL) {
        return false;
    }
    int start = graphFindPerson(g, asker);
    int goal  = graphFindPerson(g, target);
    if (start < 0 || goal < 0) {
        return false;
    }
    bool *visited = calloc(g->count, sizeof(*visited));
    if (visited == NULL) {
        return false;
    }
    bool result = dfsVisit(g, start, goal, visited);
    free(visited);
    return result;
}

size_t graphPersonCount(const Graph *g) {
    return g == NULL ? 0 : g->count;
}

const char *graphPersonName(const Graph *g, int index) {
    if (g == NULL || index < 0 || (size_t)index >= g->count) {
        return NULL;
    }
    return g->names[index];
}
