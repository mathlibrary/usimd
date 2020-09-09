typedef __m512i v_s32;

#define v_load_si128(a) _mm_loadu_si512((const __m512i*)(a))
#define v_store_si128(a,b) _mm_storeu_si512((__m512i)*(a),b)
#define v_add_s32(a,b) _mm512_add_epi32(a,b)