#ifndef _COMMON_H
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char uchar;
#define _COMMON_H
#ifdef DOUBLE_T
    #define FLOAT_T double
#else
    #define FLOAT_T float
#endif

#define VEC_LT256 (!defined(DOUBLE_T) || (defined(DOUBLE_T) && V_SIMD_F64 && V_SIMD > 128))
#endif