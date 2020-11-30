#!/bin/bash
x86_platform="x86"
arm_platform="arm"

function pureCFloat() {
    # multithread : -fopenmp auto-vectorize: -fopt-info-optall-optimized -ftree-vectorize
    echo "pure C + float"
    gcc bench.c -march=native -fopenmp -ffast-math -g -O3 -o bench.o
    ./bench.o
}

function pureCDouble() {
    echo "pure C + double"
    gcc bench.c -DDOUBLE_T -march=native -O3 -ffast-math -o bench.o
    ./bench.o
}

function SSE2Float() {
    echo "SSE2 enabled + float"
    gcc bench.c -DV_HAVE_SSE2 -march=native -O3 -ffast-math -mfma -o bench.o
    ./bench.o
}

function SSE2Double() {
    echo "SSE2 enabled + double"
    gcc bench.c -DDOUBLE_T -DV_HAVE_SSE2 -o bench.o
    ./bench.o
}

function SSE3Float() {
    echo "SSE3 enabled + float"
    gcc bench.c -DV_HAVE_SSE2 -DV_HAVE_SSE3 -march=native -O3 -ffast-math -msse3 -o bench.o
    ./bench.o
}

function SSE3Double() {
    echo "SSE3 enabled + double"
    gcc bench.c -DDOUBLE_T -DV_HAVE_SSE2 -DV_HAVE_SSE3 -march=native -O3 -ffast-math -msse3 -o bench.o
    ./bench.o
}

function AVX2Float() {
    echo "AVX2 enabled + float"
    gcc bench.c -DV_HAVE_AVX -DV_HAVE_AVX2 -march=native -O3 -ffast-math -mavx2 -mfma -o bench.o
    ./bench.o
}

function AVX2Double() {
    echo "AVX2 enabled + double"
    gcc bench.c -DDOUBLE_T -DV_HAVE_AVX -DV_HAVE_AVX2 -march=native -O3 -ffast-math -mavx2 -mfma -o bench.o
    ./bench.o
}

function NEONFloat() {
    echo "Neon enabled + float"
    gcc bench.c -DV_HAVE_NEON -o bench.o
    ./bench.o
}

function NEONDouble() {
    echo "Neon enabled + double"
    gcc bench.c -DDOUBLE_T -DV_HAVE_NEON -o bench.o
    ./bench.o
}
pureCFloat
#pureCDouble
if [ "$1" != "${arm_platform}" ]; then
    SSE2Float
    #SSE2Double
    SSE3Float
    #SSE3Double
    AVX2Float
    #AVX2Double
    echo "finished"
else
    NEONFloat
    NEONDouble
fi