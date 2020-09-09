
#define NPY_SIMD 128
#ifdef __aarch64__
    #define NPY_SIMD_F64 1
#else
    #define NPY_SIMD_F64 0
#endif

typedef int32x4_t v_s32;

#define v_load_si128(a) vld1q_s32((const int32_t*)a)
#define v_store_si128(a,b) vst1q_s32((int32_t*)a, b)
#define v_add_s32(a,b) vaddq_s32(a,b)
