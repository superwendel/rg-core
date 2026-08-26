// rg_time public API correctness tests

#include "../src/rg_time.h"
#include "../src/rg_time.h"

#include <math.h>
#include <stdio.h>

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
	return fabs(a - b) <= tolerance;
}

static void test_frequency_and_conversions(void)
{
	u64 frequency = rg_time_ticks_per_second();
	CHECK(frequency > 0u);
	CHECK(nearly_equal(rg_time_ticks_to_seconds(frequency), 1.0, 0.000001));
	CHECK(nearly_equal(rg_time_ticks_to_ms(frequency), 1000.0, 0.001));
	CHECK(nearly_equal(rg_time_ticks_to_us(frequency), 1000000.0, 1.0));
	CHECK(rg_time_seconds_to_ticks(1.0) == frequency);
	CHECK(rg_time_ms_to_ticks(1000.0) == frequency);
	CHECK(rg_time_us_to_ticks(1000000.0) == frequency);
	CHECK(rg_time_seconds_to_ticks(0.0) == 0u);
	CHECK(rg_time_ms_to_ticks(-1.0) == 0u);
	CHECK(rg_time_us_to_ticks(NAN) == 0u);
	CHECK(rg_time_seconds_to_ticks(HUGE_VAL) == UINT64_MAX);
	CHECK(rg_time_ticks_to_seconds(0u) == 0.0);
	CHECK(rg_time_ticks_to_ms(0u) == 0.0);
	CHECK(rg_time_ticks_to_us(0u) == 0.0);
}

static void test_monotonic_clock(void)
{
	u64 first = rg_time_ticks();
	u64 second = rg_time_ticks();
	CHECK(second >= first);

	rg_time_sleep_ns(0u);
	rg_time_sleep_us(0u);
	rg_time_sleep_ms(1u);
	u64 after_sleep = rg_time_ticks();
	CHECK(after_sleep > second);
	CHECK(rg_time_ticks_to_seconds(after_sleep - second) > 0.0);
	CHECK(rg_time_seconds() > 0.0);
	CHECK(rg_time_ms() > 0.0);
	CHECK(rg_time_us() > 0.0);

	rg_time_yield();
}

int main(void)
{
	rg_time_init();
	rg_time_init();
	test_frequency_and_conversions();
	test_monotonic_clock();
	printf("rg_time: %d checks, %d failures\n", tests_run, tests_failed);
	return tests_failed == 0 ? 0 : 1;
}
