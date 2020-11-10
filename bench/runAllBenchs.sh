#!/bin/bash
x86_platform="x86"
arm_platform="arm"

function pureCFloat() {
    echo "pure C + float"
    gcc bench.c -o bench.o
    ./bench.o
}

function pureCDouble() {
    echo "pure C + double"
    gcc bench.c -DDOUBLE_T -o bench.o
    ./bench.o
}

function SSE2Float() {
    echo "SSE2 enabled + float"
    gcc bench.c -DNPY_HAVE_SSE2 -mfma -o bench.o
    ./bench.o
}

function SSE2Double() {
    echo "SSE2 enabled + double"
    gcc bench.c -DDOUBLE_T -DNPY_HAVE_SSE2 -DNPY_HAVE_SSE3 -DNPY_HAVE_AVX -DNPY_HAVE_FMA3 -mfma -o bench.o
    ./bench.o
}

function AVX2Float() {
    echo "AVX2 enabled + float"
    gcc bench.c -DNPY_HAVE_AVX -DNPY_HAVE_AVX2 -mavx2 -o bench.o
    ./bench.o
}

function AVX2Double() {
    echo "AVX2 enabled + double"
    gcc bench.c -DDOUBLE_T -DNPY_HAVE_AVX -DNPY_HAVE_AVX2 -mavx2 -o bench.o
    ./bench.o
}

function NEONFloat() {
    echo "Neon enabled + float"
    gcc bench.c -DNPY_HAVE_NEON -o bench.o
    ./bench.o
}

function NEONDouble() {
    echo "Neon enabled + double"
    gcc bench.c -DDOUBLE_T -DNPY_HAVE_NEON -o bench.o
    ./bench.o
}
pureCFloat
pureCDouble
if [ "$1" != "${arm_platform}" ]; then
    SSE2Float
    SSE2Double
    AVX2Float
    AVX2Double
else
    NEONFloat
    NEONDouble
fi