#include<stdio.h>
#include "../src/usimd.h"

void usimd_add(const int *a, const int *b, int *c, int len)
{
#if NPY_SIMD
	for (int i=0; i < len/4; i++) {
		v_s8 v1 = v_load_si128(a+i*4);
		v_s8 v2 = v_load_si128(b+i*4);
		v_s8 v3 = v_add_s32(v1, v2);
		v_store_si128(c+i*4, v3);
	}
	return;
#endif
	for(int i=0; i < len; i++){
		c[i] = a[i] + b[i];
	}
}

int main(void)
{
    int a[8] = {1,2,3,4,5,6,7,8};
	int b[8] = {1,2,3,4,5,6,7,8};
	int c[8];
	usimd_add(a, b, c, 8);
	for(int i=0;i<8;i++) {
		print("%d ", c[i]);
	}
}
