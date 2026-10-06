#include <stdio.h>
#define N 4

// Safe stdin -> stdout line echo

void good_echo() {
    char buf[N];
    int i;

    while (fgets(buf, N, stdin) != NULL) {
        for (i = 0; buf[i] != '\n' && buf[i] != '\0'; i++) {
            if (putchar(buf[i]) == EOF) {
                return;
            }
        }

        if (buf[i] == '\n') {
            putchar('\n');
            return;
        }
    }
}

int main() {
    good_echo();
    return 0;
}
