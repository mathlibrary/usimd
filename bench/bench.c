#include"../src/lib/vadd.c"
#include"../src/lib/vpi.c"
#include"../src/lib/vsum.c"
#include "bench.h"

void bench_pi() 
{
    printf("###############################\n");
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
    printf("###############################\n");
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
    printf("###############################\n");
    int scale = 1000000;
    float *input;
    int loops = 10;
    double timeg;
    if (( input = (float *)malloc(sizeof(float) * scale)) == NULL){
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

int main()
{
    bench_pi();
    bench_add();
    bench_sum();
    return 0;
}