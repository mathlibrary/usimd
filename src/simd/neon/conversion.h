#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_NEON_CVT_H
#define _V_SIMD_NEON_CVT_H

// convert boolean vectors to integer vectors
#define v_cvt_u8_b8(A)   A
#define v_cvt_s8_b8(A)   vreinterpretq_s8_u8(A)
#define v_cvt_u16_b16(A) A
#define v_cvt_s16_b16(A) vreinterpretq_s16_u16(A)
#define v_cvt_u32_b32(A) A
#define v_cvt_s32_b32(A) vreinterpretq_s32_u32(A)
#define v_cvt_u64_b64(A) A
#define v_cvt_s64_b64(A) vreinterpretq_s64_u64(A)
#define v_cvt_f32_b32(A) vreinterpretq_f32_u32(A)
#define v_cvt_f64_b64(A) vreinterpretq_f64_u64(A)

// convert integer vectors to boolean vectors
#define v_cvt_b8_u8(BL)   BL
#define v_cvt_b8_s8(BL)   vreinterpretq_u8_s8(BL)
#define v_cvt_b16_u16(BL) BL
#define v_cvt_b16_s16(BL) vreinterpretq_u16_s16(BL)
#define v_cvt_b32_u32(BL) BL
#define v_cvt_b32_s32(BL) vreinterpretq_u32_s32(BL)
#define v_cvt_b64_u64(BL) BL
#define v_cvt_b64_s64(BL) vreinterpretq_u64_s64(BL)
#define v_cvt_b32_f32(BL) vreinterpretq_u32_f32(BL)
#define v_cvt_b64_f64(BL) vreinterpretq_u64_f64(BL)

// expand
V_FINLINE void v_expand_u8_u16(v_u8 data, v_u16 *low, v_u16 *high) {
    *low = vmovl_u8(vget_low_u8(data));
    *high = vmovl_u8(vget_high_u8(data));
}

V_FINLINE void v_expand_u16_u32(v_u16 data, v_u32 *low, v_u32 *high) {
    *low = vmovl_u16(vget_low_u16(data));
    *high = vmovl_u16(vget_high_u16(data));
}

#endif // _V_SIMD_NEON_CVT_H
