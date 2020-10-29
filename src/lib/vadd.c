#include<stdio.h>
#include "../simd/simd.h"

void usimd_add(const float *a, const float *b, float *c, int len)
{
    int i=0;
#if NPY_SIMD
	int vsteps = npyv_nlanes_f32;
	int n = len & -vsteps;
	for (; i < n; i+=vsteps) {
		npyv_f32 v1 = npyv_load_f32(a+i);
		npyv_f32 v2 = npyv_load_f32(b+i);
		npyv_f32 v3 = npyv_add_f32(v1, v2);
		npyv_store_f32(c+i, v3);
	}
#endif
	for(; i < len; i++){
		c[i] = a[i] + b[i];
	}
}