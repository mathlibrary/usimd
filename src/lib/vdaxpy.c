
//#include "common.h"
static void daxpy(int n, FLOAT *x, FLOAT *y, FLOAT *alpha)
{
    int n1 = n & -16;
    int register i = 0;
    FLOAT a = *alpha;
#if NPY_SIMD
#ifdef DOUBLE
    npyv_f64 __alpha, tmp;
    __alpha = npyv_setall_f64(*alpha);
    const int vstep = npyv_nlanes_f64;
    for (; i < n; i += vstep)
    {
        tmp = npyv_muladd_f64(__alpha, npyv_load_f64(x + i), npyv_load_f64(y + i));
        npyv_storeu_f64(y + i, tmp);
    }
#else
    npyv_f32 __alpha, tmp;
    __alpha = npyv_setall_f32(*alpha);
    const int vstep = npyv_nlanes_f32;
    for (; i < n; i += vstep)
    {
        tmp = npyv_muladd_f32(__alpha, npyv_load_f32(x + i), npyv_load_f32(y + i));
        npyv_storeu_f32(y + i, tmp);
    }
#endif
#else
    while (i < n)
    {
        y[i] += a * x[i];
        y[i + 1] += a * x[i + 1];
        y[i + 2] += a * x[i + 2];
        y[i + 3] += a * x[i + 3];
        y[i + 4] += a * x[i + 4];
        y[i + 5] += a * x[i + 5];
        y[i + 6] += a * x[i + 6];
        y[i + 7] += a * x[i + 7];
        i += 8;
    }
#endif
    i = n1;
    while (i < n)
    {
        y[i] += a * x[i];
        i++;
    }
}