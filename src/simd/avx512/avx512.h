#ifndef _NPY_SIMD_H_
    #error "Not a standalone header"
#endif
#define NPY_SIMD 512
#define NPY_SIMD_WIDTH 64
#define NPY_SIMD_F64 1
// Enough limit to allow us to use _mm512_i32gather_* and _mm512_i32scatter_*
#define NPY_SIMD_MAXLOAD_STRIDE32  (0x7fffffff / 16)
#define NPY_SIMD_MAXSTORE_STRIDE32 (0x7fffffff / 16)

typedef __m512i v_u8;
typedef __m512i v_s8;
typedef __m512i v_u16;
typedef __m512i v_s16;
typedef __m512i v_u32;
typedef __m512i v_s32;
typedef __m512i v_u64;
typedef __m512i v_s64;
typedef __m512  v_f32;
typedef __m512d v_f64;

#ifdef NPY_HAVE_AVX512BW
typedef __mmask64 v_b8;
typedef __mmask32 v_b16;
#else
typedef __m512i v_b8;
typedef __m512i v_b16;
#endif
typedef __mmask16 v_b32;
typedef __mmask8  v_b64;

typedef struct { __m512i val[2]; } v_m512ix2;
typedef v_m512ix2 v_u8x2;
typedef v_m512ix2 v_s8x2;
typedef v_m512ix2 v_u16x2;
typedef v_m512ix2 v_s16x2;
typedef v_m512ix2 v_u32x2;
typedef v_m512ix2 v_s32x2;
typedef v_m512ix2 v_u64x2;
typedef v_m512ix2 v_s64x2;

typedef struct { __m512i val[3]; } v_m512ix3;
typedef v_m512ix3 v_u8x3;
typedef v_m512ix3 v_s8x3;
typedef v_m512ix3 v_u16x3;
typedef v_m512ix3 v_s16x3;
typedef v_m512ix3 v_u32x3;
typedef v_m512ix3 v_s32x3;
typedef v_m512ix3 v_u64x3;
typedef v_m512ix3 v_s64x3;

typedef struct { __m512  val[2]; } v_f32x2;
typedef struct { __m512d val[2]; } v_f64x2;
typedef struct { __m512  val[3]; } v_f32x3;
typedef struct { __m512d val[3]; } v_f64x3;

#define v_nlanes_u8  64
#define v_nlanes_s8  64
#define v_nlanes_u16 32
#define v_nlanes_s16 32
#define v_nlanes_u32 16
#define v_nlanes_s32 16
#define v_nlanes_u64 8
#define v_nlanes_s64 8
#define v_nlanes_f32 16
#define v_nlanes_f64 8

#include "utils.h"
#include "memory.h"
#include "misc.h"
#include "reorder.h"
#include "operators.h"
#include "conversion.h"
#include "arithmetic.h"
#include "math.h"
