#ifndef BIN_PACKING_H
#define BIN_PACKING_H

#include <time.h>

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
 *   stopSearch       - flag to stop searching
 *
 * OUTPUT:
 *   solutionCount    - number of solutions found
 *   solutionCapacity - memory allocated for solutions in terms of Solution
 *   solutions        - pointer to resulting Solution array (NULL if !solutionFound)
 *
 * OWNERSHIP:
 *   volumes          - owned by caller
 *   solutions        - allocated and managed internally by the solver, freed
 *                      by caller via freeSolver or clearSolver, but no directly
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
    int stopSearch;
    int error;
    char *errorMsg;
    int solutionCount;
    int solutionCapacity;
    Solution *solutions;
} Solver;

/* --- CONSTRUCTORS --- */

// Returns NULL on unsuccessful malloc
Solver *solverInit(void);

// Returns NULL on unsuccessful malloc
// n - number of items to fit into bins
Solution *solutionInit(int n);

/* --- DESTRUCTORS --- */

// Frees Solution's whichBin array, sets it to NULL and binCount to 0
void clearSolution(Solution *sol);

// Same as clearSolution, but also frees Solution itself, sets it to NULL
void freeSolution(Solution *sol);

// Frees count Solution elements from list and list itself, sets it to NULL
void freeSolutions(Solution *list, int count);

/*
 * Frees Solver's solutions array via freeSolutions, resets runtime
 * values via resetSolver
*/
void clearSolver(Solver *s);

// Same as clearSolver, but also frees Solver itself, sets it to NULL
void freeSolver(Solver *s);

/* --- ALGORITHM IMPLEMENTATIONS --- */

void firstFit(Solver *s);
void firstFitDecreasing(Solver *s);
/*
 * whichBin   - array size of s->n; see Solution.whichBin
 * binVolumes - array of size s->n;
 *
 * Caller is responsible for freeing whichBin and binVolumes arrays
*/
void backtrack(Solver *s, int idx,
               int *whichBin,
               int *binVolumes,
               int usedBins,
               int *bestBinCount);
void firstMatch(Solver * s);
void fullSearch(Solver * s);

/* --- ENTRY POINT --- */

/*
 * Calls algorithms based on s->mode, returns solutions
 * via s->solutions (see Solver and Solution)
*/
void solve(Solver *s);

/* --- OTHER --- */

// Dinamically add sol to s->solutions
void addSolution(Solver *s, Solution sol);

// Print sol, needs items volume array
void printSolution(int *volumes, Solution sol);

// Print s->solutions
void printSolutions(Solver *s);

// Descending compare for sorting
int compareDesc(const void *a, const void *b);

// Returns nonzero if now - start >= timeoutMs
int isTimeout(clock_t start, int timeoutMs);

// Sets Solver non-input fields to the defaults
void resetSolver(Solver *s);

#endif
