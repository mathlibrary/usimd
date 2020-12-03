/**/
#include "../src/lib/vadd.c"
#include "../src/lib/vsum.c"
#include "../src/lib/vdot.c"
#include "../src/lib/vmuladd.c"
#include "../src/lib/vdaxpy.c"
#include "../src/lib/vcmul.c"
#include "../src/lib/vaddindex.c"
#include "../src/lib/vaddeven.c"
#include "../src/lib/vfcond.c"
#include "../src/lib/varrmax.c"
#include "../src/lib/vmatrixmul.c"
#include "../src/lib/vpopcnt.c"
#include "bench.h"

void bench_add(int scale)
{
    FLOAT_T *input1 = getFinput(scale);
    FLOAT_T *input2 = getFinput(scale);
    FLOAT_T *output = getFinput(scale);
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        usimd_add(input1, input2, output, scale);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_sum(int scale)
{
    FLOAT_T *input = getFinput(scale);
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        usimd_sum(scale, input, 1);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_dot(int scale)
{
    FLOAT_T *input1 = getFinput(scale);
    FLOAT_T *input2 = getFinput(scale);
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        FLOAT_T actual = usimd_dot(scale, input1, 1, input2, 1);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_muladd(int scale)
{
    FLOAT_T *input1 = getFinput(scale);
    FLOAT_T *input2 = getFinput(scale);
    FLOAT_T *input3 = getFinput(scale);
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        usimd_muladd(input1, input2, input3, scale);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_cmul(int scale)
{
    FLOAT_T *input1 = getFinput(scale);
    FLOAT_T *input2 = getFinput(scale);
    FLOAT_T *output = getFinput(scale);
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        usimd_cmul(input1, input2, output, scale / 2);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_axpy(int scale)
{
    FLOAT_T *input1 = getFinput(scale);
    FLOAT_T *input2 = getFinput(scale);
    FLOAT_T alpha = 3.5;
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        usimd_daxpy(scale, input1, input2, &alpha);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_vaddindex(int scale)
{
    FLOAT_T *input = getFinput(scale);
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        usimd_addindex(scale, input);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_vaddeven(int scale)
{
    FLOAT_T *input = getFinput(scale);
    FLOAT_T *output = getFinput(scale / 2);
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        usimd_addeven(input, output, scale);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_vfcond(int scale)
{
    FLOAT_T *input = getFinput(scale);
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        usimd_fcond(input, scale);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_varrmax(int scale)
{
    FLOAT_T *input = getFinput(scale);
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        usimd_arrmax(input, scale);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_matrixmul(int M, int N, int K)
{
    FLOAT_T **mat1 = get2Darr(M, N);
    FLOAT_T **mat2 = get2Darr(N, K);
    FLOAT_T **result = get2Darr(M, K);
    int loops = 10;
    double timeg;
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < K; j++)
        {
            result[i][j] = 0;
        }
    }
    printf("%s with scale %dx%d:\n", __FUNCTION__, M, N);
    for (int l = 0; l < loops; l++)
    {
        begin();
        usimd_matrixmul_t_simd(mat1, mat2, result, M, N, K);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(M*N, timeg);
}

void bench_vpopcount(int scale)
{
    uint64_t *input = getIntArr(scale);
    int loops = 10;
    double timeg;
    printf("%s with scale %d:\n", __FUNCTION__, scale);
    for (int l = 0; l < loops; l++)
    {
        begin();
        harley_seal(input, scale);
        end();
        timeg += getsecs();
    }
    getMFlops(scale, timeg);
}

int main()
{
    int scale = 1e6;
    int scalex2 = 2e6;
    int scalex4 = 4e7;
    int scalex8 = 8e7;
    int x1024 = 1024;
    /*start benching
    bench_sum(scalex4);
    bench_sum(scalex8);
    bench_add(scalex4);
    bench_add(scalex8);
    bench_muladd(scalex4);
    bench_muladd(scalex8);
    bench_axpy(scalex4);
    bench_axpy(scalex8);
    bench_dot(scalex4);
    bench_dot(scalex8);
    bench_cmul(scalex4);
    bench_cmul(scalex8);
    bench_vaddeven(scalex4);
    bench_vaddeven(scalex8);
    bench_vfcond(scalex4);
    bench_vfcond(scalex8);
    bench_varrmax(scalex4);
    bench_varrmax(scalex8);
    bench_matrixmul(x1024,x1024,x1024);*/
    bench_vpopcount(1<<25);
    return 0;
}