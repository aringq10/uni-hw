#include <bits/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REPS (unsigned long)1000000
#define COUNT (unsigned long)16000

void *my_memset(void *s, int c, size_t n);
void *my_memset1(void *s, int c, size_t n);
void *basic_memset(void *s, int c, size_t n);

int main() {
    void *dst = malloc(COUNT);
    if (!dst) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < REPS; i++) {
        my_memset1(dst, i, COUNT);
    }
}
