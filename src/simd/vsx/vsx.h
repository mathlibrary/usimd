#ifndef _NPY_SIMD_H_
    #error "Not a standalone header"
#endif

#define NPY_SIMD 128
#define NPY_SIMD_WIDTH 16
#define NPY_SIMD_F64 1

typedef __vector unsigned char      v_u8;
typedef __vector signed char        v_s8;
typedef __vector unsigned short     v_u16;
typedef __vector signed short       v_s16;
typedef __vector unsigned int       v_u32;
typedef __vector signed int         v_s32;
typedef __vector unsigned long long v_u64;
typedef __vector signed long long   v_s64;
typedef __vector float              v_f32;
typedef __vector double             v_f64;

typedef struct { v_u8  val[2]; } v_u8x2;
typedef struct { v_s8  val[2]; } v_s8x2;
typedef struct { v_u16 val[2]; } v_u16x2;
typedef struct { v_s16 val[2]; } v_s16x2;
typedef struct { v_u32 val[2]; } v_u32x2;
typedef struct { v_s32 val[2]; } v_s32x2;
typedef struct { v_u64 val[2]; } v_u64x2;
typedef struct { v_s64 val[2]; } v_s64x2;
typedef struct { v_f32 val[2]; } v_f32x2;
typedef struct { v_f64 val[2]; } v_f64x2;

typedef struct { v_u8  val[3]; } v_u8x3;
typedef struct { v_s8  val[3]; } v_s8x3;
typedef struct { v_u16 val[3]; } v_u16x3;
typedef struct { v_s16 val[3]; } v_s16x3;
typedef struct { v_u32 val[3]; } v_u32x3;
typedef struct { v_s32 val[3]; } v_s32x3;
typedef struct { v_u64 val[3]; } v_u64x3;
typedef struct { v_s64 val[3]; } v_s64x3;
typedef struct { v_f32 val[3]; } v_f32x3;
typedef struct { v_f64 val[3]; } v_f64x3;

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

// using __bool with typdef cause ambiguous errors
#define v_b8  __vector __bool char
#define v_b16 __vector __bool short
#define v_b32 __vector __bool int
#define v_b64 __vector __bool long long

#include "memory.h"
#include "misc.h"
#include "reorder.h"
#include "operators.h"
#include "conversion.h"
#include "arithmetic.h"
#include "math.h"
