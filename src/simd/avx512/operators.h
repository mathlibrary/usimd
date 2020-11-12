#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX512_OPERATORS_H
#define _V_SIMD_AVX512_OPERATORS_H

/***************************
 * Shifting
 ***************************/

// left
#ifdef V_HAVE_AVX512BW
    #define v_shl_u16(A, C) _mm512_sll_epi16(A, _mm_cvtsi32_si128(C))
#else
    #define V_IMPL_AVX512_SHIFT(FN, INTRIN)          \
        V_FINLINE __m512i v_##FN(__m512i a, int c) \
        {                                               \
            __m256i l  = v512_lower_si256(a);        \
            __m256i h  = v512_higher_si256(a);       \
            __m128i cv = _mm_cvtsi32_si128(c);          \
            l = _mm256_##INTRIN(l, cv);                 \
            h = _mm256_##INTRIN(h, cv);                 \
            return v512_combine_si256(l, h);         \
        }

    V_IMPL_AVX512_SHIFT(shl_u16, sll_epi16)
#endif
#define v_shl_s16 v_shl_u16
#define v_shl_u32(A, C) _mm512_sll_epi32(A, _mm_cvtsi32_si128(C))
#define v_shl_s32(A, C) _mm512_sll_epi32(A, _mm_cvtsi32_si128(C))
#define v_shl_u64(A, C) _mm512_sll_epi64(A, _mm_cvtsi32_si128(C))
#define v_shl_s64(A, C) _mm512_sll_epi64(A, _mm_cvtsi32_si128(C))

// left by an immediate constant
#ifdef V_HAVE_AVX512BW
    #define v_shli_u16 _mm512_slli_epi16
#else
    #define v_shli_u16 v_shl_u16
#endif
#define v_shli_s16  v_shl_u16
#define v_shli_u32 _mm512_slli_epi32
#define v_shli_s32 _mm512_slli_epi32
#define v_shli_u64 _mm512_slli_epi64
#define v_shli_s64 _mm512_slli_epi64

// right
#ifdef V_HAVE_AVX512BW
    #define v_shr_u16(A, C) _mm512_srl_epi16(A, _mm_cvtsi32_si128(C))
    #define v_shr_s16(A, C) _mm512_sra_epi16(A, _mm_cvtsi32_si128(C))
#else
    V_IMPL_AVX512_SHIFT(shr_u16, srl_epi16)
    V_IMPL_AVX512_SHIFT(shr_s16, sra_epi16)
#endif
#define v_shr_u32(A, C) _mm512_srl_epi32(A, _mm_cvtsi32_si128(C))
#define v_shr_s32(A, C) _mm512_sra_epi32(A, _mm_cvtsi32_si128(C))
#define v_shr_u64(A, C) _mm512_srl_epi64(A, _mm_cvtsi32_si128(C))
#define v_shr_s64(A, C) _mm512_sra_epi64(A, _mm_cvtsi32_si128(C))

// right by an immediate constant
#ifdef V_HAVE_AVX512BW
    #define v_shri_u16 _mm512_srli_epi16
    #define v_shri_s16 _mm512_srai_epi16
#else
    #define v_shri_u16 v_shr_u16
    #define v_shri_s16 v_shr_s16
#endif
#define v_shri_u32 _mm512_srli_epi32
#define v_shri_s32 _mm512_srai_epi32
#define v_shri_u64 _mm512_srli_epi64
#define v_shri_s64 _mm512_srai_epi64

/***************************
 * Logical
 ***************************/

// AND
#define v_and_u8  _mm512_and_si512
#define v_and_s8  _mm512_and_si512
#define v_and_u16 _mm512_and_si512
#define v_and_s16 _mm512_and_si512
#define v_and_u32 _mm512_and_si512
#define v_and_s32 _mm512_and_si512
#define v_and_u64 _mm512_and_si512
#define v_and_s64 _mm512_and_si512
#ifdef V_HAVE_AVX512DQ
    #define v_and_f32 _mm512_and_ps
    #define v_and_f64 _mm512_and_pd
#else
    V_IMPL_AVX512_FROM_SI512_PS_2ARG(v_and_f32, _mm512_and_si512)
    V_IMPL_AVX512_FROM_SI512_PD_2ARG(v_and_f64, _mm512_and_si512)
#endif

// OR
#define v_or_u8  _mm512_or_si512
#define v_or_s8  _mm512_or_si512
#define v_or_u16 _mm512_or_si512
#define v_or_s16 _mm512_or_si512
#define v_or_u32 _mm512_or_si512
#define v_or_s32 _mm512_or_si512
#define v_or_u64 _mm512_or_si512
#define v_or_s64 _mm512_or_si512
#ifdef V_HAVE_AVX512DQ
    #define v_or_f32 _mm512_or_ps
    #define v_or_f64 _mm512_or_pd
#else
    V_IMPL_AVX512_FROM_SI512_PS_2ARG(v_or_f32, _mm512_or_si512)
    V_IMPL_AVX512_FROM_SI512_PD_2ARG(v_or_f64, _mm512_or_si512)
#endif

// XOR
#define v_xor_u8  _mm512_xor_si512
#define v_xor_s8  _mm512_xor_si512
#define v_xor_u16 _mm512_xor_si512
#define v_xor_s16 _mm512_xor_si512
#define v_xor_u32 _mm512_xor_si512
#define v_xor_s32 _mm512_xor_si512
#define v_xor_u64 _mm512_xor_si512
#define v_xor_s64 _mm512_xor_si512
#ifdef V_HAVE_AVX512DQ
    #define v_xor_f32 _mm512_xor_ps
    #define v_xor_f64 _mm512_xor_pd
#else
    V_IMPL_AVX512_FROM_SI512_PS_2ARG(v_xor_f32, _mm512_xor_si512)
    V_IMPL_AVX512_FROM_SI512_PD_2ARG(v_xor_f64, _mm512_xor_si512)
#endif

// NOT
#define v_not_u8(A) _mm512_xor_si512(A, _mm512_set1_epi32(-1))
#define v_not_s8  v_not_u8
#define v_not_u16 v_not_u8
#define v_not_s16 v_not_u8
#define v_not_u32 v_not_u8
#define v_not_s32 v_not_u8
#define v_not_u64 v_not_u8
#define v_not_s64 v_not_u8
#ifdef V_HAVE_AVX512DQ
    #define v_not_f32(A) _mm512_xor_ps(A, _mm512_castsi512_ps(_mm512_set1_epi32(-1)))
    #define v_not_f64(A) _mm512_xor_pd(A, _mm512_castsi512_pd(_mm512_set1_epi32(-1)))
#else
    #define v_not_f32(A) _mm512_castsi512_ps(v_not_u32(_mm512_castps_si512(A)))
    #define v_not_f64(A) _mm512_castsi512_pd(v_not_u64(_mm512_castpd_si512(A)))
#endif

/***************************
 * Comparison
 ***************************/

// int Equal
#ifdef V_HAVE_AVX512BW
    #define v_cmpeq_u8  _mm512_cmpeq_epu8_mask
    #define v_cmpeq_s8  _mm512_cmpeq_epi8_mask
    #define v_cmpeq_u16 _mm512_cmpeq_epu16_mask
    #define v_cmpeq_s16 _mm512_cmpeq_epi16_mask
#else
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_cmpeq_u8,  _mm256_cmpeq_epi8)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_cmpeq_u16, _mm256_cmpeq_epi16)
    #define v_cmpeq_s8  v_cmpeq_u8
    #define v_cmpeq_s16 v_cmpeq_u16
#endif
#define v_cmpeq_u32 _mm512_cmpeq_epu32_mask
#define v_cmpeq_s32 _mm512_cmpeq_epi32_mask
#define v_cmpeq_u64 _mm512_cmpeq_epu64_mask
#define v_cmpeq_s64 _mm512_cmpeq_epi64_mask

// int not equal
#ifdef V_HAVE_AVX512BW
    #define v_cmpneq_u8  _mm512_cmpneq_epu8_mask
    #define v_cmpneq_s8  _mm512_cmpneq_epi8_mask
    #define v_cmpneq_u16 _mm512_cmpneq_epu16_mask
    #define v_cmpneq_s16 _mm512_cmpneq_epi16_mask
#else
    #define v_cmpneq_u8(A, B) v_not_u8(v_cmpeq_u8(A, B))
    #define v_cmpneq_u16(A, B) v_not_u16(v_cmpeq_u16(A, B))
    #define v_cmpneq_s8  v_cmpneq_u8
    #define v_cmpneq_s16 v_cmpneq_u16
#endif
#define v_cmpneq_u32 _mm512_cmpneq_epu32_mask
#define v_cmpneq_s32 _mm512_cmpneq_epi32_mask
#define v_cmpneq_u64 _mm512_cmpneq_epu64_mask
#define v_cmpneq_s64 _mm512_cmpneq_epi64_mask

// greater than
#ifdef V_HAVE_AVX512BW
    #define v_cmpgt_u8  _mm512_cmpgt_epu8_mask
    #define v_cmpgt_s8  _mm512_cmpgt_epi8_mask
    #define v_cmpgt_u16 _mm512_cmpgt_epu16_mask
    #define v_cmpgt_s16 _mm512_cmpgt_epi16_mask
#else
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_cmpgt_s8,  _mm256_cmpgt_epi8)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v_cmpgt_s16, _mm256_cmpgt_epi16)
    V_FINLINE __m512i v_cmpgt_u8(__m512i a, __m512i b)
    {
        const __m512i sbit = _mm512_set1_epi32(0x80808080);
        return v_cmpgt_s8(_mm512_xor_si512(a, sbit), _mm512_xor_si512(b, sbit));
    }
    V_FINLINE __m512i v_cmpgt_u16(__m512i a, __m512i b)
    {
        const __m512i sbit = _mm512_set1_epi32(0x80008000);
        return v_cmpgt_s16(_mm512_xor_si512(a, sbit), _mm512_xor_si512(b, sbit));
    }
#endif
#define v_cmpgt_u32 _mm512_cmpgt_epu32_mask
#define v_cmpgt_s32 _mm512_cmpgt_epi32_mask
#define v_cmpgt_u64 _mm512_cmpgt_epu64_mask
#define v_cmpgt_s64 _mm512_cmpgt_epi64_mask

// greater than or equal
#ifdef V_HAVE_AVX512BW
    #define v_cmpge_u8  _mm512_cmpge_epu8_mask
    #define v_cmpge_s8  _mm512_cmpge_epi8_mask
    #define v_cmpge_u16 _mm512_cmpge_epu16_mask
    #define v_cmpge_s16 _mm512_cmpge_epi16_mask
#else
    #define v_cmpge_u8(A, B)  v_not_u8(v_cmpgt_u8(B, A))
    #define v_cmpge_s8(A, B)  v_not_s8(v_cmpgt_s8(B, A))
    #define v_cmpge_u16(A, B) v_not_u16(v_cmpgt_u16(B, A))
    #define v_cmpge_s16(A, B) v_not_s16(v_cmpgt_s16(B, A))
#endif
#define v_cmpge_u32 _mm512_cmpge_epu32_mask
#define v_cmpge_s32 _mm512_cmpge_epi32_mask
#define v_cmpge_u64 _mm512_cmpge_epu64_mask
#define v_cmpge_s64 _mm512_cmpge_epi64_mask

// less than
#define v_cmplt_u8(A, B)  v_cmpgt_u8(B, A)
#define v_cmplt_s8(A, B)  v_cmpgt_s8(B, A)
#define v_cmplt_u16(A, B) v_cmpgt_u16(B, A)
#define v_cmplt_s16(A, B) v_cmpgt_s16(B, A)
#define v_cmplt_u32(A, B) v_cmpgt_u32(B, A)
#define v_cmplt_s32(A, B) v_cmpgt_s32(B, A)
#define v_cmplt_u64(A, B) v_cmpgt_u64(B, A)
#define v_cmplt_s64(A, B) v_cmpgt_s64(B, A)

// less than or equal
#define v_cmple_u8(A, B)  v_cmpge_u8(B, A)
#define v_cmple_s8(A, B)  v_cmpge_s8(B, A)
#define v_cmple_u16(A, B) v_cmpge_u16(B, A)
#define v_cmple_s16(A, B) v_cmpge_s16(B, A)
#define v_cmple_u32(A, B) v_cmpge_u32(B, A)
#define v_cmple_s32(A, B) v_cmpge_s32(B, A)
#define v_cmple_u64(A, B) v_cmpge_u64(B, A)
#define v_cmple_s64(A, B) v_cmpge_s64(B, A)

// precision comparison
#define v_cmpeq_f32(A, B)  _mm512_cmp_ps_mask(A, B, _CMP_EQ_OQ)
#define v_cmpeq_f64(A, B)  _mm512_cmp_pd_mask(A, B, _CMP_EQ_OQ)
#define v_cmpneq_f32(A, B) _mm512_cmp_ps_mask(A, B, _CMP_NEQ_OQ)
#define v_cmpneq_f64(A, B) _mm512_cmp_pd_mask(A, B, _CMP_NEQ_OQ)
#define v_cmplt_f32(A, B)  _mm512_cmp_ps_mask(A, B, _CMP_LT_OQ)
#define v_cmplt_f64(A, B)  _mm512_cmp_pd_mask(A, B, _CMP_LT_OQ)
#define v_cmple_f32(A, B)  _mm512_cmp_ps_mask(A, B, _CMP_LE_OQ)
#define v_cmple_f64(A, B)  _mm512_cmp_pd_mask(A, B, _CMP_LE_OQ)
#define v_cmpgt_f32(A, B)  _mm512_cmp_ps_mask(A, B, _CMP_GT_OQ)
#define v_cmpgt_f64(A, B)  _mm512_cmp_pd_mask(A, B, _CMP_GT_OQ)
#define v_cmpge_f32(A, B)  _mm512_cmp_ps_mask(A, B, _CMP_GE_OQ)
#define v_cmpge_f64(A, B)  _mm512_cmp_pd_mask(A, B, _CMP_GE_OQ)

#endif // _V_SIMD_AVX512_OPERATORS_H
