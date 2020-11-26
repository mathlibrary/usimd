#include "./cutest/CuTest.h"
#include "../src/lib/common.h"

#include "../src/lib/vadd.c"
#include "../src/lib/vmuladd.c"
#include "../src/lib/vcmul.c"
#include "../src/lib/vsum.c"
#include "../src/lib/vdot.c"
#include "../src/lib/vdaxpy.c"
#include "../src/lib/vaddindex.c"

void TestVadd(CuTest *tc) {
	FLOAT_T input1[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T input2[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T actual[8];
	FLOAT_T expected[8] = {2,4,6,8,10,12,14,16};
	usimd_add(input1, input2, actual, 8);
	for(int i=0;i<8;i++) {
		CuAssertDblEquals(tc,expected[i], actual[i], 1e-6);
	}
}

void TestVsum(CuTest *tc) {
	FLOAT_T input[100];
	for(int i=0;i<100;i++) {
		input[i] = i+1;
	}
    double actual = usimd_sum(100,input,1);
    double expected = 5050;
	CuAssertDblEquals(tc,expected, actual , 1e-6);
}

void TestVdot(CuTest *tc) {
	FLOAT_T input1[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T input2[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T actual = usimd_dot(8,input1, 1, input2, 1);
	FLOAT_T expected = 204;
	CuAssertDblEquals(tc,expected, actual , 1e-6);
}

void TestVmuladd(CuTest *tc) {
	FLOAT_T input1[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T input2[8] = {1,2,3,4,5,6,7,8};
	FLOAT_T input3[8] = {1,2,3,4,5,6,7,8};
	usimd_muladd(input1,input2,input3,8);
	FLOAT_T expected[] = {2,6,12,20,30,42,56,72};
	for(int i=0;i<8;i++) 
	{
		CuAssertDblEquals(tc,expected[i], input3[i], 1e-6);
	}
}

void TestVcmul(CuTest *tc) {
	FLOAT_T input1[18] = {1,-2,-3,4,-5,-6,-7,8,2,9,5,4,6,6,10,20,20,10};
	FLOAT_T input2[18] = {1,2,3,4,5,6,7,8,3,5,4,5,7,-7,-10,20,-20,10};
	FLOAT_T output[18];
	usimd_cmul(input1,input2,output,9);
	FLOAT_T expected[] = {5,0,-25,0,11,-60,-113,0,-39,37,0,41,84,0,-500,0,-500,0};
	for(int i=0;i<18;i++) 
	{
		CuAssertDblEquals(tc,expected[i], output[i], 1e-6);
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

void TestVaddindex(CuTest *tc) {
	FLOAT_T input[105];
	FLOAT_T output[105];
	for(int i=0;i<105;i++) {
		input[i] = i+1;
		output[i] = input[i] + i;
	}
    usimd_addindex(105,input);
	for(int i=0;i<105;i++) {
		CuAssertDblEquals(tc,output[i], input[i] , 1e-6);
	}
}

CuSuite* USIMDGetSuite() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestVadd);
	SUITE_ADD_TEST(suite, TestVmuladd);
	SUITE_ADD_TEST(suite, TestVcmul);
	SUITE_ADD_TEST(suite, TestVsum);
	SUITE_ADD_TEST(suite, TestVdot);
	SUITE_ADD_TEST(suite, TestVdaxpy);
	SUITE_ADD_TEST(suite, TestVaddindex);
	return suite;
}