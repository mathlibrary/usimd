#include<stdio.h>
#include "common.h"
#include "../simd/simd.h"
/*
	After actual testing, unrolled by 4 shows the best performance.Here is the result:
	For float type:
	pure C :            884 MFlops
	SIMD+no unroll:    1760 MFlops
	SIMD+unroll by 2:  1944 MFlops
	SIMD+unroll by 4:  1946 MFlops
	SIMD+unroll by 5:  1777 MFlops
	For double type:
	pure C :            780 MFlops
	SIMD+no unroll:    813 MFlops
	SIMD+unroll by 2:  884 MFlops
	SIMD+unroll by 4:  972 MFlops
	SIMD+unroll by 5:  968 MFlops
	
*/
void usimd_muladd(const FLOAT_T *data0, const FLOAT_T *data1, FLOAT_T *data_out, int len)
{
    int i=len;
#if NPY_SIMD
#ifdef DOUBLE_T
	int vstep = npyv_nlanes_f64;
	int vstepx4 = vstep*4;
	for (; i >= vstepx4; i -= vstepx4, data0 += vstepx4, data1 += vstepx4, data_out += vstepx4) {
		npyv_f64 a0 = npyv_load_f64(data0);
		npyv_f64 b0 = npyv_load_f64(data1);
		npyv_f64 c0 = npyv_load_f64(data_out);
		npyv_f64 a1 = npyv_load_f64(data0 + vstep);
		npyv_f64 b1 = npyv_load_f64(data1 + vstep);
		npyv_f64 c1 = npyv_load_f64(data_out + vstep);
		npyv_f64 a2 = npyv_load_f64(data0 + vstep * 2);
		npyv_f64 b2 = npyv_load_f64(data1 + vstep * 2);
		npyv_f64 c2 = npyv_load_f64(data_out + vstep * 2);
		npyv_f64 a3 = npyv_load_f64(data0 + vstep * 3);
		npyv_f64 b3 = npyv_load_f64(data1 + vstep * 3);
		npyv_f64 c3 = npyv_load_f64(data_out + vstep * 3);
		npyv_f64 abc0 = npyv_muladd_f64(a0, b0, c0);
		npyv_f64 abc1 = npyv_muladd_f64(a1, b1, c1);
		npyv_f64 abc2 = npyv_muladd_f64(a2, b2, c2);
		npyv_f64 abc3 = npyv_muladd_f64(a3, b3, c3);
		npyv_store_f64(data_out , abc0);
		npyv_store_f64(data_out + vstep , abc1);
		npyv_store_f64(data_out + vstep * 2, abc2);
		npyv_store_f64(data_out + vstep * 3, abc3);
	}
#else
	int vstep = npyv_nlanes_f32;
	int vstepx4 = vstep*4;
	for (; i >= vstepx4; i -= vstepx4, data0 += vstepx4, data1 += vstepx4, data_out += vstepx4) {
		npyv_f32 a0 = npyv_load_f32(data0);
		npyv_f32 b0 = npyv_load_f32(data1);
		npyv_f32 c0 = npyv_load_f32(data_out);
		npyv_f32 a1 = npyv_load_f32(data0 + vstep);
		npyv_f32 b1 = npyv_load_f32(data1 + vstep);
		npyv_f32 c1 = npyv_load_f32(data_out + vstep);
		npyv_f32 a2 = npyv_load_f32(data0 + vstep * 2);
		npyv_f32 b2 = npyv_load_f32(data1 + vstep * 2);
		npyv_f32 c2 = npyv_load_f32(data_out + vstep * 2);
		npyv_f32 a3 = npyv_load_f32(data0 + vstep * 3);
		npyv_f32 b3 = npyv_load_f32(data1 + vstep * 3);
		npyv_f32 c3 = npyv_load_f32(data_out + vstep * 3);
		npyv_f32 abc0 = npyv_muladd_f32(a0, b0, c0);
		npyv_f32 abc1 = npyv_muladd_f32(a1, b1, c1);
		npyv_f32 abc2 = npyv_muladd_f32(a2, b2, c2);
		npyv_f32 abc3 = npyv_muladd_f32(a3, b3, c3);
		npyv_store_f32(data_out , abc0);
		npyv_store_f32(data_out + vstep , abc1);
		npyv_store_f32(data_out + vstep * 2, abc2);
		npyv_store_f32(data_out + vstep * 3, abc3);
	}
#endif
#endif
	for(; i >=0; i--){
		data_out[i] += data0[i] * data1[i];
	}
}