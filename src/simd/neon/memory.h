#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_NEON_MEMORY_H
#define _V_SIMD_NEON_MEMORY_H

#include "misc.h"

/***************************
 * load/store
 ***************************/
// GCC requires literal type definitions for pointers types otherwise it causes ambiguous errors
#define V_IMPL_NEON_MEM(SFX, CTYPE)                                           \
    V_FINLINE v_##SFX v_load_##SFX(const v_lanetype_##SFX *ptr)       \
    { return vld1q_##SFX((const CTYPE*)ptr); }                                   \
    V_FINLINE v_##SFX v_loada_##SFX(const v_lanetype_##SFX *ptr)      \
    { return vld1q_##SFX((const CTYPE*)ptr); }                                   \
    V_FINLINE v_##SFX v_loads_##SFX(const v_lanetype_##SFX *ptr)      \
    { return vld1q_##SFX((const CTYPE*)ptr); }                                   \
    V_FINLINE v_##SFX v_loadl_##SFX(const v_lanetype_##SFX *ptr)      \
    {                                                                            \
        return vcombine_##SFX(                                                   \
            vld1_##SFX((const CTYPE*)ptr), vdup_n_##SFX(0)                       \
        );                                                                       \
    }                                                                            \
    V_FINLINE void v_store_##SFX(v_lanetype_##SFX *ptr, v_##SFX vec)  \
    { vst1q_##SFX((CTYPE*)ptr, vec); }                                           \
    V_FINLINE void v_storea_##SFX(v_lanetype_##SFX *ptr, v_##SFX vec) \
    { vst1q_##SFX((CTYPE*)ptr, vec); }                                           \
    V_FINLINE void v_stores_##SFX(v_lanetype_##SFX *ptr, v_##SFX vec) \
    { vst1q_##SFX((CTYPE*)ptr, vec); }                                           \
    V_FINLINE void v_storel_##SFX(v_lanetype_##SFX *ptr, v_##SFX vec) \
    { vst1_##SFX((CTYPE*)ptr, vget_low_##SFX(vec)); }                            \
    V_FINLINE void v_storeh_##SFX(v_lanetype_##SFX *ptr, v_##SFX vec) \
    { vst1_##SFX((CTYPE*)ptr, vget_high_##SFX(vec)); }

V_IMPL_NEON_MEM(u8,  uint8_t)
V_IMPL_NEON_MEM(s8,  int8_t)
V_IMPL_NEON_MEM(u16, uint16_t)
V_IMPL_NEON_MEM(s16, int16_t)
V_IMPL_NEON_MEM(u32, uint32_t)
V_IMPL_NEON_MEM(s32, int32_t)
V_IMPL_NEON_MEM(u64, uint64_t)
V_IMPL_NEON_MEM(s64, int64_t)
V_IMPL_NEON_MEM(f32, float)
#if V_SIMD_F64
V_IMPL_NEON_MEM(f64, double)
#endif
/***************************
 * Non-contiguous Load
 ***************************/
V_FINLINE v_s32 v_loadn_s32(const s_int32 *ptr, s_intp stride)
{
    switch (stride) {
    case 2:
        return vld2q_s32((const int32_t*)ptr).val[0];
    case 3:
        return vld3q_s32((const int32_t*)ptr).val[0];
    case 4:
        return vld4q_s32((const int32_t*)ptr).val[0];
    default:;
        int32x2_t ax = vcreate_s32(*ptr);
        int32x4_t a = vcombine_s32(ax, ax);
                  a = vld1q_lane_s32((const int32_t*)ptr + stride,   a, 1);
                  a = vld1q_lane_s32((const int32_t*)ptr + stride*2, a, 2);
                  a = vld1q_lane_s32((const int32_t*)ptr + stride*3, a, 3);
        return a;
    }
}

V_FINLINE v_u32 v_loadn_u32(const s_uint32 *ptr, s_intp stride)
{
    return v_reinterpret_u32_s32(
        v_loadn_s32((const s_int32*)ptr, stride)
    );
}
V_FINLINE v_f32 v_loadn_f32(const float *ptr, s_intp stride)
{
    return v_reinterpret_f32_s32(
        v_loadn_s32((const s_int32*)ptr, stride)
    );
}
//// 64
V_FINLINE v_s64 v_loadn_s64(const s_int64 *ptr, s_intp stride)
{
    return vcombine_s64(
        vld1_s64((const int64_t*)ptr), vld1_s64((const int64_t*)ptr + stride)
    );
}
V_FINLINE v_u64 v_loadn_u64(const s_uint64 *ptr, s_intp stride)
{
    return v_reinterpret_u64_s64(
        v_loadn_s64((const s_int64*)ptr, stride)
    );
}
#if V_SIMD_F64
V_FINLINE v_f64 v_loadn_f64(const double *ptr, s_intp stride)
{
    return v_reinterpret_f64_s64(
        v_loadn_s64((const s_int64*)ptr, stride)
    );
}
#endif
/***************************
 * Non-contiguous Store
 ***************************/
//// 32
V_FINLINE void v_storen_s32(s_int32 *ptr, s_intp stride, v_s32 a)
{
    vst1q_lane_s32((int32_t*)ptr, a, 0);
    vst1q_lane_s32((int32_t*)ptr + stride, a, 1);
    vst1q_lane_s32((int32_t*)ptr + stride*2, a, 2);
    vst1q_lane_s32((int32_t*)ptr + stride*3, a, 3);
}
V_FINLINE void v_storen_u32(s_uint32 *ptr, s_intp stride, v_u32 a)
{ v_storen_s32((s_int32*)ptr, stride, v_reinterpret_s32_u32(a)); }
V_FINLINE void v_storen_f32(float *ptr, s_intp stride, v_f32 a)
{ v_storen_s32((s_int32*)ptr, stride, v_reinterpret_s32_f32(a)); }
//// 64
V_FINLINE void v_storen_s64(s_int64 *ptr, s_intp stride, v_s64 a)
{
    vst1q_lane_s64((int64_t*)ptr, a, 0);
    vst1q_lane_s64((int64_t*)ptr + stride, a, 1);
}
V_FINLINE void v_storen_u64(s_uint64 *ptr, s_intp stride, v_u64 a)
{ v_storen_s64((s_int64*)ptr, stride, v_reinterpret_s64_u64(a)); }

#if V_SIMD_F64
V_FINLINE void v_storen_f64(double *ptr, s_intp stride, v_f64 a)
{ v_storen_s64((s_int64*)ptr, stride, v_reinterpret_s64_f64(a)); }
#endif

/*********************************
 * Partial Load
 *********************************/
//// 32
V_FINLINE v_s32 v_load_till_s32(const s_int32 *ptr, s_uintp nlane, s_int32 fill)
{
    assert(nlane > 0);
    switch(nlane) {
    case 1:
        return vld1q_lane_s32((const int32_t*)ptr, vdupq_n_s32(fill), 0);
    case 2:
        return vcombine_s32(vld1_s32((const int32_t*)ptr), vdup_n_s32(fill));
    case 3:
        return vcombine_s32(
            vld1_s32((const int32_t*)ptr),
            vld1_lane_s32((const int32_t*)ptr + 2, vdup_n_s32(fill), 0)
        );
    default:
        return v_load_s32(ptr);
    }
}
// fill zero to rest lanes
V_FINLINE v_s32 v_load_tillz_s32(const s_int32 *ptr, s_uintp nlane)
{ return v_load_till_s32(ptr, nlane, 0); }
//// 64
V_FINLINE v_s64 v_load_till_s64(const s_int64 *ptr, s_uintp nlane, s_int64 fill)
{
    assert(nlane > 0);
    if (nlane == 1) {
        return vcombine_s64(vld1_s64((const int64_t*)ptr), vdup_n_s64(fill));
    }
    return v_load_s64(ptr);
}
// fill zero to rest lanes
V_FINLINE v_s64 v_load_tillz_s64(const s_int64 *ptr, s_uintp nlane)
{ return v_load_till_s64(ptr, nlane, 0); }

/*********************************
 * Non-contiguous partial load
 *********************************/
//// 32
V_FINLINE v_s32
v_loadn_till_s32(const s_int32 *ptr, s_intp stride, s_uintp nlane, s_int32 fill)
{
    assert(nlane > 0);
    int32x4_t vfill = vdupq_n_s32(fill);
    switch(nlane) {
    case 3:
        vfill = vld1q_lane_s32((const int32_t*)ptr + stride*2, vfill, 2);
    case 2:
        vfill = vld1q_lane_s32((const int32_t*)ptr + stride, vfill, 1);
    case 1:
        vfill = vld1q_lane_s32((const int32_t*)ptr, vfill, 0);
        return vfill;
    default:
        return v_loadn_s32(ptr, stride);
    }
}
V_FINLINE v_s32
v_loadn_tillz_s32(const s_int32 *ptr, s_intp stride, s_uintp nlane)
{ return v_loadn_till_s32(ptr, stride, nlane, 0); }

V_FINLINE v_s64
v_loadn_till_s64(const s_int64 *ptr, s_intp stride, s_uintp nlane, s_int64 fill)
{
    assert(nlane > 0);
    if (nlane == 1) {
        return vcombine_s64(vld1_s64((const int64_t*)ptr), vdup_n_s64(fill));
    }
    return v_loadn_s64(ptr, stride);
}
// fill zero to rest lanes
V_FINLINE v_s64 v_loadn_tillz_s64(const s_int64 *ptr, s_intp stride, s_uintp nlane)
{ return v_loadn_till_s64(ptr, stride, nlane, 0); }

/*********************************
 * Partial store
 *********************************/
//// 32
V_FINLINE void v_store_till_s32(s_int32 *ptr, s_uintp nlane, v_s32 a)
{
    assert(nlane > 0);
    switch(nlane) {
    case 1:
        vst1q_lane_s32((int32_t*)ptr, a, 0);
        break;
    case 2:
        vst1_s32((int32_t*)ptr, vget_low_s32(a));
        break;
    case 3:
        vst1_s32((int32_t*)ptr, vget_low_s32(a));
        vst1q_lane_s32((int32_t*)ptr + 2, a, 2);
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
        vst1q_lane_s64((int64_t*)ptr, a, 0);
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
    default:
        vst1q_lane_s32((int32_t*)ptr + stride*3, a, 3);
    case 3:
        vst1q_lane_s32((int32_t*)ptr + stride*2, a, 2);
    case 2:
        vst1q_lane_s32((int32_t*)ptr + stride, a, 1);
    case 1:
        vst1q_lane_s32((int32_t*)ptr, a, 0);
        break;
    }
}
//// 64
V_FINLINE void v_storen_till_s64(s_int64 *ptr, s_intp stride, s_uintp nlane, v_s64 a)
{
    assert(nlane > 0);
    if (nlane == 1) {
        vst1q_lane_s64((int64_t*)ptr, a, 0);
        return;
    }
    v_storen_s64(ptr, stride, a);
}

/*****************************************************************
 * Implement partial load/store for u32/f32/u64/f64... via casting
 *****************************************************************/
#define V_IMPL_NEON_REST_PARTIAL_TYPES(F_SFX, T_SFX)                                     \
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

V_IMPL_NEON_REST_PARTIAL_TYPES(u32, s32)
V_IMPL_NEON_REST_PARTIAL_TYPES(f32, s32)
V_IMPL_NEON_REST_PARTIAL_TYPES(u64, s64)
#if V_SIMD_F64
V_IMPL_NEON_REST_PARTIAL_TYPES(f64, s64)
#endif

#endif // _V_SIMD_NEON_MEMORY_H
