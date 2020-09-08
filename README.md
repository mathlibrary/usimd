# USIMD-全平台性能优化最佳实践

[TOC]

背景：众所周知，编译器会自动根据平台的并行指令集优化可并行化的代码块，但是这种优化限制非常大，大部分情况下不会产生最佳指令流水，无法最大化X86/ARM的CPU能力，所以目前主流的做法是手工编写汇编/Intrinsic产生最优的并行代码段，然而针对各个指令集架构分别编写一套代码可维护性和可拓展性都很差，随着这些代码越来越多，社区维护的成本越来越高，急需一套可推广的通用指令集优化技术方案，这就是今天要介绍的主题-USIMD优化。

## 缘起：USIMD诞生之初

假如要求一个向量所有元素的乘积，很容易写出如下的C语言代码

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

因为是简单的循环结构，很自然的会用SIMD技术进行加速，**熟悉ARM Neon**指令集的可以这么写：

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

经过改造的这段并行化代码比C语言的版本性能提升50%以上，但是很多场景是在X86平台上使用的，于是另外一个**熟悉SSE指令**的人照葫芦画瓢将代码改成下面的形式：

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

代码的性能同样在X86平台得到了提升，但是X86可不止SSE这一套指令集啊，另外还有SSE2/SSE3/SSE4.1, 后面随着寄存器尺寸翻倍，还有AVX2、AVX512等诸多指令集，而AVX512又有严重的[分裂版本](https://en.wikipedia.org/wiki/AVX-512)，可见碎片化是非常严重的，参考下面这张图：

![](./Intel-SIMD.png)

不止如此，IBM的PowerPC也不是吃素的，它也有自己的VSX指令集，所以一份完整的多平台SIMD优化方案可能会类似下面的代码：

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

是不是有一种精神分裂的感觉？这还是一个最简单的函数演化出来的场景，现实中场景比向量求乘积复杂的多，所以我一直不提倡使用汇编进行SIMD指令编排的原因，为了一丁点的性能提升牺牲了太多可维护性，而且对程序员的技能要求很高，OpenCV意识到了这个问题，在它的硬件抽象层进行了USIMD实践：

- 提供一套用于抽象intrinsics的基础结构
- 在编译时使用编译器宏和CPU检测将抽象intrinsics函数转换为具体的intrinsics调用
- 在运行时，CPU特性检测代码将进一步限制可用的指令集，并选择一条最佳的循环路径

经过实践，这套理论的可行性得到了大家的认可，近期又在Numpy社区进行了推广，这套理论也趋于成熟，说到这里可能会有点难以理解，上面这个向量乘积在USIMD框架下的代码类似于：

```C
int MultiplyIntList(int const *l1, int n)
{
    int s = 1;
#ifdef NPY_SIMD
    int dim4 = n >> 2;
    n &= 3;
    npyv_s32 sum_vec = npy_setall_s32(1);
    npyv_s32 data_vec;
    for(;dim4 > 0; dim4--){
        data_vec = npy_load_s32(l1);
        sum_vec = npy_mulq_s32(sum_vec, data_vec);
        l1+=4;
    }
    s = npy_mulr_s32(sum_vec);
#endif
    while (n--) {
        s *= (*l1++);
    }
    return s;
}
```

这一份代码可以跨X86/ARM/PowerPC，在所有平台都得到了最优的SIMD性能优化！！！

## 兴起：USIMD发展壮大

Numpy是Python的一种开源的数值计算扩展。这种工具可用来存储和处理大型矩阵，比Python自身的嵌套列表结构要高效的多，支持大量的维度数组与矩阵运算，此外也针对数组运算提供大量的数学函数库，大家比较熟悉的世界上第一张黑洞照片的生成就有[Numpy的功劳](https://numpy.org/case-studies/blackhole-image/)，目前也广泛应用于机器学习图像处理中，社区在USIMD引进之前有大量的SSE、AVX的优化实现，这些代码的维护给社区带来严重的技术债务，OpenCV重要贡献者 [Sayed Adel](https://github.com/seiko2plus) 给这种现状带来了一丝曙光，他为了提高Numpy在PowerPC上的运行效率，提出了[NEP38](https://numpy.org/neps/nep-0038-SIMD-optimizations.html)提案，并作为主要贡献者实现了[相关功能](https://github.com/numpy/numpy/pull/13516)，大家可以在[文档]( https://numpy.org/devdocs/reference/simd/simd-optimizations.html )中找到这种方案的详细描述，这里摘取主要步骤：

### 1-配置

在通过命令参数构建源文件之前，由用户配置所需的优化：

- `--cpu-baseline`：最少的必要优化基线。
- `--cpu-dispatch`：其他非基线的优化分发。

### 2-发现环境

在这一部分中，会检查编译器和平台体系结构，并缓存一些中间结果以加快重建速度。

### 3-验证所请求的优化

通过针对编译器进行测试，并根据请求的优化来查看编译器可以提供的CPU特性支持。

### 4-生成主配置头

生成的头文件`_cpu_dispatch.h`包含上一步中已验证的所需优化的所有指令集定义。

它还包含用于定义NumPy的Python级模块属性`__cpu_baseline__`和其他C定义`__cpu_dispaٍtch__`。

### 5-可调度的源代码和配置语句

 可调度源是特殊的**C**文件，可以使用不同的编译器标志以及不同的**C** 定义多次编译这些**C**文件。并且会影响代码执行路径，以根据“ **配置语句** ” 为每个编译对象启用某些指令集，这些指令集必须在**C** 注释之间声明，`(/**/)`并在每个**可分发源**的顶部以特殊标记**@targets开头**。同时，如果通过command参数禁用了优化，则可调度源将被视为普通**C**源`--disable-optimization`。 

**Numpy通过四个步骤处理可调度源**：

- **（A）识别**：就像源模板和F2PY一样，可分发源需要特殊扩展名`*.dispatch.c` 来标记C 可分发源文件
- **（B）解析和验证**：在此步骤中，已通过上一步过滤的可分发源由配置语句对它们中的每一个逐一进行解析和验证，以便确定所需的优化。
- **（C）包装**：这是NumPy的基础结构所采用的方法，事实证明这种方法足够灵活，可以使用 影响代码路径的不同**C**定义和标志多次编译单个源。通过为与附加优化有关的每个所需优化创建一个临时**C**源代码来实现此过程，该源代码包含**C**定义的声明，并通过**C**指令**#include**包括所涉及的源代码。

- **（D）可**分发的**配置头**：基础**结构**为每个可分发的源生成一个配置头，该头主要包含两个抽象的**C**宏，用于标识生成的对象，因此它们可用于在运行时从生成的对象中分发某些符号任何**C**源的对象。它也用于前向声明。 

## 实战：USIMD最佳实践

 einsum 全称 Einstein summation convention（爱因斯坦求和约定），是爱因斯坦 1916 年提出的一种标记约定，  可以替代但不限于 矩阵转置、矩阵乘法、求迹、张量乘法、数组求和等函数，可以说是一个万能函数，是Numpy中最有用的函数之一，由于其强大的表现力和智能循环，它在速度和内存效率方面通常可以超越我们常见的array函数，einsum速度快的奥秘在哪里？通过查看[源码](https://github.com/numpy/numpy/blob/master/numpy/core/src/multiarray/einsum.c.src)可以发现大量SSE代码：

```C
#if EINSUM_USE_SSE1 && @float32@
    /* Use aligned instructions if possible */
    if (EINSUM_IS_SSE_ALIGNED(data0) && EINSUM_IS_SSE_ALIGNED(data1) &&
        EINSUM_IS_SSE_ALIGNED(data_out)) {
        /* Unroll the loop by 8 */
        while (count >= 8) {
            count -= 8;

/**begin repeat2
 * #i = 0, 4#
 */
            a = _mm_mul_ps(_mm_load_ps(data0+@i@), _mm_load_ps(data1+@i@));
            b = _mm_add_ps(a, _mm_load_ps(data_out+@i@));
            _mm_store_ps(data_out+@i@, b);
/**end repeat2**/
            data0 += 8;
            data1 += 8;
            data_out += 8;
        }

        /* Finish off the loop */
        goto finish_after_unrolled_loop;
    }
#elif EINSUM_USE_SSE2 && @float64@
    /* Use aligned instructions if possible */
    if (EINSUM_IS_SSE_ALIGNED(data0) && EINSUM_IS_SSE_ALIGNED(data1) &&
        EINSUM_IS_SSE_ALIGNED(data_out)) {
        /* Unroll the loop by 8 */
        while (count >= 8) {
            count -= 8;

/**begin repeat2
 * #i = 0, 2, 4, 6#
 */
            a = _mm_mul_pd(_mm_load_pd(data0+@i@), _mm_load_pd(data1+@i@));
            b = _mm_add_pd(a, _mm_load_pd(data_out+@i@));
            _mm_store_pd(data_out+@i@, b);
/**end repeat2**/
            data0 += 8;
            data1 += 8;
            data_out += 8;
        }

        /* Finish off the loop */
        goto finish_after_unrolled_loop;
    }
#endif

    /* Unroll the loop by 8 */
    while (count >= 8) {
        count -= 8;

#if EINSUM_USE_SSE1 && @float32@
/**begin repeat2
 * #i = 0, 4#
 */
        a = _mm_mul_ps(_mm_loadu_ps(data0+@i@), _mm_loadu_ps(data1+@i@));
        b = _mm_add_ps(a, _mm_loadu_ps(data_out+@i@));
        _mm_storeu_ps(data_out+@i@, b);
/**end repeat2**/
#elif EINSUM_USE_SSE2 && @float64@
/**begin repeat2
 * #i = 0, 2, 4, 6#
 */
        a = _mm_mul_pd(_mm_loadu_pd(data0+@i@), _mm_loadu_pd(data1+@i@));
        b = _mm_add_pd(a, _mm_loadu_pd(data_out+@i@));
        _mm_storeu_pd(data_out+@i@, b);
/**end repeat2**/
#else
/**begin repeat2
 * #i = 0, 1, 2, 3, 4, 5, 6, 7#
 */
        data_out[@i@] = @to@(@from@(data0[@i@]) *
                             @from@(data1[@i@]) +
                             @from@(data_out[@i@]));
/**end repeat2**/
#endif	
```

可以看到有针对SSE/SSE2指令集进行针对性优化，性能自然是提高了，但是不可移植，需要写另外一套ARM/VSX代码来实现同样的优化效果，为了实现跨平台效果，决定对其进行USIMD改造，改造步骤如下：

1. 抽取需要进行分发的函数模块，einsum模块对外的接口是`PyArray_EinsteinSum`，但是这个接口里面只有一个函数`get_sum_of_products_function`是涉及SIMD并行化算法的，于是将其抽取为`einsum_get_sum_of_products_function`

```
static sum_of_products_fn
get_sum_of_products_function(int nop, int type_num, npy_intp itemsize, npy_intp const *fixed_strides)
{
    #ifndef NPY_DISABLE_OPTIMIZATION
        #include "einsum.dispatch.h"
    #endif
    NPY_CPU_DISPATCH_CALL(return einsum_get_sum_of_products_function,
        (nop, type_num, itemsize, fixed_strides))
}
```

2. 将所有涉及SIMD并行化算法的子函数抽取到einsum.dispatch.c.src文件中，这个文件是分发的关键，在编译时会根据`__cpu_baseline__`和`__cpu_dispatch__`分发出多个不同指令集的文件，分发的默认值是需要在头部指定的，比如我希望这个文件在X86下能分发出SSE2 AVX2 AVX512F的文件，在ARM下能分发出Neon,ASIMD的文件，因此在头部添加：

   ```
   /**
    * @targets $maxopt baseline
    * SSE2 (AVX2 FMA3) AVX512F
    * NEON ASIMD ASIMDHP
      */
   ```

3. API改造，在USIMD中提供了以下常用的宏定义

   | 宏定义       | 解释                                                |
   | ------------ | --------------------------------------------------- |
   | NPY_SIMD     | 并行化能力，SSE/VSX/NEON是128位，AVX2是256位        |
   | NPY_SIMD_F64 | 是否具备处理双浮点型float 64位数据的能力            |
   | npy_add_u8   | 无符号8位加法函数API，目前USIMD提供了260+个函数API  |
   | npy_u8       | 无符号8位数据类型API，目前USIMD提供了20+数据类型API |

   运用以上宏可以将上述SSE代码改造为：

   ```C
   #if NPY_SIMD && @float32@
       /* Use aligned instructions if possible */
       if (EINSUM_IS_ALIGNED(data0) && EINSUM_IS_ALIGNED(data1) &&
           EINSUM_IS_ALIGNED(data_out)) {
           /* Unroll the loop by UNROLL_BY */
           while (count >= UNROLL_BY) {
               count -= UNROLL_BY;
               // two simd pack in one round
               for(int i = 0; i < 2; i++) {
                   pack_size = i * UNROLL_BY / 2;
                   a = npyv_mul_f32(npyv_loada_f32(data0+pack_size), npyv_loada_f32(data1+pack_size));
                   b = npyv_add_f32(a, npyv_loada_f32(data_out+pack_size));
                   npyv_storea_f32(data_out+pack_size, b);
               }
               data0 += UNROLL_BY;
               data1 += UNROLL_BY;
               data_out += UNROLL_BY;
           }
   
           /* Finish off the loop */
           goto finish_after_unrolled_loop;
       }
   #elif NPY_SIMD_F64 && @float64@
       /* Use aligned instructions if possible */
       if (EINSUM_IS_ALIGNED(data0) && EINSUM_IS_ALIGNED(data1) &&
           EINSUM_IS_ALIGNED(data_out)) {
           /* Unroll the loop by UNROLL_BY */
           while (count >= UNROLL_BY) {
               count -= UNROLL_BY;
               for (int i  = 0; i < 4; i++) {
                   pack_size = i * UNROLL_BY / 4;
                   a = npyv_mul_f64(npyv_loada_f64(data0+pack_size), npyv_loada_f64(data1+pack_size));
                   b = npyv_add_f64(a, npyv_loada_f64(data_out+pack_size));
                   npyv_storea_f64(data_out+pack_size, b);
               }
               data0 += UNROLL_BY;
               data1 += UNROLL_BY;
               data_out += UNROLL_BY;
           }
           /* Finish off the loop */
           goto finish_after_unrolled_loop;
       }
   #endif
   
       /* Unroll the loop by UNROLL_BY */
       while (count >= UNROLL_BY) {
           count -= UNROLL_BY;
   
   #if NPY_SIMD && @float32@
           for (int i  = 0; i < 2; i++) {
               pack_size = i * UNROLL_BY / 2;
               a = npyv_mul_f32(npyv_load_f32(data0+pack_size), npyv_load_f32(data1+pack_size));
               b = npyv_add_f32(a, npyv_load_f32(data_out+pack_size));
               npyv_store_f32(data_out+pack_size, b);
           }
   #elif NPY_SIMD_F64 && @float64@
           for (int i  = 0; i < 4; i++) {
               pack_size = i * UNROLL_BY / 4;
               a = npyv_mul_f64(npyv_load_f64(data0+pack_size), npyv_load_f64(data1+pack_size));
               b = npyv_add_f64(a, npyv_load_f64(data_out+pack_size));
               npyv_store_f64(data_out+pack_size, b);
           }
   #else
       for (int i = 0; i < UNROLL_BY; i++) {
           data_out[i] = @to@(@from@(data0[i]) *
                                    @from@(data1[i]) +
                                    @from@(data_out[i]));
       }
   #endif
   ```

4. 运行测试用例和Benchmark

| einsum sub module triggered                   | master    | einsum-usimd | ratio |
| --------------------------------------------- | --------- | ------------ | ----- |
| sum_of_products_stride0_contig_outstride0_two | 110±0.5μs | 100±0.5μs    | 0.90  |
| sum_of_products_contig_stride0_outstride0_two | 110±0.5μs | 101±0.5μs    | 0.90  |
| sum_of_products_contig_stride0_outcontig_two  | 23.7±1ms  | 20.2±0.3ms   | 0.85  |
| sum_of_products_contig_two                    | 152±0.8μs | 140±2μs      | 0.91  |
| sum_of_products_stride0_contig_outcontig_two  | 946±10μs  | 841±20μs     | 0.89  |
| contig_contig_outstride0_two                  | 522±5μs   | 353±7μs      | 0.68  |
| sum_of_products_contig_outstride0_one         | 871±10μs  | 502±4μs      | 0.58  |

经过USIMD改造之后，至少有如下几个好处：

- 跨平台能力增强：只需要熟悉USIMD的API，无需熟悉各平台的指令集差异；
- 代码可维护性大大增强：USIMD的API都是经过专业的跨平台测试，这个基础设施的可靠性很高，而且都是最优的底层实现，程序员只需编写一套代码；
- 抽象层次提高：可拓展性大大增强。

## 原理：USIMD的实现机制

USIMD框架可分为编译时、运行时和API三部分，首先是编译时：

![../../_images/opt-infra.png](https://numpy.org/devdocs/_images/opt-infra.png)

首先判断有没有打开`--disable-optimization`开关，如果打开了就走正常编译链接流程，否则就会从源码中扫描`*.dispatch.c`的文件，初始化底层API库，根据运行环境判断架构平台(X86/ARM/PowerPC)，如果发现`*.dispatch.c`已经分发了指定特性(缓存到build目录下的temp)，就把这些分发文件加入到源码编译树中，以便后面可以编译链接，若没有缓存(第一次编译或修改了特性baseline)，就解析--cpu-baseline和--cpu-dispatch里面的特性，结合编译器的支持程度，生成配置头(定义了需要分发的特性)，比如有如下baseline:

```c
/**
 * @targets $maxopt baseline
 * SSE2 (AVX2 FMA3) AVX512F
 * NEON NEON_VFPV4
 */
```

在同时支持AVX2和FMA3并且支持AVX512F的编译器编译之后会生成

```c
#define NPY__CPU_DISPATCH_CALL(CHK, CB, ...) \
	NPY__CPU_DISPATCH_EXPAND_(CB((CHK(AVX512_SKX)), AVX512F, __VA_ARGS__)) \
	NPY__CPU_DISPATCH_EXPAND_(CB((CHK(AVX)&&CHK(F16C)&&CHK(FMA3)&&CHK(AVX2)), AVX2, __VA_ARGS__))
```

这些代码的意思相当于：

```C
if (NPY_CPU_HAVE(AVX512F)) {
   return XXX_avx512f(...);
} else if (NPY_CPU_HAVE(AVX2) && NPY_CPU_HAVE(FMA3)) {
   return XXX_avx2__fma3(...);
} else {
    // fallback to the baseline
    return XXX(...);
}
```

现在编译器把针对各个特性的patch包生成了，可是运行的时候在别的机器上运行，别的机器的CPU不一定支持AVX或者SSE啊，这个时候怎么办？接下来就需要运行时发挥作用了。

![image-20200731111126479](./image-cpu-feature-detect.png)

在运行时会通过下面这段代码检测机器是否支持某个特性：

X86平台：

```C
#if defined(_MSC_VER) || defined (__INTEL_COMPILER)
    return _xgetbv(0);
#elif defined(__GNUC__) || defined(__clang__)
    unsigned int eax, edx;
    __asm(".byte 0x0F, 0x01, 0xd0" : "=a"(eax), "=d"(edx) : "c"(0));
    return eax;
#else
    return 0;
#endif
```

Power平台:

```C
unsigned int hwcap = getauxval(AT_HWCAP);
if ((hwcap & PPC_FEATURE_HAS_VSX) == 0)
     return;
hwcap = getauxval(AT_HWCAP2);
```

ARM平台：

```
if (getauxval != 0) {
        hwcap = getauxval(NPY__HWCAP);
    #ifdef __arm__
        hwcap2 = getauxval(NPY__HWCAP2);
    #endif
    } else {
        unsigned long auxv[2];
        int fd = open("/proc/self/auxv", O_RDONLY);
        if (fd >= 0) {
            while (read(fd, &auxv, sizeof(auxv)) == sizeof(auxv)) {
                if (auxv[0] == NPY__HWCAP) {
                    hwcap = auxv[1];
                }
            #ifdef __arm__
                else if (auxv[0] == NPY__HWCAP2) {
                    hwcap2 = auxv[1];
                }
            #endif
                // detect the end
                else if (auxv[0] == 0 && auxv[1] == 0) {
                    break;
                }
            }
            close(fd);
        }
    }
    if (hwcap == 0 && hwcap2 == 0) {
        /*
         * try parsing with /proc/cpuinfo, if sandboxed
         * failback to compiler definitions
        */
        if(!get_feature_from_proc_cpuinfo(&hwcap, &hwcap2)) {
            return 0;
        }
    }
```

通过上述检测，CPU的特性就可以全部识别出来，如果某个特性是支持的，相关的宏就会打开，这样就实现了运行时分发包的目的。

底层API可以分为如下几类：

| 指令分类 | 示例             |
| -------- | ---------------- |
| 算术     | npyv_add_u8      |
| 转换     | npyv_cvt_u8_b8   |
| 内存     | npyv_load_f32    |
| 重排序   | npyv_combinel_u8 |
| 位运算   | npyv_shl_u16     |
| 杂项     | npyv_zero_u8     |
| 数据类型 | npyv_u8          |

以X86平台的SSE作为基准API，其它平台如果没有相对应的指令，就使用模拟的方式实现。