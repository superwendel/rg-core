// rg_assert disabled behavior tests

#define RG_ASSERT_ENABLED 0
#define RG_ENSURE_ENABLED 0
#define RG_ASSERT_ABORT() ((void)0)
#define RG_ASSERT_BREAK() ((void)0)

static void test_handler(const char* expression, const char* file, int line, const char* message);
#define RG_ASSERT_HANDLER test_handler

#include "../src/rg_assert.h"

#include <stdio.h>

static int tests_run;
static int tests_failed;
static int handler_calls;
static int side_effect_count;

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

static int side_effect(void)
{
	side_effect_count++;
	return 0;
}

static void test_handler(const char* expression, const char* file, int line, const char* message)
{
	RG_ASSERT_UNUSED(expression);
	RG_ASSERT_UNUSED(file);
	RG_ASSERT_UNUSED(line);
	RG_ASSERT_UNUSED(message);
	handler_calls++;
}

int main(void)
{
	volatile int condition_true = 1;
	RG_ASSERT(side_effect());
	CHECK(side_effect_count == 0);
	CHECK(handler_calls == 0);
	CHECK(RG_ENSURE(side_effect()) == 0);
	CHECK(side_effect_count == 1);
	CHECK(handler_calls == 0);
	CHECK(RG_ENSURE(condition_true) == 1);

	if (tests_failed == 0)
		printf("All %d disabled rg_assert checks passed\n", tests_run);
	else
		printf("%d of %d disabled rg_assert checks failed\n", tests_failed, tests_run);
	return tests_failed == 0 ? 0 : 1;
}
