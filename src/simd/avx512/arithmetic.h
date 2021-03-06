#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX512_ARITHMETIC_H
#define _V_SIMD_AVX512_ARITHMETIC_H

#include "../avx2/utils.h"

/***************************
 * Addition
 ***************************/
// non-saturated
#ifdef V_HAVE_AVX512BW
    #define v_add_u8  _mm512_add_epi8
    #define v_add_u16 _mm512_add_epi16
#else
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_add_u8,  _mm256_add_epi8)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_add_u16, _mm256_add_epi16)
#endif
#define v_add_s8  v_add_u8
#define v_add_s16 v_add_u16
#define v_add_u32 _mm512_add_epi32
#define v_add_s32 _mm512_add_epi32
#define v_add_u64 _mm512_add_epi64
#define v_add_s64 _mm512_add_epi64
#define v_add_f32 _mm512_add_ps
#define v_add_f64 _mm512_add_pd

// saturated
#ifdef V_HAVE_AVX512BW
    #define v_adds_u8  _mm512_adds_epu8
    #define v_adds_s8  _mm512_adds_epi8
    #define v_adds_u16 _mm512_adds_epu16
    #define v_adds_s16 _mm512_adds_epi16
#else
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_adds_u8,  _mm256_adds_epu8)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_adds_s8,  _mm256_adds_epi8)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_adds_u16, _mm256_adds_epu16)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_adds_s16, _mm256_adds_epi16)
#endif
// TODO: rest, after implment Packs intrins

/***************************
 * Subtraction
 ***************************/
// non-saturated
#ifdef V_HAVE_AVX512BW
    #define v_sub_u8  _mm512_sub_epi8
    #define v_sub_u16 _mm512_sub_epi16
#else
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_sub_u8,  _mm256_sub_epi8)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_sub_u16, _mm256_sub_epi16)
#endif
#define v_sub_s8  v_sub_u8
#define v_sub_s16 v_sub_u16
#define v_sub_u32 _mm512_sub_epi32
#define v_sub_s32 _mm512_sub_epi32
#define v_sub_u64 _mm512_sub_epi64
#define v_sub_s64 _mm512_sub_epi64
#define v_sub_f32 _mm512_sub_ps
#define v_sub_f64 _mm512_sub_pd

// saturated
#ifdef V_HAVE_AVX512BW
    #define v_subs_u8  _mm512_subs_epu8
    #define v_subs_s8  _mm512_subs_epi8
    #define v_subs_u16 _mm512_subs_epu16
    #define v_subs_s16 _mm512_subs_epi16
#else
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_subs_u8,  _mm256_subs_epu8)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_subs_s8,  _mm256_subs_epi8)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_subs_u16, _mm256_subs_epu16)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_subs_s16, _mm256_subs_epi16)
#endif
// TODO: rest, after implment Packs intrins

/***************************
 * Multiplication
 ***************************/
// non-saturated
#ifdef V_HAVE_AVX512BW
V_FINLINE __m512i v_mul_u8(__m512i a, __m512i b)
{
    __m512i even = _mm512_mullo_epi16(a, b);
    __m512i odd  = _mm512_mullo_epi16(_mm512_srai_epi16(a, 8), _mm512_srai_epi16(b, 8));
            odd  = _mm512_slli_epi16(odd, 8);
    return _mm512_mask_blend_epi8(0xAAAAAAAAAAAAAAAA, even, odd);
}
#else
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_mul_u8, v256_mul_u8)
#endif

#ifdef V_HAVE_AVX512BW
    #define v_mul_u16 _mm512_mullo_epi16
#else
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_mul_u16, _mm256_mullo_epi16)
#endif
#define v_mul_s8  v_mul_u8
#define v_mul_s16 v_mul_u16
#define v_mul_u32 _mm512_mullo_epi32
#define v_mul_s32 _mm512_mullo_epi32
#define v_mul_f32 _mm512_mul_ps
#define v_mul_f64 _mm512_mul_pd

// complex types
V_FINLINE v_f64 v_cmul_f64(__m512d a, __m512d b)
{
    v_f64 r=_mm512_permute_pd(b,85)*_mm512_permute_pd(a,127);
	return _mm512_fmaddsub_pd(b,_mm512_movedup_pd(a),r);
}

// saturated
// TODO: after implment Packs intrins

/***************************
 * Division
 ***************************/
// TODO: emulate integer division
#define v_div_f32 _mm512_div_ps
#define v_div_f64 _mm512_div_pd

/***************************
 * FUSED
 ***************************/
// multiply and add, a*b + c
#define v_muladd_f32 _mm512_fmadd_ps
#define v_muladd_f64 _mm512_fmadd_pd
// multiply and subtract, a*b - c
#define v_mulsub_f32 _mm512_fmsub_ps
#define v_mulsub_f64 _mm512_fmsub_pd
// negate multiply and add, -(a*b) + c
#define v_nmuladd_f32 _mm512_fnmadd_ps
#define v_nmuladd_f64 _mm512_fnmadd_pd
// negate multiply and subtract, -(a*b) - c
#define v_nmulsub_f32 _mm512_fnmsub_ps
#define v_nmulsub_f64 _mm512_fnmsub_pd

/***************************
 * Reduce Sum
 * there are three ways to implement reduce sum for AVX512:
 * 1- split(256) /add /split(128) /add /hadd /hadd /extract
 * 2- shuff(cross) /add /shuff(cross) /add /shuff /add /shuff /add /extract
 * 3- _mm512_reduce_add_ps/pd
 * The first one is been widely used by many projects
 * 
 * the second one is used by Intel Compiler, maybe because the
 * latency of hadd increased by (2-3) starting from Skylake-X which makes two
 * extra shuffles(non-cross) cheaper. check https://godbolt.org/z/s3G9Er for more info.
 * 
 * The third one is almost the same as the second one but only works for
 * intel compiler/GCC 7.1/Clang 4, we still need to support older GCC.
 ***************************/
V_FINLINE int v_sum_u32(__m512i a)
{
    __m256i half = _mm256_add_epi32(_mm512_castsi512_si256(a), _mm512_extracti32x8_epi32(a, 1));
    __m128i quarter = _mm_add_epi32(_mm256_castsi256_si128(half), _mm256_extracti128_si256(half, 1));
    quarter = _mm_hadd_epi32(quarter, quarter);
    return _mm_cvtsi128_si32(_mm_hadd_epi32(quarter, quarter));
}

#ifdef V_HAVE_AVX512F_REDUCE
    #define v_sum_f32 _mm512_reduce_add_ps
    #define v_sum_f64 _mm512_reduce_add_pd
#else
    V_FINLINE float v_sum_f32(v_f32 a)
    {
        __m512 h64   = _mm512_shuffle_f32x4(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        __m512 sum32 = _mm512_add_ps(a, h64);
        __m512 h32   = _mm512_shuffle_f32x4(sum32, sum32, _MM_SHUFFLE(1, 0, 3, 2));
        __m512 sum16 = _mm512_add_ps(sum32, h32);
        __m512 h16   = _mm512_permute_ps(sum16, _MM_SHUFFLE(1, 0, 3, 2));
        __m512 sum8  = _mm512_add_ps(sum16, h16);
        __m512 h4    = _mm512_permute_ps(sum8, _MM_SHUFFLE(2, 3, 0, 1));
        __m512 sum4  = _mm512_add_ps(sum8, h4);
        return _mm_cvtss_f32(_mm512_castps512_ps128(sum4));
    }
    V_FINLINE double v_sum_f64(v_f64 a)
    {
        __m512d h64   = _mm512_shuffle_f64x2(a, a, _MM_SHUFFLE(3, 2, 3, 2));
        __m512d sum32 = _mm512_add_pd(a, h64);
        __m512d h32   = _mm512_permutex_pd(sum32, _MM_SHUFFLE(1, 0, 3, 2));
        __m512d sum16 = _mm512_add_pd(sum32, h32);
        __m512d h16   = _mm512_permute_pd(sum16, _MM_SHUFFLE(2, 3, 0, 1));
        __m512d sum8  = _mm512_add_pd(sum16, h16);
        return _mm_cvtsd_f64(_mm512_castpd512_pd128(sum8));
    }
#endif

#endif // _V_SIMD_AVX512_ARITHMETIC_H
