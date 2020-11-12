#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX2_REORDER_H
#define _V_SIMD_AVX2_REORDER_H

// combine lower part of two vectors
#define v_combinel_u8(A, B) _mm256_permute2x128_si256(A, B, 0x20)
#define v_combinel_s8  v_combinel_u8
#define v_combinel_u16 v_combinel_u8
#define v_combinel_s16 v_combinel_u8
#define v_combinel_u32 v_combinel_u8
#define v_combinel_s32 v_combinel_u8
#define v_combinel_u64 v_combinel_u8
#define v_combinel_s64 v_combinel_u8
#define v_combinel_f32(A, B) _mm256_permute2f128_ps(A, B, 0x20)
#define v_combinel_f64(A, B) _mm256_permute2f128_pd(A, B, 0x20)

// combine higher part of two vectors
#define v_combineh_u8(A, B) _mm256_permute2x128_si256(A, B, 0x31)
#define v_combineh_s8  v_combineh_u8
#define v_combineh_u16 v_combineh_u8
#define v_combineh_s16 v_combineh_u8
#define v_combineh_u32 v_combineh_u8
#define v_combineh_s32 v_combineh_u8
#define v_combineh_u64 v_combineh_u8
#define v_combineh_s64 v_combineh_u8
#define v_combineh_f32(A, B) _mm256_permute2f128_ps(A, B, 0x31)
#define v_combineh_f64(A, B) _mm256_permute2f128_pd(A, B, 0x31)

// combine two vectors from lower and higher parts of two other vectors
V_FINLINE v_m256ix2 v__combine(__m256i a, __m256i b)
{
    v_m256ix2 r;
    __m256i a1b0 = _mm256_permute2x128_si256(a, b, 0x21);
    r.val[0] = _mm256_blend_epi32(a, a1b0, 0xF0);
    r.val[1] = _mm256_blend_epi32(b, a1b0, 0xF);
    return r;
}
V_FINLINE v_f32x2 v_combine_f32(__m256 a, __m256 b)
{
    v_f32x2 r;
    __m256 a1b0 = _mm256_permute2f128_ps(a, b, 0x21);
    r.val[0] = _mm256_blend_ps(a, a1b0, 0xF0);
    r.val[1] = _mm256_blend_ps(b, a1b0, 0xF);
    return r;
}
V_FINLINE v_f64x2 v_combine_f64(__m256d a, __m256d b)
{
    v_f64x2 r;
    __m256d a1b0 = _mm256_permute2f128_pd(a, b, 0x21);
    r.val[0] = _mm256_blend_pd(a, a1b0, 0xC);
    r.val[1] = _mm256_blend_pd(b, a1b0, 0x3);
    return r;
}
#define v_combine_u8  v__combine
#define v_combine_s8  v__combine
#define v_combine_u16 v__combine
#define v_combine_s16 v__combine
#define v_combine_u32 v__combine
#define v_combine_s32 v__combine
#define v_combine_u64 v__combine
#define v_combine_s64 v__combine

// interleave two vectors
#define NPYV_IMPL_AVX2_ZIP_U(T_VEC, LEN)                    \
    V_FINLINE T_VEC##x2 v_zip_u##LEN(T_VEC a, T_VEC b) \
    {                                                       \
        __m256i ab0 = _mm256_unpacklo_epi##LEN(a, b);       \
        __m256i ab1 = _mm256_unpackhi_epi##LEN(a, b);       \
        return v__combine(ab0, ab1);                     \
    }

NPYV_IMPL_AVX2_ZIP_U(v_u8,  8)
NPYV_IMPL_AVX2_ZIP_U(v_u16, 16)
NPYV_IMPL_AVX2_ZIP_U(v_u32, 32)
NPYV_IMPL_AVX2_ZIP_U(v_u64, 64)
#define v_zip_s8  v_zip_u8
#define v_zip_s16 v_zip_u16
#define v_zip_s32 v_zip_u32
#define v_zip_s64 v_zip_u64

V_FINLINE v_f32x2 v_zip_f32(__m256 a, __m256 b)
{
    __m256 ab0 = _mm256_unpacklo_ps(a, b);
    __m256 ab1 = _mm256_unpackhi_ps(a, b);
    return v_combine_f32(ab0, ab1);
}
V_FINLINE v_f64x2 v_zip_f64(__m256d a, __m256d b)
{
    __m256d ab0 = _mm256_unpacklo_pd(a, b);
    __m256d ab1 = _mm256_unpackhi_pd(a, b);
    return v_combine_f64(ab0, ab1);
}

#endif // _V_SIMD_AVX2_REORDER_H
