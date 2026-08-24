// rg_algo correctness and compatibility tests

#include "../src/rg_algo.h"
#include "../src/rg_algo.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// Test Framework
// =============================================================================

static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
	if (!(cond)) { \
		printf("  FAIL: %s (line %d)\n", msg, __LINE__); \
		g_tests_failed++; \
		return; \
	} \
} while (0)

#define TEST_PASS() do { g_tests_passed++; } while (0)

// =============================================================================
// Helpers
// =============================================================================

static u32 rng_next(u32* state)
{
	u32 x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*state = x;
	return x;
}

static u64 rng_next_u64(u32* state)
{
	u64 hi = (u64)rng_next(state);
	u64 lo = (u64)rng_next(state);
	return (hi << 32u) | lo;
}

static int int_less(const int* a, const int* b)
{
	return *a < *b;
}

static int i32_less(const i32* a, const i32* b)
{
	return *a < *b;
}

static int u64_less(const u64* a, const u64* b)
{
	return *a < *b;
}

typedef struct Item
{
	int key;
	int id;
} Item;

typedef struct Item32
{
	i32 key;
	i32 id;
} Item32;

typedef struct Item64
{
	u64 key;
	u32 id;
} Item64;

static int item_less(const Item* a, const Item* b)
{
	return a->key < b->key;
}

static u32 item32_key(const Item32* value)
{
	return (u32)value->key ^ 0x80000000u;
}

static u64 item64_key(const Item64* value)
{
	return value->key;
}

static int qsort_int_cmp(const void* a, const void* b)
{
	int ia = *(const int*)a;
	int ib = *(const int*)b;
	return (ia > ib) - (ia < ib);
}

static int qsort_i32_cmp(const void* a, const void* b)
{
	i32 ia = *(const i32*)a;
	i32 ib = *(const i32*)b;
	return (ia > ib) - (ia < ib);
}

static int qsort_u64_cmp(const void* a, const void* b)
{
	u64 ia = *(const u64*)a;
	u64 ib = *(const u64*)b;
	return (ia > ib) - (ia < ib);
}

RG_ALGO_DEFINE(int, IntAlgo, int_less);
RG_ALGO_DEFINE(Item, ItemAlgo, item_less);
#define RG_ALGO_I32_KEY(ptr) ((u32)(*(const i32*)(ptr)) ^ 0x80000000u)
RG_ALGO_DEFINE(i32, I32Algo, i32_less);
RG_ALGO_RADIX_U32_DEFINE(i32, I32Radix, RG_ALGO_I32_KEY);
RG_ALGO_RADIX_U32_DEFINE(Item32, Item32Radix, item32_key);
#define RG_ALGO_U64_KEY(ptr) (*(const u64*)(ptr))
RG_ALGO_DEFINE(u64, U64Algo, u64_less);
RG_ALGO_RADIX_U64_DEFINE(u64, U64Radix, RG_ALGO_U64_KEY);
RG_ALGO_RADIX_U64_DEFINE(Item64, Item64Radix, item64_key);

// =============================================================================
// Tests
// =============================================================================

static void test_sort_basic(void)
{
	enum { COUNT = 512 };
	const size_t count = COUNT;
	int data[COUNT];
	int ref[COUNT];

	u32 state = 0x12345678u;
	for (size_t i = 0; i < count; i++)
	{
		data[i] = (int)(rng_next(&state) % 1000u) - 500;
		ref[i] = data[i];
	}

	rg_algo_sort_IntAlgo(data, count);
	qsort(ref, count, sizeof(int), qsort_int_cmp);

	TEST_ASSERT(rg_algo_is_sorted_IntAlgo(data, count), "sort basic sorted");
	for (size_t i = 0; i < count; i++)
	{
		TEST_ASSERT(data[i] == ref[i], "sort basic matches qsort");
	}

	TEST_PASS();
	printf("  PASS: sort basic\n");
}

static void test_sort_nth_small(void)
{
	enum { MAX_COUNT = 33, TRIAL_COUNT = 24 };
	static const size_t sizes[] = {0, 1, 2, 3, 7, 23, 24, 25, 31, 32, 33};
	int original[MAX_COUNT];
	int data[MAX_COUNT];
	int ref[MAX_COUNT];
	u32 state = 0x31415926u;

	for (size_t size_idx = 0; size_idx < sizeof(sizes) / sizeof(sizes[0]); size_idx++)
	{
		size_t count = sizes[size_idx];
		for (size_t trial = 0; trial < TRIAL_COUNT; trial++)
		{
			for (size_t i = 0; i < count; i++)
			{
				if (trial == 0u)
				{
					original[i] = (int)(i / 3u) - 5;
				}
				else if (trial == 1u)
				{
					original[i] = (int)((count - 1u - i) / 3u) - 5;
				}
				else if (trial == 2u)
				{
					original[i] = 7;
				}
				else
				{
					original[i] = (int)(rng_next(&state) % 17u) - 8;
				}
			}

			memcpy(ref, original, count * sizeof(int));
			qsort(ref, count, sizeof(int), qsort_int_cmp);

			memcpy(data, original, count * sizeof(int));
			rg_algo_sort_IntAlgo(data, count);
			for (size_t i = 0; i < count; i++)
			{
				TEST_ASSERT(data[i] == ref[i], "small sort matches qsort");
			}

			for (size_t nth = 0; nth < count; nth++)
			{
				memcpy(data, original, count * sizeof(int));
				rg_algo_nth_element_IntAlgo(data, count, nth);
				TEST_ASSERT(data[nth] == ref[nth], "small nth value");
				for (size_t i = 0; i < nth; i++)
				{
					TEST_ASSERT(!int_less(&data[nth], &data[i]), "small nth left partition");
				}
				for (size_t i = nth + 1u; i < count; i++)
				{
					TEST_ASSERT(!int_less(&data[i], &data[nth]), "small nth right partition");
				}
			}
		}
	}

	TEST_PASS();
	printf("  PASS: small sort/nth guardrails\n");
}

static void test_stable_sort(void)
{
	enum { COUNT = 128 };
	const size_t count = COUNT;
	Item data[COUNT];
	Item scratch[COUNT];

	for (size_t i = 0; i < count; i++)
	{
		data[i].key = (int)(i % 7);
		data[i].id = (int)i;
	}

	rg_algo_stable_sort_ItemAlgo(data, count, scratch, count);

	for (size_t i = 1; i < count; i++)
	{
		TEST_ASSERT(data[i - 1].key <= data[i].key, "stable sort order");
		if (data[i - 1].key == data[i].key)
		{
			TEST_ASSERT(data[i - 1].id < data[i].id, "stable sort stability");
		}
	}

	TEST_PASS();
	printf("  PASS: stable sort\n");
}

static void test_stable_sort_patterns(void)
{
	enum { MAX_COUNT = 129 };
	static const size_t sizes[] = {0, 1, 2, 3, 23, 24, 25, 31, 32, 33,
								   63, 64, 65, 127, 128, 129};
	Item data[MAX_COUNT];
	Item scratch[MAX_COUNT];

	for (size_t size_idx = 0; size_idx < sizeof(sizes) / sizeof(sizes[0]); size_idx++)
	{
		size_t count = sizes[size_idx];
		for (size_t pattern = 0; pattern < 3u; pattern++)
		{
			for (size_t i = 0; i < count; i++)
			{
				if (pattern == 0u)
				{
					data[i].key = (int)(i / 3u);
				}
				else if (pattern == 1u)
				{
					data[i].key = (int)((count - 1u - i) / 3u);
				}
				else
				{
					data[i].key = 11;
				}
				data[i].id = (int)i;
			}

			rg_algo_stable_sort_ItemAlgo(data, count, scratch, count);
			for (size_t i = 1; i < count; i++)
			{
				TEST_ASSERT(data[i - 1u].key <= data[i].key, "stable pattern order");
				if (data[i - 1u].key == data[i].key)
				{
					TEST_ASSERT(data[i - 1u].id < data[i].id, "stable pattern stability");
				}
			}
		}
	}

	TEST_PASS();
	printf("  PASS: stable sort patterns/boundaries\n");
}

static void test_nth_element(void)
{
	enum { COUNT = 513 };
	const size_t count = COUNT;
	const size_t nth = count / 2;
	int data[COUNT];
	int ref[COUNT];

	u32 state = 0xdeadbeefu;
	for (size_t i = 0; i < count; i++)
	{
		data[i] = (int)(rng_next(&state) % 2000u) - 1000;
		ref[i] = data[i];
	}

	rg_algo_nth_element_IntAlgo(data, count, nth);
	qsort(ref, count, sizeof(int), qsort_int_cmp);

	TEST_ASSERT(data[nth] == ref[nth], "nth element value");
	for (size_t i = 0; i < nth; i++)
	{
		TEST_ASSERT(!int_less(&data[nth], &data[i]), "nth element left partition");
	}
	for (size_t i = nth + 1; i < count; i++)
	{
		TEST_ASSERT(!int_less(&data[i], &data[nth]), "nth element right partition");
	}

	TEST_PASS();
	printf("  PASS: nth element\n");
}

static void test_radix_sort_basic(void)
{
	enum { COUNT = 1024 };
	const size_t count = COUNT;
	i32 data[COUNT];
	i32 ref[COUNT];
	i32 scratch[COUNT];

	u32 state = 0x98765432u;
	for (size_t i = 0; i < count; i++)
	{
		data[i] = (i32)(rng_next(&state) ^ 0x80000000u);
		ref[i] = data[i];
	}

	rg_algo_radix_sort_I32Radix(data, count, scratch, count);
	qsort(ref, count, sizeof(i32), qsort_i32_cmp);

	TEST_ASSERT(rg_algo_is_sorted_I32Algo(data, count), "radix sort sorted");
	for (size_t i = 0; i < count; i++)
	{
		TEST_ASSERT(data[i] == ref[i], "radix sort matches qsort");
	}

	TEST_PASS();
	printf("  PASS: radix sort\n");
}

static void test_radix_sort_stable(void)
{
	enum { COUNT = 256 };
	Item32 data[COUNT];
	Item32 scratch[COUNT];

	for (size_t i = 0; i < COUNT; i++)
	{
		data[i].key = (i32)(i % 5);
		data[i].id = (i32)i;
	}

	rg_algo_radix_sort_Item32Radix(data, COUNT, scratch, COUNT);

	for (size_t i = 1; i < COUNT; i++)
	{
		TEST_ASSERT(data[i - 1].key <= data[i].key, "radix stable order");
		if (data[i - 1].key == data[i].key)
		{
			TEST_ASSERT(data[i - 1].id < data[i].id, "radix stable");
		}
	}

	TEST_PASS();
	printf("  PASS: radix stability\n");
}

static void test_radix_sort_u64(void)
{
	enum { COUNT = 257 };
	u64 data[COUNT];
	u64 ref[COUNT];
	u64 scratch[COUNT];
	Item64 items[COUNT];
	Item64 item_scratch[COUNT];
	u32 state = 0x6a09e667u;

	for (size_t pattern = 0; pattern < 4u; pattern++)
	{
		for (size_t i = 0; i < COUNT; i++)
		{
			if (pattern == 0u)
			{
				data[i] = rng_next_u64(&state);
			}
			else if (pattern == 1u)
			{
				data[i] = UINT64_C(0x8badf00d12345678);
			}
			else if (pattern == 2u)
			{
				data[i] = ((u64)rng_next(&state) << 32u) | UINT64_C(0x89abcdef);
			}
			else
			{
				data[i] = UINT64_C(0xdeadbeef00000000) | (u64)rng_next(&state);
			}
			ref[i] = data[i];
		}

		if (pattern == 0u)
		{
			data[0] = ref[0] = 0u;
			data[1] = ref[1] = UINT64_MAX;
			data[2] = ref[2] = UINT64_C(0x8000000000000000);
		}

		rg_algo_radix_sort_U64Radix(data, COUNT, scratch, COUNT);
		qsort(ref, COUNT, sizeof(u64), qsort_u64_cmp);
		TEST_ASSERT(rg_algo_is_sorted_U64Algo(data, COUNT), "u64 radix sorted");
		for (size_t i = 0; i < COUNT; i++)
		{
			TEST_ASSERT(data[i] == ref[i], "u64 radix matches qsort");
		}
	}

	for (size_t i = 0; i < COUNT; i++)
	{
		items[i].key = ((u64)(i % 7u) << 56u) | (u64)(i % 3u);
		items[i].id = (u32)i;
	}
	rg_algo_radix_sort_Item64Radix(items, COUNT, item_scratch, COUNT);
	for (size_t i = 1; i < COUNT; i++)
	{
		TEST_ASSERT(items[i - 1u].key <= items[i].key, "u64 radix stable order");
		if (items[i - 1u].key == items[i].key)
		{
			TEST_ASSERT(items[i - 1u].id < items[i].id, "u64 radix stability");
		}
	}

	TEST_PASS();
	printf("  PASS: u64 radix patterns/stability\n");
}

static void test_bounds_and_search(void)
{
	int data[] = {1, 2, 2, 2, 3, 5, 5, 7};
	const size_t count = sizeof(data) / sizeof(data[0]);

	int key = 2;
	size_t lower = rg_algo_lower_bound_IntAlgo(data, count, &key);
	size_t upper = rg_algo_upper_bound_IntAlgo(data, count, &key);
	TEST_ASSERT(lower == 1, "lower_bound key 2");
	TEST_ASSERT(upper == 4, "upper_bound key 2");

	size_t found = rg_algo_binary_search_IntAlgo(data, count, &key);
	TEST_ASSERT(found == lower, "binary_search first duplicate");
	TEST_ASSERT(data[found] == 2, "binary_search value");

	key = 4;
	lower = rg_algo_lower_bound_IntAlgo(data, count, &key);
	upper = rg_algo_upper_bound_IntAlgo(data, count, &key);
	TEST_ASSERT(lower == 5, "lower_bound missing");
	TEST_ASSERT(upper == 5, "upper_bound missing");
	found = rg_algo_binary_search_IntAlgo(data, count, &key);
	TEST_ASSERT(found == RG_ALGO_INVALID, "binary_search missing");

	key = 0;
	TEST_ASSERT(rg_algo_lower_bound_IntAlgo(data, count, &key) == 0, "lower_bound below");
	key = 9;
	TEST_ASSERT(rg_algo_lower_bound_IntAlgo(data, count, &key) == count, "lower_bound above");

	TEST_PASS();
	printf("  PASS: bounds/search\n");
}

static void test_minmax(void)
{
	int data[] = {5, -2, 9, 4, -7, 9, 0};
	const size_t count = sizeof(data) / sizeof(data[0]);

	size_t min_idx = rg_algo_min_index_IntAlgo(data, count);
	size_t max_idx = rg_algo_max_index_IntAlgo(data, count);
	TEST_ASSERT(min_idx == 4, "min index");
	TEST_ASSERT(max_idx == 2 || max_idx == 5, "max index");

	size_t min_pair = 0;
	size_t max_pair = 0;
	rg_algo_minmax_index_IntAlgo(data, count, &min_pair, &max_pair);
	TEST_ASSERT(min_pair == 4, "minmax min");
	TEST_ASSERT(max_pair == 2 || max_pair == 5, "minmax max");

	TEST_PASS();
	printf("  PASS: min/max\n");
}

static void test_is_sorted(void)
{
	int sorted[] = {-3, -1, 0, 2, 5, 9};
	int unsorted[] = {1, 3, 2, 4};

	TEST_ASSERT(rg_algo_is_sorted_IntAlgo(sorted, sizeof(sorted) / sizeof(sorted[0])) == 1,
				"is_sorted true");
	TEST_ASSERT(rg_algo_is_sorted_IntAlgo(unsorted, sizeof(unsorted) / sizeof(unsorted[0])) == 0,
				"is_sorted false");

	TEST_PASS();
	printf("  PASS: is_sorted\n");
}

static void test_empty_and_direct_helpers(void)
{
	int key = 4;
	size_t min_index = 0u;
	size_t max_index = 0u;

	rg_algo_sort_IntAlgo(NULL, 0u);
	TEST_ASSERT(rg_algo_is_sorted_IntAlgo(NULL, 0u) == 1, "empty is sorted");
	TEST_ASSERT(rg_algo_lower_bound_IntAlgo(NULL, 0u, &key) == 0u, "empty lower bound");
	TEST_ASSERT(rg_algo_upper_bound_IntAlgo(NULL, 0u, &key) == 0u, "empty upper bound");
	TEST_ASSERT(rg_algo_binary_search_IntAlgo(NULL, 0u, &key) == RG_ALGO_INVALID,
	            "empty binary search");
	TEST_ASSERT(rg_algo_min_index_IntAlgo(NULL, 0u) == RG_ALGO_INVALID, "empty min");
	TEST_ASSERT(rg_algo_max_index_IntAlgo(NULL, 0u) == RG_ALGO_INVALID, "empty max");
	rg_algo_minmax_index_IntAlgo(NULL, 0u, &min_index, &max_index);
	TEST_ASSERT(min_index == RG_ALGO_INVALID, "empty minmax min");
	TEST_ASSERT(max_index == RG_ALGO_INVALID, "empty minmax max");

	int single = 9;
	rg_algo_stable_sort_IntAlgo(&single, 1u, NULL, 0u);
	i32 single_i32 = 11;
	rg_algo_radix_sort_I32Radix(&single_i32, 1u, NULL, 0u);
	TEST_ASSERT(single == 9 && single_i32 == 11, "single-element no-op");

	int insertion[] = {5, -1, 3, 3, 0};
	rg_algo_insertion_sort_IntAlgo(insertion, RG_ARRAY_COUNT(insertion));
	TEST_ASSERT(rg_algo_is_sorted_IntAlgo(insertion, RG_ARRAY_COUNT(insertion)),
	            "direct insertion sort");

	int heap[] = {8, 2, 7, -4, 6, 1};
	rg_algo_heap_sort_IntAlgo(heap, RG_ARRAY_COUNT(heap));
	TEST_ASSERT(rg_algo_is_sorted_IntAlgo(heap, RG_ARRAY_COUNT(heap)), "direct heap sort");

	TEST_PASS();
	printf("  PASS: empty inputs/direct helpers\n");
}

// =============================================================================
// Main
// =============================================================================

static void run_tests(void)
{
	printf("\n=== rg_algo Correctness Tests ===\n\n");

	test_sort_basic();
	test_sort_nth_small();
	test_stable_sort();
	test_stable_sort_patterns();
	test_nth_element();
	test_radix_sort_basic();
	test_radix_sort_stable();
	test_radix_sort_u64();
	test_bounds_and_search();
	test_minmax();
	test_is_sorted();
	test_empty_and_direct_helpers();

	printf("\n=== Results: %d passed, %d failed ===\n", g_tests_passed, g_tests_failed);
}

int main(void)
{
	printf("rg_algo test suite\n");
	printf("==================\n");
	run_tests();

	return g_tests_failed > 0 ? 1 : 0;
}
