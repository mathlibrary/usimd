#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX512_REORDER_H
#define _V_SIMD_AVX512_REORDER_H

// combine lower part of two vectors
#define v_combinel_u8(A, B) _mm512_inserti64x4(A, _mm512_castsi512_si256(B), 1)
#define v_combinel_s8  v_combinel_u8
#define v_combinel_u16 v_combinel_u8
#define v_combinel_s16 v_combinel_u8
#define v_combinel_u32 v_combinel_u8
#define v_combinel_s32 v_combinel_u8
#define v_combinel_u64 v_combinel_u8
#define v_combinel_s64 v_combinel_u8
#define v_combinel_f64(A, B) _mm512_insertf64x4(A, _mm512_castpd512_pd256(B), 1)
#ifdef V_HAVE_AVX512DQ
    #define v_combinel_f32(A, B) \
        _mm512_insertf32x8(A, _mm512_castps512_ps256(B), 1)
#else
    #define v_combinel_f32(A, B) \
        _mm512_castsi512_ps(v_combinel_u8(_mm512_castps_si512(A), _mm512_castps_si512(B)))
#endif

// combine higher part of two vectors
#define v_combineh_u8(A, B) _mm512_inserti64x4(B, _mm512_extracti64x4_epi64(A, 1), 0)
#define v_combineh_s8  v_combineh_u8
#define v_combineh_u16 v_combineh_u8
#define v_combineh_s16 v_combineh_u8
#define v_combineh_u32 v_combineh_u8
#define v_combineh_s32 v_combineh_u8
#define v_combineh_u64 v_combineh_u8
#define v_combineh_s64 v_combineh_u8
#define v_combineh_f64(A, B) _mm512_insertf64x4(B, _mm512_extractf64x4_pd(A, 1), 0)
#ifdef V_HAVE_AVX512DQ
    #define v_combineh_f32(A, B) \
        _mm512_insertf32x8(B, _mm512_extractf32x8_ps(A, 1), 0)
#else
    #define v_combineh_f32(A, B) \
        _mm512_castsi512_ps(v_combineh_u8(_mm512_castps_si512(A), _mm512_castps_si512(B)))
#endif

// combine two vectors from lower and higher parts of two other vectors
V_FINLINE v_m512ix2 v__combine(__m512i a, __m512i b)
{
    v_m512ix2 r;
    r.val[0] = v_combinel_u8(a, b);
    r.val[1] = v_combineh_u8(a, b);
    return r;
}
V_FINLINE v_f32x2 v_combine_f32(__m512 a, __m512 b)
{
    v_f32x2 r;
    r.val[0] = v_combinel_f32(a, b);
    r.val[1] = v_combineh_f32(a, b);
    return r;
}
V_FINLINE v_f64x2 v_combine_f64(__m512d a, __m512d b)
{
    v_f64x2 r;
    r.val[0] = v_combinel_f64(a, b);
    r.val[1] = v_combineh_f64(a, b);
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
#ifndef V_HAVE_AVX512BW
    V_IMPL_AVX512_FROM_AVX2_2ARG(v__unpacklo_epi8,  _mm256_unpacklo_epi8)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v__unpackhi_epi8,  _mm256_unpackhi_epi8)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v__unpacklo_epi16, _mm256_unpacklo_epi16)
    V_IMPL_AVX512_FROM_AVX2_2ARG(v__unpackhi_epi16, _mm256_unpackhi_epi16)
#endif

V_FINLINE v_u64x2 v_zip_u64(__m512i a, __m512i b)
{
    v_u64x2 r;
    r.val[0] = _mm512_permutex2var_epi64(a, v_set_u64(0, 8, 1, 9, 2, 10, 3, 11), b);
    r.val[1] = _mm512_permutex2var_epi64(a, v_set_u64(4, 12, 5, 13, 6, 14, 7, 15), b);
    return r;
}
#define v_zip_s64 v_zip_u64

V_FINLINE v_u8x2 v_zip_u8(__m512i a, __m512i b)
{
    v_u8x2 r;
#ifdef V_HAVE_AVX512VBMI
    r.val[0] = _mm512_permutex2var_epi8(a,
        v_set_u8(0,  64, 1,  65, 2,  66, 3,  67, 4,  68, 5,  69, 6,  70, 7,  71,
                    8,  72, 9,  73, 10, 74, 11, 75, 12, 76, 13, 77, 14, 78, 15, 79,
                    16, 80, 17, 81, 18, 82, 19, 83, 20, 84, 21, 85, 22, 86, 23, 87,
                    24, 88, 25, 89, 26, 90, 27, 91, 28, 92, 29, 93, 30, 94, 31, 95), b);
    r.val[1] = _mm512_permutex2var_epi8(a,
        v_set_u8(32, 96,  33, 97,  34, 98,  35, 99,  36, 100, 37, 101, 38, 102, 39, 103,
                    40, 104, 41, 105, 42, 106, 43, 107, 44, 108, 45, 109, 46, 110, 47, 111,
                    48, 112, 49, 113, 50, 114, 51, 115, 52, 116, 53, 117, 54, 118, 55, 119,
                    56, 120, 57, 121, 58, 122, 59, 123, 60, 124, 61, 125, 62, 126, 63, 127), b);
#else
    #ifdef V_HAVE_AVX512BW
    __m512i ab0 = _mm512_unpacklo_epi8(a, b);
    __m512i ab1 = _mm512_unpackhi_epi8(a, b);
    #else
    __m512i ab0 = v__unpacklo_epi8(a, b);
    __m512i ab1 = v__unpackhi_epi8(a, b);
    #endif
    r.val[0] = _mm512_permutex2var_epi64(ab0, v_set_u64(0, 1, 8, 9, 2, 3, 10, 11), ab1);
    r.val[1] = _mm512_permutex2var_epi64(ab0, v_set_u64(4, 5, 12, 13, 6, 7, 14, 15), ab1);
#endif
    return r;
}
#define v_zip_s8 v_zip_u8

V_FINLINE v_u16x2 v_zip_u16(__m512i a, __m512i b)
{
    v_u16x2 r;
#ifdef V_HAVE_AVX512BW
    r.val[0] = _mm512_permutex2var_epi16(a,
        v_set_u16(0, 32, 1, 33, 2, 34, 3, 35, 4, 36, 5, 37, 6, 38, 7, 39,
                     8, 40, 9, 41, 10, 42, 11, 43, 12, 44, 13, 45, 14, 46, 15, 47), b);
    r.val[1] = _mm512_permutex2var_epi16(a,
        v_set_u16(16, 48, 17, 49, 18, 50, 19, 51, 20, 52, 21, 53, 22, 54, 23, 55,
                     24, 56, 25, 57, 26, 58, 27, 59, 28, 60, 29, 61, 30, 62, 31, 63), b);
#else
    __m512i ab0 = v__unpacklo_epi16(a, b);
    __m512i ab1 = v__unpackhi_epi16(a, b);
    r.val[0] = _mm512_permutex2var_epi64(ab0, v_set_u64(0, 1, 8, 9, 2, 3, 10, 11), ab1);
    r.val[1] = _mm512_permutex2var_epi64(ab0, v_set_u64(4, 5, 12, 13, 6, 7, 14, 15), ab1);
#endif
    return r;
}
#define v_zip_s16 v_zip_u16

V_FINLINE v_u32x2 v_zip_u32(__m512i a, __m512i b)
{
    v_u32x2 r;
    r.val[0] = _mm512_permutex2var_epi32(a,
        v_set_u32(0, 16, 1, 17, 2, 18, 3, 19, 4, 20, 5, 21, 6, 22, 7, 23), b);
    r.val[1] = _mm512_permutex2var_epi32(a,
        v_set_u32(8, 24, 9, 25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31), b);
    return r;
}
#define v_zip_s32 v_zip_u32

V_FINLINE v_f32x2 v_zip_f32(__m512 a, __m512 b)
{
    v_f32x2 r;
    r.val[0] = _mm512_permutex2var_ps(a,
        v_set_u32(0, 16, 1, 17, 2, 18, 3, 19, 4, 20, 5, 21, 6, 22, 7, 23), b);
    r.val[1] = _mm512_permutex2var_ps(a,
        v_set_u32(8, 24, 9, 25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31), b);
    return r;
}

V_FINLINE v_f64x2 v_zip_f64(__m512d a, __m512d b)
{
    v_f64x2 r;
    r.val[0] = _mm512_permutex2var_pd(a, v_set_u64(0, 8, 1, 9, 2, 10, 3, 11), b);
    r.val[1] = _mm512_permutex2var_pd(a, v_set_u64(4, 12, 5, 13, 6, 14, 7, 15), b);
    return r;
}

#endif // _V_SIMD_AVX512_REORDER_H
