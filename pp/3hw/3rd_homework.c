/*
    Code written by Aringas Civilka 2025
    for a procedural programming class in
    Vilnius University.

    Assignment:
    Parašyti funkciją, kuri iš duoto teksto išmeta
    žodžius, vienodai skaitomus iš pradžios ir iš
    galo (tarpų skaičius turi likti nepakitęs).
*/

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const unsigned int wordLengthLimit = 1024;
const unsigned int readBufferSize = 1024;
char *programName;

const char infoMsg[] = 
    "-------------------------------------------------------------------\n"
    "This program removes all palindrome words from the given input file\n"
    "and outputs the new text into the output file. Words get skipped\n"
    "if they go over the word length limit specified at the top of the\n"
    "source code file.\n"
    "Replace input/output file names with '-' to read from stdin and\n"
    "write to stdout respectively.\n"
    "-------------------------------------------------------------------\n";

void printUsage() {
    printf(
        "Usage: %s inputFile outputFile\n"
        "Use -h for more details\n"
        , programName
    );
}

void removePalindromes(FILE *inputStream, FILE *outputStream);
bool isPalindrome(char *word, int n);

int main(int argc, char *argv[]) {
    programName = argv[0];
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            printf(infoMsg);
            printUsage();
            return 0;
        }
    }
    if (argc != 3) {
        printUsage();
        return 1;
    }

    FILE *inputStream, *outputStream;
    if (strcmp(argv[1], "-") == 0) {
        inputStream = stdin;
    }
    else {
        inputStream = fopen(argv[1], "r");
        if (!inputStream) {
            fprintf(stderr, "An error ocurred while opening the input file\n");
            printUsage();
            return 1;
        }
    }
    if (strcmp(argv[2], "-") == 0) {
        outputStream = stdout;
    }
    else {
        outputStream = fopen(argv[2], "w");
        if (!outputStream) {
            fprintf(stderr, "An error ocurred while opening the output file\n");
            printUsage();
            return 1;
        }
    }

    removePalindromes(inputStream, outputStream);

    if (fclose(inputStream) != 0) {
        fprintf(stderr, "An error ocurred while closing the input stream\n");
        return 1;
    }
    if (fclose(outputStream) != 0) {
        fprintf(stderr, "An error ocurred while closing the output stream\n");
        return 1;
    }
    return 0;
}

void removePalindromes(FILE *inputStream, FILE *outputStream) {
    char buf[readBufferSize];
    char word[wordLengthLimit];
    int wordLen = 0;
    bool isWord = false;
    bool lenLimReached = false;

    int palindromesRemoved = 0;
    int wordsSkipped = 0;

    if (inputStream == stdin) {
        printf("To stop giving input, enter 'q' followed by <Enter>\n");
    }

    while (fgets(buf, readBufferSize, inputStream)) {
        if (inputStream == stdin && buf[0] == 'q' && buf[1] == '\n') {
            break;
        }
        for (int i = 0; buf[i] != '\0'; i++) {
            char c = buf[i];

            if (c == ' ' || c == '\n' || c == '\t') {
                if (isWord) {
                    if (lenLimReached) {
                        lenLimReached = false;
                        wordsSkipped++;
                    }
                    else if (!isPalindrome(word, wordLen)) {
                        if (fwrite(word, 1, wordLen, outputStream) != wordLen) {
                            printf("An error occured while writing to output stream\n");
                            return;
                        }
                    }
                    else {
                        palindromesRemoved++;
                    }
                }
                if (fputc(c, outputStream) == EOF) {
                    printf("An error occured while writing to output stream\n");
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

    printf("-------------------------------\n");
    printf("Palindromes removed: %d\n", palindromesRemoved);
    printf("Words skipped: %d\n", wordsSkipped);
    printf("-------------------------------\n");
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
