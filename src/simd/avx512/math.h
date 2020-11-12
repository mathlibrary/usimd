#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX512_MATH_H
#define _V_SIMD_AVX512_MATH_H

/***************************
 * Elementary
 ***************************/
// Square root
#define v_sqrt_f32 _mm512_sqrt_ps
#define v_sqrt_f64 _mm512_sqrt_pd

// Reciprocal
V_FINLINE v_f32 v_recip_f32(v_f32 a)
{ return _mm512_div_ps(_mm512_set1_ps(1.0f), a); }
V_FINLINE v_f64 v_recip_f64(v_f64 a)
{ return _mm512_div_pd(_mm512_set1_pd(1.0), a); }

// Absolute
V_FINLINE v_f32 v_abs_f32(v_f32 a)
{
#if 0 // def V_HAVE_AVX512DQ
    return _mm512_range_ps(a, a, 8);
#else
    return v_and_f32(
        a, _mm512_castsi512_ps(_mm512_set1_epi32(0x7fffffff))
    );
#endif
}
V_FINLINE v_f64 v_abs_f64(v_f64 a)
{
#if 0 // def V_HAVE_AVX512DQ
    return _mm512_range_pd(a, a, 8);
#else
    return v_and_f64(
        a, _mm512_castsi512_pd(_mm512_set1_epi64(0x7fffffffffffffffLL))
    );
#endif
}

// Square
V_FINLINE v_f32 v_square_f32(v_f32 a)
{ return _mm512_mul_ps(a, a); }
V_FINLINE v_f64 v_square_f64(v_f64 a)
{ return _mm512_mul_pd(a, a); }

#endif
