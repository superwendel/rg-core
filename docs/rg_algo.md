# rg_algo

`rg_algo.h` generates type-safe sorting, selection, searching, and min/max
functions for caller-defined C types. It depends only on
[`rg_defs.h`](rg_defs.md) and performs no allocation.

## Setup

Define a strict less-than comparator, then instantiate the algorithms at file
scope:

```c
#include "rg_algo.h"

static int entity_less(const Entity* a, const Entity* b)
{
    return a->depth < b->depth;
}

RG_ALGO_DEFINE(Entity, EntityAlgo, entity_less);
```

The name argument becomes the suffix of every generated type and function.
Each generated function is `static inline`, so separate translation units and
unity builds do not share mutable state or require an implementation toggle.

Comparators must implement strict weak ordering. They should return nonzero
only when the first value belongs before the second value.

## Generated algorithms

`RG_ALGO_DEFINE(type, name, less_fn)` generates these primary operations:

| Operation | Generated function |
| --- | --- |
| Unstable sort | `rg_algo_sort_name(data, count)` |
| Stable sort | `rg_algo_stable_sort_name(data, count, scratch, scratch_count)` |
| Partial selection | `rg_algo_nth_element_name(data, count, nth)` |
| Sorted check | `rg_algo_is_sorted_name(data, count)` |
| Lower bound | `rg_algo_lower_bound_name(data, count, key)` |
| Upper bound | `rg_algo_upper_bound_name(data, count, key)` |
| Binary search | `rg_algo_binary_search_name(data, count, key)` |
| Minimum/maximum | `rg_algo_min_index_name` / `rg_algo_max_index_name` |
| Minimum and maximum | `rg_algo_minmax_index_name(data, count, out_min, out_max)` |

For the example above:

```c
Entity entities[256];
size_t entity_count = 0;

rg_algo_sort_EntityAlgo(entities, entity_count);
```

The regular sort recognizes already nondecreasing input and returns after one
linear scan. Nonincreasing input is reversed in place after the same scan.
Other inputs use introsort: partitioning handles large ranges, insertion sort
finishes small ranges, and heap sort provides the worst-case fallback. It is
not stable.

The stable sort uses insertion-sorted runs followed by stable merging. Scratch
must contain at least `count` elements and must not overlap the input:

```c
Entity scratch[256];
rg_algo_stable_sort_EntityAlgo(entities, entity_count,
                               scratch, RG_ARRAY_COUNT(scratch));
```

`rg_algo_nth_element_name` places the value that a complete sort would put at
`nth` into that position. Values before it do not compare greater and values
after it do not compare less, but neither side is otherwise sorted.

Search operations require sorted input. Binary search returns the first
equivalent element, or `RG_ALGO_INVALID` when the key is absent. Min/max
operations also return `RG_ALGO_INVALID` for an empty input.

Insertion sort and heap sort are also emitted as
`rg_algo_insertion_sort_name` and `rg_algo_heap_sort_name` when direct control
over the algorithm is useful.

## Radix sorting

The radix macros generate stable least-significant-digit sorts for unsigned
32-bit or 64-bit keys:

```c
typedef struct RenderItem
{
    i32 depth;
    u32 insertion_order;
} RenderItem;

static u32 render_item_key(const RenderItem* item)
{
    return (u32)item->depth ^ UINT32_C(0x80000000);
}

RG_ALGO_RADIX_U32_DEFINE(RenderItem, RenderItemRadix, render_item_key);

RenderItem items[1024];
RenderItem scratch[1024];
size_t item_count = 0;

rg_algo_radix_sort_RenderItemRadix(items, item_count,
                                   scratch, RG_ARRAY_COUNT(scratch));
```

XORing the sign bit maps signed two's-complement order into unsigned key order.
For naturally unsigned keys, return the key directly. Use
`RG_ALGO_RADIX_U64_DEFINE` for `u64` keys.

The key extractor should be deterministic and free of side effects because it
may be evaluated more than once per value. As with stable comparison sorting,
radix scratch must contain at least `count` elements and must not overlap the
input.

## Choosing a sort

| Requirement | Use |
| --- | --- |
| Arbitrary comparator, any C type, no allocation or caller scratch | `rg_algo_sort_name` |
| Stable arbitrary ordering with caller scratch | `rg_algo_stable_sort_name` |
| Stable 32-bit or 64-bit integer-key throughput with caller scratch | `rg_algo_radix_sort_name` |

## Performance

On the published one-million-element random `int32` workload, the typed radix
sort completed in 15.38 ms versus 89.63 ms for `std::sort` and 142.05 ms for
`qsort`. See the [core benchmark report](benchmarks/rg_core.md#rg_algo) for
record sorting, selection, methodology, comparison versions, and limitations.

## Configuration

Define options before including the header:

```c
#define RG_ALGO_ASSERT(condition)       // Custom assertion macro
#define RG_ALGO_INVALID SIZE_MAX        // Invalid index sentinel
#define RG_ALGO_INSERTION_CUTOFF 24     // Introsort insertion cutoff
#define RG_ALGO_STABLE_RUN 8            // Stable-sort insertion run
#define RG_ALGO_STACK_CAP 64             // Introsort range stack capacity
#define RG_ALGO_RADIX_BITS 8             // Bits processed per radix pass
#include "rg_algo.h"
```

`RG_ALGO_RADIX_BITS` may range from 4 through 16. More bits reduce the number
of passes but enlarge the stack-allocated bucket table. The default uses 256
buckets. If the introsort range stack reaches `RG_ALGO_STACK_CAP`, the current
range safely falls back to heap sort.

## Complexity

- Introsort: `O(n log n)` worst-case time and no caller scratch.
- Stable sort: `O(n log n)` time and `count * sizeof(type)` scratch.
- `nth_element`: linear expected time and quadratic worst-case time.
- Binary search and bounds: `O(log n)` time.
- Min/max operations: `O(n)` time.
- Radix sort: proportional to the processed passes, elements, and bucket count.

## Thread safety

The header has no mutable global state. Generated algorithms may run
concurrently on independent arrays. Concurrent access to the same array
requires caller synchronization.
