#include<stdio.h>
#include "../src/usimd.h"
void usimd_add(const float *a, const float *b, float *c, int len)
{
#if NPY_SIMD
	for (int i=0; i < len/4; i++) {
		v_s32 v1 = v_load_f32(a+i*4);
		v_s32 v2 = v_load_f32(b+i*4);
		v_s32 v3 = v_add_f32(v1, v2);
		v_store_f32(c+i*4, v3);
	}
	return;
#endif
	for(int i=0; i < len; i++){
		c[i] = a[i] + b[i];
	}
}

int main()
{
	dispatch();
	int a[8] = {1,2,3,4,5,6,7,8};
	int b[8] = {1,2,3,4,5,6,7,8};
	int c[8];
	usimd_add(a, b, c, 8);
	for(int i=0;i<8;i++) {
		printf("%d ", c[i]);
	}
	return 0;
}
