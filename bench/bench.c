#include<stdio.h>
#include"../src/lib/vadd.c"
#include"../src/lib/vpi.c"
#include "bench.h"

void bench_pi() 
{
    int scale = 100*1024*1024;
    printf("bench pi native");
    begin();
    compute_pi_native(scale);
    end();
    getMFlops(scale, getsecs());
    printf("bench pi avx");
    begin();
    compute_pi_omp_avx(scale);
    end();
    getMFlops(scale, getsecs());
    printf("bench pi avx+loop");
    begin();
    compute_pi_omp_avx_loop(scale);
    end();
    getMFlops(scale, getsecs());
}

void bench_add() 
{
    int scale = 1000000;
    float *input1;
    float *input2;
    float *output;
    int loops = 10;
    double timeg;
    if (( input1 = (float *)malloc(sizeof(float) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
    if (( input2 = (float *)malloc(sizeof(float) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
    if (( output = (float *)malloc(sizeof(float) * scale)) == NULL){
        fprintf(stderr,"Out of Memory!!\n");exit(1);
    }
    printf("bench add");
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

int main()
{
    bench_pi();
    bench_add();
    return 0;
}