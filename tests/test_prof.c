// rg_prof correctness tests

#define RG_PROF_ENABLED 1
#include "../src/rg_prof.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

static i32 tests_passed;
static i32 tests_failed;

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

#define TEST_ASSERT_CLOSE(actual, expected, tolerance, message)                         \
	do                                                                                  \
	{                                                                                   \
		f64 difference = fabs((f64)(actual) - (f64)(expected));                         \
		if (difference > (f64)(tolerance))                                              \
		{                                                                               \
			printf("  FAIL: %s (line %d, diff=%.6f)\n", message, __LINE__, difference); \
			tests_failed++;                                                             \
			return;                                                                     \
		}                                                                               \
	} while (0)

#define TEST_PASS(name)               \
	do                                \
	{                                 \
		tests_passed++;               \
		printf("  PASS: %s\n", name); \
	} while (0)

static void count_event(const RgProfEvent* event, void* user)
{
	RG_UNUSED(event);
	u32* count = (u32*)user;
	(*count)++;
}

static void set_event(RgProfEvent* event, const char* name, u8 type, u64 start, u64 end)
{
	memset(event, 0, sizeof(*event));
	event->name = name;
	event->type = type;
	event->start = start;
	event->end = end;
}

static void test_init_and_layout(void)
{
	RgArena arena = rg_arena_create(MB(2));
	TEST_ASSERT(arena.memory != NULL, "arena create");

	RgProf prof;
	TEST_ASSERT(rg_prof_init(&prof, &arena, 2, 8), "prof init");
	TEST_ASSERT(prof.ticks_per_second > 0u, "timer frequency");
	size_t event_size = sizeof(RgProfEvent);
	size_t thread_slot_size = sizeof(RgProfThreadSlot);
	TEST_ASSERT(event_size <= 32u, "compact event");
	TEST_ASSERT(thread_slot_size == RG_CACHE_LINE_SIZE, "thread slot size");
	TEST_ASSERT(RG_IS_ALIGNED(prof.thread_slots, RG_CACHE_LINE_SIZE), "thread slots aligned");

	RgProfThread* main_thread = rg_prof_register_thread(&prof, "main");
	RgProfThread* worker_thread = rg_prof_register_thread(&prof, "worker");
	TEST_ASSERT(main_thread != NULL && worker_thread != NULL, "thread registration");
	TEST_ASSERT((size_t)((u8*)worker_thread - (u8*)main_thread) == RG_CACHE_LINE_SIZE,
	            "thread states isolated by cache line");
	TEST_ASSERT(prof.thread_count == 2u, "thread count");
	TEST_ASSERT(rg_prof_register_thread(&prof, "overflow") == NULL, "thread limit");

	TEST_PASS("init and layout");
}

static void test_default_and_conversions(void)
{
	RgArena arena = rg_arena_create(MB(2));
	TEST_ASSERT(arena.memory != NULL, "arena create");

	RgProf prof;
	TEST_ASSERT(rg_prof_init_default(&prof, &arena), "default init");
	TEST_ASSERT(prof.max_threads == RG_PROF_DEFAULT_MAX_THREADS, "default threads");
	TEST_ASSERT(prof.events_per_thread == RG_PROF_DEFAULT_EVENTS_PER_THREAD, "default events");
	TEST_ASSERT_CLOSE(rg_prof_ticks_to_ms(&prof, prof.ticks_per_second), 1000.0, 0.001,
	                  "ticks to milliseconds");
	TEST_ASSERT_CLOSE(rg_prof_ticks_to_us(&prof, prof.ticks_per_second), 1000000.0, 0.001,
	                  "ticks to microseconds");

	TEST_PASS("defaults and conversions");
}

static void test_events_and_depth(void)
{
	RgArena arena = rg_arena_create(MB(1));
	RgProf prof;
	TEST_ASSERT(rg_prof_init(&prof, &arena, 1, 16), "prof init");
	RgProfThread* thread = rg_prof_register_thread(&prof, "main");
	TEST_ASSERT(thread != NULL, "thread register");
	rg_prof_set_thread(thread);

	RG_PROF_EVENT("Instant");
	RG_PROF_SCOPE("Outer")
	{
		RG_PROF_EVENT("Inner");
	}
	RG_PROF_FRAME_BEGIN("Frame");
	RG_PROF_FRAME_END("Frame");

	TEST_ASSERT(thread->count == 5u, "event count");
	TEST_ASSERT(thread->events[0].type == RG_PROF_EVENT_INSTANT, "instant type");
	TEST_ASSERT(thread->events[1].type == RG_PROF_EVENT_SCOPE, "scope type");
	TEST_ASSERT(thread->events[1].end >= thread->events[1].start, "scope duration");
	TEST_ASSERT(thread->events[2].depth == 1u, "nested depth");
	TEST_ASSERT(thread->events[3].type == RG_PROF_EVENT_FRAME_BEGIN, "frame begin");
	TEST_ASSERT(thread->events[4].type == RG_PROF_EVENT_FRAME_END, "frame end");
	TEST_ASSERT(thread->completed_frames == 1u && thread->frame_open == 0u, "frame completed");
	TEST_ASSERT(thread->depth == 0u, "balanced depth");

	u32 count = 0u;
	rg_prof_iterate_events(&prof, count_event, &count);
	TEST_ASSERT(count == thread->count, "event iteration");

	TEST_PASS("events and depth");
}

static void test_scope_control_flow(void)
{
	RgArena arena = rg_arena_create(MB(1));
	RgProf prof;
	TEST_ASSERT(rg_prof_init(&prof, &arena, 1, 8), "prof init");
	RgProfThread* thread = rg_prof_register_thread(&prof, "main");
	TEST_ASSERT(thread != NULL, "thread register");
	rg_prof_set_thread(thread);

	RG_PROF_SCOPE("Break")
	{
		break;
	}
	TEST_ASSERT(thread->depth == 0u, "break closes scope");
	TEST_ASSERT(thread->events[0].end >= thread->events[0].start, "break records end");

	RG_PROF_SCOPE_ON(thread, "Continue")
	{
		continue;
	}
	TEST_ASSERT(thread->depth == 0u, "continue closes scope");
	TEST_ASSERT(thread->events[1].end >= thread->events[1].start, "continue records end");

	RG_PROF_EVENT_ON(thread, "Direct");
	TEST_ASSERT(thread->events[2].type == RG_PROF_EVENT_INSTANT, "direct event");

	TEST_PASS("scope control flow");
}

static void test_overflow(void)
{
	RgArena arena = rg_arena_create(MB(1));
	RgProf prof;
	TEST_ASSERT(rg_prof_init(&prof, &arena, 1, 1), "prof init");
	RgProfThread* thread = rg_prof_register_thread(&prof, "main");
	TEST_ASSERT(thread != NULL, "thread register");
	rg_prof_set_thread(thread);

	RG_PROF_SCOPE("Outer")
	{
		RG_PROF_SCOPE("Dropped")
		{
		}
	}
	RG_PROF_EVENT("Dropped too");
	TEST_ASSERT(thread->count == 1u, "capacity held");
	TEST_ASSERT(thread->dropped == 2u, "drops counted");
	TEST_ASSERT(thread->depth == 0u, "overflow depth balanced");
	rg_prof_thread_reset(thread);
	TEST_ASSERT(thread->count == 0u && thread->dropped == 0u && thread->depth == 0u,
	            "thread reset");

	TEST_PASS("overflow");
}

static void test_allocation_rollback(void)
{
	u8 storage[128];
	RgArena arena = {(char*)storage, sizeof(storage), 0u, sizeof(storage)};
	RgProf prof;
	TEST_ASSERT(rg_prof_init(&prof, &arena, 1, 8), "prof init");
	size_t used_before = arena.used;
	TEST_ASSERT(rg_prof_register_thread(&prof, "main") == NULL, "register allocation failure");
	TEST_ASSERT(arena.used == used_before, "register rollback");
	TEST_ASSERT(prof.thread_count == 0u, "failed registration not published");

	TEST_PASS("allocation rollback");
}

static void test_history(void)
{
	RgArena arena = rg_arena_create(MB(1));
	RgProf prof;
	TEST_ASSERT(rg_prof_init(&prof, &arena, 1, 16), "prof init");
	RgProfThread* thread = rg_prof_register_thread(&prof, "main");
	TEST_ASSERT(thread != NULL, "thread register");

	const char* sections[] = {"Update", "Render"};
	RgProfHistory history;
	TEST_ASSERT(rg_prof_history_init(&history, &arena, 2, sections, RG_ARRAY_COUNT(sections)),
	            "history init");
	TEST_ASSERT(rg_prof_history_capture(&history, thread) == 0, "reject empty capture");

	u64 tick = prof.ticks_per_second / 1000u;
	TEST_ASSERT(tick > 0u, "millisecond tick");
	set_event(&thread->events[0], "Frame", RG_PROF_EVENT_FRAME_BEGIN, 10u * tick, 10u * tick);
	set_event(&thread->events[1], "Update", RG_PROF_EVENT_SCOPE, 11u * tick, 14u * tick);
	set_event(&thread->events[2], "Render", RG_PROF_EVENT_SCOPE, 15u * tick, 17u * tick);
	set_event(&thread->events[3], "Update", RG_PROF_EVENT_SCOPE, 18u * tick, 19u * tick);
	set_event(&thread->events[4], "Frame", RG_PROF_EVENT_FRAME_END, 20u * tick, 20u * tick);
	thread->count = 5u;
	thread->completed_frames = 1u;

	TEST_ASSERT(rg_prof_history_capture(&history, thread), "capture frame");
	const RgProfFrameSample* latest = rg_prof_history_latest(&history);
	TEST_ASSERT(latest != NULL && latest->frame_index == 0u, "latest frame index");
	TEST_ASSERT_CLOSE(latest->frame_ms, 10.0, 0.01, "frame duration");
	TEST_ASSERT_CLOSE(rg_prof_history_latest_section_ms(&history, 0), 4.0, 0.01,
	                  "summed update scopes");
	TEST_ASSERT_CLOSE(rg_prof_history_latest_section_ms(&history, 1), 2.0, 0.01,
	                  "render scope");
	TEST_ASSERT(rg_prof_history_capture(&history, thread) == 0, "reject duplicate frame");

	thread->events[4].start = 22u * tick;
	thread->completed_frames = 2u;
	TEST_ASSERT(rg_prof_history_capture(&history, thread), "capture second frame");
	thread->events[4].start = 24u * tick;
	thread->completed_frames = 3u;
	TEST_ASSERT(rg_prof_history_capture(&history, thread), "capture ring wrap");
	latest = rg_prof_history_latest(&history);
	TEST_ASSERT(latest->frame_index == 2u, "history frame index");
	TEST_ASSERT(history.count == 2u, "history ring count");
	TEST_ASSERT_CLOSE(rg_prof_history_average_frame_ms(&history, 0), 13.0, 0.01,
	                  "history frame average");
	TEST_ASSERT_CLOSE(rg_prof_history_average_section_ms(&history, 0, 2), 4.0, 0.01,
	                  "history section average");

	TEST_PASS("frame history");
}

int main(void)
{
	printf("rg_prof test suite\n");
	printf("==================\n");
	if (rg_malloc(MB(16)) != 0)
	{
		printf("Failed to initialize rg_mem.\n");
		return 1;
	}
	RG_UNUSED(rg_remaining());
	RG_UNUSED(rg_used());
	RG_UNUSED(rg_total());
	RgArena unused_arena = {0};
	rg_arena_free(&unused_arena);

	test_init_and_layout();
	test_default_and_conversions();
	test_events_and_depth();
	test_scope_control_flow();
	test_overflow();
	test_allocation_rollback();
	test_history();

	printf("\n%d passed, %d failed\n", tests_passed, tests_failed);
	rg_free();
	return tests_failed == 0 ? 0 : 1;
}
