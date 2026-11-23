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

double poly(double a[], double x, long degree);
double poly1(double a[], double x, long degree);
double poly2(double a[], double x, long degree);
double poly4x(double a[], double x, long degree);
int test_psum(float a[], float p[], long n);
void psum1a(float a[], float p[], long n);
void psum(float a[], float p[], long n);
void psum1(float a[], float p[], long n);
void psum2(float a[], float p[], long n);

int main() {
    float *src = malloc(COUNT * sizeof(float));
    if (!src) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    float *dst = malloc(COUNT * sizeof(float));
    if (!dst) {
        free(src);
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < REPS; i++) {
        psum1(src, dst, COUNT);
    }

    if (test_psum(src, dst, COUNT) != -1) printf("INCORRECT");
    else printf("CORRECT");
}
