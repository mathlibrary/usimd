#ifndef _NPY_SIMD_H_
#define _NPY_SIMD_H_
/**
 * the NumPy C SIMD vectorization interface "NPYV" are types and functions intended
 * to simplify vectorization of code on different platforms, currently supports
 * the following SIMD extensions SSE, AVX2, AVX512, VSX and NEON.
 *
 * TODO: Add an independent sphinx doc.
*/
#include "simd_utils.h"
#include <assert.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char  npy_uint8;
typedef char   npy_int8;
typedef unsigned short npy_uint16;
typedef short  npy_int16;
typedef unsigned int npy_uint32;
typedef int  npy_int32;
typedef unsigned int npy_uintp;
typedef int  npy_intp;
typedef unsigned long npy_uint64;
typedef long  npy_int64;

typedef npy_uint8  npyv_lanetype_u8;
typedef npy_int8   npyv_lanetype_s8;
typedef npy_uint16 npyv_lanetype_u16;
typedef npy_int16  npyv_lanetype_s16;
typedef npy_uint32 npyv_lanetype_u32;
typedef npy_int32  npyv_lanetype_s32;
typedef npy_uint64 npyv_lanetype_u64;
typedef npy_int64  npyv_lanetype_s64;

// lane type by intrin suffix

typedef float      npyv_lanetype_f32;
typedef double     npyv_lanetype_f64;

#if defined(_MSC_VER)
        #define NPY_INLINE __inline
#elif defined(__GNUC__)
    #if defined(__STRICT_ANSI__)
         #define NPY_INLINE __inline__
    #else
         #define NPY_INLINE inline
    #endif
#else
    #define NPY_INLINE
#endif

#ifdef _MSC_VER
    #define NPY_FINLINE static __forceinline
#elif defined(__GNUC__)
    #define NPY_FINLINE static NPY_INLINE __attribute__((always_inline))
#else
    #define NPY_FINLINE static
#endif

// include head
/** SSE **/
#ifdef NPY_HAVE_SSE
#include <xmmintrin.h>
#endif
/** SSE2 **/
#ifdef NPY_HAVE_SSE2
#include <emmintrin.h>
#endif
/** SSE3 **/
#ifdef NPY_HAVE_SSE3
#include <pmmintrin.h>
#endif
/** SSSE3 **/
#ifdef NPY_HAVE_SSSE3
#include <tmmintrin.h>
#endif
/** SSE41 **/
#ifdef NPY_HAVE_SSE4_1
#include <smmintrin.h>
#endif

/** AVX **/
#ifdef NPY_HAVE_AVX
#include <immintrin.h>
#endif

/** NEON **/
#ifdef NPY_HAVE_NEON
#include <arm_neon.h>
#endif

#if defined(NPY_HAVE_AVX512F) && !defined(NPY_SIMD_FORCE_256) && !defined(NPY_SIMD_FORCE_128)
    #include "avx512/avx512.h"
#elif defined(NPY_HAVE_AVX2) && !defined(NPY_SIMD_FORCE_128)
    #include "avx2/avx2.h"
#elif defined(NPY_HAVE_SSE2)
    #include "sse/sse.h"
#endif

// TODO: Add support for VSX(2.06) and BE Mode
#if defined(NPY_HAVE_VSX2) && defined(__LITTLE_ENDIAN__)
    #include "vsx/vsx.h"
#endif

#ifdef NPY_HAVE_NEON
    #include "neon/neon.h"
#endif

#ifndef NPY_SIMD
    #define NPY_SIMD 0
    #define NPY_SIMD_WIDTH 0
    #define NPY_SIMD_F64 0
#endif
/**
 * Some SIMD extensions currently(AVX2, AVX512F) require (de facto)
 * a maximum number of strides sizes when dealing with non-contiguous memory access.
 *
 * Therefore the following functions must be used to check the maximum
 * acceptable limit of strides before using any of non-contiguous load/store intrinsics.
 *
 * For instance:
 *  npy_intp ld_stride = step[0] / sizeof(float);
 *  npy_intp st_stride = step[1] / sizeof(float);
 *
 *  if (npyv_loadable_stride_f32(ld_stride) && npyv_storable_stride_f32(st_stride)) {
 *      for (;;)
 *          npyv_f32 a = npyv_loadn_f32(ld_pointer, ld_stride);
 *          // ...
 *          npyv_storen_f32(st_pointer, st_stride, a);
 *  }
 *  else {
 *      for (;;)
 *          // C scalars
 *  }
 */
#ifndef NPY_SIMD_MAXLOAD_STRIDE32
    #define NPY_SIMD_MAXLOAD_STRIDE32 0
#endif
#ifndef NPY_SIMD_MAXSTORE_STRIDE32
    #define NPY_SIMD_MAXSTORE_STRIDE32 0
#endif
#ifndef NPY_SIMD_MAXLOAD_STRIDE64
    #define NPY_SIMD_MAXLOAD_STRIDE64 0
#endif
#ifndef NPY_SIMD_MAXSTORE_STRIDE64
    #define NPY_SIMD_MAXSTORE_STRIDE64 0
#endif
#define NPYV_IMPL_MAXSTRIDE(SFX, MAXLOAD, MAXSTORE) \
    NPY_FINLINE int npyv_loadable_stride_##SFX(npy_intp stride) \
    { return MAXLOAD > 0 ? llabs(stride) <= MAXLOAD : 1; } \
    NPY_FINLINE int npyv_storable_stride_##SFX(npy_intp stride) \
    { return MAXSTORE > 0 ? llabs(stride) <= MAXSTORE : 1; }
#if NPY_SIMD
    NPYV_IMPL_MAXSTRIDE(u32, NPY_SIMD_MAXLOAD_STRIDE32, NPY_SIMD_MAXSTORE_STRIDE32)
    NPYV_IMPL_MAXSTRIDE(s32, NPY_SIMD_MAXLOAD_STRIDE32, NPY_SIMD_MAXSTORE_STRIDE32)
    NPYV_IMPL_MAXSTRIDE(f32, NPY_SIMD_MAXLOAD_STRIDE32, NPY_SIMD_MAXSTORE_STRIDE32)
    NPYV_IMPL_MAXSTRIDE(u64, NPY_SIMD_MAXLOAD_STRIDE64, NPY_SIMD_MAXSTORE_STRIDE64)
    NPYV_IMPL_MAXSTRIDE(s64, NPY_SIMD_MAXLOAD_STRIDE64, NPY_SIMD_MAXSTORE_STRIDE64)
#endif
#if NPY_SIMD_F64
    NPYV_IMPL_MAXSTRIDE(f64, NPY_SIMD_MAXLOAD_STRIDE64, NPY_SIMD_MAXSTORE_STRIDE64)
#endif

#ifdef __cplusplus
}
#endif
#endif // _NPY_SIMD_H_
