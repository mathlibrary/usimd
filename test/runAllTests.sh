#!/bin/bash
x86_platform="x86"
arm_platform="arm"
if [[ "$*" != ${arm_platform} ]]; then
#    echo "SSE2 Open"
#    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DNPY_HAVE_SSE2 -o main
#    ./main.exe
    echo "AVX2 enabled + Single-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DNPY_HAVE_AVX -DNPY_HAVE_AVX2 -mavx2 -o main
    ./main.exe
    echo "AVX2 enabled + Double-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DDOUBLE_T -DNPY_HAVE_AVX -DNPY_HAVE_AVX2 -mavx2 -o main
    ./main.exe
else
    echo "Pure C + Single-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -o main.o
    ./main.o

    echo "Pure C + Double-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DDOUBLE_T -o main.o
    ./main.o

    echo "Neon enabled + Single-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DNPY_HAVE_NEON -o main.o
    ./main.o

    echo "Neon enabled + Double-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DDOUBLE_T -DNPY_HAVE_NEON -o main.o
    ./main.o
fi