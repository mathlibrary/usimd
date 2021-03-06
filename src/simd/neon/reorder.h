#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_NEON_REORDER_H
#define _V_SIMD_NEON_REORDER_H

// combine lower part of two vectors
#ifdef __aarch64__
    #define v_combinel_u8(A, B)  vreinterpretq_u8_u64(vzip1q_u64(vreinterpretq_u64_u8(A), vreinterpretq_u64_u8(B)))
    #define v_combinel_s8(A, B)  vreinterpretq_s8_u64(vzip1q_u64(vreinterpretq_u64_s8(A), vreinterpretq_u64_s8(B)))
    #define v_combinel_u16(A, B) vreinterpretq_u16_u64(vzip1q_u64(vreinterpretq_u64_u16(A), vreinterpretq_u64_u16(B)))
    #define v_combinel_s16(A, B) vreinterpretq_s16_u64(vzip1q_u64(vreinterpretq_u64_s16(A), vreinterpretq_u64_s16(B)))
    #define v_combinel_u32(A, B) vreinterpretq_u32_u64(vzip1q_u64(vreinterpretq_u64_u32(A), vreinterpretq_u64_u32(B)))
    #define v_combinel_s32(A, B) vreinterpretq_s32_u64(vzip1q_u64(vreinterpretq_u64_s32(A), vreinterpretq_u64_s32(B)))
    #define v_combinel_u64       vzip1q_u64
    #define v_combinel_s64       vzip1q_s64
    #define v_combinel_f32(A, B) vreinterpretq_f32_u64(vzip1q_u64(vreinterpretq_u64_f32(A), vreinterpretq_u64_f32(B)))
    #define v_combinel_f64       vzip1q_f64
#else
    #define v_combinel_u8(A, B)  vcombine_u8(vget_low_u8(A), vget_low_u8(B))
    #define v_combinel_s8(A, B)  vcombine_s8(vget_low_s8(A), vget_low_s8(B))
    #define v_combinel_u16(A, B) vcombine_u16(vget_low_u16(A), vget_low_u16(B))
    #define v_combinel_s16(A, B) vcombine_s16(vget_low_s16(A), vget_low_s16(B))
    #define v_combinel_u32(A, B) vcombine_u32(vget_low_u32(A), vget_low_u32(B))
    #define v_combinel_s32(A, B) vcombine_s32(vget_low_s32(A), vget_low_s32(B))
    #define v_combinel_u64(A, B) vcombine_u64(vget_low_u64(A), vget_low_u64(B))
    #define v_combinel_s64(A, B) vcombine_s64(vget_low_s64(A), vget_low_s64(B))
    #define v_combinel_f32(A, B) vcombine_f32(vget_low_f32(A), vget_low_f32(B))
#endif

// combine higher part of two vectors
#ifdef __aarch64__
    #define v_combineh_u8(A, B)  vreinterpretq_u8_u64(vzip2q_u64(vreinterpretq_u64_u8(A), vreinterpretq_u64_u8(B)))
    #define v_combineh_s8(A, B)  vreinterpretq_s8_u64(vzip2q_u64(vreinterpretq_u64_s8(A), vreinterpretq_u64_s8(B)))
    #define v_combineh_u16(A, B) vreinterpretq_u16_u64(vzip2q_u64(vreinterpretq_u64_u16(A), vreinterpretq_u64_u16(B)))
    #define v_combineh_s16(A, B) vreinterpretq_s16_u64(vzip2q_u64(vreinterpretq_u64_s16(A), vreinterpretq_u64_s16(B)))
    #define v_combineh_u32(A, B) vreinterpretq_u32_u64(vzip2q_u64(vreinterpretq_u64_u32(A), vreinterpretq_u64_u32(B)))
    #define v_combineh_s32(A, B) vreinterpretq_s32_u64(vzip2q_u64(vreinterpretq_u64_s32(A), vreinterpretq_u64_s32(B)))
    #define v_combineh_u64       vzip2q_u64
    #define v_combineh_s64       vzip2q_s64
    #define v_combineh_f32(A, B) vreinterpretq_f32_u64(vzip2q_u64(vreinterpretq_u64_f32(A), vreinterpretq_u64_f32(B)))
    #define v_combineh_f64       vzip2q_f64
#else
    #define v_combineh_u8(A, B)  vcombine_u8(vget_high_u8(A), vget_high_u8(B))
    #define v_combineh_s8(A, B)  vcombine_s8(vget_high_s8(A), vget_high_s8(B))
    #define v_combineh_u16(A, B) vcombine_u16(vget_high_u16(A), vget_high_u16(B))
    #define v_combineh_s16(A, B) vcombine_s16(vget_high_s16(A), vget_high_s16(B))
    #define v_combineh_u32(A, B) vcombine_u32(vget_high_u32(A), vget_high_u32(B))
    #define v_combineh_s32(A, B) vcombine_s32(vget_high_s32(A), vget_high_s32(B))
    #define v_combineh_u64(A, B) vcombine_u64(vget_high_u64(A), vget_high_u64(B))
    #define v_combineh_s64(A, B) vcombine_s64(vget_high_s64(A), vget_high_s64(B))
    #define v_combineh_f32(A, B) vcombine_f32(vget_high_f32(A), vget_high_f32(B))
#endif

// combine two vectors from lower and higher parts of two other vectors
#define V_IMPL_NEON_COMBINE(T_VEC, SFX)                     \
    V_FINLINE T_VEC##x2 v_combine_##SFX(T_VEC a, T_VEC b) \
    {                                                          \
        T_VEC##x2 r;                                           \
        r.val[0] = V_CAT(v_combinel_, SFX)(a, b);         \
        r.val[1] = V_CAT(v_combineh_, SFX)(a, b);         \
        return r;                                              \
    }

V_IMPL_NEON_COMBINE(v_u8,  u8)
V_IMPL_NEON_COMBINE(v_s8,  s8)
V_IMPL_NEON_COMBINE(v_u16, u16)
V_IMPL_NEON_COMBINE(v_s16, s16)
V_IMPL_NEON_COMBINE(v_u32, u32)
V_IMPL_NEON_COMBINE(v_s32, s32)
V_IMPL_NEON_COMBINE(v_u64, u64)
V_IMPL_NEON_COMBINE(v_s64, s64)
V_IMPL_NEON_COMBINE(v_f32, f32)
#ifdef __aarch64__
V_IMPL_NEON_COMBINE(v_f64, f64)
#endif

// interleave two vectors
#define V_IMPL_NEON_ZIP(T_VEC, SFX)                       \
    V_FINLINE T_VEC##x2 v_zip_##SFX(T_VEC a, T_VEC b)   \
    {                                                        \
        T_VEC##x2 r;                                         \
        r.val[0] = vzip1q_##SFX(a, b);                       \
        r.val[1] = vzip2q_##SFX(a, b);                       \
        return r;                                            \
    }

#ifdef __aarch64__
    V_IMPL_NEON_ZIP(v_u8,  u8)
    V_IMPL_NEON_ZIP(v_s8,  s8)
    V_IMPL_NEON_ZIP(v_u16, u16)
    V_IMPL_NEON_ZIP(v_s16, s16)
    V_IMPL_NEON_ZIP(v_u32, u32)
    V_IMPL_NEON_ZIP(v_s32, s32)
    V_IMPL_NEON_ZIP(v_f32, f32)
    V_IMPL_NEON_ZIP(v_f64, f64)
#else
    #define v_zip_u8  vzipq_u8
    #define v_zip_s8  vzipq_s8
    #define v_zip_u16 vzipq_u16
    #define v_zip_s16 vzipq_s16
    #define v_zip_u32 vzipq_u32
    #define v_zip_s32 vzipq_s32
    #define v_zip_f32 vzipq_f32
#endif
#define v_zip_u64 v_combine_u64
#define v_zip_s64 v_combine_s64

#endif // _V_SIMD_NEON_REORDER_H
