// rg_hash public API correctness tests

#define RG_HASH_ASSERT(condition) ((void)sizeof(condition))

#include "../src/rg_hash.h"
#include "../src/rg_hash.h"

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

RGINLINE uint64_t test_zero_hash(uint32_t key)
{
	RG_UNUSED(key);
	return 0;
}

RG_HASH_MAP_DEFINE(uint32_t, uint32_t, U32Map, rg_hash_u32, rg_hash_eq_u32);
RG_HASH_MAP_DEFINE(const char*, uint32_t, StrMap, rg_hash_str, rg_hash_eq_str);
RG_HASH_MAP_DEFINE(uint32_t, uint32_t, CollisionMap, test_zero_hash, rg_hash_eq_u32);
RG_HASH_SET_DEFINE(uint32_t, U32Set, rg_hash_u32, rg_hash_eq_u32);
RG_HASH_SET_DEFINE(uint32_t, CollisionSet, test_zero_hash, rg_hash_eq_u32);

static RgArena arena;

static uint32_t rng_next(uint32_t* state)
{
	uint32_t value = *state;
	value ^= value << 13;
	value ^= value >> 17;
	value ^= value << 5;
	*state = value;
	return value;
}

static void test_hash_functions(void)
{
	static const char unaligned[] = "xabc";
	const char* text = "rg_hash";
	const char* fox = "The quick brown fox jumps over the lazy dog";

	CHECK(rg_hash_bytes(NULL, 0, 0) == UINT64_C(0xef46db3751d8e999));
	CHECK(rg_hash_str("") == UINT64_C(0xef46db3751d8e999));
	CHECK(rg_hash_str("a") == UINT64_C(0xd24ec4f1a98c6e5b));
	CHECK(rg_hash_str("abc") == UINT64_C(0x44bc2cf5ad770999));
	CHECK(rg_hash_str(fox) == UINT64_C(0x0b242d361fda71bc));
	CHECK(rg_hash_bytes(text, strlen(text), RG_HASH_SEED) == rg_hash_str(text));
	CHECK(rg_hash_bytes(unaligned + 1, 3, 0) == rg_hash_str("abc"));
	CHECK(rg_hash_u32(42) == rg_hash_u64(42));
	CHECK(rg_hash_bswap32(UINT32_C(0x12345678)) == UINT32_C(0x78563412));
	CHECK(rg_hash_bswap64(UINT64_C(0x0123456789abcdef)) ==
	      UINT64_C(0xefcdab8967452301));

	int value = 42;
	CHECK(rg_hash_ptr(&value) == rg_hash_u64((uint64_t)(uintptr_t)&value));
	CHECK(rg_hash_eq_u32(7, 7));
	CHECK(!rg_hash_eq_u64(7, 8));
	CHECK(rg_hash_eq_ptr(&value, &value));
	CHECK(rg_hash_eq_str("same", "same"));
}

static void test_capacity_helpers(void)
{
	CHECK(rg_hash_pow2_ceil(0) == 1);
	CHECK(rg_hash_pow2_ceil(1) == 1);
	CHECK(rg_hash_pow2_ceil(9) == 16);
	CHECK(rg_hash_capacity_for(0) == 0);
	CHECK(rg_hash_capacity_for(1) >= RG_HASH_MIN_CAP);
	CHECK((rg_hash_capacity_for(100) & (rg_hash_capacity_for(100) - 1)) == 0);
	CHECK(rg_hash_capacity_for(SIZE_MAX) == 0);
	CHECK(rg_hash_should_grow(0, 0));
	CHECK(!rg_hash_should_grow(0, RG_HASH_MIN_CAP));
	CHECK(rg_hash_should_grow(SIZE_MAX, SIZE_MAX));
}

static void test_map_basic(void)
{
	rg_arena_reset(&arena);

	U32Map map;
	rg_hash_map_init(U32Map, &map, &arena);
	CHECK(rg_hash_map_is_empty(&map));
	CHECK(rg_hash_map_reserve(U32Map, &map, 100));
	CHECK(rg_hash_map_capacity(&map) >= 100);

	int is_new = 0;
	rg_hash_map_put(U32Map, &map, 10u, 100u, &is_new);
	CHECK(is_new == 1);
	rg_hash_map_put(U32Map, &map, 20u, 200u, &is_new);
	CHECK(is_new == 1);
	CHECK(rg_hash_map_count(&map) == 2);
	CHECK(rg_hash_map_contains(U32Map, &map, 10u));

	uint32_t* value = rg_hash_map_get_ptr(U32Map, &map, 10u);
	CHECK(value != NULL && *value == 100u);
	rg_hash_map_put(U32Map, &map, 10u, 111u, &is_new);
	CHECK(is_new == 0);
	value = rg_hash_map_get_ptr(U32Map, &map, 10u);
	CHECK(value != NULL && *value == 111u);

	const U32Map* const_map = &map;
	const uint32_t* const_value = rg_hash_map_get_ptr_const(U32Map, const_map, 10u);
	CHECK(const_value != NULL && *const_value == 111u);

	int removed = 0;
	rg_hash_map_remove(U32Map, &map, 20u, &removed);
	CHECK(removed == 1);
	rg_hash_map_remove(U32Map, &map, 20u, &removed);
	CHECK(removed == 0);
	CHECK(rg_hash_map_count(&map) == 1);

	size_t capacity = rg_hash_map_capacity(&map);
	rg_hash_map_clear(U32Map, &map);
	CHECK(rg_hash_map_is_empty(&map));
	CHECK(rg_hash_map_capacity(&map) == capacity);
	rg_hash_map_free(U32Map, &map);
	CHECK(map.entries == NULL && map.arena == NULL);
}

static void test_map_random(void)
{
	enum
	{
		KEY_COUNT = 1024,
		OP_COUNT = 30000
	};
	uint8_t present[KEY_COUNT] = {0};
	uint32_t expected_values[KEY_COUNT] = {0};
	uint32_t state = UINT32_C(0x12345678);

	rg_arena_reset(&arena);
	U32Map map;
	rg_hash_map_init(U32Map, &map, &arena);

	for (size_t i = 0; i < OP_COUNT; i++)
	{
		uint32_t key = rng_next(&state) & (KEY_COUNT - 1);
		uint32_t value = rng_next(&state);
		uint32_t action = rng_next(&state) % 3u;
		if (action == 0)
		{
			int is_new = 0;
			rg_hash_map_put(U32Map, &map, key, value, &is_new);
			CHECK(is_new == (present[key] == 0));
			present[key] = 1;
			expected_values[key] = value;
		}
		else if (action == 1)
		{
			int removed = 0;
			rg_hash_map_remove(U32Map, &map, key, &removed);
			CHECK(removed == (present[key] != 0));
			present[key] = 0;
		}
		else
		{
			uint32_t* found = rg_hash_map_get_ptr(U32Map, &map, key);
			CHECK((found != NULL) == (present[key] != 0));
			if (found != NULL)
				CHECK(*found == expected_values[key]);
		}
	}

	size_t expected_count = 0;
	for (size_t key = 0; key < KEY_COUNT; key++)
	{
		expected_count += present[key] != 0;
		uint32_t* found = rg_hash_map_get_ptr(U32Map, &map, (uint32_t)key);
		CHECK((found != NULL) == (present[key] != 0));
		if (found != NULL)
			CHECK(*found == expected_values[key]);
	}
	CHECK(rg_hash_map_count(&map) == expected_count);
}

static void test_map_collisions(void)
{
	rg_arena_reset(&arena);
	CollisionMap map;
	rg_hash_map_init(CollisionMap, &map, &arena);

	for (uint32_t key = 0; key < 64; key++)
		rg_hash_map_put(CollisionMap, &map, key, key * 3, NULL);
	CHECK(rg_hash_map_count(&map) == 64);

	for (uint32_t key = 0; key < 64; key++)
	{
		uint32_t* value = rg_hash_map_get_ptr(CollisionMap, &map, key);
		CHECK(value != NULL && *value == key * 3);
	}
	for (uint32_t key = 0; key < 64; key += 2)
		rg_hash_map_remove(CollisionMap, &map, key, NULL);
	for (uint32_t key = 0; key < 64; key++)
		CHECK(rg_hash_map_contains(CollisionMap, &map, key) == ((key & 1u) != 0));
}

static void test_string_map(void)
{
	static const char* keys[] = {"apple", "banana", "cherry", "date", "elderberry"};
	rg_arena_reset(&arena);
	StrMap map;
	rg_hash_map_init(StrMap, &map, &arena);

	for (size_t i = 0; i < RG_ARRAY_COUNT(keys); i++)
		rg_hash_map_put(StrMap, &map, keys[i], (uint32_t)i, NULL);
	for (size_t i = 0; i < RG_ARRAY_COUNT(keys); i++)
	{
		uint32_t* value = rg_hash_map_get_ptr(StrMap, &map, keys[i]);
		CHECK(value != NULL && *value == (uint32_t)i);
	}
	CHECK(rg_hash_map_get_ptr(StrMap, &map, "missing") == NULL);
}

static void test_set_random(void)
{
	enum
	{
		KEY_COUNT = 4096,
		OP_COUNT = 30000
	};
	uint8_t present[KEY_COUNT] = {0};
	uint32_t state = UINT32_C(0xdeadbeef);

	rg_arena_reset(&arena);
	U32Set set;
	rg_hash_set_init(U32Set, &set, &arena);
	CHECK(rg_hash_set_reserve(U32Set, &set, 256));

	for (size_t i = 0; i < OP_COUNT; i++)
	{
		uint32_t key = rng_next(&state) & (KEY_COUNT - 1);
		uint32_t action = rng_next(&state) % 3u;
		if (action == 0)
		{
			int is_new = 0;
			rg_hash_set_insert(U32Set, &set, key, &is_new);
			CHECK(is_new == (present[key] == 0));
			present[key] = 1;
		}
		else if (action == 1)
		{
			int removed = 0;
			rg_hash_set_remove(U32Set, &set, key, &removed);
			CHECK(removed == (present[key] != 0));
			present[key] = 0;
		}
		else
		{
			CHECK(rg_hash_set_contains(U32Set, &set, key) == (present[key] != 0));
		}
	}

	size_t expected_count = 0;
	for (size_t key = 0; key < KEY_COUNT; key++)
		expected_count += present[key] != 0;
	CHECK(rg_hash_set_count(&set) == expected_count);

	rg_hash_set_clear(U32Set, &set);
	CHECK(rg_hash_set_is_empty(&set));
	rg_hash_set_free(U32Set, &set);
	CHECK(set.entries == NULL && set.arena == NULL);
}

static void test_set_collisions(void)
{
	rg_arena_reset(&arena);
	CollisionSet set;
	rg_hash_set_init(CollisionSet, &set, &arena);

	for (uint32_t key = 0; key < 64; key++)
		rg_hash_set_insert(CollisionSet, &set, key, NULL);
	for (uint32_t key = 0; key < 64; key += 2)
		rg_hash_set_remove(CollisionSet, &set, key, NULL);
	for (uint32_t key = 0; key < 64; key++)
		CHECK(rg_hash_set_contains(CollisionSet, &set, key) == ((key & 1u) != 0));
}

static void test_allocation_failure(void)
{
	RgArena tiny = rg_arena_create(KB(1));
	CHECK(tiny.memory != NULL);

	U32Map map;
	rg_hash_map_init(U32Map, &map, &tiny);
	CHECK(!rg_hash_map_reserve(U32Map, &map, 1000));
	CHECK(map.entries == NULL);
	CHECK(map.count == 0);
	CHECK(map.cap == 0);
	CHECK(rg_hash_map_try_put(U32Map, &map, 1, 2) == 1);

	CHECK(rg_hash_map_count(&map) == 1);
	rg_arena_free(&tiny);

	char byte = 0;
	RgArena full = {0};
	full.memory = &byte;
	U32Map failed_map;
	rg_hash_map_init(U32Map, &failed_map, &full);
	CHECK(rg_hash_map_try_put(U32Map, &failed_map, 1, 2) == -1);
	CHECK(failed_map.entries == NULL && failed_map.count == 0);
}

int main(void)
{
	if (rg_malloc(MB(32)) != 0)
	{
		printf("Failed to initialize the memory pool.\n");
		return 1;
	}
	CHECK(rg_total() >= MB(32));
	CHECK(rg_used() == 0);
	CHECK(rg_remaining() == rg_total());
	arena = rg_arena_create(MB(16));
	if (arena.memory == NULL)
	{
		printf("Failed to create the test arena.\n");
		rg_free();
		return 1;
	}

	test_hash_functions();
	test_capacity_helpers();
	test_map_basic();
	test_map_random();
	test_map_collisions();
	test_string_map();
	test_set_random();
	test_set_collisions();
	test_allocation_failure();

	rg_arena_free(&arena);
	rg_free();
	printf("rg_hash: %d checks, %d failures\n", tests_run, tests_failed);
	return tests_failed == 0 ? 0 : 1;
}
