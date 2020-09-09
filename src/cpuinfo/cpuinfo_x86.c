
enum v_x86_cpu_features
{
    V_CPU_FEATURE_NONE = 0,
    // X86
    V_CPU_FEATURE_MMX               = 1,
    V_CPU_FEATURE_SSE               = 2,
    V_CPU_FEATURE_SSE2              = 3,
    V_CPU_FEATURE_SSE3              = 4,
    V_CPU_FEATURE_SSSE3             = 5,
    V_CPU_FEATURE_SSE41             = 6,
    V_CPU_FEATURE_POPCNT            = 7,
    V_CPU_FEATURE_SSE42             = 8,
    V_CPU_FEATURE_AVX               = 9,
    V_CPU_FEATURE_F16C              = 10,
    V_CPU_FEATURE_XOP               = 11,
    V_CPU_FEATURE_FMA4              = 12,
    V_CPU_FEATURE_FMA3              = 13,
    V_CPU_FEATURE_AVX2              = 14,
    V_CPU_FEATURE_FMA               = 15, // AVX2 & FMA3, provides backward compatibility

    V_CPU_FEATURE_AVX512F           = 30,
    V_CPU_FEATURE_AVX512CD          = 31,
    V_CPU_FEATURE_AVX512ER          = 32,
    V_CPU_FEATURE_AVX512PF          = 33,
    V_CPU_FEATURE_AVX5124FMAPS      = 34,
    V_CPU_FEATURE_AVX5124VNNIW      = 35,
    V_CPU_FEATURE_AVX512VPOPCNTDQ   = 36,
    V_CPU_FEATURE_AVX512BW          = 37,
    V_CPU_FEATURE_AVX512DQ          = 38,
    V_CPU_FEATURE_AVX512VL          = 39,
    V_CPU_FEATURE_AVX512IFMA        = 40,
    V_CPU_FEATURE_AVX512VBMI        = 41,
    V_CPU_FEATURE_AVX512VNNI        = 42,
    V_CPU_FEATURE_AVX512VBMI2       = 43,
    V_CPU_FEATURE_AVX512BITALG      = 44,

    // X86 CPU Groups
    // Knights Landing (F,CD,ER,PF)
    V_CPU_FEATURE_AVX512_KNL        = 101,
    // Knights Mill    (F,CD,ER,PF,4FMAPS,4VNNIW,VPOPCNTDQ)
    V_CPU_FEATURE_AVX512_KNM        = 102,
    // Skylake-X       (F,CD,BW,DQ,VL)
    V_CPU_FEATURE_AVX512_SKX        = 103,
    // Cascade Lake    (F,CD,BW,DQ,VL,VNNI)
    V_CPU_FEATURE_AVX512_CLX        = 104,
    // Cannon Lake     (F,CD,BW,DQ,VL,IFMA,VBMI)
    V_CPU_FEATURE_AVX512_CNL        = 105,
    // Ice Lake        (F,CD,BW,DQ,VL,IFMA,VBMI,VNNI,VBMI2,BITALG,VPOPCNTDQ)
    V_CPU_FEATURE_AVX512_ICL        = 106
};

int v_x86_cpu_have[107];

#if defined(__x86_64__) || defined(__amd64__) || defined(__x86_64) || defined(_M_AMD64) || defined( __i386__ ) || defined(i386) || defined(_M_IX86)
#ifdef _MSC_VER
    #include <intrin.h>
#elif defined(__INTEL_COMPILER)
    #include <immintrin.h>
#endif

static int
v_cpu_getxcr0(void)
{
#if defined(_MSC_VER) || defined (__INTEL_COMPILER)
    return _xgetbv(0);
#elif defined(__GNUC__) || defined(__clang__)
    /* named form of xgetbv not supported on OSX, so must use byte form, see:
     * https://github.com/asmjit/asmjit/issues/78
    */
    unsigned int eax, edx;
    __asm(".byte 0x0F, 0x01, 0xd0" : "=a"(eax), "=d"(edx) : "c"(0));
    return eax;
#else
    return 0;
#endif
}

static void
v_cpu_cpuid(int reg[4], int func_id)
{
#if defined(_MSC_VER)
    __cpuidex(reg, func_id, 0);
#elif defined(__INTEL_COMPILER)
    __cpuid(reg, func_id);
#elif defined(__GNUC__) || defined(__clang__)
    #if (defined( __i386__ ) || defined(i386) || defined(_M_IX86)) && defined(__PIC__)
        // %ebx may be the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "=a" (reg[0]), "=r" (reg[1]), "=c" (reg[2]),
                  "=d" (reg[3])
                : "a" (func_id), "c" (0)
        );
    #else
        __asm__("cpuid\n\t"
                : "=a" (reg[0]), "=b" (reg[1]), "=c" (reg[2]),
                  "=d" (reg[3])
                : "a" (func_id), "c" (0)
        );
    #endif
#else
    reg[0] = 0;
#endif
}

static void
v_x86_init_features(void)
{
    memset(v_x86_cpu_have, 0, sizeof(v_x86_cpu_have[0]) * 107);

    // validate platform support
    int reg[] = {0, 0, 0, 0};
    v_cpu_cpuid(reg, 0);
    if (reg[0] == 0) {
       v_x86_cpu_have[V_CPU_FEATURE_MMX]  = 1;
       v_x86_cpu_have[V_CPU_FEATURE_SSE]  = 1;
       v_x86_cpu_have[V_CPU_FEATURE_SSE2] = 1;
       #ifdef defined(__x86_64__) || defined(__amd64__) || defined(__x86_64) || defined(_M_AMD64)
           v_x86_cpu_have[V_CPU_FEATURE_SSE3] = 1;
       #endif
       return;
    }

    v_cpu_cpuid(reg, 1);
    v_x86_cpu_have[V_CPU_FEATURE_MMX]    = (reg[3] & (1 << 23)) != 0;
    v_x86_cpu_have[V_CPU_FEATURE_SSE]    = (reg[3] & (1 << 25)) != 0;
    v_x86_cpu_have[V_CPU_FEATURE_SSE2]   = (reg[3] & (1 << 26)) != 0;
    v_x86_cpu_have[V_CPU_FEATURE_SSE3]   = (reg[2] & (1 << 0))  != 0;
    v_x86_cpu_have[V_CPU_FEATURE_SSSE3]  = (reg[2] & (1 << 9))  != 0;
    v_x86_cpu_have[V_CPU_FEATURE_SSE41]  = (reg[2] & (1 << 19)) != 0;
    v_x86_cpu_have[V_CPU_FEATURE_POPCNT] = (reg[2] & (1 << 23)) != 0;
    v_x86_cpu_have[V_CPU_FEATURE_SSE42]  = (reg[2] & (1 << 20)) != 0;
    v_x86_cpu_have[V_CPU_FEATURE_F16C]   = (reg[2] & (1 << 29)) != 0;

    // check OSXSAVE
    if ((reg[2] & (1 << 27)) == 0)
        return;
    // check AVX OS support
    int xcr = v_cpu_getxcr0();
    if ((xcr & 6) != 6)
        return;
    v_x86_cpu_have[V_CPU_FEATURE_AVX]    = (reg[2] & (1 << 28)) != 0;
    if (!v_x86_cpu_have[V_CPU_FEATURE_AVX])
        return;
    v_x86_cpu_have[V_CPU_FEATURE_FMA3]   = (reg[2] & (1 << 12)) != 0;

    // second call to the cpuid to get extended AMD feature bits
    v_cpu_cpuid(reg, 0x80000001);
    v_x86_cpu_have[V_CPU_FEATURE_XOP]    = (reg[2] & (1 << 11)) != 0;
    v_x86_cpu_have[V_CPU_FEATURE_FMA4]   = (reg[2] & (1 << 16)) != 0;

    // third call to the cpuid to get extended AVX2 & AVX512 feature bits
    v_cpu_cpuid(reg, 7);
    v_x86_cpu_have[V_CPU_FEATURE_AVX2]   = (reg[1] & (1 << 5))  != 0;
    if (!v_x86_cpu_have[V_CPU_FEATURE_AVX2])
        return;
    // detect AVX2 & FMA3
    v_x86_cpu_have[V_CPU_FEATURE_FMA]    = v_x86_cpu_have[V_CPU_FEATURE_FMA3];

    // check AVX512 OS support
    if ((xcr & 0xe6) != 0xe6)
        return;
    v_x86_cpu_have[V_CPU_FEATURE_AVX512F]  = (reg[1] & (1 << 16)) != 0;
    v_x86_cpu_have[V_CPU_FEATURE_AVX512CD] = (reg[1] & (1 << 28)) != 0;
    if (v_x86_cpu_have[V_CPU_FEATURE_AVX512F] && v_x86_cpu_have[V_CPU_FEATURE_AVX512CD]) {
        // Knights Landing
        v_x86_cpu_have[V_CPU_FEATURE_AVX512PF]        = (reg[1] & (1 << 26)) != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512ER]        = (reg[1] & (1 << 27)) != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512_KNL]      = v_x86_cpu_have[V_CPU_FEATURE_AVX512ER] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512PF];
        // Knights Mill
        v_x86_cpu_have[V_CPU_FEATURE_AVX512VPOPCNTDQ] = (reg[2] & (1 << 14)) != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX5124VNNIW]    = (reg[3] & (1 << 2))  != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX5124FMAPS]    = (reg[3] & (1 << 3))  != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512_KNM]      = v_x86_cpu_have[V_CPU_FEATURE_AVX512_KNL] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX5124FMAPS] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX5124VNNIW] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512VPOPCNTDQ];

        // Skylake-X
        v_x86_cpu_have[V_CPU_FEATURE_AVX512DQ]        = (reg[1] & (1 << 17)) != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512BW]        = (reg[1] & (1 << 30)) != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512VL]        = (reg[1] & (1 << 31)) != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512_SKX]      = v_x86_cpu_have[V_CPU_FEATURE_AVX512BW] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512DQ] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512VL];
        // Cascade Lake
        v_x86_cpu_have[V_CPU_FEATURE_AVX512VNNI]      = (reg[2] & (1 << 11)) != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512_CLX]      = v_x86_cpu_have[V_CPU_FEATURE_AVX512_SKX] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512VNNI];

        // Cannon Lake
        v_x86_cpu_have[V_CPU_FEATURE_AVX512IFMA]      = (reg[1] & (1 << 21)) != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512VBMI]      = (reg[2] & (1 << 1))  != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512_CNL]      = v_x86_cpu_have[V_CPU_FEATURE_AVX512_SKX] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512IFMA] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512VBMI];
        // Ice Lake
        v_x86_cpu_have[V_CPU_FEATURE_AVX512VBMI2]     = (reg[2] & (1 << 6))  != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512BITALG]    = (reg[2] & (1 << 12)) != 0;
        v_x86_cpu_have[V_CPU_FEATURE_AVX512_ICL]      = v_x86_cpu_have[V_CPU_FEATURE_AVX512_CLX] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512_CNL] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512VBMI2] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512BITALG] &&
                                                         v_x86_cpu_have[V_CPU_FEATURE_AVX512VPOPCNTDQ];
    }
}
#endif