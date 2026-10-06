#include <limits.h>
#include <stdint.h>
#include <stdio.h>

typedef enum {NEG, ZERO, POS, OTHER} range_t;

range_t cfind_range(float x)
{
    int result;
    if (x < 0)
        result = NEG;
    else if (x == 0)
        result = ZERO;
    else if (x > 0)
        result = POS;
    else
        result = OTHER;
    return result;
}

range_t find_range(float x);
range_t find_range_2(float x);

union {
    float f;
    uint32_t u;
} arg;

int main() {
    for (uint64_t c = 0; c <= UINT32_MAX; c++) {
        arg.u = c;
        range_t expected = cfind_range(arg.f);
        range_t got      = find_range_2(arg.f);
        if (expected != got) {
            fprintf(stderr, "expected %d, got %d\n", expected, got);
            fprintf(stderr, "arg.u %d, arg.f %f\n", arg.u, arg.f);
            return 1;
        }
    }

    return 0;
}
