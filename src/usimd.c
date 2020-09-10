#include "usimd.h"
#include "simd/intrin.h"
void dispatch() {
    /* Initialize CPU features */
    v_cpu_init();

    #ifdef V_HAVE_AVX512F
        #include "simd/intrin_avx512.h"
    #elif defined(V_HAVE_AVX2)
        printf("include AVX2");
        #include "simd/intrin_avx.h"
    #elif defined(V_HAVE_SSE2)
        #include "simd/intrin_sse.h"
    #endif
    printf("include nothing");
    #if defined(V_HAVE_VSX2) && defined(__LITTLE_ENDIAN__)
        #include "simd/intrin_vsx.h"
    #endif

    #ifdef V_HAVE_NEON
        #include "simd/intrin_neon.h"
    #endif
}

