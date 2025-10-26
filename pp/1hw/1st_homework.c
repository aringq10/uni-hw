/*
    Code written by Aringas Civilka 2025
    for a procedural programming class in
    Vilnius University.

    Assignment:
    Įvesti sveikų skaičių seką, kurios
    pabaiga žymima skaičiumi 0. Išvesti 
    narių, kurie prasideda nelyginiu 
    skaitmenimi, skaičių.
*/

#include <stdio.h>
#include <stdbool.h>

#define maxLineLength 20

bool isDigit(char c) {
    if (c >= '0' && c <= '9')
        return true;

    return false;
}

int main () {
    int count = 0;
    char inputBuffer[maxLineLength + 2]; // + 2 for '\n' and '\0'
     
    printf(
        "--------------------------------------------------------\n"
        "1. Type in an integer followed by a new line, and as\n"
        "   many as you want.\n"
        "2. The program will output the amount of them that start\n"
        "   with an odd digit.\n"
        "3. To quit, type in a zero.\n"
        "--------------------------------------------------------\n"
    );
     
    // Get input until user types in a zero 
    while (true) {
        printf("Enter a whole number: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);

        int len = 0;
        while (inputBuffer[len] != '\0')
            len++;
        
        if (len == 1) {
            printf("Type something in!!!\n");
            continue;
        }

        if (inputBuffer[len - 1] == '\n') {
            bool NaN = false;
            bool isNeg = (inputBuffer[0] == '-');
            bool isZero = (len >= isNeg + 2 && inputBuffer[isNeg] == '0');
            int leadingZeros = 0;

            for (int i = isNeg; i < len - 1; i++) {
                char c = inputBuffer[i];

                if (isZero && c != '0')
                    isZero = false;
                if (isZero) {
                    if (c != '0')
                        isZero = false;
                    else leadingZeros++;
                }

                if (isDigit(c)) 
                    continue;
                
                NaN = true;
                break;
            }

            // Handle non-integer input
            if (NaN || (isNeg && len == 2)) {
                printf("Invalid input, type in a whole number without whitespace!\n");
                continue;
            }

            if (isZero) {
                printf("Amount of given numbers starting with an odd digit: %d\n", count);
                return 0;
            }

            // Get first digit
            int digit = inputBuffer[isNeg + leadingZeros] - '0';

            count += (digit % 2 == 0) ? 0 : 1;
        } else {
            printf("Input length exceeded limit of %d\n", maxLineLength);
            while (getchar() != '\n')
                ;
        }
    }
}
