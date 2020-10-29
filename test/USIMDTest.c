#include "./cutest/CuTest.h"
#include "../src/lib/vadd.c"
#include"../src/lib/vpi.c"

void TestVadd(CuTest *tc) {
	float input1[8] = {1,2,3,4,5,6,7,8};
	float input2[8] = {1,2,3,4,5,6,7,8};
	float actual[8];
	float expected[8] = {2,4,6,8,10,12,14,16};
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

CuSuite* USIMDGetSuite() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestVadd);
	SUITE_ADD_TEST(suite, TestVPi);
	return suite;
}