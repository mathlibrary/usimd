#ifndef NPY_SIMD
    #error "Not a standalone header"
#endif

#ifndef _NPY_SIMD_VSX_MEMORY_H
#define _NPY_SIMD_VSX_MEMORY_H

#include "misc.h"

/****************************
 * Private utilities
 ****************************/
// TODO: test load by cast
#define VSX__CAST_lOAD 0
#if VSX__CAST_lOAD
    #define v__load(T_VEC, PTR) (*((T_VEC*)(PTR)))
#else
    /**
     * CLANG fails to load unaligned addresses via vec_xl, vec_xst
     * so we failback to vec_vsx_ld, vec_vsx_st
     */
    #if (defined(__GNUC__) && !defined(vec_xl)) || (defined(__clang__) && !defined(__IBMC__))
        #define v__load(T_VEC, PTR) vec_vsx_ld(0, PTR)
    #else
        #define v__load(T_VEC, PTR) vec_xl(0, PTR)
    #endif
#endif
// unaligned store
#if (defined(__GNUC__) && !defined(vec_xl)) || (defined(__clang__) && !defined(__IBMC__))
    #define v__store(PTR, VEC) vec_vsx_st(VEC, 0, PTR)
#else
    #define v__store(PTR, VEC) vec_xst(VEC, 0, PTR)
#endif

// avoid aliasing rules
#ifdef __cplusplus
    template<typename T_PTR>
    NPY_FINLINE npy_uint64 *v__ptr2u64(const T_PTR *ptr)
    { npy_uint64 *ptr64 = (npy_uint64*)ptr; return ptr64; }
#else
    NPY_FINLINE npy_uint64 *v__ptr2u64(const void *ptr)
    { npy_uint64 *ptr64 = (npy_uint64*)ptr; return ptr64; }
#endif // __cplusplus

// load lower part
NPY_FINLINE v_u64 v__loadl(const void *ptr)
{
    #if defined(__clang__) && !defined(__IBMC__)
        // vec_promote doesn't support doubleword on clang
        return v_setall_u64(*v__ptr2u64(ptr));
    #else
        return vec_promote(*v__ptr2u64(ptr), 0);
    #endif
}
// store lower part
#define v__storel(PTR, VEC) \
    *v__ptr2u64(PTR) = vec_extract(((v_u64)VEC), 0)

#define v__storeh(PTR, VEC) \
    *v__ptr2u64(PTR) = vec_extract(((v_u64)VEC), 1)

/****************************
 * load/store
 ****************************/
#define NPYV_IMPL_VSX_MEM(SFX, DW_CAST)                                                 \
    NPY_FINLINE v_##SFX v_load_##SFX(const v_lanetype_##SFX *ptr)              \
    { return (v_##SFX)v__load(v_##SFX, (const v_lanetype_##DW_CAST*)ptr); } \
    NPY_FINLINE v_##SFX v_loada_##SFX(const v_lanetype_##SFX *ptr)             \
    { return (v_##SFX)vec_ld(0, (const v_lanetype_u32*)ptr); }                    \
    NPY_FINLINE v_##SFX v_loads_##SFX(const v_lanetype_##SFX *ptr)             \
    { return v_loada_##SFX(ptr); }                                                   \
    NPY_FINLINE v_##SFX v_loadl_##SFX(const v_lanetype_##SFX *ptr)             \
    { return (v_##SFX)v__loadl(ptr); }                                            \
    NPY_FINLINE void v_store_##SFX(v_lanetype_##SFX *ptr, v_##SFX vec)         \
    { v__store((v_lanetype_##DW_CAST*)ptr, (v_##DW_CAST)vec); }                \
    NPY_FINLINE void v_storea_##SFX(v_lanetype_##SFX *ptr, v_##SFX vec)        \
    { vec_st((v_u32)vec, 0, (v_lanetype_u32*)ptr); }                              \
    NPY_FINLINE void v_stores_##SFX(v_lanetype_##SFX *ptr, v_##SFX vec)        \
    { v_storea_##SFX(ptr, vec); }                                                    \
    NPY_FINLINE void v_storel_##SFX(v_lanetype_##SFX *ptr, v_##SFX vec)        \
    { v__storel(ptr, vec); }                                                         \
    NPY_FINLINE void v_storeh_##SFX(v_lanetype_##SFX *ptr, v_##SFX vec)        \
    { v__storeh(ptr, vec); }

NPYV_IMPL_VSX_MEM(u8,  u8)
NPYV_IMPL_VSX_MEM(s8,  s8)
NPYV_IMPL_VSX_MEM(u16, u16)
NPYV_IMPL_VSX_MEM(s16, s16)
NPYV_IMPL_VSX_MEM(u32, u32)
NPYV_IMPL_VSX_MEM(s32, s32)
NPYV_IMPL_VSX_MEM(u64, f64)
NPYV_IMPL_VSX_MEM(s64, f64)
NPYV_IMPL_VSX_MEM(f32, f32)
NPYV_IMPL_VSX_MEM(f64, f64)

/***************************
 * Non-contiguous Load
 ***************************/
//// 32
NPY_FINLINE v_u32 v_loadn_u32(const npy_uint32 *ptr, npy_intp stride)
{
    return v_set_u32(
        ptr[stride * 0], ptr[stride * 1],
        ptr[stride * 2], ptr[stride * 3]
    );
}
NPY_FINLINE v_s32 v_loadn_s32(const npy_int32 *ptr, npy_intp stride)
{ return (v_s32)v_loadn_u32((const npy_uint32*)ptr, stride); }
NPY_FINLINE v_f32 v_loadn_f32(const float *ptr, npy_intp stride)
{ return (v_f32)v_loadn_u32((const npy_uint32*)ptr, stride); }
//// 64
NPY_FINLINE v_u64 v_loadn_u64(const npy_uint64 *ptr, npy_intp stride)
{ return v_set_u64(ptr[0], ptr[stride]); }
NPY_FINLINE v_s64 v_loadn_s64(const npy_int64 *ptr, npy_intp stride)
{ return v_set_s64(ptr[0], ptr[stride]); }
NPY_FINLINE v_f64 v_loadn_f64(const double *ptr, npy_intp stride)
{ return v_set_f64(ptr[0], ptr[stride]); }
/***************************
 * Non-contiguous Store
 ***************************/
//// 32
NPY_FINLINE void v_storen_u32(npy_uint32 *ptr, npy_intp stride, v_u32 a)
{
    ptr[stride * 0] = vec_extract(a, 0);
    ptr[stride * 1] = vec_extract(a, 1);
    ptr[stride * 2] = vec_extract(a, 2);
    ptr[stride * 3] = vec_extract(a, 3);
}
NPY_FINLINE void v_storen_s32(npy_int32 *ptr, npy_intp stride, v_s32 a)
{ v_storen_u32((npy_uint32*)ptr, stride, (v_u32)a); }
NPY_FINLINE void v_storen_f32(float *ptr, npy_intp stride, v_f32 a)
{ v_storen_u32((npy_uint32*)ptr, stride, (v_u32)a); }
//// 64
NPY_FINLINE void v_storen_u64(npy_uint64 *ptr, npy_intp stride, v_u64 a)
{
    ptr[stride * 0] = vec_extract(a, 0);
    ptr[stride * 1] = vec_extract(a, 1);
}
NPY_FINLINE void v_storen_s64(npy_int64 *ptr, npy_intp stride, v_s64 a)
{ v_storen_u64((npy_uint64*)ptr, stride, (v_u64)a); }
NPY_FINLINE void v_storen_f64(double *ptr, npy_intp stride, v_f64 a)
{ v_storen_u64((npy_uint64*)ptr, stride, (v_u64)a); }

/*********************************
 * Partial Load
 *********************************/
//// 32
NPY_FINLINE v_s32 v_load_till_s32(const npy_int32 *ptr, npy_uintp nlane, npy_int32 fill)
{
    assert(nlane > 0);
    v_s32 vfill = v_setall_s32(fill);
    switch(nlane) {
    case 1:
        return vec_insert(ptr[0], vfill, 0);
    case 2:
        return (v_s32)vec_insert(
            *v__ptr2u64(ptr), (v_u64)vfill, 0
        );
    case 3:
        vfill = vec_insert(ptr[2], vfill, 2);
        return (v_s32)vec_insert(
            *v__ptr2u64(ptr), (v_u64)vfill, 0
        );
    default:
        return v_load_s32(ptr);
    }
}
// fill zero to rest lanes
NPY_FINLINE v_s32 v_load_tillz_s32(const npy_int32 *ptr, npy_uintp nlane)
{ return v_load_till_s32(ptr, nlane, 0); }
//// 64
NPY_FINLINE v_s64 v_load_till_s64(const npy_int64 *ptr, npy_uintp nlane, npy_int64 fill)
{
    assert(nlane > 0);
    if (nlane == 1) {
        return v_set_s64(ptr[0], fill);
    }
    return v_load_s64(ptr);
}
// fill zero to rest lanes
NPY_FINLINE v_s64 v_load_tillz_s64(const npy_int64 *ptr, npy_uintp nlane)
{  return v_load_till_s64(ptr, nlane, 0); }
/*********************************
 * Non-contiguous partial load
 *********************************/
//// 32
NPY_FINLINE v_s32
v_loadn_till_s32(const npy_int32 *ptr, npy_intp stride, npy_uintp nlane, npy_int32 fill)
{
    assert(nlane > 0);
    v_s32 vfill = v_setall_s32(fill);
    switch(nlane) {
    case 3:
        vfill = vec_insert(ptr[stride*2], vfill, 2);
    case 2:
        vfill = vec_insert(ptr[stride], vfill, 1);
    case 1:
        vfill = vec_insert(*ptr, vfill, 0);
        break;
    default:
        return v_loadn_s32(ptr, stride);
    } // switch
    return vfill;
}
// fill zero to rest lanes
NPY_FINLINE v_s32
v_loadn_tillz_s32(const npy_int32 *ptr, npy_intp stride, npy_uintp nlane)
{ return v_loadn_till_s32(ptr, stride, nlane, 0); }
//// 64
NPY_FINLINE v_s64
v_loadn_till_s64(const npy_int64 *ptr, npy_intp stride, npy_uintp nlane, npy_int64 fill)
{
    assert(nlane > 0);
    if (nlane == 1) {
        return v_set_s64(*ptr, fill);
    }
    return v_loadn_s64(ptr, stride);
}
// fill zero to rest lanes
NPY_FINLINE v_s64 v_loadn_tillz_s64(const npy_int64 *ptr, npy_intp stride, npy_uintp nlane)
{ return v_loadn_till_s64(ptr, stride, nlane, 0); }
/*********************************
 * Partial store
 *********************************/
//// 32
NPY_FINLINE void v_store_till_s32(npy_int32 *ptr, npy_uintp nlane, v_s32 a)
{
    assert(nlane > 0);
    switch(nlane) {
    case 1:
        *ptr = vec_extract(a, 0);
        break;
    case 2:
        v_storel_s32(ptr, a);
        break;
    case 3:
        v_storel_s32(ptr, a);
        ptr[2] = vec_extract(a, 2);
        break;
    default:
        v_store_s32(ptr, a);
    }
}
//// 64
NPY_FINLINE void v_store_till_s64(npy_int64 *ptr, npy_uintp nlane, v_s64 a)
{
    assert(nlane > 0);
    if (nlane == 1) {
        v_storel_s64(ptr, a);
        return;
    }
    v_store_s64(ptr, a);
}
/*********************************
 * Non-contiguous partial store
 *********************************/
//// 32
NPY_FINLINE void v_storen_till_s32(npy_int32 *ptr, npy_intp stride, npy_uintp nlane, v_s32 a)
{
    assert(nlane > 0);
    switch(nlane) {
    default:
        ptr[stride*3] = vec_extract(a, 3);
    case 3:
        ptr[stride*2] = vec_extract(a, 2);
    case 2:
        ptr[stride*1] = vec_extract(a, 1);
    case 1:
        ptr[stride*0] = vec_extract(a, 0);
        break;
    }
}
//// 64
NPY_FINLINE void v_storen_till_s64(npy_int64 *ptr, npy_intp stride, npy_uintp nlane, v_s64 a)
{
    assert(nlane > 0);
    if (nlane == 1) {
        v_storel_s64(ptr, a);
        return;
    }
    v_storen_s64(ptr, stride, a);
}
/*****************************************************************
 * Implement partial load/store for u32/f32/u64/f64... via casting
 *****************************************************************/
#define NPYV_IMPL_VSX_REST_PARTIAL_TYPES(F_SFX, T_SFX)                                      \
    NPY_FINLINE v_##F_SFX v_load_till_##F_SFX                                         \
    (const v_lanetype_##F_SFX *ptr, npy_uintp nlane, v_lanetype_##F_SFX fill)         \
    {                                                                                       \
        union {                                                                             \
            v_lanetype_##F_SFX from_##F_SFX;                                             \
            v_lanetype_##T_SFX to_##T_SFX;                                               \
        } pun = {.from_##F_SFX = fill};                                                     \
        return v_reinterpret_##F_SFX##_##T_SFX(v_load_till_##T_SFX(                   \
            (const v_lanetype_##T_SFX *)ptr, nlane, pun.to_##T_SFX                       \
        ));                                                                                 \
    }                                                                                       \
    NPY_FINLINE v_##F_SFX v_loadn_till_##F_SFX                                        \
    (const v_lanetype_##F_SFX *ptr, npy_intp stride, npy_uintp nlane,                    \
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
    NPY_FINLINE v_##F_SFX v_load_tillz_##F_SFX                                        \
    (const v_lanetype_##F_SFX *ptr, npy_uintp nlane)                                     \
    {                                                                                       \
        return v_reinterpret_##F_SFX##_##T_SFX(v_load_tillz_##T_SFX(                  \
            (const v_lanetype_##T_SFX *)ptr, nlane                                       \
        ));                                                                                 \
    }                                                                                       \
    NPY_FINLINE v_##F_SFX v_loadn_tillz_##F_SFX                                       \
    (const v_lanetype_##F_SFX *ptr, npy_intp stride, npy_uintp nlane)                    \
    {                                                                                       \
        return v_reinterpret_##F_SFX##_##T_SFX(v_loadn_tillz_##T_SFX(                 \
            (const v_lanetype_##T_SFX *)ptr, stride, nlane                               \
        ));                                                                                 \
    }                                                                                       \
    NPY_FINLINE void v_store_till_##F_SFX                                                \
    (v_lanetype_##F_SFX *ptr, npy_uintp nlane, v_##F_SFX a)                           \
    {                                                                                       \
        v_store_till_##T_SFX(                                                            \
            (v_lanetype_##T_SFX *)ptr, nlane,                                            \
            v_reinterpret_##T_SFX##_##F_SFX(a)                                           \
        );                                                                                  \
    }                                                                                       \
    NPY_FINLINE void v_storen_till_##F_SFX                                               \
    (v_lanetype_##F_SFX *ptr, npy_intp stride, npy_uintp nlane, v_##F_SFX a)          \
    {                                                                                       \
        v_storen_till_##T_SFX(                                                           \
            (v_lanetype_##T_SFX *)ptr, stride, nlane,                                    \
            v_reinterpret_##T_SFX##_##F_SFX(a)                                           \
        );                                                                                  \
    }

NPYV_IMPL_VSX_REST_PARTIAL_TYPES(u32, s32)
NPYV_IMPL_VSX_REST_PARTIAL_TYPES(f32, s32)
NPYV_IMPL_VSX_REST_PARTIAL_TYPES(u64, s64)
NPYV_IMPL_VSX_REST_PARTIAL_TYPES(f64, s64)

#endif // _NPY_SIMD_VSX_MEMORY_H
