// 4.88 CPE
double poly(double a[], double x, long degree)
{
    long i;
    double result = a[0];
    double xpwr = x; /* Equals x^i at start of loop */
    for (i = 1; i <= degree; i++) {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }
    return result;
}

// 2.57 CPE, 2.87 CPE
double poly1(double a[], double x, long degree)
{
    long i;
    double res = a[0];
    double res1 = 0;
    double res2 = 0;
    double x2 = x * x;
    double xpwr = x;
    double xpwr2 = x2;

    for (i = 1; i <= degree - 1; i += 2) {
        res1 += a[i] * xpwr;
        xpwr *= x2;
        res2 += a[i + 1] * xpwr2;
        xpwr2 *= x2;
    }

    for (; i <= degree; i++) {
        res += a[i] * xpwr;
    }

    res += res1 + res2;

    return res;
}

// 2.32 CPE
double poly2(double a[], double x, long degree)
{
    long i;
    double res = a[0];
    double res1 = 0;
    double res2 = 0;
    double res3 = 0;
    double x2 = x * x;
    double x3 = x2 * x;
    double xpwr = x;
    double xpwr2 = x2;
    double xpwr3 = x3;

    for (i = 1; i <= degree - 2; i += 3) {
        res1 += a[i] * xpwr;
        xpwr *= x3;
        res2 += a[i + 1] * xpwr2;
        xpwr2 *= x3;
        res3 += a[i + 2] * xpwr3;
        xpwr3 *= x3;
    }

    for (; i <= degree; i++) {
        double d = i % 3 == 0 ? xpwr3 : i % 2 == 0 ? xpwr2 : xpwr;
        res += a[i] * d;
    }

    res += res1 + res2 + res3;

    return res;
}

/* Accumulate multiple values in parallel */
// From the solution manual
// CPE 1.75
double poly4x(double a[], double x, long degree)
{
    long i;
    double result0 = a[0];
    double result1 = 0;
    double result2 = 0;
    double result3 = 0;
    double x4 = (x*x)*(x*x);
    double xpwr = x;
    for (i = 1; i <= degree-3; i+=4) {
        double nxpwr = x4 * xpwr;
        result0 += a[i] * xpwr;
        result1 += a[i+1] * xpwr;
        result2 += a[i+2] * xpwr;
        result3 += a[i+3] * xpwr;
        xpwr = nxpwr;
    }
    result0 += x*(result1 + x*(result2 + x*result3));
    for (; i <= degree; i++) {
        result0 += a[i] * xpwr;
        xpwr = x * xpwr;
    }
    return result0;
}
