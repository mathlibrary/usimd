
#include "common.h"
#include "../simd/simd.h"

void usimd_daxpy(int n, FLOAT_T *data0, FLOAT_T *data1, FLOAT_T *alpha)
{
    int i = n;
    FLOAT_T a = *alpha;
#if NPY_SIMD>128 && !defined(DOUBLE_T)
#ifdef DOUBLE_T
	int vstep = v_nlanes_f64;
	int vstepx4 = vstep*4;
    v_f64 __alpha = v_setall_f64(*alpha);
	for (; i >= vstepx4; i -= vstepx4, data0 += vstepx4, data1 += vstepx4) {
        v_f64 a0 = v_load_f64(data0);
		v_f64 b0 = v_load_f64(data1);
        v_f64 a1 = v_load_f64(data0 + vstep);
		v_f64 b1 = v_load_f64(data1 + vstep);
        v_f64 a2 = v_load_f64(data0 + vstep * 2);
		v_f64 b2 = v_load_f64(data1 + vstep * 2);
		v_f64 a3 = v_load_f64(data0 + vstep * 3);
		v_f64 b3 = v_load_f64(data1 + vstep * 3);
		v_f64 ab0 = v_muladd_f64(__alpha, a0, b0);
        v_f64 ab1 = v_muladd_f64(__alpha, a1, b1);
        v_f64 ab2 = v_muladd_f64(__alpha, a2, b2);
        v_f64 ab3 = v_muladd_f64(__alpha, a3, b3);
		v_store_f64(data1 , ab0);
		v_store_f64(data1 + vstep, ab1);
		v_store_f64(data1 + vstep * 2, ab2);
        v_store_f64(data1 + vstep * 3, ab3);
	}
#else
	int vstep = v_nlanes_f32;
	int vstepx4 = vstep*4;
    v_f32 __alpha = v_setall_f32(*alpha);
	for (; i >= vstepx4; i -= vstepx4, data0 += vstepx4, data1 += vstepx4) {
        v_f32 a0 = v_load_f32(data0);
		v_f32 b0 = v_load_f32(data1);
        v_f32 a1 = v_load_f32(data0 + vstep);
		v_f32 b1 = v_load_f32(data1 + vstep);
        v_f32 a2 = v_load_f32(data0 + vstep * 2);
		v_f32 b2 = v_load_f32(data1 + vstep * 2);
		v_f32 a3 = v_load_f32(data0 + vstep * 3);
		v_f32 b3 = v_load_f32(data1 + vstep * 3);
		v_f32 ab0 = v_muladd_f32(__alpha, a0, b0);
        v_f32 ab1 = v_muladd_f32(__alpha, a1, b1);
        v_f32 ab2 = v_muladd_f32(__alpha, a2, b2);
        v_f32 ab3 = v_muladd_f32(__alpha, a3, b3);
		v_store_f32(data1 , ab0);
		v_store_f32(data1 + vstep, ab1);
		v_store_f32(data1 + vstep * 2, ab2);
        v_store_f32(data1 + vstep * 3, ab3);
	}
#endif
#else
    for (; i >= 4; i -= 4, data0 += 4, data1 += 4)
    {
        data1[0] += a * data0[0];
        data1[1] += a * data0[1];
        data1[2] += a * data0[2];
        data1[3] += a * data0[3];
    }
#endif
    for (; i > 0; --i, ++data0, ++data1)
    {
        *data1 += a * (*data0);
    }
}