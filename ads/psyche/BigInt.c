#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "BigInt.h"

#define BASE 100
#define BASE_DIGITS 2

// helper functions
void bi_ensure(BigInt *a, int needed) {
    if (needed <= a->capacity) {
        return;
    }
    int newcap = (needed < 8) ? 8 : needed * 2;
    a->digits = realloc(a->digits, newcap * sizeof(int));
    assert(a->digits);
    for (int i = a->capacity; i < newcap; i++) {
        a->digits[i] = 0;
    }
    a->capacity = newcap;
}

BigInt *bi_alloc(int capacity) {
    BigInt *a = malloc(sizeof(BigInt));
    assert(a);
    a->capacity = capacity < 8 ? 8 : capacity;
    a->digits = calloc(a->capacity, sizeof(int));
    assert(a->digits);
    a->size = 1;
    a->sign = 1;
    return a;
}

// remove trailing zeros
void bi_trim(BigInt *a) {
    while (a->size > 1 && a->digits[a->size - 1] == 0) {
        a->size--;
    }
    if (a->size == 1 && a->digits[0] == 0) {
        a->sign = 1;
    }
}

int bi_is_zero(const BigInt *a) {
    return (a->size == 1 && a->digits[0] == 0);
}

/* Compare magnitudes only. Returns -1, 0, or +1. */
int bi_cmp_mag(const BigInt *a, const BigInt *b) {
    if (a->size != b->size) {
        return (a->size > b->size) ? 1 : -1;
    }
    for (int i = a->size - 1; i >= 0; i--) {
        if (a->digits[i] != b->digits[i]) {
            return (a->digits[i] > b->digits[i]) ? 1 : -1;
        }
    }
    return 0;
}

/* dst = |a| + |b| */
BigInt *bi_add_mag(const BigInt *a, const BigInt *b) {
    int n = (a->size > b->size ? a->size : b->size) + 1;
    BigInt *res = bi_alloc(n);
    res->size   = n;
    int carry = 0;
    for (int i = 0; i < n; i++) {
        int da = (i < a->size) ? a->digits[i] : 0;
        int db = (i < b->size) ? b->digits[i] : 0;
        int sum = da + db + carry;
        res->digits[i] = sum % BASE;
        carry          = sum / BASE;
    }
    bi_trim(res);
    return res;
}

/* dst = |a| - |b|  (assumes |a| >= |b|) */
BigInt *bi_sub_mag(const BigInt *a, const BigInt *b) {
    BigInt *res = bi_alloc(a->size);
    res->size   = a->size;
    int borrow = 0;
    for (int i = 0; i < a->size; i++) {
        int da = a->digits[i];
        int db = (i < b->size) ? b->digits[i] : 0;
        int diff = da - db - borrow;
        if (diff < 0) {
            diff += BASE; borrow = 1;
        }
        else {
            borrow = 0;
        }
        res->digits[i] = diff;
    }
    bi_trim(res);
    return res;
}


// public functions
void bigint_free(BigInt *a) {
    if (!a) {
        return;
    }
    free(a->digits);
    free(a);
}

BigInt *bigint_copy(const BigInt *a) {
    BigInt *c = bi_alloc(a->size);
    c->size   = a->size;
    c->sign   = a->sign;
    memcpy(c->digits, a->digits, a->size * sizeof(int));
    return c;
}

/* Parse decimal string (optional leading '-') */
BigInt *bigint_from_str(const char *s) {
    int sign = 1;
    if (*s == '-') {
        sign = -1; s++;
    }
    else if (*s == '+') {
        s++;
    }

    /* skip leading zeros */
    while (*s == '0' && *(s+1)) {
        s++;
    }

    int len = (int)strlen(s);
    int nlimbs = (len + BASE_DIGITS - 1) / BASE_DIGITS;
    BigInt *a = bi_alloc(nlimbs);
    a->size = nlimbs;
    a->sign = sign;

    int limb = 0, mul = 1;
    for (int i = 0; i < len; i++) {
        int d = s[len - 1 - i] - '0';
        a->digits[limb] += d * mul;
        mul *= 10;
        if (mul == BASE) {
            limb++; mul = 1;
        }
    }

    bi_trim(a);
    if (bi_is_zero(a)) {
        a->sign = 1;
    }
    return a;
}

/* Convert to decimal string; caller must free() the result */
char *bigint_to_str(const BigInt *a) {
    /* upper bound: sign + 2 chars per limb + NUL */
    int buflen = 1 + a->size * BASE_DIGITS + 1;
    char *buf  = malloc(buflen);
    assert(buf);

    char *p = buf;
    if (a->sign == -1 && !bi_is_zero(a)) {
        *p++ = '-';
    }

    /* most-significant limb (no leading zero pad) */
    int i = a->size - 1;
    p += sprintf(p, "%d", a->digits[i]);

    /* remaining limbs padded to exactly 2 digits */
    for (i--; i >= 0; i--) {
        p += sprintf(p, "%02d", a->digits[i]);
    }

    *p = '\0';
    return buf;
}

/* Compare: returns -1, 0, +1 */
int bigint_cmp(const BigInt *a, const BigInt *b) {
    if (a->sign != b->sign) {
        if (bi_is_zero(a) && bi_is_zero(b)) {
            return 0;
        }
        return (a->sign > b->sign) ? 1 : -1;
    }
    int mag = bi_cmp_mag(a, b);
    return (a->sign == 1) ? mag : -mag;
}

BigInt *bigint_add(const BigInt *a, const BigInt *b) {
    BigInt *res;
    if (a->sign == b->sign) {
        res = bi_add_mag(a, b);
        res->sign = a->sign;
    } else {
        int c = bi_cmp_mag(a, b);
        if (c == 0) {
            res = bi_alloc(1); /* zero */
        } else if (c > 0) {
            res = bi_sub_mag(a, b);
            res->sign = a->sign;
        } else {
            res = bi_sub_mag(b, a);
            res->sign = b->sign;
        }
    }
    if (bi_is_zero(res)) {
        res->sign = 1;
    }
    return res;
}

BigInt *bigint_sub(const BigInt *a, const BigInt *b) {
    /* a - b  ==  a + (-b) */
    BigInt *neg_b = bigint_copy(b);
    neg_b->sign = -neg_b->sign;
    if (bi_is_zero(neg_b)) {
        neg_b->sign = 1;
    }
    BigInt *res = bigint_add(a, neg_b);
    bigint_free(neg_b);
    return res;
}

BigInt *bigint_mul(const BigInt *a, const BigInt *b) {
    int n = a->size, m = b->size;
    BigInt *res = bi_alloc(n + m);
    res->size = n + m;
    res->sign = (a->sign == b->sign) ? 1 : -1;

    for (int i = 0; i < n; i++) {
        int carry = 0;
        for (int j = 0; j < m; j++) {
            int idx  = i + j;
            bi_ensure(res, idx + 1);
            if (idx >= res->size) {
                res->size = idx + 1;
            }
            int prod = res->digits[idx] + a->digits[i] * b->digits[j] + carry;
            res->digits[idx] = prod % BASE;
            carry            = prod / BASE;
        }
        int idx = i + m;
        bi_ensure(res, idx + 1);
        if (idx >= res->size) {
            res->size = idx + 1;
        }
        res->digits[idx] += carry;
    }

    bi_trim(res);
    if (bi_is_zero(res)) {
        res->sign = 1;
    }
    return res;
}

void bi_divmod_mag(const BigInt *a, const BigInt *b, BigInt **q_out, BigInt **r_out) {
    int n = a->size;

    BigInt *quot = bi_alloc(n);
    quot->size   = n;

    BigInt *rem = bi_alloc(n + 1);
    rem->size   = 1;

    for (int i = n - 1; i >= 0; i--) {
        bi_ensure(rem, rem->size + 1);
        for (int k = rem->size; k > 0; k--) {
            rem->digits[k] = rem->digits[k - 1];
        }
        rem->digits[0] = a->digits[i];
        rem->size++;
        bi_trim(rem);
        int lo = 0, hi = BASE - 1, q = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;

            BigInt *mid_bi = bi_alloc(1);
            mid_bi->digits[0] = mid;
            mid_bi->size      = 1;

            BigInt *prod = bigint_mul(mid_bi, b);
            bigint_free(mid_bi);

            int cmp = bi_cmp_mag(prod, rem);
            bigint_free(prod);

            if (cmp <= 0) {
                q = mid; lo = mid + 1;
            }
            else {
                hi = mid - 1;
            }
        }

        quot->digits[i] = q;

        if (q > 0) {
            BigInt *q_bi = bi_alloc(1);
            q_bi->digits[0] = q;
            q_bi->size      = 1;
            BigInt *sub = bigint_mul(q_bi, b);
            bigint_free(q_bi);
            BigInt *new_rem = bi_sub_mag(rem, sub);
            bigint_free(sub);
            bigint_free(rem);
            rem = new_rem;
        }
    }

    bi_trim(quot);
    bi_trim(rem);
    *q_out = quot;
    *r_out = rem;
}

void bigint_divmod(const BigInt *a, const BigInt *b, BigInt **q, BigInt **r) {
    assert(!bi_is_zero(b) && "bigint_divmod: division by zero");

    if (bi_is_zero(a)) {
        *q = bi_alloc(1);
        *r = bi_alloc(1);
        return;
    }

    if (bi_cmp_mag(a, b) < 0) {
        *q = bi_alloc(1);
        *r = bigint_copy(a);
        (*r)->sign = a->sign;
        return;
    }

    BigInt *ua = bigint_copy(a); 
    ua->sign = 1;
    BigInt *ub = bigint_copy(b); 
    ub->sign = 1;

    bi_divmod_mag(ua, ub, q, r);
    bigint_free(ua);
    bigint_free(ub);

    (*q)->sign = (a->sign == b->sign) ? 1 : -1;
    if (bi_is_zero(*q)) {
        (*q)->sign = 1;
    }

    (*r)->sign = a->sign;
    if (bi_is_zero(*r)) {
        (*r)->sign = 1;
    }
}

// division
BigInt *bigint_div(const BigInt *a, const BigInt *b) {
    BigInt *q, *r;
    bigint_divmod(a, b, &q, &r);
    bigint_free(r);
    return q;
}

// modulo
BigInt *bigint_mod(const BigInt *a, const BigInt *b) {
    BigInt *q, *r;
    bigint_divmod(a, b, &q, &r);
    bigint_free(q);
    return r;
}
