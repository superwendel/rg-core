// rg_prof disabled-build tests

#define RG_PROF_ENABLED 0
#include "../src/rg_prof.h"

#include <stdio.h>

static i32 tests_passed;
static i32 tests_failed;
static i32 side_effects;

#define TEST_ASSERT(condition, message)                          \
	do                                                           \
	{                                                            \
		if (!(condition))                                        \
		{                                                        \
			printf("  FAIL: %s (line %d)\n", message, __LINE__); \
			tests_failed++;                                      \
			return;                                              \
		}                                                        \
	} while (0)

static const char* side_effect_name(void)
{
	side_effects++;
	return "event";
}

static RgProfThread* side_effect_thread(void)
{
	side_effects++;
	return NULL;
}

static void test_disabled_macros(void)
{
	i32 body_count = 0;
	RG_PROF_SCOPE(side_effect_name())
	{
		body_count++;
	}
	RG_PROF_SCOPE_ON(side_effect_thread(), side_effect_name())
	{
		body_count++;
	}
	RG_PROF_EVENT(side_effect_name());
	RG_PROF_EVENT_ON(side_effect_thread(), side_effect_name());
	RG_PROF_FRAME_BEGIN(side_effect_name());
	RG_PROF_FRAME_BEGIN_ON(side_effect_thread(), side_effect_name());
	RG_PROF_FRAME_END(side_effect_name());
	RG_PROF_FRAME_END_ON(side_effect_thread(), side_effect_name());

	TEST_ASSERT(body_count == 2, "disabled scopes execute once");
	TEST_ASSERT(side_effects == 0, "disabled macro arguments are not evaluated");
	tests_passed++;
	printf("  PASS: disabled macros\n");
}

static void test_disabled_api(void)
{
	RgProf prof;
	RgArena arena;
	RgProfHistory history;
	TEST_ASSERT(rg_prof_init_default(&prof, &arena), "disabled init");
	TEST_ASSERT(rg_prof_register_thread(&prof, "main") != NULL, "disabled registration");
	TEST_ASSERT(rg_prof_get_thread() == NULL, "disabled TLS");
	TEST_ASSERT(rg_prof_ticks_now(&prof) == 0u, "disabled ticks");
	TEST_ASSERT(rg_prof_ticks_to_ms(&prof, 1u) == 0.0, "disabled conversion");
	TEST_ASSERT(rg_prof_history_init(&history, &arena, 64, NULL, 0), "disabled history init");
	TEST_ASSERT(rg_prof_history_latest(&history) == NULL, "disabled history empty");

	tests_passed++;
	printf("  PASS: disabled API\n");
}

int main(void)
{
	printf("rg_prof disabled test suite\n");
	printf("===========================\n");
	test_disabled_macros();
	test_disabled_api();
	printf("\n%d passed, %d failed\n", tests_passed, tests_failed);
	return tests_failed == 0 ? 0 : 1;
}
