// rg_containers public API correctness tests

#define RG_CONTAINERS_ASSERT(condition) ((void)sizeof(condition))

#include "../src/rg_containers.h"
#include "../src/rg_containers.h"

#include <stdint.h>
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

typedef struct TestPair
{
	uint64_t first;
	uint64_t second;
} TestPair;

RG_ARRAY_DEFINE(int, IntArray);
RG_ARRAY_DEFINE(TestPair, PairArray);
RG_SMALLVEC_DEFINE(int, IntSmallVec, 4);
RG_RING_DEFINE(int, IntRing);
RG_SPARSE_SET_DEFINE(uint32_t, EntitySet);
RG_SPARSE_SET_DEFINE(int32_t, SignedEntitySet);

static RgArena arena;

static void test_capacity_helpers(void)
{
	CHECK(rg_container_next_cap(0, 1) >= 1);
	CHECK(rg_container_next_cap(8, 9) == 16);
	CHECK(rg_container_next_cap(8, 100) == 100);
	CHECK(rg_container_next_cap(100, 50) == 100);
	CHECK(rg_container_next_cap(SIZE_MAX / 2 + 1, SIZE_MAX) == SIZE_MAX);
}

static void test_array(void)
{
	rg_arena_reset(&arena);

	IntArray arr;
	rg_array_init(&arr, &arena);
	CHECK(rg_array_count(&arr) == 0);
	CHECK(rg_array_capacity(&arr) == 0);
	CHECK(rg_array_data(&arr) == NULL);

	rg_array_reserve(int, &arr, 64);
	CHECK(rg_array_capacity(&arr) >= 64);
	for (int i = 0; i < 16; i++)
		rg_array_push(int, &arr, i);
	CHECK(rg_array_count(&arr) == 16);
	CHECK(*rg_array_back(&arr) == 15);

	int value = 16;
	rg_array_push(int, &arr, value++);
	CHECK(value == 17);
	CHECK(arr.data[16] == 16);

	int* slot = rg_array_push_ptr(int, &arr);
	CHECK(slot != NULL);
	if (slot != NULL)
		*slot = 17;
	CHECK(rg_array_count(&arr) == 18);
	CHECK(arr.data[17] == 17);

	int old_second = arr.data[1];
	rg_array_insert(int, &arr, 0, arr.data[1]);
	CHECK(rg_array_count(&arr) == 19);
	CHECK(arr.data[0] == old_second);
	CHECK(arr.data[1] == 0);

	rg_array_insert(int, &arr, arr.len, 99);
	CHECK(*rg_array_back(&arr) == 99);
	rg_array_remove_shift(int, &arr, 0);
	CHECK(arr.data[0] == 0);

	size_t before_swap = arr.len;
	int last = arr.data[arr.len - 1];
	rg_array_remove_swap(&arr, 2);
	CHECK(arr.len == before_swap - 1);
	CHECK(arr.data[2] == last);
	CHECK(rg_array_pop(&arr) == 17);

	rg_array_resize(int, &arr, 40);
	CHECK(arr.len == 40);
	rg_array_resize(int, &arr, 3);
	CHECK(arr.len == 3);
	rg_array_clear(&arr);
	CHECK(arr.len == 0);
	CHECK(arr.cap >= 64);

	rg_array_free(&arr);
	CHECK(arr.data == NULL && arr.len == 0 && arr.cap == 0 && arr.arena == NULL);

	PairArray pairs;
	rg_array_init(&pairs, &arena);
	TestPair pair = {1, 2};
	rg_array_push(TestPair, &pairs, pair);
	CHECK(pairs.data != NULL);
	CHECK(RG_IS_ALIGNED(pairs.data, RG_ALIGNOF(TestPair)));
	CHECK(pairs.data[0].first == 1 && pairs.data[0].second == 2);
}

static void test_smallvec(void)
{
	rg_arena_reset(&arena);

	IntSmallVec vec;
	rg_smallvec_init(&vec, &arena);
	CHECK(rg_smallvec_is_inline(&vec));
	CHECK(rg_smallvec_capacity(&vec) == 4);

	for (int i = 0; i < 4; i++)
		rg_smallvec_push(int, &vec, i + 10);
	CHECK(rg_smallvec_is_inline(&vec));
	CHECK(*rg_smallvec_back(&vec) == 13);

	int* slot = rg_smallvec_push_ptr(int, &vec);
	CHECK(slot != NULL);
	if (slot != NULL)
		*slot = 14;
	CHECK(!rg_smallvec_is_inline(&vec));
	CHECK(rg_smallvec_count(&vec) == 5);
	for (int i = 0; i < 5; i++)
		CHECK(vec.data[i] == i + 10);

	int old_second = vec.data[1];
	rg_smallvec_insert(int, &vec, 0, vec.data[1]);
	CHECK(vec.data[0] == old_second);
	CHECK(vec.data[1] == 10);
	rg_smallvec_remove_shift(int, &vec, 0);
	CHECK(vec.data[0] == 10);
	rg_smallvec_remove_swap(&vec, 1);
	CHECK(vec.len == 4);
	CHECK(rg_smallvec_pop(&vec) == 13);

	rg_smallvec_clear(&vec);
	CHECK(vec.len == 0);
	CHECK(!rg_smallvec_is_inline(&vec));
	rg_smallvec_free(&vec);
	CHECK(vec.data == NULL && vec.len == 0 && vec.cap == 0 && vec.arena == NULL);
}

static void test_ring(void)
{
	rg_arena_reset(&arena);

	IntRing ring;
	rg_ring_init(int, &ring, &arena, 4);
	CHECK(ring.data != NULL);
	CHECK(rg_ring_empty(&ring));
	CHECK(rg_ring_capacity(&ring) == 4);
	CHECK(rg_ring_space(&ring) == 4);

	int out = -1;
	CHECK(!rg_ring_peek(int, &ring, &out));
	CHECK(out == -1);
	for (int i = 1; i <= 4; i++)
		CHECK(rg_ring_push(int, &ring, i));
	CHECK(rg_ring_full(&ring));
	CHECK(rg_ring_space(&ring) == 0);
	CHECK(!rg_ring_push(int, &ring, 5));
	CHECK(rg_ring_peek(int, &ring, &out));
	CHECK(out == 1);

	CHECK(rg_ring_pop(int, &ring, &out) && out == 1);
	CHECK(rg_ring_pop(int, &ring, &out) && out == 2);
	CHECK(rg_ring_push(int, &ring, 5));
	CHECK(rg_ring_push(int, &ring, 6));
	for (int expected = 3; expected <= 6; expected++)
		CHECK(rg_ring_pop(int, &ring, &out) && out == expected);
	CHECK(rg_ring_empty(&ring));

	for (int i = 1; i <= 4; i++)
		rg_ring_push_overwrite(int, &ring, i);
	rg_ring_push_overwrite(int, &ring, 5);
	CHECK(rg_ring_count(&ring) == 4);
	CHECK(rg_ring_pop(int, &ring, &out) && out == 2);

	rg_ring_clear(&ring);
	CHECK(rg_ring_empty(&ring));
	CHECK(rg_ring_capacity(&ring) == 4);
	rg_ring_free(&ring);
	CHECK(ring.data == NULL && ring.cap == 0 && ring.arena == NULL);
}

static void test_sparse_set(void)
{
	rg_arena_reset(&arena);

	EntitySet set;
	rg_sparse_set_init(&set, &arena);
	CHECK(rg_sparse_set_dense_count(&set) == 0);
	CHECK(rg_sparse_set_index_of(&set, 10u) == RG_SPARSE_INVALID);

	rg_sparse_set_reserve_dense(uint32_t, &set, 16);
	rg_sparse_set_reserve_sparse(&set, 15);
	CHECK(rg_sparse_set_dense_capacity(&set) >= 16);
	CHECK(rg_sparse_set_sparse_capacity(&set) >= 16);
	for (size_t i = 0; i < 16; i++)
		CHECK(set.sparse[i] == RG_SPARSE_INVALID);

	rg_sparse_set_insert(uint32_t, &set, 10u);
	rg_sparse_set_insert(uint32_t, &set, 20u);
	rg_sparse_set_insert(uint32_t, &set, 30u);
	rg_sparse_set_insert(uint32_t, &set, 20u);
	CHECK(rg_sparse_set_dense_count(&set) == 3);
	CHECK(rg_sparse_set_contains(&set, 10u));
	CHECK(rg_sparse_set_contains(&set, 20u));
	CHECK(rg_sparse_set_contains(&set, 30u));

	rg_sparse_set_remove(&set, 20u);
	CHECK(rg_sparse_set_dense_count(&set) == 2);
	CHECK(!rg_sparse_set_contains(&set, 20u));
	CHECK(rg_sparse_set_index_of(&set, 30u) < set.dense_count);
	rg_sparse_set_remove(&set, 999u);
	CHECK(rg_sparse_set_dense_count(&set) == 2);

	uint32_t* dense = rg_sparse_set_dense(&set);
	CHECK(dense != NULL);
	CHECK((dense[0] == 10u && dense[1] == 30u) ||
	      (dense[0] == 30u && dense[1] == 10u));

	rg_sparse_set_clear(&set);
	CHECK(rg_sparse_set_dense_count(&set) == 0);
	CHECK(!rg_sparse_set_contains(&set, 10u));
	rg_sparse_set_insert(uint32_t, &set, 10u);
	CHECK(rg_sparse_set_contains(&set, 10u));

	rg_sparse_set_free(&set);
	CHECK(set.dense == NULL && set.sparse == NULL && set.arena == NULL);

	SignedEntitySet signed_set;
	rg_sparse_set_init(&signed_set, &arena);
	CHECK(!rg_sparse_set_contains(&signed_set, -1));
}

static void test_allocation_failure(void)
{
	unsigned char storage[128];
	RgArena limited = {0};
	limited.memory = (char*)storage;
	limited.capacity = sizeof(storage);
	limited.committed = sizeof(storage);

	IntArray arr;
	rg_array_init(&arr, &limited);
	rg_array_reserve(int, &arr, 8);
	for (int i = 0; i < 8; i++)
		rg_array_push(int, &arr, i);
	CHECK(arr.len == 8);
	for (size_t i = 0; i < arr.len; i++)
		CHECK(arr.data[i] == (int)i);
	size_t old_len = arr.len;
	size_t old_cap = arr.cap;
	int* old_data = arr.data;
	limited.capacity = limited.used;
	rg_array_resize(int, &arr, SIZE_MAX);
	CHECK(arr.data == old_data && arr.len == old_len && arr.cap == old_cap);
	CHECK(rg_array_push_ptr(int, &arr) == NULL);

	char byte = 0;
	RgArena full = {0};
	full.memory = &byte;

	IntSmallVec vec;
	rg_smallvec_init(&vec, &full);
	for (int i = 0; i < 4; i++)
		rg_smallvec_push(int, &vec, i);
	CHECK(rg_smallvec_push_ptr(int, &vec) == NULL);
	CHECK(vec.data == vec.inline_data && vec.len == 4);

	IntRing ring;
	rg_ring_init(int, &ring, &full, 4);
	CHECK(ring.data == NULL && ring.cap == 0 && ring.mask == 0);
	CHECK(!rg_ring_push(int, &ring, 1));
	CHECK(ring.head == 0 && ring.tail == 0);

	rg_ring_init(int, &ring, &limited, 3);
	CHECK(ring.data == NULL && ring.cap == 0);
	rg_ring_init(int, &ring, &limited, (size_t)1 << (sizeof(size_t) * 8 - 1));
	CHECK(ring.data == NULL && ring.cap == 0);

	EntitySet set;
	rg_sparse_set_init(&set, &full);
	rg_sparse_set_reserve_dense(uint32_t, &set, 1);
	rg_sparse_set_reserve_sparse(&set, 1);
	CHECK(set.dense_count == 0 && set.dense == NULL && set.sparse == NULL);
	rg_sparse_set_reserve_sparse(&set, SIZE_MAX);
	CHECK(set.sparse == NULL && set.sparse_cap == 0);
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

	test_capacity_helpers();
	test_array();
	test_smallvec();
	test_ring();
	test_sparse_set();
	test_allocation_failure();

	rg_arena_free(&arena);
	rg_free();
	printf("rg_containers: %d checks, %d failures\n", tests_run, tests_failed);
	return tests_failed == 0 ? 0 : 1;
}
