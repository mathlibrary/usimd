#include<stdio.h>
#include "common.h"
#include "../simd/simd.h"

void usimd_muladd(const FLOAT_T *a, const FLOAT_T *b, FLOAT_T *c, int len)
{
    int i=0;
#if NPY_SIMD
#ifdef DOUBLE_T
	int vsteps = npyv_nlanes_f64;
	int n = len & -vsteps;
	for (; i < n; i+=vsteps) {
		npyv_f64 v1 = npyv_load_f64(a+i);
		npyv_f64 v2 = npyv_load_f64(b+i);
		npyv_f64 v3 = npyv_load_f64(c+i);
		npyv_f64 v4 = npyv_muladd_f64(v1, v2, v3);
		npyv_store_f64(c+i, v4);
	}
#else
	int vsteps = npyv_nlanes_f32;
	int n = len & -vsteps;
	for (; i < n; i+=vsteps) {
		npyv_f32 v1 = npyv_load_f32(a+i);
		npyv_f32 v2 = npyv_load_f32(b+i);
		npyv_f32 v3 = npyv_load_f32(c+i);
		npyv_f32 v4 = npyv_muladd_f32(v1, v2, v3);
		npyv_store_f32(c+i, v4);
	}
#endif
#endif
	for(; i < len; i++){
		c[i] += a[i] * b[i];
	}
}