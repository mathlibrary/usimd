#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX2_OPERATORS_H
#define _V_SIMD_AVX2_OPERATORS_H

/***************************
 * Shifting
 ***************************/

// left
#define v_shl_u16(A, C) _mm256_sll_epi16(A, _mm_cvtsi32_si128(C))
#define v_shl_s16(A, C) _mm256_sll_epi16(A, _mm_cvtsi32_si128(C))
#define v_shl_u32(A, C) _mm256_sll_epi32(A, _mm_cvtsi32_si128(C))
#define v_shl_s32(A, C) _mm256_sll_epi32(A, _mm_cvtsi32_si128(C))
#define v_shl_u64(A, C) _mm256_sll_epi64(A, _mm_cvtsi32_si128(C))
#define v_shl_s64(A, C) _mm256_sll_epi64(A, _mm_cvtsi32_si128(C))

// left by an immediate constant
#define v_shli_u16 _mm256_slli_epi16
#define v_shli_s16 _mm256_slli_epi16
#define v_shli_u32 _mm256_slli_epi32
#define v_shli_s32 _mm256_slli_epi32
#define v_shli_u64 _mm256_slli_epi64
#define v_shli_s64 _mm256_slli_epi64

// right
#define v_shr_u16(A, C) _mm256_srl_epi16(A, _mm_cvtsi32_si128(C))
#define v_shr_s16(A, C) _mm256_sra_epi16(A, _mm_cvtsi32_si128(C))
#define v_shr_u32(A, C) _mm256_srl_epi32(A, _mm_cvtsi32_si128(C))
#define v_shr_s32(A, C) _mm256_sra_epi32(A, _mm_cvtsi32_si128(C))
#define v_shr_u64(A, C) _mm256_srl_epi64(A, _mm_cvtsi32_si128(C))
V_FINLINE __m256i v_shr_s64(__m256i a, int c)
{
    const __m256i sbit = _mm256_set1_epi64x(0x8000000000000000);
    const __m128i c64  = _mm_cvtsi32_si128(c);
    __m256i r = _mm256_srl_epi64(_mm256_add_epi64(a, sbit), c64);
    return _mm256_sub_epi64(r, _mm256_srl_epi64(sbit, c64));
}

// right by an immediate constant
#define v_shri_u16 _mm256_srli_epi16
#define v_shri_s16 _mm256_srai_epi16
#define v_shri_u32 _mm256_srli_epi32
#define v_shri_s32 _mm256_srai_epi32
#define v_shri_u64 _mm256_srli_epi64
#define v_shri_s64  v_shr_s64

/***************************
 * Logical
 ***************************/
// AND
#define v_and_u8  _mm256_and_si256
#define v_and_s8  _mm256_and_si256
#define v_and_u16 _mm256_and_si256
#define v_and_s16 _mm256_and_si256
#define v_and_u32 _mm256_and_si256
#define v_and_s32 _mm256_and_si256
#define v_and_u64 _mm256_and_si256
#define v_and_s64 _mm256_and_si256
#define v_and_f32 _mm256_and_ps
#define v_and_f64 _mm256_and_pd

#define v_andnot_f32 _mm256_andnot_ps
#define v_andnot_f64 _mm256_andnot_pd
// OR
#define v_or_u8  _mm256_or_si256
#define v_or_s8  _mm256_or_si256
#define v_or_u16 _mm256_or_si256
#define v_or_s16 _mm256_or_si256
#define v_or_u32 _mm256_or_si256
#define v_or_s32 _mm256_or_si256
#define v_or_u64 _mm256_or_si256
#define v_or_s64 _mm256_or_si256
#define v_or_f32 _mm256_or_ps
#define v_or_f64 _mm256_or_pd

// XOR
#define v_xor_u8  _mm256_xor_si256
#define v_xor_s8  _mm256_xor_si256
#define v_xor_u16 _mm256_xor_si256
#define v_xor_s16 _mm256_xor_si256
#define v_xor_u32 _mm256_xor_si256
#define v_xor_s32 _mm256_xor_si256
#define v_xor_u64 _mm256_xor_si256
#define v_xor_s64 _mm256_xor_si256
#define v_xor_f32 _mm256_xor_ps
#define v_xor_f64 _mm256_xor_pd

// NOT
#define v_not_u8(A) _mm256_xor_si256(A, _mm256_set1_epi32(-1))
#define v_not_s8  v_not_u8
#define v_not_u16 v_not_u8
#define v_not_s16 v_not_u8
#define v_not_u32 v_not_u8
#define v_not_s32 v_not_u8
#define v_not_u64 v_not_u8
#define v_not_s64 v_not_u8
#define v_not_f32(A) _mm256_xor_ps(A, _mm256_castsi256_ps(_mm256_set1_epi32(-1)))
#define v_not_f64(A) _mm256_xor_pd(A, _mm256_castsi256_pd(_mm256_set1_epi32(-1)))

/***************************
 * Comparison
 ***************************/

// int Equal
#define v_cmpeq_u8  _mm256_cmpeq_epi8
#define v_cmpeq_s8  _mm256_cmpeq_epi8
#define v_cmpeq_u16 _mm256_cmpeq_epi16
#define v_cmpeq_s16 _mm256_cmpeq_epi16
#define v_cmpeq_u32 _mm256_cmpeq_epi32
#define v_cmpeq_s32 _mm256_cmpeq_epi32
#define v_cmpeq_u64 _mm256_cmpeq_epi64
#define v_cmpeq_s64 _mm256_cmpeq_epi64

// int Not Equal
#define v_cmpneq_u8(A, B) v_not_u8(_mm256_cmpeq_epi8(A, B))
#define v_cmpneq_s8 v_cmpneq_u8
#define v_cmpneq_u16(A, B) v_not_u16(_mm256_cmpeq_epi16(A, B))
#define v_cmpneq_s16 v_cmpneq_u16
#define v_cmpneq_u32(A, B) v_not_u32(_mm256_cmpeq_epi32(A, B))
#define v_cmpneq_s32 v_cmpneq_u32
#define v_cmpneq_u64(A, B) v_not_u64(_mm256_cmpeq_epi64(A, B))
#define v_cmpneq_s64 v_cmpneq_u64

// signed greater than
#define v_cmpgt_s8  _mm256_cmpgt_epi8
#define v_cmpgt_s16 _mm256_cmpgt_epi16
#define v_cmpgt_s32 _mm256_cmpgt_epi32
#define v_cmpgt_s64 _mm256_cmpgt_epi64

// signed greater than or equal
#define v_cmpge_s8(A, B)  v_not_s8(_mm256_cmpgt_epi8(B, A))
#define v_cmpge_s16(A, B) v_not_s16(_mm256_cmpgt_epi16(B, A))
#define v_cmpge_s32(A, B) v_not_s32(_mm256_cmpgt_epi32(B, A))
#define v_cmpge_s64(A, B) v_not_s64(_mm256_cmpgt_epi64(B, A))

// unsigned greater than
#define V_IMPL_AVX2_UNSIGNED_GT(LEN, SIGN)                    \
    V_FINLINE __m256i v_cmpgt_u##LEN(__m256i a, __m256i b)  \
    {                                                            \
        const __m256i sbit = _mm256_set1_epi32(SIGN);            \
        return _mm256_cmpgt_epi##LEN(                            \
            _mm256_xor_si256(a, sbit), _mm256_xor_si256(b, sbit) \
        );                                                       \
    }

V_IMPL_AVX2_UNSIGNED_GT(8,  0x80808080)
V_IMPL_AVX2_UNSIGNED_GT(16, 0x80008000)
V_IMPL_AVX2_UNSIGNED_GT(32, 0x80000000)

V_FINLINE __m256i v_cmpgt_u64(__m256i a, __m256i b)
{
    const __m256i sbit = _mm256_set1_epi64x(0x8000000000000000);
    return _mm256_cmpgt_epi64(_mm256_xor_si256(a, sbit), _mm256_xor_si256(b, sbit));
}

// unsigned greater than or equal
V_FINLINE __m256i v_cmpge_u8(__m256i a, __m256i b)
{ return _mm256_cmpeq_epi8(a, _mm256_max_epu8(a, b)); }
V_FINLINE __m256i v_cmpge_u16(__m256i a, __m256i b)
{ return _mm256_cmpeq_epi16(a, _mm256_max_epu16(a, b)); }
V_FINLINE __m256i v_cmpge_u32(__m256i a, __m256i b)
{ return _mm256_cmpeq_epi32(a, _mm256_max_epu32(a, b)); }
#define v_cmpge_u64(A, B) v_not_u64(v_cmpgt_u64(B, A))

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
#define v_cmpeq_f32(A, B)  _mm256_castps_si256(_mm256_cmp_ps(A, B, _CMP_EQ_OQ))
#define v_cmpeq_f64(A, B)  _mm256_castpd_si256(_mm256_cmp_pd(A, B, _CMP_EQ_OQ))
#define v_cmpneq_f32(A, B) _mm256_castps_si256(_mm256_cmp_ps(A, B, _CMP_NEQ_OQ))
#define v_cmpneq_f64(A, B) _mm256_castpd_si256(_mm256_cmp_pd(A, B, _CMP_NEQ_OQ))
#define v_cmplt_f32(A, B)  _mm256_castps_si256(_mm256_cmp_ps(A, B, _CMP_LT_OQ))
#define v_cmplt_f64(A, B)  _mm256_castpd_si256(_mm256_cmp_pd(A, B, _CMP_LT_OQ))
#define v_cmple_f32(A, B)  _mm256_castps_si256(_mm256_cmp_ps(A, B, _CMP_LE_OQ))
#define v_cmple_f64(A, B)  _mm256_castpd_si256(_mm256_cmp_pd(A, B, _CMP_LE_OQ))
#define v_cmpgt_f32(A, B)  _mm256_cmp_ps(A, B, _CMP_GT_OQ)
#define v_cmpgt_f64(A, B)  _mm256_cmp_pd(A, B, _CMP_GT_OQ)
#define v_cmpge_f32(A, B)  _mm256_castps_si256(_mm256_cmp_ps(A, B, _CMP_GE_OQ)
#define v_cmpge_f64(A, B)  _mm256_castpd_si256(_mm256_cmp_pd(A, B, _CMP_GE_OQ))

#define v_max_f32 _mm256_max_ps
#define v_max_f64 _mm256_max_pd

#endif // _V_SIMD_AVX2_OPERATORS_H
