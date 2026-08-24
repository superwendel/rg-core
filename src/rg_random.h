// rg_random - Deterministic random generation and distributions for C
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header xoshiro256** generator with uniform and distribution helpers.
//
// USAGE:
//   #include "rg_random.h"
//
//   RgRng rng;
//   rg_rng_seed(&rng, 1234);
//
//   u32 v = rg_random_u32(&rng);
//   f32 f = rg_random_range_f32(&rng, -1.0f, 1.0f);
//   f32 n = rg_random_normal_f32(&rng, 0.0f, 1.0f);
//
// OPTIONS:
//   #define RG_RANDOM_ASSERT(x)            - Custom assert macro
//   #define RG_RANDOM_SEED                 - Caller default seed (default: 0)
//   #define RG_RANDOM_FORCE_PORTABLE_MUL128 - Disable native 128-bit multiply
//
// NOTES:
//   - The generator is xoshiro256** seeded through splitmix64.
//   - This library is deterministic but is not cryptographically secure.
//   - Each RNG state may be used by only one thread at a time.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_RANDOM_H
#define RG_RANDOM_H

#include "rg_defs.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#if !defined(RG_RANDOM_FORCE_PORTABLE_MUL128)
#if RG_COMPILER_MSVC && RG_ARCH_X64
#include <intrin.h>
#define RG_RANDOM_USE_UMUL128 1
#elif defined(__SIZEOF_INT128__)
#define RG_RANDOM_USE_INT128 1
#endif
#endif

// =============================================================================
// CONFIGURATION
// =============================================================================

#ifndef RG_RANDOM_ASSERT
#include <assert.h>
#define RG_RANDOM_ASSERT(condition) assert(condition)
#endif

#ifndef RG_RANDOM_SEED
#define RG_RANDOM_SEED 0
#endif

// =============================================================================
// TYPE DEFINITIONS
// =============================================================================

/**
 * @brief xoshiro256** RNG state
 */
typedef struct RgRng
{
	u64 state[4];
	u32 has_spare;
	f32 spare;
} RgRng;

/**
 * @brief Cached normal distribution state
 */
typedef struct RgNormalCache
{
	u32 has_spare;
	f32 spare;
} RgNormalCache;

// =============================================================================
// PUBLIC API - Core RNG
// =============================================================================

/**
 * @brief Seed a RgRng from a 64-bit seed (splitmix64)
 * @param rng RNG state
 * @param seed 64-bit seed
 */
RGINLINE void rg_rng_seed(RgRng* rng, u64 seed);

/**
 * @brief Seed a RgRng from a 4x64 state (must not be all-zero)
 * @param rng RNG state
 * @param seed_state 4x64 seed values
 */
RGINLINE void rg_rng_seed_state(RgRng* rng, const u64 seed_state[4]);

/**
 * @brief Next 64-bit value from xoshiro256**
 * @param rng RNG state
 * @return 64-bit random value
 */
RGINLINE u64 rg_rng_next_u64(RgRng* rng);

/**
 * @brief Next 32-bit value from xoshiro256**
 * @param rng RNG state
 * @return 32-bit random value
 */
RGINLINE u32 rg_rng_next_u32(RgRng* rng);

// =============================================================================
// PUBLIC API - Uniform Helpers
// =============================================================================

/**
 * @brief Uniform random u32
 */
RGINLINE u32 rg_random_u32(RgRng* rng);

/**
 * @brief Uniform random u64
 */
RGINLINE u64 rg_random_u64(RgRng* rng);

/**
 * @brief Uniform random f32 in [0, 1)
 */
RGINLINE f32 rg_random_f32(RgRng* rng);

/**
 * @brief Uniform random f64 in [0, 1)
 */
RGINLINE f64 rg_random_f64(RgRng* rng);

/**
 * @brief Unbiased random u32 in [0, bound)
 */
RGINLINE u32 rg_random_bounded_u32(RgRng* rng, u32 bound);

/**
 * @brief Unbiased random u64 in [0, bound)
 */
RGINLINE u64 rg_random_bounded_u64(RgRng* rng, u64 bound);

/**
 * @brief Random u32 in [min, max] (inclusive)
 */
RGINLINE u32 rg_random_range_u32(RgRng* rng, u32 min, u32 max);

/**
 * @brief Random u64 in [min, max] (inclusive)
 */
RGINLINE u64 rg_random_range_u64(RgRng* rng, u64 min, u64 max);

/**
 * @brief Random i32 in [min, max] (inclusive)
 */
RGINLINE i32 rg_random_range_i32(RgRng* rng, i32 min, i32 max);

/**
 * @brief Random i64 in [min, max] (inclusive)
 */
RGINLINE i64 rg_random_range_i64(RgRng* rng, i64 min, i64 max);

/**
 * @brief Random f32 in [min, max)
 */
RGINLINE f32 rg_random_range_f32(RgRng* rng, f32 min, f32 max);

/**
 * @brief Random f64 in [min, max)
 */
RGINLINE f64 rg_random_range_f64(RgRng* rng, f64 min, f64 max);

/**
 * @brief Random boolean (0 or 1)
 */
RGINLINE int rg_random_bool(RgRng* rng);

/**
 * @brief Random sign (-1 or +1)
 */
RGINLINE int rg_random_sign(RgRng* rng);

/**
 * @brief Shuffle array in-place (Fisher-Yates)
 * @param data Pointer to array
 * @param count Element count
 * @param stride Size of each element in bytes
 */
RGINLINE void rg_random_shuffle(void* data, size_t count, size_t stride, RgRng* rng);

/**
 * @brief Fill buffer with random bytes
 * @param data Destination buffer
 * @param size Number of bytes
 */
RGINLINE void rg_random_fill_bytes(void* data, size_t size, RgRng* rng);

// =============================================================================
// PUBLIC API - Distributions
// =============================================================================

/**
 * @brief Generate two normal (Gaussian) samples
 * @param mean Mean of the distribution
 * @param stddev Standard deviation (must be > 0)
 * @param out0 First output
 * @param out1 Second output
 */
RGINLINE void rg_random_normal2_f32(RgRng* rng, f32 mean, f32 stddev, f32* out0, f32* out1);

/**
 * @brief Reset cached normal state
 * @param cache Cache state
 */
RGINLINE void rg_random_normal_cache_reset(RgNormalCache* cache);

/**
 * @brief Normal (Gaussian) sample
 * @param mean Mean of the distribution
 * @param stddev Standard deviation (must be > 0)
 * @return Sample from N(mean, stddev)
 */
RGINLINE f32 rg_random_normal_f32(RgRng* rng, f32 mean, f32 stddev);

/**
 * @brief Normal (Gaussian) sample with cached spare
 * @param mean Mean of the distribution
 * @param stddev Standard deviation (must be > 0)
 * @param cache Cache state for spare sample
 * @return Sample from N(mean, stddev)
 */
RGINLINE f32 rg_random_normal_f32_cached(RgRng* rng, RgNormalCache* cache, f32 mean, f32 stddev);

/**
 * @brief Exponential sample
 * @param lambda Rate parameter (must be > 0)
 * @return Sample from exponential distribution
 */
RGINLINE f32 rg_random_exponential_f32(RgRng* rng, f32 lambda);

// =============================================================================
// IMPLEMENTATION
// =============================================================================

#define RG_RANDOM_SPLITMIX64_GAMMA 0x9E3779B97F4A7C15ULL
#define RG_RANDOM_SPLITMIX64_M1    0xBF58476D1CE4E5B9ULL
#define RG_RANDOM_SPLITMIX64_M2    0x94D049BB133111EBULL
#define RG_RANDOM_PI_F32           3.14159265358979323846f

RGINLINE u64 rg_random_rotl64(u64 value, int r)
{
	return (value << r) | (value >> (64 - r));
}

RGINLINE u64 rg_random_splitmix64_next(u64* state)
{
	u64 z = (*state += RG_RANDOM_SPLITMIX64_GAMMA);
	z = (z ^ (z >> 30)) * RG_RANDOM_SPLITMIX64_M1;
	z = (z ^ (z >> 27)) * RG_RANDOM_SPLITMIX64_M2;
	return z ^ (z >> 31);
}

RGINLINE u64 rg_random_mul_u64_wide(u64 a, u64 b, u64* low)
{
#if defined(RG_RANDOM_USE_UMUL128)
	u64 high;
	*low = _umul128(a, b, &high);
	return high;
#elif defined(RG_RANDOM_USE_INT128)
	unsigned __int128 product = (unsigned __int128)a * (unsigned __int128)b;
	*low = (u64)product;
	return (u64)(product >> 64);
#else
	u64 a0 = (u32)a;
	u64 a1 = a >> 32;
	u64 b0 = (u32)b;
	u64 b1 = b >> 32;

	u64 product = a0 * b0;
	u64 word0 = (u32)product;
	u64 carry = product >> 32;

	product = a1 * b0 + carry;
	u64 word1 = (u32)product;
	u64 word2 = product >> 32;

	product = a0 * b1 + word1;
	*low = (product << 32) + word0;
	return a1 * b1 + word2 + (product >> 32);
#endif
}

RGINLINE i32 rg_random_u32_to_i32(u32 value)
{
	if (value <= (u32)INT32_MAX)
	{
		return (i32)value;
	}
	return INT32_MIN + (i32)(value - ((u32)INT32_MAX + 1u));
}

RGINLINE i64 rg_random_u64_to_i64(u64 value)
{
	if (value <= (u64)INT64_MAX)
	{
		return (i64)value;
	}
	return INT64_MIN + (i64)(value - ((u64)INT64_MAX + UINT64_C(1)));
}

RGINLINE void rg_rng_seed(RgRng* rng, u64 seed)
{
	RG_RANDOM_ASSERT(rng != NULL);
	u64 sm = seed;
	for (size_t i = 0; i < 4; i++)
	{
		rng->state[i] = rg_random_splitmix64_next(&sm);
	}

	if ((rng->state[0] | rng->state[1] | rng->state[2] | rng->state[3]) == 0)
	{
		rng->state[0] = 1;
	}

	rng->has_spare = 0;
	rng->spare = 0.0f;
}

RGINLINE void rg_rng_seed_state(RgRng* rng, const u64 seed_state[4])
{
	RG_RANDOM_ASSERT(rng != NULL);
	RG_RANDOM_ASSERT(seed_state != NULL);
	rng->state[0] = seed_state[0];
	rng->state[1] = seed_state[1];
	rng->state[2] = seed_state[2];
	rng->state[3] = seed_state[3];

	if ((rng->state[0] | rng->state[1] | rng->state[2] | rng->state[3]) == 0)
	{
		rng->state[0] = 1;
	}

	rng->has_spare = 0;
	rng->spare = 0.0f;
}

RGINLINE u64 rg_rng_next_u64(RgRng* rng)
{
	u64* s = rng->state;
	const u64 result = rg_random_rotl64(s[1] * 5ULL, 7) * 9ULL;
	const u64 t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;
	s[3] = rg_random_rotl64(s[3], 45);

	return result;
}

RGINLINE u32 rg_rng_next_u32(RgRng* rng)
{
	return (u32)(rg_rng_next_u64(rng) >> 32);
}

RGINLINE u32 rg_random_u32(RgRng* rng)
{
	return rg_rng_next_u32(rng);
}

RGINLINE u64 rg_random_u64(RgRng* rng)
{
	return rg_rng_next_u64(rng);
}

RGINLINE f32 rg_random_f32(RgRng* rng)
{
	u32 value = rg_rng_next_u32(rng);
	return (f32)(value >> 8) * (1.0f / 16777216.0f);
}

RGINLINE f64 rg_random_f64(RgRng* rng)
{
	u64 value = rg_rng_next_u64(rng);
	return (f64)(value >> 11) * (1.0 / 9007199254740992.0);
}

RGINLINE u32 rg_random_bounded_u32(RgRng* rng, u32 bound)
{
	if (bound == 0)
	{
		return 0;
	}

	u32 threshold = (u32)(0u - bound) % bound;
	for (;;)
	{
		u32 value = rg_rng_next_u32(rng);
		u64 product = (u64)value * (u64)bound;
		u32 low = (u32)product;
		if (low >= threshold)
		{
			return (u32)(product >> 32);
		}
	}
}

RGINLINE u64 rg_random_bounded_u64(RgRng* rng, u64 bound)
{
	if (bound == 0)
	{
		return 0;
	}

	u64 threshold = (u64)(0ULL - bound) % bound;
	for (;;)
	{
		u64 value = rg_rng_next_u64(rng);
		u64 low;
		u64 high = rg_random_mul_u64_wide(value, bound, &low);
		if (low >= threshold)
		{
			return high;
		}
	}
}

RGINLINE u32 rg_random_range_u32(RgRng* rng, u32 min, u32 max)
{
	RG_RANDOM_ASSERT(min <= max);
	if (min == max)
	{
		return min;
	}
	if (min == 0 && max == UINT32_MAX)
	{
		return rg_rng_next_u32(rng);
	}
	return min + rg_random_bounded_u32(rng, (max - min) + 1u);
}

RGINLINE u64 rg_random_range_u64(RgRng* rng, u64 min, u64 max)
{
	RG_RANDOM_ASSERT(min <= max);
	if (min == max)
	{
		return min;
	}
	if (min == 0 && max == UINT64_MAX)
	{
		return rg_rng_next_u64(rng);
	}
	return min + rg_random_bounded_u64(rng, (max - min) + 1ULL);
}

RGINLINE i32 rg_random_range_i32(RgRng* rng, i32 min, i32 max)
{
	RG_RANDOM_ASSERT(min <= max);
	if (min == max)
	{
		return min;
	}
	if (min == INT32_MIN && max == INT32_MAX)
	{
		return rg_random_u32_to_i32(rg_rng_next_u32(rng));
	}

	u32 umin = (u32)min;
	u32 umax = (u32)max;
	u32 range = (umax - umin) + 1u;
	return rg_random_u32_to_i32(umin + rg_random_bounded_u32(rng, range));
}

RGINLINE i64 rg_random_range_i64(RgRng* rng, i64 min, i64 max)
{
	RG_RANDOM_ASSERT(min <= max);
	if (min == max)
	{
		return min;
	}
	if (min == INT64_MIN && max == INT64_MAX)
	{
		return rg_random_u64_to_i64(rg_rng_next_u64(rng));
	}

	u64 umin = (u64)min;
	u64 umax = (u64)max;
	u64 range = (umax - umin) + 1ULL;
	return rg_random_u64_to_i64(umin + rg_random_bounded_u64(rng, range));
}

RGINLINE f32 rg_random_range_f32(RgRng* rng, f32 min, f32 max)
{
	RG_RANDOM_ASSERT(min <= max);
	return min + (max - min) * rg_random_f32(rng);
}

RGINLINE f64 rg_random_range_f64(RgRng* rng, f64 min, f64 max)
{
	RG_RANDOM_ASSERT(min <= max);
	return min + (max - min) * rg_random_f64(rng);
}

RGINLINE int rg_random_bool(RgRng* rng)
{
	return (int)(rg_rng_next_u32(rng) & 1u);
}

RGINLINE int rg_random_sign(RgRng* rng)
{
	return rg_random_bool(rng) ? 1 : -1;
}

RGINLINE void rg_random_shuffle(void* data, size_t count, size_t stride, RgRng* rng)
{
	RG_RANDOM_ASSERT(data != NULL || count == 0);
	RG_RANDOM_ASSERT(stride == 0 || count <= SIZE_MAX / stride);
	if (count <= 1 || stride == 0)
	{
		return;
	}
	if (count > SIZE_MAX / stride)
	{
		return;
	}

	u8* base = (u8*)data;
	for (size_t i = count - 1; i > 0; i--)
	{
		size_t j = (size_t)rg_random_range_u64(rng, 0, (u64)i);
		if (i == j)
		{
			continue;
		}

		u8* a = base + i * stride;
		u8* b = base + j * stride;
		for (size_t k = 0; k < stride; k++)
		{
			u8 tmp = a[k];
			a[k] = b[k];
			b[k] = tmp;
		}
	}
}

RGINLINE void rg_random_fill_bytes(void* data, size_t size, RgRng* rng)
{
	RG_RANDOM_ASSERT(data != NULL || size == 0);
	u8* dst = (u8*)data;
	size_t i = 0;
	while (size - i >= 8)
	{
		u64 value = rg_rng_next_u64(rng);
		dst[i + 0] = (u8)(value);
		dst[i + 1] = (u8)(value >> 8);
		dst[i + 2] = (u8)(value >> 16);
		dst[i + 3] = (u8)(value >> 24);
		dst[i + 4] = (u8)(value >> 32);
		dst[i + 5] = (u8)(value >> 40);
		dst[i + 6] = (u8)(value >> 48);
		dst[i + 7] = (u8)(value >> 56);
		i += 8;
	}
	if (i < size)
	{
		u64 value = rg_rng_next_u64(rng);
		for (size_t j = 0; i < size; i++, j++)
		{
			dst[i] = (u8)(value >> (j * 8));
		}
	}
}

RGINLINE void rg_random_normal2_f32(RgRng* rng, f32 mean, f32 stddev, f32* out0, f32* out1)
{
	RG_RANDOM_ASSERT(stddev > 0.0f);
	RG_RANDOM_ASSERT(out0 != NULL);
	RG_RANDOM_ASSERT(out1 != NULL);
	rng->has_spare = 0;

	f32 u1 = rg_random_f32(rng);
	f32 u2 = rg_random_f32(rng);
	if (u1 < 1.0e-7f)
	{
		u1 = 1.0e-7f;
	}

	f32 r = sqrtf(-2.0f * logf(u1));
	f32 theta = 2.0f * RG_RANDOM_PI_F32 * u2;
	f32 z0 = r * cosf(theta);
	f32 z1 = r * sinf(theta);

	*out0 = mean + z0 * stddev;
	*out1 = mean + z1 * stddev;
}

RGINLINE void rg_random_normal_cache_reset(RgNormalCache* cache)
{
	RG_RANDOM_ASSERT(cache != NULL);
	cache->has_spare = 0;
	cache->spare = 0.0f;
}

RGINLINE f32 rg_random_normal_f32(RgRng* rng, f32 mean, f32 stddev)
{
	RG_RANDOM_ASSERT(stddev > 0.0f);
	if (rng->has_spare)
	{
		rng->has_spare = 0;
		return mean + rng->spare * stddev;
	}

	f32 u1 = rg_random_f32(rng);
	f32 u2 = rg_random_f32(rng);
	if (u1 < 1.0e-7f)
	{
		u1 = 1.0e-7f;
	}

	f32 r = sqrtf(-2.0f * logf(u1));
	f32 theta = 2.0f * RG_RANDOM_PI_F32 * u2;
	f32 z0 = r * cosf(theta);
	f32 z1 = r * sinf(theta);

	rng->spare = z1;
	rng->has_spare = 1;
	return mean + z0 * stddev;
}

RGINLINE f32 rg_random_normal_f32_cached(RgRng* rng, RgNormalCache* cache, f32 mean, f32 stddev)
{
	RG_RANDOM_ASSERT(stddev > 0.0f);
	RG_RANDOM_ASSERT(cache != NULL);

	if (cache->has_spare)
	{
		cache->has_spare = 0;
		return mean + cache->spare * stddev;
	}

	f32 u1 = rg_random_f32(rng);
	f32 u2 = rg_random_f32(rng);
	if (u1 < 1.0e-7f)
	{
		u1 = 1.0e-7f;
	}

	f32 r = sqrtf(-2.0f * logf(u1));
	f32 theta = 2.0f * RG_RANDOM_PI_F32 * u2;
	f32 z0 = r * cosf(theta);
	f32 z1 = r * sinf(theta);

	cache->spare = z1;
	cache->has_spare = 1;
	return mean + z0 * stddev;
}

RGINLINE f32 rg_random_exponential_f32(RgRng* rng, f32 lambda)
{
	RG_RANDOM_ASSERT(lambda > 0.0f);
	f32 u = rg_random_f32(rng);
	if (u < 1.0e-7f)
	{
		u = 1.0e-7f;
	}
	return -logf(u) / lambda;
}

#undef RG_RANDOM_USE_UMUL128
#undef RG_RANDOM_USE_INT128
#undef RG_RANDOM_SPLITMIX64_GAMMA
#undef RG_RANDOM_SPLITMIX64_M1
#undef RG_RANDOM_SPLITMIX64_M2
#undef RG_RANDOM_PI_F32

#endif // RG_RANDOM_H
