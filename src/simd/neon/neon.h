#ifndef _V_SIMD_H_
    #error "Not a standalone header"
#endif

#define V_SIMD 128
#define V_SIMD_WIDTH 16

#ifdef __aarch64__
    #define V_SIMD_F64 1
#else
    #define V_SIMD_F64 0
#endif

typedef uint8x16_t  v_u8;
typedef int8x16_t   v_s8;
typedef uint16x8_t  v_u16;
typedef int16x8_t   v_s16;
typedef uint32x4_t  v_u32;
typedef int32x4_t   v_s32;
typedef uint64x2_t  v_u64;
typedef int64x2_t   v_s64;
typedef float32x4_t v_f32;
#if V_SIMD_F64
typedef float64x2_t v_f64;
#endif

typedef uint8x16_t  v_b8;
typedef uint16x8_t  v_b16;
typedef uint32x4_t  v_b32;
typedef uint64x2_t  v_b64;

typedef uint8x16x2_t  v_u8x2;
typedef int8x16x2_t   v_s8x2;
typedef uint16x8x2_t  v_u16x2;
typedef int16x8x2_t   v_s16x2;
typedef uint32x4x2_t  v_u32x2;
typedef int32x4x2_t   v_s32x2;
typedef uint64x2x2_t  v_u64x2;
typedef int64x2x2_t   v_s64x2;
typedef float32x4x2_t v_f32x2;
#if V_SIMD_F64
typedef float64x2x2_t v_f64x2;
#endif

typedef uint8x16x3_t  v_u8x3;
typedef int8x16x3_t   v_s8x3;
typedef uint16x8x3_t  v_u16x3;
typedef int16x8x3_t   v_s16x3;
typedef uint32x4x3_t  v_u32x3;
typedef int32x4x3_t   v_s32x3;
typedef uint64x2x3_t  v_u64x3;
typedef int64x2x3_t   v_s64x3;
typedef float32x4x3_t v_f32x3;
#if V_SIMD_F64
typedef float64x2x3_t v_f64x3;
#endif

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
