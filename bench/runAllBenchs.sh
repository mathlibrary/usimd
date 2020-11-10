#!/bin/bash
x86_platform="x86"
arm_platform="arm"

function ARMBaselineFloatBench() {
    echo "ARM C + float"
    gcc bench.c -o bench.o
    ./bench.o
}

function ARMBaselineDoubleBench() {
    echo "ARM C + double"
    gcc bench.c -DDOUBLE_T -o bench.o
    ./bench.o
}

function X86BaselineFloatBench() {
    echo "C + float"
    gcc bench.c -mavx2 -o bench
    ./bench.exe
}

function X86BaselineDoubleBench() {
    echo "C + double"
    gcc bench.c -DDOUBLE_T -mavx2 -o bench
    ./bench.exe
}

function SSE2FloatBench() {
    echo "SSE2 enabled + float"
    gcc bench.c -DNPY_HAVE_SSE2 -mavx2 -o bench
    ./bench.exe
}

function SSE2DoubleBench() {
    echo "SSE2 enabled + double"
    gcc bench.c -DDOUBLE_T -DNPY_HAVE_SSE2 -DNPY_HAVE_FMA3 -DNPY_HAVE_SSE3 -mfma -o bench
    ./bench.exe
}

function AVX2FloatBench() {
    echo "AVX2 enabled + float"
    gcc bench.c -DNPY_HAVE_AVX -DNPY_HAVE_AVX2 -mavx2 -o bench
    ./bench.exe
}

function AVX2DoubleBench() {
    echo "AVX2 enabled + double"
    gcc bench.c -DDOUBLE_T -DNPY_HAVE_AVX -DNPY_HAVE_AVX2 -mavx2 -o bench
    ./bench.exe
}

function NEONFloatBench() {
    echo "Neon enabled + float"
    gcc bench.c -DNPY_HAVE_NEON -o bench.o
    ./bench.o
}

function NEONDoubleBench() {
    echo "Neon enabled + double"
    gcc bench.c -DDOUBLE_T -DNPY_HAVE_NEON -o bench.o
    ./bench.o
}

if [ "$1" != "${arm_platform}" ]; then
    #X86BaselineDoubleBench
    #SSE2DoubleBench
    #X86BaselineDoubleBench
    #AVX2DoubleBench
    
    #X86BaselineFloatBench
    #SSE2FloatBench
    X86BaselineFloatBench
    AVX2FloatBench
else
    ARMBaselineFloatBench
    NEONFloatBench
fi