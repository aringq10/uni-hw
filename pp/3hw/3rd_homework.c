// Guess how much it will take - 1.5h
// 2025-11-22
// Planning - 10:05-10:35
// Code 10:45 - 11:48
// Testing 11:52 - 12:20
// Finishing up 12:20 - 12:33

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const unsigned int readBufferSize = 2; // Has to be larger than 1
const unsigned int wordLengthLimit = 100;
char *programName;

void printUsage() {
    printf(
        "Usage: %s inputFile outputFile\n"
        "*give the -h or --help flag after the program name for more details\n"
        , programName
    );
}

bool isPalindrome(char *word, int n) {
    for (int l = 0, r = n - 1; r - l >= 1; l++, r--) {
        if (word[l] != word[r]) {
            return false;
        }
    }

    return true;
}

void removePalindromes(FILE *inputF, FILE *outputF) {
    char buf[readBufferSize];
    char word[wordLengthLimit];
    int wordLen = 0;
    bool notWord = true;
    bool lenLimReached = false;

    int palindromesFound = 0;
    int wordsSkipped = 0;
    int charsWritten = 0;
    int charsOriginal = 0;

    while (fgets(buf, readBufferSize, inputF) != NULL) {
        for (int i = 0; buf[i] != '\0'; i++) {
            char cc = buf[i];

            if (cc == ' ' || cc == '\n' || cc == '\t') {
                charsOriginal++;
                charsOriginal += wordLen;
                if (!notWord) {
                    if (lenLimReached) {
                        lenLimReached = false;
                        wordsSkipped++;
                    }
                    else if (!isPalindrome(word, wordLen)) {
                        if (fwrite(word, wordLen, 1, outputF) != 1) {
                            printf("An error occured while writing to file\n");
                            return;
                        }
                        charsWritten += wordLen;
                    }
                    else {
                        palindromesFound++;
                    }
                }
                if (fputc(cc, outputF) == EOF) {
                    printf("An error occured while writing to file\n");
                    return;
                }
                charsWritten++;
                wordLen = 0;
                notWord = true;
                continue;
            }
            else {
                notWord = false;
                if (lenLimReached || wordLen + 1 > wordLengthLimit) {
                    lenLimReached = true;
                }
                else {
                    word[wordLen] = cc;
                    wordLen++;
                }
            }
       }
    }

    printf("Palindromes removed: %d\n", palindromesFound);
    printf("Words skipped: %d\n", wordsSkipped);
    printf("Original char count: %d\n", charsOriginal);
    printf("Chars written: %d\n", charsWritten);
}

int main(int argc, char *argv[]) {
    programName = argv[0];
    if (argc != 3) {
        printUsage();
        return 1;
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf(
            "-------------------------------------------------------------------\n"
            "This program removes all palindrome words from the given input file\n"
            "and outputs the new text into the output file. Words get skipped\n"
            "if they go over the word length limit specified at the top of the\n"
            "source code file.\n"
            "-------------------------------------------------------------------\n"
        );
        printUsage();
        return 0;
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
