#include "common.h"
#include "../simd/simd.h"

void usimd_addindex(int n, FLOAT_T *x)
{
    int i = 0;
#if V_SIMD
#if defined(DOUBLE_T)
    const int vstep = v_nlanes_f64;
    const int unrollx = n & -vstep;
    v_f64 x_vec, index;
    for (; i < unrollx; i += vstep)
    {
        x_vec = v_load_f64(x + i);
        index = v_set_f64(i, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7,
                          i + 8, i + 9, i + 10, i + 11, i + 12, i + 13, i + 14, i + 15);
        x_vec = v_add_f64(x_vec, index);
        v_store_f64(x + i, x_vec);
    }
#else
    const int vstep = v_nlanes_f32;
    const int unrollx = n & -vstep;
    v_f32 x_vec, index;
    for (; i < unrollx; i += vstep)
    {
        x_vec = v_load_f32(x + i);
        index = v_set_f32(i, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7,
                          i + 8, i + 9, i + 10, i + 11, i + 12, i + 13, i + 14, i + 15);
        x_vec = v_add_f32(x_vec, index);
        v_store_f32(x + i, x_vec);
    }
#endif
#else
    int n1 = n & -4;
    for (; i < n1; i += 4)
    {
        x[i] = x[i] + i;
        x[i + 1] = x[i + 1] + i + 1;
        x[i + 2] = x[i + 2] + i + 2;
        x[i + 3] = x[i + 3] + i + 3;
    }
#endif
    while (i < n)
    {
        x[i] = x[i] + i;
        i++;
    }
}
