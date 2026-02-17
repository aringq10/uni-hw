// A function for checking whether three points form a right triangle

#include <stdio.h>
#include <stdlib.h>

typedef struct Pair {
    int x;
    int y;
} Pair;

typedef struct {
    Pair a;
    Pair b;
    Pair c;
    int  res;
} TriangleTest;

TriangleTest tests[] = {
    { {0, 0}, {3, 0}, {0, 4}, 1 },
    { {1, 1}, {1, 6}, {6, 1}, 1 },
    { {-2, -3}, {-2, 1}, {3, -3}, 1 },
    { {0, 0}, {1, 1}, {2, 0}, 1 },

    { {0, 0}, {5, 0}, {3, 4}, 0 },
    { {0, 0}, {2, 0}, {1, 2}, 0 },
    { {0, 0}, {4, 0}, {1, 1}, 0 },
    { {1, 1}, {2, 2}, {3, 3}, 0 },
};

const int num_tests = sizeof(tests) / sizeof(tests[0]);

int isTriangle(Pair A, Pair B, Pair C) {
    Pair AB = { B.x - A.x, B.y - A.y };
    Pair BC = { C.x - B.x, C.y - B.y };
    Pair AC = { C.x - A.x, C.y - A.y };

    if (AB.x * BC.x + AB.y * BC.y == 0 ||
        BC.x * AC.x + BC.y * AC.y == 0 ||
        AB.x * AC.x + AB.y * AC.y == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int main(int argc, char** argv) {
    int pass = 1;
    for (int i = 0; i < num_tests; i++) {
        TriangleTest *t = &tests[i];

        int result = isTriangle(t->a, t->b, t->c);

        if (result != t->res) {
            printf("FAIL: test %d\n", i + 1);
            pass = 0;
        }
    }

    if (pass) {
        printf("All tests passed\n");
    }
}
