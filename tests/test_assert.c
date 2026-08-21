// rg_assert enabled correctness tests

#define RG_ASSERT_ENABLED 1
#define RG_ENSURE_ENABLED 1
#define RG_ASSERT_ABORT() ((void)0)
#define RG_ASSERT_BREAK() ((void)0)

static void test_handler(const char* expression, const char* file, int line, const char* message);
#define RG_ASSERT_HANDLER test_handler

#include "../src/rg_assert.h"
#include "../src/rg_assert.h"

#include <stdio.h>
#include <string.h>

static int tests_run;
static int tests_failed;
static int handler_calls;
static char handler_expression[64];
static char handler_message[128];

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

static void reset_handler(void)
{
	handler_calls = 0;
	handler_expression[0] = '\0';
	handler_message[0] = '\0';
}

static void test_handler(const char* expression, const char* file, int line, const char* message)
{
	RG_ASSERT_UNUSED(file);
	RG_ASSERT_UNUSED(line);
	handler_calls++;
	strncpy(handler_expression, expression != NULL ? expression : "", sizeof(handler_expression) - 1);
	handler_expression[sizeof(handler_expression) - 1] = '\0';
	strncpy(handler_message, message != NULL ? message : "", sizeof(handler_message) - 1);
	handler_message[sizeof(handler_message) - 1] = '\0';
}

static void test_assertions(void)
{
	reset_handler();
	RG_ASSERT(1);
	CHECK(handler_calls == 0);
	RG_ASSERT(0);
	CHECK(handler_calls == 1);
	CHECK(strcmp(handler_expression, "0") == 0);
	CHECK(handler_message[0] == '\0');

	reset_handler();
	RG_ASSERT_MSG(0, "value %d", 7);
	CHECK(handler_calls == 1);
	CHECK(strcmp(handler_expression, "0") == 0);
	CHECK(strcmp(handler_message, "value 7") == 0);
}

static void test_ensures(void)
{
	volatile int condition_true = 1;
	volatile int condition_false = 0;
	reset_handler();
	CHECK(RG_ENSURE(condition_true) == 1);
	CHECK(handler_calls == 0);
	CHECK(RG_ENSURE_MSG(condition_false, "ensure %s", "failed") == 0);
	CHECK(handler_calls == 1);
	CHECK(strcmp(handler_expression, "condition_false") == 0);
	CHECK(strcmp(handler_message, "ensure failed") == 0);
}

static void test_panic(void)
{
	reset_handler();
	RG_PANIC_MSG("panic %d", 3);
	CHECK(handler_calls == 1);
	CHECK(strcmp(handler_expression, "PANIC") == 0);
	CHECK(strcmp(handler_message, "panic 3") == 0);
}

int main(void)
{
	test_assertions();
	test_ensures();
	test_panic();

	if (tests_failed == 0)
		printf("All %d rg_assert checks passed\n", tests_run);
	else
		printf("%d of %d rg_assert checks failed\n", tests_failed, tests_run);
	return tests_failed == 0 ? 0 : 1;
}
