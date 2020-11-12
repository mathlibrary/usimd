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
#if V_SIMD
#ifdef DOUBLE_T
	int vstep = v_nlanes_f64;
	int vstepx4 = vstep*4;
	for (; i >= vstepx4; i -= vstepx4, data0 += vstepx4, data1 += vstepx4, data_out += vstepx4) {
		v_f64 a0 = v_load_f64(data0);
		v_f64 b0 = v_load_f64(data1);
		v_f64 c0 = v_load_f64(data_out);
		v_f64 a1 = v_load_f64(data0 + vstep);
		v_f64 b1 = v_load_f64(data1 + vstep);
		v_f64 c1 = v_load_f64(data_out + vstep);
		v_f64 a2 = v_load_f64(data0 + vstep * 2);
		v_f64 b2 = v_load_f64(data1 + vstep * 2);
		v_f64 c2 = v_load_f64(data_out + vstep * 2);
		v_f64 a3 = v_load_f64(data0 + vstep * 3);
		v_f64 b3 = v_load_f64(data1 + vstep * 3);
		v_f64 c3 = v_load_f64(data_out + vstep * 3);
		v_f64 abc0 = v_muladd_f64(a0, b0, c0);
		v_f64 abc1 = v_muladd_f64(a1, b1, c1);
		v_f64 abc2 = v_muladd_f64(a2, b2, c2);
		v_f64 abc3 = v_muladd_f64(a3, b3, c3);
		v_store_f64(data_out , abc0);
		v_store_f64(data_out + vstep , abc1);
		v_store_f64(data_out + vstep * 2, abc2);
		v_store_f64(data_out + vstep * 3, abc3);
	}
#else
	int vstep = v_nlanes_f32;
	int vstepx4 = vstep*4;
	for (; i >= vstepx4; i -= vstepx4, data0 += vstepx4, data1 += vstepx4, data_out += vstepx4) {
		v_f32 a0 = v_load_f32(data0);
		v_f32 b0 = v_load_f32(data1);
		v_f32 c0 = v_load_f32(data_out);
		v_f32 a1 = v_load_f32(data0 + vstep);
		v_f32 b1 = v_load_f32(data1 + vstep);
		v_f32 c1 = v_load_f32(data_out + vstep);
		v_f32 a2 = v_load_f32(data0 + vstep * 2);
		v_f32 b2 = v_load_f32(data1 + vstep * 2);
		v_f32 c2 = v_load_f32(data_out + vstep * 2);
		v_f32 a3 = v_load_f32(data0 + vstep * 3);
		v_f32 b3 = v_load_f32(data1 + vstep * 3);
		v_f32 c3 = v_load_f32(data_out + vstep * 3);
		v_f32 abc0 = v_muladd_f32(a0, b0, c0);
		v_f32 abc1 = v_muladd_f32(a1, b1, c1);
		v_f32 abc2 = v_muladd_f32(a2, b2, c2);
		v_f32 abc3 = v_muladd_f32(a3, b3, c3);
		v_store_f32(data_out , abc0);
		v_store_f32(data_out + vstep , abc1);
		v_store_f32(data_out + vstep * 2, abc2);
		v_store_f32(data_out + vstep * 3, abc3);
	}
#endif
#endif
	for(; i >=0; i--){
		data_out[i] += data0[i] * data1[i];
	}
}