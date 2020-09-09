typedef __m256i v_s32;

#define v_load_si128(a) _mm_loadu_si256((const __m256i*)(a))
#define v_store_si128(a,b) _mm_storeu_si256((__m256i)*(a),b)
#define v_add_s32(a,b) _mm256_add_epi32(a,b)