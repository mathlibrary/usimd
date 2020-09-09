
#define NPY_SIMD 128
#define NPY_SIMD_F64 1

typedef _m128i v_s32;

#define v_load_si128(a) _mm_loadu_si128((const __m128i*)(a))
#define v_store_si128(a,b) _mm_storeu_si128((__m128i)*(a),b)
#define v_add_s32(a,b) _mm_add_epi32(a,b)

