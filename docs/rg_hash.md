# rg_hash

`rg_hash.h` provides deterministic 64-bit hashing and arena-backed, typed
robin-hood hash maps and sets. It is a single-header C library built on
[`rg_mem.h`](rg_mem.md).

## Performance

For 500,000 reserved integer-key entries, insertion completed in 25.81 ms
versus 79.91 ms for `std::unordered_map`; removal completed in 25.97 ms versus
78.30 ms. See the [core benchmark report](benchmarks/rg-core.md#rg_hash) for
`stb_ds` results, methodology, and allocation details.

## Setup

Include `rg_hash.h`, initialize the global memory pool, and create an arena for
the table storage:

```c
#include "rg_hash.h"

rg_malloc(MB(64));
RgArena arena = rg_arena_create(MB(16));
```

All generated functions are `static inline`, so the header works naturally in
unity builds. Normal header guards also make repeated inclusion harmless.

## Hash functions

```c
uint64_t rg_hash_bytes(const void* data, size_t len, uint64_t seed);
uint64_t rg_hash_str(const char* str);
uint64_t rg_hash_u32(uint32_t value);
uint64_t rg_hash_u64(uint64_t value);
uint64_t rg_hash_ptr(const void* ptr);
```

`rg_hash_bytes` implements xxHash64 and produces the same result on
little-endian and big-endian targets. A null data pointer is valid only when
`len` is zero. `rg_hash_str` uses `RG_HASH_SEED` and expects a non-null,
null-terminated string.

Equality helpers are provided for common key types:

- `rg_hash_eq_u32`
- `rg_hash_eq_u64`
- `rg_hash_eq_ptr`
- `rg_hash_eq_str`

## Hash maps

Define a map type at file scope:

```c
RG_HASH_MAP_DEFINE(uint32_t, float, U32FloatMap,
                   rg_hash_u32, rg_hash_eq_u32);
```

Then initialize and use it:

```c
U32FloatMap map;
rg_hash_map_init(U32FloatMap, &map, &arena);

if (!rg_hash_map_reserve(U32FloatMap, &map, 1024))
{
    // The arena could not provide enough storage.
}

int is_new = 0;
rg_hash_map_put(U32FloatMap, &map, 42u, 3.14f, &is_new);

float* value = rg_hash_map_get_ptr(U32FloatMap, &map, 42u);

int removed = 0;
rg_hash_map_remove(U32FloatMap, &map, 42u, &removed);
```

Map operations:

- `rg_hash_map_init`, `rg_hash_map_clear`, `rg_hash_map_free`
- `rg_hash_map_reserve`
- `rg_hash_map_put`, `rg_hash_map_try_put`, `rg_hash_map_remove`
- `rg_hash_map_get_ptr`, `rg_hash_map_get_ptr_const`
- `rg_hash_map_contains`
- `rg_hash_map_count`, `rg_hash_map_capacity`, `rg_hash_map_is_empty`

`rg_hash_map_reserve` returns nonzero on success and zero when the requested
capacity cannot be represented or the arena cannot satisfy the allocation.
`rg_hash_map_try_put` returns `1` for an insertion, `0` for an update, and `-1`
for allocation failure. The regular `rg_hash_map_put` wrapper writes only the
inserted/not-inserted result. Its optional `out_is_new` argument and the
`out_removed` argument are `int*`; pass `NULL` when a result is not needed.

## Hash sets

Define and use a set in the same way:

```c
RG_HASH_SET_DEFINE(uint64_t, EntitySet, rg_hash_u64, rg_hash_eq_u64);

EntitySet entities;
rg_hash_set_init(EntitySet, &entities, &arena);
rg_hash_set_insert(EntitySet, &entities, entity_id, NULL);

if (rg_hash_set_contains(EntitySet, &entities, entity_id))
{
    // Found it.
}
```

Set operations:

- `rg_hash_set_init`, `rg_hash_set_clear`, `rg_hash_set_free`
- `rg_hash_set_reserve`
- `rg_hash_set_insert`, `rg_hash_set_try_insert`, `rg_hash_set_remove`
- `rg_hash_set_contains`
- `rg_hash_set_count`, `rg_hash_set_capacity`, `rg_hash_set_is_empty`

`rg_hash_set_try_insert` returns `1` for a new key, `0` when the key already
exists, and `-1` for allocation failure.

## Arena ownership and pointer lifetime

Growing a table allocates a replacement array from its arena. The old array is
not reclaimed until the arena is reset or released. Reserving the expected
entry count up front avoids those abandoned growth allocations.

`rg_hash_map_clear` and `rg_hash_set_clear` retain the current allocation for
reuse. The `free` operations only detach the table from its arena; they do not
reclaim arena memory.

Pointers returned by `rg_hash_map_get_ptr` can be invalidated by insertion,
reservation, or removal. Keys and values are copied into table entries. Pointer
and string keys remain owned by the caller, so their referenced storage must
remain valid and string contents must not change while stored.

## Configuration

Define options before including the header:

```c
#define RG_HASH_ASSERT(condition)  // Custom assertion macro
#define RG_HASH_MIN_CAP            // Minimum capacity; default: 8
#define RG_HASH_MAX_LOAD_NUM       // Load numerator; default: 7
#define RG_HASH_MAX_LOAD_DEN       // Load denominator; default: 10
#define RG_HASH_SEED               // Default string/integer seed; default: 0
#define RG_HASH_INVALID            // Invalid index sentinel; default: SIZE_MAX
```

The load numerator and denominator must describe a value strictly between zero
and one. Hash value zero is reserved for empty slots and is remapped internally,
so custom hash functions may still return zero.

## Complexity and thread safety

Lookup, insertion, and removal are O(1) on average and O(n) under pathological
collisions. A map or set may be used by only one thread at a time unless access
is synchronized externally. Separate tables and arenas may be used concurrently.
