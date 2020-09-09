#include "cpuinfo.h"


void v_cpu_init() {
  #if defined(__x86_64__) || defined(__amd64__) || defined(__x86_64) || defined(_M_AMD64) || defined( __i386__ ) || defined(i386) || defined(_M_IX86)
    v_x86_init_features();
    #if(v_x86_cpu_have[30]) #define V_HAVE_AVX512F
    #elif(v_x86_cpu_have[14]) #define V_HAVE_AVX2
    #elif(v_x86_cpu_have[3]) #define V_HAVE_SSE2
    #endif
  #elif defined(__arm__) || defined(__arch64__)
    v_arm_init_features();
    if(v_arm_cpu_have[3]) #define V_HAVE_NEON
  #endif
}