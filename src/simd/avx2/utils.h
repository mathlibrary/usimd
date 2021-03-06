#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX2_UTILS_H
#define _V_SIMD_AVX2_UTILS_H

#define v256_shuffle_odd(A)    _mm256_permute4x64_epi64(A, _MM_SHUFFLE(3, 1, 2, 0))
#define v256_shuffle_odd_ps(A) _mm256_castsi256_ps(v256_shuffle_odd(_mm256_castps_si256(A)))
#define v256_shuffle_odd_pd(A) _mm256_permute4x64_pd(A, _MM_SHUFFLE(3, 1, 2, 0))

V_FINLINE __m256i v256_mul_u8(__m256i a, __m256i b)
{
    const __m256i mask = _mm256_set1_epi32(0xFF00FF00);
    __m256i even = _mm256_mullo_epi16(a, b);
    __m256i odd  = _mm256_mullo_epi16(_mm256_srai_epi16(a, 8), _mm256_srai_epi16(b, 8));
            odd  = _mm256_slli_epi16(odd, 8);
    return _mm256_blendv_epi8(even, odd, mask);
}

#endif // _V_SIMD_AVX2_UTILS_H
