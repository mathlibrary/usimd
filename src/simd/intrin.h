#ifndef _NPY_SIMD_H_
#define _NPY_SIMD_H_
/**
 * the NumPy C SIMD vectorization interface "NPYV" are types and functions intended
 * to simplify vectorization of code on different platforms, currently supports
 * the following SIMD extensions SSE, AVX2, AVX512, VSX and NEON.
 *
 * TODO: Add an independent sphinx doc.
*/
#include "numpy/npy_common.h"
#include "npy_cpu_dispatch.h"
#include "simd_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

// lane type by intrin suffix
typedef npy_uint8  npyv_lanetype_u8;
typedef npy_int8   npyv_lanetype_s8;
typedef npy_uint16 npyv_lanetype_u16;
typedef npy_int16  npyv_lanetype_s16;
typedef npy_uint32 npyv_lanetype_u32;
typedef npy_int32  npyv_lanetype_s32;
typedef npy_uint64 npyv_lanetype_u64;
typedef npy_int64  npyv_lanetype_s64;
typedef float      npyv_lanetype_f32;
typedef double     npyv_lanetype_f64;

#ifndef NPY_SIMD
    #define NPY_SIMD 0
    #define NPY_SIMD_WIDTH 0
    #define NPY_SIMD_F64 0
#endif

#ifdef __cplusplus
}
#endif
#endif // _NPY_SIMD_H_
