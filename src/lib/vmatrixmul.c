#include "common.h"
#include "../simd/simd.h"

void chunked_mm_simd(FLOAT_T **matrix_a, FLOAT_T **matrix_b, FLOAT_T **result_matrix,
                     int M, int N, int K, int chunk, int n_chunks)
{
#if V_SIMD
#ifdef DOUBLE_T
    v_f64 va0, va1, va2, va3;
    v_f64 vb0, vb1, vb2, vb3;
    v_f64 vresult;
    const int vstep = v_nlanes_f64;
#else
    v_f32 va0, va1, va2, va3;
    v_f32 vb0, vb1, vb2, vb3;
    v_f32 vresult;
    const int vstep = v_nlanes_f32;
#endif
    const int unrollx4 = K & -vstep * 4;
#endif
    for (int i = chunk * (M / n_chunks); i < (chunk + 1) * (M / n_chunks); i++)
    {
        for (int j = 0; j < N; j++)
        {
            int k = 0;
#if V_SIMD
#ifdef DOUBLE_T
            v_f64 vresult0 = v_zero_f64();
            v_f64 vresult1 = v_zero_f64();
            v_f64 vresult2 = v_zero_f64();
            v_f64 vresult3 = v_zero_f64();
            for (; k < unrollx4; k += vstep * 4)
            {
                // load
                va0 = v_load_f64(matrix_a[i] + k);
                vb0 = v_load_f64(matrix_b[j] + k);
                va1 = v_load_f64(matrix_a[i] + k + vstep);
                vb1 = v_load_f64(matrix_b[j] + k + vstep);
                va2 = v_load_f64(matrix_a[i] + k + vstep * 2);
                vb2 = v_load_f64(matrix_b[j] + k + vstep * 2);
                va3 = v_load_f64(matrix_a[i] + k + vstep * 3);
                vb3 = v_load_f64(matrix_b[j] + k + vstep * 3);
                // multiply
                vresult0 = v_muladd_f64(va0, vb0, vresult0);
                vresult1 = v_muladd_f64(va1, vb1, vresult1);
                vresult2 = v_muladd_f64(va2, vb2, vresult2);
                vresult3 = v_muladd_f64(va3, vb3, vresult3);
            }
            vresult0 = v_add_f64(v_add_f64(vresult0, vresult1), v_add_f64(vresult2, vresult3));
            // store
            result_matrix[i][j] += v_sum_f64(vresult0);
#else
            v_f32 vresult0 = v_zero_f32();
            v_f32 vresult1 = v_zero_f32();
            v_f32 vresult2 = v_zero_f32();
            v_f32 vresult3 = v_zero_f32();
            for (; k < unrollx4; k += vstep * 4)
            {
                // load
                va0 = v_load_f32(matrix_a[i] + k);
                vb0 = v_load_f32(matrix_b[j] + k);
                va1 = v_load_f32(matrix_a[i] + k + vstep);
                vb1 = v_load_f32(matrix_b[j] + k + vstep);
                va2 = v_load_f32(matrix_a[i] + k + vstep * 2);
                vb2 = v_load_f32(matrix_b[j] + k + vstep * 2);
                va3 = v_load_f32(matrix_a[i] + k + vstep * 3);
                vb3 = v_load_f32(matrix_b[j] + k + vstep * 3);
                // multiply
                vresult0 = v_muladd_f32(va0, vb0, vresult0);
                vresult1 = v_muladd_f32(va1, vb1, vresult1);
                vresult2 = v_muladd_f32(va2, vb2, vresult2);
                vresult3 = v_muladd_f32(va3, vb3, vresult3);
            }
            vresult0 = v_add_f32(v_add_f32(vresult0, vresult1), v_add_f32(vresult2, vresult3));
            // store
            result_matrix[i][j] += v_sum_f32(vresult0);
#endif
#endif
            for (; k < K; k++)
            {
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[j][k];
            }
        }
    }
}

/* matrix_b is transposed, accelerate through manully SIMD optimzation and multiple thread openmp
  1024x1024: 
        -O3: 
            float:
                no flag: 7.35 MFlops   0.570316 sec
                no auto-vectorization: 2.50 MFlops   1.679677 sec
                SSE2 manual-vectorization: 17.21 MFlops   0.243752 sec
                SSE3 manual-vectorization: 17.44 MFlops   0.240529 sec
                AVX2 auto-vectorization: 19.88 MFlops   0.210939 sec
            double:
                no flag: 9.29 MFlops   0.451565 sec
                no auto-vectorization: 2.51 MFlops   1.673892 sec
                SSE2 manual-vectorization: 0.84 MFlops   4.983339 sec
                SSE3 manual-vectorization: 7.65 MFlops   0.548441 sec
                AVX2 manual-vectorization: 8.28 MFlops   0.506253 sec
   Conclusion: multiple thread openmp is not useful combined with SIMD technique
   about half performance is lost after the multi-thread.
*/
void usimd_matrixmul_t_mp_simd(FLOAT_T **matrix_a, FLOAT_T **matrix_b, FLOAT_T **result_matrix,
                               int M, int N, int K)
{
#pragma omp parallel for
    for (int chunk = 0; chunk < 4; chunk++)
    {
        chunked_mm_simd(matrix_a, matrix_b, result_matrix, M, N, K, chunk, 4);
    }
}

/* matrix_b is transposed, accelerate through manully SIMD optimzation
  1024x1024: 
        -O3: 
            float:
                no flag: 19.63 MFlops   0.213680 sec
                no auto-vectorization: 2.50 MFlops   1.679677 sec
                SSE2 manual-vectorization: 17.31 MFlops   0.242294 sec
                SSE3 manual-vectorization: 17.62 MFlops   0.237998 sec
                AVX2 auto-vectorization: 18.73 MFlops   0.223934 sec
            double:
                no flag: 9.20 MFlops   0.455877 sec
                no auto-vectorization: 2.51 MFlops   1.673892 sec
                SSE2 manual-vectorization: 0.85 MFlops   4.913763 sec
                SSE3 manual-vectorization: 7.81 MFlops   0.536739 sec
                AVX2 manual-vectorization: 8.85 MFlops   0.473846 sec
   Conclusion: USIMD is really useful if -O3 is not specified, The performance is competitive when -O3 is opened.
   (SSE2 is the special case, which have side effect.)
*/
void usimd_matrixmul_t_simd(FLOAT_T **matrix_a, FLOAT_T **matrix_b, FLOAT_T **result_matrix,
                            int M, int N, int K)
{
#if V_SIMD
#ifdef DOUBLE_T
    v_f64 va0, va1, va2, va3;
    v_f64 vb0, vb1, vb2, vb3;
    v_f64 vresult;
    const int vstep = v_nlanes_f64;
#else
    v_f32 va0, va1, va2, va3;
    v_f32 vb0, vb1, vb2, vb3;
    v_f32 vresult;
    const int vstep = v_nlanes_f32;
#endif
    const int unrollx4 = K & -vstep * 4;
#endif
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int k = 0;
#if V_SIMD
#ifdef DOUBLE_T
            v_f64 vresult0 = v_zero_f64();
            v_f64 vresult1 = v_zero_f64();
            v_f64 vresult2 = v_zero_f64();
            v_f64 vresult3 = v_zero_f64();
            for (; k < unrollx4; k += vstep * 4)
            {
                // load
                va0 = v_load_f64(matrix_a[i] + k);
                vb0 = v_load_f64(matrix_b[j] + k);
                va1 = v_load_f64(matrix_a[i] + k + vstep);
                vb1 = v_load_f64(matrix_b[j] + k + vstep);
                va2 = v_load_f64(matrix_a[i] + k + vstep * 2);
                vb2 = v_load_f64(matrix_b[j] + k + vstep * 2);
                va3 = v_load_f64(matrix_a[i] + k + vstep * 3);
                vb3 = v_load_f64(matrix_b[j] + k + vstep * 3);
                // multiply
                vresult0 = v_muladd_f64(va0, vb0, vresult0);
                vresult1 = v_muladd_f64(va1, vb1, vresult1);
                vresult2 = v_muladd_f64(va2, vb2, vresult2);
                vresult3 = v_muladd_f64(va3, vb3, vresult3);
            }
            vresult0 = v_add_f64(v_add_f64(vresult0, vresult1), v_add_f64(vresult2, vresult3));
            // store
            result_matrix[i][j] += v_sum_f64(vresult0);
#else
            v_f32 vresult0 = v_zero_f32();
            v_f32 vresult1 = v_zero_f32();
            v_f32 vresult2 = v_zero_f32();
            v_f32 vresult3 = v_zero_f32();
            for (; k < unrollx4; k += vstep * 4)
            {
                // load
                va0 = v_load_f32(matrix_a[i] + k);
                vb0 = v_load_f32(matrix_b[j] + k);
                va1 = v_load_f32(matrix_a[i] + k + vstep);
                vb1 = v_load_f32(matrix_b[j] + k + vstep);
                va2 = v_load_f32(matrix_a[i] + k + vstep * 2);
                vb2 = v_load_f32(matrix_b[j] + k + vstep * 2);
                va3 = v_load_f32(matrix_a[i] + k + vstep * 3);
                vb3 = v_load_f32(matrix_b[j] + k + vstep * 3);
                // multiply
                vresult0 = v_muladd_f32(va0, vb0, vresult0);
                vresult1 = v_muladd_f32(va1, vb1, vresult1);
                vresult2 = v_muladd_f32(va2, vb2, vresult2);
                vresult3 = v_muladd_f32(va3, vb3, vresult3);
            }
            vresult0 = v_add_f32(v_add_f32(vresult0, vresult1), v_add_f32(vresult2, vresult3));
            // store
            result_matrix[i][j] += v_sum_f32(vresult0);
#endif
#endif
            for (; k < K; k++)
            {
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[j][k];
            }
        }
    }
}

void chunked_mm(FLOAT_T **matrix_a, FLOAT_T **matrix_b, FLOAT_T **result_matrix,
                int M, int N, int K, int chunk, int n_chunks)
{
    for (int i = chunk * (M / n_chunks); i < (chunk + 1) * (M / n_chunks); i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < K; k++)
            {
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
}

/* openmp multithread optimize
   1024x1024: 
        -O4 -fopenmp: 
            no flag: 7.14 MFlops   0.587229 sec
            no auto-vectorization: 7.12 MFlops   0.589066 sec
            SSE auto-vectorization: 6.97 MFlops   0.601566 sec
            AVX auto-vectorization: 6.58 MFlops   0.637504 sec
*/
void usimd_matrixmul_mp(FLOAT_T **matrix_a, FLOAT_T **matrix_b, FLOAT_T **result_matrix,
                        int M, int N, int K)
{
#pragma omp parallel for
    for (int chunk = 0; chunk < 4; chunk++)
    {
        chunked_mm(matrix_a, matrix_b, result_matrix, M, N, K, chunk, 4);
    }
}

/* matrix_b is transposed, openmp multithread optimize
  1024x1024: 
        -O4 -fopenmp: 
            no flag: 6.41 MFlops   0.654692 sec
            no auto-vectorization: 6.33 MFlops   0.662504 sec
            SSE auto-vectorization: 6.16 MFlops   0.681254 sec
            AVX auto-vectorization: 6.44 MFlops   0.651567 sec
*/
void usimd_matrixmul_t_mp(FLOAT_T **matrix_a, FLOAT_T **matrix_b, FLOAT_T **result_matrix,
                          int M, int N, int K)
{
#pragma omp parallel for
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < K; k++)
            {
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[j][k];
            }
        }
    }
}

/* matrix_b is transposed, accelerate through fast memory access
  1024x1024: 
        -O4: 
            no flag: 19.31 MFlops   0.217188 sec
            no auto-vectorization: 2.50 MFlops   1.679677 sec
            SSE auto-vectorization: 19.59 MFlops   0.214063 sec
            AVX auto-vectorization: 19.45 MFlops   0.215625 sec
*/
void usimd_matrixmul_t(FLOAT_T **matrix_a, FLOAT_T **matrix_b, FLOAT_T **result_matrix,
                       int M, int N, int K)
{
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < K; k++)
            {
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[j][k];
            }
        }
    }
}

/* normal matrix multiplition
  1024x1024: 
        -O2: 2.18 MFlops   1.920264 sec
        -O4: 
            no flag: 3.62 MFlops   1.159844 sec
            no auto-vectorization: 2.20 MFlops   1.909180 sec
            SSE auto-vectorization: 3.66 MFlops   1.146838 sec
            AVX auto-vectorization: 3.71 MFlops   1.131227 sec
*/
void usimd_matrixmul_normal(FLOAT_T **matrix_a, FLOAT_T **matrix_b, FLOAT_T **result_matrix,
                            int M, int N, int K)
{
    for (int i = 0; i < M; i++)
    { // iterate over rows of matrix A/result matrix
        for (int j = 0; j < N; j++)
        { // iterate over columns matrix B/result matrix
            for (int k = 0; k < K; k++)
            { // iterate over columns of matrix A and rows of matrix B
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
}