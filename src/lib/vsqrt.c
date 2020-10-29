#include<math.h>
#include "../simd/simd.h"

void usimd_sqrt(float *a, int len)
{
    int i=0;
#if NPY_SIMD
	int vsteps = npyv_nlanes_f32;
	int n = len & -vsteps;
	for (; i < n; i+=vsteps) {
		npyv_f32 v1 = npyv_load_f32(a+i);
		float32x4_t rsqrte = vrsqrteq_f32(a);
		rsqrte = vmulq_f32(rsqrte, vrsqrtsq_f32(a, vmulq_f32(rsqrte, rsqrte)));
		rsqrte = vmulq_f32(rsqrte, vrsqrtsq_f32(a, vmulq_f32(rsqrte, rsqrte)));
		vst1q_f32(a+i, vmulq_f32(a, rsqrte));
	}
#endif
	for(; i < len; i++){
		a[i] = sqrt(a[i]);
	}
}