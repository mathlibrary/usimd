#define V_HAVE_SSE2
#include "common.h"
#include "../simd/simd.h"

void usimd_threshold(char *src, char *dst,  int len, char thresh, char maxval )
{
    int i = 0;
#if V_SIMD
    const int vstep = v_nlanes_u8;
    const int unrollx2 = len & -vstep*2;
    v_u8 vthreshold = v_setall_u8(thresh);
    v_u8 vvalue = v_setall_u8(maxval);
    for(int i = 0; i < unrollx2; i += vstep*2 )
    {
        __builtin_prefetch(src + i + vstep*20);
        v_u8 v0 = v_load_u8(src + i);
        v_u8 v1 = v_load_u8(src + i + 16);
        v_u8 r0 = v_cmpge_u8(v0, vthreshold);
        v_u8 r1 = v_cmpge_u8(v1, vthreshold);
        v_u8 r0a = v_and_u8(r0, vvalue);
        v_u8 r1a = v_and_u8(r1, vvalue);
        v_store_u8(dst + i, r0a);
        v_store_u8(dst + i + vstep, r1a);
    }
#endif
    for(; i < len; i++) {
        dst[i] = src[i] <= thresh ? 0 : maxval;
    }
}

int main() {
    char src[10] = {1,2,3,4,5,6,7,8,9,10};
    char dst[10];
    usimd_threshold(src,dst,10,5,50);
    puts("");
}