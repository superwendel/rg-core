// rg_sprintf - Fast, portable printf-style formatting for C
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header sprintf-style formatter.
//
// USAGE:
//   #include "rg_sprintf.h"
//   // All functions are static - just include and use
//
// OPTIONS:
//   #define RG_SPRINTF_NO_SIMD     - Disable SIMD optimizations
//   #define RG_SPRINTF_SECURE      - Enable security checks (null checks, width/precision limits)
//   #define RG_SPRINTF_ASSERT(x)   - Custom assert macro
//
// Author: Steven Wendel (superwendel)

#ifndef RG_SPRINTF_H
#define RG_SPRINTF_H

#include "rg_defs.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <float.h>
#include <math.h>

// All functions are static for unity build compatibility

// =============================================================================
// PUBLIC API
// =============================================================================

/**
 * @brief Format string to buffer (like sprintf)
 * @param buf Destination buffer
 * @param fmt Printf-style format string
 * @return Number of characters written (excluding null terminator)
 */
static int rg_sprintf(char* buf, const char* fmt, ...);

/**
 * @brief Format string to buffer with size limit (like snprintf)
 * @param buf Destination buffer
 * @param count Maximum number of characters to write (including null)
 * @param fmt Printf-style format string
 * @return Number of characters that would have been written (excluding null)
 */
static int rg_snprintf(char* buf, size_t count, const char* fmt, ...);

/**
 * @brief Format string to buffer using va_list
 * @param buf Destination buffer
 * @param fmt Printf-style format string
 * @param args Variable argument list
 * @return Number of characters written (excluding null terminator)
 */
static int rg_vsprintf(char* buf, const char* fmt, va_list args);

/**
 * @brief Format string to buffer with size limit using va_list
 * @param buf Destination buffer
 * @param count Maximum number of characters to write (including null)
 * @param fmt Printf-style format string
 * @param args Variable argument list
 * @return Number of characters that would have been written (excluding null)
 */
static int rg_vsnprintf(char* buf, size_t count, const char* fmt, va_list args);

/** @brief Callback type for custom output chunks (files, network, etc.) */
typedef void (*rg_sprintf_callback)(const char* buf, void* user, int len);

/**
 * @brief Format string with callback for custom output
 * @param cb Callback function called with formatted chunks
 * @param user User data passed to callback
 * @param fmt Printf-style format string
 * @return Number of characters written
 */
static int rg_sprintf_cb(rg_sprintf_callback cb, void* user, const char* fmt, ...);

/**
 * @brief Format string with callback using va_list
 * @param cb Callback function called with formatted chunks
 * @param user User data passed to callback
 * @param fmt Printf-style format string
 * @param args Variable argument list
 * @return Number of characters written
 */
static int rg_vsprintf_cb(rg_sprintf_callback cb, void* user, const char* fmt, va_list args);

/**
 * @brief Fast signed 32-bit integer to string
 * @param value Integer value to convert
 * @param buf Destination buffer (must hold at least 12 bytes)
 * @return Pointer to end of string (one past last char)
 */
RGINLINE char* rg_itoa(int32_t value, char* buf);

/**
 * @brief Fast unsigned 32-bit integer to string
 * @param value Integer value to convert
 * @param buf Destination buffer (must hold at least 11 bytes)
 * @return Pointer to end of string (one past last char)
 */
RGINLINE char* rg_utoa(uint32_t value, char* buf);

/**
 * @brief Fast signed 64-bit integer to string
 * @param value Integer value to convert
 * @param buf Destination buffer (must hold at least 21 bytes)
 * @return Pointer to end of string (one past last char)
 */
RGINLINE char* rg_i64toa(int64_t value, char* buf);

/**
 * @brief Fast unsigned 64-bit integer to string
 * @param value Integer value to convert
 * @param buf Destination buffer (must hold at least 21 bytes)
 * @return Pointer to end of string (one past last char)
 */
RGINLINE char* rg_u64toa(uint64_t value, char* buf);

/**
 * @brief Convert binary data to hex string
 * @param data Source data
 * @param len Number of bytes to convert
 * @param out Output buffer (must hold at least len*2+1 bytes)
 * @param uppercase Use uppercase hex digits (A-F) if non-zero
 * @return Pointer to end of string (null terminator position)
 */
RGINLINE char* rg_to_hex(const void* data, size_t len, char* out, int uppercase);

/**
 * @brief Convert hex string to binary data
 * @param hex Hex string (must be even length)
 * @param hex_len Length of hex string
 * @param out Output buffer (must hold at least hex_len/2 bytes)
 * @return Number of bytes written, or -1 on error
 */
RGINLINE int rg_from_hex(const char* hex, size_t hex_len, void* out);

/**
 * @brief Fast float to string
 * @param value Float value to convert
 * @param buf Destination buffer
 * @param precision Number of decimal places
 * @return Pointer to end of string (one past last char)
 */
RGINLINE char* rg_ftoa(float value, char* buf, int precision);

/**
 * @brief Fast double to string
 * @param value Double value to convert
 * @param buf Destination buffer
 * @param precision Number of decimal places
 * @return Pointer to end of string (one past last char)
 */
RGINLINE char* rg_dtoa(double value, char* buf, int precision);

/** @brief String builder for efficient concatenation */
typedef struct
{
	char* buf;
	size_t len;
	size_t cap;
} RgBuilder;

/**
 * @brief Initialize a string builder
 * @param b Builder to initialize
 * @param buf Buffer to use for building
 * @param cap Capacity of buffer
 */
RGINLINE void rg_builder_init(RgBuilder* b, char* buf, size_t cap);

/**
 * @brief Append a null-terminated string
 * @param b Builder
 * @param str String to append
 */
RGINLINE void rg_builder_append(RgBuilder* b, const char* str);

/**
 * @brief Append a string with explicit length
 * @param b Builder
 * @param str String to append
 * @param n Number of characters to append
 */
RGINLINE void rg_builder_appendn(RgBuilder* b, const char* str, size_t n);

/**
 * @brief Append formatted string (printf-style)
 * @param b Builder
 * @param fmt Format string
 */
static void rg_builder_appendf(RgBuilder* b, const char* fmt, ...);

/**
 * @brief Append a single character
 * @param b Builder
 * @param c Character to append
 */
RGINLINE void rg_builder_append_char(RgBuilder* b, char c);

/**
 * @brief Append a signed 64-bit integer
 * @param b Builder
 * @param value Integer value
 */
RGINLINE void rg_builder_append_int(RgBuilder* b, int64_t value);

/**
 * @brief Append an unsigned 64-bit integer
 * @param b Builder
 * @param value Integer value
 */
RGINLINE void rg_builder_append_uint(RgBuilder* b, uint64_t value);

/**
 * @brief Append binary data as lowercase hex
 * @param b Builder
 * @param data Source data
 * @param len Number of bytes to convert
 */
RGINLINE void rg_builder_append_hex(RgBuilder* b, const void* data, size_t len);

/**
 * @brief Append a floating-point number
 * @param b Builder
 * @param value Double value
 * @param precision Number of decimal places
 */
RGINLINE void rg_builder_append_float(RgBuilder* b, double value, int precision);

/**
 * @brief Finish building and return length
 * @param b Builder
 * @return Length of built string (excluding null terminator)
 */
RGINLINE size_t rg_builder_finish(RgBuilder* b);

/**
 * @brief Reset builder for reuse
 * @param b Builder
 */
RGINLINE void rg_builder_reset(RgBuilder* b);

// =============================================================================
// IMPLEMENTATION
// =============================================================================

#ifndef RG_SPRINTF_ASSERT
#include <assert.h>
#define RG_SPRINTF_ASSERT(x) assert(x)
#endif

// SIMD detection and includes
#ifndef RG_SPRINTF_NO_SIMD
#if defined(_MSC_VER)
#include <intrin.h>
#define RG_HAS_SSE2 1
#if defined(__AVX2__) || (defined(_MSC_VER) && defined(__AVX2__))
#define RG_HAS_AVX2 1
#endif
#elif defined(__GNUC__) || defined(__clang__)
#if defined(__SSE2__)
#include <emmintrin.h>
#define RG_HAS_SSE2 1
#endif
#if defined(__SSSE3__)
#include <tmmintrin.h>
#define RG_HAS_SSSE3 1
#endif
#if defined(__AVX2__)
#include <immintrin.h>
#define RG_HAS_AVX2 1
#endif
#endif
#endif

// -----------------------------------------------------------------------------
// Lookup Tables
// -----------------------------------------------------------------------------

// Two-digit decimal pairs: "00", "01", ..., "99"
// Wrapped in struct for 2-byte alignment (enables uint16_t reads)
static struct
{
	short temp; // Force 2-byte alignment
	char pair[201];
} rg_digit_pairs_aligned =
    {
        0,
        "00010203040506070809"
        "10111213141516171819"
        "20212223242526272829"
        "30313233343536373839"
        "40414243444546474849"
        "50515253545556575859"
        "60616263646566676869"
        "70717273747576777879"
        "80818283848586878889"
        "90919293949596979899"};
#define rg_digit_pairs rg_digit_pairs_aligned.pair

// Hex lookup tables (no null terminator - used as lookup, not strings)
static const char rg_hex_upper[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
static const char rg_hex_lower[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

// Hex decode table (-1 = invalid)
static const int8_t rg_hex_decode[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,           // 0-9
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // A-F
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, // a-f
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

// Powers of 10 for float conversion
static const double rg_pow10_pos[20] = {
    1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9,
    1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19};

static const double rg_pow10_neg[20] = {
    1e0, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9,
    1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15, 1e-16, 1e-17, 1e-18, 1e-19};

// -----------------------------------------------------------------------------
// Internal Helpers
// -----------------------------------------------------------------------------

// Count digits in a 32-bit unsigned integer
RGINLINE int rg_count_digits_u32(uint32_t n)
{
	if (n < 10) return 1;
	if (n < 100) return 2;
	if (n < 1000) return 3;
	if (n < 10000) return 4;
	if (n < 100000) return 5;
	if (n < 1000000) return 6;
	if (n < 10000000) return 7;
	if (n < 100000000) return 8;
	if (n < 1000000000) return 9;
	return 10;
}

// Count digits in a 64-bit unsigned integer
RGINLINE int rg_count_digits_u64(uint64_t n)
{
	if (n < 10ULL) return 1;
	if (n < 100ULL) return 2;
	if (n < 1000ULL) return 3;
	if (n < 10000ULL) return 4;
	if (n < 100000ULL) return 5;
	if (n < 1000000ULL) return 6;
	if (n < 10000000ULL) return 7;
	if (n < 100000000ULL) return 8;
	if (n < 1000000000ULL) return 9;
	if (n < 10000000000ULL) return 10;
	if (n < 100000000000ULL) return 11;
	if (n < 1000000000000ULL) return 12;
	if (n < 10000000000000ULL) return 13;
	if (n < 100000000000000ULL) return 14;
	if (n < 1000000000000000ULL) return 15;
	if (n < 10000000000000000ULL) return 16;
	if (n < 100000000000000000ULL) return 17;
	if (n < 1000000000000000000ULL) return 18;
	if (n < 10000000000000000000ULL) return 19;
	return 20;
}

// -----------------------------------------------------------------------------
// Integer Conversion
// -----------------------------------------------------------------------------

// Fast unsigned 32-bit to string - writes backwards using 2-digit pairs
// Uses 16-bit writes for speed (single memory op instead of two)
RGINLINE char* rg_utoa(uint32_t value, char* buf)
{
#ifdef RG_SPRINTF_SECURE
	if (!buf) return NULL;
#endif
	char* p = buf;

	// Handle 0 specially
	if (value == 0)
	{
		*p++ = '0';
		*p = '\0';
		return p;
	}

	// Count digits and position pointer at end
	int digits = rg_count_digits_u32(value);
	p += digits;
	*p = '\0';
	char* end = p;

	// Write 2 digits at a time
	while (value >= 100)
	{
		uint32_t idx = (value % 100) * 2;
		value /= 100;
		p -= 2;
		memcpy(p, &rg_digit_pairs[idx], 2);
	}

	// Handle remaining 1-2 digits
	if (value >= 10)
	{
		p -= 2;
		memcpy(p, &rg_digit_pairs[value * 2], 2);
	}
	else
	{
		*--p = '0' + (char)value;
	}

	return end;
}

// Fast signed 32-bit to string
RGINLINE char* rg_itoa(int32_t value, char* buf)
{
#ifdef RG_SPRINTF_SECURE
	if (!buf) return NULL;
#endif
	if (value < 0)
	{
		*buf++ = '-';
		// Handle INT32_MIN specially to avoid overflow
		if (value == INT32_MIN)
		{
			return rg_utoa((uint32_t)INT32_MAX + 1, buf);
		}
		return rg_utoa((uint32_t)(-value), buf);
	}
	return rg_utoa((uint32_t)value, buf);
}

// Helper: write exactly 8 digits (for 10^8 chunks)
RGINLINE void rg_write_8_digits(char* p, uint32_t n)
{
	// Write 4 pairs of digits = 8 digits total
	uint32_t q, idx;

	q = n / 100;
	idx = (n - q * 100) * 2;
	memcpy(p + 6, &rg_digit_pairs[idx], 2);
	n = q;

	q = n / 100;
	idx = (n - q * 100) * 2;
	memcpy(p + 4, &rg_digit_pairs[idx], 2);
	n = q;

	q = n / 100;
	idx = (n - q * 100) * 2;
	memcpy(p + 2, &rg_digit_pairs[idx], 2);
	n = q;

	memcpy(p, &rg_digit_pairs[n * 2], 2);
}

// Fast unsigned 64-bit to string - uses 10^8 chunking to minimize 64-bit divisions
RGINLINE char* rg_u64toa(uint64_t value, char* buf)
{
#ifdef RG_SPRINTF_SECURE
	if (!buf) return NULL;
#endif
	char* p = buf;

	if (value == 0)
	{
		*p++ = '0';
		*p = '\0';
		return p;
	}

	// Use 32-bit path for values that fit
	if (value <= UINT32_MAX)
	{
		return rg_utoa((uint32_t)value, buf);
	}

	int digits = rg_count_digits_u64(value);
	p += digits;
	*p = '\0';
	char* end = p;

	// Process in 10^8 chunks to use fast 32-bit division
	while (value >= 100000000ULL)
	{
		uint32_t chunk = (uint32_t)(value % 100000000ULL);
		value /= 100000000ULL;
		p -= 8;
		rg_write_8_digits(p, chunk);
	}

	// Handle remaining value (now fits in 32 bits)
	uint32_t n32 = (uint32_t)value;

	// Write remaining digits
	while (n32 >= 100)
	{
		uint32_t idx = (n32 % 100) * 2;
		n32 /= 100;
		p -= 2;
		memcpy(p, &rg_digit_pairs[idx], 2);
	}

	if (n32 >= 10)
	{
		p -= 2;
		memcpy(p, &rg_digit_pairs[n32 * 2], 2);
	}
	else
	{
		*--p = '0' + (char)n32;
	}

	return end;
}

// Fast signed 64-bit to string
RGINLINE char* rg_i64toa(int64_t value, char* buf)
{
#ifdef RG_SPRINTF_SECURE
	if (!buf) return NULL;
#endif
	if (value < 0)
	{
		*buf++ = '-';
		if (value == INT64_MIN)
		{
			return rg_u64toa((uint64_t)INT64_MAX + 1, buf);
		}
		return rg_u64toa((uint64_t)(-value), buf);
	}
	return rg_u64toa((uint64_t)value, buf);
}

// -----------------------------------------------------------------------------
// Hex Conversion
// -----------------------------------------------------------------------------

// Scalar hex encoding
RGINLINE char* rg_to_hex_scalar(const uint8_t* data, size_t len, char* out, const char* hex_table)
{
	for (size_t i = 0; i < len; i++)
	{
		uint8_t byte = data[i];
		*out++ = hex_table[byte >> 4];
		*out++ = hex_table[byte & 0x0F];
	}
	*out = '\0';
	return out;
}

#if defined(RG_HAS_SSSE3)
// SIMD hex encoding using SSSE3 pshufb
RGINLINE char* rg_to_hex_simd(const uint8_t* data, size_t len, char* out, int uppercase)
{
	__m128i hex_lut = uppercase
	                      ? _mm_setr_epi8('0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F')
	                      : _mm_setr_epi8('0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f');
	__m128i mask_lo = _mm_set1_epi8(0x0F);

	char* p = out;
	size_t i = 0;

	// Process 16 bytes at a time -> 32 hex chars
	for (; i + 16 <= len; i += 16)
	{
		__m128i input = _mm_loadu_si128((__m128i*)(data + i));

		// Split into high and low nibbles
		__m128i lo = _mm_and_si128(input, mask_lo);
		__m128i hi = _mm_and_si128(_mm_srli_epi16(input, 4), mask_lo);

		// Lookup hex chars
		__m128i hex_lo = _mm_shuffle_epi8(hex_lut, lo);
		__m128i hex_hi = _mm_shuffle_epi8(hex_lut, hi);

		// Interleave high and low
		__m128i result_lo = _mm_unpacklo_epi8(hex_hi, hex_lo);
		__m128i result_hi = _mm_unpackhi_epi8(hex_hi, hex_lo);

		// Store results
		_mm_storeu_si128((__m128i*)p, result_lo);
		_mm_storeu_si128((__m128i*)(p + 16), result_hi);
		p += 32;
	}

	// Handle remaining bytes with scalar
	const char* hex_table = uppercase ? rg_hex_upper : rg_hex_lower;
	for (; i < len; i++)
	{
		uint8_t byte = data[i];
		*p++ = hex_table[byte >> 4];
		*p++ = hex_table[byte & 0x0F];
	}

	*p = '\0';
	return p;
}
#endif

RGINLINE char* rg_to_hex(const void* data, size_t len, char* out, int uppercase)
{
	const uint8_t* bytes = (const uint8_t*)data;

#if defined(RG_HAS_SSSE3)
	if (len >= 16)
	{
		return rg_to_hex_simd(bytes, len, out, uppercase);
	}
#endif

	const char* hex_table = uppercase ? rg_hex_upper : rg_hex_lower;
	return rg_to_hex_scalar(bytes, len, out, hex_table);
}

RGINLINE int rg_from_hex(const char* hex, size_t hex_len, void* out)
{
	if (hex_len % 2 != 0) return -1;

	uint8_t* bytes = (uint8_t*)out;
	size_t byte_len = hex_len / 2;

	for (size_t i = 0; i < byte_len; i++)
	{
		int8_t hi = rg_hex_decode[(uint8_t)hex[i * 2]];
		int8_t lo = rg_hex_decode[(uint8_t)hex[i * 2 + 1]];
		if (hi < 0 || lo < 0) return -1;
		bytes[i] = (uint8_t)((hi << 4) | lo);
	}

	return (int)byte_len;
}

// -----------------------------------------------------------------------------
// Float Conversion (Fast Approximate)
// -----------------------------------------------------------------------------

typedef union
{
	double d;
	uint64_t u;
} rg_double_bits;

RGINLINE char* rg_dtoa(double value, char* buf, int precision)
{
	char* p = buf;

	// Handle special cases
	rg_double_bits bits;
	bits.d = value;

	// Check for negative
	if (bits.u >> 63)
	{
		*p++ = '-';
		value = -value;
		bits.d = value;
	}

	// Extract exponent
	int exp = (int)((bits.u >> 52) & 0x7FF);

	// Check for infinity
	if (exp == 0x7FF)
	{
		uint64_t mantissa = bits.u & 0xFFFFFFFFFFFFFULL;
		if (mantissa == 0)
		{
			memcpy(p, "inf", 3);
			p += 3;
		}
		else
		{
			memcpy(p, "nan", 3);
			p += 3;
		}
		*p = '\0';
		return p;
	}

	// Handle zero
	if (value == 0.0)
	{
		*p++ = '0';
		if (precision > 0)
		{
			*p++ = '.';
			for (int i = 0; i < precision; i++)
			{
				*p++ = '0';
			}
		}
		*p = '\0';
		return p;
	}

	// Clamp precision
	if (precision < 0) precision = 6;
	if (precision > 17) precision = 17;

	// Normalize very large/small numbers
	int decimal_exp = 0;
	if (value >= 1e18)
	{
		while (value >= 1e18)
		{
			value /= 1e18;
			decimal_exp += 18;
		}
	}
	else if (value < 1e-17 && value > 0)
	{
		while (value < 1e-17)
		{
			value *= 1e18;
			decimal_exp -= 18;
		}
	}

	// Scale to integer range based on precision
	double scale = rg_pow10_pos[precision < 19 ? precision : 18];

	uint64_t integer_part = (uint64_t)value;
	double frac = value - (double)integer_part;
	uint64_t frac_scaled = (uint64_t)(frac * scale + 0.5);

	// Handle carry from fractional rounding
	uint64_t max_frac = (uint64_t)rg_pow10_pos[precision];
	if (frac_scaled >= max_frac)
	{
		frac_scaled -= max_frac;
		integer_part++;
	}

	// Write integer part
	p = rg_u64toa(integer_part, p);

	// Write fractional part
	if (precision > 0)
	{
		*p++ = '.';

		if (frac_scaled == 0)
		{
			// All zeros
			for (int i = 0; i < precision; i++)
			{
				*p++ = '0';
			}
		}
		else
		{
			// Count digits in frac_scaled to add leading zeros
			int frac_digits = rg_count_digits_u64(frac_scaled);
			int leading_zeros = precision - frac_digits;

			// Add leading zeros
			while (leading_zeros > 0)
			{
				*p++ = '0';
				leading_zeros--;
			}

			// Write fractional digits
			char frac_buf[21];
			char* frac_end = rg_u64toa(frac_scaled, frac_buf);
			size_t frac_len = frac_end - frac_buf;
			memcpy(p, frac_buf, frac_len);
			p += frac_len;
		}
	}

	// Handle scientific notation for extreme values
	if (decimal_exp != 0)
	{
		*p++ = 'e';
		p = rg_itoa(decimal_exp, p);
	}

	*p = '\0';
	return p;
}

RGINLINE char* rg_ftoa(float value, char* buf, int precision)
{
	return rg_dtoa((double)value, buf, precision);
}

RGINLINE char* rg_trim_float_zeros(char* begin, char* end)
{
	char* exponent = end;
	char* decimal = NULL;

	for (char* p = begin; p < end; p++)
	{
		if (*p == '.')
		{
			decimal = p;
		}
		else if (*p == 'e' || *p == 'E')
		{
			exponent = p;
			break;
		}
	}

	if (!decimal || decimal >= exponent)
	{
		return end;
	}

	char* trimmed = exponent;
	while (trimmed > decimal + 1 && trimmed[-1] == '0')
	{
		trimmed--;
	}
	if (trimmed == decimal + 1 && trimmed[-1] == '0')
	{
		trimmed--;
	}
	if (trimmed == decimal + 1)
	{
		trimmed--;
	}

	if (exponent < end)
	{
		size_t exponent_len = (size_t)(end - exponent);
		memmove(trimmed, exponent, exponent_len);
		trimmed += exponent_len;
	}

	*trimmed = '\0';
	return trimmed;
}

// -----------------------------------------------------------------------------
// sprintf Core
// -----------------------------------------------------------------------------

// Internal sprintf context
typedef struct
{
	char* buf;
	char* end;
	size_t count;
	rg_sprintf_callback callback;
	void* user;
	int has_limit;
	char tmp[512]; // Temp buffer for callback mode
} rg_sprintf_ctx;

// Flush callback buffer
RGINLINE void rg_flush(rg_sprintf_ctx* ctx)
{
	if (ctx->callback && ctx->buf > ctx->tmp)
	{
		ptrdiff_t len = ctx->buf - ctx->tmp;
		ctx->callback(ctx->tmp, ctx->user, (int)len);
		ctx->buf = ctx->tmp;
	}
}

// Word-at-a-time strlen - faster than libc for short strings
RGINLINE size_t rg_strlen(const char* s)
{
	const char* start = s;

	// Align to 4-byte boundary
	while ((uintptr_t)s & 3)
	{
		if (*s == 0) return (size_t)(s - start);
		s++;
	}

	// Scan 4 bytes at a time using null byte detection trick
	for (;;)
	{
		uint32_t v;
		memcpy(&v, s, sizeof(v)); // Strict aliasing safe - compilers optimize to direct load
		if ((v - 0x01010101U) & ~v & 0x80808080U) break;
		s += 4;
	}

	// Find exact position of null
	while (*s) s++;
	return (size_t)(s - start);
}

// Output a single character
RGINLINE void rg_putc(rg_sprintf_ctx* ctx, char c)
{
	if (ctx->callback)
	{
		if (ctx->buf >= ctx->tmp + sizeof(ctx->tmp) - 1)
		{
			rg_flush(ctx);
		}
		*ctx->buf++ = c;
	}
	else if (!ctx->has_limit)
	{
		*ctx->buf++ = c;
	}
	else if (ctx->buf < ctx->end)
	{
		*ctx->buf++ = c;
	}
	ctx->count++;
}

// Output a string - optimized bulk copy
RGINLINE void rg_puts(rg_sprintf_ctx* ctx, const char* s, size_t len)
{
	if (ctx->callback)
	{
		// Callback mode: may need to flush
		while (len > 0)
		{
			size_t avail = sizeof(ctx->tmp) - (size_t)(ctx->buf - ctx->tmp);
			if (avail == 0)
			{
				rg_flush(ctx);
				avail = sizeof(ctx->tmp);
			}
			size_t chunk = (len < avail) ? len : avail;
			memcpy(ctx->buf, s, chunk);
			ctx->buf += chunk;
			s += chunk;
			len -= chunk;
			ctx->count += chunk;
		}
	}
	else if (!ctx->has_limit)
	{
		memcpy(ctx->buf, s, len);
		ctx->buf += len;
		ctx->count += len;
	}
	else
	{
		// Direct buffer mode: fast path
		size_t avail = (size_t)(ctx->end - ctx->buf);
		size_t to_copy = (len < avail) ? len : avail;
		memcpy(ctx->buf, s, to_copy);
		ctx->buf += to_copy;
		ctx->count += len; // Count full length even if truncated
	}
}

// Output padding - optimized for common case
RGINLINE void rg_pad(rg_sprintf_ctx* ctx, char c, int count)
{
	if (count <= 0) return;

	if (ctx->callback)
	{
		while (count > 0)
		{
			rg_putc(ctx, c);
			count--;
		}
	}
	else if (!ctx->has_limit)
	{
		memset(ctx->buf, c, (size_t)count);
		ctx->buf += count;
		ctx->count += count;
	}
	else
	{
		// Direct buffer mode: fast path
		size_t avail = (size_t)(ctx->end - ctx->buf);
		size_t to_write = ((size_t)count < avail) ? (size_t)count : avail;
		memset(ctx->buf, c, to_write);
		ctx->buf += to_write;
		ctx->count += count;
	}
}

// Format flags
#define RG_FLAG_LEFT (1 << 0)
#define RG_FLAG_PLUS (1 << 1)
#define RG_FLAG_SPACE (1 << 2)
#define RG_FLAG_HASH (1 << 3)
#define RG_FLAG_ZERO (1 << 4)
#define RG_FLAG_LONG (1 << 5)
#define RG_FLAG_LLONG (1 << 6)

// Secure mode limits (only enforced with RG_SPRINTF_SECURE)
#ifdef RG_SPRINTF_SECURE
#define RG_MAX_WIDTH 4096
#define RG_MAX_PRECISION 1024
#endif

// Core vsnprintf implementation
static int rg_vsnprintf_internal(rg_sprintf_ctx* ctx, const char* fmt, va_list args)
{
	ctx->count = 0;

	if (!ctx->callback && !ctx->has_limit &&
	    fmt[0] == '%' && fmt[1] == 's' && fmt[2] == '\0')
	{
		char* str = va_arg(args, char*);
		if (!str) str = "(null)";

		// Tiny strings are common in logs/UI; avoid separate strlen + memcpy.
		char* out = ctx->buf;
		size_t len = 0;
		int done = 0;
		for (; len < 16; len++)
		{
			char c = str[len];
			out[len] = c;
			if (c == '\0')
			{
				done = 1;
				break;
			}
		}
		if (!done)
		{
			size_t tail_len = rg_strlen(str + 16);
			memcpy(out + 16, str + 16, tail_len);
			len += tail_len;
		}

		ctx->buf += len;
		ctx->count = len;
		return (int)ctx->count;
	}

	if (!ctx->has_limit && fmt[0] == '%' && fmt[2] == '\0')
	{
		if (fmt[1] == 'd' || fmt[1] == 'i')
		{
			int val = va_arg(args, int);
			char* start = ctx->buf;
			char* end = rg_itoa((int32_t)val, ctx->buf);
			ctx->buf = end;
			ctx->count = (size_t)(end - start);
			return (int)ctx->count;
		}

		if (fmt[1] == 'u')
		{
			unsigned int val = va_arg(args, unsigned int);
			char* start = ctx->buf;
			char* end = rg_utoa((uint32_t)val, ctx->buf);
			ctx->buf = end;
			ctx->count = (size_t)(end - start);
			return (int)ctx->count;
		}

		if (fmt[1] == 'x')
		{
			uint32_t val = va_arg(args, uint32_t);
			char tmp[16];
			char* p = tmp + sizeof(tmp) - 1;
			*p = '\0';
			if (val == 0)
			{
				*--p = '0';
			}
			else
			{
				while (val)
				{
					*--p = rg_hex_lower[val & 0xF];
					val >>= 4;
				}
			}

			size_t len = (size_t)((tmp + sizeof(tmp) - 1) - p);
			memcpy(ctx->buf, p, len);
			ctx->buf += len;
			ctx->count = len;
			return (int)ctx->count;
		}

		if (fmt[1] == 'o')
		{
			uint32_t val = va_arg(args, uint32_t);
			char tmp[16];
			char* p = tmp + sizeof(tmp) - 1;
			*p = '\0';
			if (val == 0)
			{
				*--p = '0';
			}
			else
			{
				while (val)
				{
					*--p = (char)('0' + (val & 7));
					val >>= 3;
				}
			}

			size_t len = (size_t)((tmp + sizeof(tmp) - 1) - p);
			memcpy(ctx->buf, p, len);
			ctx->buf += len;
			ctx->count = len;
			return (int)ctx->count;
		}
	}

	if (!ctx->callback && !ctx->has_limit && fmt[0] == '%' && fmt[1] == '1' &&
	    fmt[2] == '0' && fmt[3] == 's' && fmt[4] == '\0')
	{
		char* str = va_arg(args, char*);
		if (!str) str = "(null)";
		size_t len = 0;
		while (len < 10 && str[len] != '\0')
		{
			len++;
		}
		if (len == 10 && str[10] != '\0')
		{
			len += rg_strlen(str + 10);
		}

		char* out = ctx->buf;
		if (len >= 10)
		{
			memcpy(out, str, len);
			ctx->buf += len;
			ctx->count = len;
			return (int)ctx->count;
		}

		int pad = 10 - (int)len;
		memset(out, ' ', (size_t)pad);
		memcpy(out + pad, str, len);
		ctx->buf += 10;
		ctx->count = 10;
		return (int)ctx->count;
	}

	if (!ctx->callback && !ctx->has_limit && fmt[0] == '%' && fmt[1] == '-' &&
	    fmt[2] == '1' && fmt[3] == '0' && fmt[4] == 's' && fmt[5] == '\0')
	{
		char* str = va_arg(args, char*);
		if (!str) str = "(null)";
		size_t len = 0;
		while (len < 10 && str[len] != '\0')
		{
			len++;
		}
		if (len == 10 && str[10] != '\0')
		{
			len += rg_strlen(str + 10);
		}

		char* out = ctx->buf;
		if (len >= 10)
		{
			memcpy(out, str, len);
			ctx->buf += len;
			ctx->count = len;
			return (int)ctx->count;
		}

		memcpy(out, str, len);
		memset(out + len, ' ', 10 - len);
		ctx->buf += 10;
		ctx->count = 10;
		return (int)ctx->count;
	}

	if (!ctx->has_limit && fmt[0] == '%' && fmt[1] == '-' &&
	    fmt[2] == '8' && (fmt[3] == 'd' || fmt[3] == 'i') && fmt[4] == '\0')
	{
		int32_t val = va_arg(args, int);
		char* out = ctx->buf;
		int sign_len = 0;
		uint32_t uval;

		if (val < 0)
		{
			sign_len = 1;
			*out++ = '-';
			if (val == INT32_MIN)
			{
				uval = (uint32_t)INT32_MAX + 1;
			}
			else
			{
				uval = (uint32_t)(-val);
			}
		}
		else
		{
			uval = (uint32_t)val;
		}

		int digits = rg_count_digits_u32(uval);
		rg_utoa(uval, out);
		out += digits;

		int pad = 8 - (digits + sign_len);
		if (pad > 0)
		{
			size_t pad_len = (size_t)pad;
			memset(out, ' ', pad_len);
			out += pad_len;
		}

		ctx->count = (size_t)(out - ctx->buf);
		ctx->buf = out;
		return (int)ctx->count;
	}

	enum
	{
		RG_LEN_DEFAULT = 0,
		RG_LEN_LONG,
		RG_LEN_LLONG,
		RG_LEN_SIZE,
		RG_LEN_PTRDIFF,
		RG_LEN_INTMAX
	};

	while (*fmt)
	{
		if (*fmt != '%')
		{
			rg_putc(ctx, *fmt++);
			continue;
		}

		fmt++; // Skip '%'

		// Handle %%
		if (*fmt == '%')
		{
			rg_putc(ctx, '%');
			fmt++;
			continue;
		}

		// Parse flags
		int flags = 0;
		int length = RG_LEN_DEFAULT;
		while (1)
		{
			switch (*fmt)
			{
				case '-':
					flags |= RG_FLAG_LEFT;
					fmt++;
					continue;
				case '+':
					flags |= RG_FLAG_PLUS;
					fmt++;
					continue;
				case ' ':
					flags |= RG_FLAG_SPACE;
					fmt++;
					continue;
				case '#':
					flags |= RG_FLAG_HASH;
					fmt++;
					continue;
				case '0':
					flags |= RG_FLAG_ZERO;
					fmt++;
					continue;
			}
			break;
		}

		// Parse width
		int width = 0;
		if (*fmt == '*')
		{
			width = va_arg(args, int);
			if (width < 0)
			{
				flags |= RG_FLAG_LEFT;
				width = -width;
			}
			fmt++;
		}
		else
		{
			while (*fmt >= '0' && *fmt <= '9')
			{
				width = width * 10 + (*fmt++ - '0');
			}
		}
#ifdef RG_SPRINTF_SECURE
		if (width > RG_MAX_WIDTH) width = RG_MAX_WIDTH;
#endif

		// Parse precision
		int precision = -1;
		if (*fmt == '.')
		{
			fmt++;
			precision = 0;
			if (*fmt == '*')
			{
				precision = va_arg(args, int);
				if (precision < 0) precision = -1;
				fmt++;
			}
			else
			{
				while (*fmt >= '0' && *fmt <= '9')
				{
					precision = precision * 10 + (*fmt++ - '0');
				}
			}
		}
#ifdef RG_SPRINTF_SECURE
		if (precision > RG_MAX_PRECISION) precision = RG_MAX_PRECISION;
#endif

		// Parse length modifier
		while (1)
		{
			switch (*fmt)
			{
				case 'h': fmt++; continue; // Ignored for simplicity
				case 'l':
					if (flags & RG_FLAG_LONG)
					{
						flags |= RG_FLAG_LLONG;
						length = RG_LEN_LLONG;
					}
					else
					{
						flags |= RG_FLAG_LONG;
						length = RG_LEN_LONG;
					}
					fmt++;
					continue;
				case 'z':
					length = RG_LEN_SIZE;
					// size_t is 64-bit on 64-bit platforms
#if SIZE_MAX > UINT32_MAX
					flags |= RG_FLAG_LLONG;
#else
					flags |= RG_FLAG_LONG;
#endif
					fmt++;
					continue;
				case 't':
					length = RG_LEN_PTRDIFF;
					// ptrdiff_t is 64-bit on 64-bit platforms
#if PTRDIFF_MAX > INT32_MAX
					flags |= RG_FLAG_LLONG;
#else
					flags |= RG_FLAG_LONG;
#endif
					fmt++;
					continue;
				case 'j':
					length = RG_LEN_INTMAX;
					flags |= RG_FLAG_LLONG;
					fmt++;
					continue;
			}
			break;
		}

		// Handle format specifier
		char num_buf[64];
		char* num_end;
		char* num_buf_end = num_buf + sizeof(num_buf) - 1;
		const char* str;
		size_t str_len;
		char sign_char = 0;
		const char* prefix = "";
		int prefix_len = 0;
		int is_integer = 0;
		int is_zero = 0;
		int is_octal = 0;

		switch (*fmt++)
		{
			case 'd':
			case 'i':
			{
				// FAST PATH: simple %d with no formatting - bypass output_number
				if (width == 0 && precision < 0 && !(flags & (RG_FLAG_PLUS | RG_FLAG_SPACE | RG_FLAG_LLONG | RG_FLAG_LONG)))
				{
					int32_t val = va_arg(args, int);
					if (val >= 0)
					{
						char* end = rg_utoa((uint32_t)val, num_buf);
						size_t len = (size_t)(end - num_buf);
						if (!ctx->has_limit || ctx->buf + len <= ctx->end)
						{
							memcpy(ctx->buf, num_buf, len);
							ctx->buf += len;
							ctx->count += len;
							continue;
						}
						// Fall through to slow path if buffer full
						str = num_buf;
						str_len = len;
						is_integer = 1;
						is_zero = (str_len == 1 && str[0] == '0');
						goto output_number;
					}
					else
					{
						// Negative - handle sign + digits in fast path
						if (val == INT32_MIN)
						{
							num_end = rg_utoa((uint32_t)INT32_MAX + 1, num_buf);
						}
						else
						{
							num_end = rg_utoa((uint32_t)(-val), num_buf);
						}
						size_t len = (size_t)(num_end - num_buf);
						if (!ctx->has_limit || ctx->buf + len + 1 <= ctx->end)
						{
							*ctx->buf++ = '-';
							memcpy(ctx->buf, num_buf, len);
							ctx->buf += len;
							ctx->count += len + 1;
							continue;
						}
						// Fall through to slow path
						sign_char = '-';
						str = num_buf;
						str_len = len;
						is_integer = 1;
						is_zero = (str_len == 1 && str[0] == '0');
						goto output_number;
					}
				}

				// FAST PATH: simple %lld with no formatting
				if (width == 0 && precision < 0 && (flags & RG_FLAG_LLONG) &&
				    !(flags & (RG_FLAG_PLUS | RG_FLAG_SPACE)))
				{
					int64_t val = va_arg(args, int64_t);
					if (val >= 0)
					{
						char* end = rg_u64toa((uint64_t)val, num_buf);
						size_t len = (size_t)(end - num_buf);
						if (!ctx->has_limit || ctx->buf + len <= ctx->end)
						{
							memcpy(ctx->buf, num_buf, len);
							ctx->buf += len;
							ctx->count += len;
							continue;
						}
						// Fall through to slow path if buffer full
						str = num_buf;
						str_len = len;
						is_integer = 1;
						is_zero = (str_len == 1 && str[0] == '0');
						goto output_number;
					}
					else
					{
						// Negative - handle sign + digits in fast path
						if (val == INT64_MIN)
						{
							num_end = rg_u64toa((uint64_t)INT64_MAX + 1, num_buf);
						}
						else
						{
							num_end = rg_u64toa((uint64_t)(-val), num_buf);
						}
						size_t len = (size_t)(num_end - num_buf);
						if (!ctx->has_limit || ctx->buf + len + 1 <= ctx->end)
						{
							*ctx->buf++ = '-';
							memcpy(ctx->buf, num_buf, len);
							ctx->buf += len;
							ctx->count += len + 1;
							continue;
						}
						// Fall through to slow path
						sign_char = '-';
						str = num_buf;
						str_len = len;
						is_integer = 1;
						is_zero = (str_len == 1 && str[0] == '0');
						goto output_number;
					}
				}

				// Use 64-bit path for long long, or for long on LP64 systems
				if ((flags & RG_FLAG_LLONG) || ((flags & RG_FLAG_LONG) && sizeof(long) == 8))
				{
					int64_t val;
					if (flags & RG_FLAG_LLONG)
					{
						val = va_arg(args, int64_t);
					}
					else
					{
						val = va_arg(args, long); // 64-bit on LP64
					}
					if (val < 0)
					{
						sign_char = '-';
						if (val == INT64_MIN)
						{
							num_end = rg_u64toa((uint64_t)INT64_MAX + 1, num_buf);
						}
						else
						{
							num_end = rg_u64toa((uint64_t)(-val), num_buf);
						}
					}
					else
					{
						if (flags & RG_FLAG_PLUS) sign_char = '+';
						else if (flags & RG_FLAG_SPACE) sign_char = ' ';
						num_end = rg_u64toa((uint64_t)val, num_buf);
					}
				}
				else
				{
					// Use fast 32-bit path for int, or long on LLP64 (Windows)
					int32_t val;
					if (flags & RG_FLAG_LONG)
					{
						val = (int32_t)va_arg(args, long); // 32-bit on Windows
					}
					else
					{
						val = va_arg(args, int);
					}
					if (val < 0)
					{
						sign_char = '-';
						if (val == INT32_MIN)
						{
							num_end = rg_utoa((uint32_t)INT32_MAX + 1, num_buf);
						}
						else
						{
							num_end = rg_utoa((uint32_t)(-val), num_buf);
						}
					}
					else
					{
						if (flags & RG_FLAG_PLUS) sign_char = '+';
						else if (flags & RG_FLAG_SPACE) sign_char = ' ';
						num_end = rg_utoa((uint32_t)val, num_buf);
					}
				}
				str = num_buf;
				str_len = num_end - num_buf;
				is_integer = 1;
				is_zero = (str_len == 1 && str[0] == '0');
				goto output_number;
			}

			case 'u':
			{
				// FAST PATH: simple %u with no formatting
				if (width == 0 && precision < 0 && !(flags & (RG_FLAG_LLONG | RG_FLAG_LONG)))
				{
					uint32_t val = va_arg(args, unsigned int);
					char* end = rg_utoa(val, num_buf);
					size_t len = (size_t)(end - num_buf);
					if (!ctx->has_limit || ctx->buf + len <= ctx->end)
					{
						memcpy(ctx->buf, num_buf, len);
						ctx->buf += len;
						ctx->count += len;
						continue;
					}
					// Fall through to slow path
					str = num_buf;
					str_len = len;
					is_integer = 1;
					is_zero = (str_len == 1 && str[0] == '0');
					goto output_number;
				}

				// Use 64-bit path for long long, or for long on LP64 systems
				if ((flags & RG_FLAG_LLONG) || ((flags & RG_FLAG_LONG) && sizeof(long) == 8))
				{
					uint64_t val;
					if (flags & RG_FLAG_LLONG)
					{
						val = va_arg(args, uint64_t);
					}
					else
					{
						val = va_arg(args, unsigned long); // 64-bit on LP64
					}
					num_end = rg_u64toa(val, num_buf);
				}
				else if (flags & RG_FLAG_LONG)
				{
					num_end = rg_utoa((uint32_t)va_arg(args, unsigned long), num_buf); // 32-bit on Windows
				}
				else
				{
					num_end = rg_utoa(va_arg(args, unsigned int), num_buf);
				}
				str = num_buf;
				str_len = num_end - num_buf;
				is_integer = 1;
				is_zero = (str_len == 1 && str[0] == '0');
				goto output_number;
			}

			case 'x':
			case 'X':
			{
				const char* hex_table = (fmt[-1] == 'X') ? rg_hex_upper : rg_hex_lower;

				// FAST PATH: simple %x with no formatting
				if (width == 0 && precision < 0 && !(flags & (RG_FLAG_HASH | RG_FLAG_LLONG | RG_FLAG_LONG)))
				{
					uint32_t uval = va_arg(args, unsigned int);
					uint32_t orig = uval;
					// Write backwards into temp buffer
					char* hp = num_buf_end;
					if (uval == 0)
					{
						*--hp = '0';
					}
					else
					{
						while (uval)
						{
							*--hp = hex_table[uval & 0xF];
							uval >>= 4;
						}
					}
					size_t len = (size_t)(num_buf_end - hp);
					if (!ctx->has_limit || ctx->buf + len <= ctx->end)
					{
						memcpy(ctx->buf, hp, len);
						ctx->buf += len;
						ctx->count += len;
						continue;
					}
					// Fall through to slow path
					str = hp;
					str_len = len;
					is_integer = 1;
					is_zero = (orig == 0);
					goto output_number;
				}

				uint64_t val;
				if (flags & RG_FLAG_LLONG) val = va_arg(args, uint64_t);
				else if (flags & RG_FLAG_LONG) val = va_arg(args, unsigned long);
				else val = va_arg(args, unsigned int);
				uint64_t orig = val;

				// Write backwards to avoid reversal
				char* p = num_buf_end;
				if (val == 0)
				{
					*--p = '0';
				}
				else
				{
					while (val)
					{
						*--p = hex_table[val & 0xF];
						val >>= 4;
					}
				}

				if ((flags & RG_FLAG_HASH) && orig != 0)
				{
					prefix = (fmt[-1] == 'X') ? "0X" : "0x";
					prefix_len = 2;
				}

				str = p;
				str_len = (size_t)(num_buf_end - p);
				is_integer = 1;
				is_zero = (orig == 0);
				goto output_number;
			}

			case 'o':
			{
				uint64_t val;
				if (flags & RG_FLAG_LLONG) val = va_arg(args, uint64_t);
				else if (flags & RG_FLAG_LONG) val = va_arg(args, unsigned long);
				else val = va_arg(args, unsigned int);
				uint64_t orig = val;

				// Write backwards to avoid reversal
				char* p = num_buf_end;
				if (val == 0)
				{
					*--p = '0';
				}
				else
				{
					while (val)
					{
						*--p = '0' + (val & 7);
						val >>= 3;
					}
				}

				if ((flags & RG_FLAG_HASH) && *p != '0')
				{
					prefix = "0";
					prefix_len = 1;
				}

				str = p;
				str_len = (size_t)(num_buf_end - p);
				is_integer = 1;
				is_zero = (orig == 0);
				is_octal = 1;
				goto output_number;
			}

			case 'p':
			{
				void* ptr = va_arg(args, void*);
				uint64_t val = (uint64_t)(uintptr_t)ptr;

				if (val == 0)
				{
					str = "(nil)";
					str_len = 5;
					goto output_string;
				}

				// Write backwards to avoid reversal
				char* p = num_buf_end;
				while (val)
				{
					*--p = rg_hex_lower[val & 0xF];
					val >>= 4;
				}

				prefix = "0x";
				prefix_len = 2;
				str = p;
				str_len = (size_t)(num_buf_end - p);
				goto output_number;
			}

			case 'f':
			case 'F':
			{
				double val = va_arg(args, double);
				if (precision < 0) precision = 6;
				rg_double_bits bits;
				bits.d = val;

				if (bits.u >> 63)
				{
					sign_char = '-';
					val = -val;
				}
				else if (flags & RG_FLAG_PLUS)
				{
					sign_char = '+';
				}
				else if (flags & RG_FLAG_SPACE)
				{
					sign_char = ' ';
				}

				num_end = rg_dtoa(val, num_buf, precision);
				str = num_buf;
				str_len = (size_t)(num_end - num_buf);
				goto output_float;
			}

			case 'e':
			case 'E':
			{
				double val = va_arg(args, double);
				if (precision < 0) precision = 6;

				rg_double_bits bits;
				bits.d = val;

				if (bits.u >> 63)
				{
					sign_char = '-';
					val = -val;
				}
				else if (flags & RG_FLAG_PLUS)
				{
					sign_char = '+';
				}
				else if (flags & RG_FLAG_SPACE)
				{
					sign_char = ' ';
				}

				if (isinf(val) || isnan(val))
				{
					num_end = rg_dtoa(val, num_buf, precision);
					str = num_buf;
					str_len = (size_t)(num_end - num_buf);
					goto output_float;
				}

				int exp_val = 0;
				if (val != 0.0)
				{
					// Normalize to 1.xxx
					while (val >= 10.0)
					{
						val /= 10.0;
						exp_val++;
					}
					while (val < 1.0 && val > 0)
					{
						val *= 10.0;
						exp_val--;
					}
				}

				// Write mantissa
				char* p = num_buf;
				p = rg_dtoa(val, p, precision);

				// Write exponent
				*p++ = (fmt[-1] == 'E') ? 'E' : 'e';
				*p++ = (exp_val >= 0) ? '+' : '-';
				if (exp_val < 0) exp_val = -exp_val;
				if (exp_val < 10) *p++ = '0';
				p = rg_itoa(exp_val, p);

				str = num_buf;
				str_len = (size_t)(p - num_buf);
				goto output_float;
			}

			case 'g':
			case 'G':
			{
				double val = va_arg(args, double);
				if (precision < 0) precision = 6;
				if (precision == 0) precision = 1;

				rg_double_bits bits;
				bits.d = val;

				if (bits.u >> 63)
				{
					sign_char = '-';
					val = -val;
				}
				else if (flags & RG_FLAG_PLUS)
				{
					sign_char = '+';
				}
				else if (flags & RG_FLAG_SPACE)
				{
					sign_char = ' ';
				}

				if (isinf(val) || isnan(val))
				{
					num_end = rg_dtoa(val, num_buf, precision);
					str = num_buf;
					str_len = (size_t)(num_end - num_buf);
					goto output_float;
				}

				// Use %e if exponent < -4 or >= precision, else %f
				double abs_val = val;
				double norm = abs_val;
				int use_exp = 0;
				int exp_val = 0;

				if (abs_val != 0.0)
				{
					while (norm >= 10.0)
					{
						norm /= 10.0;
						exp_val++;
					}
					while (norm < 1.0 && norm > 0)
					{
						norm *= 10.0;
						exp_val--;
					}
					if (exp_val < -4 || exp_val >= precision) use_exp = 1;
				}

				if (use_exp)
				{
					char* p = num_buf;
					int exp_out = exp_val;

					p = rg_dtoa(norm, p, precision - 1);
					*p++ = (fmt[-1] == 'G') ? 'E' : 'e';
					*p++ = (exp_out >= 0) ? '+' : '-';
					if (exp_out < 0) exp_out = -exp_out;
					if (exp_out < 10) *p++ = '0';
					p = rg_itoa(exp_out, p);
					if (!(flags & RG_FLAG_HASH))
					{
						p = rg_trim_float_zeros(num_buf, p);
					}

					str = num_buf;
					str_len = (size_t)(p - num_buf);
				}
				else
				{
					// For %g with %f style, precision is significant digits
					// decimal_places = precision - (integer digits)
					// integer digits = exp_val + 1 for values >= 1, or 0 for values < 1
					int decimal_places = precision - exp_val - 1;
					if (decimal_places < 0) decimal_places = 0;
					num_end = rg_dtoa(val, num_buf, decimal_places);
					if (!(flags & RG_FLAG_HASH))
					{
						num_end = rg_trim_float_zeros(num_buf, num_end);
					}
					str = num_buf;
					str_len = (size_t)(num_end - num_buf);
				}
				goto output_float;
			}

			case 's':
			{
				str = va_arg(args, char*);
				if (!str) str = "(null)";

				// FAST PATH: simple %s with no width/precision
				if (width == 0 && precision < 0)
				{
					if (!ctx->callback && !ctx->has_limit)
					{
						// Tiny strings are common in logs/UI; avoid separate strlen + memcpy.
						char* out = ctx->buf;
						size_t len = 0;
						int done = 0;
						for (; len < 16; len++)
						{
							char c = str[len];
							out[len] = c;
							if (c == '\0')
							{
								done = 1;
								break;
							}
						}
						if (!done)
						{
							size_t tail_len = rg_strlen(str + 16);
							memcpy(out + 16, str + 16, tail_len);
							len += tail_len;
						}

						ctx->buf += len;
						ctx->count += len;
						continue;
					}

					size_t len = 0;
					for (; len < 16 && str[len] != '\0'; len++)
					{
					}
					if (len == 16)
					{
						len += rg_strlen(str + 16);
					}
					if (ctx->callback)
					{
						rg_puts(ctx, str, len);
						continue;
					}
					if (!ctx->has_limit)
					{
						memcpy(ctx->buf, str, len);
						ctx->buf += len;
						ctx->count += len;
						continue;
					}
					size_t avail = (size_t)(ctx->end - ctx->buf);
					size_t to_copy = (len < avail) ? len : avail;
					memcpy(ctx->buf, str, to_copy);
					ctx->buf += to_copy;
					ctx->count += len;
					continue;
				}

				// Slow path with width/precision
				str_len = strlen(str);
				if (precision >= 0 && str_len > (size_t)precision)
				{
					str_len = precision;
				}
				goto output_string;
			}

			case 'c':
			{
				num_buf[0] = (char)va_arg(args, int);
				str = num_buf;
				str_len = 1;
				goto output_string;
			}

			case 'n':
			{
				switch (length)
				{
					case RG_LEN_LONG:
					{
						long* n = va_arg(args, long*);
						if (n) *n = (long)ctx->count;
						break;
					}
					case RG_LEN_LLONG:
					{
						long long* n = va_arg(args, long long*);
						if (n) *n = (long long)ctx->count;
						break;
					}
					case RG_LEN_SIZE:
					{
						size_t* n = va_arg(args, size_t*);
						if (n) *n = (size_t)ctx->count;
						break;
					}
					case RG_LEN_PTRDIFF:
					{
						ptrdiff_t* n = va_arg(args, ptrdiff_t*);
						if (n) *n = (ptrdiff_t)ctx->count;
						break;
					}
					case RG_LEN_INTMAX:
					{
						intmax_t* n = va_arg(args, intmax_t*);
						if (n) *n = (intmax_t)ctx->count;
						break;
					}
					default:
					{
						int* n = va_arg(args, int*);
						if (n) *n = (int)ctx->count;
						break;
					}
				}
				continue;
			}

			default:
				// Unknown specifier, output as-is
				rg_putc(ctx, '%');
				rg_putc(ctx, fmt[-1]);
				continue;
		}

	output_number:
	{
		// C standard: 0 flag is ignored when precision is specified for integers
		if (precision >= 0)
		{
			flags &= ~RG_FLAG_ZERO;
		}

		if (is_integer && precision == 0 && is_zero && !(is_octal && (flags & RG_FLAG_HASH)))
		{
			str_len = 0;
		}

		int total_len = prefix_len + (int)str_len;
		if (sign_char) total_len++;

		// Apply precision for numbers (minimum digits)
		int zero_pad = 0;
		if (precision > (int)str_len)
		{
			zero_pad = precision - (int)str_len;
			total_len += zero_pad;
		}

		int pad_len = (width > total_len) ? width - total_len : 0;

		// Left padding with spaces
		if (!(flags & RG_FLAG_LEFT) && !(flags & RG_FLAG_ZERO))
		{
			rg_pad(ctx, ' ', pad_len);
		}

		// Sign
		if (sign_char) rg_putc(ctx, sign_char);

		// Prefix
		rg_puts(ctx, prefix, prefix_len);

		// Zero padding
		if (!(flags & RG_FLAG_LEFT) && (flags & RG_FLAG_ZERO))
		{
			rg_pad(ctx, '0', pad_len);
		}

		// Precision zero padding
		rg_pad(ctx, '0', zero_pad);

		// Number
		rg_puts(ctx, str, str_len);

		// Right padding
		if (flags & RG_FLAG_LEFT)
		{
			rg_pad(ctx, ' ', pad_len);
		}
		continue;
	}

	output_float:
	{
		int total_len = (int)str_len + (sign_char ? 1 : 0);
		int pad_len = (width > total_len) ? width - total_len : 0;

		if (!(flags & RG_FLAG_LEFT) && !(flags & RG_FLAG_ZERO))
		{
			rg_pad(ctx, ' ', pad_len);
		}

		if (sign_char) rg_putc(ctx, sign_char);

		if (!(flags & RG_FLAG_LEFT) && (flags & RG_FLAG_ZERO))
		{
			rg_pad(ctx, '0', pad_len);
		}

		rg_puts(ctx, str, str_len);

		if (flags & RG_FLAG_LEFT)
		{
			rg_pad(ctx, ' ', pad_len);
		}
		continue;
	}

	output_string:
	{
		int pad_len = (width > (int)str_len) ? width - (int)str_len : 0;

		// C standard: 0 flag has no effect on strings
		if (!(flags & RG_FLAG_LEFT))
		{
			rg_pad(ctx, ' ', pad_len);
		}

		rg_puts(ctx, str, str_len);

		if (flags & RG_FLAG_LEFT)
		{
			rg_pad(ctx, ' ', pad_len);
		}
		continue;
	}
	}

	// Flush any remaining callback buffer
	rg_flush(ctx);

	return (int)ctx->count;
}

static int rg_vsnprintf(char* buf, size_t count, const char* fmt, va_list args)
{
#ifdef RG_SPRINTF_SECURE
	if ((!buf && count > 0) || !fmt) return -1;
#endif
	rg_sprintf_ctx ctx;
	ctx.callback = NULL;
	ctx.user = NULL;
	ctx.has_limit = 1;

	char dummy;
	if (count == 0)
	{
		ctx.buf = &dummy;
		ctx.end = &dummy;
	}
	else
	{
		ctx.buf = buf;
		ctx.end = buf + count - 1; // Leave room for null terminator
	}

	int result = rg_vsnprintf_internal(&ctx, fmt, args);

	// Null terminate
	if (count > 0)
	{
		*ctx.buf = '\0';
	}

	return result;
}

static int rg_vsprintf(char* buf, const char* fmt, va_list args)
{
#ifdef RG_SPRINTF_SECURE
	if (!buf || !fmt) return -1;
#endif
	rg_sprintf_ctx ctx;
	ctx.buf = buf;
	ctx.end = NULL;
	ctx.callback = NULL;
	ctx.user = NULL;
	ctx.has_limit = 0;

	int result = rg_vsnprintf_internal(&ctx, fmt, args);
	*ctx.buf = '\0';

	return result;
}

static int rg_sprintf(char* buf, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int result = rg_vsprintf(buf, fmt, args);
	va_end(args);
	return result;
}

static int rg_snprintf(char* buf, size_t count, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int result = rg_vsnprintf(buf, count, fmt, args);
	va_end(args);
	return result;
}

static int rg_vsprintf_cb(rg_sprintf_callback cb, void* user, const char* fmt, va_list args)
{
#ifdef RG_SPRINTF_SECURE
	if (!cb || !fmt) return -1;
#endif
	rg_sprintf_ctx ctx;
	ctx.buf = ctx.tmp;
	ctx.end = ctx.tmp + sizeof(ctx.tmp);
	ctx.callback = cb;
	ctx.user = user;
	ctx.has_limit = 1;

	int result = rg_vsnprintf_internal(&ctx, fmt, args);
	return result;
}

static int rg_sprintf_cb(rg_sprintf_callback cb, void* user, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int result = rg_vsprintf_cb(cb, user, fmt, args);
	va_end(args);
	return result;
}

// -----------------------------------------------------------------------------
// String Builder
// -----------------------------------------------------------------------------

RGINLINE void rg_builder_init(RgBuilder* b, char* buf, size_t cap)
{
#ifdef RG_SPRINTF_SECURE
	RG_SPRINTF_ASSERT(b != NULL && "rg_builder_init: null builder");
	RG_SPRINTF_ASSERT(buf != NULL && "rg_builder_init: null buffer");
#endif
	b->buf = buf;
	b->len = 0;
	b->cap = cap;
	if (cap > 0) buf[0] = '\0';
}

RGINLINE void rg_builder_reset(RgBuilder* b)
{
	b->len = 0;
	if (b->cap > 0) b->buf[0] = '\0';
}

RGINLINE void rg_builder_append_char(RgBuilder* b, char c)
{
	if (b->len + 1 < b->cap)
	{
		b->buf[b->len++] = c;
		b->buf[b->len] = '\0';
	}
}

RGINLINE void rg_builder_appendn(RgBuilder* b, const char* str, size_t n)
{
	size_t available = (b->cap > b->len + 1) ? b->cap - b->len - 1 : 0;
	size_t to_copy = (n < available) ? n : available;

	if (to_copy > 0)
	{
		memcpy(b->buf + b->len, str, to_copy);
		b->len += to_copy;
		b->buf[b->len] = '\0';
	}
}

RGINLINE void rg_builder_append(RgBuilder* b, const char* str)
{
	rg_builder_appendn(b, str, strlen(str));
}

static void rg_builder_appendf(RgBuilder* b, const char* fmt, ...)
{
	size_t available = (b->cap > b->len) ? b->cap - b->len : 0;

	va_list args;
	va_start(args, fmt);
	int written = rg_vsnprintf(b->buf + b->len, available, fmt, args);
	va_end(args);

	if (written > 0)
	{
		b->len += (size_t)written;
		if (b->len >= b->cap) b->len = b->cap - 1;
	}
}

RGINLINE void rg_builder_append_int(RgBuilder* b, int64_t value)
{
	char tmp[24];
	char* end = rg_i64toa(value, tmp);
	rg_builder_appendn(b, tmp, end - tmp);
}

RGINLINE void rg_builder_append_uint(RgBuilder* b, uint64_t value)
{
	char tmp[24];
	char* end = rg_u64toa(value, tmp);
	rg_builder_appendn(b, tmp, end - tmp);
}

RGINLINE void rg_builder_append_hex(RgBuilder* b, const void* data, size_t len)
{
	size_t hex_len = len * 2;
	size_t available = (b->cap > b->len + 1) ? b->cap - b->len - 1 : 0;

	if (hex_len <= available)
	{
		rg_to_hex(data, len, b->buf + b->len, 0);
		b->len += hex_len;
	}
	else
	{
		// Partial write
		size_t bytes = available / 2;
		if (bytes > 0)
		{
			rg_to_hex(data, bytes, b->buf + b->len, 0);
			b->len += bytes * 2;
		}
	}
}

RGINLINE void rg_builder_append_float(RgBuilder* b, double value, int precision)
{
	char tmp[64];
	char* end = rg_dtoa(value, tmp, precision);
	rg_builder_appendn(b, tmp, end - tmp);
}

RGINLINE size_t rg_builder_finish(RgBuilder* b)
{
	if (b->cap > 0 && b->len < b->cap)
	{
		b->buf[b->len] = '\0';
	}
	return b->len;
}

// End of rg_sprintf.h
#endif // RG_SPRINTF_H
