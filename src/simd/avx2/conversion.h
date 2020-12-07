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
V_FINLINE void v_expand_u8_u16(v_u8 data, v_u16 *low, v_u16 *high) {
    const __m256i z = _mm256_setzero_si256();
    *low = _mm256_unpacklo_epi8(data, z);
    *high = _mm256_unpackhi_epi8(data, z);
}

V_FINLINE void v_expand_u16_u32(v_u16 data, v_u32 *low, v_u32 *high) {
    const __m256i z = _mm256_setzero_si256();
    *low = _mm256_unpacklo_epi16(data, z);
    *high = _mm256_unpackhi_epi16(data, z);
}

#endif // _V_SIMD_AVX2_CVT_H
