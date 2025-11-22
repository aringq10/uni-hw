/*
    Code written by Aringas Civilka 2025
    for a procedural programming class in
    Vilnius University.

    Assignment:
    Parašyti funkciją, kuri iš duoto teksto išmeta
    žodžius, vienodai skaitomus iš pradžios ir iš
    galo (tarpų skaičius turi likti nepakitęs).
*/

// TODO
// optimize writing to padding to file(store in padding buffer and then write)
// if first arg skipped, take input from stdin
// if second arg skipped, print to stdout

// Guess how much it will take - 1.5h
// 2025-11-22
// Planning - 10:05-10:35
// Code 10:45 - 11:48
// Testing 11:52 - 12:20
// Finishing up 12:20 - 12:35

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const unsigned int wordLengthLimit = 100;
const unsigned int readBufferSize = 2; // Has to be larger than 1
                                       // or program will run forever
char *programName;

const char infoMsg[] = 
    "-------------------------------------------------------------------\n"
    "This program removes all palindrome words from the given input file\n"
    "and outputs the new text into the output file. Words get skipped\n"
    "if they go over the word length limit specified at the top of the\n"
    "source code file.\n"
    "-------------------------------------------------------------------\n";

void printUsage() {
    printf(
        "Usage: %s inputFile outputFile\n"
        "Try %s -h or %s --help for more details\n"
        , programName, programName, programName
    );
}

bool isPalindrome(char *word, int n);
void removePalindromes(FILE *inputF, FILE *outputF);

int main(int argc, char *argv[]) {
    programName = argv[0];
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            printf(infoMsg);
            printUsage();
            return 0;
        }
    }
    if (argc != 3) {
        printUsage();
        return 1;
    }

    FILE *inputF = fopen(argv[1], "r");
    if (inputF == NULL) {
        printf("An error ocurred while opening the input file\n");
        printUsage();
        return 1;
    }
    FILE *outputF = fopen(argv[2], "w");
    if (outputF == NULL) {
        printf("An error ocurred while opening the output file\n");
        printUsage();
        return 1;
    }

    removePalindromes(inputF, outputF);

    if (fclose(inputF) != 0) {
        printf("An error ocurred while closing the input file\n");
        return 1;
    }
    if (fclose(outputF) != 0) {
        printf("An error ocurred while closing the output file\n");
        return 1;
    }
    return 0;
}

void removePalindromes(FILE *inputF, FILE *outputF) {
    char buf[readBufferSize];
    char word[wordLengthLimit];
    int wordLen = 0;
    bool isWord = false;
    bool lenLimReached = false;

    int palindromesRemoved = 0;
    int wordsSkipped = 0;

    while (fgets(buf, readBufferSize, inputF) != NULL) {
        for (int i = 0; buf[i] != '\0'; i++) {
            char c = buf[i];

            if (c == ' ' || c == '\n' || c == '\t') {
                if (isWord) {
                    if (lenLimReached) {
                        lenLimReached = false;
                        wordsSkipped++;
                    }
                    else if (!isPalindrome(word, wordLen)) {
                        if (fwrite(word, wordLen, 1, outputF) != 1) {
                            printf("An error occured while writing to file\n");
                            return;
                        }
                    }
                    else {
                        palindromesRemoved++;
                    }
                }
                if (fputc(c, outputF) == EOF) {
                    printf("An error occured while writing to file\n");
                    return;
                }
                wordLen = 0;
                isWord = false;
                continue;
            }
            else {
                isWord = true;
                if (lenLimReached || wordLen + 1 > wordLengthLimit) {
                    lenLimReached = true;
                }
                else {
                    word[wordLen] = c;
                    wordLen++;
                }
            }
       }
    }

    printf("Palindromes removed: %d\n", palindromesRemoved);
    printf("Words skipped: %d\n", wordsSkipped);
}

bool isPalindrome(char *word, int n) {
    if (n == 0) {
        return false;
    }
    for (int l = 0, r = n - 1; r - l > 0; l++, r--) {
        while (l < n && !isalpha(word[l])) {
            l++;
        }
        while (r >= 0 && !isalpha(word[r])) {
            r--;
        }
        if (tolower(word[l]) != tolower(word[r])) {
            return false;
        }
    }
    return true;
}
