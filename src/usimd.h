#ifndef USIMD_H_
#define USIMD_H_
#include "cpuinfo/cpuinfo.h"

#if defined(NPY_HAVE_AVX512F) && !defined(NPY_SIMD_FORCE_256) && !defined(NPY_SIMD_FORCE_128)
    #include "simd/intrin_avx512.h"
#elif defined(NPY_HAVE_AVX2) && !defined(NPY_SIMD_FORCE_128)
    #include "simd/intrin_avx.h"
#elif defined(NPY_HAVE_SSE2)
    #include "simd/intrin_sse.h"
#endif

#if defined(NPY_HAVE_VSX2) && defined(__LITTLE_ENDIAN__)
    #include "simd/intrin_vsx.h"
#endif

#ifdef NPY_HAVE_NEON
    #include "simd/intrin_neon.h"
#endif

#endif
