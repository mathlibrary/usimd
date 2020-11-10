
#include "common.h"
#include "../simd/simd.h"

void usimd_daxpy(int n, FLOAT_T *data0, FLOAT_T *data1, FLOAT_T *alpha)
{
    int i = n;
    FLOAT_T a = *alpha;
#if NPY_SIMD>128 && !defined(DOUBLE_T)
#ifdef DOUBLE_T
	int vstep = npyv_nlanes_f64;
	int vstepx4 = vstep*4;
    npyv_f64 __alpha = npyv_setall_f64(*alpha);
	for (; i >= vstepx4; i -= vstepx4, data0 += vstepx4, data1 += vstepx4) {
        npyv_f64 a0 = npyv_load_f64(data0);
		npyv_f64 b0 = npyv_load_f64(data1);
        npyv_f64 a1 = npyv_load_f64(data0 + vstep);
		npyv_f64 b1 = npyv_load_f64(data1 + vstep);
        npyv_f64 a2 = npyv_load_f64(data0 + vstep * 2);
		npyv_f64 b2 = npyv_load_f64(data1 + vstep * 2);
		npyv_f64 a3 = npyv_load_f64(data0 + vstep * 3);
		npyv_f64 b3 = npyv_load_f64(data1 + vstep * 3);
		npyv_f64 ab0 = npyv_muladd_f64(__alpha, a0, b0);
        npyv_f64 ab1 = npyv_muladd_f64(__alpha, a1, b1);
        npyv_f64 ab2 = npyv_muladd_f64(__alpha, a2, b2);
        npyv_f64 ab3 = npyv_muladd_f64(__alpha, a3, b3);
		npyv_store_f64(data1 , ab0);
		npyv_store_f64(data1 + vstep, ab1);
		npyv_store_f64(data1 + vstep * 2, ab2);
        npyv_store_f64(data1 + vstep * 3, ab3);
	}
#else
	int vstep = npyv_nlanes_f32;
	int vstepx4 = vstep*4;
    npyv_f32 __alpha = npyv_setall_f32(*alpha);
	for (; i >= vstepx4; i -= vstepx4, data0 += vstepx4, data1 += vstepx4) {
        npyv_f32 a0 = npyv_load_f32(data0);
		npyv_f32 b0 = npyv_load_f32(data1);
        npyv_f32 a1 = npyv_load_f32(data0 + vstep);
		npyv_f32 b1 = npyv_load_f32(data1 + vstep);
        npyv_f32 a2 = npyv_load_f32(data0 + vstep * 2);
		npyv_f32 b2 = npyv_load_f32(data1 + vstep * 2);
		npyv_f32 a3 = npyv_load_f32(data0 + vstep * 3);
		npyv_f32 b3 = npyv_load_f32(data1 + vstep * 3);
		npyv_f32 ab0 = npyv_muladd_f32(__alpha, a0, b0);
        npyv_f32 ab1 = npyv_muladd_f32(__alpha, a1, b1);
        npyv_f32 ab2 = npyv_muladd_f32(__alpha, a2, b2);
        npyv_f32 ab3 = npyv_muladd_f32(__alpha, a3, b3);
		npyv_store_f32(data1 , ab0);
		npyv_store_f32(data1 + vstep, ab1);
		npyv_store_f32(data1 + vstep * 2, ab2);
        npyv_store_f32(data1 + vstep * 3, ab3);
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