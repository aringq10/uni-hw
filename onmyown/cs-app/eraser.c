/*
 * self-deleting executable
*/

#include <unistd.h>
#include <stdio.h>

#define N 1024

int main(int argc, char **argv) {
    char buf[N];

    ssize_t s = readlink("/proc/self/exe", buf, N - 1);

    if (s < 0 || s > N - 1) {
        fprintf(stderr, "could not read filename");
        return 1;
    } else {
        buf[s] = '\0';
    }

    // do some stuff

    if (unlink(buf) != 0) {
        fprintf(stderr, "could not delete file");
        return 1;
    }
    
    printf("Bye, won't see u again!\n");
    return 0;
}
