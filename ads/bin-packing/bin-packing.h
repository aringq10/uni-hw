#ifndef BIN_PACKING_H
#define BIN_PACKING_H

#include <time.h>

#define MAX_N 1000

typedef enum {
    FULL,
    FIRST_MATCH,
    HEURISTIC
} SearchMode;

typedef struct {
    int* volumes;
    int capacity, n;

    SearchMode mode;
    int heuristic;
    int (*isTimeout)(clock_t start, int timeout);
    int timeout_ms;
    clock_t start_time;
    int timed_out;
    int solution_found;
} Solver;

void Solver_init(Solver* solver);

int firstFit(Solver *s);
int firstFitDecreasing(Solver *s);
void solve(Solver *solver);

#endif
