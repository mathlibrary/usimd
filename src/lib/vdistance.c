#include<math.h>
#include "common.h"
#include "../simd/simd.h"
#define BX 4
/* BenchTest
pure C + float
bench_distance with scale 100000000:
     830.13 MFlops   0.481854 sec
###############################
pure C + double
bench_distance with scale 100000000:
     825.77 MFlops   0.484394 sec
###############################
SSE2 enabled + float
bench_distance with scale 100000000:
    1600.56 MFlops   0.249912 sec
###############################
SSE2 enabled + double
bench_distance with scale 100000000:
     774.70 MFlops   0.516328 sec
###############################
SSE3 enabled + float
bench_distance with scale 100000000:
    1609.37 MFlops   0.248545 sec
###############################
SSE3 enabled + double
bench_distance with scale 100000000:
     798.88 MFlops   0.500703 sec
###############################
AVX2 enabled + float
bench_distance with scale 100000000:
    2946.65 MFlops   0.135747 sec
###############################
AVX2 enabled + double
bench_distance with scale 100000000:
    1500.86 MFlops   0.266514 sec
###############################
*/
FLOAT_T usimd_distance(int n, FLOAT_T *data0, FLOAT_T *data1)
{
    int i = n;
    FLOAT_T dis = 0.0f;
    FLOAT_T temp;
#if V_SIMD
#ifdef DOUBLE_T
	int vstep = v_nlanes_f64;
	int vstepx4 = vstep*4;
    v_f64 res0 = v_zero_f64();
    v_f64 res1 = v_zero_f64();
    v_f64 res2 = v_zero_f64();
    v_f64 res3 = v_zero_f64();
	for (; i >= vstepx4; i -= vstepx4, data0 += vstepx4, data1 += vstepx4) {
        v_f64 a0 = v_load_f64(data0);
		v_f64 b0 = v_load_f64(data1);
        v_f64 a1 = v_load_f64(data0 + vstep);
		v_f64 b1 = v_load_f64(data1 + vstep);
        v_f64 a2 = v_load_f64(data0 + vstep * 2);
		v_f64 b2 = v_load_f64(data1 + vstep * 2);
		v_f64 a3 = v_load_f64(data0 + vstep * 3);
		v_f64 b3 = v_load_f64(data1 + vstep * 3);
		v_f64 ab0 = v_sub_f64(a0, b0);
        v_f64 ab1 = v_sub_f64(a1, b1);
        v_f64 ab2 = v_sub_f64(a2, b2);
        v_f64 ab3 = v_sub_f64(a3, b3);
        res0 = v_muladd_f64(ab0, ab0, res0);
        res1 = v_muladd_f64(ab1, ab1, res1);
        res2 = v_muladd_f64(ab2, ab2, res2);
        res3 = v_muladd_f64(ab3, ab3, res3);
	}
    res0 = v_add_f64(v_add_f64(res0,res1), v_add_f64(res2,res3));
    dis = v_sum_f64(res0);
#else
	int vstep = v_nlanes_f32;
	int vstepx4 = vstep*4;
    v_f32 res0 = v_zero_f32();
    v_f32 res1 = v_zero_f32();
    v_f32 res2 = v_zero_f32();
    v_f32 res3 = v_zero_f32();
	for (; i >= vstepx4; i -= vstepx4, data0 += vstepx4, data1 += vstepx4) {
        v_f32 a0 = v_load_f32(data0);
		v_f32 b0 = v_load_f32(data1);
        v_f32 a1 = v_load_f32(data0 + vstep);
		v_f32 b1 = v_load_f32(data1 + vstep);
        v_f32 a2 = v_load_f32(data0 + vstep * 2);
		v_f32 b2 = v_load_f32(data1 + vstep * 2);
		v_f32 a3 = v_load_f32(data0 + vstep * 3);
		v_f32 b3 = v_load_f32(data1 + vstep * 3);
		v_f32 ab0 = v_sub_f32(a0, b0);
        v_f32 ab1 = v_sub_f32(a1, b1);
        v_f32 ab2 = v_sub_f32(a2, b2);
        v_f32 ab3 = v_sub_f32(a3, b3);
        res0 = v_muladd_f32(ab0, ab0, res0);
        res1 = v_muladd_f32(ab1, ab1, res1);
        res2 = v_muladd_f32(ab2, ab2, res2);
        res3 = v_muladd_f32(ab3, ab3, res3);
	}
    res0 = v_add_f32(v_add_f32(res0,res1), v_add_f32(res2,res3));
    dis = v_sum_f32(res0);
#endif
#else
    FLOAT_T resarr[4] = {0.0};
    FLOAT_T temparr[4];
    for (; i >= BX; i -= BX, data0 += BX, data1 += BX)
    {
        for(int j = 0;j<BX;j++){
            temparr[j] = (*(data0+j)) - (*(data1+j));
            resarr[j] += temparr[j]*temparr[j];
        }
    }
    for(int j = 0;j<BX;j++) {
        dis += resarr[j];
    }
#endif
    
    for(;i > 0; data0++, data1++, i--)
    {
        temp = *data0 - *data1;
        dis += temp*temp;
    }
    return sqrt(dis);
}