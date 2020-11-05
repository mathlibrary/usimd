#!/bin/bash
x86_platform="x86"
arm_platform="arm"

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

function ARMBaselineFloatBench() {
    echo "C + float"
    gcc bench.c -mavx2 -o bench
    ./bench.exe
}

function ARMBaselineDoubleBench() {
    echo "C + double"
    gcc bench.c -DDOUBLE_T -mavx2 -o bench
    ./bench.exe
}

function SSE2Bench() {
    echo "SSE2 enabled + float"
    gcc bench.c -DNPY_HAVE_SSE2 -mavx2 -o bench
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
    gcc bench.c -DNPY_HAVE_NEON -o bench
    ./bench
}



function main() {
    if [[ "$1" != ${arm_platform} ]]; then
        #AVX2FloatBench
        X86BaselineDoubleBench
        AVX2DoubleBench
    else
        NEONFloatBench
    fi
}
main