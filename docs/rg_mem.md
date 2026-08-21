# rg_mem

`rg_mem.h` provides a global operating-system memory pool and lightweight bump
arenas for persistent, temporary, and per-frame allocations.

## Integration

```c
#include "rg_mem.h"

int main(void)
{
  if (rg_malloc(MB(256)) != 0)
    return 1;

  RgArena persistent = rg_arena_create(MB(128));
  RgArena scratch = rg_arena_create(MB(4));

  GameState* state = RG_ARENA_PUSH_STRUCT(&persistent, GameState);
  Vec3* positions = RG_ARENA_PUSH_ARRAY(&scratch, Vec3, 1000);

  rg_arena_reset(&scratch);
  rg_arena_free(&scratch);
  rg_arena_free(&persistent);
  rg_free();
  return state != NULL && positions != NULL ? 0 : 1;
}
```

Call `rg_malloc` once before creating arenas. Repeated calls are successful
no-ops until `rg_free` releases the pool. The pool size is rounded up to the
operating-system page size.

## Pool API

```c
int rg_malloc(size_t total_bytes);
void rg_free(void);
size_t rg_total(void);
size_t rg_used(void);
size_t rg_remaining(void);
```

`rg_used` includes arena capacity and any alignment between arenas. Destroying
an individual arena does not reclaim pool space; all pool memory is released
together by `rg_free`.

## Arena API

```c
RgArena rg_arena_create(size_t capacity);
void* rg_arena_alloc(RgArena* arena, size_t size);
void* rg_arena_alloc_aligned(RgArena* arena, size_t size, size_t alignment);
void* rg_arena_alloc_array(RgArena* arena, size_t element_size,
                           size_t count, size_t alignment);
void rg_arena_reset(RgArena* arena);
void rg_arena_decommit(RgArena* arena);
void rg_arena_free(RgArena* arena);
size_t rg_arena_remaining(const RgArena* arena);
```

Allocation returns `NULL` for zero-sized requests, invalid alignment, integer
overflow, or insufficient capacity. `RG_ARENA_PUSH_STRUCT` and
`RG_ARENA_PUSH_ARRAY` align allocations to the requested type; array-size
multiplication is checked for overflow.

`rg_arena_reset` makes all arena storage reusable. With `RG_MALLOC_SECURE`, it
first clears the used bytes. On lazy-commit platforms, `rg_arena_decommit`
releases committed pages while retaining the reserved address range.

## Configuration

Define options before including the header:

```c
#define RG_MALLOC_SECURE
#define RG_MALLOC_ASSERT(condition) my_assert(condition)
#define RG_MALLOC_DEFAULT_ALIGNMENT 16
#define RG_MALLOC_LAZY_COMMIT 1
#include "rg_mem.h"
```

Lazy commit uses `VirtualAlloc` and is currently Windows-only. It defaults to
enabled on Windows. Linux and macOS use `mmap` with lazy commit disabled.

## Unity builds and threads

The pool state is `static`. A unity build has one shared pool, while separately
compiled translation units each receive independent pool state.

Initialize and release the pool, and create arenas, from one thread. An arena
can be used by one thread at a time; use separate arenas for concurrent work.
