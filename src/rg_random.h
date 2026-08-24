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
//   uint32_t v = rg_random_u32(&rng);
//   float f = rg_random_range_f32(&rng, -1.0f, 1.0f);
//   float n = rg_random_normal_f32(&rng, 0.0f, 1.0f);
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
	uint64_t state[4];
	uint32_t has_spare;
	float spare;
} RgRng;

/**
 * @brief Cached normal distribution state
 */
typedef struct RgNormalCache
{
	uint32_t has_spare;
	float spare;
} RgNormalCache;

// =============================================================================
// PUBLIC API - Core RNG
// =============================================================================

/**
 * @brief Seed a RgRng from a 64-bit seed (splitmix64)
 * @param rng RNG state
 * @param seed 64-bit seed
 */
RGINLINE void rg_rng_seed(RgRng* rng, uint64_t seed);

/**
 * @brief Seed a RgRng from a 4x64 state (must not be all-zero)
 * @param rng RNG state
 * @param seed_state 4x64 seed values
 */
RGINLINE void rg_rng_seed_state(RgRng* rng, const uint64_t seed_state[4]);

/**
 * @brief Next 64-bit value from xoshiro256**
 * @param rng RNG state
 * @return 64-bit random value
 */
RGINLINE uint64_t rg_rng_next_u64(RgRng* rng);

/**
 * @brief Next 32-bit value from xoshiro256**
 * @param rng RNG state
 * @return 32-bit random value
 */
RGINLINE uint32_t rg_rng_next_u32(RgRng* rng);

// =============================================================================
// PUBLIC API - Uniform Helpers
// =============================================================================

/**
 * @brief Uniform random uint32
 */
RGINLINE uint32_t rg_random_u32(RgRng* rng);

/**
 * @brief Uniform random uint64
 */
RGINLINE uint64_t rg_random_u64(RgRng* rng);

/**
 * @brief Uniform random float in [0, 1)
 */
RGINLINE float rg_random_f32(RgRng* rng);

/**
 * @brief Uniform random double in [0, 1)
 */
RGINLINE double rg_random_f64(RgRng* rng);

/**
 * @brief Unbiased random uint32 in [0, bound)
 */
RGINLINE uint32_t rg_random_bounded_u32(RgRng* rng, uint32_t bound);

/**
 * @brief Unbiased random uint64 in [0, bound)
 */
RGINLINE uint64_t rg_random_bounded_u64(RgRng* rng, uint64_t bound);

/**
 * @brief Random uint32 in [min, max] (inclusive)
 */
RGINLINE uint32_t rg_random_range_u32(RgRng* rng, uint32_t min, uint32_t max);

/**
 * @brief Random uint64 in [min, max] (inclusive)
 */
RGINLINE uint64_t rg_random_range_u64(RgRng* rng, uint64_t min, uint64_t max);

/**
 * @brief Random int32 in [min, max] (inclusive)
 */
RGINLINE int32_t rg_random_range_i32(RgRng* rng, int32_t min, int32_t max);

/**
 * @brief Random int64 in [min, max] (inclusive)
 */
RGINLINE int64_t rg_random_range_i64(RgRng* rng, int64_t min, int64_t max);

/**
 * @brief Random float in [min, max)
 */
RGINLINE float rg_random_range_f32(RgRng* rng, float min, float max);

/**
 * @brief Random double in [min, max)
 */
RGINLINE double rg_random_range_f64(RgRng* rng, double min, double max);

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
RGINLINE void rg_random_normal2_f32(RgRng* rng, float mean, float stddev, float* out0, float* out1);

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
RGINLINE float rg_random_normal_f32(RgRng* rng, float mean, float stddev);

/**
 * @brief Normal (Gaussian) sample with cached spare
 * @param mean Mean of the distribution
 * @param stddev Standard deviation (must be > 0)
 * @param cache Cache state for spare sample
 * @return Sample from N(mean, stddev)
 */
RGINLINE float rg_random_normal_f32_cached(RgRng* rng, RgNormalCache* cache, float mean, float stddev);

/**
 * @brief Exponential sample
 * @param lambda Rate parameter (must be > 0)
 * @return Sample from exponential distribution
 */
RGINLINE float rg_random_exponential_f32(RgRng* rng, float lambda);

// =============================================================================
// IMPLEMENTATION
// =============================================================================

#define RG_RANDOM_SPLITMIX64_GAMMA 0x9E3779B97F4A7C15ULL
#define RG_RANDOM_SPLITMIX64_M1    0xBF58476D1CE4E5B9ULL
#define RG_RANDOM_SPLITMIX64_M2    0x94D049BB133111EBULL
#define RG_RANDOM_PI_F32           3.14159265358979323846f

RGINLINE uint64_t rg_random_rotl64(uint64_t value, int r)
{
	return (value << r) | (value >> (64 - r));
}

RGINLINE uint64_t rg_random_splitmix64_next(uint64_t* state)
{
	uint64_t z = (*state += RG_RANDOM_SPLITMIX64_GAMMA);
	z = (z ^ (z >> 30)) * RG_RANDOM_SPLITMIX64_M1;
	z = (z ^ (z >> 27)) * RG_RANDOM_SPLITMIX64_M2;
	return z ^ (z >> 31);
}

RGINLINE uint64_t rg_random_mul_u64_wide(uint64_t a, uint64_t b, uint64_t* low)
{
#if defined(RG_RANDOM_USE_UMUL128)
	uint64_t high;
	*low = _umul128(a, b, &high);
	return high;
#elif defined(RG_RANDOM_USE_INT128)
	unsigned __int128 product = (unsigned __int128)a * (unsigned __int128)b;
	*low = (uint64_t)product;
	return (uint64_t)(product >> 64);
#else
	uint64_t a0 = (uint32_t)a;
	uint64_t a1 = a >> 32;
	uint64_t b0 = (uint32_t)b;
	uint64_t b1 = b >> 32;

	uint64_t product = a0 * b0;
	uint64_t word0 = (uint32_t)product;
	uint64_t carry = product >> 32;

	product = a1 * b0 + carry;
	uint64_t word1 = (uint32_t)product;
	uint64_t word2 = product >> 32;

	product = a0 * b1 + word1;
	*low = (product << 32) + word0;
	return a1 * b1 + word2 + (product >> 32);
#endif
}

RGINLINE int32_t rg_random_u32_to_i32(uint32_t value)
{
	if (value <= (uint32_t)INT32_MAX)
	{
		return (int32_t)value;
	}
	return INT32_MIN + (int32_t)(value - ((uint32_t)INT32_MAX + 1u));
}

RGINLINE int64_t rg_random_u64_to_i64(uint64_t value)
{
	if (value <= (uint64_t)INT64_MAX)
	{
		return (int64_t)value;
	}
	return INT64_MIN + (int64_t)(value - ((uint64_t)INT64_MAX + UINT64_C(1)));
}

RGINLINE void rg_rng_seed(RgRng* rng, uint64_t seed)
{
	RG_RANDOM_ASSERT(rng != NULL);
	uint64_t sm = seed;
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

RGINLINE void rg_rng_seed_state(RgRng* rng, const uint64_t seed_state[4])
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

RGINLINE uint64_t rg_rng_next_u64(RgRng* rng)
{
	uint64_t* s = rng->state;
	const uint64_t result = rg_random_rotl64(s[1] * 5ULL, 7) * 9ULL;
	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;
	s[3] = rg_random_rotl64(s[3], 45);

	return result;
}

RGINLINE uint32_t rg_rng_next_u32(RgRng* rng)
{
	return (uint32_t)(rg_rng_next_u64(rng) >> 32);
}

RGINLINE uint32_t rg_random_u32(RgRng* rng)
{
	return rg_rng_next_u32(rng);
}

RGINLINE uint64_t rg_random_u64(RgRng* rng)
{
	return rg_rng_next_u64(rng);
}

RGINLINE float rg_random_f32(RgRng* rng)
{
	uint32_t value = rg_rng_next_u32(rng);
	return (float)(value >> 8) * (1.0f / 16777216.0f);
}

RGINLINE double rg_random_f64(RgRng* rng)
{
	uint64_t value = rg_rng_next_u64(rng);
	return (double)(value >> 11) * (1.0 / 9007199254740992.0);
}

RGINLINE uint32_t rg_random_bounded_u32(RgRng* rng, uint32_t bound)
{
	if (bound == 0)
	{
		return 0;
	}

	uint32_t threshold = (uint32_t)(0u - bound) % bound;
	for (;;)
	{
		uint32_t value = rg_rng_next_u32(rng);
		uint64_t product = (uint64_t)value * (uint64_t)bound;
		uint32_t low = (uint32_t)product;
		if (low >= threshold)
		{
			return (uint32_t)(product >> 32);
		}
	}
}

RGINLINE uint64_t rg_random_bounded_u64(RgRng* rng, uint64_t bound)
{
	if (bound == 0)
	{
		return 0;
	}

	uint64_t threshold = (uint64_t)(0ULL - bound) % bound;
	for (;;)
	{
		uint64_t value = rg_rng_next_u64(rng);
		uint64_t low;
		uint64_t high = rg_random_mul_u64_wide(value, bound, &low);
		if (low >= threshold)
		{
			return high;
		}
	}
}

RGINLINE uint32_t rg_random_range_u32(RgRng* rng, uint32_t min, uint32_t max)
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

RGINLINE uint64_t rg_random_range_u64(RgRng* rng, uint64_t min, uint64_t max)
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

RGINLINE int32_t rg_random_range_i32(RgRng* rng, int32_t min, int32_t max)
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

	uint32_t umin = (uint32_t)min;
	uint32_t umax = (uint32_t)max;
	uint32_t range = (umax - umin) + 1u;
	return rg_random_u32_to_i32(umin + rg_random_bounded_u32(rng, range));
}

RGINLINE int64_t rg_random_range_i64(RgRng* rng, int64_t min, int64_t max)
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

	uint64_t umin = (uint64_t)min;
	uint64_t umax = (uint64_t)max;
	uint64_t range = (umax - umin) + 1ULL;
	return rg_random_u64_to_i64(umin + rg_random_bounded_u64(rng, range));
}

RGINLINE float rg_random_range_f32(RgRng* rng, float min, float max)
{
	RG_RANDOM_ASSERT(min <= max);
	return min + (max - min) * rg_random_f32(rng);
}

RGINLINE double rg_random_range_f64(RgRng* rng, double min, double max)
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

	uint8_t* base = (uint8_t*)data;
	for (size_t i = count - 1; i > 0; i--)
	{
		size_t j = (size_t)rg_random_range_u64(rng, 0, (uint64_t)i);
		if (i == j)
		{
			continue;
		}

		uint8_t* a = base + i * stride;
		uint8_t* b = base + j * stride;
		for (size_t k = 0; k < stride; k++)
		{
			uint8_t tmp = a[k];
			a[k] = b[k];
			b[k] = tmp;
		}
	}
}

RGINLINE void rg_random_fill_bytes(void* data, size_t size, RgRng* rng)
{
	RG_RANDOM_ASSERT(data != NULL || size == 0);
	uint8_t* dst = (uint8_t*)data;
	size_t i = 0;
	while (size - i >= 8)
	{
		uint64_t value = rg_rng_next_u64(rng);
		dst[i + 0] = (uint8_t)(value);
		dst[i + 1] = (uint8_t)(value >> 8);
		dst[i + 2] = (uint8_t)(value >> 16);
		dst[i + 3] = (uint8_t)(value >> 24);
		dst[i + 4] = (uint8_t)(value >> 32);
		dst[i + 5] = (uint8_t)(value >> 40);
		dst[i + 6] = (uint8_t)(value >> 48);
		dst[i + 7] = (uint8_t)(value >> 56);
		i += 8;
	}
	if (i < size)
	{
		uint64_t value = rg_rng_next_u64(rng);
		for (size_t j = 0; i < size; i++, j++)
		{
			dst[i] = (uint8_t)(value >> (j * 8));
		}
	}
}

RGINLINE void rg_random_normal2_f32(RgRng* rng, float mean, float stddev, float* out0, float* out1)
{
	RG_RANDOM_ASSERT(stddev > 0.0f);
	RG_RANDOM_ASSERT(out0 != NULL);
	RG_RANDOM_ASSERT(out1 != NULL);
	rng->has_spare = 0;

	float u1 = rg_random_f32(rng);
	float u2 = rg_random_f32(rng);
	if (u1 < 1.0e-7f)
	{
		u1 = 1.0e-7f;
	}

	float r = sqrtf(-2.0f * logf(u1));
	float theta = 2.0f * RG_RANDOM_PI_F32 * u2;
	float z0 = r * cosf(theta);
	float z1 = r * sinf(theta);

	*out0 = mean + z0 * stddev;
	*out1 = mean + z1 * stddev;
}

RGINLINE void rg_random_normal_cache_reset(RgNormalCache* cache)
{
	RG_RANDOM_ASSERT(cache != NULL);
	cache->has_spare = 0;
	cache->spare = 0.0f;
}

RGINLINE float rg_random_normal_f32(RgRng* rng, float mean, float stddev)
{
	RG_RANDOM_ASSERT(stddev > 0.0f);
	if (rng->has_spare)
	{
		rng->has_spare = 0;
		return mean + rng->spare * stddev;
	}

	float u1 = rg_random_f32(rng);
	float u2 = rg_random_f32(rng);
	if (u1 < 1.0e-7f)
	{
		u1 = 1.0e-7f;
	}

	float r = sqrtf(-2.0f * logf(u1));
	float theta = 2.0f * RG_RANDOM_PI_F32 * u2;
	float z0 = r * cosf(theta);
	float z1 = r * sinf(theta);

	rng->spare = z1;
	rng->has_spare = 1;
	return mean + z0 * stddev;
}

RGINLINE float rg_random_normal_f32_cached(RgRng* rng, RgNormalCache* cache, float mean, float stddev)
{
	RG_RANDOM_ASSERT(stddev > 0.0f);
	RG_RANDOM_ASSERT(cache != NULL);

	if (cache->has_spare)
	{
		cache->has_spare = 0;
		return mean + cache->spare * stddev;
	}

	float u1 = rg_random_f32(rng);
	float u2 = rg_random_f32(rng);
	if (u1 < 1.0e-7f)
	{
		u1 = 1.0e-7f;
	}

	float r = sqrtf(-2.0f * logf(u1));
	float theta = 2.0f * RG_RANDOM_PI_F32 * u2;
	float z0 = r * cosf(theta);
	float z1 = r * sinf(theta);

	cache->spare = z1;
	cache->has_spare = 1;
	return mean + z0 * stddev;
}

RGINLINE float rg_random_exponential_f32(RgRng* rng, float lambda)
{
	RG_RANDOM_ASSERT(lambda > 0.0f);
	float u = rg_random_f32(rng);
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
