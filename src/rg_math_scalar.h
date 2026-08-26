// rg_math_scalar - scalar math helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides scalar arithmetic, approximation, interpolation, and conversion helpers.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_SCALAR_H
#define RG_MATH_SCALAR_H

#include "rg_math_core.h"

RG_MATH_EXTERN_C_BEGIN

// =============================================================================
// SCALAR MATH - PUBLIC API
// =============================================================================

#if RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
RGINLINE f32 rg_rsqrtf(f32 x)
{
	__m128 val = _mm_set_ss(x);
#if RG_MATH_RSQRT_MODE == RG_MATH_RSQRT_MODE_SQRT_RCP
	return _mm_cvtss_f32(_mm_rcp_ss(_mm_sqrt_ss(val)));
#else
	return _mm_cvtss_f32(_mm_rsqrt_ss(val));
#endif
}
#else
#define rg_rsqrtf(x) (1.0f / sqrtf((x)))
#endif
#if RG_MATH_MAX_PERF
RGINLINE f32 rg_absf(f32 x)
{
	union
	{
		f32 f;
		u32 u;
	} bits;
	bits.f = x;
	bits.u &= 0x7FFFFFFFu;
	return bits.f;
}
#else
#define rg_absf(x) fabsf((x))
#endif
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
RGINLINE f32 rg_sqrtf(f32 x)
{
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x)));
}
#else
#define rg_sqrtf(x) sqrtf((x))
#endif
#if defined(RG_MATH_SSE41) && RG_MATH_MAX_PERF
RGINLINE f32 rg_floorf(f32 x)
{
	return _mm_cvtss_f32(_mm_floor_ss(_mm_setzero_ps(), _mm_set_ss(x)));
}

RGINLINE f32 rg_ceilf(f32 x)
{
	return _mm_cvtss_f32(_mm_ceil_ss(_mm_setzero_ps(), _mm_set_ss(x)));
}

RGINLINE f32 rg_roundf(f32 x)
{
	__m128 xv = _mm_set_ss(x);
	__m128 sign = _mm_and_ps(xv, _mm_castsi128_ps(_mm_set1_epi32((int)0x80000000u)));
	__m128 half = _mm_or_ps(_mm_set_ss(0.5f), sign);
	return _mm_cvtss_f32(_mm_round_ss(_mm_setzero_ps(), _mm_add_ss(xv, half), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
}
#else
#define rg_floorf(x) floorf((x))
#define rg_ceilf(x) ceilf((x))
#define rg_roundf(x) roundf((x))
#endif
#ifdef RG_MATH_FAST_TRIG
#define rg_sinf(x) rg_sinf_fast((x))
#define rg_cosf(x) rg_cosf_fast((x))
#else
#define rg_sinf(x) sinf((x))
#define rg_cosf(x) cosf((x))
#endif
#define rg_tanf(x) tanf((x))
#ifdef RG_MATH_FAST_TRIG
#define rg_asinf(x) rg_asinf_fast((x))
#define rg_acosf(x) rg_acosf_fast((x))
#define rg_atanf(x) rg_math_atanf_fast_impl((x))
#define rg_atan2f(y, x) rg_atan2f_fast((y), (x))
#else
#define rg_asinf(x) asinf((x))
#define rg_acosf(x) acosf((x))
#define rg_atanf(x) atanf((x))
#define rg_atan2f(y, x) atan2f((y), (x))
#endif
#ifdef RG_MATH_FAST_EXP
#define rg_expf(x) rg_expf_fast((x))
#define rg_logf(x) rg_logf_fast((x))
#define rg_log2f(x) rg_log2f_fast((x))
#define rg_log10f(x) rg_log10f_fast((x))
#define rg_powf(x, y) rg_powf_fast((x), (y))
#else
#define rg_expf(x) expf((x))
#define rg_logf(x) logf((x))
#define rg_log2f(x) log2f((x))
#define rg_log10f(x) log10f((x))
#define rg_powf(x, y) powf((x), (y))
#endif
#else
/**
 * @brief Fast inverse square root (1/sqrt(x))
 * @param x Input value (must be positive)
 * @return Approximate 1/sqrt(x)
 */
RGINLINE f32 rg_rsqrtf(f32 x);

/**
 * @brief Fast approximate square root
 * @param x Input value (must be non-negative)
 * @return Approximate sqrt(x)
 */
RGINLINE f32 rg_sqrtf(f32 x);

/**
 * @brief Absolute value (branchless)
 * @param x Input value
 * @return |x|
 */
RGINLINE f32 rg_absf(f32 x);

/**
 * @brief Floor function (largest integer <= x)
 * @param x Input value
 * @return floor(x)
 */
RGINLINE f32 rg_floorf(f32 x);

/**
 * @brief Ceiling function (smallest integer >= x)
 * @param x Input value
 * @return ceil(x)
 */
RGINLINE f32 rg_ceilf(f32 x);

/**
 * @brief Round to nearest integer
 * @param x Input value
 * @return round(x)
 */
RGINLINE f32 rg_roundf(f32 x);
#endif

/**
 * @brief Fractional part of a f32
 * @param x Input value
 * @return x - floor(x)
 */
RGINLINE f32 rg_fracf(f32 x);

/**
 * @brief Floating-point modulo
 * @param x Dividend
 * @param y Divisor
 * @return x mod y
 */
RGINLINE f32 rg_fmodf(f32 x, f32 y);

/**
 * @brief Minimum of two floats
 * @param a First value
 * @param b Second value
 * @return min(a, b)
 */
RGINLINE f32 rg_minf(f32 a, f32 b);

/**
 * @brief Maximum of two floats
 * @param a First value
 * @param b Second value
 * @return max(a, b)
 */
RGINLINE f32 rg_maxf(f32 a, f32 b);

/**
 * @brief Clamp value to range
 * @param x Value to clamp
 * @param lo Lower bound
 * @param hi Upper bound
 * @return clamped value in [lo, hi]
 */
RGINLINE f32 rg_clampf(f32 x, f32 lo, f32 hi);

/**
 * @brief Linear interpolation
 * @param a Start value
 * @param b End value
 * @param t Interpolation factor [0, 1]
 * @return a + (b - a) * t
 */
RGINLINE f32 rg_lerpf(f32 a, f32 b, f32 t);

/**
 * @brief Smooth step (cubic Hermite interpolation)
 * @param edge0 Lower edge
 * @param edge1 Upper edge
 * @param x Input value
 * @return Smooth interpolation in [0, 1]
 */
RGINLINE f32 rg_smoothstepf(f32 edge0, f32 edge1, f32 x);

/**
 * @brief Sign of an integer value
 * @param val Input value
 * @return -1 if val < 0, 0 if val == 0, 1 if val > 0
 */
RGINLINE int rg_sign(int val);

/**
 * @brief Convert degrees to radians
 * @param deg Angle in degrees
 * @return Angle in radians
 */
RGINLINE f32 rg_rad(f32 deg);

/**
 * @brief Convert radians to degrees
 * @param rad Angle in radians
 * @return Angle in degrees
 */
RGINLINE f32 rg_deg(f32 rad);

/**
 * @brief Convert degrees to radians in-place
 * @param deg Pointer to degrees value (overwritten with radians)
 */
RGINLINE void rg_make_rad(f32* deg);

/**
 * @brief Convert radians to degrees in-place
 * @param rad Pointer to radians value (overwritten with degrees)
 */
RGINLINE void rg_make_deg(f32* rad);

/**
 * @brief Square a f32 value
 * @param x Input value
 * @return x * x
 */
RGINLINE f32 rg_pow2(f32 x);

/**
 * @brief Minimum of two floats
 * @param a First value
 * @param b Second value
 * @return min(a, b)
 */
RGINLINE f32 rg_min(f32 a, f32 b);

/**
 * @brief Maximum of two floats
 * @param a First value
 * @param b Second value
 * @return max(a, b)
 */
RGINLINE f32 rg_max(f32 a, f32 b);

/**
 * @brief Minimum of two integers
 * @param a First value
 * @param b Second value
 * @return min(a, b)
 */
RGINLINE int rg_imin(int a, int b);

/**
 * @brief Maximum of two integers
 * @param a First value
 * @param b Second value
 * @return max(a, b)
 */
RGINLINE int rg_imax(int a, int b);

/**
 * @brief Clamp value to range
 * @param val Value to clamp
 * @param min_val Lower bound
 * @param max_val Upper bound
 * @return clamped value in [min_val, max_val]
 */
RGINLINE f32 rg_clamp(f32 val, f32 min_val, f32 max_val);

/**
 * @brief Clamp value to [0, 1]
 * @param val Value to clamp
 * @return clamped value in [0, 1]
 */
RGINLINE f32 rg_clamp_zo(f32 val);

/**
 * @brief Linear interpolation
 * @param from Start value
 * @param to End value
 * @param t Interpolation factor [0, 1]
 * @return from + (to - from) * t
 */
RGINLINE f32 rg_lerp(f32 from, f32 to, f32 t);

/**
 * @brief Clamped linear interpolation
 * @param from Start value
 * @param to End value
 * @param t Interpolation factor clamped to [0, 1]
 * @return from + (to - from) * t
 */
RGINLINE f32 rg_lerpc(f32 from, f32 to, f32 t);

/**
 * @brief Threshold function
 * @param edge Threshold
 * @param x Input value
 * @return 0.0 if x < edge, else 1.0
 */
RGINLINE f32 rg_step(f32 edge, f32 x);

/**
 * @brief Smooth Hermite interpolation
 * @param t Interpolation factor
 * @return t^2 * (3 - 2t)
 */
RGINLINE f32 rg_smooth(f32 t);

/**
 * @brief Smooth step with edges
 * @param edge0 Lower edge
 * @param edge1 Upper edge
 * @param x Input value
 * @return Smooth interpolation in [0, 1]
 */
RGINLINE f32 rg_smoothstep(f32 edge0, f32 edge1, f32 x);

/**
 * @brief Smooth interpolation between two values
 * @param from Start value
 * @param to End value
 * @param t Interpolation factor
 * @return from + smooth(t) * (to - from)
 */
RGINLINE f32 rg_smoothinterp(f32 from, f32 to, f32 t);

/**
 * @brief Clamped smooth interpolation between two values
 * @param from Start value
 * @param to End value
 * @param t Interpolation factor clamped to [0, 1]
 * @return from + smooth(t) * (to - from)
 */
RGINLINE f32 rg_smoothinterpc(f32 from, f32 to, f32 t);

/**
 * @brief Compare two floats with epsilon
 * @param a First value
 * @param b Second value
 * @return 1 if |a - b| <= RG_EPSILON
 */
RGINLINE int rg_eq(f32 a, f32 b);

/**
 * @brief Percentage of current between from and to
 * @param from Start value
 * @param to End value
 * @param current Current value
 * @return (current - from) / (to - from)
 */
RGINLINE f32 rg_percent(f32 from, f32 to, f32 current);

/**
 * @brief Clamped percentage of current between from and to
 * @param from Start value
 * @param to End value
 * @param current Current value
 * @return clamped percent in [0, 1]
 */
RGINLINE f32 rg_percentc(f32 from, f32 to, f32 current);

/**
 * @brief Swap two floats
 * @param a First value
 * @param b Second value
 */
RGINLINE void rg_swapf(f32* a, f32* b);

/**
 * @brief Sign function
 * @param x Input value
 * @return -1 if x < 0, 0 if x == 0, 1 if x > 0
 */
RGINLINE f32 rg_signf(f32 x);

/**
 * @brief Copy sign from one value to another
 * @param x Value to modify
 * @param y Value to copy sign from
 * @return |x| * sign(y)
 */
RGINLINE f32 rg_copysignf(f32 x, f32 y);

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
/**
 * @brief Sine (radians)
 * @param x Angle in radians
 * @return sin(x)
 */
RGINLINE f32 rg_sinf(f32 x);

/**
 * @brief Cosine (radians)
 * @param x Angle in radians
 * @return cos(x)
 */
RGINLINE f32 rg_cosf(f32 x);
#endif

/**
 * @brief Fast sine approximation (radians)
 * @param x Angle in radians
 * @return Approximate sin(x)
 */
RGINLINE f32 rg_sinf_fast(f32 x);

/**
 * @brief Fast cosine approximation (radians)
 * @param x Angle in radians
 * @return Approximate cos(x)
 */
RGINLINE f32 rg_cosf_fast(f32 x);

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
/**
 * @brief Tangent (radians)
 * @param x Angle in radians
 * @return tan(x)
 */
RGINLINE f32 rg_tanf(f32 x);

/**
 * @brief Arcsine (radians)
 * @param x Value in [-1, 1]
 * @return asin(x)
 */
RGINLINE f32 rg_asinf(f32 x);
#endif

/**
 * @brief Fast arcsine approximation (radians)
 * @param x Value in [-1, 1]
 * @return Approximate asin(x)
 */
RGINLINE f32 rg_asinf_fast(f32 x);

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
/**
 * @brief Arccosine (radians)
 * @param x Value in [-1, 1]
 * @return acos(x)
 */
RGINLINE f32 rg_acosf(f32 x);
#endif

/**
 * @brief Fast arccosine approximation (radians)
 * @param x Value in [-1, 1]
 * @return Approximate acos(x)
 */
RGINLINE f32 rg_acosf_fast(f32 x);

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
/**
 * @brief Arctangent (radians)
 * @param x Input value
 * @return atan(x)
 */
RGINLINE f32 rg_atanf(f32 x);

/**
 * @brief Arctangent of y/x (radians)
 * @param y Y component
 * @param x X component
 * @return atan2(y, x)
 */
RGINLINE f32 rg_atan2f(f32 y, f32 x);
#endif

/**
 * @brief Fast arctangent of y/x approximation (radians)
 * @param y Y component
 * @param x X component
 * @return Approximate atan2(y, x)
 */
RGINLINE f32 rg_atan2f_fast(f32 y, f32 x);

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
/**
 * @brief Exponential function
 * @param x Exponent
 * @return exp(x)
 */
RGINLINE f32 rg_expf(f32 x);

/**
 * @brief Natural logarithm
 * @param x Input value
 * @return log(x)
 */
RGINLINE f32 rg_logf(f32 x);

/**
 * @brief Base-2 logarithm
 * @param x Input value
 * @return log2(x)
 */
RGINLINE f32 rg_log2f(f32 x);

/**
 * @brief Base-10 logarithm
 * @param x Input value
 * @return log10(x)
 */
RGINLINE f32 rg_log10f(f32 x);

/**
 * @brief Power function
 * @param x Base value
 * @param y Exponent
 * @return x^y
 */
RGINLINE f32 rg_powf(f32 x, f32 y);
#endif

/**
 * @brief Sine/cosine (radians)
 * @param x Angle in radians
 * @param out_sin Output sine (required)
 * @param out_cos Output cosine (required)
 */
RGINLINE void rg_sincosf(f32 x, f32* out_sin, f32* out_cos);

/**
 * @brief Fast exp approximation
 * @param x Exponent
 * @return Approximate exp(x)
 */
RGINLINE f32 rg_expf_fast(f32 x);

/**
 * @brief Fast log approximation
 * @param x Input value (positive)
 * @return Approximate log(x)
 */
RGINLINE f32 rg_logf_fast(f32 x);

/**
 * @brief Fast log2 approximation
 * @param x Input value (positive)
 * @return Approximate log2(x)
 */
RGINLINE f32 rg_log2f_fast(f32 x);

/**
 * @brief Fast log10 approximation
 * @param x Input value (positive)
 * @return Approximate log10(x)
 */
RGINLINE f32 rg_log10f_fast(f32 x);

/**
 * @brief Fast pow approximation
 * @param x Base value (positive)
 * @param y Exponent
 * @return Approximate x^y
 */
RGINLINE f32 rg_powf_fast(f32 x, f32 y);

// =============================================================================

RG_MATH_EXTERN_C_END

// IMPLEMENTATION
// =============================================================================

// Union for f32 bit manipulation
typedef union rg_float_bits
{
	f32 f;
	u32 u;
	i32 i;
} rg_float_bits;

// -----------------------------------------------------------------------------
// Fast inverse square root (SSE or Quake III algorithm)
// -----------------------------------------------------------------------------

#ifdef RG_MATH_SSE
#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
RGINLINE f32 rg_rsqrtf(f32 x)
{
#if RG_MATH_MAX_PERF || defined(RG_MATH_FAST_MATH)
	__m128 val = _mm_set_ss(x);
#if RG_MATH_RSQRT_MODE == RG_MATH_RSQRT_MODE_SQRT_RCP
	__m128 sqrt = _mm_sqrt_ss(val);
	__m128 inv = _mm_rcp_ss(sqrt);
	return _mm_cvtss_f32(inv);
#else
	return _mm_cvtss_f32(_mm_rsqrt_ss(val));
#endif
#elif RG_MATH_USE_LIBC
	return 1.0f / sqrtf(x);
#else
	__m128 val = _mm_set_ss(x);
	__m128 rsqrt = _mm_rsqrt_ss(val);
	// One Newton-Raphson iteration for better precision
	__m128 half = _mm_set_ss(0.5f);
	__m128 three = _mm_set_ss(3.0f);
	__m128 muls = _mm_mul_ss(_mm_mul_ss(val, rsqrt), rsqrt);
	rsqrt = _mm_mul_ss(_mm_mul_ss(half, rsqrt), _mm_sub_ss(three, muls));
	return _mm_cvtss_f32(rsqrt);
#endif
}
#endif
RGINLINE f32 rg_rsqrtf_fast(f32 x)
{
	__m128 val = _mm_set_ss(x);
	return _mm_cvtss_f32(_mm_rsqrt_ss(val));
}
#else
#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
RGINLINE f32 rg_rsqrtf(f32 x)
{
#if RG_MATH_USE_LIBC
	return 1.0f / sqrtf(x);
#elif RG_MATH_MAX_PERF || defined(RG_MATH_FAST_MATH)
	rg_float_bits conv;
	conv.f = x;
	conv.u = 0x5F3759DF - (conv.u >> 1); // Initial guess (no refinement)
	return conv.f;
#else
	rg_float_bits conv;
	conv.f = x;
	conv.u = 0x5F3759DF - (conv.u >> 1);           // Initial guess
	conv.f *= 1.5f - (x * 0.5f * conv.f * conv.f); // Newton-Raphson iteration 1
	conv.f *= 1.5f - (x * 0.5f * conv.f * conv.f); // Newton-Raphson iteration 2
	return conv.f;
#endif
}
#endif
RGINLINE f32 rg_rsqrtf_fast(f32 x)
{
	rg_float_bits conv;
	conv.f = x;
	conv.u = 0x5F3759DF - (conv.u >> 1); // Initial guess (no refinement)
	return conv.f;
}
#endif

#ifdef RG_MATH_SSE
#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
RGINLINE f32 rg_sqrtf(f32 x)
{
#if !RG_MATH_MAX_PERF
	if (x <= 0.0f) return 0.0f;
#endif
#if RG_MATH_USE_LIBC
	return sqrtf(x);
#elif defined(RG_MATH_FAST_MATH)
	return x * rg_rsqrtf_fast(x);
#else
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(x)));
#endif
}
#endif
#else
#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
RGINLINE f32 rg_sqrtf(f32 x)
{
#if !RG_MATH_MAX_PERF
	if (x <= 0.0f) return 0.0f;
#endif
#if RG_MATH_USE_LIBC
	return sqrtf(x);
#elif defined(RG_MATH_FAST_MATH)
	return x * rg_rsqrtf_fast(x);
#else
	return x * rg_rsqrtf(x);
#endif
}
#endif
#endif

// -----------------------------------------------------------------------------
// Branchless absolute value (clear sign bit)
// -----------------------------------------------------------------------------

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
RGINLINE f32 rg_absf(f32 x)
{
#if RG_MATH_MAX_PERF || !RG_MATH_USE_LIBC
	rg_float_bits conv;
	conv.f = x;
	conv.u &= 0x7FFFFFFF; // Clear sign bit
	return conv.f;
#else
	return fabsf(x);
#endif
}

// -----------------------------------------------------------------------------
// Floor, ceil, round (using integer truncation)
// For values >= 2^23, floats are already integers (no fractional part)
// -----------------------------------------------------------------------------

#define RG_FLOAT_NO_FRACTION 8388608.0f // 2^23

RGINLINE f32 rg_floorf(f32 x)
{
#if RG_MATH_USE_LIBC
	return floorf(x);
#else
	// Large values are already integers (f32 can't represent fractions)
	if (x >= RG_FLOAT_NO_FRACTION || x <= -RG_FLOAT_NO_FRACTION)
	{
		return x;
	}
	i32 i = (i32)x;
	return (f32)(i - (x < (f32)i));
#endif
}

RGINLINE f32 rg_ceilf(f32 x)
{
#if RG_MATH_USE_LIBC
	return ceilf(x);
#else
	// Large values are already integers (f32 can't represent fractions)
	if (x >= RG_FLOAT_NO_FRACTION || x <= -RG_FLOAT_NO_FRACTION)
	{
		return x;
	}
	i32 i = (i32)x;
	return (f32)(i + (x > (f32)i));
#endif
}

RGINLINE f32 rg_roundf(f32 x)
{
#if RG_MATH_USE_LIBC
	return roundf(x);
#else
	// Round half away from zero (C99 behavior)
	return x >= 0.0f ? rg_floorf(x + 0.5f) : rg_ceilf(x - 0.5f);
#endif
}
#endif

RGINLINE f32 rg_fracf(f32 x)
{
	return x - rg_floorf(x);
}

// -----------------------------------------------------------------------------
// Floating-point modulo
// -----------------------------------------------------------------------------

RGINLINE f32 rg_fmodf(f32 x, f32 y)
{
	if (y == 0.0f) return 0.0f;
#if RG_MATH_MAX_PERF
	return x - y * rg_floorf(x / y);
#elif RG_MATH_USE_LIBC
	if (x >= 0.0f && y > 0.0f)
	{
		return fmodf(x, y);
	}
	return x - y * rg_floorf(x / y);
#else
	return x - y * rg_floorf(x / y);
#endif
}

// -----------------------------------------------------------------------------
// Min, max, clamp
// -----------------------------------------------------------------------------

RGINLINE f32 rg_minf(f32 a, f32 b)
{
	return a < b ? a : b;
}

RGINLINE f32 rg_maxf(f32 a, f32 b)
{
	return a > b ? a : b;
}

RGINLINE f32 rg_clampf(f32 x, f32 lo, f32 hi)
{
	return !(x >= lo) ? lo : (x > hi ? hi : x);
}

// -----------------------------------------------------------------------------
// Interpolation
// -----------------------------------------------------------------------------

RGINLINE f32 rg_lerpf(f32 a, f32 b, f32 t)
{
	return a + (b - a) * t;
}

RGINLINE f32 rg_smoothstepf(f32 edge0, f32 edge1, f32 x)
{
	// Handle degenerate case to avoid division by zero
	if (edge1 <= edge0)
	{
		return (x < edge0) ? 0.0f : 1.0f;
	}
	f32 t = rg_clampf((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	return t * t * (3.0f - 2.0f * t);
}

RGINLINE int rg_sign(int val)
{
	return ((val >> 31) - (-val >> 31));
}

RGINLINE f32 rg_rad(f32 deg)
{
	return deg * RG_DEG_TO_RAD;
}

RGINLINE f32 rg_deg(f32 rad)
{
	return rad * RG_RAD_TO_DEG;
}

RGINLINE void rg_make_rad(f32* deg)
{
	*deg = *deg * RG_DEG_TO_RAD;
}

RGINLINE void rg_make_deg(f32* rad)
{
	*rad = *rad * RG_RAD_TO_DEG;
}

RGINLINE f32 rg_pow2(f32 x)
{
	return x * x;
}

RGINLINE f32 rg_min(f32 a, f32 b)
{
	return rg_minf(a, b);
}

RGINLINE f32 rg_max(f32 a, f32 b)
{
	return rg_maxf(a, b);
}

RGINLINE int rg_imin(int a, int b)
{
	return a < b ? a : b;
}

RGINLINE int rg_imax(int a, int b)
{
	return a > b ? a : b;
}

RGINLINE f32 rg_clamp(f32 val, f32 min_val, f32 max_val)
{
	return rg_clampf(val, min_val, max_val);
}

RGINLINE f32 rg_clamp_zo(f32 val)
{
	return rg_clampf(val, 0.0f, 1.0f);
}

RGINLINE f32 rg_lerp(f32 from, f32 to, f32 t)
{
	return rg_lerpf(from, to, t);
}

RGINLINE f32 rg_lerpc(f32 from, f32 to, f32 t)
{
	return rg_lerpf(from, to, rg_clamp_zo(t));
}

RGINLINE f32 rg_step(f32 edge, f32 x)
{
	return x < edge ? 0.0f : 1.0f;
}

RGINLINE f32 rg_smooth(f32 t)
{
	return t * t * (3.0f - 2.0f * t);
}

RGINLINE f32 rg_smoothstep(f32 edge0, f32 edge1, f32 x)
{
	return rg_smoothstepf(edge0, edge1, x);
}

RGINLINE f32 rg_smoothinterp(f32 from, f32 to, f32 t)
{
	return from + rg_smooth(t) * (to - from);
}

RGINLINE f32 rg_smoothinterpc(f32 from, f32 to, f32 t)
{
	return rg_smoothinterp(from, to, rg_clamp_zo(t));
}

RGINLINE int rg_eq(f32 a, f32 b)
{
	return rg_absf(a - b) <= RG_EPSILON;
}

RGINLINE f32 rg_percent(f32 from, f32 to, f32 current)
{
	f32 range = to - from;
	if (range == 0.0f)
	{
		return 1.0f;
	}
	return (current - from) / range;
}

RGINLINE f32 rg_percentc(f32 from, f32 to, f32 current)
{
	return rg_clamp_zo(rg_percent(from, to, current));
}

RGINLINE void rg_swapf(f32* a, f32* b)
{
	f32 t = *a;
	*a = *b;
	*b = t;
}

// -----------------------------------------------------------------------------
// Sign and copysign
// -----------------------------------------------------------------------------

RGINLINE f32 rg_signf(f32 x)
{
	if (x > 0.0f) return 1.0f;
	if (x < 0.0f) return -1.0f;
	return 0.0f;
}

RGINLINE f32 rg_copysignf(f32 x, f32 y)
{
	rg_float_bits xb, yb;
	xb.f = x;
	yb.f = y;
	xb.u = (xb.u & 0x7FFFFFFF) | (yb.u & 0x80000000);
	return xb.f;
}

// -----------------------------------------------------------------------------
// Trig/log/exp/pow (math.h wrappers)
// -----------------------------------------------------------------------------

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
RGINLINE f32 rg_sinf(f32 x)
{
#ifdef RG_MATH_FAST_TRIG
	return rg_sinf_fast(x);
#else
	return sinf(x);
#endif
}

RGINLINE f32 rg_cosf(f32 x)
{
#ifdef RG_MATH_FAST_TRIG
	return rg_cosf_fast(x);
#else
	return cosf(x);
#endif
}
#endif

RGINLINE f32 rg_sinf_fast(f32 x)
{
#if RG_MATH_MAX_PERF
	f32 quotient = rg_roundf(x * (1.0f / RG_TAU));
	f32 y = x - RG_TAU * quotient;
	if (y > RG_HALF_PI)
	{
		y = RG_PI - y;
	}
	else if (y < -RG_HALF_PI)
	{
		y = -RG_PI - y;
	}

	f32 y2 = y * y;
	return y * (1.0f + y2 * (-0.1666666664f + y2 * (0.0083333315f + y2 * (-0.0001984090f + y2 * 0.0000027526f))));
#else
	return sinf(x);
#endif
}

RGINLINE f32 rg_cosf_fast(f32 x)
{
#if RG_MATH_MAX_PERF
	f32 quotient = rg_roundf(x * (1.0f / RG_TAU));
	f32 y = x - RG_TAU * quotient;
	f32 sign;
	if (y > RG_HALF_PI)
	{
		y = RG_PI - y;
		sign = -1.0f;
	}
	else if (y < -RG_HALF_PI)
	{
		y = -RG_PI - y;
		sign = -1.0f;
	}
	else
	{
		sign = 1.0f;
	}

	f32 y2 = y * y;
	f32 c = 1.0f + y2 * (-0.5f + y2 * (0.041666638f + y2 * (-0.0013888378f + y2 * 2.4760495e-05f)));
	return sign * c;
#else
	return cosf(x);
#endif
}

#if !defined(RG_MATH_HAS_BUILTIN_SINCOSF)
#if defined(__has_builtin)
#if __has_builtin(__builtin_sincosf)
#define RG_MATH_HAS_BUILTIN_SINCOSF 1
#endif
#endif
#endif

#if !defined(RG_MATH_HAS_BUILTIN_SINCOSF)
#if defined(__GNUC__) && !defined(_MSC_VER)
#define RG_MATH_HAS_BUILTIN_SINCOSF 1
#endif
#endif

RGINLINE void rg_sincosf(f32 x, f32* out_sin, f32* out_cos)
{
#if RG_MATH_MAX_PERF
	f32 quotient = x * (1.0f / RG_TAU);
	if (x >= 0.0f)
	{
		quotient = (f32)((int)(quotient + 0.5f));
	}
	else
	{
		quotient = (f32)((int)(quotient - 0.5f));
	}

	f32 y = x - RG_TAU * quotient;
	f32 sign;
	if (y > RG_HALF_PI)
	{
		y = RG_PI - y;
		sign = -1.0f;
	}
	else if (y < -RG_HALF_PI)
	{
		y = -RG_PI - y;
		sign = -1.0f;
	}
	else
	{
		sign = 1.0f;
	}

	f32 y2 = y * y;
	f32 s = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
	f32 c = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*out_sin = s;
	*out_cos = sign * c;
#elif defined(RG_MATH_HAS_BUILTIN_SINCOSF)
	__builtin_sincosf(x, out_sin, out_cos);
#else
	f32 s = sinf(x);
	f32 c = cosf(x);
	*out_sin = s;
	*out_cos = c;
#endif
}

#if RG_MATH_MAX_PERF && defined(RG_MATH_SSE)
RGINLINE void rg_sincosf4(__m128 x, __m128* out_sin, __m128* out_cos)
{
	const __m128 inv_tau = _mm_set1_ps(1.0f / RG_TAU);
	const __m128 half = _mm_set1_ps(0.5f);
	const __m128 tau = _mm_set1_ps(RG_TAU);
	const __m128 pi = _mm_set1_ps(RG_PI);
	const __m128 half_pi = _mm_set1_ps(RG_HALF_PI);
	const __m128 neg_zero = _mm_set1_ps(-0.0f);
	const __m128 one = _mm_set1_ps(1.0f);
	const __m128 neg_one = _mm_set1_ps(-1.0f);

	__m128 y;
	__m128 cos_sign;
	__m128 abs_x = _mm_andnot_ps(neg_zero, x);
	if (_mm_movemask_ps(_mm_cmple_ps(abs_x, half_pi)) == 0xF)
	{
		y = x;
		cos_sign = one;
	}
	else
	{
		__m128 quotient = _mm_mul_ps(x, inv_tau);
		__m128 q_positive = _mm_cvtepi32_ps(_mm_cvttps_epi32(_mm_add_ps(quotient, half)));
		__m128 q_negative = _mm_cvtepi32_ps(_mm_cvttps_epi32(_mm_sub_ps(quotient, half)));
		__m128 is_negative = _mm_cmplt_ps(x, _mm_setzero_ps());
		quotient = _mm_or_ps(_mm_and_ps(is_negative, q_negative), _mm_andnot_ps(is_negative, q_positive));

		y = _mm_sub_ps(x, _mm_mul_ps(tau, quotient));
		__m128 sign_bit = _mm_and_ps(y, neg_zero);
		__m128 signed_pi = _mm_or_ps(pi, sign_bit);
		__m128 abs_y = _mm_andnot_ps(neg_zero, y);
		__m128 reflected = _mm_sub_ps(signed_pi, y);
		__m128 in_range = _mm_cmple_ps(abs_y, half_pi);
		y = _mm_or_ps(_mm_and_ps(in_range, y), _mm_andnot_ps(in_range, reflected));

		cos_sign = _mm_or_ps(_mm_and_ps(in_range, one), _mm_andnot_ps(in_range, neg_one));
	}
	__m128 y2 = _mm_mul_ps(y, y);

	__m128 s = _mm_set1_ps(-0.00019840874f);
	s = RG_MATH_FMADD_PS(s, y2, _mm_set1_ps(0.0083333310f));
	s = RG_MATH_FMADD_PS(s, y2, _mm_set1_ps(-0.16666667f));
	s = RG_MATH_FMADD_PS(s, y2, _mm_set1_ps(1.0f));
	s = _mm_mul_ps(s, y);

	__m128 c = _mm_set1_ps(-0.0013888378f);
	c = RG_MATH_FMADD_PS(c, y2, _mm_set1_ps(0.041666638f));
	c = RG_MATH_FMADD_PS(c, y2, _mm_set1_ps(-0.5f));
	c = RG_MATH_FMADD_PS(c, y2, _mm_set1_ps(1.0f));
	c = _mm_mul_ps(c, cos_sign);

	*out_sin = s;
	*out_cos = c;
}
#endif

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
RGINLINE f32 rg_tanf(f32 x)
{
	return tanf(x);
}

RGINLINE f32 rg_asinf(f32 x)
{
#ifdef RG_MATH_FAST_TRIG
	return rg_asinf_fast(x);
#else
	return asinf(x);
#endif
}
#endif

RGINLINE f32 rg_asinf_fast(f32 x)
{
	return RG_HALF_PI - rg_acosf_fast(x);
}

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
RGINLINE f32 rg_acosf(f32 x)
{
#ifdef RG_MATH_FAST_TRIG
	return rg_acosf_fast(x);
#else
	return acosf(x);
#endif
}
#endif

RGINLINE f32 rg_acosf_fast(f32 x)
{
	f32 ax = rg_absf(x);
	f32 r = -0.0187293f;
	r = r * ax + 0.0742610f;
	r = r * ax - 0.2121144f;
	r = r * ax + 1.5707288f;
	r *= rg_sqrtf(1.0f - ax);
	return (x < 0.0f) ? (RG_PI - r) : r;
}

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
RGINLINE f32 rg_atanf(f32 x)
{
#ifdef RG_MATH_FAST_TRIG
	return rg_math_atanf_fast_impl(x);
#else
	return atanf(x);
#endif
}

RGINLINE f32 rg_atan2f(f32 y, f32 x)
{
#ifdef RG_MATH_FAST_TRIG
	return rg_atan2f_fast(y, x);
#else
	return atan2f(y, x);
#endif
}
#endif

RGINLINE f32 rg_math_atanf_fast_pos01(f32 x)
{
	f32 x2 = x * x;
	return x * (0.99997726f + x2 * (-0.33262347f + x2 * (0.19354346f + x2 * (-0.11643287f + x2 * (0.05265332f - x2 * 0.01172120f)))));
}

RGINLINE f32 rg_math_atanf_fast_impl(f32 x)
{
	f32 sign = 1.0f;
	if (x < 0.0f)
	{
		x = -x;
		sign = -1.0f;
	}
	int complement = x > 1.0f;
	if (complement)
	{
		x = 1.0f / x;
	}
	f32 y = rg_math_atanf_fast_pos01(x);
	if (complement)
	{
		y = RG_HALF_PI - y;
	}
	return sign * y;
}

RGINLINE f32 rg_atan2f_fast(f32 y, f32 x)
{
	if (x == 0.0f)
	{
		return (y > 0.0f) ? RG_HALF_PI : ((y < 0.0f) ? -RG_HALF_PI : 0.0f);
	}

	f32 ax = rg_absf(x);
	f32 ay = rg_absf(y);
	f32 r;
	if (ax >= ay)
	{
		r = rg_math_atanf_fast_pos01(ay / ax);
	}
	else
	{
		r = RG_HALF_PI - rg_math_atanf_fast_pos01(ax / ay);
	}
	if (x < 0.0f)
	{
		r = RG_PI - r;
	}
	return (y < 0.0f) ? -r : r;
}

#if !(RG_MATH_USE_LIBC && RG_MATH_LIBC_ALIASES)
RGINLINE f32 rg_expf(f32 x)
{
#ifdef RG_MATH_FAST_EXP
	return rg_expf_fast(x);
#else
	return expf(x);
#endif
}

RGINLINE f32 rg_logf(f32 x)
{
#ifdef RG_MATH_FAST_EXP
	return rg_logf_fast(x);
#else
	return logf(x);
#endif
}

RGINLINE f32 rg_log2f(f32 x)
{
#ifdef RG_MATH_FAST_EXP
	return rg_log2f_fast(x);
#else
	return log2f(x);
#endif
}

RGINLINE f32 rg_log10f(f32 x)
{
#ifdef RG_MATH_FAST_EXP
	return rg_log10f_fast(x);
#else
	return log10f(x);
#endif
}

RGINLINE f32 rg_powf(f32 x, f32 y)
{
#ifdef RG_MATH_FAST_EXP
	return rg_powf_fast(x, y);
#else
	return powf(x, y);
#endif
}
#endif

RGINLINE f32 rg_expf_fast(f32 x)
{
	f32 y = x * RG_INV_LN2;
	y = rg_clampf(y, -126.0f, 128.0f);
	rg_float_bits conv;
	conv.i = (i32)(y * 8388608.0f + 1065353216.0f);
	return conv.f;
}

RGINLINE f32 rg_log2f_fast(f32 x)
{
	rg_float_bits conv;
	conv.f = x;
	i32 ex = (i32)(conv.u >> 23) - 127;
	conv.u = (conv.u & 0x7FFFFF) | 0x3F800000;
	f32 t = conv.f - 1.0f;
	f32 ln = t - 0.5f * t * t + 0.333333343f * t * t * t;
	return (f32)ex + ln * RG_INV_LN2;
}

RGINLINE f32 rg_logf_fast(f32 x)
{
	return rg_log2f_fast(x) * RG_LN2;
}

RGINLINE f32 rg_log10f_fast(f32 x)
{
	return rg_log2f_fast(x) * RG_INV_LOG2_10;
}

RGINLINE f32 rg_powf_fast(f32 x, f32 y)
{
	return rg_expf_fast(y * rg_logf_fast(x));
}

// =============================================================================

#endif // RG_MATH_SCALAR_H
