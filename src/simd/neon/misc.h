#ifndef V_SIMD
    #error "Not a standalone header"
#endif

#ifndef _V_SIMD_NEON_MISC_H
#define _V_SIMD_NEON_MISC_H

// vector with zero lanes
#define v_zero_u8()  vreinterpretq_u8_s32(v_zero_s32())
#define v_zero_s8()  vreinterpretq_s8_s32(v_zero_s32())
#define v_zero_u16() vreinterpretq_u16_s32(v_zero_s32())
#define v_zero_s16() vreinterpretq_s16_s32(v_zero_s32())
#define v_zero_u32() vdupq_n_u32((unsigned)0)
#define v_zero_s32() vdupq_n_s32((int)0)
#define v_zero_u64() vreinterpretq_u64_s32(v_zero_s32())
#define v_zero_s64() vreinterpretq_s64_s32(v_zero_s32())
#define v_zero_f32() vdupq_n_f32(0.0f)
#define v_zero_f64() vdupq_n_f64(0.0)

// vector with a specific value set to all lanes
#define v_setall_u8  vdupq_n_u8
#define v_setall_s8  vdupq_n_s8
#define v_setall_u16 vdupq_n_u16
#define v_setall_s16 vdupq_n_s16
#define v_setall_u32 vdupq_n_u32
#define v_setall_s32 vdupq_n_s32
#define v_setall_u64 vdupq_n_u64
#define v_setall_s64 vdupq_n_s64
#define v_setall_f32 vdupq_n_f32
#define v_setall_f64 vdupq_n_f64

// vector with specific values set to each lane and
// set a specific value to all remained lanes
V_FINLINE uint8x16_t v__set_u8(s_uint8 i0, s_uint8 i1, s_uint8 i2, s_uint8 i3,
    s_uint8 i4, s_uint8 i5, s_uint8 i6, s_uint8 i7, s_uint8 i8, s_uint8 i9,
    s_uint8 i10, s_uint8 i11, s_uint8 i12, s_uint8 i13, s_uint8 i14, s_uint8 i15)
{
    const uint8_t V_DECL_ALIGNED(16) data[16] = {
        i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15
    };
    return vld1q_u8(data);
}
#define v_setf_u8(FILL, ...)  v__set_u8(V__SET_FILL_16(s_uint8, FILL, __VA_ARGS__))

V_FINLINE int8x16_t v__set_s8(s_int8 i0, s_int8 i1, s_int8 i2, s_int8 i3,
    s_int8 i4, s_int8 i5, s_int8 i6, s_int8 i7, s_int8 i8, s_int8 i9,
    s_int8 i10, s_int8 i11, s_int8 i12, s_int8 i13, s_int8 i14, s_int8 i15)
{
    const int8_t V_DECL_ALIGNED(16) data[16] = {
        i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15
    };
    return vld1q_s8(data);
}
#define v_setf_s8(FILL, ...)  v__set_s8(V__SET_FILL_16(s_int8, FILL, __VA_ARGS__))

V_FINLINE uint16x8_t v__set_u16(s_uint16 i0, s_uint16 i1, s_uint16 i2, s_uint16 i3,
    s_uint16 i4, s_uint16 i5, s_uint16 i6, s_uint16 i7)
{
    const uint16_t V_DECL_ALIGNED(16) data[8] = {i0, i1, i2, i3, i4, i5, i6, i7};
    return vld1q_u16(data);
}
#define v_setf_u16(FILL, ...) v__set_u16(V__SET_FILL_8(s_uint16, FILL, __VA_ARGS__))

V_FINLINE int16x8_t v__set_s16(s_int16 i0, s_int16 i1, s_int16 i2, s_int16 i3,
    s_int16 i4, s_int16 i5, s_int16 i6, s_int16 i7)
{
    const int16_t V_DECL_ALIGNED(16) data[8] = {i0, i1, i2, i3, i4, i5, i6, i7};
    return vld1q_s16(data);
}
#define v_setf_s16(FILL, ...) v__set_s16(V__SET_FILL_8(s_int16, FILL, __VA_ARGS__))

V_FINLINE uint32x4_t v__set_u32(s_uint32 i0, s_uint32 i1, s_uint32 i2, s_uint32 i3)
{
    const uint32_t V_DECL_ALIGNED(16) data[4] = {i0, i1, i2, i3};
    return vld1q_u32(data);
}
#define v_setf_u32(FILL, ...) v__set_u32(V__SET_FILL_4(s_uint32, FILL, __VA_ARGS__))

V_FINLINE int32x4_t v__set_s32(s_int32 i0, s_int32 i1, s_int32 i2, s_int32 i3)
{
    const int32_t V_DECL_ALIGNED(16) data[4] = {i0, i1, i2, i3};
    return vld1q_s32(data);
}
#define v_setf_s32(FILL, ...) v__set_s32(V__SET_FILL_4(s_int32, FILL, __VA_ARGS__))

V_FINLINE uint64x2_t v__set_u64(s_uint64 i0, s_uint64 i1)
{
    const uint64_t V_DECL_ALIGNED(16) data[2] = {i0, i1};
    return vld1q_u64(data);
}
#define v_setf_u64(FILL, ...) v__set_u64(V__SET_FILL_2(s_int64, FILL, __VA_ARGS__))

V_FINLINE int64x2_t v__set_s64(s_int64 i0, s_int64 i1)
{
    const int64_t V_DECL_ALIGNED(16) data[2] = {i0, i1};
    return vld1q_s64(data);
}
#define v_setf_s64(FILL, ...) v__set_s64(V__SET_FILL_2(s_int64, FILL, __VA_ARGS__))

V_FINLINE float32x4_t v__set_f32(float i0, float i1, float i2, float i3)
{
    const float V_DECL_ALIGNED(16) data[4] = {i0, i1, i2, i3};
    return vld1q_f32(data);
}
#define v_setf_f32(FILL, ...) v__set_f32(V__SET_FILL_4(float, FILL, __VA_ARGS__))

#ifdef __aarch64__
V_FINLINE float64x2_t v__set_f64(double i0, double i1)
{
    const double V_DECL_ALIGNED(16) data[2] = {i0, i1};
    return vld1q_f64(data);
}
#define v_setf_f64(FILL, ...) v__set_f64(V__SET_FILL_2(double, FILL, __VA_ARGS__))
#endif

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
#define v_select_u8  vbslq_u8
#define v_select_s8  vbslq_s8
#define v_select_u16 vbslq_u16
#define v_select_s16 vbslq_s16
#define v_select_u32 vbslq_u32
#define v_select_s32 vbslq_s32
#define v_select_u64 vbslq_u64
#define v_select_s64 vbslq_s64
#define v_select_f32 vbslq_f32
#define v_select_f64 vbslq_f64

// Reinterpret
#define v_reinterpret_u8_u8(X) X
#define v_reinterpret_u8_s8  vreinterpretq_u8_s8
#define v_reinterpret_u8_u16 vreinterpretq_u8_u16
#define v_reinterpret_u8_s16 vreinterpretq_u8_s16
#define v_reinterpret_u8_u32 vreinterpretq_u8_u32
#define v_reinterpret_u8_s32 vreinterpretq_u8_s32
#define v_reinterpret_u8_u64 vreinterpretq_u8_u64
#define v_reinterpret_u8_s64 vreinterpretq_u8_s64
#define v_reinterpret_u8_f32 vreinterpretq_u8_f32
#define v_reinterpret_u8_f64 vreinterpretq_u8_f64

#define v_reinterpret_s8_s8(X) X
#define v_reinterpret_s8_u8  vreinterpretq_s8_u8
#define v_reinterpret_s8_u16 vreinterpretq_s8_u16
#define v_reinterpret_s8_s16 vreinterpretq_s8_s16
#define v_reinterpret_s8_u32 vreinterpretq_s8_u32
#define v_reinterpret_s8_s32 vreinterpretq_s8_s32
#define v_reinterpret_s8_u64 vreinterpretq_s8_u64
#define v_reinterpret_s8_s64 vreinterpretq_s8_s64
#define v_reinterpret_s8_f32 vreinterpretq_s8_f32
#define v_reinterpret_s8_f64 vreinterpretq_s8_f64

#define v_reinterpret_u16_u16(X) X
#define v_reinterpret_u16_u8  vreinterpretq_u16_u8
#define v_reinterpret_u16_s8  vreinterpretq_u16_s8
#define v_reinterpret_u16_s16 vreinterpretq_u16_s16
#define v_reinterpret_u16_u32 vreinterpretq_u16_u32
#define v_reinterpret_u16_s32 vreinterpretq_u16_s32
#define v_reinterpret_u16_u64 vreinterpretq_u16_u64
#define v_reinterpret_u16_s64 vreinterpretq_u16_s64
#define v_reinterpret_u16_f32 vreinterpretq_u16_f32
#define v_reinterpret_u16_f64 vreinterpretq_u16_f64

#define v_reinterpret_s16_s16(X) X
#define v_reinterpret_s16_u8  vreinterpretq_s16_u8
#define v_reinterpret_s16_s8  vreinterpretq_s16_s8
#define v_reinterpret_s16_u16 vreinterpretq_s16_u16
#define v_reinterpret_s16_u32 vreinterpretq_s16_u32
#define v_reinterpret_s16_s32 vreinterpretq_s16_s32
#define v_reinterpret_s16_u64 vreinterpretq_s16_u64
#define v_reinterpret_s16_s64 vreinterpretq_s16_s64
#define v_reinterpret_s16_f32 vreinterpretq_s16_f32
#define v_reinterpret_s16_f64 vreinterpretq_s16_f64

#define v_reinterpret_u32_u32(X) X
#define v_reinterpret_u32_u8  vreinterpretq_u32_u8
#define v_reinterpret_u32_s8  vreinterpretq_u32_s8
#define v_reinterpret_u32_u16 vreinterpretq_u32_u16
#define v_reinterpret_u32_s16 vreinterpretq_u32_s16
#define v_reinterpret_u32_s32 vreinterpretq_u32_s32
#define v_reinterpret_u32_u64 vreinterpretq_u32_u64
#define v_reinterpret_u32_s64 vreinterpretq_u32_s64
#define v_reinterpret_u32_f32 vreinterpretq_u32_f32
#define v_reinterpret_u32_f64 vreinterpretq_u32_f64

#define v_reinterpret_s32_s32(X) X
#define v_reinterpret_s32_u8  vreinterpretq_s32_u8
#define v_reinterpret_s32_s8  vreinterpretq_s32_s8
#define v_reinterpret_s32_u16 vreinterpretq_s32_u16
#define v_reinterpret_s32_s16 vreinterpretq_s32_s16
#define v_reinterpret_s32_u32 vreinterpretq_s32_u32
#define v_reinterpret_s32_u64 vreinterpretq_s32_u64
#define v_reinterpret_s32_s64 vreinterpretq_s32_s64
#define v_reinterpret_s32_f32 vreinterpretq_s32_f32
#define v_reinterpret_s32_f64 vreinterpretq_s32_f64

#define v_reinterpret_u64_u64(X) X
#define v_reinterpret_u64_u8  vreinterpretq_u64_u8
#define v_reinterpret_u64_s8  vreinterpretq_u64_s8
#define v_reinterpret_u64_u16 vreinterpretq_u64_u16
#define v_reinterpret_u64_s16 vreinterpretq_u64_s16
#define v_reinterpret_u64_u32 vreinterpretq_u64_u32
#define v_reinterpret_u64_s32 vreinterpretq_u64_s32
#define v_reinterpret_u64_s64 vreinterpretq_u64_s64
#define v_reinterpret_u64_f32 vreinterpretq_u64_f32
#define v_reinterpret_u64_f64 vreinterpretq_u64_f64

#define v_reinterpret_s64_s64(X) X
#define v_reinterpret_s64_u8  vreinterpretq_s64_u8
#define v_reinterpret_s64_s8  vreinterpretq_s64_s8
#define v_reinterpret_s64_u16 vreinterpretq_s64_u16
#define v_reinterpret_s64_s16 vreinterpretq_s64_s16
#define v_reinterpret_s64_u32 vreinterpretq_s64_u32
#define v_reinterpret_s64_s32 vreinterpretq_s64_s32
#define v_reinterpret_s64_u64 vreinterpretq_s64_u64
#define v_reinterpret_s64_f32 vreinterpretq_s64_f32
#define v_reinterpret_s64_f64 vreinterpretq_s64_f64

#define v_reinterpret_f32_f32(X) X
#define v_reinterpret_f32_u8  vreinterpretq_f32_u8
#define v_reinterpret_f32_s8  vreinterpretq_f32_s8
#define v_reinterpret_f32_u16 vreinterpretq_f32_u16
#define v_reinterpret_f32_s16 vreinterpretq_f32_s16
#define v_reinterpret_f32_u32 vreinterpretq_f32_u32
#define v_reinterpret_f32_s32 vreinterpretq_f32_s32
#define v_reinterpret_f32_u64 vreinterpretq_f32_u64
#define v_reinterpret_f32_s64 vreinterpretq_f32_s64
#define v_reinterpret_f32_f64 vreinterpretq_f32_f64

#define v_reinterpret_f64_f64(X) X
#define v_reinterpret_f64_u8  vreinterpretq_f64_u8
#define v_reinterpret_f64_s8  vreinterpretq_f64_s8
#define v_reinterpret_f64_u16 vreinterpretq_f64_u16
#define v_reinterpret_f64_s16 vreinterpretq_f64_s16
#define v_reinterpret_f64_u32 vreinterpretq_f64_u32
#define v_reinterpret_f64_s32 vreinterpretq_f64_s32
#define v_reinterpret_f64_u64 vreinterpretq_f64_u64
#define v_reinterpret_f64_s64 vreinterpretq_f64_s64
#define v_reinterpret_f64_f32 vreinterpretq_f64_f32

// Only required by AVX2/AVX512
#define v_cleanup() ((void)0)

#endif // _V_SIMD_NEON_MISC_H
