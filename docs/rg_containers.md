# rg_containers

`rg_containers.h` provides macro-typed dynamic arrays, small vectors, ring
buffers, and sparse sets backed by [`rg_mem.h`](rg_mem.md). The containers use
direct element storage and require no implementation block.

## Performance

In the published benchmark, one million pushes into a reserved `rg_array`
completed in 0.69 ms versus 1.48 ms for `std::vector`. Reserved sparse-set
insertion completed in 0.38 ms versus 2.01 ms for EnTT. See the
[core benchmark report](benchmarks/rg-core.md#rg_containers) for the complete
container results, methodology, and allocation caveats.

## Setup

Initialize the global memory pool, create an arena, and define container types
at file scope:

```c
#include "src/rg_containers.h"

RG_ARRAY_DEFINE(int, IntArray);
RG_SMALLVEC_DEFINE(int, IntSmallVec, 8);
RG_RING_DEFINE(int, IntRing);
RG_SPARSE_SET_DEFINE(uint32_t, EntitySet);

rg_malloc(MB(64));
RgArena arena = rg_arena_create(MB(16));
```

The header has a normal include guard, so repeated inclusion is harmless. Its
helper functions have internal linkage and work naturally in unity builds.

## Dynamic arrays

```c
IntArray values;
rg_array_init(&values, &arena);
rg_array_reserve(int, &values, 1024);
rg_array_push(int, &values, 42);

int* next = rg_array_push_ptr(int, &values);
if (next != NULL)
    *next = 99;

int last = rg_array_pop(&values);
```

Array operations:

- `rg_array_init`, `rg_array_clear`, `rg_array_free`
- `rg_array_reserve`, `rg_array_resize`
- `rg_array_push`, `rg_array_push_ptr`, `rg_array_pop`
- `rg_array_insert`, `rg_array_remove_swap`, `rg_array_remove_shift`
- `rg_array_count`, `rg_array_capacity`, `rg_array_data`, `rg_array_back`

`rg_array_resize` does not initialize newly exposed elements. Insert and shift
removal preserve order; swap removal replaces the removed element with the
last element. `back` and `pop` require a nonempty array.

## Small vectors

A small vector stores its first elements inside the container and allocates
only when that inline capacity is exceeded:

```c
IntSmallVec values;
rg_smallvec_init(&values, &arena);
rg_smallvec_push(int, &values, 42);

if (rg_smallvec_is_inline(&values))
{
	// No arena allocation has occurred.
}
```

The inline capacity passed to `RG_SMALLVEC_DEFINE` must be greater than zero.
Once a vector spills into its arena, clearing it retains the allocated storage;
it does not move back to inline storage.

Small-vector operations:

- `rg_smallvec_init`, `rg_smallvec_clear`, `rg_smallvec_free`
- `rg_smallvec_reserve`, `rg_smallvec_push`, `rg_smallvec_push_ptr`
- `rg_smallvec_pop`, `rg_smallvec_insert`
- `rg_smallvec_remove_swap`, `rg_smallvec_remove_shift`
- `rg_smallvec_count`, `rg_smallvec_capacity`, `rg_smallvec_data`
- `rg_smallvec_back`, `rg_smallvec_is_inline`

## Ring buffers

Ring buffers have a fixed, power-of-two capacity. Masked indexing keeps their
push and pop paths constant time.

```c
IntRing events;
rg_ring_init(int, &events, &arena, 1024);

if (rg_ring_push(int, &events, 7))
{
	int event;
	rg_ring_pop(int, &events, &event);
}
```

`rg_ring_push` returns zero when the ring is full. `rg_ring_push_overwrite`
instead discards the oldest value. `rg_ring_pop` and `rg_ring_peek` return zero
when the ring is empty and leave the output unchanged.

Ring operations:

- `rg_ring_init`, `rg_ring_clear`, `rg_ring_free`
- `rg_ring_push`, `rg_ring_push_overwrite`, `rg_ring_pop`, `rg_ring_peek`
- `rg_ring_count`, `rg_ring_capacity`, `rg_ring_space`
- `rg_ring_empty`, `rg_ring_full`

## Sparse sets

Sparse sets provide constant-time membership, insertion, and removal for
integer IDs while keeping active IDs in a compact dense array.

```c
EntitySet entities;
rg_sparse_set_init(&entities, &arena);
rg_sparse_set_reserve_dense(uint32_t, &entities, 4096);
rg_sparse_set_reserve_sparse(&entities, 65535);

rg_sparse_set_insert(uint32_t, &entities, entity_id);
if (rg_sparse_set_contains(&entities, entity_id))
{
	size_t dense_index = rg_sparse_set_index_of(&entities, entity_id);
}
```

IDs must be nonnegative, convertible to `size_t`, and less than `SIZE_MAX`.
Removal swaps the final dense ID into the removed slot, so dense order is not
stable. Clearing a set retains both allocations.

Sparse-set operations:

- `rg_sparse_set_init`, `rg_sparse_set_clear`, `rg_sparse_set_free`
- `rg_sparse_set_reserve_dense`, `rg_sparse_set_reserve_sparse`
- `rg_sparse_set_insert`, `rg_sparse_set_remove`
- `rg_sparse_set_contains`, `rg_sparse_set_index_of`
- `rg_sparse_set_dense`, `rg_sparse_set_dense_count`
- `rg_sparse_set_dense_capacity`, `rg_sparse_set_sparse_capacity`

## Arena behavior and failure handling

Growth allocates replacement storage and copies existing elements. The old
block remains in the arena until that arena is reset or released. Reserve the
expected capacity up front to avoid abandoned growth blocks and make memory use
predictable.

`clear` retains storage. The `free` macros only detach and zero a container;
they do not reclaim individual arena blocks. Resetting or releasing an arena
invalidates every container and element pointer backed by it.

The default assertion reports invalid arguments and arena exhaustion. Reserve
operations preserve existing storage and capacity when growth fails, and a
failed ring allocation leaves its data pointer null and capacity zero. The
regular push, insert, and sparse-set mutation macros are deliberately minimal
and assume that their arena can satisfy required growth. If exhaustion must be
recoverable, reserve first and verify that the requested capacity was reached.
Likewise, verify ring initialization before calling its mutation macros when a
custom assertion handler returns.

## Performance and complexity

- Array and small-vector push/pop: amortized O(1)
- Ordered array and small-vector insertion/removal: O(n)
- Swap removal: O(1)
- Ring push/pop/peek: O(1)
- Sparse-set membership, insertion, and removal: O(1)

Container operations are macros so element access remains typed and inline.
Pass pointers to stable container lvalues and use side-effect-free expressions:
some macro arguments can be evaluated more than once. Operations on one
container and its arena require external synchronization; separate containers
and arenas can be used concurrently.

## Configuration

Define options before including the header:

```c
#define RG_CONTAINERS_ASSERT(condition)  // Custom assertion macro
#define RG_CONTAINERS_MIN_CAP             // Initial growth capacity; default: 8
#define RG_SPARSE_INVALID                 // Missing dense index; default: SIZE_MAX
```

`RG_CONTAINERS_MIN_CAP` must be at least one.
