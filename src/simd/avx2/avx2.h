#ifndef _V_SIMD_H_
    #error "Not a standalone header"
#endif

#define V_SIMD 256
#define V_SIMD_WIDTH 32
#define V_SIMD_F64 1
// Enough limit to allow us to use _mm256_i32gather_*
#define V_SIMD_MAXLOAD_STRIDE32 (0x7fffffff / 8)

typedef __m256i v_u8;
typedef __m256i v_s8;
typedef __m256i v_u16;
typedef __m256i v_s16;
typedef __m256i v_u32;
typedef __m256i v_s32;
typedef __m256i v_u64;
typedef __m256i v_s64;
typedef __m256  v_f32;
typedef __m256d v_f64;

typedef __m256i v_b8;
typedef __m256i v_b16;
typedef __m256i v_b32;
typedef __m256i v_b64;

typedef struct { __m256i val[2]; } v_m256ix2;
typedef v_m256ix2 v_u8x2;
typedef v_m256ix2 v_s8x2;
typedef v_m256ix2 v_u16x2;
typedef v_m256ix2 v_s16x2;
typedef v_m256ix2 v_u32x2;
typedef v_m256ix2 v_s32x2;
typedef v_m256ix2 v_u64x2;
typedef v_m256ix2 v_s64x2;

typedef struct { __m256i val[3]; } v_m256ix3;
typedef v_m256ix3 v_u8x3;
typedef v_m256ix3 v_s8x3;
typedef v_m256ix3 v_u16x3;
typedef v_m256ix3 v_s16x3;
typedef v_m256ix3 v_u32x3;
typedef v_m256ix3 v_s32x3;
typedef v_m256ix3 v_u64x3;
typedef v_m256ix3 v_s64x3;

typedef struct { __m256  val[2]; } v_f32x2;
typedef struct { __m256d val[2]; } v_f64x2;
typedef struct { __m256  val[3]; } v_f32x3;
typedef struct { __m256d val[3]; } v_f64x3;

#define v_nlanes_u8  32
#define v_nlanes_s8  32
#define v_nlanes_u16 16
#define v_nlanes_s16 16
#define v_nlanes_u32 8
#define v_nlanes_s32 8
#define v_nlanes_u64 4
#define v_nlanes_s64 4
#define v_nlanes_f32 8
#define v_nlanes_f64 4

#include "utils.h"
#include "memory.h"
#include "misc.h"
#include "reorder.h"
#include "operators.h"
#include "conversion.h"
#include "arithmetic.h"
#include "math.h"
