#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bin-packing.h"

char *resolveSearchMode(SearchMode m) {
    switch (m) {
        case FULL:
            return "FULL_SEARCH";
            break;
        case FIRST_MATCH:
            return "FIRST_MATCH";
            break;
        case HEURISTIC:
            return "HEURISTIC";
            break;
        default:
            return "\0";
            break;
    }
}

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
            "   M            (Bin capacity)\n"
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
                    char *endptr;
                    heuristic = strtol(argv[++i], &endptr, 10);
                    if (endptr == argv[i] || *endptr != '\0') {
                        fprintf(stderr, "Invalid heuristic_number: %s\n", argv[i]);
                        return 1;
                    }
                }
            } else {
                fprintf(stderr, "Unknown mode: %s\n", argv[i]);
                return 1;
            }
        } else if (strcmp(argv[i], "-timeout") == 0 && i + 1 < argc) {
            char *endptr;
            timeout = strtol(argv[++i], &endptr, 10);
            if (endptr == argv[i] || *endptr != '\0') {
                fprintf(stderr, "Invalid timeout: %s\n", argv[i]);
                return 1;
            }
        }
    }

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
    } else if (n <= 0) {
        fprintf(stderr, "Number of items must be positive: %d\n", n);
        return 1;
    }

    int *volumes = malloc(n * sizeof(int));
    if (!volumes) {
        fprintf(stderr, "Malloc failed for volumes array\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(input, "%d", &volumes[i]) != 1) {
            fprintf(stderr, "Error reading volume of item %d\n", i + 1);
            free(volumes); return 1;
        } else if (volumes[i] > capacity) {
            fprintf(stderr, "Volumes of all items must be less than the capacity\n");
            free(volumes); return 1;
        }
    }

    if (input != stdin) fclose(input);

    Solver *s = solverInit();
    if (!s) {
        fprintf(stderr, "Malloc failed for Solver\n");
        free(volumes); return 1;
    }

    s->volumes = volumes;
    s->capacity = capacity;
    s->n = n;
    s->mode = mode;
    s->heuristic = heuristic;
    s->timeoutMs = timeout;

    solve(s);
    double elapsedMs = (double)(s->endTime - s->startTime) * 1000.0 / CLOCKS_PER_SEC;

    printf("Input data:\n");
    printf("M         (bin capacity):    %d\n", capacity);
    printf("N         (Number of items): %d\n", n);
    printf("v1 ... vN (Item volumes):    ");
    for (int i = 0; i < n; i++) {
        printf("%d ", volumes[i]);
    }
    printf("\n");
    printf("Search mode: %s\n", resolveSearchMode(mode));
    printf("Heuristic level: %d\n", heuristic);
    printf("Timeout: %d ms\n\n", timeout);

    printf("Output data:\n");
    printf("Each line represents a possible solution, where\n");
    printf("each [...] structure is a bin and its contents\n");
    printf("are volumes of items that it contains\n\n");

    if (s->error) {
        fprintf(stderr, "Solve Error: %s\n", s->errorMsg);
        freeSolver(s); free(volumes); return 1;
    } else if (s->timedOut) {
        printf("Program timed out: %.4f ms\n", elapsedMs);
    }

    if (s->solutionFound) {
        printf("Time elapsed: %.4f ms\n", elapsedMs);
        printSolutions(s);
    } else {
        printf("No solution found\n");
    }

    freeSolver(s);
    free(volumes);

    return 0;
}
