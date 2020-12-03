#include "common.h"
#include "../simd/simd.h"
/*
Ref: https://arxiv.org/abs/1611.07612
*/
uint64_t c1 = 0x5555555555555555ULL;
uint64_t c2 = 0x3333333333333333ULL;
uint64_t c4 = 0x0F0F0F0F0F0F0F0FULL;
uint64_t c8 = 0x00FF00FF00FF00FFULL;
uint64_t c16 = 0x0000FFFF0000FFFFULL;
uint64_t c32 = 0x00000000FFFFFFFFULL;
// 727.16 MFlops   0.184577 sec
int popcnt_native(uint64_t *d, size_t size)
{
    int sum = 0;
    int count = 0;
    uint64_t x;
    for (int i = 0; i < size; i++)
    {
        count = 0;
        x = d[i];
        for (int j = 0; j < 64; j++)
        {
            count += (x >> j) & 1;
        }
        sum += count;
    }
    return sum;
}
// 690.60 MFlops   0.194350 sec
int popcnt_treeadd(uint64_t *d, size_t size)
{
    int sum = 0;
    uint64_t x;
    for (int i = 0; i < size; i++)
    {
        x = d[i];
        x = (x & c1) + ((x >> 1) & c1);
        x = (x & c2) + ((x >> 2) & c2);
        x = (x & c4) + ((x >> 4) & c4);
        x = (x & c8) + ((x >> 8) & c8);
        x = (x & c16) + ((x >> 16) & c16);
        sum += (x & c32) + ((x >> 32) & c32);
    }
    return sum;
}

int popcnt_Wilkes_Wheeler_Gill(uint64_t x)
{
    x -= (x >> 1) & c1;
    x = ((x >> 2) & c2) + (x & c2);
    x = (x + (x >> 4)) & c4;
    x *= 0x0101010101010101ULL;
    return x >> 56;
}

//683.75 MFlops   0.196296 sec
int popcnt_WWG(uint64_t *d, size_t size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += popcnt_Wilkes_Wheeler_Gill(d[i]);
    }
    return sum;
}
/*
 The downside of the Wegner approach for modern processors is that
the unpredictable loop termination adds a mispredicted
branch penalty of at least 10 cycles [21], which for
short loops can be more expensive than the operations
performed by the loop. 
734.91 MFlops   0.182632 sec
*/
int popcnt_Wegner(uint64_t *d, size_t size)
{
    int sum = 0;
    uint64_t x;
    for (int i = 0; i < size; i++)
    {
        x = d[i];
        int v = 0;
        while (x != 0)
        {
            x &= x - 1;
            v++;
        }
        sum += v;
    }
    return sum;
}

void CSA(uint64_t *h, uint64_t *l,
         uint64_t a, uint64_t b, uint64_t c)
{
    uint64_t u = a ^ b;
    *h = (a & b) | (u & c);
    *l = u ^ c;
}
// 670.39 MFlops   0.200207 sec
uint64_t harley_seal(uint64_t *d,
                     size_t size)
{
    uint64_t total = 0, ones = 0, twos = 0,
             fours = 0, eights = 0, sixteens = 0;
    uint64_t twosA, twosB, foursA, foursB, eightsA,
        eightsB;
    for (size_t i = 0; i < size - size % 16;
         i += 16)
    {
        CSA(&twosA, &ones, ones, d[i + 0], d[i + 1]);
        CSA(&twosB, &ones, ones, d[i + 2], d[i + 3]);
        CSA(&foursA, &twos, twos, twosA, twosB);
        CSA(&twosA, &ones, ones, d[i + 4], d[i + 5]);
        CSA(&twosB, &ones, ones, d[i + 6], d[i + 7]);
        CSA(&foursB, &twos, twos, twosA, twosB);
        CSA(&eightsA, &fours, fours, foursA, foursB);
        CSA(&twosA, &ones, ones, d[i + 8], d[i + 9]);
        CSA(&twosB, &ones, ones, d[i + 10], d[i + 11]);
        CSA(&foursA, &twos, twos, twosA, twosB);
        CSA(&twosA, &ones, ones, d[i + 12], d[i + 13]);
        CSA(&twosB, &ones, ones, d[i + 14], d[i + 15]);
        CSA(&foursB, &twos, twos, twosA, twosB);
        CSA(&eightsB, &fours, fours, foursA,
            foursB);
        CSA(&sixteens, &eights, eights, eightsA,
            eightsB);
        total += popcnt_Wilkes_Wheeler_Gill(sixteens);
    }
    total = 16 * total + 8 * popcnt_Wilkes_Wheeler_Gill(eights) + 4 * popcnt_Wilkes_Wheeler_Gill(fours) + 2 * popcnt_Wilkes_Wheeler_Gill(twos) + popcnt_Wilkes_Wheeler_Gill(ones);
    for (size_t i = size - size % 16; i < size; i++)
        total += popcnt_Wilkes_Wheeler_Gill(d[i]);
    return total;
}
#if V_SIMD && defined(V_HAVE_SSSE3)
void mula_bytes_128(uint64_t *data)
{
    __m128i v = _mm_load_si128(data);
    __m128i lookup = _mm_setr_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4);
    __m128i low_mask = _mm_set1_epi8(0x0f);
    __m128i lo = _mm_and_si128(v, low_mask);
    __m128i hi = _mm_and_si128(
        _mm_srli_epi16(v, 4), low_mask);
    __m128i cnt1 =
        _mm_shuffle_epi8(lookup, lo);
    __m128i cnt2 =
        _mm_shuffle_epi8(lookup, hi);
    _mm_store_si128(data, _mm_add_epi8(cnt1, cnt2));
}
#endif

#if V_SIMD > 128
__m256i mula_bytes_256(__m256i v)
{
    __m256i lookup =
        _mm256_setr_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2,
                         2, 3, 2, 3, 3, 4, 0, 1, 1, 2, 1, 2, 2, 3,
                         1, 2, 2, 3, 2, 3, 3, 4);
    __m256i low_mask = _mm256_set1_epi8(0x0f);
    __m256i lo = _mm256_and_si256(v, low_mask);
    __m256i hi = _mm256_and_si256(_mm256_srli_epi32(v, 4), low_mask);
    __m256i popcnt1 = _mm256_shuffle_epi8(lookup,
                                          lo);
    __m256i popcnt2 = _mm256_shuffle_epi8(lookup,
                                          hi);
    __m256i total = _mm256_add_epi8(popcnt1, popcnt2);
    total = _mm256_sad_epu8(total,
                            _mm256_setzero_si256());
    return total;
}

void CSA_256(__m256i *h, __m256i *l, __m256i a,
             __m256i b, __m256i c)
{
    __m256i u = _mm256_xor_si256(a, b);
    *h = _mm256_or_si256(_mm256_and_si256(a, b),
                         _mm256_and_si256(u, c));
    *l = _mm256_xor_si256(u, c);
}

uint64_t avx_hs(__m256i *d, uint64_t size)
{
    __m256i total = _mm256_setzero_si256();
    __m256i ones = _mm256_setzero_si256();
    __m256i twos = _mm256_setzero_si256();
    __m256i fours = _mm256_setzero_si256();
    __m256i eights = _mm256_setzero_si256();
    __m256i sixteens = _mm256_setzero_si256();
    __m256i twosA, twosB, foursA, foursB,
        eightsA, eightsB;
    for (uint64_t i = 0; i < size; i += 16)
    {
        CSA_256(&twosA, &ones, ones, d[i], d[i + 1]);
        CSA_256(&twosB, &ones, ones, d[i + 2], d[i + 3]);
        CSA_256(&foursA, &twos, twos, twosA, twosB);
        CSA_256(&twosA, &ones, ones, d[i + 4], d[i + 5]);
        CSA_256(&twosB, &ones, ones, d[i + 6], d[i + 7]);
        CSA_256(&foursB, &twos, twos, twosA, twosB);
        CSA_256(&eightsA, &fours, fours, foursA, foursB);
        CSA_256(&twosA, &ones, ones, d[i + 8], d[i + 9]);
        CSA_256(&twosB, &ones, ones, d[i + 10], d[i + 11]);
        CSA_256(&foursA, &twos, twos, twosA, twosB);
        CSA_256(&twosA, &ones, ones, d[i + 12], d[i + 13]);
        CSA_256(&twosB, &ones, ones, d[i + 14], d[i + 15]);
        CSA_256(&foursB, &twos, twos, twosA, twosB);
        CSA_256(&eightsB, &fours, fours, foursA, foursB);
        CSA_256(&sixteens, &eights, eights, eightsA,
                eightsB);
        total = _mm256_add_epi64(total, mula_bytes_256(sixteens));
    }
    total = _mm256_slli_epi64(total, 4);
    total = _mm256_add_epi64(total,
                             _mm256_slli_epi64(mula_bytes_256(eights), 3));
    total = _mm256_add_epi64(total,
                             _mm256_slli_epi64(mula_bytes_256(fours), 2));
    total = _mm256_add_epi64(total,
                             _mm256_slli_epi64(mula_bytes_256(twos), 1));
    total = _mm256_add_epi64(total, mula_bytes_256(ones));
    return _mm256_extract_epi64(total, 0) + _mm256_extract_epi64(total, 1) + _mm256_extract_epi64(total, 2) + _mm256_extract_epi64(total, 3);
}
#endif