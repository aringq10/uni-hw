#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bin-packing.h"

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    SearchMode mode = FULL;
    int timeout = -1;
    int heuristic = 1;

    if (argc < 2) {
        fprintf(stderr,
            "Usage: %s\n"
            "   [-|failo_vardas]\n"
            "   [-mode [fullSearch|firstMatchSearch|heuristic heuristic_number]]\n"
            "   [-timeout miliseconds]\n"
            "Data format:\n"
            "   M            (Box capacity)\n"
            "   N            (Number of items)\n"
            "   v1 v2 ... vN (Volumes of items)\n",
            argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-") != 0) {
        input = fopen(argv[1], "r");
        if (!input) {
            perror("File error");
            return 1;
        }
    }

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-mode") == 0 && i + 1 < argc) {
            i++;
            if (strcmp(argv[i], "fullSearch") == 0) {
                mode = FULL;
            } else if (strcmp(argv[i], "firstMatchSearch") == 0) {
                mode = FIRST_MATCH;
            } else if (strcmp(argv[i], "heuristic") == 0) {
                mode = HEURISTIC;

                if (i + 1 < argc) {
                    heuristic = atoi(argv[++i]);
                }
            }
        } else if (strcmp(argv[i], "-timeout") == 0 && i + 1 < argc) {
            timeout = atoi(argv[++i]);
        }
    }

    int volumes[MAX_N];
    int n;
    int capacity;

    if (fscanf(input, "%d", &capacity) != 1) {
        fprintf(stderr, "Error reading capacity\n");
        return 1;
    } else if (capacity <= 0) {
        fprintf(stderr, "Capacity must be positive: %d\n", capacity);
        return 1;
    }

    if (fscanf(input, "%d", &n) != 1) {
        fprintf(stderr, "Error reading number of items\n");
        return 1;
    } else if (n <= 0 || n > MAX_N) {
        fprintf(stderr, "Invalid number of items: %d\n", n);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(input, "%d", &volumes[i]) != 1) {
            fprintf(stderr, "Error reading volume of item %d\n", i + 1);
            return 1;
        } else if (volumes[i] > capacity) {
            fprintf(stderr, "Volumes of all items must be less than the capacity\n");
            return 1;
        }
    }

    if (input != stdin) fclose(input);

    Solver *s = solverInit();
    if (!s) {
        fprintf(stderr, "Malloc failed for Solver\n");
        return 1;
    }

    s->volumes = volumes;
    s->capacity = capacity;
    s->n = n;
    s->mode = mode;
    s->heuristic = heuristic;
    s->timeoutMs = timeout;

    solve(s);
    double elapsedMs = (double)(s->endTime - s->startTime) * 1000.0 / CLOCKS_PER_SEC;

    if (s->error) {
        fprintf(stderr, "Solve Error: %s\n", s->errorMsg);
        return 1;
    } else if (s->timedOut) {
        printf("Timed out: %.4fms\n", elapsedMs);
    } else if (s->solutionFound) {
        printf("Timed elapsed: %.4fms\n", elapsedMs);
        printSolutions(s);
    } else {
        printf("No solution found\n");
    }

    freeSolver(s);

    return 0;
}
