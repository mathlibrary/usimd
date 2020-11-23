#include <stdio.h>
#include "common.h"
#include "../simd/simd.h"
void usimd_cmul(const FLOAT_T *data0, const FLOAT_T *data1, FLOAT_T *data_out, int len)
{
	int i = 0;
	int n = 2*len;
#if V_SIMD > 128
#ifdef DOUBLE_T
	int vstep = v_nlanes_f64;
	const int unrollx4 = n & (-vstep * 4);
	const int unrollx = n & -vstep;
	v_f64 x0,x1,x2,x3;
	v_f64 y0,y1,y2,y3;
	for (; i < unrollx4; i += vstep*4)
	{
		x0=v_load_f64(data0 + i);
		y0=v_load_f64(data1 + i);
		x1=v_load_f64(data0 + i + vstep);
		y1=v_load_f64(data1 + i + vstep);
		x2=v_load_f64(data0 + i + vstep*2);
		y2=v_load_f64(data1 + i + vstep*2);
		x3=v_load_f64(data0 + i + vstep*3);
		y3=v_load_f64(data1 + i + vstep*3);
		v_store_f64(data_out + i,v_cmul_f64(x0, y0));
		v_store_f64(data_out + i + vstep,v_cmul_f64(x1, y1));
		v_store_f64(data_out + i + vstep*2,v_cmul_f64(x2, y2));
		v_store_f64(data_out + i + vstep*3,v_cmul_f64(x3, y3));
	}
	for (; i < unrollx; i += vstep)
	{
		x0=v_load_f64(data0 + i);
		y0=v_load_f64(data1 + i);
		v_store_f64(data_out + i,v_cmul_f64(x0, y0));
	}
#endif
#endif
	for (; i < n; i += 2)
	{
		data_out[i] = data0[i] * data1[i] - data0[i + 1] * data1[i + 1];
		data_out[i + 1] = data0[i] * data1[i + 1] + data0[i + 1] * data1[i];
	}
}