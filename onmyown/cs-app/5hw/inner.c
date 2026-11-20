#define data_t float

typedef struct {
    long len;
    data_t *data;
} vec_rec, *vec_ptr;

long vec_length(vec_ptr v)
{
    return v->len;
}

data_t *get_vec_start(vec_ptr v)
{
    return v->data;
}

/* Inner product. Accumulate in temporary */
void inner4(vec_ptr u, vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;

    for (i = 0; i < length; i++) {
        sum = sum + udata[i] * vdata[i];
    }
    *dest = sum;
}

void inner5(vec_ptr u, vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(u);
    long limit = length - 5;
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;

    for (i = 0; i < limit; i += 6) {
        sum += udata[i + 0] * vdata[i + 0]
             + udata[i + 1] * vdata[i + 1]
             + udata[i + 2] * vdata[i + 2]
             + udata[i + 3] * vdata[i + 3]
             + udata[i + 4] * vdata[i + 4]
             + udata[i + 5] * vdata[i + 5];
    }

    for (; i < length; i++) {
        sum += udata[i] * vdata[i];
    }

    *dest = sum;
}

void inner6(vec_ptr u, vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(u);
    long limit = length - 5;
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t acc0 = (data_t) 0;
    data_t acc1 = (data_t) 0;
    data_t acc2 = (data_t) 0;
    data_t acc3 = (data_t) 0;
    data_t acc4 = (data_t) 0;
    data_t acc5 = (data_t) 0;

    for (i = 0; i < limit; i += 6) {
        acc0 += udata[i + 0] * vdata[i + 0];
        acc1 += udata[i + 1] * vdata[i + 1];
        acc2 += udata[i + 2] * vdata[i + 2];
        acc3 += udata[i + 3] * vdata[i + 3];
        acc4 += udata[i + 4] * vdata[i + 4];
        acc5 += udata[i + 5] * vdata[i + 5];
    }

    acc0 += acc1 + acc2 + acc3 + acc4 + acc5;

    for (; i < length; i++) {
        acc0 += udata[i] * vdata[i];
    }

    *dest = acc0;
}

void inner7(vec_ptr u, vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(u);
    long limit = length - 5;
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;

    for (i = 0; i < limit; i += 6) {
        data_t p1 = udata[i + 0] * vdata[i + 0] + udata[i + 1] * vdata[i + 1] + udata[i + 2] * vdata[i + 2];
        data_t p2 = udata[i + 3] * vdata[i + 3] + udata[i + 4] * vdata[i + 4] + udata[i + 5] * vdata[i + 5] + p1;
        sum += p2;
    }

    for (; i < length; i++) {
        sum += udata[i] * vdata[i];
    }

    *dest = sum;
}
