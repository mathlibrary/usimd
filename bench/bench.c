/**/
#include"../src/lib/vadd.c"
#include"../src/lib/vpi.c"
#include"../src/lib/vsum.c"
#include"../src/lib/vdot.c"
#include"../src/lib/vsqrt.c"

#include"../src/lib/vmuladd.c"

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
    printf("###############################\n");
}

void bench_add() 
{
    int scale = 1000000;
    FLOAT_T *input1;
    FLOAT_T *input2;
    FLOAT_T *output;
    int loops = 10;
    double timeg;
    if (( input1 = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
    if (( input2 = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
    if (( output = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
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
    printf("###############################\n");
}

void bench_sum() 
{
    int scale = 1000000;
    FLOAT_T *input;
    int loops = 10;
    double timeg;
    if (( input = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
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
    printf("###############################\n");
}

void bench_dot() 
{
    int scale = 1000000;
    FLOAT_T *input1;
    FLOAT_T *input2;
    int loops = 10;
    double timeg;
    if (( input1 = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
    if (( input2 = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
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
    printf("###############################\n");
}

void bench_sqrt() 
{
    int scale = 1000000;
    FLOAT_T *input;
    int loops = 10;
    double timeg;
    if (( input = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
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
    printf("###############################\n");
}

void bench_muladd(int scale) 
{
    FLOAT_T *input1;
    FLOAT_T *input2;
    FLOAT_T *input3;
    int loops = 10;
    double timeg;
    if (( input1 = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
    if (( input2 = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
    if (( input3 = (FLOAT_T *)malloc(sizeof(FLOAT_T) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
    printf("bench muladd:\n");
    for (int l=0; l<loops; l++)
    {
        begin();
        usimd_muladd(input1, input2, input3, scale);
        end();
        timeg += getsecs();
    }
    timeg /= loops;
    getMFlops(scale, timeg);
    printf("###############################\n");
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
    bench_add();
    bench_sum();
    bench_dot();
    bench_sqrt();
    */
    bench_muladd(scale);
    bench_muladd(scalex2);
    bench_muladd(scalex4);
    bench_muladd(scalex8);
    return 0;
}