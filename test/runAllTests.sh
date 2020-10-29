#!/bin/bash
x86_platform="x86"
arm_platform="arm"
if [[ "$*" = ${x86_platform} ]]; then
#    echo "SSE2 Open"
#    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DNPY_HAVE_SSE2 -o main
#    ./main.exe
    echo "AVX2 Open"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DNPY_HAVE_AVX -DNPY_HAVE_AVX2 -mavx2 -o main
    ./main.exe
else
    echo "Neon Open"
    gcc AllTests.c ./cutest/CuTest.c USIMDTest.c -DNPY_HAVE_NEON -o main
    ./main
fi