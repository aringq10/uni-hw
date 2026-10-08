#include <stdio.h>

// 5 + 32 = 37 (%)
// 5 + 16 = non-printable char
#define ASCII_OFFSET 32
// 96 is a multiple of 16
// 96 + 15 = 111 (o)
// 112 + 15 = 127 (non-printable DEL)
#define ASCII_MAX_OFFSET 96
// 0b1000, to keep the low 4 bits intact
#define ASCII_STEP 16

char s[] = {9, 15, 14, 5, 6, 7};

int main() {
    for (int o = ASCII_OFFSET; o < ASCII_MAX_OFFSET; o += ASCII_STEP) {
        for (int i = 0; i < sizeof(s); i++) {
            putchar(s[i] + o);
        }
        putchar('\n');
    }

    return 0;
}
