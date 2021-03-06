#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_AVX512_MEMORY_H
#define _V_SIMD_AVX512_MEMORY_H

#include "misc.h"

/***************************
 * load/store
 ***************************/
#if defined(__GNUC__)
    // GCC expect pointer argument type to be `void*` instead of `const void *`,
    // which cause a massive warning.
    #define v__loads(PTR) _mm512_stream_load_si512((__m512i*)(PTR))
#else
    #define v__loads(PTR) _mm512_stream_load_si512((const __m512i*)(PTR))
#endif
#if defined(_MSC_VER) && defined(_M_IX86)
    // workaround msvc(32bit) overflow bug, reported at
    // https://developercommunity.visualstudio.com/content/problem/911872/u.html
    V_FINLINE __m512i v__loadl(const __m256i *ptr)
    {
        __m256i a = _mm256_loadu_si256(ptr);
        return _mm512_inserti64x4(_mm512_castsi256_si512(a), a, 0);
    }
#else
    #define v__loadl(PTR) \
        _mm512_castsi256_si512(_mm256_loadu_si256(PTR))
#endif
#define V_IMPL_AVX512_MEM_INT(CTYPE, SFX)                                 \
    V_FINLINE v_##SFX v_load_##SFX(const CTYPE *ptr)                 \
    { return _mm512_loadu_si512((const __m512i*)ptr); }                      \
    V_FINLINE v_##SFX v_loada_##SFX(const CTYPE *ptr)                \
    { return _mm512_load_si512((const __m512i*)ptr); }                       \
    V_FINLINE v_##SFX v_loads_##SFX(const CTYPE *ptr)                \
    { return v__loads(ptr); }                                             \
    V_FINLINE v_##SFX v_loadl_##SFX(const CTYPE *ptr)                \
    { return v__loadl((const __m256i *)ptr); }                            \
    V_FINLINE void v_store_##SFX(CTYPE *ptr, v_##SFX vec)            \
    { _mm512_storeu_si512((__m512i*)ptr, vec); }                             \
    V_FINLINE void v_storea_##SFX(CTYPE *ptr, v_##SFX vec)           \
    { _mm512_store_si512((__m512i*)ptr, vec); }                              \
    V_FINLINE void v_stores_##SFX(CTYPE *ptr, v_##SFX vec)           \
    { _mm512_stream_si512((__m512i*)ptr, vec); }                             \
    V_FINLINE void v_storel_##SFX(CTYPE *ptr, v_##SFX vec)           \
    { _mm256_storeu_si256((__m256i*)ptr, v512_lower_si256(vec)); }        \
    V_FINLINE void v_storeh_##SFX(CTYPE *ptr, v_##SFX vec)           \
    { _mm256_storeu_si256((__m256i*)(ptr), v512_higher_si256(vec)); }

V_IMPL_AVX512_MEM_INT(s_uint8,  u8)
V_IMPL_AVX512_MEM_INT(s_int8,   s8)
V_IMPL_AVX512_MEM_INT(s_uint16, u16)
V_IMPL_AVX512_MEM_INT(s_int16,  s16)
V_IMPL_AVX512_MEM_INT(s_uint32, u32)
V_IMPL_AVX512_MEM_INT(s_int32,  s32)
V_IMPL_AVX512_MEM_INT(s_uint64, u64)
V_IMPL_AVX512_MEM_INT(s_int64,  s64)

// unaligned load
#define v_load_f32(PTR) _mm512_loadu_ps((const __m512*)(PTR))
#define v_load_f64(PTR) _mm512_loadu_pd((const __m512d*)(PTR))
// aligned load
#define v_loada_f32(PTR) _mm512_load_ps((const __m512*)(PTR))
#define v_loada_f64(PTR) _mm512_load_pd((const __m512d*)(PTR))
// load lower part
#if defined(_MSC_VER) && defined(_M_IX86)
    #define v_loadl_f32(PTR) _mm512_castsi512_ps(v__loadl((const __m256i *)(PTR)))
    #define v_loadl_f64(PTR) _mm512_castsi512_pd(v__loadl((const __m256i *)(PTR)))
#else
    #define v_loadl_f32(PTR) _mm512_castps256_ps512(_mm256_loadu_ps(PTR))
    #define v_loadl_f64(PTR) _mm512_castpd256_pd512(_mm256_loadu_pd(PTR))
#endif
// stream load
#define v_loads_f32(PTR) _mm512_castsi512_ps(v__loads(PTR))
#define v_loads_f64(PTR) _mm512_castsi512_pd(v__loads(PTR))
// unaligned store
#define v_store_f32 _mm512_storeu_ps
#define v_store_f64 _mm512_storeu_pd
// aligned store
#define v_storea_f32 _mm512_store_ps
#define v_storea_f64 _mm512_store_pd
// stream store
#define v_stores_f32 _mm512_stream_ps
#define v_stores_f64 _mm512_stream_pd
// store lower part
#define v_storel_f32(PTR, VEC) _mm256_storeu_ps(PTR, v512_lower_ps256(VEC))
#define v_storel_f64(PTR, VEC) _mm256_storeu_pd(PTR, v512_lower_pd256(VEC))
// store higher part
#define v_storeh_f32(PTR, VEC) _mm256_storeu_ps(PTR, v512_higher_ps256(VEC))
#define v_storeh_f64(PTR, VEC) _mm256_storeu_pd(PTR, v512_higher_pd256(VEC))
/***************************
 * Non-contiguous Load
 ***************************/
//// 32
V_FINLINE v_u32 v_loadn_u32(const s_uint32 *ptr, s_intp stride)
{
    assert(llabs(stride) <= V_SIMD_MAXLOAD_STRIDE32);
    const __m512i steps = v_set_s32(
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    );
    const __m512i idx = _mm512_mullo_epi32(steps, _mm512_set1_epi32((int)stride));
    return _mm512_i32gather_epi32(idx, (const __m512i*)ptr, 4);
}
V_FINLINE v_s32 v_loadn_s32(const s_int32 *ptr, s_intp stride)
{ return v_loadn_u32((const s_uint32*)ptr, stride); }
V_FINLINE v_f32 v_loadn_f32(const float *ptr, s_intp stride)
{ return _mm512_castsi512_ps(v_loadn_u32((const s_uint32*)ptr, stride)); }
//// 64
V_FINLINE v_u64 v_loadn_u64(const s_uint64 *ptr, s_intp stride)
{
    const __m512i idx = _mm512_setr_epi64(
        0*stride, 1*stride, 2*stride, 3*stride,
        4*stride, 5*stride, 6*stride, 7*stride
    );
    return _mm512_i64gather_epi64(idx, (const __m512i*)ptr, 8);
}
V_FINLINE v_s64 v_loadn_s64(const s_int64 *ptr, s_intp stride)
{ return v_loadn_u64((const s_uint64*)ptr, stride); }
V_FINLINE v_f64 v_loadn_f64(const double *ptr, s_intp stride)
{ return _mm512_castsi512_pd(v_loadn_u64((const s_uint64*)ptr, stride)); }
/***************************
 * Non-contiguous Store
 ***************************/
//// 32
V_FINLINE void v_storen_u32(s_uint32 *ptr, s_intp stride, v_u32 a)
{
    assert(llabs(stride) <= V_SIMD_MAXSTORE_STRIDE32);
    const __m512i steps = _mm512_setr_epi32(
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    );
    const __m512i idx = _mm512_mullo_epi32(steps, _mm512_set1_epi32((int)stride));
    _mm512_i32scatter_epi32((__m512i*)ptr, idx, a, 4);
}
V_FINLINE void v_storen_s32(s_int32 *ptr, s_intp stride, v_s32 a)
{ v_storen_u32((s_uint32*)ptr, stride, a); }
V_FINLINE void v_storen_f32(float *ptr, s_intp stride, v_f32 a)
{ v_storen_u32((s_uint32*)ptr, stride, _mm512_castps_si512(a)); }
//// 64
V_FINLINE void v_storen_u64(s_uint64 *ptr, s_intp stride, v_u64 a)
{
    const __m512i idx = _mm512_setr_epi64(
        0*stride, 1*stride, 2*stride, 3*stride,
        4*stride, 5*stride, 6*stride, 7*stride
    );
    _mm512_i64scatter_epi64((__m512i*)ptr, idx, a, 8);
}
V_FINLINE void v_storen_s64(s_int64 *ptr, s_intp stride, v_s64 a)
{ v_storen_u64((s_uint64*)ptr, stride, a); }
V_FINLINE void v_storen_f64(double *ptr, s_intp stride, v_f64 a)
{ v_storen_u64((s_uint64*)ptr, stride, _mm512_castpd_si512(a)); }

/*********************************
 * Partial Load
 *********************************/
//// 32
V_FINLINE v_s32 v_load_till_s32(const s_int32 *ptr, s_uintp nlane, s_int32 fill)
{
    assert(nlane > 0);
    const __m512i vfill = _mm512_set1_epi32(fill);
    const __mmask16 mask = nlane > 31 ? -1 : (1 << nlane) - 1;
    return _mm512_mask_loadu_epi32(vfill, mask, (const __m512i*)ptr);
}
// fill zero to rest lanes
V_FINLINE v_s32 v_load_tillz_s32(const s_int32 *ptr, s_uintp nlane)
{
    assert(nlane > 0);
    const __mmask16 mask = nlane > 31 ? -1 : (1 << nlane) - 1;
    return _mm512_maskz_loadu_epi32(mask, (const __m512i*)ptr);
}
//// 64
V_FINLINE v_s64 v_load_till_s64(const s_int64 *ptr, s_uintp nlane, s_int64 fill)
{
    assert(nlane > 0);
    const __m512i vfill = _mm512_set1_epi64(fill);
    const __mmask8 mask = nlane > 31 ? -1 : (1 << nlane) - 1;
    return _mm512_mask_loadu_epi64(vfill, mask, (const __m512i*)ptr);
}
// fill zero to rest lanes
V_FINLINE v_s64 v_load_tillz_s64(const s_int64 *ptr, s_uintp nlane)
{
    assert(nlane > 0);
    const __mmask8 mask = nlane > 15 ? -1 : (1 << nlane) - 1;
    return _mm512_maskz_loadu_epi64(mask, (const __m512i*)ptr);
}
/*********************************
 * Non-contiguous partial load
 *********************************/
//// 32
V_FINLINE v_s32
v_loadn_till_s32(const s_int32 *ptr, s_intp stride, s_uintp nlane, s_int32 fill)
{
    assert(nlane > 0);
    assert(llabs(stride) <= V_SIMD_MAXLOAD_STRIDE32);
    const __m512i steps = v_set_s32(
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    );
    const __m512i idx = _mm512_mullo_epi32(steps, _mm512_set1_epi32((int)stride));
    const __m512i vfill = _mm512_set1_epi32(fill);
    const __mmask16 mask = nlane > 31 ? -1 : (1 << nlane) - 1;
    return _mm512_mask_i32gather_epi32(vfill, mask, idx, (const __m512i*)ptr, 4);
}
// fill zero to rest lanes
V_FINLINE v_s32
v_loadn_tillz_s32(const s_int32 *ptr, s_intp stride, s_uintp nlane)
{ return v_loadn_till_s32(ptr, stride, nlane, 0); }
//// 64
V_FINLINE v_s64
v_loadn_till_s64(const s_int64 *ptr, s_intp stride, s_uintp nlane, s_int64 fill)
{
    assert(nlane > 0);
    const __m512i idx = _mm512_setr_epi64(
        0*stride, 1*stride, 2*stride, 3*stride,
        4*stride, 5*stride, 6*stride, 7*stride
    );
    const __m512i vfill = _mm512_set1_epi64(fill);
    const __mmask8 mask = nlane > 31 ? -1 : (1 << nlane) - 1;
    return _mm512_mask_i64gather_epi64(vfill, mask, idx, (const __m512i*)ptr, 8);
}
// fill zero to rest lanes
V_FINLINE v_s64
v_loadn_tillz_s64(const s_int64 *ptr, s_intp stride, s_uintp nlane)
{ return v_loadn_till_s64(ptr, stride, nlane, 0); }
/*********************************
 * Partial store
 *********************************/
//// 32
V_FINLINE void v_store_till_s32(s_int32 *ptr, s_uintp nlane, v_s32 a)
{
    assert(nlane > 0);
    const __mmask16 mask = nlane > 31 ? -1 : (1 << nlane) - 1;
    _mm512_mask_storeu_epi32((__m512i*)ptr, mask, a);
}
//// 64
V_FINLINE void v_store_till_s64(s_int64 *ptr, s_uintp nlane, v_s64 a)
{
    assert(nlane > 0);
    const __mmask8 mask = nlane > 15 ? -1 : (1 << nlane) - 1;
    _mm512_mask_storeu_epi64((__m512i*)ptr, mask, a);
}
/*********************************
 * Non-contiguous partial store
 *********************************/
//// 32
V_FINLINE void v_storen_till_s32(s_int32 *ptr, s_intp stride, s_uintp nlane, v_s32 a)
{
    assert(nlane > 0);
    assert(llabs(stride) <= V_SIMD_MAXSTORE_STRIDE32);
    const __m512i steps = _mm512_setr_epi32(
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    );
    const __m512i idx = _mm512_mullo_epi32(steps, _mm512_set1_epi32((int)stride));
    const __mmask16 mask = nlane > 31 ? -1 : (1 << nlane) - 1;
    _mm512_mask_i32scatter_epi32((__m512i*)ptr, mask, idx, a, 4);
}
//// 64
V_FINLINE void v_storen_till_s64(s_int64 *ptr, s_intp stride, s_uintp nlane, v_s64 a)
{
    assert(nlane > 0);
    const __m512i idx = _mm512_setr_epi64(
        0*stride, 1*stride, 2*stride, 3*stride,
        4*stride, 5*stride, 6*stride, 7*stride
    );
    const __mmask8 mask = nlane > 15 ? -1 : (1 << nlane) - 1;
    _mm512_mask_i64scatter_epi64((__m512i*)ptr, mask, idx, a, 8);
}

/*****************************************************************************
 * Implement partial load/store for u32/f32/u64/f64... via reinterpret cast
 *****************************************************************************/
#define V_IMPL_AVX512_REST_PARTIAL_TYPES(F_SFX, T_SFX)                                   \
    V_FINLINE v_##F_SFX v_load_till_##F_SFX                                         \
    (const v_lanetype_##F_SFX *ptr, s_uintp nlane, v_lanetype_##F_SFX fill)         \
    {                                                                                       \
        union {                                                                             \
            v_lanetype_##F_SFX from_##F_SFX;                                             \
            v_lanetype_##T_SFX to_##T_SFX;                                               \
        } pun = {.from_##F_SFX = fill};                                                     \
        return v_reinterpret_##F_SFX##_##T_SFX(v_load_till_##T_SFX(                   \
            (const v_lanetype_##T_SFX *)ptr, nlane, pun.to_##T_SFX                       \
        ));                                                                                 \
    }                                                                                       \
    V_FINLINE v_##F_SFX v_loadn_till_##F_SFX                                        \
    (const v_lanetype_##F_SFX *ptr, s_intp stride, s_uintp nlane,                    \
     v_lanetype_##F_SFX fill)                                                            \
    {                                                                                       \
        union {                                                                             \
            v_lanetype_##F_SFX from_##F_SFX;                                             \
            v_lanetype_##T_SFX to_##T_SFX;                                               \
        } pun = {.from_##F_SFX = fill};                                                     \
        return v_reinterpret_##F_SFX##_##T_SFX(v_loadn_till_##T_SFX(                  \
            (const v_lanetype_##T_SFX *)ptr, stride, nlane, pun.to_##T_SFX               \
        ));                                                                                 \
    }                                                                                       \
    V_FINLINE v_##F_SFX v_load_tillz_##F_SFX                                        \
    (const v_lanetype_##F_SFX *ptr, s_uintp nlane)                                     \
    {                                                                                       \
        return v_reinterpret_##F_SFX##_##T_SFX(v_load_tillz_##T_SFX(                  \
            (const v_lanetype_##T_SFX *)ptr, nlane                                       \
        ));                                                                                 \
    }                                                                                       \
    V_FINLINE v_##F_SFX v_loadn_tillz_##F_SFX                                       \
    (const v_lanetype_##F_SFX *ptr, s_intp stride, s_uintp nlane)                    \
    {                                                                                       \
        return v_reinterpret_##F_SFX##_##T_SFX(v_loadn_tillz_##T_SFX(                 \
            (const v_lanetype_##T_SFX *)ptr, stride, nlane                               \
        ));                                                                                 \
    }                                                                                       \
    V_FINLINE void v_store_till_##F_SFX                                                \
    (v_lanetype_##F_SFX *ptr, s_uintp nlane, v_##F_SFX a)                           \
    {                                                                                       \
        v_store_till_##T_SFX(                                                            \
            (v_lanetype_##T_SFX *)ptr, nlane,                                            \
            v_reinterpret_##T_SFX##_##F_SFX(a)                                           \
        );                                                                                  \
    }                                                                                       \
    V_FINLINE void v_storen_till_##F_SFX                                               \
    (v_lanetype_##F_SFX *ptr, s_intp stride, s_uintp nlane, v_##F_SFX a)          \
    {                                                                                       \
        v_storen_till_##T_SFX(                                                           \
            (v_lanetype_##T_SFX *)ptr, stride, nlane,                                    \
            v_reinterpret_##T_SFX##_##F_SFX(a)                                           \
        );                                                                                  \
    }

V_IMPL_AVX512_REST_PARTIAL_TYPES(u32, s32)
V_IMPL_AVX512_REST_PARTIAL_TYPES(f32, s32)
V_IMPL_AVX512_REST_PARTIAL_TYPES(u64, s64)
V_IMPL_AVX512_REST_PARTIAL_TYPES(f64, s64)

#endif // _V_SIMD_AVX512_MEMORY_H
