#include<math.h>
#include "../simd/simd.h"
#include "common.h"

void usimd_sqrt(FLOAT_T *a, int len)
{
    int i=0;
#if NPY_SIMD
#if NPY_HAVE_NEON && !defined(DOUBLE_T)
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
#endif
	for(; i < len; i++){
		a[i] = sqrt(a[i]);
	}
}

FLOAT_T Q_sqrt( FLOAT_T number )
{
	long i;
	FLOAT_T x2, y;
	const FLOAT_T threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil FLOAT_Ting point bit level hacking（对浮点数的邪恶位元hack）
	i  = 0x5f3759df - ( i >> 1 );
    // i  = 0x5f375a86 - ( i >> 1 );               // what the fuck?（这他妈的是怎么回事？）
	y  = * ( FLOAT_T * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration （第一次迭代）
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed（第二次迭代，可以删除）
	return number * y;
}