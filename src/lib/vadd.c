#include<stdio.h>
#include "common.h"
#include "../simd/simd.h"

void usimd_add(const FLOAT_T *a, const FLOAT_T *b, FLOAT_T *c, int len)
{
    int i=0;
#if NPY_SIMD && VEC_LT256
#ifdef DOUBLE_T
	int vsteps = v_nlanes_f64;
	int n = len & -vsteps;
	for (; i < n; i+=vsteps) {
		v_f64 v1 = v_load_f64(a+i);
		v_f64 v2 = v_load_f64(b+i);
		v_f64 v3 = v_add_f64(v1, v2);
		v_store_f64(c+i, v3);
	}
#else
	int vsteps = v_nlanes_f32;
	int n = len & -vsteps;
	for (; i < n; i+=vsteps) {
		v_f32 v1 = v_load_f32(a+i);
		v_f32 v2 = v_load_f32(b+i);
		v_f32 v3 = v_add_f32(v1, v2);
		v_store_f32(c+i, v3);
	}
#endif
	
#endif
	for(; i < len; i++){
		c[i] = a[i] + b[i];
	}
}