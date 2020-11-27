# Best practices for cross platform performance optimization

Background: As we all know, the compiler will automatically optimize the parallelizable code blocks according to the parallel instruction set of the platform, but this optimization limit is very large, in most cases it will not produce the best instruction pipeline, and cannot maximize the X86/ARM CPU Ability, so the current mainstream approach is to manually write assembly/Intrinsic to generate the optimal parallel code segment. However, writing a set of codes for each instruction set architecture is very poor in maintainability and scalability. As these codes increases, The cost of community maintenance is getting higher, there is an urgent need for a general-purpose instruction set optimization technical solution that can be promoted. This project is the basically the fork of numpy's usimd work, The distribution mechanism has modified in order to seperate from numpy, The test system uses cuTest and the bench system born from openblas.

## Origin: The birth of USIMD

If the product of all elements of a vector is required, it is easy to write the following C language code.

```c
int MultiplyIntList(int const *l1, int n)

{
  int s = 1;
  while (n--) {
​    s *= (*l1++);
  }
  return s;
}
```

Because it is a simple loop structure, it is natural to use SIMD technology for acceleration. Those who are familiar with the ARM Neon instruction set can write as follows:

```c
int MultiplyIntList(int const *l1, int n)
{
    int s = 1;
#ifdef HAVE_NEON
    int dim4 = n >> 2;
    n &= 3;
    int32x4_t sum_vec = vdupq_n_s32(1);
    int32x4_t data_vec;
    for(;dim4 > 0; dim4--){
        data_vec = vld1q_s32(l1);
        sum_vec = vmulq_s32(sum_vec, data_vec);
        l1+=4;
    }
    s = vgetq_lane_s32(sum_vec,0)*vgetq_lane_s32(sum_vec,1)*
    vgetq_lane_s32(sum_vec,2)*vgetq_lane_s32(sum_vec,3);
#endif
    while (n--) {
        s *= (*l1++);
    }
    return s;
}
```

The performance of this modified parallelized code is more than 50% higher than that of the C language version, but many scenarios are used on the X86 platform, so another person who is familiar with SSE instructions changed the code to the following form:

```c
int MultiplyIntList(int const *l1, int n)
{
    int s = 1;
#ifdef HAVE_NEON
    ...
#elif HAVE_SSE
    int dim4 = n >> 2;
    n &= 3;
    _m128i sum_vec = _mm_setzero_si128(1);
    _m128i data_vec;
    for(;dim4 > 0; dim4--){
        data_vec = _mm_setr_epi32(l1);
        sum_vec = _mm_mullo_epi32(sum_vec, data_vec);
        l1+=4;
    }
    data_vec = _mm_shuffle_epi32(sum_vec, sum_vec, _MM_SHUFFLE(2,3,0,1));	
    sum_vec = _mm_mullo_epi32(sum_vec, data_vec);	
    data_vec = _mm_shuffle_epi32(sum_vec, sum_vec, _MM_SHUFFLE(1,0,3,2));	
    sum_vec = _mm_mullo_epi32(data_vec, accum_sse);
    _mm_store_ss(&s, sum_vec);
#endif
    while (n--) {
        s *= (*l1++);
    }
    return s;
}
```

The performance of the code has also been improved on the X86 platform, but X86 is not limited to the SSE instruction set, there are also SSE2/SSE3/SSE4.1, followed by the doubling of the register size, and many instructions such as AVX2, AVX512, etc. AVX512 has a serious [split version](https://en.wikipedia.org/wiki/AVX-512). It can be seen that fragmentation is very serious. Refer to the following picture:

![](./Intel-SIMD.png)

Not only that, IBM's PowerPC is not vegetarian, it also has its own VSX instruction set, so a complete multi-platform SIMD optimization solution may be similar to the following code:

```C
int MultiplyIntList(int const *l1, int n)
{
    int s = 1;
#ifdef HAVE_NEON
    ...
#elif HAVE_SSE
    ...
#elif HAVE_SSE2
    ...
#elif HAVE_AVX2
    ...
#elif HAVE_VSX
    ...
#endif
    while (n--) {
        s *= (*l1++);
    }
    return s;
}
```

Is there a feeling of schizophrenia? 
This is still a scene evolved from the simplest function. In reality, the scene is much more complicated than vector product. Therefore, I have always discouraged the use of assembly for SIMD instruction layout. I sacrificed too much maintainability for a little performance improvement. 
, And the programmer’s skills are very demanding. OpenCV is aware of this problem and implements USIMD practice in its hardware abstraction layer:

-Provide a set of basic structure for abstract intrinsics

-Use compiler macros and CPU detection at compile time to convert abstract intrinsics functions into concrete intrinsics calls

-At runtime, the CPU feature detection code will further limit the available instruction set and choose an optimal loop path

After practice, the feasibility of this theory has been recognized by everyone. Recently, it has been promoted in the Numpy community. This theory has also become mature. It may be a bit difficult to understand here. The above vector product is under the USIMD framework. 
The code is similar to:

```C
int MultiplyIntList(int const *l1, int n)
{
    int s = 1;
#ifdef V_SIMD
    int dim4 = n >> 2;
    n &= 3;
    v_s32 sum_vec = v_setall_s32(1);
    v_s32 data_vec;
    for(;dim4 > 0; dim4--){
        data_vec = v_load_s32(l1);
        sum_vec = v_mulq_s32(sum_vec, data_vec);
        l1+=4;
    }
    s = v_mul_s32(sum_vec);
#endif
    while (n--) {
        s *= (*l1++);
    }
    return s;
}
```

This piece of code can cross X86/ARM/PowerPC, and has got the best SIMD performance optimization on all platforms! ! !

## Rise: The grows of USIMD 

Numpy is an open source numerical computing extension of Python. This tool can be used to store and process large matrices. It is much more efficient than Python's own nested list structure. It supports a large number of dimensional arrays and matrix operations. In addition, it also provides a large number of mathematical function libraries for array operations. It is more familiar to everyone. 
[Numpy’s credit](https://numpy.org/case-studies/blackhole-image/) was generated for the first black hole photo above, and it is also widely used in machine learning image processing. Before the introduction of USIMD,there are a large number of optimized implementations of SSE and AVX. The maintenance of these codes has brought serious technical debt to the community. An important contributor to OpenCV [Sayed Adel](https://github.com/seiko2plus) brought universal intrinsics in order to improve the data parallelization efficiency in Numpy, he proposed [NEP38](https://numpy.org/neps/nep-0038-SIMD-optimizations.html) and implemented it at [Related Function](https://github.com/numpy/numpy/pull/13516), you can find this in [document]( https://numpy.org/devdocs/reference/simd/simd-optimizations.html).

## Benefit: USIMD

After the USIMD transformation, there are at least the following benefits:

-Cross-platform capability enhancement: only need to familiar with the API of USIMD, no need to be familiar with the difference in instruction set of each platform;

-Code maintainability is greatly enhanced: USIMD's APIs have been fullly tested across platforms. This infrastructure is highly reliable and is the best implementation. developers only need to write a set of codes;

-Improved abstraction level: The scalability is greatly enhanced.