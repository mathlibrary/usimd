#ifndef NPY_SIMD
    #error "Not a standalone header"
#endif

#ifndef _NPY_SIMD_NEON_OPERATORS_H
#define _NPY_SIMD_NEON_OPERATORS_H

/***************************
 * Shifting
 ***************************/

// left
#define v_shl_u16(A, C) vshlq_u16(A, v_setall_s16(C))
#define v_shl_s16(A, C) vshlq_s16(A, v_setall_s16(C))
#define v_shl_u32(A, C) vshlq_u32(A, v_setall_s32(C))
#define v_shl_s32(A, C) vshlq_s32(A, v_setall_s32(C))
#define v_shl_u64(A, C) vshlq_u64(A, v_setall_s64(C))
#define v_shl_s64(A, C) vshlq_s64(A, v_setall_s64(C))

// left by an immediate constant
#define v_shli_u16 vshlq_n_u16
#define v_shli_s16 vshlq_n_s16
#define v_shli_u32 vshlq_n_u32
#define v_shli_s32 vshlq_n_s32
#define v_shli_u64 vshlq_n_u64
#define v_shli_s64 vshlq_n_s64

// right
#define v_shr_u16(A, C) vshlq_u16(A, v_setall_s16(-(C)))
#define v_shr_s16(A, C) vshlq_s16(A, v_setall_s16(-(C)))
#define v_shr_u32(A, C) vshlq_u32(A, v_setall_s32(-(C)))
#define v_shr_s32(A, C) vshlq_s32(A, v_setall_s32(-(C)))
#define v_shr_u64(A, C) vshlq_u64(A, v_setall_s64(-(C)))
#define v_shr_s64(A, C) vshlq_s64(A, v_setall_s64(-(C)))

// right by an immediate constant
#define v_shri_u16(VEC, C) ((C) == 0 ? VEC : vshrq_n_u16(VEC, C))
#define v_shri_s16(VEC, C) ((C) == 0 ? VEC : vshrq_n_s16(VEC, C))
#define v_shri_u32(VEC, C) ((C) == 0 ? VEC : vshrq_n_u32(VEC, C))
#define v_shri_s32(VEC, C) ((C) == 0 ? VEC : vshrq_n_s32(VEC, C))
#define v_shri_u64(VEC, C) ((C) == 0 ? VEC : vshrq_n_u64(VEC, C))
#define v_shri_s64(VEC, C) ((C) == 0 ? VEC : vshrq_n_s64(VEC, C))

/***************************
 * Logical
 ***************************/

// AND
#define v_and_u8  vandq_u8
#define v_and_s8  vandq_s8
#define v_and_u16 vandq_u16
#define v_and_s16 vandq_s16
#define v_and_u32 vandq_u32
#define v_and_s32 vandq_s32
#define v_and_u64 vandq_u64
#define v_and_s64 vandq_s64
#define v_and_f32(A, B) \
    vreinterpretq_f32_u8(vandq_u8(vreinterpretq_u8_f32(A), vreinterpretq_u8_f32(B)))
#define v_and_f64(A, B) \
    vreinterpretq_f64_u8(vandq_u8(vreinterpretq_u8_f64(A), vreinterpretq_u8_f64(B)))

// OR
#define v_or_u8  vorrq_u8
#define v_or_s8  vorrq_s8
#define v_or_u16 vorrq_u16
#define v_or_s16 vorrq_s16
#define v_or_u32 vorrq_u32
#define v_or_s32 vorrq_s32
#define v_or_u64 vorrq_u64
#define v_or_s64 vorrq_s64
#define v_or_f32(A, B) \
    vreinterpretq_f32_u8(vorrq_u8(vreinterpretq_u8_f32(A), vreinterpretq_u8_f32(B)))
#define v_or_f64(A, B) \
    vreinterpretq_f64_u8(vorrq_u8(vreinterpretq_u8_f64(A), vreinterpretq_u8_f64(B)))

// XOR
#define v_xor_u8  veorq_u8
#define v_xor_s8  veorq_s8
#define v_xor_u16 veorq_u16
#define v_xor_s16 veorq_s16
#define v_xor_u32 veorq_u32
#define v_xor_s32 veorq_s32
#define v_xor_u64 veorq_u64
#define v_xor_s64 veorq_s64
#define v_xor_f32(A, B) \
    vreinterpretq_f32_u8(veorq_u8(vreinterpretq_u8_f32(A), vreinterpretq_u8_f32(B)))
#define v_xor_f64(A, B) \
    vreinterpretq_f64_u8(veorq_u8(vreinterpretq_u8_f64(A), vreinterpretq_u8_f64(B)))

// NOT
#define v_not_u8  vmvnq_u8
#define v_not_s8  vmvnq_s8
#define v_not_u16 vmvnq_u16
#define v_not_s16 vmvnq_s16
#define v_not_u32 vmvnq_u32
#define v_not_s32 vmvnq_s32
#define v_not_u64(A) vreinterpretq_u64_u8(vmvnq_u8(vreinterpretq_u8_u64(A)))
#define v_not_s64(A) vreinterpretq_s64_u8(vmvnq_u8(vreinterpretq_u8_s64(A)))
#define v_not_f32(A) vreinterpretq_f32_u8(vmvnq_u8(vreinterpretq_u8_f32(A)))
#define v_not_f64(A) vreinterpretq_f64_u8(vmvnq_u8(vreinterpretq_u8_f64(A)))

/***************************
 * Comparison
 ***************************/

// equal
#define v_cmpeq_u8  vceqq_u8
#define v_cmpeq_s8  vceqq_s8
#define v_cmpeq_u16 vceqq_u16
#define v_cmpeq_s16 vceqq_s16
#define v_cmpeq_u32 vceqq_u32
#define v_cmpeq_s32 vceqq_s32
#define v_cmpeq_f32 vceqq_f32
#define v_cmpeq_f64 vceqq_f64

#ifdef __aarch64__
    #define v_cmpeq_u64 vceqq_u64
    #define v_cmpeq_s64 vceqq_s64
#else
    NPY_FINLINE uint64x2_t v_cmpeq_u64(uint64x2_t a, uint64x2_t b)
    {
        uint64x2_t cmpeq = vreinterpretq_u64_u32(vceqq_u32(
            vreinterpretq_u32_u64(a), vreinterpretq_u32_u64(b)
        ));
        uint64x2_t cmpeq_h = vshlq_n_u64(cmpeq, 32);
        uint64x2_t test = vandq_u64(cmpeq, cmpeq_h);
        return vreinterpretq_u64_s64(vshrq_n_s64(vreinterpretq_s64_u64(test), 32));
    }
    #define v_cmpeq_s64(A, B) \
        v_cmpeq_u64(vreinterpretq_u64_s64(A), vreinterpretq_u64_s64(B))
#endif

// not Equal
#define v_cmpneq_u8(A, B)  vmvnq_u8(vceqq_u8(A, B))
#define v_cmpneq_s8(A, B)  vmvnq_u8(vceqq_s8(A, B))
#define v_cmpneq_u16(A, B) vmvnq_u16(vceqq_u16(A, B))
#define v_cmpneq_s16(A, B) vmvnq_u16(vceqq_s16(A, B))
#define v_cmpneq_u32(A, B) vmvnq_u32(vceqq_u32(A, B))
#define v_cmpneq_s32(A, B) vmvnq_u32(vceqq_s32(A, B))
#define v_cmpneq_u64(A, B) v_not_u64(v_cmpeq_u64(A, B))
#define v_cmpneq_s64(A, B) v_not_u64(v_cmpeq_s64(A, B))
#define v_cmpneq_f32(A, B) vmvnq_u32(vceqq_f32(A, B))
#define v_cmpneq_f64(A, B) v_not_u64(vceqq_f64(A, B))

// greater than
#define v_cmpgt_u8  vcgtq_u8
#define v_cmpgt_s8  vcgtq_s8
#define v_cmpgt_u16 vcgtq_u16
#define v_cmpgt_s16 vcgtq_s16
#define v_cmpgt_u32 vcgtq_u32
#define v_cmpgt_s32 vcgtq_s32
#define v_cmpgt_f32 vcgtq_f32
#define v_cmpgt_f64 vcgtq_f64

#ifdef __aarch64__
    #define v_cmpgt_u64 vcgtq_u64
    #define v_cmpgt_s64 vcgtq_s64
#else
    NPY_FINLINE uint64x2_t v_cmpgt_s64(int64x2_t a, int64x2_t b)
    {
        int64x2_t sub = vsubq_s64(b, a);
        uint64x2_t nsame_sbit = vreinterpretq_u64_s64(veorq_s64(a, b));
        int64x2_t test = vbslq_s64(nsame_sbit, b, sub);
        int64x2_t extend_sbit = vshrq_n_s64(test, 63);
        return  vreinterpretq_u64_s64(extend_sbit);
    }
    NPY_FINLINE uint64x2_t v_cmpgt_u64(uint64x2_t a, uint64x2_t b)
    {
        const uint64x2_t sbit = v_setall_u64(0x8000000000000000);
        a = v_xor_u64(a, sbit);
        b = v_xor_u64(b, sbit);
        return v_cmpgt_s64(vreinterpretq_s64_u64(a), vreinterpretq_s64_u64(b));
    }
#endif

// greater than or equal
#define v_cmpge_u8  vcgeq_u8
#define v_cmpge_s8  vcgeq_s8
#define v_cmpge_u16 vcgeq_u16
#define v_cmpge_s16 vcgeq_s16
#define v_cmpge_u32 vcgeq_u32
#define v_cmpge_s32 vcgeq_s32
#define v_cmpge_f32 vcgeq_f32
#define v_cmpge_f64 vcgeq_f64

#ifdef __aarch64__
    #define v_cmpge_u64 vcgeq_u64
    #define v_cmpge_s64 vcgeq_s64
#else
    #define v_cmpge_u64(A, B) v_not_u64(v_cmpgt_u64(B, A))
    #define v_cmpge_s64(A, B) v_not_u64(v_cmpgt_s64(B, A))
#endif

// less than
#define v_cmplt_u8(A, B)  v_cmpgt_u8(B, A)
#define v_cmplt_s8(A, B)  v_cmpgt_s8(B, A)
#define v_cmplt_u16(A, B) v_cmpgt_u16(B, A)
#define v_cmplt_s16(A, B) v_cmpgt_s16(B, A)
#define v_cmplt_u32(A, B) v_cmpgt_u32(B, A)
#define v_cmplt_s32(A, B) v_cmpgt_s32(B, A)
#define v_cmplt_u64(A, B) v_cmpgt_u64(B, A)
#define v_cmplt_s64(A, B) v_cmpgt_s64(B, A)
#define v_cmplt_f32(A, B) v_cmpgt_f32(B, A)
#define v_cmplt_f64(A, B) v_cmpgt_f64(B, A)

// less than or equal
#define v_cmple_u8(A, B)  v_cmpge_u8(B, A)
#define v_cmple_s8(A, B)  v_cmpge_s8(B, A)
#define v_cmple_u16(A, B) v_cmpge_u16(B, A)
#define v_cmple_s16(A, B) v_cmpge_s16(B, A)
#define v_cmple_u32(A, B) v_cmpge_u32(B, A)
#define v_cmple_s32(A, B) v_cmpge_s32(B, A)
#define v_cmple_u64(A, B) v_cmpge_u64(B, A)
#define v_cmple_s64(A, B) v_cmpge_s64(B, A)
#define v_cmple_f32(A, B) v_cmpge_f32(B, A)
#define v_cmple_f64(A, B) v_cmpge_f64(B, A)

#endif // _NPY_SIMD_NEON_OPERATORS_H
