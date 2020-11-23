#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_SSE_ARITHMETIC_H
#define _V_SIMD_SSE_ARITHMETIC_H

/***************************
 * Addition
 ***************************/
// non-saturated
#define v_add_u8  _mm_add_epi8
#define v_add_s8  _mm_add_epi8
#define v_add_u16 _mm_add_epi16
#define v_add_s16 _mm_add_epi16
#define v_add_u32 _mm_add_epi32
#define v_add_s32 _mm_add_epi32
#define v_add_u64 _mm_add_epi64
#define v_add_s64 _mm_add_epi64
#define v_add_f32 _mm_add_ps
#define v_add_f64 _mm_add_pd

// saturated
#define v_adds_u8  _mm_adds_epu8
#define v_adds_s8  _mm_adds_epi8
#define v_adds_u16 _mm_adds_epu16
#define v_adds_s16 _mm_adds_epi16
// TODO: rest, after implment Packs intrins

/***************************
 * Subtraction
 ***************************/
// non-saturated
#define v_sub_u8  _mm_sub_epi8
#define v_sub_s8  _mm_sub_epi8
#define v_sub_u16 _mm_sub_epi16
#define v_sub_s16 _mm_sub_epi16
#define v_sub_u32 _mm_sub_epi32
#define v_sub_s32 _mm_sub_epi32
#define v_sub_u64 _mm_sub_epi64
#define v_sub_s64 _mm_sub_epi64
#define v_sub_f32 _mm_sub_ps
#define v_sub_f64 _mm_sub_pd

// saturated
#define v_subs_u8  _mm_subs_epu8
#define v_subs_s8  _mm_subs_epi8
#define v_subs_u16 _mm_subs_epu16
#define v_subs_s16 _mm_subs_epi16
// TODO: rest, after implment Packs intrins

/***************************
 * Multiplication
 ***************************/
// non-saturated
V_FINLINE __m128i v_mul_u8(__m128i a, __m128i b)
{
    const __m128i mask = _mm_set1_epi32(0xFF00FF00);
    __m128i even = _mm_mullo_epi16(a, b);
    __m128i odd  = _mm_mullo_epi16(_mm_srai_epi16(a, 8), _mm_srai_epi16(b, 8));
            odd  = _mm_slli_epi16(odd, 8);
    return v_select_u8(mask, odd, even);
}
#define v_mul_s8  v_mul_u8
#define v_mul_u16 _mm_mullo_epi16
#define v_mul_s16 _mm_mullo_epi16

#ifdef V_HAVE_SSE41
    #define v_mul_u32 _mm_mullo_epi32
#else
    V_FINLINE __m128i v_mul_u32(__m128i a, __m128i b)
    {
        __m128i even = _mm_mul_epu32(a, b);
        __m128i odd  = _mm_mul_epu32(_mm_srli_epi64(a, 32), _mm_srli_epi64(b, 32));
        __m128i low  = _mm_unpacklo_epi32(even, odd);
        __m128i high = _mm_unpackhi_epi32(even, odd);
        return _mm_unpacklo_epi64(low, high);
    }
#endif // V_HAVE_SSE41
#define v_mul_s32 v_mul_u32
// TODO: emulate 64-bit*/
#define v_mul_f32 _mm_mul_ps
#define v_mul_f64 _mm_mul_pd

#ifdef V_HAVE_SSE3
// complex type :(slower than pure C
V_FINLINE v_f64 v_cmul_f64(__m128d a, __m128d b)
{
  return _mm_addsub_pd(_mm_mul_pd(_mm_unpacklo_pd(a, a), b),
                       _mm_mul_pd(_mm_unpackhi_pd(a, a),
                                 _mm_shuffle_pd(b, b, 0b01)));
}
#endif

// saturated
// TODO: after implment Packs intrins

/***************************
 * Division
 ***************************/
// TODO: emulate integer division
#define v_div_f32 _mm_div_ps
#define v_div_f64 _mm_div_pd
/***************************
 * FUSED
 ***************************/
#ifdef V_HAVE_FMA3
    // multiply and add, a*b + c
    #define v_muladd_f32 _mm_fmadd_ps
    #define v_muladd_f64 _mm_fmadd_pd
    // multiply and subtract, a*b - c
    #define v_mulsub_f32 _mm_fmsub_ps
    #define v_mulsub_f64 _mm_fmsub_pd
    // negate multiply and add, -(a*b) + c
    #define v_nmuladd_f32 _mm_fnmadd_ps
    #define v_nmuladd_f64 _mm_fnmadd_pd
    // negate multiply and subtract, -(a*b) - c
    #define v_nmulsub_f32 _mm_fnmsub_ps
    #define v_nmulsub_f64 _mm_fnmsub_pd
#elif defined(V_HAVE_FMA4)
    // multiply and add, a*b + c
    #define v_muladd_f32 _mm_macc_ps
    #define v_muladd_f64 _mm_macc_pd
    // multiply and subtract, a*b - c
    #define v_mulsub_f32 _mm_msub_ps
    #define v_mulsub_f64 _mm_msub_pd
    // negate multiply and add, -(a*b) + c
    #define v_nmuladd_f32 _mm_nmacc_ps
    #define v_nmuladd_f64 _mm_nmacc_pd
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
#endif // V_HAVE_FMA3
#ifndef V_HAVE_FMA3 // for FMA4 and NON-FMA3
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
V_FINLINE float v_sum_f32(__m128 a)
{
#ifdef V_HAVE_SSE3
    __m128 sum_halves = _mm_hadd_ps(a, a);
    return _mm_cvtss_f32(_mm_hadd_ps(sum_halves, sum_halves));
#else
    __m128 t1 = _mm_movehl_ps(a, a);
    __m128 t2 = _mm_add_ps(a, t1);
    __m128 t3 = _mm_shuffle_ps(t2, t2, 1);
    __m128 t4 = _mm_add_ss(t2, t3);
    return _mm_cvtss_f32(t4); 
#endif
}

V_FINLINE double v_sum_f64(__m128d a)
{
#ifdef V_HAVE_SSE3
    return _mm_cvtsd_f64(_mm_hadd_pd(a, a));
#else
    return _mm_cvtsd_f64(_mm_add_pd(a, _mm_unpackhi_pd(a, a)));
#endif
}

#endif // _V_SIMD_SSE_ARITHMETIC_H


