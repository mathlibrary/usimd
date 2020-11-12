#ifndef NPY_SIMD
    #error "Not a standalone header"
#endif

#ifndef _NPY_SIMD_VSX_OPERATORS_H
#define _NPY_SIMD_VSX_OPERATORS_H

/***************************
 * Shifting
 ***************************/

// Left
#define v_shl_u16(A, C) vec_sl(A, v_setall_u16(C))
#define v_shl_s16(A, C) vec_sl(A, v_setall_u16(C))
#define v_shl_u32(A, C) vec_sl(A, v_setall_u32(C))
#define v_shl_s32(A, C) vec_sl(A, v_setall_u32(C))
#define v_shl_u64(A, C) vec_sl(A, v_setall_u64(C))
#define v_shl_s64(A, C) vec_sl(A, v_setall_u64(C))

// Left by an immediate constant
#define v_shli_u16 v_shl_u16
#define v_shli_s16 v_shl_s16
#define v_shli_u32 v_shl_u32
#define v_shli_s32 v_shl_s32
#define v_shli_u64 v_shl_u64
#define v_shli_s64 v_shl_s64

// Right
#define v_shr_u16(A, C) vec_sr(A,  v_setall_u16(C))
#define v_shr_s16(A, C) vec_sra(A, v_setall_u16(C))
#define v_shr_u32(A, C) vec_sr(A,  v_setall_u32(C))
#define v_shr_s32(A, C) vec_sra(A, v_setall_u32(C))
#define v_shr_u64(A, C) vec_sr(A,  v_setall_u64(C))
#define v_shr_s64(A, C) vec_sra(A, v_setall_u64(C))

// Right by an immediate constant
#define v_shri_u16 v_shr_u16
#define v_shri_s16 v_shr_s16
#define v_shri_u32 v_shr_u32
#define v_shri_s32 v_shr_s32
#define v_shri_u64 v_shr_u64
#define v_shri_s64 v_shr_s64

/***************************
 * Logical
 ***************************/

// AND
#define v_and_u8  vec_and
#define v_and_s8  vec_and
#define v_and_u16 vec_and
#define v_and_s16 vec_and
#define v_and_u32 vec_and
#define v_and_s32 vec_and
#define v_and_u64 vec_and
#define v_and_s64 vec_and
#define v_and_f32 vec_and
#define v_and_f64 vec_and

// OR
#define v_or_u8  vec_or
#define v_or_s8  vec_or
#define v_or_u16 vec_or
#define v_or_s16 vec_or
#define v_or_u32 vec_or
#define v_or_s32 vec_or
#define v_or_u64 vec_or
#define v_or_s64 vec_or
#define v_or_f32 vec_or
#define v_or_f64 vec_or

// XOR
#define v_xor_u8  vec_xor
#define v_xor_s8  vec_xor
#define v_xor_u16 vec_xor
#define v_xor_s16 vec_xor
#define v_xor_u32 vec_xor
#define v_xor_s32 vec_xor
#define v_xor_u64 vec_xor
#define v_xor_s64 vec_xor
#define v_xor_f32 vec_xor
#define v_xor_f64 vec_xor

// NOT
// note: we implement v_not_b*(boolen types) for internal use*/
#define NPYV_IMPL_VSX_NOT_INT(VEC_LEN)                                 \
    NPY_FINLINE v_u##VEC_LEN v_not_u##VEC_LEN(v_u##VEC_LEN a) \
    { return vec_nor(a, a); }                                          \
    NPY_FINLINE v_s##VEC_LEN v_not_s##VEC_LEN(v_s##VEC_LEN a) \
    { return vec_nor(a, a); }                                          \
    NPY_FINLINE v_b##VEC_LEN v_not_b##VEC_LEN(v_b##VEC_LEN a) \
    { return vec_nor(a, a); }

NPYV_IMPL_VSX_NOT_INT(8)
NPYV_IMPL_VSX_NOT_INT(16)
NPYV_IMPL_VSX_NOT_INT(32)

// up to gcc5 vec_nor doesn't support bool long long
#if defined(__GNUC__) && __GNUC__ > 5
    NPYV_IMPL_VSX_NOT_INT(64)
#else
    NPY_FINLINE v_u64 v_not_u64(v_u64 a)
    { return vec_nor(a, a); }
    NPY_FINLINE v_s64 v_not_s64(v_s64 a)
    { return vec_nor(a, a); }
    NPY_FINLINE v_b64 v_not_b64(v_b64 a)
    { return (v_b64)vec_nor((v_u64)a, (v_u64)a); }
#endif

NPY_FINLINE v_f32 v_not_f32(v_f32 a)
{ return vec_nor(a, a); }
NPY_FINLINE v_f64 v_not_f64(v_f64 a)
{ return vec_nor(a, a); }

/***************************
 * Comparison
 ***************************/

// Int Equal
#define v_cmpeq_u8  vec_cmpeq
#define v_cmpeq_s8  vec_cmpeq
#define v_cmpeq_u16 vec_cmpeq
#define v_cmpeq_s16 vec_cmpeq
#define v_cmpeq_u32 vec_cmpeq
#define v_cmpeq_s32 vec_cmpeq
#define v_cmpeq_u64 vec_cmpeq
#define v_cmpeq_s64 vec_cmpeq
#define v_cmpeq_f32 vec_cmpeq
#define v_cmpeq_f64 vec_cmpeq

// Int Not Equal
#ifdef NPY_HAVE_VSX3
    #define v_cmpneq_u8  vec_cmpne
    #define v_cmpneq_s8  vec_cmpne
    #define v_cmpneq_u16 vec_cmpne
    #define v_cmpneq_s16 vec_cmpne
    #define v_cmpneq_u32 vec_cmpne
    #define v_cmpneq_s32 vec_cmpne
    #define v_cmpneq_u64 vec_cmpne
    #define v_cmpneq_s64 vec_cmpne
    #define v_cmpneq_f32 vec_cmpne
    #define v_cmpneq_f64 vec_cmpne
#else
    #define v_cmpneq_u8(A, B)  v_not_b8(vec_cmpeq(A, B))
    #define v_cmpneq_s8(A, B)  v_not_b8(vec_cmpeq(A, B))
    #define v_cmpneq_u16(A, B) v_not_b16(vec_cmpeq(A, B))
    #define v_cmpneq_s16(A, B) v_not_b16(vec_cmpeq(A, B))
    #define v_cmpneq_u32(A, B) v_not_b32(vec_cmpeq(A, B))
    #define v_cmpneq_s32(A, B) v_not_b32(vec_cmpeq(A, B))
    #define v_cmpneq_u64(A, B) v_not_b64(vec_cmpeq(A, B))
    #define v_cmpneq_s64(A, B) v_not_b64(vec_cmpeq(A, B))
    #define v_cmpneq_f32(A, B) v_not_b32(vec_cmpeq(A, B))
    #define v_cmpneq_f64(A, B) v_not_b64(vec_cmpeq(A, B))
#endif

// Greater than
#define v_cmpgt_u8  vec_cmpgt
#define v_cmpgt_s8  vec_cmpgt
#define v_cmpgt_u16 vec_cmpgt
#define v_cmpgt_s16 vec_cmpgt
#define v_cmpgt_u32 vec_cmpgt
#define v_cmpgt_s32 vec_cmpgt
#define v_cmpgt_u64 vec_cmpgt
#define v_cmpgt_s64 vec_cmpgt
#define v_cmpgt_f32 vec_cmpgt
#define v_cmpgt_f64 vec_cmpgt

// Greater than or equal
// up to gcc5 vec_cmpge only supports single and double precision
#if defined(__GNUC__) && __GNUC__ > 5
    #define v_cmpge_u8  vec_cmpge
    #define v_cmpge_s8  vec_cmpge
    #define v_cmpge_u16 vec_cmpge
    #define v_cmpge_s16 vec_cmpge
    #define v_cmpge_u32 vec_cmpge
    #define v_cmpge_s32 vec_cmpge
    #define v_cmpge_u64 vec_cmpge
    #define v_cmpge_s64 vec_cmpge
#else
    #define v_cmpge_u8(A, B)  v_not_b8(vec_cmpgt(B, A))
    #define v_cmpge_s8(A, B)  v_not_b8(vec_cmpgt(B, A))
    #define v_cmpge_u16(A, B) v_not_b16(vec_cmpgt(B, A))
    #define v_cmpge_s16(A, B) v_not_b16(vec_cmpgt(B, A))
    #define v_cmpge_u32(A, B) v_not_b32(vec_cmpgt(B, A))
    #define v_cmpge_s32(A, B) v_not_b32(vec_cmpgt(B, A))
    #define v_cmpge_u64(A, B) v_not_b64(vec_cmpgt(B, A))
    #define v_cmpge_s64(A, B) v_not_b64(vec_cmpgt(B, A))
#endif
#define v_cmpge_f32 vec_cmpge
#define v_cmpge_f64 vec_cmpge

// Less than
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

// Less than or equal
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

#endif // _NPY_SIMD_VSX_OPERATORS_H
