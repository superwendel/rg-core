// rg_random public API correctness tests

#include "../src/rg_random.h"
#include "../src/rg_random.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int tests_run;
static int tests_failed;

#define CHECK(condition)                                                \
	do                                                                  \
	{                                                                   \
		tests_run++;                                                    \
		if (!(condition))                                               \
		{                                                               \
			printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
			tests_failed++;                                             \
		}                                                               \
	} while (0)

#define CHECK_CLOSE(actual, expected, tolerance)                              \
	do                                                                         \
	{                                                                          \
		f64 check_actual = (f64)(actual);                                      \
		f64 check_expected = (f64)(expected);                                  \
		f64 check_difference = fabs(check_actual - check_expected);            \
		CHECK(check_difference <= (f64)(tolerance));                           \
	} while (0)

static void test_seed_and_golden_sequence(void)
{
	static const u64 expected_state[4] = {
		UINT64_C(0x161922c645ce50e8),
		UINT64_C(0xad760cafa1697b60),
		UINT64_C(0x3501ff44902ca50d),
		UINT64_C(0x417cb9a826d831df),
	};
	static const u64 expected_values[16] = {
		UINT64_C(0xe01d6fafc557f1b9),
		UINT64_C(0xbd627ebe4406b404),
		UINT64_C(0x2c23132b578b57db),
		UINT64_C(0x2e8b319d4d1f276a),
		UINT64_C(0x608d57acf53888e4),
		UINT64_C(0x9f44d4fe68bdc399),
		UINT64_C(0x2bf98c082c7cd85a),
		UINT64_C(0x42f3aa03d402664c),
		UINT64_C(0x947052f518f6cd76),
		UINT64_C(0xe824c04694af22fe),
		UINT64_C(0xf62eb8e231e58c1f),
		UINT64_C(0x3a689212ef96862c),
		UINT64_C(0xc1b57a101af31521),
		UINT64_C(0xb2f695ba8cbd53e5),
		UINT64_C(0x64882e3cf23efab6),
		UINT64_C(0xaad8240dc43a5e20),
	};
	static const u32 expected_u32[8] = {
		UINT32_C(0xe01d6faf),
		UINT32_C(0xbd627ebe),
		UINT32_C(0x2c23132b),
		UINT32_C(0x2e8b319d),
		UINT32_C(0x608d57ac),
		UINT32_C(0x9f44d4fe),
		UINT32_C(0x2bf98c08),
		UINT32_C(0x42f3aa03),
	};

	RgRng rng;
	rg_rng_seed(&rng, UINT64_C(0x123456789abcdef0));
	CHECK(memcmp(rng.state, expected_state, sizeof(expected_state)) == 0);
	CHECK(rng.has_spare == 0);
	CHECK(rng.spare == 0.0f);
	for (size_t i = 0; i < RG_ARRAY_COUNT(expected_values); i++)
		CHECK(rg_rng_next_u64(&rng) == expected_values[i]);

	rg_rng_seed(&rng, UINT64_C(0x123456789abcdef0));
	for (size_t i = 0; i < RG_ARRAY_COUNT(expected_u32); i++)
		CHECK(rg_rng_next_u32(&rng) == expected_u32[i]);

	u64 zero_state[4] = {0, 0, 0, 0};
	rng.has_spare = 1;
	rng.spare = 5.0f;
	rg_rng_seed_state(&rng, zero_state);
	CHECK(rng.state[0] == 1);
	CHECK(rng.state[1] == 0 && rng.state[2] == 0 && rng.state[3] == 0);
	CHECK(rng.has_spare == 0 && rng.spare == 0.0f);

	u64 explicit_state[4] = {1, 2, 3, 4};
	rg_rng_seed_state(&rng, explicit_state);
	CHECK(memcmp(rng.state, explicit_state, sizeof(explicit_state)) == 0);
}

static void check_wide_multiply(u64 a, u64 b,
	                            u64 expected_high, u64 expected_low)
{
	u64 low = UINT64_MAX;
	u64 high = rg_random_mul_u64_wide(a, b, &low);
	CHECK(high == expected_high);
	CHECK(low == expected_low);
}

static void test_wide_multiply(void)
{
	check_wide_multiply(0, UINT64_MAX, 0, 0);
	check_wide_multiply(UINT64_MAX, UINT64_MAX,
	                    UINT64_C(0xfffffffffffffffe), UINT64_C(1));
	check_wide_multiply(UINT64_C(0x0123456789abcdef),
	                    UINT64_C(0xfedcba9876543210),
	                    UINT64_C(0x0121fa00ad77d742),
	                    UINT64_C(0x2236d88fe5618cf0));
	check_wide_multiply(UINT64_C(0x8000000000000000), UINT64_C(2),
	                    UINT64_C(1), UINT64_C(0));
	check_wide_multiply(UINT64_C(0xdeadbeefcafebabe),
	                    UINT64_C(0x9e3779b97f4a7c15),
	                    UINT64_C(0x899f7d0d7e552218),
	                    UINT64_C(0x0eec9d9346455996));
}

static void test_bounded_golden_sequences(void)
{
	static const u64 bounds64[] = {
		UINT64_C(1),
		UINT64_C(2),
		UINT64_C(3),
		UINT64_C(10),
		UINT64_C(0xffffffff),
		UINT64_C(0x100000001),
		UINT64_C(0x8000000000000001),
		UINT64_MAX,
	};
	static const u64 expected64[] = {
		UINT64_C(0),
		UINT64_C(1),
		UINT64_C(0),
		UINT64_C(1),
		UINT64_C(0x608d57ac),
		UINT64_C(0x9f44d4ff),
		UINT64_C(0x4a38297a8c7b66bb),
		UINT64_C(0xe824c04694af22fd),
	};
	static const u32 bounds32[] = {1, 2, 3, 10, UINT32_MAX};
	static const u32 expected32[] = {
		UINT32_C(0),
		UINT32_C(1),
		UINT32_C(0),
		UINT32_C(1),
		UINT32_C(0x608d57ab),
	};

	RgRng rng;
	rg_rng_seed(&rng, UINT64_C(0x123456789abcdef0));
	for (size_t i = 0; i < RG_ARRAY_COUNT(bounds64); i++)
		CHECK(rg_random_bounded_u64(&rng, bounds64[i]) == expected64[i]);

	rg_rng_seed(&rng, UINT64_C(0x123456789abcdef0));
	for (size_t i = 0; i < RG_ARRAY_COUNT(bounds32); i++)
		CHECK(rg_random_bounded_u32(&rng, bounds32[i]) == expected32[i]);

	rg_rng_seed(&rng, 42);
	RgRng unchanged = rng;
	CHECK(rg_random_bounded_u32(&rng, 0) == 0);
	CHECK(memcmp(&rng, &unchanged, sizeof(rng)) == 0);
	CHECK(rg_random_bounded_u64(&rng, 0) == 0);
	CHECK(memcmp(&rng, &unchanged, sizeof(rng)) == 0);
}

static void test_uniform_ranges(void)
{
	RgRng rng;
	rg_rng_seed(&rng, 42);
	RgRng unchanged = rng;
	CHECK(rg_random_range_u32(&rng, 7, 7) == 7);
	CHECK(memcmp(&rng, &unchanged, sizeof(rng)) == 0);
	CHECK(rg_random_range_i64(&rng, -9, -9) == -9);
	CHECK(memcmp(&rng, &unchanged, sizeof(rng)) == 0);

	for (size_t i = 0; i < 20000; i++)
	{
		u32 value_u32 = rg_random_range_u32(&rng, 5, 12);
		u64 value_u64 = rg_random_range_u64(&rng, UINT64_C(0x100000000),
		                                          UINT64_C(0x100000123));
		i32 value_i32 = rg_random_range_i32(&rng, -700, 900);
		i64 value_i64 = rg_random_range_i64(&rng, INT64_C(-5000000000),
		                                          INT64_C(7000000000));
		f32 value_f32 = rg_random_f32(&rng);
		f64 value_f64 = rg_random_f64(&rng);
		f32 range_f32 = rg_random_range_f32(&rng, -3.0f, 7.0f);
		f64 range_f64 = rg_random_range_f64(&rng, -11.0, 13.0);

		CHECK(value_u32 >= 5 && value_u32 <= 12);
		CHECK(value_u64 >= UINT64_C(0x100000000) && value_u64 <= UINT64_C(0x100000123));
		CHECK(value_i32 >= -700 && value_i32 <= 900);
		CHECK(value_i64 >= INT64_C(-5000000000) && value_i64 <= INT64_C(7000000000));
		CHECK(value_f32 >= 0.0f && value_f32 < 1.0f);
		CHECK(value_f64 >= 0.0 && value_f64 < 1.0);
		CHECK(range_f32 >= -3.0f && range_f32 <= 7.0f);
		CHECK(range_f64 >= -11.0 && range_f64 <= 13.0);
	}

	rg_rng_seed(&rng, 123);
	RgRng reference = rng;
	CHECK(rg_random_range_u32(&rng, 0, UINT32_MAX) == rg_rng_next_u32(&reference));
	CHECK(rg_random_range_u64(&rng, 0, UINT64_MAX) == rg_rng_next_u64(&reference));
	CHECK(rg_random_range_i32(&rng, INT32_MIN, INT32_MAX) ==
	      rg_random_u32_to_i32(rg_rng_next_u32(&reference)));
	CHECK(rg_random_range_i64(&rng, INT64_MIN, INT64_MAX) ==
	      rg_random_u64_to_i64(rg_rng_next_u64(&reference)));

	CHECK(rg_random_u32_to_i32(UINT32_C(0x7fffffff)) == INT32_MAX);
	CHECK(rg_random_u32_to_i32(UINT32_C(0x80000000)) == INT32_MIN);
	CHECK(rg_random_u32_to_i32(UINT32_MAX) == -1);
	CHECK(rg_random_u64_to_i64(UINT64_C(0x7fffffffffffffff)) == INT64_MAX);
	CHECK(rg_random_u64_to_i64(UINT64_C(0x8000000000000000)) == INT64_MIN);
	CHECK(rg_random_u64_to_i64(UINT64_MAX) == -1);
}

static void test_shuffle_and_fill(void)
{
	static const u32 expected_shuffle[16] = {
		1, 12, 3, 13, 14, 7, 4, 6, 0, 10, 2, 9, 11, 15, 8, 5,
	};
	static const u8 expected_bytes[19] = {
		0xb4, 0xea, 0xc8, 0x55, 0xeb, 0xa7, 0x44, 0x59, 0x7e, 0x86,
		0xd8, 0x31, 0x3d, 0x4e, 0x62, 0x90, 0xc3, 0x15, 0x42,
	};
	u32 values[16];
	u8 bytes[19] = {0};
	RgRng rng;

	for (size_t i = 0; i < RG_ARRAY_COUNT(values); i++)
		values[i] = (u32)i;
	rg_rng_seed(&rng, 99);
	rg_random_shuffle(values, RG_ARRAY_COUNT(values), sizeof(values[0]), &rng);
	CHECK(memcmp(values, expected_shuffle, sizeof(values)) == 0);

	rg_rng_seed(&rng, 99);
	rg_random_fill_bytes(bytes, sizeof(bytes), &rng);
	CHECK(memcmp(bytes, expected_bytes, sizeof(bytes)) == 0);

	rg_rng_seed(&rng, 99);
	RgRng unchanged = rng;
	rg_random_shuffle(NULL, 0, sizeof(u32), &rng);
	CHECK(memcmp(&rng, &unchanged, sizeof(rng)) == 0);
	rg_random_shuffle(values, RG_ARRAY_COUNT(values), 0, &rng);
	CHECK(memcmp(&rng, &unchanged, sizeof(rng)) == 0);
	rg_random_fill_bytes(NULL, 0, &rng);
	CHECK(memcmp(&rng, &unchanged, sizeof(rng)) == 0);
}

static void test_boolean_helpers(void)
{
	RgRng rng;
	rg_rng_seed(&rng, 777);
	for (size_t i = 0; i < 1000; i++)
	{
		int boolean = rg_random_bool(&rng);
		int sign = rg_random_sign(&rng);
		CHECK(boolean == 0 || boolean == 1);
		CHECK(sign == -1 || sign == 1);
	}
}

static void test_distributions(void)
{
	const size_t sample_count = 200000;
	RgRng rng;
	rg_rng_seed(&rng, 1234);

	f64 sum = 0.0;
	f64 sum_squares = 0.0;
	for (size_t i = 0; i < sample_count; i++)
	{
		f32 value = rg_random_normal_f32(&rng, 0.0f, 1.0f);
		sum += value;
		sum_squares += (f64)value * (f64)value;
	}
	f64 mean = sum / (f64)sample_count;
	f64 variance = sum_squares / (f64)sample_count - mean * mean;
	CHECK_CLOSE(mean, 0.0, 0.02);
	CHECK_CLOSE(sqrt(variance), 1.0, 0.02);

	rg_rng_seed(&rng, 1234);
	RgNormalCache cache;
	rg_random_normal_cache_reset(&cache);
	sum = 0.0;
	for (size_t i = 0; i < sample_count; i++)
		sum += rg_random_normal_f32_cached(&rng, &cache, 3.0f, 2.0f);
	CHECK_CLOSE(sum / (f64)sample_count, 3.0, 0.04);

	rg_rng_seed(&rng, 5678);
	sum = 0.0;
	for (size_t i = 0; i < sample_count; i++)
	{
		f32 value = rg_random_exponential_f32(&rng, 2.0f);
		CHECK(value >= 0.0f && isfinite(value));
		sum += value;
	}
	CHECK_CLOSE(sum / (f64)sample_count, 0.5, 0.02);

	rg_rng_seed(&rng, 11);
	(void)rg_random_normal_f32(&rng, 0.0f, 1.0f);
	CHECK(rng.has_spare == 1);
	f32 out0 = 0.0f;
	f32 out1 = 0.0f;
	rg_random_normal2_f32(&rng, 2.0f, 0.5f, &out0, &out1);
	CHECK(rng.has_spare == 0);
	CHECK(isfinite(out0) && isfinite(out1));
	rg_rng_seed(&rng, 11);
	CHECK(rng.has_spare == 0);
}

int main(void)
{
	test_seed_and_golden_sequence();
	test_wide_multiply();
	test_bounded_golden_sequences();
	test_uniform_ranges();
	test_shuffle_and_fill();
	test_boolean_helpers();
	test_distributions();

	printf("rg_random: %d checks, %d failures\n", tests_run, tests_failed);
	return tests_failed == 0 ? 0 : 1;
}
