#include "common.h"
#include "../simd/simd.h"

FLOAT_T usimd_sum(int n, FLOAT_T *x, int inc_x)
{
	int i = 0;
	FLOAT_T sumf = 0.0;
	if (n <= 0 || inc_x <= 0)
		return (sumf);
	n *= inc_x;
	if (inc_x == 1)
	{
#if NPY_SIMD
#ifdef DOUBLE_T
		const int vstep = npyv_nlanes_f64;
		const int unrollx2 = n & (-vstep * 2);
		const int unrollx = n & -vstep;
		npyv_f64 vsum0 = npyv_zero_f64();
		npyv_f64 vsum1 = npyv_zero_f64();
		while (i < unrollx2)
		{
			vsum0 = npyv_add_f64(vsum0, npyv_load_f64(x));
			vsum1 = npyv_add_f64(vsum1, npyv_load_f64(x + vstep));
			i += vstep * 2;
		}
		vsum0 = npyv_add_f64(vsum0, vsum1);
		while (i < unrollx)
		{
			vsum0 = npyv_add_f64(vsum0, npyv_load_f64(x + i));
			i += vstep;
		}
		sumf = npyv_sum_f64(vsum0);
#else
		const int vstep = npyv_nlanes_f32;
		const int unrollx4 = n & (-vstep * 4);
		const int unrollx = n & -vstep;
		npyv_f32 vsum0 = npyv_zero_f32();
		npyv_f32 vsum1 = npyv_zero_f32();
		npyv_f32 vsum2 = npyv_zero_f32();
		npyv_f32 vsum3 = npyv_zero_f32();
		while (i < unrollx4)
		{
			vsum0 = npyv_add_f32(vsum0, npyv_load_f32(x));
			vsum1 = npyv_add_f32(vsum1, npyv_load_f32(x + vstep));
			vsum2 = npyv_add_f32(vsum2, npyv_load_f32(x + vstep * 2));
			vsum3 = npyv_add_f32(vsum3, npyv_load_f32(x + vstep * 3));
			i += vstep * 4;
		}
		vsum0 = npyv_add_f32(
			npyv_add_f32(vsum0, vsum1), npyv_add_f32(vsum2, vsum3));
		while (i < unrollx)
		{
			vsum0 = npyv_add_f32(vsum0, npyv_load_f32(x + i));
			i += vstep;
		}
		sumf = npyv_sum_f32(vsum0);
#endif
#else
		int n1 = n & -4;
		for (; i < n1; i += 4)
		{
			sumf += x[i] + x[i + 1] + x[i + 2] + x[i + 3];
		}
#endif
	}
	while (i < n)
	{
		sumf += x[i];
		i += inc_x;
	}
	return (sumf);
}