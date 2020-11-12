#ifndef NPY_SIMD
    #error "Not a standalone header"
#endif

#ifndef _NPY_SIMD_VSX_ARITHMETIC_H
#define _NPY_SIMD_VSX_ARITHMETIC_H

/***************************
 * Addition
 ***************************/
// non-saturated
#define v_add_u8  vec_add
#define v_add_s8  vec_add
#define v_add_u16 vec_add
#define v_add_s16 vec_add
#define v_add_u32 vec_add
#define v_add_s32 vec_add
#define v_add_u64 vec_add
#define v_add_s64 vec_add
#define v_add_f32 vec_add
#define v_add_f64 vec_add

// saturated
#define v_adds_u8  vec_adds
#define v_adds_s8  vec_adds
#define v_adds_u16 vec_adds
#define v_adds_s16 vec_adds

/***************************
 * Subtraction
 ***************************/
// non-saturated
#define v_sub_u8  vec_sub
#define v_sub_s8  vec_sub
#define v_sub_u16 vec_sub
#define v_sub_s16 vec_sub
#define v_sub_u32 vec_sub
#define v_sub_s32 vec_sub
#define v_sub_u64 vec_sub
#define v_sub_s64 vec_sub
#define v_sub_f32 vec_sub
#define v_sub_f64 vec_sub

// saturated
#define v_subs_u8  vec_subs
#define v_subs_s8  vec_subs
#define v_subs_u16 vec_subs
#define v_subs_s16 vec_subs

/***************************
 * Multiplication
 ***************************/
// non-saturated
// up to GCC 6 vec_mul only supports precisions and llong
#if defined(__GNUC__) && __GNUC__ < 7
    #define NPYV_IMPL_VSX_MUL(T_VEC, SFX, ...)              \
        NPY_FINLINE T_VEC v_mul_##SFX(T_VEC a, T_VEC b)  \
        {                                                   \
            const v_u8 ev_od = {__VA_ARGS__};            \
            return vec_perm(                                \
                (T_VEC)vec_mule(a, b),                      \
                (T_VEC)vec_mulo(a, b), ev_od                \
            );                                              \
        }

    NPYV_IMPL_VSX_MUL(v_u8,  u8,  0, 16, 2, 18, 4, 20, 6, 22, 8, 24, 10, 26, 12, 28, 14, 30)
    NPYV_IMPL_VSX_MUL(v_s8,  s8,  0, 16, 2, 18, 4, 20, 6, 22, 8, 24, 10, 26, 12, 28, 14, 30)
    NPYV_IMPL_VSX_MUL(v_u16, u16, 0, 1, 16, 17, 4, 5, 20, 21, 8, 9, 24, 25, 12, 13, 28, 29)
    NPYV_IMPL_VSX_MUL(v_s16, s16, 0, 1, 16, 17, 4, 5, 20, 21, 8, 9, 24, 25, 12, 13, 28, 29)

    // vmuluwm can be used for unsigned or signed 32-bit integers
    #define NPYV_IMPL_VSX_MUL_32(T_VEC, SFX)                \
        NPY_FINLINE T_VEC v_mul_##SFX(T_VEC a, T_VEC b)  \
        {                                                   \
            T_VEC ret;                                      \
            __asm__ __volatile__(                           \
                "vmuluwm %0,%1,%2" :                        \
                "=v" (ret) : "v" (a), "v" (b)               \
            );                                              \
            return ret;                                     \
        }

    NPYV_IMPL_VSX_MUL_32(v_u32, u32)
    NPYV_IMPL_VSX_MUL_32(v_s32, s32)

#else
    #define v_mul_u8  vec_mul
    #define v_mul_s8  vec_mul
    #define v_mul_u16 vec_mul
    #define v_mul_s16 vec_mul
    #define v_mul_u32 vec_mul
    #define v_mul_s32 vec_mul
#endif
#define v_mul_f32 vec_mul
#define v_mul_f64 vec_mul

/***************************
 * Division
 ***************************/
#define v_div_f32 vec_div
#define v_div_f64 vec_div

/***************************
 * FUSED
 ***************************/
// multiply and add, a*b + c
#define v_muladd_f32 vec_madd
#define v_muladd_f64 vec_madd
// multiply and subtract, a*b - c
#define v_mulsub_f32 vec_msub
#define v_mulsub_f64 vec_msub
// negate multiply and add, -(a*b) + c
#define v_nmuladd_f32 vec_nmsub // equivalent to -(a*b - c)
#define v_nmuladd_f64 vec_nmsub
// negate multiply and subtract, -(a*b) - c
#define v_nmulsub_f32 vec_nmadd // equivalent to -(a*b + c)
#define v_nmulsub_f64 vec_nmadd

// Horizontal add: Calculates the sum of all vector elements.
NPY_FINLINE float v_sum_f32(v_f32 a)
{
    v_f32 sum = vec_add(a, v_combineh_f32(a, a));
    return vec_extract(sum, 0) + vec_extract(sum, 1);
}

NPY_FINLINE double v_sum_f64(v_f64 a)
{
    return vec_extract(a, 0) + vec_extract(a, 1);
}

#endif // _NPY_SIMD_VSX_ARITHMETIC_H
