#ifndef _V_SIMD_H_
#define _V_SIMD_H_
/**
 * the NumPy C SIMD vectorization interface "V" are types and functions intended
 * to simplify vectorization of code on different platforms, currently supports
 * the following SIMD extensions SSE, AVX2, AVX512, VSX and NEON.
 *
 * TODO: Add an independent sphinx doc.
*/
#include "simd_utils.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char  s_uint8;
typedef char   s_int8;
typedef unsigned short s_uint16;
typedef short  s_int16;
typedef unsigned int s_uint32;
typedef int  s_int32;
typedef unsigned int s_uintp;
typedef int  s_intp;
typedef unsigned long s_uint64;
typedef long  s_int64;

typedef s_uint8  v_lanetype_u8;
typedef s_int8   v_lanetype_s8;
typedef s_uint16 v_lanetype_u16;
typedef s_int16  v_lanetype_s16;
typedef s_uint32 v_lanetype_u32;
typedef s_int32  v_lanetype_s32;
typedef s_uint64 v_lanetype_u64;
typedef s_int64  v_lanetype_s64;

// lane type by intrin suffix

typedef float      v_lanetype_f32;
typedef double     v_lanetype_f64;

#if defined(_MSC_VER)
        #define V_INLINE __inline
#elif defined(__GNUC__)
    #if defined(__STRICT_ANSI__)
         #define V_INLINE __inline__
    #else
         #define V_INLINE inline
    #endif
#else
    #define V_INLINE
#endif

#ifdef _MSC_VER
    #define V_FINLINE static __forceinline
#elif defined(__GNUC__)
    #define V_FINLINE static V_INLINE __attribute__((always_inline))
#else
    #define V_FINLINE static
#endif

#if defined(__GNUC__) || defined(__ICC) || defined(__clang__)
    #define V_DECL_ALIGNED(x) __attribute__ ((aligned (x)))
#elif defined(_MSC_VER)
    #define V_DECL_ALIGNED(x) __declspec(align(x))
#else
    #define V_DECL_ALIGNED(x)
#endif

#define V_CAT__(a, b) a ## b
#define V_CAT_(a, b) V_CAT__(a, b)
#define V_CAT(a, b) V_CAT_(a, b)
#define V_EXPAND(x) x
// include head
/** SSE **/
#ifdef V_HAVE_SSE
#include <xmmintrin.h>
#endif
/** SSE2 **/
#ifdef V_HAVE_SSE2
#include <emmintrin.h>
#endif
/** SSE3 **/
#ifdef V_HAVE_SSE3
#include <pmmintrin.h>
#endif
/** SSSE3 **/
#ifdef V_HAVE_SSSE3
#include <tmmintrin.h>
#endif
/** SSE41 **/
#ifdef V_HAVE_SSE4_1
#include <smmintrin.h>
#endif
/** SSE42 **/
#ifdef V_HAVE_SSE4_2
#include <nmmintrin.h>
#endif
/** AVX **/
#ifdef V_HAVE_AVX
#include <immintrin.h>
#endif

/** NEON **/
#ifdef V_HAVE_NEON
#include <arm_neon.h>
#endif



#if defined(V_HAVE_AVX512F) && !defined(V_SIMD_FORCE_256) && !defined(V_SIMD_FORCE_128)
    #include "avx512/avx512.h"
#elif defined(V_HAVE_AVX2) && !defined(V_SIMD_FORCE_128)
    #include "avx2/avx2.h"
#elif defined(V_HAVE_SSE2)
    #include "sse/sse.h"
#endif

// TODO: Add support for VSX(2.06) and BE Mode
#if defined(V_HAVE_VSX2) && defined(__LITTLE_ENDIAN__)
    #include "vsx/vsx.h"
#endif

#ifdef V_HAVE_NEON
    #include "neon/neon.h"
#endif

#ifndef V_SIMD
    #define V_SIMD 0
    #define V_SIMD_WIDTH 0
    #define V_SIMD_F64 0
#endif
/**
 * Some SIMD extensions currently(AVX2, AVX512F) require (de facto)
 * a maximum number of strides sizes when dealing with non-contiguous memory access.
 *
 * Therefore the following functions must be used to check the maximum
 * acceptable limit of strides before using any of non-contiguous load/store intrinsics.
 *
 * For instance:
 *  s_intp ld_stride = step[0] / sizeof(float);
 *  s_intp st_stride = step[1] / sizeof(float);
 *
 *  if (v_loadable_stride_f32(ld_stride) && v_storable_stride_f32(st_stride)) {
 *      for (;;)
 *          v_f32 a = v_loadn_f32(ld_pointer, ld_stride);
 *          // ...
 *          v_storen_f32(st_pointer, st_stride, a);
 *  }
 *  else {
 *      for (;;)
 *          // C scalars
 *  }
 */
#ifndef V_SIMD_MAXLOAD_STRIDE32
    #define V_SIMD_MAXLOAD_STRIDE32 0
#endif
#ifndef V_SIMD_MAXSTORE_STRIDE32
    #define V_SIMD_MAXSTORE_STRIDE32 0
#endif
#ifndef V_SIMD_MAXLOAD_STRIDE64
    #define V_SIMD_MAXLOAD_STRIDE64 0
#endif
#ifndef V_SIMD_MAXSTORE_STRIDE64
    #define V_SIMD_MAXSTORE_STRIDE64 0
#endif
#define V_IMPL_MAXSTRIDE(SFX, MAXLOAD, MAXSTORE) \
    V_FINLINE int v_loadable_stride_##SFX(s_intp stride) \
    { return MAXLOAD > 0 ? llabs(stride) <= MAXLOAD : 1; } \
    V_FINLINE int v_storable_stride_##SFX(s_intp stride) \
    { return MAXSTORE > 0 ? llabs(stride) <= MAXSTORE : 1; }
#if V_SIMD
    V_IMPL_MAXSTRIDE(u32, V_SIMD_MAXLOAD_STRIDE32, V_SIMD_MAXSTORE_STRIDE32)
    V_IMPL_MAXSTRIDE(s32, V_SIMD_MAXLOAD_STRIDE32, V_SIMD_MAXSTORE_STRIDE32)
    V_IMPL_MAXSTRIDE(f32, V_SIMD_MAXLOAD_STRIDE32, V_SIMD_MAXSTORE_STRIDE32)
    V_IMPL_MAXSTRIDE(u64, V_SIMD_MAXLOAD_STRIDE64, V_SIMD_MAXSTORE_STRIDE64)
    V_IMPL_MAXSTRIDE(s64, V_SIMD_MAXLOAD_STRIDE64, V_SIMD_MAXSTORE_STRIDE64)
#endif
#if V_SIMD_F64
    V_IMPL_MAXSTRIDE(f64, V_SIMD_MAXLOAD_STRIDE64, V_SIMD_MAXSTORE_STRIDE64)
#endif

#ifdef __cplusplus
}
#endif
#endif // _V_SIMD_H_
