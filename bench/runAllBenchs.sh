#!/bin/bash
x86_platform="x86"
arm_platform="arm"
if [[ "$1" = ${x86_platform} ]]; then
    echo "SSE2 Open"
    gcc bench.c -DNPY_HAVE_SSE2 -o bench
    ./bench.exe
    echo "AVX2 Open"
    gcc bench.c -DNPY_HAVE_AVX -DNPY_HAVE_AVX2 -mavx2 -o bench
    ./bench.exe
else
    echo "Neon Open"
    gcc bench.c -DNPY_HAVE_NEON -o bench
    ./bench
fi