// rg_mem - Arena-based memory allocation for C
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header global pool and bump-arena allocator.
//
// USAGE:
//   #include "rg_mem.h"
//
//   rg_malloc(MB(256));
//   RgArena persistent = rg_arena_create(MB(128));
//   RgArena scratch = rg_arena_create(MB(4));
//
//   GameState* state = RG_ARENA_PUSH_STRUCT(&persistent, GameState);
//   Vec3* positions = RG_ARENA_PUSH_ARRAY(&scratch, Vec3, 1000);
//   rg_arena_reset(&scratch);
//
//   rg_arena_free(&scratch);
//   rg_arena_free(&persistent);
//   rg_free();
//
// OPTIONS:
//   #define RG_MALLOC_SECURE             - Zero used memory on arena reset
//   #define RG_MALLOC_ASSERT(x)           - Custom assertion macro
//   #define RG_MALLOC_DEFAULT_ALIGNMENT   - Default allocation alignment (16)
//   #define RG_MALLOC_LAZY_COMMIT         - Reserve first and commit on demand
//
// UNITY BUILD NOTE:
//   - The global pool state is static. A unity build shares one pool;
//     separately compiled translation units each receive independent state.
//
// THREAD SAFETY:
//   - Initialize the pool and create arenas from one thread.
//   - An arena can be used by one thread at a time. Use separate arenas for
//     concurrent allocation.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MEM_H
#define RG_MEM_H

#include "rg_defs.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef RG_MALLOC_ASSERT
#include <assert.h>
#define RG_MALLOC_ASSERT(condition) assert(condition)
#endif

#ifndef RG_MALLOC_DEFAULT_ALIGNMENT
#define RG_MALLOC_DEFAULT_ALIGNMENT 16
#endif

#if RG_MALLOC_DEFAULT_ALIGNMENT == 0 || \
	(RG_MALLOC_DEFAULT_ALIGNMENT & (RG_MALLOC_DEFAULT_ALIGNMENT - 1)) != 0
#error RG_MALLOC_DEFAULT_ALIGNMENT must be a nonzero power of two
#endif

#ifndef RG_MALLOC_LAZY_COMMIT
#if RG_PLATFORM_WINDOWS
#define RG_MALLOC_LAZY_COMMIT 1
#else
#define RG_MALLOC_LAZY_COMMIT 0
#endif
#endif

#if RG_MALLOC_LAZY_COMMIT != 0 && RG_MALLOC_LAZY_COMMIT != 1
#error RG_MALLOC_LAZY_COMMIT must be 0 or 1
#endif

#if RG_MALLOC_LAZY_COMMIT && !RG_PLATFORM_WINDOWS
#error RG_MALLOC_LAZY_COMMIT is currently supported only on Windows
#endif

typedef struct RgArena
{
	char* memory;
	size_t capacity;
	size_t used;
	size_t committed;
} RgArena;

static int rg_malloc(size_t total_bytes);
static void rg_free(void);
static size_t rg_remaining(void);
static size_t rg_used(void);
static size_t rg_total(void);

static RgArena rg_arena_create(size_t capacity);
RGINLINE void* rg_arena_alloc(RgArena* arena, size_t size);
RGINLINE void* rg_arena_alloc_aligned(RgArena* arena, size_t size, size_t alignment);
RGINLINE void* rg_arena_alloc_array(RgArena* arena, size_t element_size, size_t count, size_t alignment);
RGINLINE void rg_arena_reset(RgArena* arena);
RGINLINE void rg_arena_decommit(RgArena* arena);
static void rg_arena_free(RgArena* arena);
RGINLINE size_t rg_arena_remaining(const RgArena* arena);

#define RG_ARENA_PUSH_STRUCT(arena, type) \
	((type*)rg_arena_alloc_aligned((arena), sizeof(type), RG_ALIGNOF(type)))

#define RG_ARENA_PUSH_ARRAY(arena, type, count) \
	((type*)rg_arena_alloc_array((arena), sizeof(type), (count), RG_ALIGNOF(type)))

// =============================================================================
// PLATFORM MEMORY
// =============================================================================

#if RG_PLATFORM_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

RGINLINE void* rg_mem_os_alloc(size_t size)
{
#if RG_MALLOC_LAZY_COMMIT
	return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE);
#else
	return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#endif
}

RGINLINE void rg_mem_os_free(void* ptr, size_t size)
{
	RG_UNUSED(size);
	VirtualFree(ptr, 0, MEM_RELEASE);
}

#if RG_MALLOC_LAZY_COMMIT
RGINLINE int rg_mem_os_commit(void* ptr, size_t size)
{
	return VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != NULL;
}

RGINLINE void rg_mem_os_decommit(void* ptr, size_t size)
{
	if (ptr != NULL && size > 0)
		VirtualFree(ptr, size, MEM_DECOMMIT);
}
#endif

#elif RG_PLATFORM_LINUX || RG_PLATFORM_MACOS
#include <sys/mman.h>
#include <unistd.h>

#if RG_PLATFORM_MACOS && !defined(MAP_ANONYMOUS)
#define MAP_ANONYMOUS MAP_ANON
#endif

RGINLINE void* rg_mem_os_alloc(size_t size)
{
	void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	return ptr == MAP_FAILED ? NULL : ptr;
}

RGINLINE void rg_mem_os_free(void* ptr, size_t size)
{
	munmap(ptr, size);
}

#else
#error rg_mem.h requires Windows, Linux, or macOS
#endif

// =============================================================================
// GLOBAL POOL
// =============================================================================

static char* rg_mem_pool;
static size_t rg_mem_pool_size;
static size_t rg_mem_pool_used;
static size_t rg_mem_page_size = 4096u;
static int rg_mem_initialized;

RGINLINE int rg_mem_align_up_size(size_t value, size_t alignment, size_t* result)
{
	if (alignment == 0 || (alignment & (alignment - 1)) != 0)
		return 0;
	size_t mask = alignment - 1;
	if (value > SIZE_MAX - mask)
		return 0;
	*result = (value + mask) & ~mask;
	return 1;
}

static int rg_malloc(size_t total_bytes)
{
	if (rg_mem_initialized)
		return 0;
	if (total_bytes == 0)
		return -1;

#if RG_PLATFORM_WINDOWS
	SYSTEM_INFO info = {0};
	GetSystemInfo(&info);
	if (info.dwPageSize > 0)
		rg_mem_page_size = (size_t)info.dwPageSize;
#else
	long page_size = sysconf(_SC_PAGESIZE);
	if (page_size > 0)
		rg_mem_page_size = (size_t)page_size;
#endif

	if (!rg_mem_align_up_size(total_bytes, rg_mem_page_size, &total_bytes))
		return -1;
	rg_mem_pool = (char*)rg_mem_os_alloc(total_bytes);
	if (rg_mem_pool == NULL)
		return -1;

	rg_mem_pool_size = total_bytes;
	rg_mem_pool_used = 0;
	rg_mem_initialized = 1;
	return 0;
}

static void rg_free(void)
{
	if (rg_mem_pool != NULL)
		rg_mem_os_free(rg_mem_pool, rg_mem_pool_size);
	rg_mem_pool = NULL;
	rg_mem_pool_size = 0;
	rg_mem_pool_used = 0;
	rg_mem_page_size = 4096u;
	rg_mem_initialized = 0;
}

static size_t rg_remaining(void)
{
	return rg_mem_pool_size - rg_mem_pool_used;
}

static size_t rg_used(void)
{
	return rg_mem_pool_used;
}

static size_t rg_total(void)
{
	return rg_mem_pool_size;
}

RGINLINE void* rg_mem_pool_alloc(size_t size)
{
	if (size == 0)
		return NULL;
	RG_MALLOC_ASSERT(rg_mem_initialized && "rg_malloc() must be called first");
	if (!rg_mem_initialized)
		return NULL;

#if RG_MALLOC_LAZY_COMMIT
	size_t alignment = rg_mem_page_size;
#else
	size_t alignment = RG_MALLOC_DEFAULT_ALIGNMENT;
#endif
	size_t aligned_offset;
	if (!rg_mem_align_up_size(rg_mem_pool_used, alignment, &aligned_offset))
		return NULL;
	if (aligned_offset > rg_mem_pool_size || size > rg_mem_pool_size - aligned_offset)
		return NULL;

	void* ptr = rg_mem_pool + aligned_offset;
	rg_mem_pool_used = aligned_offset + size;
	return ptr;
}

// =============================================================================
// ARENAS
// =============================================================================

static RgArena rg_arena_create(size_t capacity)
{
	RgArena arena = {0};
	if (capacity == 0)
		return arena;

#if RG_MALLOC_LAZY_COMMIT
	if (!rg_mem_align_up_size(capacity, rg_mem_page_size, &capacity))
		return arena;
#endif

	arena.memory = (char*)rg_mem_pool_alloc(capacity);
	if (arena.memory == NULL)
		return arena;
	arena.capacity = capacity;
#if RG_MALLOC_LAZY_COMMIT
	arena.committed = 0;
#else
	arena.committed = capacity;
#endif
	return arena;
}

RGINLINE void* rg_arena_alloc(RgArena* arena, size_t size)
{
	return rg_arena_alloc_aligned(arena, size, RG_MALLOC_DEFAULT_ALIGNMENT);
}

RGINLINE void* rg_arena_alloc_aligned(RgArena* arena, size_t size, size_t alignment)
{
	RG_MALLOC_ASSERT(arena != NULL && arena->memory != NULL);
	RG_MALLOC_ASSERT(alignment != 0 && (alignment & (alignment - 1)) == 0);
	if (arena == NULL || arena->memory == NULL || size == 0 ||
	    alignment == 0 || (alignment & (alignment - 1)) != 0)
		return NULL;

	uintptr_t current = (uintptr_t)(arena->memory + arena->used);
	uintptr_t mask = (uintptr_t)alignment - 1u;
	size_t padding = (size_t)(((uintptr_t)alignment - (current & mask)) & mask);
	if (arena->used > arena->capacity || padding > arena->capacity - arena->used)
		return NULL;
	size_t aligned_offset = arena->used + padding;
	if (size > arena->capacity - aligned_offset)
		return NULL;
	size_t new_used = aligned_offset + size;

#if RG_MALLOC_LAZY_COMMIT
	if (new_used > arena->committed)
	{
		size_t commit_target;
		if (!rg_mem_align_up_size(new_used, rg_mem_page_size, &commit_target) ||
		    commit_target > arena->capacity)
			return NULL;
		size_t commit_size = commit_target - arena->committed;
		void* commit_ptr = arena->memory + arena->committed;
		if (!rg_mem_os_commit(commit_ptr, commit_size))
			return NULL;
		arena->committed = commit_target;
	}
#endif

	arena->used = new_used;
	return arena->memory + aligned_offset;
}

RGINLINE void* rg_arena_alloc_array(RgArena* arena, size_t element_size, size_t count, size_t alignment)
{
	if (element_size == 0 || count == 0 || count > SIZE_MAX / element_size)
		return NULL;
	return rg_arena_alloc_aligned(arena, element_size * count, alignment);
}

RGINLINE void rg_arena_reset(RgArena* arena)
{
	RG_MALLOC_ASSERT(arena != NULL);
	if (arena == NULL)
		return;
#if defined(RG_MALLOC_SECURE)
	if (arena->memory != NULL && arena->used > 0)
		memset(arena->memory, 0, arena->used);
#endif
	arena->used = 0;
}

RGINLINE void rg_arena_decommit(RgArena* arena)
{
#if RG_MALLOC_LAZY_COMMIT
	if (arena == NULL || arena->memory == NULL || arena->committed == 0)
		return;
	rg_mem_os_decommit(arena->memory, arena->committed);
	arena->committed = 0;
	arena->used = 0;
#else
	RG_UNUSED(arena);
#endif
}

static void rg_arena_free(RgArena* arena)
{
	if (arena == NULL)
		return;
#if RG_MALLOC_LAZY_COMMIT
	rg_arena_decommit(arena);
#endif
	memset(arena, 0, sizeof(*arena));
}

RGINLINE size_t rg_arena_remaining(const RgArena* arena)
{
	RG_MALLOC_ASSERT(arena != NULL);
	if (arena == NULL || arena->used > arena->capacity)
		return 0;
	return arena->capacity - arena->used;
}

#endif // RG_MEM_H
