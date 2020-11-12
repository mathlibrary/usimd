#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_SSE_MEMORY_H
#define _V_SIMD_SSE_MEMORY_H

#include "misc.h"

/***************************
 * load/store
 ***************************/
// stream load
#ifdef V_HAVE_SSE41
    #define v__loads(PTR) _mm_stream_load_si128((__m128i *)(PTR))
#else
    #define v__loads(PTR) _mm_load_si128((const __m128i *)(PTR))
#endif
#define NPYV_IMPL_SSE_MEM_INT(CTYPE, SFX)                                    \
    V_FINLINE v_##SFX v_load_##SFX(const CTYPE *ptr)                 \
    { return _mm_loadu_si128((const __m128i*)ptr); }                         \
    V_FINLINE v_##SFX v_loada_##SFX(const CTYPE *ptr)                \
    { return _mm_load_si128((const __m128i*)ptr); }                          \
    V_FINLINE v_##SFX v_loads_##SFX(const CTYPE *ptr)                \
    { return v__loads(ptr); }                                             \
    V_FINLINE v_##SFX v_loadl_##SFX(const CTYPE *ptr)                \
    { return _mm_loadl_epi64((const __m128i*)ptr); }                         \
    V_FINLINE void v_store_##SFX(CTYPE *ptr, v_##SFX vec)            \
    { _mm_storeu_si128((__m128i*)ptr, vec); }                                \
    V_FINLINE void v_storea_##SFX(CTYPE *ptr, v_##SFX vec)           \
    { _mm_store_si128((__m128i*)ptr, vec); }                                 \
    V_FINLINE void v_stores_##SFX(CTYPE *ptr, v_##SFX vec)           \
    { _mm_stream_si128((__m128i*)ptr, vec); }                                \
    V_FINLINE void v_storel_##SFX(CTYPE *ptr, v_##SFX vec)           \
    { _mm_storel_epi64((__m128i *)ptr, vec); }                               \
    V_FINLINE void v_storeh_##SFX(CTYPE *ptr, v_##SFX vec)           \
    { _mm_storel_epi64((__m128i *)ptr, _mm_unpackhi_epi64(vec, vec)); }

NPYV_IMPL_SSE_MEM_INT(s_uint8,  u8)
NPYV_IMPL_SSE_MEM_INT(s_int8,   s8)
NPYV_IMPL_SSE_MEM_INT(s_uint16, u16)
NPYV_IMPL_SSE_MEM_INT(s_int16,  s16)
NPYV_IMPL_SSE_MEM_INT(s_uint32, u32)
NPYV_IMPL_SSE_MEM_INT(s_int32,  s32)
NPYV_IMPL_SSE_MEM_INT(s_uint64, u64)
NPYV_IMPL_SSE_MEM_INT(s_int64,  s64)

// unaligned load
#define v_load_f32 _mm_loadu_ps
#define v_load_f64 _mm_loadu_pd
// aligned load
#define v_loada_f32 _mm_load_ps
#define v_loada_f64 _mm_load_pd
// load lower part
#define v_loadl_f32(PTR) _mm_castsi128_ps(v_loadl_u32((const s_uint32*)(PTR)))
#define v_loadl_f64(PTR) _mm_castsi128_pd(v_loadl_u32((const s_uint32*)(PTR)))
// stream load
#define v_loads_f32(PTR) _mm_castsi128_ps(v__loads(PTR))
#define v_loads_f64(PTR) _mm_castsi128_pd(v__loads(PTR))
// unaligned store
#define v_store_f32 _mm_storeu_ps
#define v_store_f64 _mm_storeu_pd
// aligned store
#define v_storea_f32 _mm_store_ps
#define v_storea_f64 _mm_store_pd
// stream store
#define v_stores_f32 _mm_stream_ps
#define v_stores_f64 _mm_stream_pd
// store lower part
#define v_storel_f32(PTR, VEC) _mm_storel_epi64((__m128i*)(PTR), _mm_castps_si128(VEC));
#define v_storel_f64(PTR, VEC) _mm_storel_epi64((__m128i*)(PTR), _mm_castpd_si128(VEC));
// store higher part
#define v_storeh_f32(PTR, VEC) v_storeh_u32((s_uint32*)(PTR), _mm_castps_si128(VEC))
#define v_storeh_f64(PTR, VEC) v_storeh_u32((s_uint32*)(PTR), _mm_castpd_si128(VEC))
/***************************
 * Non-contiguous Load
 ***************************/
//// 32
V_FINLINE v_s32 v_loadn_s32(const s_int32 *ptr, s_intp stride)
{
    __m128i a = _mm_cvtsi32_si128(*ptr);
#ifdef V_HAVE_SSE41
    a = _mm_insert_epi32(a, ptr[stride],   1);
    a = _mm_insert_epi32(a, ptr[stride*2], 2);
    a = _mm_insert_epi32(a, ptr[stride*3], 3);
#else
    __m128i a1 = _mm_cvtsi32_si128(ptr[stride]);
    __m128i a2 = _mm_cvtsi32_si128(ptr[stride*2]);
    __m128i a3 = _mm_cvtsi32_si128(ptr[stride*3]);
    a = _mm_unpacklo_epi32(a, a1);
    a = _mm_unpacklo_epi64(a, _mm_unpacklo_epi32(a2, a3));
#endif
    return a;
}
V_FINLINE v_u32 v_loadn_u32(const s_uint32 *ptr, s_intp stride)
{ return v_loadn_s32((const s_int32*)ptr, stride); }
V_FINLINE v_f32 v_loadn_f32(const float *ptr, s_intp stride)
{ return _mm_castsi128_ps(v_loadn_s32((const s_int32*)ptr, stride)); }
//// 64
V_FINLINE v_f64 v_loadn_f64(const double *ptr, s_intp stride)
{ return _mm_loadh_pd(v_loadl_f64(ptr), ptr + stride); }
V_FINLINE v_u64 v_loadn_u64(const s_uint64 *ptr, s_intp stride)
{ return _mm_castpd_si128(v_loadn_f64((const double*)ptr, stride)); }
V_FINLINE v_s64 v_loadn_s64(const s_int64 *ptr, s_intp stride)
{ return _mm_castpd_si128(v_loadn_f64((const double*)ptr, stride)); }
/***************************
 * Non-contiguous Store
 ***************************/
//// 32
V_FINLINE void v_storen_s32(s_int32 *ptr, s_intp stride, v_s32 a)
{
    ptr[stride * 0] = _mm_cvtsi128_si32(a);
#ifdef V_HAVE_SSE41
    ptr[stride * 1] = _mm_extract_epi32(a, 1);
    ptr[stride * 2] = _mm_extract_epi32(a, 2);
    ptr[stride * 3] = _mm_extract_epi32(a, 3);
#else
    ptr[stride * 1] = _mm_cvtsi128_si32(_mm_shuffle_epi32(a, _MM_SHUFFLE(0, 0, 0, 1)));
    ptr[stride * 2] = _mm_cvtsi128_si32(_mm_shuffle_epi32(a, _MM_SHUFFLE(0, 0, 0, 2)));
    ptr[stride * 3] = _mm_cvtsi128_si32(_mm_shuffle_epi32(a, _MM_SHUFFLE(0, 0, 0, 3)));
#endif
}
V_FINLINE void v_storen_u32(s_uint32 *ptr, s_intp stride, v_u32 a)
{ v_storen_s32((s_int32*)ptr, stride, a); }
V_FINLINE void v_storen_f32(float *ptr, s_intp stride, v_f32 a)
{ v_storen_s32((s_int32*)ptr, stride, _mm_castps_si128(a)); }
//// 64
V_FINLINE void v_storen_f64(double *ptr, s_intp stride, v_f64 a)
{
    _mm_storel_pd(ptr, a);
    _mm_storeh_pd(ptr + stride, a);
}
V_FINLINE void v_storen_u64(s_uint64 *ptr, s_intp stride, v_u64 a)
{ v_storen_f64((double*)ptr, stride, _mm_castsi128_pd(a)); }
V_FINLINE void v_storen_s64(s_int64 *ptr, s_intp stride, v_s64 a)
{ v_storen_f64((double*)ptr, stride, _mm_castsi128_pd(a)); }

/*********************************
 * Partial Load
 *********************************/
#if defined(__clang__) && __clang_major__ > 7
    /**
     * Clang >=8 perform aggressive optimization that tends to
     * zero the bits of upper half part of vectors even
     * when we try to fill it up with certain scalars,
     * which my lead to zero division errors.
    */
    #define NPYV__CLANG_ZEROUPPER
#endif
//// 32
V_FINLINE v_s32 v_load_till_s32(const s_int32 *ptr, s_uintp nlane, s_int32 fill)
{
    assert(nlane > 0);
#ifdef NPYV__CLANG_ZEROUPPER
    if (nlane > 3) {
        return v_load_s32(ptr);
    }
    s_int32 V_DECL_ALIGNED(16) data[4] = {fill, fill, fill, fill};
    for (s_uint64 i = 0; i < nlane; ++i) {
        data[i] = ptr[i];
    }
    return v_loada_s32(data);
#else
    #ifndef V_HAVE_SSE41
        const short *wptr = (const short*)ptr;
    #endif
    const __m128i vfill = v_setall_s32(fill);
    __m128i a;
    switch(nlane) {
    case 2:
        return _mm_castpd_si128(
            _mm_loadl_pd(_mm_castsi128_pd(vfill), (double*)ptr)
        );
    #ifdef V_HAVE_SSE41
        case 1:
            return _mm_insert_epi32(vfill, ptr[0], 0);
        case 3:
            a = _mm_loadl_epi64((const __m128i*)ptr);
            a = _mm_insert_epi32(a, ptr[2], 2);
            a = _mm_insert_epi32(a, fill, 3);
            return a;
    #else
        case 1:
            a = _mm_insert_epi16(vfill, wptr[0], 0);
            return _mm_insert_epi16(a, wptr[1], 1);
        case 3:
            a = _mm_loadl_epi64((const __m128i*)ptr);
            a = _mm_unpacklo_epi64(a, vfill);
            a = _mm_insert_epi16(a, wptr[4], 4);
            a = _mm_insert_epi16(a, wptr[5], 5);
            return a;
    #endif // V_HAVE_SSE41
        default:
            return v_load_s32(ptr);
        }
#endif
}
// fill zero to rest lanes
V_FINLINE v_s32 v_load_tillz_s32(const s_int32 *ptr, s_uintp nlane)
{
    assert(nlane > 0);
    switch(nlane) {
    case 1:
        return _mm_cvtsi32_si128(*ptr);
    case 2:
        return _mm_loadl_epi64((const __m128i*)ptr);
    case 3:;
        v_s32 a = _mm_loadl_epi64((const __m128i*)ptr);
    #ifdef V_HAVE_SSE41
        return _mm_insert_epi32(a, ptr[2], 2);
    #else
        return _mm_unpacklo_epi64(a, _mm_cvtsi32_si128(ptr[2]));
    #endif
    default:
        return v_load_s32(ptr);
    }
}
//// 64
V_FINLINE v_s64 v_load_till_s64(const s_int64 *ptr, s_uintp nlane, s_int64 fill)
{
    assert(nlane > 0);
#ifdef NPYV__CLANG_ZEROUPPER
    if (nlane <= 2) {
        s_int64 V_DECL_ALIGNED(16) data[2] = {fill, fill};
        for (s_uint64 i = 0; i < nlane; ++i) {
            data[i] = ptr[i];
        }
        return v_loada_s64(data);
    }
#else
    if (nlane == 1) {
        const __m128i vfill = v_setall_s64(fill);
        return _mm_castpd_si128(
            _mm_loadl_pd(_mm_castsi128_pd(vfill), (double*)ptr)
        );
    }
#endif
    return v_load_s64(ptr);
}
// fill zero to rest lanes
V_FINLINE v_s64 v_load_tillz_s64(const s_int64 *ptr, s_uintp nlane)
{
    assert(nlane > 0);
    if (nlane == 1) {
        return _mm_loadl_epi64((const __m128i*)ptr);
    }
    return v_load_s64(ptr);
}
/*********************************
 * Non-contiguous partial load
 *********************************/
//// 32
V_FINLINE v_s32
v_loadn_till_s32(const s_int32 *ptr, s_intp stride, s_uintp nlane, s_int32 fill)
{
    assert(nlane > 0);
#ifdef NPYV__CLANG_ZEROUPPER
    if (nlane > 3) {
        return v_loadn_s32(ptr, stride);
    }
    s_int32 V_DECL_ALIGNED(16) data[4] = {fill, fill, fill, fill};
    for (s_uint64 i = 0; i < nlane; ++i) {
        data[i] = ptr[stride*i];
    }
    return v_loada_s32(data);
#else
    __m128i vfill = v_setall_s32(fill);
    #ifndef V_HAVE_SSE41
        const short *wptr = (const short*)ptr;
    #endif
    switch(nlane) {
    #ifdef V_HAVE_SSE41
        case 3:
            vfill = _mm_insert_epi32(vfill, ptr[stride*2], 2);
        case 2:
            vfill = _mm_insert_epi32(vfill, ptr[stride], 1);
        case 1:
            vfill = _mm_insert_epi32(vfill, ptr[0], 0);
            break;
    #else
        case 3:
            vfill = _mm_unpacklo_epi32(_mm_cvtsi32_si128(ptr[stride*2]), vfill);
        case 2:
            vfill = _mm_unpacklo_epi64(_mm_unpacklo_epi32(
                _mm_cvtsi32_si128(*ptr), _mm_cvtsi32_si128(ptr[stride])
            ), vfill);
            break;
        case 1:
            vfill = _mm_insert_epi16(vfill, wptr[0], 0);
            vfill = _mm_insert_epi16(vfill, wptr[1], 1);
            break;
    #endif // V_HAVE_SSE41
    default:
        return v_loadn_s32(ptr, stride);
    } // switch
    return vfill;
#endif
}
// fill zero to rest lanes
V_FINLINE v_s32
v_loadn_tillz_s32(const s_int32 *ptr, s_intp stride, s_uintp nlane)
{
    assert(nlane > 0);
    switch(nlane) {
    case 1:
        return _mm_cvtsi32_si128(ptr[0]);
    case 2:;
        v_s32 a = _mm_cvtsi32_si128(ptr[0]);
#ifdef V_HAVE_SSE41
        return _mm_insert_epi32(a, ptr[stride], 1);
#else
        return _mm_unpacklo_epi32(a, _mm_cvtsi32_si128(ptr[stride]));
#endif // V_HAVE_SSE41
    case 3:;
        a = _mm_cvtsi32_si128(ptr[0]);
#ifdef V_HAVE_SSE41
        a = _mm_insert_epi32(a, ptr[stride], 1);
        a = _mm_insert_epi32(a, ptr[stride*2], 2);
        return a;
#else
        a = _mm_unpacklo_epi32(a, _mm_cvtsi32_si128(ptr[stride]));
        a = _mm_unpacklo_epi64(a, _mm_cvtsi32_si128(ptr[stride*2]));
        return a;
#endif // V_HAVE_SSE41
    default:
        return v_loadn_s32(ptr, stride);
    }
}
//// 64
V_FINLINE v_s64
v_loadn_till_s64(const s_int64 *ptr, s_intp stride, s_uintp nlane, s_int64 fill)
{
    assert(nlane > 0);
#ifdef NPYV__CLANG_ZEROUPPER
    if (nlane <= 2) {
        s_int64 V_DECL_ALIGNED(16) data[2] = {fill, fill};
        for (s_uint64 i = 0; i < nlane; ++i) {
            data[i] = ptr[i*stride];
        }
        return v_loada_s64(data);
    }
#else
    if (nlane == 1) {
        const __m128i vfill = v_setall_s64(fill);
        return _mm_castpd_si128(
            _mm_loadl_pd(_mm_castsi128_pd(vfill), (double*)ptr)
        );
    }
#endif
    return v_loadn_s64(ptr, stride);
}
// fill zero to rest lanes
V_FINLINE v_s64 v_loadn_tillz_s64(const s_int64 *ptr, s_intp stride, s_uintp nlane)
{
    assert(nlane > 0);
    if (nlane == 1) {
        return _mm_loadl_epi64((const __m128i*)ptr);
    }
    return v_loadn_s64(ptr, stride);
}
/*********************************
 * Partial store
 *********************************/
//// 32
V_FINLINE void v_store_till_s32(s_int32 *ptr, s_uintp nlane, v_s32 a)
{
    assert(nlane > 0);
    switch(nlane) {
    case 1:
        *ptr = _mm_cvtsi128_si32(a);
        break;
    case 2:
        _mm_storel_epi64((__m128i *)ptr, a);
        break;
    case 3:
        _mm_storel_epi64((__m128i *)ptr, a);
    #ifdef V_HAVE_SSE41
        ptr[2] = _mm_extract_epi32(a, 2);
    #else
        ptr[2] = _mm_cvtsi128_si32(_mm_shuffle_epi32(a, _MM_SHUFFLE(0, 0, 0, 2)));
    #endif
        break;
    default:
        v_store_s32(ptr, a);
    }
}
//// 64
V_FINLINE void v_store_till_s64(s_int64 *ptr, s_uintp nlane, v_s64 a)
{
    assert(nlane > 0);
    if (nlane == 1) {
        _mm_storel_epi64((__m128i *)ptr, a);
        return;
    }
    v_store_s64(ptr, a);
}
/*********************************
 * Non-contiguous partial store
 *********************************/
//// 32
V_FINLINE void v_storen_till_s32(s_int32 *ptr, s_intp stride, s_uintp nlane, v_s32 a)
{
    assert(nlane > 0);
    switch(nlane) {
#ifdef V_HAVE_SSE41
    default:
        ptr[stride*3] = _mm_extract_epi32(a, 3);
    case 3:
        ptr[stride*2] = _mm_extract_epi32(a, 2);
    case 2:
        ptr[stride*1] = _mm_extract_epi32(a, 1);
#else
    default:
        ptr[stride*3] = _mm_cvtsi128_si32(_mm_shuffle_epi32(a, _MM_SHUFFLE(0, 0, 0, 3)));
    case 3:
        ptr[stride*2] = _mm_cvtsi128_si32(_mm_shuffle_epi32(a, _MM_SHUFFLE(0, 0, 0, 2)));
    case 2:
        ptr[stride*1] = _mm_cvtsi128_si32(_mm_shuffle_epi32(a, _MM_SHUFFLE(0, 0, 0, 1)));
#endif
    case 1:
        ptr[stride*0] = _mm_cvtsi128_si32(a);
        break;
    }
}
//// 64
V_FINLINE void v_storen_till_s64(s_int64 *ptr, s_intp stride, s_uintp nlane, v_s64 a)
{
    assert(nlane > 0);
    if (nlane == 1) {
        _mm_storel_epi64((__m128i *)ptr, a);
        return;
    }
    v_storen_s64(ptr, stride, a);
}
/*****************************************************************
 * Implement partial load/store for u32/f32/u64/f64... via casting
 *****************************************************************/
#define NPYV_IMPL_SSE_REST_PARTIAL_TYPES(F_SFX, T_SFX)                                      \
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

NPYV_IMPL_SSE_REST_PARTIAL_TYPES(u32, s32)
NPYV_IMPL_SSE_REST_PARTIAL_TYPES(f32, s32)
NPYV_IMPL_SSE_REST_PARTIAL_TYPES(u64, s64)
NPYV_IMPL_SSE_REST_PARTIAL_TYPES(f64, s64)

#endif // _V_SIMD_SSE_MEMORY_H
