#include "./cutest/CuTest.h"
#include "../src/lib/common.h"

#include "../src/lib/vadd.c"
#include "../src/lib/vmuladd.c"
#include"../src/lib/vpi.c"
#include"../src/lib/vsum.c"
#include"../src/lib/vdot.c"
#include"../src/lib/vsqrt.c"
#include "../src/lib/vdaxpy.c"

void TestVadd(CuTest *tc) {
	FLOAT_T input1[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T input2[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T actual[8];
	FLOAT_T expected[8] = {2,4,6,8,10,12,14,16};
	usimd_add(input1, input2, actual, 8);
	for(int i=0;i<8;i++) {
		CuAssertDblEquals(tc,actual[i], expected[i], 1e-6);
	}
}

void TestVPi(CuTest *tc) {
	int input = 1024*1024;
    double expected = compute_pi_native(input);
    double actual = compute_pi_omp_avx(input);
	CuAssertDblEquals(tc,actual, expected, 1e-6);
}

void TestVsum(CuTest *tc) {
	FLOAT_T input[16] = {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
    double actual = usimd_sum(16,input,1);
    double expected = 72;
	CuAssertDblEquals(tc,actual, expected, 1e-6);
}

void TestVdot(CuTest *tc) {
	FLOAT_T input1[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T input2[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T actual = usimd_dot(8,input1, 1, input2, 1);
	FLOAT_T expected = 204;
	CuAssertDblEquals(tc,actual, expected, 1e-6);
}

void TestVsqrt(CuTest *tc) {
	FLOAT_T input1[8] = {100,4,9,16,25,36,49,64};
	usimd_sqrt(input1, 8);
	FLOAT_T expected[] = {10,2,3,4,5,6,7,8};
	for(int i=0;i<8;i++) 
	{
		CuAssertDblEquals(tc,input1[i], expected[i], 1e-6);
	}
}

void TestVmuladd(CuTest *tc) {
	FLOAT_T input1[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T input2[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T input3[8] = {1,2,3,4,5,6,7,8};
	usimd_muladd(input1,input2,input3,8);
	FLOAT_T expected[] = {2,6,12,20,30,42,56,72};
	for(int i=0;i<8;i++) 
	{
		CuAssertDblEquals(tc,input3[i], expected[i], 1e-6);
	}
}

void TestVdaxpy(CuTest *tc) {
	FLOAT_T input1[16] = {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
	FLOAT_T input2[16] = {1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8};
	FLOAT_T alpha = 2;
	usimd_daxpy(16, input1, input2,&alpha);
	FLOAT_T expected[16] = {3,6,9,12,15,18,21,24,3,6,9,12,15,18,21,24};
	for(int i=0;i<16;i++)
	{
		CuAssertDblEquals(tc,expected[i],input2[i] , 1e-6);
	}
}

CuSuite* USIMDGetSuite() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestVadd);
	SUITE_ADD_TEST(suite, TestVmuladd);
	SUITE_ADD_TEST(suite, TestVPi);
	SUITE_ADD_TEST(suite, TestVsum);
	SUITE_ADD_TEST(suite, TestVdot);
	SUITE_ADD_TEST(suite, TestVsqrt);
	SUITE_ADD_TEST(suite, TestVdaxpy);
	return suite;
}