#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX2_MATH_H
#define _V_SIMD_AVX2_MATH_H
/***************************
 * Elementary
 ***************************/
// Square root
#define v_sqrt_f32 _mm256_sqrt_ps
#define v_sqrt_f64 _mm256_sqrt_pd

// Reciprocal
V_FINLINE v_f32 v_recip_f32(v_f32 a)
{ return _mm256_div_ps(_mm256_set1_ps(1.0f), a); }
V_FINLINE v_f64 v_recip_f64(v_f64 a)
{ return _mm256_div_pd(_mm256_set1_pd(1.0), a); }

// Absolute
V_FINLINE v_f32 v_abs_f32(v_f32 a)
{
    return _mm256_and_ps(
        a, _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff))
    );
}
V_FINLINE v_f64 v_abs_f64(v_f64 a)
{
    return _mm256_and_pd(
        a, _mm256_castsi256_pd(v_setall_s64(0x7fffffffffffffffLL))
    );
}

// Square
V_FINLINE v_f32 v_square_f32(v_f32 a)
{ return _mm256_mul_ps(a, a); }
V_FINLINE v_f64 v_square_f64(v_f64 a)
{ return _mm256_mul_pd(a, a); }

#endif
