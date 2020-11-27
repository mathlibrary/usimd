#include "common.h"
#include "../simd/simd.h"

void usimd_addeven(const FLOAT_T *x, FLOAT_T *y, int len)
{
    int i=0;
#if defined(V_HAVE_SSE3) || defined(V_HAVE_AVX2)
#ifdef DOUBLE_T
	int vstep = v_nlanes_f64;
	int unrollx = len & -vstep;
	v_f64 half, v1, v2, avg;
	half = v_setall_f64(0.5); // set vector to all 0.5
	for(int i = 0; i < unrollx/2; i+=vstep*2) {
		v1 = v_load_f64(x+i); // load first 4 floats
		v2 = v_load_f64(x+i+vstep); // load next 4 floats
		avg = v_hadd_f64(v1, v2); // add pairs of floats
		avg = v_mul_f64(avg, half); // multiply with 0.5
		v_store_f64(y+i/2, avg); // save result
	}
#else
	int vstep = v_nlanes_f32;
	int unrollx = len & -vstep;
	v_f32 half, v1, v2, avg;
	half = v_setall_f32(0.5); // set vector to all 0.5
	for(int i = 0; i < unrollx/2; i+=vstep*2) {
		v1 = v_load_f32(x+i); // load first 4 floats
		v2 = v_load_f32(x+i+vstep); // load next 4 floats
		avg = v_hadd_f32(v1, v2); // add pairs of floats
		avg = v_mul_f32(avg, half); // multiply with 0.5
		v_store_f32(y+i/2, avg); // save result
	}
#endif
#endif
	for(; i < len/2; i++){
		y[i] = (x[2*i] + x[2*i + 1])/2;
	}
}
