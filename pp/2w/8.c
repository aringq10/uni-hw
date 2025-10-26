/*
    sort an integer by its digits without
    using arrays, only arithmetic.
*/

#include <stdio.h>

int mypow10(int p) {
    int r = 1;
    while (p--) {
        r *= 10;
    }
    return r;
}

int sort_by_digit(int n, int ascending) {
    if (n == 0)
        return 0;

    int neg = n < 0;
    if (neg)
        n *= -1;
    int s = n;

    int l = 0;
    int z = 0;

    while (n > 0) {
        z += (n % 10 == 0) ? 1 : 0;
        l++;
        n /= 10;
    }
    
    int sorted = 0;
    int rl, from, to;
    if (ascending) {
        rl = l - z;
        from = 1;
        to = 9;
    }
    else {
        rl = l;
        from = 9;
        to = 1;
    }

    int mult = 1;
    for (int i = 0; i < rl - 1; i++) {
        mult *= 10;
    }

    for (int i = from; (ascending ? i <= to : i >= to); (ascending ? i++ : i--)) {
        for (int j = 0; j < l; j++) {
            int digit = (s / mypow10(l - j - 1)) % 10;
            if (digit == i) {
                sorted += i * mult;
                mult /= 10;
            }
        }
    }

    return (neg ? -1 * sorted : sorted);
}

int main() {
    int n;
    int o;

    printf("[0] for descending, [1] for ascending:\n");

prompt_order:
    if (scanf("%1d", &o) != 1 || getchar() != '\n') {
        while (getchar() != '\n')
            ;
        goto prompt_order;
    }
    
    if (o != 0 && o != 1)
        goto prompt_order;

    printf("Enter integer up to 9 digits:\n"); 

prompt_num:
    if (scanf("%9d", &n) != 1 || getchar() != '\n') {
        printf("invalid\n");
        while (getchar() != '\n')
            ;
        goto prompt_num;
    }

    int sorted = sort_by_digit(n, o);

    printf("%d\n", sorted);
    return 0;
}
