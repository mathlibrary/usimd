#ifndef NPY_SIMD
    #error "Not a standalone header"
#endif

#ifndef _NPY_SIMD_VSX_MISC_H
#define _NPY_SIMD_VSX_MISC_H

// vector with zero lanes
#define v_zero_u8()  ((v_u8)   v_setall_s32(0))
#define v_zero_s8()  ((v_s8)   v_setall_s32(0))
#define v_zero_u16() ((v_u16)  v_setall_s32(0))
#define v_zero_s16() ((v_s16)  v_setall_s32(0))
#define v_zero_u32() v_setall_u32(0)
#define v_zero_s32() v_setall_s32(0)
#define v_zero_u64() ((v_u64) v_setall_s32(0))
#define v_zero_s64() ((v_s64) v_setall_s32(0))
#define v_zero_f32() v_setall_f32(0.0f)
#define v_zero_f64() v_setall_f64(0.0)

// vector with a specific value set to all lanes
// the safest way to generate vsplti* and vsplt* instructions
#define NPYV_IMPL_VSX_SPLTB(T_VEC, V) ((T_VEC){V, V, V, V, V, V, V, V, V, V, V, V, V, V, V, V})
#define NPYV_IMPL_VSX_SPLTH(T_VEC, V) ((T_VEC){V, V, V, V, V, V, V, V})
#define NPYV_IMPL_VSX_SPLTW(T_VEC, V) ((T_VEC){V, V, V, V})
#define NPYV_IMPL_VSX_SPLTD(T_VEC, V) ((T_VEC){V, V})

#define v_setall_u8(VAL)  NPYV_IMPL_VSX_SPLTB(v_u8,  (unsigned char)VAL)
#define v_setall_s8(VAL)  NPYV_IMPL_VSX_SPLTB(v_s8,  (signed char)VAL)
#define v_setall_u16(VAL) NPYV_IMPL_VSX_SPLTH(v_u16, (unsigned short)VAL)
#define v_setall_s16(VAL) NPYV_IMPL_VSX_SPLTH(v_s16, (short)VAL)
#define v_setall_u32(VAL) NPYV_IMPL_VSX_SPLTW(v_u32, (unsigned int)VAL)
#define v_setall_s32(VAL) NPYV_IMPL_VSX_SPLTW(v_s32, (int)VAL)
#define v_setall_f32(VAL) NPYV_IMPL_VSX_SPLTW(v_f32, VAL)
#define v_setall_u64(VAL) NPYV_IMPL_VSX_SPLTD(v_u64, (npy_uint64)VAL)
#define v_setall_s64(VAL) NPYV_IMPL_VSX_SPLTD(v_s64, (npy_int64)VAL)
#define v_setall_f64(VAL) NPYV_IMPL_VSX_SPLTD(v_f64, VAL)

// vector with specific values set to each lane and
// set a specific value to all remained lanes
#define v_setf_u8(FILL, ...)  ((v_u8){NPYV__SET_FILL_16(char, FILL, __VA_ARGS__)})
#define v_setf_s8(FILL, ...)  ((v_s8){NPYV__SET_FILL_16(char, FILL, __VA_ARGS__)})
#define v_setf_u16(FILL, ...) ((v_u16){NPYV__SET_FILL_8(short, FILL, __VA_ARGS__)})
#define v_setf_s16(FILL, ...) ((v_s16){NPYV__SET_FILL_8(short, FILL, __VA_ARGS__)})
#define v_setf_u32(FILL, ...) ((v_u32){NPYV__SET_FILL_4(int, FILL, __VA_ARGS__)})
#define v_setf_s32(FILL, ...) ((v_s32){NPYV__SET_FILL_4(int, FILL, __VA_ARGS__)})
#define v_setf_u64(FILL, ...) ((v_u64){NPYV__SET_FILL_2(npy_int64, FILL, __VA_ARGS__)})
#define v_setf_s64(FILL, ...) ((v_s64){NPYV__SET_FILL_2(npy_int64, FILL, __VA_ARGS__)})
#define v_setf_f32(FILL, ...) ((v_f32){NPYV__SET_FILL_4(float, FILL, __VA_ARGS__)})
#define v_setf_f64(FILL, ...) ((v_f64){NPYV__SET_FILL_2(double, FILL, __VA_ARGS__)})

// vector with specific values set to each lane and
// set zero to all remained lanes
#define v_set_u8(...)  v_setf_u8(0,  __VA_ARGS__)
#define v_set_s8(...)  v_setf_s8(0,  __VA_ARGS__)
#define v_set_u16(...) v_setf_u16(0, __VA_ARGS__)
#define v_set_s16(...) v_setf_s16(0, __VA_ARGS__)
#define v_set_u32(...) v_setf_u32(0, __VA_ARGS__)
#define v_set_s32(...) v_setf_s32(0, __VA_ARGS__)
#define v_set_u64(...) v_setf_u64(0, __VA_ARGS__)
#define v_set_s64(...) v_setf_s64(0, __VA_ARGS__)
#define v_set_f32(...) v_setf_f32(0, __VA_ARGS__)
#define v_set_f64(...) v_setf_f64(0, __VA_ARGS__)

// Per lane select
#define v_select_u8(MASK, A, B) vec_sel(B, A, MASK)
#define v_select_s8  v_select_u8
#define v_select_u16 v_select_u8
#define v_select_s16 v_select_u8
#define v_select_u32 v_select_u8
#define v_select_s32 v_select_u8
#define v_select_u64 v_select_u8
#define v_select_s64 v_select_u8
#define v_select_f32 v_select_u8
#define v_select_f64 v_select_u8

// Reinterpret
#define v_reinterpret_u8_u8(X) X
#define v_reinterpret_u8_s8(X) ((v_u8)X)
#define v_reinterpret_u8_u16 v_reinterpret_u8_s8
#define v_reinterpret_u8_s16 v_reinterpret_u8_s8
#define v_reinterpret_u8_u32 v_reinterpret_u8_s8
#define v_reinterpret_u8_s32 v_reinterpret_u8_s8
#define v_reinterpret_u8_u64 v_reinterpret_u8_s8
#define v_reinterpret_u8_s64 v_reinterpret_u8_s8
#define v_reinterpret_u8_f32 v_reinterpret_u8_s8
#define v_reinterpret_u8_f64 v_reinterpret_u8_s8

#define v_reinterpret_s8_s8(X) X
#define v_reinterpret_s8_u8(X) ((v_s8)X)
#define v_reinterpret_s8_u16 v_reinterpret_s8_u8
#define v_reinterpret_s8_s16 v_reinterpret_s8_u8
#define v_reinterpret_s8_u32 v_reinterpret_s8_u8
#define v_reinterpret_s8_s32 v_reinterpret_s8_u8
#define v_reinterpret_s8_u64 v_reinterpret_s8_u8
#define v_reinterpret_s8_s64 v_reinterpret_s8_u8
#define v_reinterpret_s8_f32 v_reinterpret_s8_u8
#define v_reinterpret_s8_f64 v_reinterpret_s8_u8

#define v_reinterpret_u16_u16(X) X
#define v_reinterpret_u16_u8(X) ((v_u16)X)
#define v_reinterpret_u16_s8  v_reinterpret_u16_u8
#define v_reinterpret_u16_s16 v_reinterpret_u16_u8
#define v_reinterpret_u16_u32 v_reinterpret_u16_u8
#define v_reinterpret_u16_s32 v_reinterpret_u16_u8
#define v_reinterpret_u16_u64 v_reinterpret_u16_u8
#define v_reinterpret_u16_s64 v_reinterpret_u16_u8
#define v_reinterpret_u16_f32 v_reinterpret_u16_u8
#define v_reinterpret_u16_f64 v_reinterpret_u16_u8

#define v_reinterpret_s16_s16(X) X
#define v_reinterpret_s16_u8(X) ((v_s16)X)
#define v_reinterpret_s16_s8  v_reinterpret_s16_u8
#define v_reinterpret_s16_u16 v_reinterpret_s16_u8
#define v_reinterpret_s16_u32 v_reinterpret_s16_u8
#define v_reinterpret_s16_s32 v_reinterpret_s16_u8
#define v_reinterpret_s16_u64 v_reinterpret_s16_u8
#define v_reinterpret_s16_s64 v_reinterpret_s16_u8
#define v_reinterpret_s16_f32 v_reinterpret_s16_u8
#define v_reinterpret_s16_f64 v_reinterpret_s16_u8

#define v_reinterpret_u32_u32(X) X
#define v_reinterpret_u32_u8(X) ((v_u32)X)
#define v_reinterpret_u32_s8  v_reinterpret_u32_u8
#define v_reinterpret_u32_u16 v_reinterpret_u32_u8
#define v_reinterpret_u32_s16 v_reinterpret_u32_u8
#define v_reinterpret_u32_s32 v_reinterpret_u32_u8
#define v_reinterpret_u32_u64 v_reinterpret_u32_u8
#define v_reinterpret_u32_s64 v_reinterpret_u32_u8
#define v_reinterpret_u32_f32 v_reinterpret_u32_u8
#define v_reinterpret_u32_f64 v_reinterpret_u32_u8

#define v_reinterpret_s32_s32(X) X
#define v_reinterpret_s32_u8(X) ((v_s32)X)
#define v_reinterpret_s32_s8  v_reinterpret_s32_u8
#define v_reinterpret_s32_u16 v_reinterpret_s32_u8
#define v_reinterpret_s32_s16 v_reinterpret_s32_u8
#define v_reinterpret_s32_u32 v_reinterpret_s32_u8
#define v_reinterpret_s32_u64 v_reinterpret_s32_u8
#define v_reinterpret_s32_s64 v_reinterpret_s32_u8
#define v_reinterpret_s32_f32 v_reinterpret_s32_u8
#define v_reinterpret_s32_f64 v_reinterpret_s32_u8

#define v_reinterpret_u64_u64(X) X
#define v_reinterpret_u64_u8(X) ((v_u64)X)
#define v_reinterpret_u64_s8  v_reinterpret_u64_u8
#define v_reinterpret_u64_u16 v_reinterpret_u64_u8
#define v_reinterpret_u64_s16 v_reinterpret_u64_u8
#define v_reinterpret_u64_u32 v_reinterpret_u64_u8
#define v_reinterpret_u64_s32 v_reinterpret_u64_u8
#define v_reinterpret_u64_s64 v_reinterpret_u64_u8
#define v_reinterpret_u64_f32 v_reinterpret_u64_u8
#define v_reinterpret_u64_f64 v_reinterpret_u64_u8

#define v_reinterpret_s64_s64(X) X
#define v_reinterpret_s64_u8(X) ((v_s64)X)
#define v_reinterpret_s64_s8  v_reinterpret_s64_u8
#define v_reinterpret_s64_u16 v_reinterpret_s64_u8
#define v_reinterpret_s64_s16 v_reinterpret_s64_u8
#define v_reinterpret_s64_u32 v_reinterpret_s64_u8
#define v_reinterpret_s64_s32 v_reinterpret_s64_u8
#define v_reinterpret_s64_u64 v_reinterpret_s64_u8
#define v_reinterpret_s64_f32 v_reinterpret_s64_u8
#define v_reinterpret_s64_f64 v_reinterpret_s64_u8

#define v_reinterpret_f32_f32(X) X
#define v_reinterpret_f32_u8(X) ((v_f32)X)
#define v_reinterpret_f32_s8  v_reinterpret_f32_u8
#define v_reinterpret_f32_u16 v_reinterpret_f32_u8
#define v_reinterpret_f32_s16 v_reinterpret_f32_u8
#define v_reinterpret_f32_u32 v_reinterpret_f32_u8
#define v_reinterpret_f32_s32 v_reinterpret_f32_u8
#define v_reinterpret_f32_u64 v_reinterpret_f32_u8
#define v_reinterpret_f32_s64 v_reinterpret_f32_u8
#define v_reinterpret_f32_f64 v_reinterpret_f32_u8

#define v_reinterpret_f64_f64(X) X
#define v_reinterpret_f64_u8(X) ((v_f64)X)
#define v_reinterpret_f64_s8  v_reinterpret_f64_u8
#define v_reinterpret_f64_u16 v_reinterpret_f64_u8
#define v_reinterpret_f64_s16 v_reinterpret_f64_u8
#define v_reinterpret_f64_u32 v_reinterpret_f64_u8
#define v_reinterpret_f64_s32 v_reinterpret_f64_u8
#define v_reinterpret_f64_u64 v_reinterpret_f64_u8
#define v_reinterpret_f64_s64 v_reinterpret_f64_u8
#define v_reinterpret_f64_f32 v_reinterpret_f64_u8

// Only required by AVX2/AVX512
#define v_cleanup() ((void)0)

#endif // _NPY_SIMD_VSX_MISC_H
