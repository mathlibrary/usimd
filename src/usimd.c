#include "usimd.h"

void dispatch() {
    /* Initialize CPU features */
    v_cpu_init();

    #if defined(V_HAVE_AVX512F)
        #include "simd/intrin_avx512.h"
    #elif defined(V_HAVE_AVX2)
        #include "simd/intrin_avx.h"
    #elif defined(V_HAVE_SSE2)
        #include "simd/intrin_sse.h"
    #endif

    #if defined(V_HAVE_VSX2) && defined(__LITTLE_ENDIAN__)
        #include "simd/intrin_vsx.h"
    #endif

    #ifdef NPY_HAVE_NEON
        #include "simd/intrin_neon.h"
    #endif
}

