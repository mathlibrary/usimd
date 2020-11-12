#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_NEON_MATH_H
#define _V_SIMD_NEON_MATH_H

/***************************
 * Elementary
 ***************************/
// Absolute
#define v_abs_f32 vabsq_f32
#define v_abs_f64 vabsq_f64

// Square
V_FINLINE v_f32 v_square_f32(v_f32 a)
{ return vmulq_f32(a, a); }
#if V_SIMD_F64
    V_FINLINE v_f64 v_square_f64(v_f64 a)
    { return vmulq_f64(a, a); }
#endif

// Square root
#if V_SIMD_F64
    #define v_sqrt_f32 vsqrtq_f32
    #define v_sqrt_f64 vsqrtq_f64
#else
    // Based on ARM doc, see https://developer.arm.com/documentation/dui0204/j/CIHDIACI
    V_FINLINE v_f32 v_sqrt_f32(v_f32 a)
    {
        const v_f32 zero = vdupq_n_f32(0.0f);
        const v_u32 pinf = vdupq_n_u32(0x7f800000);
        v_u32 is_zero = vceqq_f32(a, zero), is_inf = vceqq_u32(vreinterpretq_u32_f32(a), pinf);
        // guard agianst floating-point division-by-zero error
        v_f32 guard_byz = vbslq_f32(is_zero, vreinterpretq_f32_u32(pinf), a);
        // estimate to (1/√a)
        v_f32 rsqrte = vrsqrteq_f32(guard_byz);
        /**
         * Newton-Raphson iteration:
         *  x[n+1] = x[n] * (3-d * (x[n]*x[n]) )/2)
         * converges to (1/√d)if x0 is the result of VRSQRTE applied to d.
         *
         * NOTE: at least 3 iterations is needed to improve precision
         */
        rsqrte = vmulq_f32(vrsqrtsq_f32(vmulq_f32(a, rsqrte), rsqrte), rsqrte);
        rsqrte = vmulq_f32(vrsqrtsq_f32(vmulq_f32(a, rsqrte), rsqrte), rsqrte);
        rsqrte = vmulq_f32(vrsqrtsq_f32(vmulq_f32(a, rsqrte), rsqrte), rsqrte);
        // a * (1/√a)
        v_f32 sqrt = vmulq_f32(a, rsqrte);
        // return zero if the a is zero
        // - return zero if a is zero.
        // - return positive infinity if a is positive infinity
        return vbslq_f32(vorrq_u32(is_zero, is_inf), a, sqrt);
    }
#endif // V_SIMD_F64

// Reciprocal
V_FINLINE v_f32 v_recip_f32(v_f32 a)
{
#if V_SIMD_F64
    const v_f32 one = vdupq_n_f32(1.0f);
    return v_div_f32(one, a);
#else
    v_f32 recipe = vrecpeq_f32(a);
    /**
     * Newton-Raphson iteration:
     *  x[n+1] = x[n] * (2-d * x[n])
     * converges to (1/d) if x0 is the result of VRECPE applied to d.
     *
     * NOTE: at least 3 iterations is needed to improve precision
     */
    recipe = vmulq_f32(vrecpsq_f32(a, recipe), recipe);
    recipe = vmulq_f32(vrecpsq_f32(a, recipe), recipe);
    recipe = vmulq_f32(vrecpsq_f32(a, recipe), recipe);
    return recipe;
#endif
}
#if V_SIMD_F64
    V_FINLINE v_f64 v_recip_f64(v_f64 a)
    {
        const v_f64 one = vdupq_n_f64(1.0);
        return v_div_f64(one, a);
    }
#endif // V_SIMD_F64

#endif // _V_SIMD_SSE_MATH_H
