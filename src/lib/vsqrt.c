#include<math.h>
#include "../simd/simd.h"

//高精度向量化
void usimd_sqrt(float *a, int len)
{
    int i=0;
#if NPY_SIMD
#ifdef NPY_HAVE_NEON
	int vsteps = npyv_nlanes_f32;
	int n = len & -vsteps;
	for (; i < n; i+=vsteps) {
		npyv_f32 v1 = npyv_load_f32(a+i);
		float32x4_t rsqrte = vrsqrteq_f32(a);
		rsqrte = vmulq_f32(rsqrte, vrsqrtsq_f32(a, vmulq_f32(rsqrte, rsqrte)));
		rsqrte = vmulq_f32(rsqrte, vrsqrtsq_f32(a, vmulq_f32(rsqrte, rsqrte)));
		vst1q_f32(a+i, vmulq_f32(a, rsqrte));
	}
#endif
#endif
	for(; i < len; i++){
		a[i] = sqrt(a[i]);
	}
}
/*
* 快速平方根倒数算法也称为平方根倒数速算法(Fast Inverse Square Root)是用于快速计算 的一种算法。
* 此算法由于出现在《雷神之锤III竞技场》源代码中而被人们所熟知。此算法最早被认为是由约翰·卡马克
* 所发明，但后来的调查显示，该算法在这之前就于计算机图形学的硬件与软件领域有所应用，此算法至今
* 为止仍未能确切知晓算法中所使用的特殊常数的起源。
*/
float Q_sqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking（对浮点数的邪恶位元hack）
	i  = 0x5f3759df - ( i >> 1 );
    // i  = 0x5f375a86 - ( i >> 1 );               // what the fuck?（这他妈的是怎么回事？）
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration （第一次迭代）
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed（第二次迭代，可以删除）
	return number * y;
}

double InvSqrt(double x) {
    double xhalf = 0.5 * x;
    long long i = *(long long *)&x;
    i = 0x5fe6ec85e7de30da - (i >> 1);
    x = *(double *)&i;
    x = x * (1.5 - xhalf * x * x);
    x = x * (1.5 - xhalf * x * x);
    x = x * (1.5 - xhalf * x * x);
    return x;
}

double carMockSqrt(double x){
    return x * InvSqrt(x);
}

/*「袖珍计算器算法」是一种用指数函数 exp 和对数函数 ln 代替平方根函数的方法。
我们通过有限的可以使用的数学函数，得到我们想要计算的结果。
*/
int sqrt2(int x) {
	if (x == 0) {
		return 0;
	}
	int ans = exp(0.5 * log(x));
	return ((long long)(ans + 1) * (ans + 1) <= x ? ans + 1 : ans);
}
/*
由于 xx 平方根的整数部分 ans 是满足 k^2 <= x的最大 k 值，因此我们可以对 kk 进行二分查找
从而得到答案。二分查找的下界为 00，上界可以粗略地设定为 xx。在二分查找的每一步中
我们只需要比较中间元素 mid 的平方与 xx 的大小关系
并通过比较的结果调整上下界的范围。由于我们所有的运算都是整数运算，不会存在误差
因此在得到最终的答案 ans 后，也就不需要再去尝试 ans+1 了。
*/
int sqrt3(int x) {
	int l = 0, r = x, ans = -1;
	while (l <= r) {
		int mid = l + (r - l) / 2;
		if ((long long)mid * mid <= x) {
			ans = mid;
			l = mid + 1;
		} else {
			r = mid - 1;
		}
	}
	return ans;
}