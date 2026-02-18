#include "stack.h"
#include <stdlib.h>

int main() {
    Stack *s1 = Create();
    push(s1, 10);
    push(s1, 25);
    push(s1, 13);
    printf("capacity(s1) after 3 pushes: %d\n", capacity(s1));
    printf("isEmpty(s1): %d\n", isEmpty(s1));
    for (int i = 0; i < 20; i++) {
        push(s1, i*3 + 10);
    }
    printf("capacity(s1) after 23 pushes: %d\n", capacity(s1));
    char* str1 = toString(s1);
    printf("toString(s1): %s\n", str1);
    printf("count(s1): %d\n", count(s1));
    printf("isFull(s1): %d\n", isFull(s1));
    printf("pop(s1): %d\n", *pop(s1));
    printf("pop(s1): %d\n", *pop(s1));
    printf("capacity(s1): %d\n", capacity(s1));
    printf("count(s1): %d\n", count(s1));
    int freeSpace = capacity(s1) - count(s1);
    for (int i = 0; i < freeSpace; i++) {
        int num = i;
        push(s1, num);
    }
    printf("capacity(s1) after 32 pushes: %d\n", capacity(s1));
    printf("count(s1): %d\n", count(s1));
    printf("isFull(s1): %d\n", isFull(s1));
    Stack *s2 = clone(s1);
    makeEmpty(s1);
    printf("s2 = clone(s1) and makeEmpty(s1):\n");
    printf("count(s1): %d, count(s2): %d\n", count(s1), count(s2));
    printf("isEmpty(s1): %d\n", isEmpty(s1));
    char* str2 = toString(s1);
    printf("toString(s1): %s\n", str2);

    free(str1);
    free(str2);
    Done(s1);
    Done(s2);
    return 0;
}
