#ifndef _COMMON_H
#define _COMMON_H
#ifdef DOUBLE_T
    #define FLOAT_T double
#else
    #define FLOAT_T float
#endif

#define VEC_LT256 (!defined(DOUBLE_T) || (defined(DOUBLE_T) && V_SIMD_F64 && V_SIMD > 128))
#endif