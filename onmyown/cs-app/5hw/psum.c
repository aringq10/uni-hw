// returns -1 on success, otherwise the index of incorrect float
int test_psum(float a[], float p[], long n) {
    float acc = 0;
    for (int i = 0; i < n; i++) {
        acc += a[i];
        if (acc != p[i]) return i;
    }

    return -1;
}

// TESTED WITH n = 16000 and 1000000 repetitions

// 4 CPE
void psum1a(float a[], float p[], long n)
{
    long i;
    /* last_val holds p[i-1]; val holds p[i] */
    float last_val, val;
    last_val = p[0] = a[0];
    for (i = 1; i < n; i++) {
        val = last_val + a[i];
        p[i] = val;
        last_val = val;
    }
}

// 2.32 CPE
void psum(float a[], float p[], long n)
{
    long i;
    float acc = p[0] = a[0];
    for (i = 1; i < n - 1; i += 2) {
        p[i] = acc + a[i];
        acc = acc + (a[i] + a[i + 1]);
        p[i + 1] = acc;
    }

    for (; i < n; i++) {
        acc += a[i];
        p[i] = acc;
    }
}

// 1.61 CPE
void psum1(float a[], float p[], long n)
{
    long i;
    float acc = p[0] = a[0];
    for (i = 1; i < n - 2; i += 3) {
        p[i] = acc + a[i];
        p[i + 1] = acc + (a[i] + a[i + 1]);
        acc = acc + (a[i] + a[i + 1] + a[i + 2]);
        p[i + 2] = acc;
    }

    for (; i < n; i++) {
        acc += a[i];
        p[i] = acc;
    }
}
