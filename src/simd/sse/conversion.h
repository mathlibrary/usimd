#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_SSE_CVT_H
#define _V_SIMD_SSE_CVT_H

// convert mask types to integer types
#define v_cvt_u8_b8(BL)   BL
#define v_cvt_s8_b8(BL)   BL
#define v_cvt_u16_b16(BL) BL
#define v_cvt_s16_b16(BL) BL
#define v_cvt_u32_b32(BL) BL
#define v_cvt_s32_b32(BL) BL
#define v_cvt_u64_b64(BL) BL
#define v_cvt_s64_b64(BL) BL
#define v_cvt_f32_b32(BL) _mm_castsi128_ps(BL)
#define v_cvt_f64_b64(BL) _mm_castsi128_pd(BL)

// convert integer types to mask types
#define v_cvt_b8_u8(A)   A
#define v_cvt_b8_s8(A)   A
#define v_cvt_b16_u16(A) A
#define v_cvt_b16_s16(A) A
#define v_cvt_b32_u32(A) A
#define v_cvt_b32_s32(A) A
#define v_cvt_b64_u64(A) A
#define v_cvt_b64_s64(A) A
#define v_cvt_b32_f32(A) _mm_castps_si128(A)
#define v_cvt_b64_f64(A) _mm_castpd_si128(A)

// expand
V_FINLINE v_u16x2 v_expand_u8_u16(v_u8 data) {
    v_u16x2 r;
    const __m128i z = _mm_setzero_si128();
    r.val[0] = _mm_unpacklo_epi8(data, z);
    r.val[1] = _mm_unpackhi_epi8(data, z);
    return r;
}

V_FINLINE v_u32x2 v_expand_u16_u32(v_u16 data) {
    v_u32x2 r;
    const __m128i z = _mm_setzero_si128();
    r.val[0]  = _mm_unpacklo_epi16(data, z);
    r.val[1]  = _mm_unpackhi_epi16(data, z);
    return r;
}

#endif // _V_SIMD_SSE_CVT_H
