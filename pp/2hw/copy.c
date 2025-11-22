/*
    Code written by Aringas Civilka 2025
    for a procedural programming class in
    Vilnius University.

    Assignment:
    Įvesti sveiką skaičių N. Užpildyti
    matricą N*N sveikais skaičiais
    1, 2, 3, …, surašytais spirale.
*/

#define nLimit 9999

#include <stdio.h>
#include <stdlib.h>

int digitCount(int n) {
    if (n == 0)
        return 1;

    int len = 0;

    while (n != 0) {
        n /= 10;
        len++;
    }

    return len;
}

void getPosInt(int *N, int upperLimit) {
    while (1) {
        if (scanf("%9d", N) == 1 && getchar() == '\n') {
            if (*N < 1 || *N > upperLimit) {
                printf("Incorrect Input, try again: ");
                continue;
            }
            break;
        } else {
            printf("Incorrect Input, try again: ");
            while (getchar() != '\n')
                ;
            continue;
        }
    }
}

void createSpiralMatrix(int N, int **M, int **P) {
    *M = malloc(N * N * sizeof(int));
    *P = calloc(N, sizeof(int));

    if (!*M || !*P) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    
    int r = 0;        // Row
    int c = 0;        // Column
    int layer = 0;
    int len;
    
    for (int i = 1; i <= N*N; i++) {
        (*M)[r * N + c] = i;
 
        len = digitCount(i);
        if (len > (*P)[c])
            (*P)[c] = len;

        if (r == layer && c < (N - 1) - layer)
            c++; // Top side of layer
        else if (c == (N - 1) - layer && r < (N - 1) - layer)
            r++; // Right side of layer
        else if (r == (N - 1) - layer && c > layer)
            c--; // Bottom side of layer
        else if (c == layer && r > 1 + layer)
            r--; // Left side of layer
        else {   // Enter new layer
            c++;
            layer++;
        }
    }
}

void printMatrix(int n, int *M, int *P) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%-*d ", P[j], M[i * n + j]);
        }
        printf("\n");
    }
}

int main() {
    int N;
    int *M;
    int *P;

    printf("This program prints an N*N matrix in spiral order.\n");
    printf("Enter an integer N [1; %d]: ", nLimit);
    getPosInt(&N, nLimit);
    createSpiralMatrix(N, &M, &P);
    printMatrix(N, M, P);

    free(p);
    free(M);
    free(P);

    return 0;
}

