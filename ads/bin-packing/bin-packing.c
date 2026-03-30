#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bin-packing.h"

Solver *solverInit(void) {
    Solver *s = malloc(sizeof(Solver));
    if (!s) return NULL;
    resetSolver(s);
    s->n = 0;
    s->capacity = 0;
    s->volumes = NULL;
    s->mode = FULL;
    s->heuristic = 1;
    s->isTimeout = NULL;
    s->timeoutMs = -1;

    return s;
}

Solution *solutionInit(int n) {
    Solution *s = malloc(sizeof(Solution));
    int *whichBin = malloc(n * sizeof(int));
    if (s && whichBin) {
        s->binCount = 0;
        s->n = n;
        s->whichBin = whichBin;
        return s;
    } else {
        free(s); free(whichBin);
        return NULL;
    }
};

void addSolution(Solver *s, Solution sol) {
    int capacity = s->solutionCapacity;

    if (s->solutionCount >= capacity) {
        capacity = (capacity == 0) ? 4 : (capacity * 2);

        Solution *tmp = realloc(s->solutions, capacity * sizeof(Solution));
        if (!tmp) {
            s->errorMsg = "Realloc failed for s->solution";
            s->error = 1; return;
        }
        s->solutions = tmp;
        s->solutionCapacity = capacity;
    }

    int *copy = malloc(sizeof(int) * sol.n);
    if (!copy) {
        s->errorMsg = "Malloc failed for dst->whichBin";
        s->error = 1; return;
    }
    memcpy(copy, sol.whichBin, sizeof(int) * sol.n);

    Solution *dst = &s->solutions[s->solutionCount];
    dst->binCount = sol.binCount;
    dst->n = sol.n;
    dst->whichBin = copy;

    s->solutionCount++;
}

void clearSolution(Solution *sol) {
    if (!sol) return;
    free(sol->whichBin);
    sol->whichBin = NULL;
    sol->binCount = 0;
}

void freeSolution(Solution *sol) {
    if (!sol) return;
    clearSolution(sol);
    free(sol);
    sol = NULL;
}

void freeSolutions(Solution *list, int count) {
    for (int i = 0; i < count; i++) {
        clearSolution(&list[i]);
    }
    free(list);
    list = NULL;
}

void clearSolver(Solver *s) {
    if (!s) return;
    freeSolutions(s->solutions, s->solutionCount);
    resetSolver(s);
}

void freeSolver(Solver *s) {
    if (!s) return;
    clearSolver(s);
    free(s);
    s = NULL;
}

void firstFit(Solver *s) {
    int *binVolumes = calloc(s->n, sizeof(int));
    if (!binVolumes) {
        s->errorMsg = "Malloc failed for binVolumes";
        s->error = 1; return;
    }
    Solution *sol = solutionInit(s->n);
    if (!sol) {
        free(binVolumes);
        s->errorMsg = "Malloc failed for Solution";
        s->error = 1; return;
    }

    for (int i = 0; i < s->n; i++) {
        int placed = 0;

        for (int j = 0; j < sol->binCount; j++) {
            if (s->isTimeout(s->startTime, s->timeoutMs)) {
                s->timedOut = 1;
                s->solutionFound = 0;
                free(binVolumes);
                freeSolution(sol);
                return;
            };
            if (binVolumes[j] + s->volumes[i] <= s->capacity) {
                binVolumes[j] += s->volumes[i];
                sol->whichBin[i] = j + 1;
                placed = 1;
                break;
            }
        }

        if (!placed) {
            binVolumes[sol->binCount] += s->volumes[i];
            sol->whichBin[i] = sol->binCount + 1;
            sol->binCount++;
        }
    }

    s->timedOut = 0;
    s->solutionFound = 1;
    addSolution(s, *sol);
    free(binVolumes);
    freeSolution(sol);
}

void firstFitDecreasing(Solver *s) {
    qsort(s->volumes, s->n, sizeof(int), compareDesc);
    firstFit(s);
}

void backtrack(Solver *s, int idx,
               int *whichBin,
               int *binVolumes,
               int usedBins,
               int *bestBinCount)
{
    if (s->isTimeout(s->startTime, s->timeoutMs) || s->timedOut) {
        s->timedOut = 1;
        if (s->solutionCount > 0) {
            s->solutionFound = 1;
        } else s->solutionFound = 0;
        return;
    }

    if (s->mode != FIRST_MATCH && usedBins > *bestBinCount) return;

    if (s->stopSearch) return;

    if (idx == s->n) {
        if (usedBins < *bestBinCount) {
            freeSolutions(s->solutions, s->solutionCount);
            s->solutionCount = 0;
            s->solutions = NULL;
            s->solutionCapacity = 0;
            *bestBinCount = usedBins;
            Solution sol = {usedBins, s->n, whichBin};
            addSolution(s, sol);
            s->solutionFound = 1;
            if (s->mode == FIRST_MATCH || s->error) {
                s->stopSearch = 1;
            }
            if (s->error) s->solutionFound = 0;
        } else if (usedBins == *bestBinCount) {
            Solution sol = {usedBins, s->n, whichBin};
            addSolution(s, sol);
            s->solutionFound = 1;
            if (s->mode == FIRST_MATCH || s->error) {
                s->stopSearch = 1;
            }
            if (s->error) s->solutionFound = 0;
        }
        return;
    }

    int volume = s->volumes[idx];

    for (int j = 0; j < usedBins; j++) {
        if (j > 0 && binVolumes[j] == binVolumes[j-1]) continue;
        if (binVolumes[j] + volume <= s->capacity) {
            binVolumes[j] += volume;
            whichBin[idx] = j + 1;

            backtrack(s, idx + 1, whichBin, binVolumes, usedBins, bestBinCount);

            binVolumes[j] -= volume;
        }
    }

    binVolumes[usedBins] = volume;
    whichBin[idx] = usedBins + 1;

    backtrack(s, idx + 1, whichBin, binVolumes, usedBins + 1, bestBinCount);

    binVolumes[usedBins] = 0;
}

void fullSearch(Solver * s) {
    int n = s->n;

    int *binVolumes = (int *)calloc(n, sizeof(int));
    int *whichBin = (int *)malloc(n * sizeof(int));

    if (binVolumes && whichBin) {
        backtrack(s, 0, whichBin, binVolumes, 0, &n);
    } else {
        s->error = 1;
        s->errorMsg = "Malloc failed for fullSearch";
    }

    free(binVolumes);
    free(whichBin);
};

void solve(Solver *s) {
    if (s->isTimeout == NULL) {
        s->isTimeout = isTimeout;
    }

    if (s->volumes == NULL) {
        s->errorMsg = "No volumes array provided";
        s->error = 1; return;
    } else if (s->capacity <= 0) {
        s->errorMsg = "Capacity must be positive";
        s->error = 1; return;
    } else if (s->n <= 0) {
        s->errorMsg = "N must be positive";
        s->error = 1; return;
    }

    s->startTime = clock();
    switch (s->mode) {
        case FIRST_MATCH: {
            fullSearch(s);
            break;
        }
        case HEURISTIC: {
            if (s->heuristic == 1) {
                firstFitDecreasing(s);
            } else {
                firstFit(s);
            }
            break;
        }
        case FULL: {
            fullSearch(s);
            break;
        }
        default: {
            s->errorMsg = "Unknown search mode";
            s->error = 1;
        }
    }
    s->endTime = clock();
}

void printSolution(int *volumes, Solution sol) {
    int *counts = calloc(sol.binCount, sizeof(int));
    if (!counts) return;

    for (int i = 0; i < sol.n; i++) {
        counts[sol.whichBin[i] - 1]++;
    }

    int **bins = malloc(sol.binCount * sizeof(int *));
    if (!bins) return;
    for (int b = 0; b < sol.binCount; b++) {
        bins[b] = malloc(counts[b] * sizeof(int));
        counts[b] = 0;
    }

    for (int i = 0; i < sol.n; i++) {
        int b = sol.whichBin[i] - 1;
        bins[b][counts[b]++] = volumes[i];
    }

    for (int b = 0; b < sol.binCount; b++) {
        printf("[");
        for (int j = 0; j < counts[b]; j++) {
            if (j > 0) printf(", ");
            printf("%d", bins[b][j]);
        }
        printf("] ");
        free(bins[b]);
    }

    printf("\n");

    free(bins);
    free(counts);
}

void printSolutions(Solver *s) {
    Solution *sols = s->solutions;

    for (int a = 0; a < s->solutionCount; a++) {
        Solution sol = sols[a];
        printSolution(s->volumes, sol);
    }
}

int compareDesc(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

int isTimeout(clock_t start, int timeoutMs) {
    if (timeoutMs < 0) return 0;

    clock_t now = clock();
    double elapsedMs = (double)(now - start) * 1000.0 / CLOCKS_PER_SEC;

    return elapsedMs >= timeoutMs;
}

void resetSolver(Solver *s) {
    s->startTime = 0;
    s->endTime= 0;
    s->timedOut = 0;
    s->solutionFound = 0;
    s->stopSearch = 0;
    s->error = 0;
    s->errorMsg = NULL;
    s->solutionCount = 0;
    s->solutionCapacity = 0;
    s->solutions = NULL;
}
