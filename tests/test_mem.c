// rg_mem public API correctness tests

#define RG_MALLOC_ASSERT(condition) ((void)sizeof(condition))

#include "../src/rg_mem.h"
#include "../src/rg_mem.h"

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

static void reset_pool(void)
{
	rg_free();
	CHECK(rg_total() == 0);
	CHECK(rg_used() == 0);
	CHECK(rg_remaining() == 0);
}

static void test_pool_lifecycle(void)
{
	reset_pool();
	CHECK(rg_malloc(0) == -1);
	CHECK(rg_malloc(KB(64)) == 0);
	CHECK(rg_total() >= KB(64));
	CHECK(rg_used() == 0);
	CHECK(rg_remaining() == rg_total());

	size_t initial_total = rg_total();
	CHECK(rg_malloc(MB(1)) == 0);
	CHECK(rg_total() == initial_total);
	rg_free();
	CHECK(rg_total() == 0);
	CHECK(rg_malloc(SIZE_MAX) == -1);
	CHECK(rg_total() == 0);
}

static void test_arena_creation(void)
{
	reset_pool();
	CHECK(rg_malloc(KB(64)) == 0);
	RgArena empty = rg_arena_create(0);
	CHECK(empty.memory == NULL);

	RgArena arena = rg_arena_create(4097);
	CHECK(arena.memory != NULL);
	CHECK(arena.capacity >= 4097);
	CHECK(arena.used == 0);
#if RG_MALLOC_LAZY_COMMIT
	CHECK(arena.committed == 0);
#else
	CHECK(arena.committed == arena.capacity);
#endif
	CHECK(rg_used() >= arena.capacity);
	CHECK(rg_remaining() == rg_total() - rg_used());

	size_t pool_used = rg_used();
	RgArena too_large = rg_arena_create(rg_total());
	CHECK(too_large.memory == NULL);
	CHECK(rg_used() == pool_used);

	rg_arena_free(&arena);
	CHECK(arena.memory == NULL);
	CHECK(arena.capacity == 0);
	CHECK(arena.used == 0);
	CHECK(arena.committed == 0);
	CHECK(rg_used() == pool_used);
	rg_free();
}

static void test_allocation_and_alignment(void)
{
	reset_pool();
	CHECK(rg_malloc(KB(128)) == 0);
	RgArena arena = rg_arena_create(KB(64));
	CHECK(arena.memory != NULL);

	static const size_t alignments[] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
	for (size_t i = 0; i < sizeof(alignments) / sizeof(alignments[0]); ++i)
	{
		void* ptr = rg_arena_alloc_aligned(&arena, 7, alignments[i]);
		CHECK(ptr != NULL);
		CHECK(((uintptr_t)ptr & (alignments[i] - 1)) == 0);
	}

	size_t used_before = arena.used;
	CHECK(rg_arena_alloc(&arena, 0) == NULL);
	CHECK(arena.used == used_before);
	CHECK(rg_arena_alloc_aligned(&arena, 8, 0) == NULL);
	CHECK(rg_arena_alloc_aligned(&arena, 8, 3) == NULL);
	CHECK(arena.used == used_before);
	CHECK(rg_arena_alloc_aligned(NULL, 8, 8) == NULL);
	CHECK(rg_arena_remaining(NULL) == 0);

	void* default_aligned = rg_arena_alloc(&arena, 32);
	CHECK(default_aligned != NULL);
	CHECK(((uintptr_t)default_aligned & (RG_MALLOC_DEFAULT_ALIGNMENT - 1)) == 0);
	rg_free();
}

static void test_reset_and_decommit(void)
{
	reset_pool();
	CHECK(rg_malloc(KB(64)) == 0);
	RgArena arena = rg_arena_create(KB(16));
	unsigned char* bytes = (unsigned char*)rg_arena_alloc(&arena, 5000);
	CHECK(bytes != NULL);
	if (bytes == NULL)
	{
		rg_free();
		return;
	}
	memset(bytes, 0xA5, 5000);
	CHECK(arena.used >= 5000);
	rg_arena_reset(&arena);
	CHECK(arena.used == 0);
#if defined(RG_MALLOC_SECURE)
	for (size_t i = 0; i < 5000; ++i)
		CHECK(bytes[i] == 0);
#endif
	CHECK(rg_arena_alloc(&arena, 32) == arena.memory);

#if !RG_MALLOC_LAZY_COMMIT
	size_t used_before_decommit = arena.used;
#endif
	rg_arena_decommit(&arena);
#if RG_MALLOC_LAZY_COMMIT
	CHECK(arena.used == 0);
	CHECK(arena.committed == 0);
	CHECK(rg_arena_alloc(&arena, 32) == arena.memory);
#else
	CHECK(arena.used == used_before_decommit);
	CHECK(arena.committed == arena.capacity);
#endif
	rg_free();
}

static void test_exhaustion_and_overflow(void)
{
	reset_pool();
	CHECK(rg_malloc(KB(64)) == 0);
	RgArena arena = rg_arena_create(KB(8));
	CHECK(arena.memory != NULL);

	void* full = rg_arena_alloc(&arena, arena.capacity);
	CHECK(full == arena.memory);
	size_t full_used = arena.used;
	CHECK(rg_arena_alloc(&arena, 1) == NULL);
	CHECK(arena.used == full_used);

	rg_arena_reset(&arena);
	CHECK(rg_arena_alloc_array(&arena, 2, SIZE_MAX / 2 + 1, 2) == NULL);
	CHECK(arena.used == 0);
	CHECK(RG_ARENA_PUSH_ARRAY(&arena, uint64_t, SIZE_MAX / sizeof(uint64_t) + 1) == NULL);
	CHECK(arena.used == 0);

	size_t pool_used = rg_used();
	RgArena overflow = rg_arena_create(SIZE_MAX);
	CHECK(overflow.memory == NULL);
	CHECK(rg_used() == pool_used);
	rg_free();
}

typedef struct TestPair
{
	uint32_t first;
	uint64_t second;
} TestPair;

static void test_typed_macros(void)
{
	reset_pool();
	CHECK(rg_malloc(KB(64)) == 0);
	RgArena arena = rg_arena_create(KB(16));

	TestPair* pair = RG_ARENA_PUSH_STRUCT(&arena, TestPair);
	CHECK(pair != NULL);
	CHECK(((uintptr_t)pair & (RG_ALIGNOF(TestPair) - 1)) == 0);
	pair->first = 7;
	pair->second = 11;

	size_t count = 8;
	TestPair* pairs = RG_ARENA_PUSH_ARRAY(&arena, TestPair, count++);
	CHECK(pairs != NULL);
	CHECK(count == 9);
	CHECK(((uintptr_t)pairs & (RG_ALIGNOF(TestPair) - 1)) == 0);
	for (size_t i = 0; i < 8; ++i)
	{
		pairs[i].first = (uint32_t)i;
		pairs[i].second = i * 2;
	}
	CHECK(pair->first == 7);
	CHECK(pair->second == 11);
	rg_free();
}

static void test_multiple_arenas(void)
{
	reset_pool();
	CHECK(rg_malloc(KB(128)) == 0);
	RgArena persistent = rg_arena_create(KB(16));
	RgArena scratch = rg_arena_create(KB(16));
	CHECK(persistent.memory != NULL);
	CHECK(scratch.memory != NULL);

	unsigned char* persistent_data = (unsigned char*)rg_arena_alloc(&persistent, 256);
	CHECK(persistent_data != NULL);
	if (persistent_data != NULL)
		memset(persistent_data, 0x6B, 256);
	for (int frame = 0; frame < 100; ++frame)
	{
		CHECK(rg_arena_alloc(&scratch, 256) != NULL);
		rg_arena_reset(&scratch);
	}
	if (persistent_data != NULL)
	{
		CHECK(persistent_data[0] == 0x6B);
		CHECK(persistent_data[255] == 0x6B);
	}
	rg_free();
}

int main(void)
{
	test_pool_lifecycle();
	test_arena_creation();
	test_allocation_and_alignment();
	test_reset_and_decommit();
	test_exhaustion_and_overflow();
	test_typed_macros();
	test_multiple_arenas();
	rg_free();

	if (tests_failed == 0)
		printf("All %d rg_mem checks passed\n", tests_run);
	else
		printf("%d of %d rg_mem checks failed\n", tests_failed, tests_run);
	return tests_failed == 0 ? 0 : 1;
}
