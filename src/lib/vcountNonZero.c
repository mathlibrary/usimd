#include "common.h"
#include "../simd/simd.h"

#ifndef MIN
#define MIN(a, b) ((a < b) ? (a) : (b))
#endif

int usimd_countNonZero(uchar *d, size_t n)
{
    int count = 0;
    int i = 0;
#if V_SIMD
    const int vstep = v_nlanes_u8;
    const int unrollx = n & -vstep;
    const v_u8 vone = v_setall_u8(1);
    const v_u8 vzero = v_setall_u8(0);
    v_u8 vt;
    v_u8 vsum32 = v_zero_u32();
    while (i < unrollx)
    {
        v_u16 vsum16 = v_zero_u16();
        int j = i;
        while (j < MIN(unrollx, i + 65280 * v_nlanes_u16))
        {
            int k = j;
            v_u8 vsum8 = v_zero_u8();
            for (; k < MIN(unrollx, j + 255 * vstep); k += vstep)
            {
                vt = v_cmpeq_u8(v_load_u8(d + k), vzero);
                vt = v_and_u8(vt, vone);
                vsum8 = v_add_u8(vsum8, vt);
            }
            v_u16x2 part = v_expand_u8_u16(vsum8);
            vsum16 = v_add_u16(vsum16, v_add_u16(part.val[0], part.val[1]));
            j = k;
        }
        v_u16x2 part = v_expand_u16_u32(vsum16);
        vsum32 = v_add_u32(vsum32, v_add_u32(part.val[0], part.val[1]));
        i = j;
    }
    count = i - v_sum_u32(vsum32);
#endif
    for (; i < n; i++)
    {
        count += d[i] != 0;
    }
    return count;
}