#ifndef BIN_PACKING_H
#define BIN_PACKING_H

#include <time.h>

#define MAX_N 1000

typedef enum {
    FULL,
    FIRST_MATCH,
    HEURISTIC
} SearchMode;

/*
 * Solution must be initialized with solutionInit before use.
 * Solution must be freed with freeSolution after use.
 *
 * binCount  - number of bins used
 *
 * n         - number of items
 *
 * whichBin  - array of size n;
 *             whichBin[i] = index(1-based) of the bin containing item i
 */
typedef struct {
    int binCount;
    int n;
    int *whichBin;
} Solution;

/*
 * Solver must be initialized with solverInit before use.
 * Solver must be freed with freeSolver after use.
 *
 * REQUIRED INPUT FIELDS:
 *   n                - number of items (n > 0)
 *   capacity         - bin capacity (capacity > 0)
 *   volumes          - array of size n; volumes[i] = size of item i (non-NULL)
 *
 * OPTIONAL CONFIGURATION (defaults applied in solverInit):
 *   mode             - search mode (see SearchMode)
 *   heuristic        - heuristic strategy identifier
 *   isTimeout        - function returning nonzero if timeout exceeded;
 *                      called with (clock_t start, int timeoutMs)
 *   timeoutMs        - timeout in milliseconds (negative = no timeout)
 *
 * RUNTIME STATE (managed internally by solver):
 *   startTime        - timestamp when solving starts
 *   startTime        - timestamp when solving ends
 *   timedOut         - nonzero if timeout occurred
 *   solutionFound    - nonzero if a solution was found
 *   error            - nonzero if an error occurred
 *   errorMsg         - error message (NULL if no error)
 *
 * OUTPUT:
 *   solutionCount    - number of solutions found
 *   solutionCapacity - memory allocated for solutions in terms of Solution
 *   solutions        - pointer to resulting Solution array (NULL if !solutionFound)
 *
 * OWNERSHIP:
 *   volumes          - owned by caller
 *   solutions        - owned by caller; must be freed with freeSolutions,
 *                      clearSolver or freeSolver
*/
typedef struct {
    int n;
    int capacity;
    int* volumes;

    SearchMode mode;
    int heuristic;
    int (*isTimeout)(clock_t start, int timeoutMs);
    int timeoutMs;
    clock_t startTime;
    clock_t endTime;
    int timedOut;
    int solutionFound;
    int error;
    char *errorMsg;
    int solutionCount;
    int solutionCapacity;
    Solution *solutions;
} Solver;

// Constructors

Solver *solverInit(void);
Solution *solutionInit(int n);

// Destructors

void clearSolution(Solution *sol);
void freeSolution(Solution *sol);
void freeSolutions(Solution *list, int count);
void clearSolver(Solver *s);
void freeSolver(Solver *s);

// Algorithms

void firstFit(Solver *s);
void firstFitDecreasing(Solver *s);
void backtrack(Solver *s, int idx,
               int *whichBin,
               int *binVolumes,
               int usedBins,
               int *permutations,
               int *bestBinCount);
void firstMatch(Solver * s);
void fullSearch(Solver * s);

// Entry point

void solve(Solver *s);

// Other

void addSolution(Solver *s, Solution sol);
void printSolution(Solver *s, Solution sol);
void printSolutions(Solver *s);
int compareDecr(const void *a, const void *b);
int isTimeout(clock_t start, int timeoutMs);

#endif
