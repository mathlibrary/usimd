#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX2_CVT_H
#define _V_SIMD_AVX2_CVT_H

// convert mask types to integer types
#define v_cvt_u8_b8(A)   A
#define v_cvt_s8_b8(A)   A
#define v_cvt_u16_b16(A) A
#define v_cvt_s16_b16(A) A
#define v_cvt_u32_b32(A) A
#define v_cvt_s32_b32(A) A
#define v_cvt_u64_b64(A) A
#define v_cvt_s64_b64(A) A
#define v_cvt_f32_b32(A) _mm256_castsi256_ps(A)
#define v_cvt_f64_b64(A) _mm256_castsi256_pd(A)

// convert integer types to mask types
#define v_cvt_b8_u8(BL)   BL
#define v_cvt_b8_s8(BL)   BL
#define v_cvt_b16_u16(BL) BL
#define v_cvt_b16_s16(BL) BL
#define v_cvt_b32_u32(BL) BL
#define v_cvt_b32_s32(BL) BL
#define v_cvt_b64_u64(BL) BL
#define v_cvt_b64_s64(BL) BL
#define v_cvt_b32_f32(BL) _mm256_castps_si256(BL)
#define v_cvt_b64_f64(BL) _mm256_castpd_si256(BL)

// expand
V_FINLINE v_u16x2 v_expand_u8_u16(v_u8 data) {
    v_u16x2 r;
    r.val[0] = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(data));
    r.val[1] = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(data, 1));
    return r;
}

V_FINLINE v_u32x2 v_expand_u16_u32(v_u16 data) {
    v_u32x2 r;
    r.val[0] = _mm256_cvtepu16_epi32(_mm256_castsi256_si128(data));
    r.val[1] = _mm256_cvtepu16_epi32(_mm256_extracti128_si256(data, 1));
    return r;
}

#endif // _V_SIMD_AVX2_CVT_H
