#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_VSX_CVT_H
#define _V_SIMD_VSX_CVT_H

// convert boolean vectors to integer vectors
#define v_cvt_u8_b8(BL)   ((v_u8)  BL)
#define v_cvt_s8_b8(BL)   ((v_s8)  BL)
#define v_cvt_u16_b16(BL) ((v_u16) BL)
#define v_cvt_s16_b16(BL) ((v_s16) BL)
#define v_cvt_u32_b32(BL) ((v_u32) BL)
#define v_cvt_s32_b32(BL) ((v_s32) BL)
#define v_cvt_u64_b64(BL) ((v_u64) BL)
#define v_cvt_s64_b64(BL) ((v_s64) BL)
#define v_cvt_f32_b32(BL) ((v_f32) BL)
#define v_cvt_f64_b64(BL) ((v_f64) BL)

// convert integer vectors to boolean vectors
#define v_cvt_b8_u8(A)   ((v_b8)  A)
#define v_cvt_b8_s8(A)   ((v_b8)  A)
#define v_cvt_b16_u16(A) ((v_b16) A)
#define v_cvt_b16_s16(A) ((v_b16) A)
#define v_cvt_b32_u32(A) ((v_b32) A)
#define v_cvt_b32_s32(A) ((v_b32) A)
#define v_cvt_b64_u64(A) ((v_b64) A)
#define v_cvt_b64_s64(A) ((v_b64) A)
#define v_cvt_b32_f32(A) ((v_b32) A)
#define v_cvt_b64_f64(A) ((v_b64) A)

#endif // _V_SIMD_VSX_CVT_H
