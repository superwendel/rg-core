// rg_math_core - shared configuration/types for rg_math
//
// Part of the Reverse Gravity (rg_) core libraries.
// Defines the common configuration, SIMD feature detection, constants, and
// types used by every rg_math module.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_CORE_H
#define RG_MATH_CORE_H

#include "rg_defs.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>

// =============================================================================
// SIMD DETECTION AND INCLUDES
// =============================================================================

#if !defined(RG_MATH_NO_SIMD) && (RG_ARCH_X64 || RG_ARCH_X86)
#if RG_COMPILER_MSVC
#if RG_ARCH_X64 || (RG_ARCH_X86 && defined(_M_IX86_FP) && _M_IX86_FP >= 1)
#include <immintrin.h>
#include <intrin.h>
#define RG_MATH_SSE 1
#endif
#elif defined(__SSE__)
#include <x86intrin.h>
#define RG_MATH_SSE 1
#endif
#endif

#ifdef RG_MATH_SSE
#if defined(__SSE2__) || defined(__AVX__) || defined(__AVX2__) || defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2) || defined(_M_AVX) || defined(_M_AVX2)
#define RG_MATH_SSE2 1
#endif
#if defined(__AVX__) || defined(__AVX2__) || defined(_M_AVX) || defined(_M_AVX2)
#define RG_MATH_AVX 1
#endif
#if defined(__SSE4_1__) || defined(__AVX__) || defined(__AVX2__) || defined(_M_AVX) || defined(_M_AVX2)
#define RG_MATH_SSE41 1
#endif
#if defined(__FMA__) || (defined(_MSC_VER) && (defined(__AVX2__) || defined(_M_AVX2)))
#define RG_MATH_FMA 1
#endif
#if defined(__SSE3__) || defined(__AVX__) || defined(__AVX2__) || defined(_M_AVX) || defined(_M_AVX2)
#define RG_MATH_SSE3 1
#endif
#endif

// Suppress MSVC warning for anonymous structs in unions
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201) // nonstandard extension: nameless struct/union
#endif

// Alignment macro for SIMD types
#ifdef _MSC_VER
#define RG_ALIGN16 __declspec(align(16))
#define RG_ALIGN32 __declspec(align(32))
#else
#define RG_ALIGN16 __attribute__((aligned(16)))
#define RG_ALIGN32 __attribute__((aligned(32)))
#endif

#ifndef RG_MATH_EXTERN_C_BEGIN
#define RG_MATH_EXTERN_C_BEGIN
#define RG_MATH_EXTERN_C_END
#endif

// =============================================================================
// CONFIGURATION
// =============================================================================

#ifndef RG_MATH_ASSERT
#include <assert.h>
#define RG_MATH_ASSERT(x) assert(x)
#endif

#ifndef RG_MATH_MAX_PERF
#define RG_MATH_MAX_PERF 1
#endif

#if RG_MATH_MAX_PERF != 0 && RG_MATH_MAX_PERF != 1
#error RG_MATH_MAX_PERF must be 0 or 1
#endif

#ifndef RG_MATH_USE_LIBC
#if RG_MATH_MAX_PERF
#define RG_MATH_USE_LIBC 1
#else
#define RG_MATH_USE_LIBC 0
#endif
#endif

#if RG_MATH_USE_LIBC != 0 && RG_MATH_USE_LIBC != 1
#error RG_MATH_USE_LIBC must be 0 or 1
#endif

#ifndef RG_MATH_LIBC_ALIASES
#if RG_MATH_USE_LIBC
#define RG_MATH_LIBC_ALIASES 1
#else
#define RG_MATH_LIBC_ALIASES 0
#endif
#endif

#if RG_MATH_LIBC_ALIASES != 0 && RG_MATH_LIBC_ALIASES != 1
#error RG_MATH_LIBC_ALIASES must be 0 or 1
#endif

#ifndef RG_MATH_RESTRICT
#define RG_MATH_RESTRICT RG_RESTRICT
#endif

#ifndef RG_MATH_ASSUME_ALIGNED
#if RG_COMPILER_MSVC
#define RG_MATH_ASSUME_ALIGNED(ptr, alignment) (ptr)
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
#define RG_MATH_ASSUME_ALIGNED(ptr, alignment) \
	((__typeof__(ptr))__builtin_assume_aligned((ptr), (alignment)))
#else
#define RG_MATH_ASSUME_ALIGNED(ptr, alignment) (ptr)
#endif
#endif

#if RG_MATH_MAX_PERF
#ifndef RG_MATH_UNSAFE_NORMALIZE
#define RG_MATH_UNSAFE_NORMALIZE
#endif
#ifndef RG_MATH_FAST_NORMALIZE
#define RG_MATH_FAST_NORMALIZE
#endif
#endif

#define RG_MATH_RSQRT_MODE_RSQRT 0
#define RG_MATH_RSQRT_MODE_SQRT_RCP 1

#ifndef RG_MATH_RSQRT_MODE
#define RG_MATH_RSQRT_MODE RG_MATH_RSQRT_MODE_RSQRT
#endif

#if RG_MATH_RSQRT_MODE != RG_MATH_RSQRT_MODE_RSQRT && RG_MATH_RSQRT_MODE != RG_MATH_RSQRT_MODE_SQRT_RCP
#error RG_MATH_RSQRT_MODE must be RG_MATH_RSQRT_MODE_RSQRT or RG_MATH_RSQRT_MODE_SQRT_RCP
#endif

// Clip-space / handedness control (matches cglm defaults)
#define RG_MATH_CLIP_CONTROL_ZO_BIT (1 << 0) // Depth range [0, 1]
#define RG_MATH_CLIP_CONTROL_NO_BIT (1 << 1) // Depth range [-1, 1]
#define RG_MATH_CLIP_CONTROL_LH_BIT (1 << 2) // Left-handed
#define RG_MATH_CLIP_CONTROL_RH_BIT (1 << 3) // Right-handed

#define RG_MATH_CLIP_CONTROL_LH_ZO (RG_MATH_CLIP_CONTROL_LH_BIT | RG_MATH_CLIP_CONTROL_ZO_BIT)
#define RG_MATH_CLIP_CONTROL_LH_NO (RG_MATH_CLIP_CONTROL_LH_BIT | RG_MATH_CLIP_CONTROL_NO_BIT)
#define RG_MATH_CLIP_CONTROL_RH_ZO (RG_MATH_CLIP_CONTROL_RH_BIT | RG_MATH_CLIP_CONTROL_ZO_BIT)
#define RG_MATH_CLIP_CONTROL_RH_NO (RG_MATH_CLIP_CONTROL_RH_BIT | RG_MATH_CLIP_CONTROL_NO_BIT)

#ifndef RG_MATH_CLIP_CONTROL
#define RG_MATH_CLIP_CONTROL RG_MATH_CLIP_CONTROL_RH_NO
#endif

#if RG_MATH_CLIP_CONTROL != RG_MATH_CLIP_CONTROL_LH_ZO && RG_MATH_CLIP_CONTROL != RG_MATH_CLIP_CONTROL_LH_NO && RG_MATH_CLIP_CONTROL != RG_MATH_CLIP_CONTROL_RH_ZO && RG_MATH_CLIP_CONTROL != RG_MATH_CLIP_CONTROL_RH_NO
#error RG_MATH_CLIP_CONTROL must be one of the RG_MATH_CLIP_CONTROL_* modes
#endif

// =============================================================================
// CONSTANTS
// =============================================================================

#define RG_PI 3.14159265358979323846f
#define RG_HALF_PI 1.57079632679489661923f
#define RG_TAU 6.28318530717958647692f
#define RG_E 2.71828182845904523536f
#define RG_LN2 0.69314718055994530942f
#define RG_INV_LN2 1.44269504088896340736f
#define RG_INV_LOG2_10 0.30102999566398119521f
#define RG_EPSILON 1.192092896e-07f
#define RG_DEG_TO_RAD (RG_PI / 180.0f)
#define RG_RAD_TO_DEG (180.0f / RG_PI)

// =============================================================================
// TYPE DEFINITIONS - SIMD-OPTIMIZED
// =============================================================================

/** @brief 2D f32 vector (8 bytes) */
typedef union rg_vec2
{
	f32 data[2];
	struct
	{
		f32 x, y;
	};
} rg_vec2;

/** @brief 3D f32 vector - 16-byte aligned for SIMD */
#ifdef RG_MATH_VEC3_PLAIN
typedef RG_ALIGN16 union rg_vec3
{
	f32 data[4]; // 4th element is padding
	struct
	{
		f32 x, y, z, _pad;
	};
} rg_vec3;
#else
typedef RG_ALIGN16 union rg_vec3
{
	f32 data[4]; // 4th element is padding for SIMD
#ifdef RG_MATH_SSE
	__m128 simd;
#endif
	struct
	{
		f32 x, y, z, _pad;
	};
} rg_vec3;
#endif

/** @brief 4D f32 vector - 16-byte aligned for SIMD */
#ifdef RG_MATH_VEC4_PLAIN
typedef RG_ALIGN16 union rg_vec4
{
	f32 data[4];
	struct
	{
		f32 x, y, z, w;
	};
} rg_vec4;
#else
typedef RG_ALIGN16 union rg_vec4
{
	f32 data[4];
#ifdef RG_MATH_SSE
	__m128 simd;
#endif
	struct
	{
		f32 x, y, z, w;
	};
} rg_vec4;
#endif

/** @brief 2D integer vector */
typedef union rg_vec2i
{
	i32 data[2];
	struct
	{
		i32 x, y;
	};
} rg_vec2i;

/** @brief 3D integer vector - 16-byte aligned for SIMD */
typedef RG_ALIGN16 union rg_vec3i
{
	i32 data[4]; // 4th element is padding
#ifdef RG_MATH_SSE
	__m128i simd;
#endif
	struct
	{
		i32 x, y, z, _pad;
	};
} rg_vec3i;

/** @brief 4D integer vector - 16-byte aligned for SIMD */
typedef RG_ALIGN16 union rg_vec4i
{
	i32 data[4];
#ifdef RG_MATH_SSE
	__m128i simd;
#endif
	struct
	{
		i32 x, y, z, w;
	};
} rg_vec4i;

/** @brief 2x2 matrix (column-major) */
typedef struct rg_mat2
{
	f32 m[4];
} rg_mat2;

/** @brief 3x3 matrix (column-major, padded columns for SIMD) */
typedef struct rg_mat3
{
	f32 m[12]; // 3 columns of 4 floats each (last f32 unused per column)
} rg_mat3;

/** @brief 4x4 matrix (column-major) - 16-byte aligned for SIMD */
typedef RG_ALIGN16 struct rg_mat4
{
	f32 m[16];
} rg_mat4;

/** @brief 2x3 matrix (column-major, 2 columns, 3 rows) */
typedef struct rg_mat2x3
{
	f32 m[6];
} rg_mat2x3;

/** @brief 2x4 matrix (column-major, 2 columns, 4 rows) */
typedef struct rg_mat2x4
{
	f32 m[8];
} rg_mat2x4;

/** @brief 3x2 matrix (column-major, 3 columns, 2 rows) */
typedef struct rg_mat3x2
{
	f32 m[6];
} rg_mat3x2;

/** @brief 3x4 matrix (column-major, 3 columns, 4 rows) */
typedef struct rg_mat3x4
{
	f32 m[12];
} rg_mat3x4;

/** @brief 4x2 matrix (column-major, 4 columns, 2 rows) */
typedef struct rg_mat4x2
{
	f32 m[8];
} rg_mat4x2;

/** @brief 4x3 matrix (column-major, 4 columns, 3 rows) */
typedef struct rg_mat4x3
{
	f32 m[12];
} rg_mat4x3;

/** @brief Quaternion for rotations - 16-byte aligned for SIMD */
typedef RG_ALIGN16 union rg_quat
{
	f32 data[4];
#ifdef RG_MATH_SSE
	__m128 simd;
#endif
	struct
	{
		f32 x, y, z, w;
	};
} rg_quat;

#ifdef RG_MATH_SSE
#ifdef RG_MATH_VEC3_PLAIN
#define RG_VEC3_LOAD(v) _mm_load_ps((v)->data)
#define RG_VEC3_STORE(v, m) _mm_store_ps((v)->data, (m))
#else
#define RG_VEC3_LOAD(v) ((v)->simd)
#define RG_VEC3_STORE(v, m) ((v)->simd = (m))
#endif
#ifdef RG_MATH_VEC4_PLAIN
#define RG_VEC4_LOAD(v) _mm_load_ps((v)->data)
#define RG_VEC4_STORE(v, m) _mm_store_ps((v)->data, (m))
#else
#define RG_VEC4_LOAD(v) ((v)->simd)
#define RG_VEC4_STORE(v, m) ((v)->simd = (m))
#endif
#ifdef RG_MATH_FMA
#define RG_MATH_FMADD_PS(a, b, c) _mm_fmadd_ps((a), (b), (c))
#define RG_MATH_FNMADD_PS(a, b, c) _mm_fnmadd_ps((a), (b), (c))
#else
#define RG_MATH_FMADD_PS(a, b, c) _mm_add_ps(_mm_mul_ps((a), (b)), (c))
#define RG_MATH_FNMADD_PS(a, b, c) _mm_sub_ps((c), _mm_mul_ps((a), (b)))
#endif
#endif

// Legacy type aliases for compatibility
typedef rg_vec2 Vec2;
typedef rg_vec3 Vec3;
typedef rg_vec4 Vec4;
typedef rg_vec2i Vec2i;
typedef rg_vec3i Vec3i;
typedef rg_vec4i Vec4i;
typedef rg_mat2 Mat2f;
typedef rg_mat3 Mat3f;
typedef rg_mat4 Mat4f;
typedef rg_mat2x3 Mat2x3f;
typedef rg_mat2x4 Mat2x4f;
typedef rg_mat3x2 Mat3x2f;
typedef rg_mat3x4 Mat3x4f;
typedef rg_mat4x2 Mat4x2f;
typedef rg_mat4x3 Mat4x3f;
typedef rg_quat Quatf;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // RG_MATH_CORE_H
