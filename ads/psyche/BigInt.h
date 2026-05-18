#ifndef BIGINT_H
#define BIGINT_H

typedef struct {
    int *digits;
    int size;
    int capacity; 
    int sign;
} BigInt;

BigInt *bigint_from_str(const char *s);
char *bigint_to_str(const BigInt *a);
void bigint_free(BigInt *a);
BigInt *bigint_copy(const BigInt *a);
int bigint_cmp(const BigInt *a, const BigInt *b);
BigInt *bigint_add(const BigInt *a, const BigInt *b);
BigInt *bigint_sub(const BigInt *a, const BigInt *b);
BigInt *bigint_mul(const BigInt *a, const BigInt *b);
BigInt *bigint_div(const BigInt *a, const BigInt *b);
BigInt *bigint_mod(const BigInt *a, const BigInt *b);

#endif