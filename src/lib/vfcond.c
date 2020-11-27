#include "common.h"
#include "../simd/simd.h"

void usimd_fcond(FLOAT_T *x, int n )
{
    int i = 0;
#if V_SIMD
#if defined(DOUBLE_T)
    const int vstep = v_nlanes_f64;
    const int unrollx = n & -vstep;
    v_f64 vt, vmask, vp, vm, vr, ones, mones, thresholds;
    ones = v_setall_f64(1.);
    mones = v_setall_f64(-1.);
    thresholds = v_setall_f64(0.5);
    for(i = 0; i < unrollx; i+=vstep) {
        vt = v_load_f64(x+i);
        vmask = v_cmpgt_f64(vt, thresholds);
        vp = v_and_f64(vmask, ones);
        vm = v_andnot_f64(vmask, mones);
        vr = v_add_f64(vt, v_or_f64(vp, vm));
        v_store_f64(x+i, vr);
    }
#else
    const int vstep = v_nlanes_f32;
    const int unrollx = n & -vstep;
    v_f32 vt, vmask, vp, vm, vr, ones, mones, thresholds;
    ones = v_setall_f32(1.);
    mones = v_setall_f32(-1.);
    thresholds = v_setall_f32(0.5);
    for(i = 0; i < unrollx; i+=vstep) {
        vt = v_load_f32(x+i);
        vmask = v_cmpgt_f32(vt, thresholds);
        vp = v_and_f32(vmask, ones);
        vm = v_andnot_f32(vmask, mones);
        vr = v_add_f32(vt, v_or_f32(vp, vm));
        v_store_f32(x+i, vr);
    }
#endif
#endif
    for(; i < n; i++) {
        if(x[i] > 0.5){
            x[i] += 1.;
        } else {
            x[i] -= 1.;
        }
    }
}
