#include "cpuinfo.h"
#include "cpuinfo_utils.h"

void v_cpu_init() {
  #if defined(__x86_64__) || defined(__amd64__) || defined(__x86_64) || defined(_M_AMD64) || defined( __i386__ ) || defined(i386) || defined(_M_IX86)
    v_init_features();
    printf("feature inited\n");
    if(v_cpu_have[30] != 0) {
      printf("AVX512F\n");
      #define V_HAVE_AVX512F
    } else if(v_cpu_have[14] != 0) {
      printf("AVX2\n");
      #define V_HAVE_AVX2 1
    } else if(v_cpu_have[3] != 0) {
      printf("SSE2\n");
      #define V_HAVE_SSE2
    }
  #elif defined(__arm__) || defined(__arch64__)
    v_init_features();
    if(v_cpu_have[3] != 0) {
      #define V_HAVE_NEON
    }
  #endif
}