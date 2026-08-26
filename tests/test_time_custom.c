// rg_time custom-backend correctness tests

#include "../src/rg_defs.h"

#include <stdio.h>

static u64 test_ticks_value;
static u64 test_frequency_value;
static u32 test_frequency_calls;
static u64 test_sleep_ns_value;
static u32 test_sleep_calls;
static u32 test_yield_calls;

static u64 test_time_ticks(void)
{
	return test_ticks_value;
}

static u64 test_time_frequency(void)
{
	test_frequency_calls++;
	return test_frequency_value;
}

static void test_time_sleep_ns(u64 ns)
{
	test_sleep_ns_value = ns;
	test_sleep_calls++;
}

static void test_time_yield(void)
{
	test_yield_calls++;
}

#define RG_TIME_CUSTOM 1
#define RG_TIME_PLATFORM_TICKS test_time_ticks
#define RG_TIME_PLATFORM_FREQUENCY test_time_frequency
#define RG_TIME_PLATFORM_SLEEP_NS test_time_sleep_ns
#define RG_TIME_PLATFORM_YIELD test_time_yield
#include "../src/rg_time.h"
#include "../src/rg_time.h"

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

static int nearly_equal(f64 a, f64 b, f64 tolerance)
{
	f64 difference = a - b;
	if (difference < 0.0) difference = -difference;
	return difference <= tolerance;
}

static void test_custom_clock(void)
{
	test_frequency_value = 0u;
	test_frequency_calls = 0u;
	CHECK(rg_time_ticks_to_seconds(1u) == 0.0);
	CHECK(rg_time_seconds_to_ticks(1.0) == 0u);
	CHECK(test_frequency_calls == 2u);

	test_ticks_value = UINT64_C(1234567);
	test_frequency_value = UINT64_C(1000000);
	CHECK(rg_time_ticks() == test_ticks_value);
	CHECK(rg_time_ticks_per_second() == test_frequency_value);
	CHECK(test_frequency_calls == 3u);
	rg_time_init();
	rg_time_init();
	CHECK(test_frequency_calls == 4u);
	CHECK(rg_time_ticks_to_seconds(UINT64_C(250000)) == 0.25);
	CHECK(rg_time_ticks_to_ms(UINT64_C(250000)) == 250.0);
	CHECK(rg_time_ticks_to_us(UINT64_C(250000)) == 250000.0);
	CHECK(rg_time_seconds_to_ticks(0.25) == UINT64_C(250000));
	CHECK(rg_time_ms_to_ticks(1.25) == UINT64_C(1250));
	CHECK(rg_time_us_to_ticks(2.0) == UINT64_C(2));
	CHECK(nearly_equal(rg_time_seconds(), 1.234567, 0.000000001));
	CHECK(nearly_equal(rg_time_ms(), 1234.567, 0.000001));
	CHECK(rg_time_us() == 1234567.0);
	CHECK(test_frequency_calls == 4u);
}

static void test_custom_sleep_and_yield(void)
{
	test_sleep_ns_value = 0u;
	test_sleep_calls = 0u;
	rg_time_sleep_ns(0u);
	rg_time_sleep_us(0u);
	rg_time_sleep_ms(0u);
	CHECK(test_sleep_calls == 0u);

	rg_time_sleep_ns(UINT64_C(17));
	CHECK(test_sleep_calls == 1u && test_sleep_ns_value == UINT64_C(17));
	rg_time_sleep_us(UINT64_C(23));
	CHECK(test_sleep_calls == 2u && test_sleep_ns_value == UINT64_C(23000));
	rg_time_sleep_ms(UINT32_C(29));
	CHECK(test_sleep_calls == 3u && test_sleep_ns_value == UINT64_C(29000000));
	rg_time_sleep_us(UINT64_MAX);
	CHECK(test_sleep_calls == 4u && test_sleep_ns_value == UINT64_MAX);

	test_yield_calls = 0u;
	rg_time_yield();
	CHECK(test_yield_calls == 1u);
}

int main(void)
{
	test_custom_clock();
	test_custom_sleep_and_yield();
	printf("rg_time custom: %d checks, %d failures\n", tests_run, tests_failed);
	return tests_failed == 0 ? 0 : 1;
}
