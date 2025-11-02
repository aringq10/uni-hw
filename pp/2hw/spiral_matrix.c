/*
    Code written by Aringas Civilka 2025
    for a procedural programming class in
    Vilnius University.

    Assignment:
    Įvesti sveiką skaičių N. Užpildyti
    matricą N*N sveikais skaičiais
    1, 2, 3, …, surašytais spirale.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void getPosInt(int *N) {
    while (1) {
        if (scanf("%4d", N) == 1 && getchar() == '\n') {
            if (*N < 1) {
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

void spiralMatrixPrintfOnly(int N) {
    int *P = malloc(N * sizeof(int));
    int l;
    int biggestNum;

    for (short i = 0; i < N / 2; i++) {
        l = i;
        // Formula for last (upper left) member of layer
        biggestNum = 4 * N - 4 + l * 4 * N - l * (8 + l * 4);
        P[i] = digitCount(biggestNum);
    }
    for (short i = N / 2; i < N; i++) {
        l = N - i - 1;
        // Formula for bottom right member of layer
        biggestNum = 2 * N + 4 * N * l - 1 - 8 * (l * (l + 1) / 2);
        P[i] = digitCount(biggestNum);
    }

    int startingNum;    // (First member (upper left) of layer) - 1
    int num;            // Number for printing

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            // Determine layer
            if (r + 1 <= N / 2) {      // Upper half
                if (c + 1 <= N / 2)    // Upper left quadrant
                    l = (r < c) ? r : c;
                else                   // Upper right quadrant
                    l = (r < N - c - 1) ? r : N - c - 1;
            }
            else {                     // Bottom half
                if (c + 1 <= N / 2)    // Bottom left quadrant
                    l = (N - r - 1 < c) ? N - r - 1 : c;
                else                   // Bottom right quadrant
                    l = (N - r - 1 < N - c - 1) ? N - r - 1 : N - c - 1;
            }

            startingNum = l * (4 * N) - l * l * 4;

            if (r == l) {              // top side of layer
                num = startingNum + (c - l) + 1;
            }
            else if (N - c - 1 == l) { // right side of layer
                num = startingNum + N - 2 * l;
                num += r - l;
            }
            else if (N - r - 1 == l) { // bottom side of layer
                num = startingNum + N - 2 * l;
                num += N - 2 * l - 1;
                num += (N - c - 1 - l);
            }
            else {                     // left side of layer
                num = startingNum + N - 2 * l;
                num += N - 2 * l - 1;
                num += N - 2 * l - 1;
                num += (N - r - 1 - l);
            }

            printf("%-*d ", P[c], num);
        }
        printf("\n");
    }

    free(P);
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

    double print_only_time, malloc_time;
    int flag = -1;
    
    printf(
        "Choose how you would like to print the spiral matrix:\n"
        "[0] - using only printf() and logic\n"
        "[1] - using malloc() to preprocess the matrix and printf() for output\n"
        "[2] - both\n"
        "Enter a value between 0 and 2: "
    );
    
    while (1) {
        if (scanf("%1d", &flag) == 1 && getchar() == '\n') {
            if (flag == 0 || flag == 1 || flag == 2)
                break;
            printf("Enter a value between 0 and 2: ");
        }
        else {
            printf("Enter a value between 0 and 2: ");
            while (getchar() != '\n')
                ;
        }
    }
    
    printf("This program prints an N*N matrix in spiral order.\n");
    printf("Enter an integer N [1; 9999]: ");
    getPosInt(&N);


    if (flag == 0 || flag == 2) { 
        printf("---\n");
        clock_t startP = clock();

        spiralMatrixPrintfOnly(N);

        clock_t endP = clock();
        print_only_time = ((double)(endP - startP)) / CLOCKS_PER_SEC;
    }
    
    if (flag == 1 || flag == 2) {
        printf("---\n");
        clock_t startM = clock();

        createSpiralMatrix(N, &M, &P);
        printMatrix(N, M, P);
        free(M);
        free(P);

        clock_t endM = clock();
        malloc_time = ((double)(endM - startM)) / CLOCKS_PER_SEC;
    }

    printf("---\n");
    if (flag == 0 || flag == 2)
        printf("CPU time used(printf only): %.6f seconds\n", print_only_time);
    if (flag == 1 || flag == 2)
        printf("CPU time used(malloc + printf): %.6f seconds\n", malloc_time);

    return 0;
}
