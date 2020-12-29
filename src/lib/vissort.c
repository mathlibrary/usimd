#define V_HAVE_SSE2
#define V_HAVE_SSSE3
#define V_HAVE_SSE4_1
#define V_HAVE_AVX
#define V_HAVE_AVX2
#include "common.h"
#include "../simd/simd.h"
// Source Code: http://0x80.pl/notesen/2018-04-11-simd-is-sorted.html
int is_sorted(const int32_t *input, size_t n)
{
    if (n < 2)
    {
        return 1;
    }

    for (size_t i = 0; i < n - 1; i++)
    {
        if (input[i] > input[i + 1])
            return 0;
    }

    return 1;
}
/*
The simplest vectorized solution is suitable for all SIMD flavours. In a loop two vectors are filled;
one vector contains items a = input[0 .. k - 1], another b = input[1 .. k] (where k is the vector size;
for SSE k=4, for AVX2 k=8 and for AVX512 k=16).
Then the comparison for greater yields a vector a[i] > b[i]. If all its elements are zero (false) it
means that in the range 0 .. k-1 the relation is not violated.
Below is the outline of the algorithm's loop (with k=4):

Load the first vector:

curr = [    a0   |    a1   |    a2   |    a3   ]
Load the second vector:

next = [    a1   |    a2   |    a3   |    a4   ]
Compare curr > next:

mask = [ a0 > a1 | a1 > a2 | a2 > a3 | a3 > a4 ]
If any element of mask is not zero, then return false.

Otherwise advance the input pointer by k and go back to 1.
*/
#if V_SIMD == 128 && defined(V_HAVE_SSSE3) && defined(V_HAVE_SSE4_1)
int is_sorted_sse(int32_t *a, size_t n)
{

    size_t i = 0;
    if (n > 4)
    {
        for (/**/; i < n - 4; i += 4)
        {
            const __m128i curr = _mm_loadu_si128(a + i);
            const __m128i next = _mm_loadu_si128(a + i + 1);

            const __m128i mask = _mm_cmpgt_epi32(curr, next);
            if (!_mm_test_all_zeros(mask, mask))
            {
                return 0;
            }
        }
    }

    for (/**/; i + 1 < n; i++)
    {
        if (a[i] > a[i + 1])
            return 0;
    }

    return 1;
}
/*
The generic approach has one issue. The vector next shares k-1 elements with curr, but we anyway read from the memory all k elements.

With help of the SSE instruction _mm_palignr_epi8 (palingr) the number of memory accesses can be reduced. The instruction gets two 16-byte vectors, joins them into a 32-byte temporary array and then copies the selected subarray into a 16-byte vector.

We keep two vectors (chunk0 and chunk1) containing as a whole an eight-element subarray of the input. In each iteration this subarray is shifted right by one element. In each iteration we read just one chunk, i.e. it done at cost of single memory load.

The algorithm works as follows:

Before the main loop load into vector chunk0 the first portion of input:
// chunk0 = [    a0   |    a1   |    a2   |    a3   ]
__m128i chunk0 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(array));
In the loop read the next portion of array into chunk1:
// chunk1 = [    a4   |    a5   |    a6   |    a7   ]
const __m128i chunk1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(array + i + 4));
Now build the vector curr — it is simply an alias for chunk0:
// curr   = [    a0   |    a1   |    a2   |    a3   ]
const __m128i curr = chunk0;
The vector next is build from the pair chunk1:chunk0 using _mm_alignr_epi8:
// tem    = [    a0   |    a1   |    a2   |    a3   |    a4   |    a5   |    a6   |    a7   ]
//          |              chunk0                   |                 chunk1                |

// next   = [    a1   |    a2   |    a3   |    a4   ]
const __m128i next = _mm_alignr_epi8(chunk1, chunk0, 4);
Finally the vectors are compared:
// mask   = [ a0 > a1 | a1 > a2 | a2 > a3 | a3 > a4 ]
const __m128i mask = _mm_cmpgt_epi32(curr, next);
If any element of mask is not zero, then return false.
Shift right the input view:
chunk0 = chunk1;
// chunk1 will be updated at the loop beginning
Advance the input pointer by 4 and go to 1.
*/
int is_sorted_sse_2(int32_t *a, size_t n)
{

    size_t i = 0;
    if (n >= 8)
    {
        __m128i chunk0 = _mm_loadu_si128(a);
        do
        {
            const __m128i chunk1 = _mm_loadu_si128(a + i + 4);
            const __m128i curr = chunk0;
            const __m128i next = _mm_alignr_epi8(chunk1, chunk0, 4);

            const __m128i mask = _mm_cmpgt_epi32(curr, next);
            if (!_mm_test_all_zeros(mask, mask))
            {
                return 0;
            }

            chunk0 = chunk1;
            i += 4;
        } while (i < n - 4);
    }

    for (/**/; i + 1 < n; i++)
    {
        if (a[i] > a[i + 1])
            return 0;
    }

    return 1;
}
#elif V_SIMD > 128
/*
Unfortunately the AVX2 version of _mm256_alignr_epi8 doesn't operate on the whole 32-byte register, but on its 16-byte halves (lanes).

The AVX2 approach uses instruction _mm256_permutevar8x32_epi32, which moves 32-bit elements across the lanes in the given order.

In a single iteration we read eight elements:

curr = [ a0 | a1 | a2 | a3 | a4 | a5 | a6 | a7 ]
Then the vector curr is shifted (permuted) by one element right, only the last item (a7) is kept on the same position:

next = [ a1 | a2 | a3 | a4 | a5 | a6 | a7 | a7 ]
The comparison efficiently tests the first seven elements:

mask = [ a0 > a1 | a1 > a2 | a2 > a3 | a3 > a4 | a4 > a5 | a5 > a6 | a6 > a7 | a7 > a7 ]
                                                                              always false
*/
int is_sorted_avx2(int32_t *a, size_t n)
{

    const __m256i shuffle_pattern = _mm256_setr_epi32(1, 2, 3, 4, 5, 6, 7, 7);

    size_t i = 0;
    while (i < n - 8)
    {
        // curr = [ a0 | a1 | a2 | a3 | a4 | a5 | a6 | a7 ]
        const __m256i curr = _mm256_loadu_si256(a + i);
        // next = [ a1 | a2 | a3 | a4 | a5 | a6 | a7 | a7 ]
        const __m256i next = _mm256_permutevar8x32_epi32(curr, shuffle_pattern);

        // Note: the last element of curr and next is a7, thus for this element
        //       the comparison result is always zero.
        //
        // In fact, the first 7 elements are being tested.
        const __m256i mask = _mm256_cmpgt_epi32(curr, next);
        if (!_mm256_testz_si256(mask, mask))
        {
            return 0;
        }

        i += 7;
    }

    for (/**/; i + 1 < n; i++)
    {
        if (a[i] > a[i + 1])
            return 0;
    }
    return 1;
}
#endif
#define int32_SWAP(a,b) {int32_t tmp = (b); (b)=(a); (a) = tmp;}
void swap(int32_t *p1, int32_t *p2)
{
    int32_SWAP(*p1, *p2);
    p1++;
}

int main()
{
    int scale = 16;
    int32_t unordered_input[16] = {8, 9, 1, 3, 4, 1, 4, 3, 8, 9, 1, 3, 4, 1, 4, 3};
    int32_t sorted_input[16];

    for (int i = 0; i < scale; i++)
    {
        sorted_input[i] = i;
    }
    int32_t *p = unordered_input;
    swap(p, p+1);
    
    printf("is sort? %d", is_sorted_avx2(sorted_input, scale));
    return 1;
}