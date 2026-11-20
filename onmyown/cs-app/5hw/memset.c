#include <stddef.h>

#define K sizeof(unsigned long)

/* Basic implementation of memset */
void *basic_memset(void *s, int c, size_t n)
{
    size_t cnt = 0;
    unsigned char *schar = s;
    while (cnt < n) {
        *schar++ = (unsigned char) c;
        cnt++;
    }
    return s;
}

void *my_memset(void *s, int c, size_t n) {
    unsigned char b = c & 0xFF;
    unsigned long v = b;
    size_t cnt = 0;

    for (int i = 1; i < K; i++) {
        v = (v << 8) + b;
    }

    // align dst address to K
    unsigned char *dst_b = s;
    while ((unsigned long)dst_b % K != 0 && cnt < n) {
        *dst_b++ = b;
        cnt++;
    }

    // write 4*K bytes at a time
    unsigned long *dst_l = (unsigned long *)dst_b;
    if (n - cnt >= 4*K) {
        unsigned long limit = n - 4*K;
        while (cnt <= limit) {
            *dst_l = v;
            *(dst_l + 1) = v;
            *(dst_l + 2) = v;
            *(dst_l + 3) = v;
            cnt += 4*K;
            dst_l += 4;
        }
    }

    // write remaining bytes one at a time
    dst_b = (unsigned char *)dst_l;
    while (cnt < n) {
        *dst_b++ = b;
        cnt++;
    }

    return s;
}

void *my_memset1(void *s, int c, size_t n) {
    unsigned char b = c & 0xFF;
    unsigned long v = b;
    size_t cnt = 0;

    for (int i = 1; i < K; i++) {
        v = (v << 8) + b;
    }

    // align dst address to K
    unsigned char *dst_b = s;
    while ((unsigned long)dst_b % K != 0 && cnt < n) {
        *dst_b++ = b;
        cnt++;
    }

    // write bytes at a time
    unsigned long *dst_l = (unsigned long *)dst_b;
    if (n - cnt >= K) {
        unsigned long limit = n - K;
        while (cnt <= limit) {
            *dst_l++ = v;
            cnt += K;
        }
    }

    // write remaining bytes one at a time
    dst_b = (unsigned char *)dst_l;
    while (cnt < n) {
        *dst_b++ = b;
        cnt++;
    }

    return s;
}
