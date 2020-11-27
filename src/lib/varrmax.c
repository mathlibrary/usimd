#include "common.h"
#include "../simd/simd.h"

FLOAT_T usimd_arrmax(FLOAT_T *x, int n )
{
    int i = 0;
    FLOAT_T max = -INFINITY;
#if V_SIMD
#if defined(DOUBLE_T)
    const int vstep = v_nlanes_f64;
    const int unrollx = n & -vstep;
    FLOAT_T max_pair[8];
    v_f64 vt;
    v_f64 vmax = v_setall_f64(-INFINITY);
    for(i = 0; i < unrollx; i+=vstep) {
        vt = v_load_f64(x+i);
        vmax = v_max_f64(vt, vmax);
    }
    v_store_f64(max_pair, vmax);
    for(int j=0;j<vstep;j++) {
        max = fmax(max, max_pair[j]);
    }
#else
    const int vstep = v_nlanes_f32;
    const int unrollx = n & -vstep;
    FLOAT_T max_pair[16];
    v_f32 vt;
    v_f32 vmax = v_setall_f32(-INFINITY);
    for(i = 0; i < unrollx; i+=vstep) {
        vt = v_load_f32(x+i);
        vmax = v_max_f32(vt, vmax);
    }
    v_store_f32(max_pair, vmax);
    for(int j=0;j<vstep;j++) {
        max = fmax(max, max_pair[j]);
    }
#endif
#endif
    for(; i < n; i++) {
        max = fmax(max, x[i]);
    }
    return max;
}
