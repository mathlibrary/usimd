#include<stdio.h>
#include <immintrin.h>
#include "../bench/bench.h"
double compute_pi_native(int dt) {
    double pi = 0.0;
    double delta = 1.0 / dt;
    for(int i=0;i<dt;i++) {
        double x = (double)i/dt;
        pi += delta/(1.0+x*x);
    }
    return pi*4.0;
}
//SIMD
double compute_pi_omp_avx(int dt) {
    double pi = 0.0;
    double delta = 1.0 / dt;
    __m256d t1, t2, vone, vpi,vdelta;
    t2 = _mm256_set1_pd(delta);
    vone = _mm256_set1_pd(1.0);
    vpi = _mm256_set1_pd(0.0);
    vdelta = _mm256_set_pd(3*delta, 2*delta, delta, 0.0 );
    for(int i=0;i<=dt-4;i+=4) {
        // 这两步合为一步反而更快，为啥？t1 = _mm256_set_pd((i+3)*delta, (i+2)*delta, (i+1)*delta, i*delta );
        t1 = _mm256_set1_pd( i*delta );
        t1 = _mm256_add_pd(t1,vdelta);
        t1 = _mm256_mul_pd(t1,t1);
        t1 = _mm256_add_pd(t1,vone);
        t1 = _mm256_div_pd(t2,t1);
        vpi = _mm256_add_pd(vpi,t1);
    }
    //SIMD求和，为啥没有加速？
    __m256d sum_halves = _mm256_hadd_pd(vpi, vpi);
    __m128d lo = _mm256_castpd256_pd128(sum_halves);
    __m128d hi = _mm256_extractf128_pd(sum_halves, 1);
    __m128d sum = _mm_add_pd(lo, hi);
    return _mm_cvtsd_f64(sum)*4.0;
    /*
    这种求和方式和上面的差不了多少
   double tmp[4] __attribute__((aligned(32)));
   _mm256_store_pd(tmp, vpi);
   pi += tmp[0] + tmp[1] + tmp[2] + tmp[3];
   return pi * 4.0;*/
}

// SIMD+循环展开，效率没有提高，为啥？
double compute_pi_omp_avx_loop(int dt) {
    double pi = 0.0;
    double delta = 1.0 / dt;
    __m256d t1, t2, tdelta, vone, vpi1 ,vpi2 , vdelta1, vdelta2;
    tdelta = _mm256_set1_pd(delta);
    vone = _mm256_set1_pd(1.0);
    vpi1 = _mm256_set1_pd(0.0);
    vpi2 = _mm256_set1_pd(0.0);
    vdelta1 = _mm256_set_pd(3*delta, 2*delta, delta, 0.0 );
    vdelta2 = _mm256_set_pd(7*delta, 6*delta, 5*delta, 4*delta );
    for(int i=0;i<=dt-8;i+=8) {
        t1 = _mm256_set1_pd( i*delta );
        t1 = _mm256_add_pd(t1,vdelta1);
        t1 = _mm256_mul_pd(t1,t1);
        t1 = _mm256_add_pd(t1,vone);
        t1 = _mm256_div_pd(tdelta,t1);
        vpi1 = _mm256_add_pd(vpi1,t1);

        t2 = _mm256_set1_pd( (i+4)*delta );
        t2 = _mm256_add_pd(t2,vdelta2);
        t2 = _mm256_mul_pd(t2,t2);
        t2 = _mm256_add_pd(t2,vone);
        t2 = _mm256_div_pd(tdelta,t2);
        vpi2 = _mm256_add_pd(vpi2,t2);
    }
    vpi1 = _mm256_add_pd(vpi1,vpi2);
    //求和
    __m256d sum_halves = _mm256_hadd_pd(vpi1, vpi1);
    __m128d lo = _mm256_castpd256_pd128(sum_halves);
    __m128d hi = _mm256_extractf128_pd(sum_halves, 1);
    __m128d sum = _mm_add_pd(lo, hi);
    return _mm_cvtsd_f64(sum)*4.0;
    /*
    这种求和方式和上面的差不了多少
   double tmp[4] __attribute__((aligned(32)));
   _mm256_store_pd(tmp, vpi);
   pi += tmp[0] + tmp[1] + tmp[2] + tmp[3];
   return pi * 4.0;*/
}

int main()
{
    int scale = 100*1024*1024;
    begin();
    printf("result: %f", compute_pi_native(100*1024*1024));
    end(scale);
    begin();
    printf("result: %f\n", compute_pi_omp_avx(scale));
    end(scale);
    begin();
    printf("result: %f\n", compute_pi_omp_avx_loop(scale));
    end(scale);
    return 0;
}