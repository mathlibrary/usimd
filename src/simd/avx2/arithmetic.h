#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX2_ARITHMETIC_H
#define _V_SIMD_AVX2_ARITHMETIC_H

/***************************
 * Addition
 ***************************/
// non-saturated
#define v_add_u8  _mm256_add_epi8
#define v_add_s8  _mm256_add_epi8
#define v_add_u16 _mm256_add_epi16
#define v_add_s16 _mm256_add_epi16
#define v_add_u32 _mm256_add_epi32
#define v_add_s32 _mm256_add_epi32
#define v_add_u64 _mm256_add_epi64
#define v_add_s64 _mm256_add_epi64
#define v_add_f32 _mm256_add_ps
#define v_add_f64 _mm256_add_pd

// saturated
#define v_adds_u8  _mm256_adds_epu8
#define v_adds_s8  _mm256_adds_epi8
#define v_adds_u16 _mm256_adds_epu16
#define v_adds_s16 _mm256_adds_epi16
// TODO: rest, after implment Packs intrins

/***************************
 * Subtraction
 ***************************/
// non-saturated
#define v_sub_u8  _mm256_sub_epi8
#define v_sub_s8  _mm256_sub_epi8
#define v_sub_u16 _mm256_sub_epi16
#define v_sub_s16 _mm256_sub_epi16
#define v_sub_u32 _mm256_sub_epi32
#define v_sub_s32 _mm256_sub_epi32
#define v_sub_u64 _mm256_sub_epi64
#define v_sub_s64 _mm256_sub_epi64
#define v_sub_f32 _mm256_sub_ps
#define v_sub_f64 _mm256_sub_pd

// saturated
#define v_subs_u8  _mm256_subs_epu8
#define v_subs_s8  _mm256_subs_epi8
#define v_subs_u16 _mm256_subs_epu16
#define v_subs_s16 _mm256_subs_epi16
// TODO: rest, after implment Packs intrins

/***************************
 * Multiplication
 ***************************/
// non-saturated
#define v_mul_u8  v256_mul_u8
#define v_mul_s8  v_mul_u8
#define v_mul_u16 _mm256_mullo_epi16
#define v_mul_s16 _mm256_mullo_epi16
#define v_mul_u32 _mm256_mullo_epi32
#define v_mul_s32 _mm256_mullo_epi32
#define v_mul_f32 _mm256_mul_ps
#define v_mul_f64 _mm256_mul_pd

// complex type
V_FINLINE v_f64 v_cmul_f64(__m256d a, __m256d b, int isconj)
{
    v_f64 r=_mm256_permute_pd(b,5)*_mm256_permute_pd(a,15);
    if(isconj > 0) {
        return _mm256_fmaddsub_pd(b,_mm256_movedup_pd(a),r);
    }
	return _mm256_fmsubadd_pd(b,_mm256_movedup_pd(a),r);
}

// saturated
// TODO: after implment Packs intrins

/***************************
 * Division
 ***************************/
// TODO: emulate integer division
#define v_div_f32 _mm256_div_ps
#define v_div_f64 _mm256_div_pd

/***************************
 * FUSED
 ***************************/
#ifdef V_HAVE_FMA3
    // multiply and add, a*b + c
    #define v_muladd_f32 _mm256_fmadd_ps
    #define v_muladd_f64 _mm256_fmadd_pd
    // multiply and subtract, a*b - c
    #define v_mulsub_f32 _mm256_fmsub_ps
    #define v_mulsub_f64 _mm256_fmsub_pd
    // negate multiply and add, -(a*b) + c
    #define v_nmuladd_f32 _mm256_fnmadd_ps
    #define v_nmuladd_f64 _mm256_fnmadd_pd
    // negate multiply and subtract, -(a*b) - c
    #define v_nmulsub_f32 _mm256_fnmsub_ps
    #define v_nmulsub_f64 _mm256_fnmsub_pd
#else
    // multiply and add, a*b + c
    V_FINLINE v_f32 v_muladd_f32(v_f32 a, v_f32 b, v_f32 c)
    { return v_add_f32(v_mul_f32(a, b), c); }
    V_FINLINE v_f64 v_muladd_f64(v_f64 a, v_f64 b, v_f64 c)
    { return v_add_f64(v_mul_f64(a, b), c); }
    // multiply and subtract, a*b - c
    V_FINLINE v_f32 v_mulsub_f32(v_f32 a, v_f32 b, v_f32 c)
    { return v_sub_f32(v_mul_f32(a, b), c); }
    V_FINLINE v_f64 v_mulsub_f64(v_f64 a, v_f64 b, v_f64 c)
    { return v_sub_f64(v_mul_f64(a, b), c); }
    // negate multiply and add, -(a*b) + c
    V_FINLINE v_f32 v_nmuladd_f32(v_f32 a, v_f32 b, v_f32 c)
    { return v_sub_f32(c, v_mul_f32(a, b)); }
    V_FINLINE v_f64 v_nmuladd_f64(v_f64 a, v_f64 b, v_f64 c)
    { return v_sub_f64(c, v_mul_f64(a, b)); }
    // negate multiply and subtract, -(a*b) - c
    V_FINLINE v_f32 v_nmulsub_f32(v_f32 a, v_f32 b, v_f32 c)
    {
        v_f32 neg_a = v_xor_f32(a, v_setall_f32(-0.0f));
        return v_sub_f32(v_mul_f32(neg_a, b), c);
    }
    V_FINLINE v_f64 v_nmulsub_f64(v_f64 a, v_f64 b, v_f64 c)
    {
        v_f64 neg_a = v_xor_f64(a, v_setall_f64(-0.0));
        return v_sub_f64(v_mul_f64(neg_a, b), c);
    }
#endif // !V_HAVE_FMA3

// Horizontal add: Calculates the sum of all vector elements.
V_FINLINE float v_sum_f32(__m256 a)
{
    __m256 sum_halves = _mm256_hadd_ps(a, a);
    sum_halves = _mm256_hadd_ps(sum_halves, sum_halves);
    __m128 lo = _mm256_castps256_ps128(sum_halves);
    __m128 hi = _mm256_extractf128_ps(sum_halves, 1);
    __m128 sum = _mm_add_ps(lo, hi);
    return _mm_cvtss_f32(sum);
}

V_FINLINE double v_sum_f64(__m256d a)
{
    __m256d sum_halves = _mm256_hadd_pd(a, a);
    __m128d lo = _mm256_castpd256_pd128(sum_halves);
    __m128d hi = _mm256_extractf128_pd(sum_halves, 1);
    __m128d sum = _mm_add_pd(lo, hi);
    return _mm_cvtsd_f64(sum);
}

// Horizontally add adjacent pairs of single-precision (32-bit) floating-point elements
#define v_hadd_f32 _mm256_hadd_ps
#define v_hadd_f64 _mm256_hadd_pd

#endif // _V_SIMD_AVX2_ARITHMETIC_H


