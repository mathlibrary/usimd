#ifndef _V_SIMD_H_
    #error "Not a standalone header"
#endif

#define V_SIMD 128
#define V_SIMD_WIDTH 16
#define V_SIMD_F64 1

typedef __m128i v_u8;
typedef __m128i v_s8;
typedef __m128i v_u16;
typedef __m128i v_s16;
typedef __m128i v_u32;
typedef __m128i v_s32;
typedef __m128i v_u64;
typedef __m128i v_s64;
typedef __m128  v_f32;
typedef __m128d v_f64;

typedef __m128i v_b8;
typedef __m128i v_b16;
typedef __m128i v_b32;
typedef __m128i v_b64;

typedef struct { __m128i val[2]; } v_m128ix2;
typedef v_m128ix2 v_u8x2;
typedef v_m128ix2 v_s8x2;
typedef v_m128ix2 v_u16x2;
typedef v_m128ix2 v_s16x2;
typedef v_m128ix2 v_u32x2;
typedef v_m128ix2 v_s32x2;
typedef v_m128ix2 v_u64x2;
typedef v_m128ix2 v_s64x2;

typedef struct { __m128i val[3]; } v_m128ix3;
typedef v_m128ix3 v_u8x3;
typedef v_m128ix3 v_s8x3;
typedef v_m128ix3 v_u16x3;
typedef v_m128ix3 v_s16x3;
typedef v_m128ix3 v_u32x3;
typedef v_m128ix3 v_s32x3;
typedef v_m128ix3 v_u64x3;
typedef v_m128ix3 v_s64x3;

typedef struct { __m128  val[2]; } v_f32x2;
typedef struct { __m128d val[2]; } v_f64x2;
typedef struct { __m128  val[3]; } v_f32x3;
typedef struct { __m128d val[3]; } v_f64x3;

#define v_nlanes_u8  16
#define v_nlanes_s8  16
#define v_nlanes_u16 8
#define v_nlanes_s16 8
#define v_nlanes_u32 4
#define v_nlanes_s32 4
#define v_nlanes_u64 2
#define v_nlanes_s64 2
#define v_nlanes_f32 4
#define v_nlanes_f64 2

#include "memory.h"
#include "misc.h"
#include "reorder.h"
#include "operators.h"
#include "conversion.h"
#include "arithmetic.h"
#include "math.h"
