#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_VSX_MATH_H
#define _V_SIMD_VSX_MATH_H
/***************************
 * Elementary
 ***************************/
// Square root
#define v_sqrt_f32 vec_sqrt
#define v_sqrt_f64 vec_sqrt

// Reciprocal
V_FINLINE v_f32 v_recip_f32(v_f32 a)
{
    const v_f32 one = v_setall_f32(1.0f);
    return vec_div(one, a);
}
V_FINLINE v_f64 v_recip_f64(v_f64 a)
{
    const v_f64 one = v_setall_f64(1.0);
    return vec_div(one, a);
}

// Absolute
#define v_abs_f32 vec_abs
#define v_abs_f64 vec_abs

// Square
V_FINLINE v_f32 v_square_f32(v_f32 a)
{ return vec_mul(a, a); }
V_FINLINE v_f64 v_square_f64(v_f64 a)
{ return vec_mul(a, a); }

#endif // _V_SIMD_VSX_MATH_H
