#!/bin/bash
x86_platform="x86"
arm_platform="arm"
function pureCFloat() {
    echo "Pure C + Single-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -o main.o
    ./main.o
}

function pureCDouble() {
    echo "Pure C + Double-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DDOUBLE_T -o main.o
    ./main.o
}

function SSE2Float() {
    echo "SSE2 + Single-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DV_HAVE_SSE2 -o main.o
    ./main.o
}


function SSE2Double() {
    echo "SSE2 + Double-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c  -DDOUBLE_T -DV_HAVE_SSE2 -o main.o
    ./main.o
}

function SSE3Float() {
    echo "SSE3 + Single-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DV_HAVE_SSE2 -DV_HAVE_SSE3 -msse3 -o main.o
    ./main.o
}

function SSE3Double() {
    echo "SSE3 + Double-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DDOUBLE_T -DV_HAVE_SSE2 -DV_HAVE_SSE3 -msse3 -o main.o
    ./main.o
}

function AVX2Float() {
    echo "AVX2 + Single-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DV_HAVE_AVX -DV_HAVE_AVX2 -mfma -mavx2 -o main.o
    ./main.o
}

function AVX2Double() {
    echo "AVX2 + Double-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DDOUBLE_T -DV_HAVE_AVX -DV_HAVE_AVX2 -mfma -mavx2 -o main.o
    ./main.o
}

function NEONFloat() {
    echo "NEON + Single-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DV_HAVE_NEON -o main.o
    ./main.o
}

function NEONDouble() {
    echo "NEON + Double-precision"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c  -DDOUBLE_T -DV_HAVE_NEON -o main.o
    ./main.o
}

pureCFloat
pureCDouble
if [[ "$*" != ${arm_platform} ]]; then
    SSE2Float
    SSE2Double
    SSE3Float
    SSE3Double
    AVX2Float
    AVX2Double
else
    NEONFloat
    NEONDouble
fi