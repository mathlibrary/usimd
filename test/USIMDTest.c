#include "./cutest/CuTest.h"
#include "../src/lib/common.h"
#include "../bench/bench.h"

#include "../src/lib/vadd.c"
#include "../src/lib/vmuladd.c"
#include "../src/lib/vcmul.c"
#include "../src/lib/vsum.c"
#include "../src/lib/vdot.c"
#include "../src/lib/vdaxpy.c"
#include "../src/lib/vaddindex.c"
#include "../src/lib/vaddeven.c"
#include "../src/lib/vfcond.c"
#include "../src/lib/varrmax.c"
#include "../src/lib/vmatrixmul.c"
#include "../src/lib/vpopcnt.c"
#include "../src/lib/vcountNonZero.c"
#include "../src/lib/vsort.c"

void TestVadd(CuTest *tc)
{
	FLOAT_T input1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	FLOAT_T input2[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	FLOAT_T actual[8];
	FLOAT_T expected[8] = {2, 4, 6, 8, 10, 12, 14, 16};
	usimd_add(input1, input2, actual, 8);
	for (int i = 0; i < 8; i++)
	{
		CuAssertDblEquals(tc, expected[i], actual[i], 1e-6);
	}
}

void TestVsum(CuTest *tc)
{
	FLOAT_T input[100];
	for (int i = 0; i < 100; i++)
	{
		input[i] = i + 1;
	}
	double actual = usimd_sum(100, input, 1);
	double expected = 5050;
	CuAssertDblEquals(tc, expected, actual, 1e-6);
}

void TestVdot(CuTest *tc)
{
	FLOAT_T input1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	FLOAT_T input2[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	FLOAT_T actual = usimd_dot(8, input1, 1, input2, 1);
	FLOAT_T expected = 204;
	CuAssertDblEquals(tc, expected, actual, 1e-6);
}

void TestVmuladd(CuTest *tc)
{
	FLOAT_T input1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	FLOAT_T input2[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	FLOAT_T input3[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	usimd_muladd(input1, input2, input3, 8);
	FLOAT_T expected[] = {2, 6, 12, 20, 30, 42, 56, 72};
	for (int i = 0; i < 8; i++)
	{
		CuAssertDblEquals(tc, expected[i], input3[i], 1e-6);
	}
}

void TestVcmul(CuTest *tc)
{
	FLOAT_T input1[18] = {1, -2, -3, 4, -5, -6, -7, 8, 2, 9, 5, 4, 6, 6, 10, 20, 20, 10};
	FLOAT_T input2[18] = {1, 2, 3, 4, 5, 6, 7, 8, 3, 5, 4, 5, 7, -7, -10, 20, -20, 10};
	FLOAT_T output[18];
	usimd_cmul(input1, input2, output, 9);
	FLOAT_T expected[] = {5, 0, -25, 0, 11, -60, -113, 0, -39, 37, 0, 41, 84, 0, -500, 0, -500, 0};
	for (int i = 0; i < 18; i++)
	{
		CuAssertDblEquals(tc, expected[i], output[i], 1e-6);
	}
}

void TestVdaxpy(CuTest *tc)
{
	FLOAT_T input1[16] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
	FLOAT_T input2[16] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
	FLOAT_T alpha = 2;
	usimd_daxpy(16, input1, input2, &alpha);
	FLOAT_T expected[16] = {3, 6, 9, 12, 15, 18, 21, 24, 3, 6, 9, 12, 15, 18, 21, 24};
	for (int i = 0; i < 16; i++)
	{
		CuAssertDblEquals(tc, expected[i], input2[i], 1e-6);
	}
}

void TestVaddindex(CuTest *tc)
{
	FLOAT_T input[105];
	FLOAT_T output[105];
	for (int i = 0; i < 105; i++)
	{
		input[i] = i + 1;
		output[i] = input[i] + i;
	}
	usimd_addindex(105, input);
	for (int i = 0; i < 105; i++)
	{
		CuAssertDblEquals(tc, output[i], input[i], 1e-6);
	}
}

void TestVaddeven(CuTest *tc)
{
	FLOAT_T input[110];
	FLOAT_T output[55];
	FLOAT_T expected[55];
	for (int i = 0; i < 110; i++)
	{
		input[i] = i + 1;
	}
	for (int i = 0; i < 55; i++)
	{
		expected[i] = (input[2 * i] + input[2 * i + 1]) / 2;
	}
	usimd_addeven(input, output, 110);
	for (int i = 0; i < 55; i++)
	{
		CuAssertDblEquals(tc, expected[i], output[i], 1e-6);
	}
}

void TestVfcond(CuTest *tc)
{
	FLOAT_T input[20] = {1, 0.1, 2, 0.2, 0.3, 4, 0, 4, 0, 5, 1, 0.1, 2, 0.2, 0.3, 4, 0, 4, 0, 5};
	FLOAT_T expected[20] = {2, -0.9, 3, -0.8, -0.7, 5, -1, 5, -1, 6, 2, -0.9, 3, -0.8, -0.7, 5, -1, 5, -1, 6};
	usimd_fcond(input, 20);
	for (int i = 0; i < 20; i++)
	{
		CuAssertDblEquals(tc, expected[i], input[i], 1e-6);
	}
}

void TestVarrmax(CuTest *tc)
{
	FLOAT_T input[20] = {1, 3, 4, 1, 34, 651, 74, 23, 650, 23, 1, 3, 4, 1, 34, 652, 74, 23, 650, 23};
	FLOAT_T expected = 652;
	CuAssertDblEquals(tc, expected, usimd_arrmax(input, 20), 1e-6);
}

void TestVmatrixmul(CuTest *tc)
{
	FLOAT_T matrix_a[5][5] = {
		{1, 2, 3, 4, 5},
		{1, 2, 3, 4, 5},
		{1, 2, 3, 4, 5},
		{1, 2, 3, 4, 5},
		{1, 2, 3, 4, 5}};
	FLOAT_T matrix_b[5][5] = {
		{1, 2, 3, 4, 5},
		{1, 2, 3, 4, 5},
		{1, 2, 3, 4, 5},
		{1, 2, 3, 4, 5},
		{1, 2, 3, 4, 5}};
	FLOAT_T result_matrix[5][5];
	FLOAT_T expected[5][5] = {
		{15.000000, 30.000000, 45.000000, 60.000000, 75.000000},
		{15.000000, 30.000000, 45.000000, 60.000000, 75.000000},
		{15.000000, 30.000000, 45.000000, 60.000000, 75.000000},
		{15.000000, 30.000000, 45.000000, 60.000000, 75.000000},
		{15.000000, 30.000000, 45.000000, 60.000000, 75.000000}};
	FLOAT_T *p[5], *q[5], *r[5];
	FLOAT_T tmp;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			// init result
			result_matrix[i][j] = 0;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = i + 1; j < 5; j++)
		{
			//transpose matrix_b
			tmp = matrix_b[j][i];
			matrix_b[j][i] = matrix_b[i][j];
			matrix_b[i][j] = tmp;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		p[i] = matrix_a[i];
		q[i] = matrix_b[i];
		r[i] = result_matrix[i];
	}

	usimd_matrixmul_t_simd(p, q, r, 5, 5, 5);
	// check
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			//printf("%f ",result_matrix[i][j]);
			CuAssertDblEquals(tc, expected[i][j], result_matrix[i][j], 1e-6);
		}
	}
}

void TestVpopcnt(CuTest *tc)
{
	uint64_t data[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	uint64_t result[16] = {1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1};
	int scale = 16;
	int sum = 33;

	CuAssertIntEquals(tc, sum, popcnt_native(data, scale));
	CuAssertIntEquals(tc, sum, popcnt_treeadd(data, scale));
	CuAssertIntEquals(tc, sum, popcnt_Wegner(data, scale));
	CuAssertIntEquals(tc, sum, popcnt_WWG(data, scale));
	CuAssertIntEquals(tc, sum, harley_seal(data, scale));
}

void TestVcountnonzero(CuTest *tc)
{
	uchar data[80] = {
    1, 0, 3, 4, 0, 6, 7, 8, 9, 0,1, 0, 3, 4, 0, 6, 7, 8, 9, 0,
    1, 0, 3, 4, 0, 6, 7, 8, 9, 0,1, 0, 3, 4, 0, 6, 7, 8, 9, 0,
    1, 0, 3, 4, 0, 6, 7, 8, 9, 0,1, 0, 3, 4, 0, 6, 7, 8, 9, 0,
    1, 0, 3, 4, 0, 6, 7, 8, 9, 0,1, 1, 3, 4, 0, 6, 7, 8, 9, 0};
	int scale = 80;
	CuAssertIntEquals(tc, 57, usimd_countNonZero(data, scale));
}

void TestVsort(CuTest *tc)
{
	int scale = 100;
    uint32_t *arr = getInt32Arr(scale);
	uint32_t *output1 = getInt32Arr(scale);
	uint32_t *output2 = getInt32Arr(scale);
	int count = scale;
	for(int i=0;i<scale;i++) {
		arr[i] = count--;
	}
	memcpy(output1,arr,sizeof(uint32_t)*scale);
	qsort(output1,scale,sizeof(uint32_t),comp);
	memcpy(output2,arr,sizeof(uint32_t)*scale);
    usimd_quicksort(output2, 0, scale-1);
	for(int i=0;i<scale;i++) {
		CuAssertDblEquals(tc, output1[i], output1[i], 1e-6);
	}
}

CuSuite *USIMDGetSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestVadd);
	SUITE_ADD_TEST(suite, TestVmuladd);
	SUITE_ADD_TEST(suite, TestVcmul);
	SUITE_ADD_TEST(suite, TestVsum);
	SUITE_ADD_TEST(suite, TestVdot);
	SUITE_ADD_TEST(suite, TestVdaxpy);
	SUITE_ADD_TEST(suite, TestVaddindex);
	SUITE_ADD_TEST(suite, TestVaddeven);
	SUITE_ADD_TEST(suite, TestVfcond);
	SUITE_ADD_TEST(suite, TestVarrmax);
	SUITE_ADD_TEST(suite, TestVmatrixmul);
	SUITE_ADD_TEST(suite, TestVpopcnt);
	SUITE_ADD_TEST(suite, TestVcountnonzero);
	SUITE_ADD_TEST(suite, TestVsort);
	return suite;
}