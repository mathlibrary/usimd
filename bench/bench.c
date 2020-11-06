/**/
#include"../src/lib/vadd.c"
#include"../src/lib/vpi.c"
#include"../src/lib/vsum.c"
#include"../src/lib/vdot.c"
#include"../src/lib/vsqrt.c"
#include"../src/lib/vmuladd.c"
#include"../src/lib/vdaxpy.c"

#include "bench.h"

void bench_pi() 
{
    int scale = 100*1024*1024;
    printf("bench pi native:\n");
    begin();
    compute_pi_native(scale);
    end();
    getMFlops(scale, getsecs());
    printf("bench pi avx:\n");
    begin();
    compute_pi_omp_avx(scale);
    end();
    getMFlops(scale, getsecs());
    printf("bench pi avx+loop:\n");
    begin();
    compute_pi_omp_avx_loop(scale);
    end();
    getMFlops(scale, getsecs());
}

void bench_add(int scale) 
{
    FLOAT_T *input1 = getFinput(scale);
    FLOAT_T *input2 = getFinput(scale);
    FLOAT_T *output = getFinput(scale);
    int loops = 10;
    double timeg;
    printf("bench add:\n");
    for (int l=0; l<loops; l++)
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
    printf("bench sum:\n");
    for (int l=0; l<loops; l++)
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
    printf("bench dot:\n");
    for (int l=0; l<loops; l++)
    {
        begin();
        FLOAT_T actual = usimd_dot(8,input1, 1, input2, 1);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

void bench_sqrt(int scale) 
{
    FLOAT_T *input = getFinput(scale);
    int loops = 10;
    double timeg;
    printf("bench sqrt:\n");
    for (int l=0; l<loops; l++)
    {
        begin();
        usimd_sqrt(input, scale);
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
    printf("bench muladd with scale: %d\n", scale);
    for (int l=0; l<loops; l++)
    {
        begin();
        usimd_muladd(input1, input2, input3, scale);
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
    printf("bench axpy with scale: %d\n", scale);
    for (int l=0; l<loops; l++)
    {
        begin();
        usimd_daxpy(scale,input1,input2, &alpha);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
}

int main()
{
    int scale = 1e6;
    int scalex2 = 2e6;
    int scalex4 = 4e6;
    int scalex8 = 8e6;
    /*
    start benching

    bench_pi();
    bench_add(scale);
    bench_sum(scale);
    bench_dot(scale);
    bench_sqrt(scale);
    bench_muladd(scale);
    */
    bench_axpy(scalex8);
    return 0;
}