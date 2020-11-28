#include "common.h"
#include "../simd/simd.h"


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