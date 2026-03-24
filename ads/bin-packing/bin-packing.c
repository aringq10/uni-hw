#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bin-packing.h"

int compareDecr(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

int isTimeout(clock_t start, int timeout) {
    if (timeout < 0) return 0;

    clock_t now = clock();
    double elapsedMs = (double)(now - start) * 1000.0 / CLOCKS_PER_SEC;

    return elapsedMs >= timeout;
}

void Solver_init(Solver* solver) {
    solver->volumes = NULL;
    solver->capacity = 0;
    solver->n = 0;
    solver->mode = HEURISTIC;
    solver->heuristic = 1;
    solver->isTimeout = NULL;
    solver->timeout_ms = -1;
    solver->start_time = 0;
    solver->timed_out = 0;
    solver->solution_found = 0;
}

int firstFit(Solver *s) {
    s->start_time = clock();
    int *volumes = s->volumes;
    int capacity = s->capacity;
    int n = s->n;
    int bins[MAX_N] = {0};
    int binCount = 0;

    for (int i = 0; i < n; i++) {
        int placed = 0;

        for (int j = 0; j < binCount; j++) {
            if (s->isTimeout(s->start_time, s->timeout_ms)) {
                s->timed_out = 1;
                s->solution_found = 0;
                return 0;
            };
            if (bins[j] + volumes[i] <= capacity) {
                bins[j] += volumes[i];
                placed = 1;
                break;
            }
        }

        if (!placed) {
            bins[binCount++] = volumes[i];
        }
    }

    s->timed_out = 0;
    s->solution_found = 1;
    return binCount;
}

int firstFitDecreasing(Solver *s) {
    qsort(s->volumes, s->n, sizeof(int), compareDecr);
    return firstFit(s);
}

void solve(Solver *solver) {
    if (solver->isTimeout == NULL) {
        solver->isTimeout = isTimeout;
    }
    if (solver->volumes == NULL) {
        fprintf(stderr, "No volumes array provided\n");
        return;
    }
    int result;
    switch (solver->mode) {
        case FIRST_MATCH:
            solver->solution_found = 0;
            solver->timed_out = 0;
            printf("First match search not implemented\n");
            break;
        case HEURISTIC:
            if (solver->heuristic == 1) {
                result = firstFitDecreasing(solver);
            } else {
                result = firstFit(solver);
            }
            break;
        case FULL:
            solver->solution_found = 0;
            solver->timed_out = 0;
            printf("Full search not implemented\n");
            break;
        default:
            fprintf(stderr, "Unknown mode search type: %d\n", solver->mode);
            return;
    }

    if (solver->timed_out) {
        printf("Algorithm timed out\n");
    } else if (solver->solution_found) {
        printf("Minimum boxes: %d\n", result);
    } else printf("No solution found\n");
}
