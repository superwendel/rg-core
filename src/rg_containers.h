// rg_containers - Arena-backed containers for C
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header macro-typed dynamic arrays, small vectors, ring buffers, and
// sparse sets backed by rg_mem arenas.
//
// USAGE:
//   #include "rg_containers.h"
//
//   RG_ARRAY_DEFINE(int, IntArray);
//   RG_SMALLVEC_DEFINE(int, IntSmallVec, 8);
//   RG_RING_DEFINE(int, IntRing);
//   RG_SPARSE_SET_DEFINE(uint32_t, EntitySet);
//
//   RgArena arena = rg_arena_create(MB(1));
//   IntArray arr;
//   rg_array_init(&arr, &arena);
//   rg_array_push(int, &arr, 42);
//
// OPTIONS:
//   #define RG_CONTAINERS_ASSERT(x)  - Custom assert macro
//   #define RG_CONTAINERS_MIN_CAP    - Default minimum capacity (default: 8)
//   #define RG_SPARSE_INVALID        - Missing dense index (default: SIZE_MAX)
//
// NOTES:
//   - Growth allocates new storage; old blocks remain until the arena is reset.
//   - Reserve expected capacities up front for predictable memory use.
//   - Container pointer arguments must refer to stable lvalues.
//   - A container and its arena may be used by only one thread at a time.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_CONTAINERS_H
#define RG_CONTAINERS_H

#include "rg_mem.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

// =============================================================================
// CONFIGURATION
// =============================================================================

#ifndef RG_CONTAINERS_ASSERT
#include <assert.h>
#define RG_CONTAINERS_ASSERT(x) assert(x)
#endif

#ifndef RG_CONTAINERS_MIN_CAP
#define RG_CONTAINERS_MIN_CAP 8
#endif

#if RG_CONTAINERS_MIN_CAP < 1
#error RG_CONTAINERS_MIN_CAP must be at least 1
#endif

#ifndef RG_SPARSE_INVALID
#define RG_SPARSE_INVALID SIZE_MAX
#endif

// =============================================================================
// TYPE DEFINITIONS
// =============================================================================

/**
 * @brief Define a typed dynamic array struct
 * @param type Element type
 * @param name Struct name
 */
#define RG_ARRAY_DEFINE(type, name) \
	typedef struct name             \
	{                               \
		type* data;                 \
		size_t len;                 \
		size_t cap;                 \
		RgArena* arena;             \
	} name

/**
 * @brief Define a typed small-vector struct with inline storage
 * @param type Element type
 * @param name Struct name
 * @param inline_cap Inline element capacity
 */
#define RG_SMALLVEC_DEFINE(type, name, inline_cap) \
	typedef struct name                            \
	{                                              \
		type* data;                                \
		size_t len;                                \
		size_t cap;                                \
		RgArena* arena;                            \
		type inline_data[inline_cap];              \
	} name

/**
 * @brief Define a typed ring buffer struct (capacity must be power of two)
 * @param type Element type
 * @param name Struct name
 */
#define RG_RING_DEFINE(type, name) \
	typedef struct name            \
	{                              \
		type* data;                \
		size_t head;               \
		size_t tail;               \
		size_t cap;                \
		size_t mask;               \
		RgArena* arena;            \
	} name

/**
 * @brief Define a typed sparse set struct
 * @param id_type ID type (must be non-negative, convertible to size_t)
 * @param name Struct name
 */
#define RG_SPARSE_SET_DEFINE(id_type, name) \
	typedef struct name                     \
	{                                       \
		id_type* dense;                     \
		size_t dense_count;                 \
		size_t dense_cap;                   \
		size_t* sparse;                     \
		size_t sparse_cap;                  \
		RgArena* arena;                     \
	} name

// =============================================================================
// PUBLIC API - Dynamic Array
// =============================================================================

/**
 * @brief Initialize a dynamic array with an arena
 * @param arr Pointer to array
 * @param arena_ptr Arena for allocations
 */
#define rg_array_init(arr, arena_ptr) \
	do                                \
	{                                 \
		(arr)->data = NULL;           \
		(arr)->len = 0;               \
		(arr)->cap = 0;               \
		(arr)->arena = (arena_ptr);   \
	} while (0)

/**
 * @brief Clear array length (keeps capacity)
 * @param arr Pointer to array
 */
#define rg_array_clear(arr) \
	do                      \
	{                       \
		(arr)->len = 0;     \
	} while (0)

/**
 * @brief Free array (no-op for arena, clears fields)
 * @param arr Pointer to array
 */
#define rg_array_free(arr)   \
	do                       \
	{                        \
		(arr)->data = NULL;  \
		(arr)->len = 0;      \
		(arr)->cap = 0;      \
		(arr)->arena = NULL; \
	} while (0)

/**
 * @brief Get array length
 * @param arr Pointer to array
 * @return Element count
 */
#define rg_array_count(arr) ((arr)->len)

/**
 * @brief Get array capacity
 * @param arr Pointer to array
 * @return Capacity in elements
 */
#define rg_array_capacity(arr) ((arr)->cap)

/**
 * @brief Get array data pointer
 * @param arr Pointer to array
 * @return Data pointer
 */
#define rg_array_data(arr) ((arr)->data)

/**
 * @brief Get pointer to last element (array must be non-empty)
 * @param arr Pointer to array
 * @return Pointer to last element
 */
#define rg_array_back(arr) \
	(RG_CONTAINERS_ASSERT((arr)->len > 0), &((arr)->data[(arr)->len - 1]))

/**
 * @brief Reserve capacity for array
 * @param type Element type
 * @param arr Pointer to array
 * @param min_cap Minimum capacity
 */
#define rg_array_reserve(type, arr, min_cap)        \
	do                                              \
	{                                               \
		if ((arr)->cap < (min_cap))                 \
		{                                           \
			(arr)->data = (type*)rg_container_grow( \
			    (arr)->data,                        \
			    (arr)->len,                         \
			    &(arr)->cap,                        \
			    sizeof(type),                       \
			    RG_ALIGNOF(type),                   \
			    (min_cap),                          \
			    (arr)->arena);                      \
		}                                           \
	} while (0)

/**
 * @brief Resize array length (does not initialize new elements)
 * @param type Element type
 * @param arr Pointer to array
 * @param new_len New length
 */
#define rg_array_resize(type, arr, new_len)           \
	do                                                \
	{                                                 \
		size_t _rg_new_len = (size_t)(new_len);       \
		if (_rg_new_len > (arr)->cap)                 \
		{                                             \
			rg_array_reserve(type, arr, _rg_new_len); \
		}                                             \
		if (_rg_new_len <= (arr)->cap)                \
		{                                             \
			(arr)->len = _rg_new_len;                 \
		}                                             \
	} while (0)

/**
 * @brief Push value to array
 * @param type Element type
 * @param arr Pointer to array
 * @param value Value to push
 */
#define rg_array_push(type, arr, value)              \
	do                                               \
	{                                                \
		rg_array_reserve(type, arr, (arr)->len + 1); \
		(arr)->data[(arr)->len++] = (value);         \
	} while (0)

/**
 * @brief Push and return pointer to new element
 * @param type Element type
 * @param arr Pointer to array
 * @return Pointer to new element
 */
#define rg_array_push_ptr(type, arr) \
	((arr)->len == SIZE_MAX ? (RG_CONTAINERS_ASSERT(0), (type*)NULL) : ((arr)->len < (arr)->cap ? &((arr)->data[(arr)->len++]) : ((arr)->data = (type*)rg_container_grow((arr)->data, (arr)->len, &(arr)->cap, sizeof(type), RG_ALIGNOF(type), (arr)->len + 1, (arr)->arena), RG_CONTAINERS_ASSERT((arr)->len < (arr)->cap), (arr)->len < (arr)->cap ? &((arr)->data[(arr)->len++]) : (type*)NULL)))

/**
 * @brief Pop last element (array must be non-empty)
 * @param arr Pointer to array
 * @return Popped value
 */
#define rg_array_pop(arr) \
	(RG_CONTAINERS_ASSERT((arr)->len > 0), (arr)->data[--(arr)->len])

/**
 * @brief Insert value at index (preserves order)
 * @param type Element type
 * @param arr Pointer to array
 * @param index Index to insert at
 * @param value Value to insert
 */
#define rg_array_insert(type, arr, index, value)              \
	do                                                        \
	{                                                         \
		size_t _rg_index = (size_t)(index);                   \
		RG_CONTAINERS_ASSERT(_rg_index <= (arr)->len);        \
		RG_CONTAINERS_ASSERT((arr)->len < SIZE_MAX);          \
		if (_rg_index <= (arr)->len && (arr)->len < SIZE_MAX) \
		{                                                     \
			type _rg_value = (value);                         \
			size_t _rg_len = (arr)->len;                      \
			rg_array_reserve(type, arr, _rg_len + 1);         \
			if (_rg_len < (arr)->cap)                         \
			{                                                 \
				size_t _rg_move = _rg_len - _rg_index;        \
				if (_rg_move > 0)                             \
				{                                             \
					memmove(&(arr)->data[_rg_index + 1],      \
					        &(arr)->data[_rg_index],          \
					        _rg_move * sizeof(type));         \
				}                                             \
				(arr)->data[_rg_index] = _rg_value;           \
				(arr)->len = _rg_len + 1;                     \
			}                                                 \
		}                                                     \
	} while (0)

/**
 * @brief Remove element by swapping with last (does not preserve order)
 * @param arr Pointer to array
 * @param index Index to remove
 */
#define rg_array_remove_swap(arr, index)                    \
	do                                                      \
	{                                                       \
		size_t _rg_index = (size_t)(index);                 \
		RG_CONTAINERS_ASSERT(_rg_index < (arr)->len);       \
		size_t _rg_last = (arr)->len - 1;                   \
		if (_rg_index != _rg_last)                          \
		{                                                   \
			(arr)->data[_rg_index] = (arr)->data[_rg_last]; \
		}                                                   \
		(arr)->len = _rg_last;                              \
	} while (0)

/**
 * @brief Remove element and preserve order (shifts elements)
 * @param type Element type
 * @param arr Pointer to array
 * @param index Index to remove
 */
#define rg_array_remove_shift(type, arr, index)       \
	do                                                \
	{                                                 \
		size_t _rg_index = (size_t)(index);           \
		RG_CONTAINERS_ASSERT(_rg_index < (arr)->len); \
		size_t _rg_move = (arr)->len - _rg_index - 1; \
		if (_rg_move > 0)                             \
		{                                             \
			memmove(&(arr)->data[_rg_index],          \
			        &(arr)->data[_rg_index + 1],      \
			        _rg_move * sizeof(type));         \
		}                                             \
		(arr)->len--;                                 \
	} while (0)

// =============================================================================
// PUBLIC API - Small Vector
// =============================================================================

/**
 * @brief Initialize a small-vector with inline storage
 * @param vec Pointer to small-vector
 * @param arena_ptr Arena for spill allocations
 */
#define rg_smallvec_init(vec, arena_ptr)                                                    \
	do                                                                                      \
	{                                                                                       \
		size_t _rg_inline_cap = sizeof((vec)->inline_data) / sizeof((vec)->inline_data[0]); \
		(vec)->data = (vec)->inline_data;                                                   \
		(vec)->len = 0;                                                                     \
		(vec)->cap = _rg_inline_cap;                                                        \
		(vec)->arena = (arena_ptr);                                                         \
	} while (0)

/**
 * @brief Clear small-vector length (keeps capacity)
 * @param vec Pointer to small-vector
 */
#define rg_smallvec_clear(vec) \
	do                         \
	{                          \
		(vec)->len = 0;        \
	} while (0)

/**
 * @brief Free small-vector (no-op for arena, clears fields)
 * @param vec Pointer to small-vector
 */
#define rg_smallvec_free(vec) \
	do                        \
	{                         \
		(vec)->data = NULL;   \
		(vec)->len = 0;       \
		(vec)->cap = 0;       \
		(vec)->arena = NULL;  \
	} while (0)

/**
 * @brief Get small-vector length
 * @param vec Pointer to small-vector
 * @return Element count
 */
#define rg_smallvec_count(vec) ((vec)->len)

/**
 * @brief Get small-vector capacity
 * @param vec Pointer to small-vector
 * @return Capacity in elements
 */
#define rg_smallvec_capacity(vec) ((vec)->cap)

/**
 * @brief Get small-vector data pointer
 * @param vec Pointer to small-vector
 * @return Data pointer
 */
#define rg_smallvec_data(vec) ((vec)->data)

/**
 * @brief Get pointer to last element (vector must be non-empty)
 * @param vec Pointer to small-vector
 * @return Pointer to last element
 */
#define rg_smallvec_back(vec) \
	(RG_CONTAINERS_ASSERT((vec)->len > 0), &((vec)->data[(vec)->len - 1]))

/**
 * @brief Check whether the small-vector is using inline storage
 * @param vec Pointer to small-vector
 * @return 1 if inline, 0 if spilled
 */
#define rg_smallvec_is_inline(vec) ((vec)->data == (vec)->inline_data)

/**
 * @brief Reserve capacity for small-vector
 * @param type Element type
 * @param vec Pointer to small-vector
 * @param min_cap Minimum capacity
 */
#define rg_smallvec_reserve(type, vec, min_cap)     \
	do                                              \
	{                                               \
		if ((vec)->cap < (min_cap))                 \
		{                                           \
			(vec)->data = (type*)rg_container_grow( \
			    (vec)->data,                        \
			    (vec)->len,                         \
			    &(vec)->cap,                        \
			    sizeof(type),                       \
			    RG_ALIGNOF(type),                   \
			    (min_cap),                          \
			    (vec)->arena);                      \
		}                                           \
	} while (0)

/**
 * @brief Push value to small-vector
 * @param type Element type
 * @param vec Pointer to small-vector
 * @param value Value to push
 */
#define rg_smallvec_push(type, vec, value)              \
	do                                                  \
	{                                                   \
		rg_smallvec_reserve(type, vec, (vec)->len + 1); \
		(vec)->data[(vec)->len++] = (value);            \
	} while (0)

/**
 * @brief Push and return pointer to new element
 * @param type Element type
 * @param vec Pointer to small-vector
 * @return Pointer to new element
 */
#define rg_smallvec_push_ptr(type, vec) \
	((vec)->len == SIZE_MAX ? (RG_CONTAINERS_ASSERT(0), (type*)NULL) : ((vec)->len < (vec)->cap ? &((vec)->data[(vec)->len++]) : ((vec)->data = (type*)rg_container_grow((vec)->data, (vec)->len, &(vec)->cap, sizeof(type), RG_ALIGNOF(type), (vec)->len + 1, (vec)->arena), RG_CONTAINERS_ASSERT((vec)->len < (vec)->cap), (vec)->len < (vec)->cap ? &((vec)->data[(vec)->len++]) : (type*)NULL)))

/**
 * @brief Pop last element (vector must be non-empty)
 * @param vec Pointer to small-vector
 * @return Popped value
 */
#define rg_smallvec_pop(vec) \
	(RG_CONTAINERS_ASSERT((vec)->len > 0), (vec)->data[--(vec)->len])

/**
 * @brief Insert value at index (preserves order)
 * @param type Element type
 * @param vec Pointer to small-vector
 * @param index Index to insert at
 * @param value Value to insert
 */
#define rg_smallvec_insert(type, vec, index, value)           \
	do                                                        \
	{                                                         \
		size_t _rg_index = (size_t)(index);                   \
		RG_CONTAINERS_ASSERT(_rg_index <= (vec)->len);        \
		RG_CONTAINERS_ASSERT((vec)->len < SIZE_MAX);          \
		if (_rg_index <= (vec)->len && (vec)->len < SIZE_MAX) \
		{                                                     \
			type _rg_value = (value);                         \
			size_t _rg_len = (vec)->len;                      \
			rg_smallvec_reserve(type, vec, _rg_len + 1);      \
			if (_rg_len < (vec)->cap)                         \
			{                                                 \
				size_t _rg_move = _rg_len - _rg_index;        \
				if (_rg_move > 0)                             \
				{                                             \
					memmove(&(vec)->data[_rg_index + 1],      \
					        &(vec)->data[_rg_index],          \
					        _rg_move * sizeof(type));         \
				}                                             \
				(vec)->data[_rg_index] = _rg_value;           \
				(vec)->len = _rg_len + 1;                     \
			}                                                 \
		}                                                     \
	} while (0)

/**
 * @brief Remove element by swapping with last (does not preserve order)
 * @param vec Pointer to small-vector
 * @param index Index to remove
 */
#define rg_smallvec_remove_swap(vec, index)                 \
	do                                                      \
	{                                                       \
		size_t _rg_index = (size_t)(index);                 \
		RG_CONTAINERS_ASSERT(_rg_index < (vec)->len);       \
		size_t _rg_last = (vec)->len - 1;                   \
		if (_rg_index != _rg_last)                          \
		{                                                   \
			(vec)->data[_rg_index] = (vec)->data[_rg_last]; \
		}                                                   \
		(vec)->len = _rg_last;                              \
	} while (0)

/**
 * @brief Remove element and preserve order (shifts elements)
 * @param type Element type
 * @param vec Pointer to small-vector
 * @param index Index to remove
 */
#define rg_smallvec_remove_shift(type, vec, index)    \
	do                                                \
	{                                                 \
		size_t _rg_index = (size_t)(index);           \
		RG_CONTAINERS_ASSERT(_rg_index < (vec)->len); \
		size_t _rg_move = (vec)->len - _rg_index - 1; \
		if (_rg_move > 0)                             \
		{                                             \
			memmove(&(vec)->data[_rg_index],          \
			        &(vec)->data[_rg_index + 1],      \
			        _rg_move * sizeof(type));         \
		}                                             \
		(vec)->len--;                                 \
	} while (0)

// =============================================================================
// PUBLIC API - Ring Buffer
// =============================================================================

/**
 * @brief Initialize a ring buffer (capacity must be power of two)
 * @param type Element type
 * @param ring Pointer to ring buffer
 * @param arena_ptr Arena for allocations
 * @param capacity Number of elements (power of two)
 */
#define rg_ring_init(type, ring, arena_ptr, capacity)                      \
	do                                                                     \
	{                                                                      \
		size_t _rg_cap = (size_t)(capacity);                               \
		RgArena* _rg_arena = (arena_ptr);                                  \
		type* _rg_data = NULL;                                             \
		RG_CONTAINERS_ASSERT(_rg_arena != NULL);                           \
		RG_CONTAINERS_ASSERT(_rg_cap > 0);                                 \
		RG_CONTAINERS_ASSERT(RG_IS_POWER_OF_2(_rg_cap));                   \
		if (_rg_arena != NULL && _rg_cap > 0 && RG_IS_POWER_OF_2(_rg_cap)) \
		{                                                                  \
			_rg_data = (type*)rg_container_alloc_array(                    \
			    _rg_arena, _rg_cap, sizeof(type), RG_ALIGNOF(type));       \
			RG_CONTAINERS_ASSERT(_rg_data != NULL);                        \
		}                                                                  \
		(ring)->data = _rg_data;                                           \
		(ring)->head = 0;                                                  \
		(ring)->tail = 0;                                                  \
		(ring)->cap = _rg_data != NULL ? _rg_cap : 0;                      \
		(ring)->mask = _rg_data != NULL ? _rg_cap - 1 : 0;                 \
		(ring)->arena = _rg_arena;                                         \
	} while (0)

/**
 * @brief Clear ring buffer (keeps capacity)
 * @param ring Pointer to ring buffer
 */
#define rg_ring_clear(ring) \
	do                      \
	{                       \
		(ring)->head = 0;   \
		(ring)->tail = 0;   \
	} while (0)

/**
 * @brief Free ring buffer (no-op for arena, clears fields)
 * @param ring Pointer to ring buffer
 */
#define rg_ring_free(ring)    \
	do                        \
	{                         \
		(ring)->data = NULL;  \
		(ring)->head = 0;     \
		(ring)->tail = 0;     \
		(ring)->cap = 0;      \
		(ring)->mask = 0;     \
		(ring)->arena = NULL; \
	} while (0)

/**
 * @brief Get ring buffer count
 * @param ring Pointer to ring buffer
 * @return Element count
 */
#define rg_ring_count(ring) ((ring)->head - (ring)->tail)

/**
 * @brief Get ring buffer capacity
 * @param ring Pointer to ring buffer
 * @return Capacity in elements
 */
#define rg_ring_capacity(ring) ((ring)->cap)

/**
 * @brief Get ring buffer free space
 * @param ring Pointer to ring buffer
 * @return Remaining capacity
 */
#define rg_ring_space(ring) ((ring)->cap - rg_ring_count(ring))

/**
 * @brief Check if ring buffer is empty
 * @param ring Pointer to ring buffer
 * @return 1 if empty, 0 otherwise
 */
#define rg_ring_empty(ring) (rg_ring_count(ring) == 0)

/**
 * @brief Check if ring buffer is full
 * @param ring Pointer to ring buffer
 * @return 1 if full, 0 otherwise
 */
#define rg_ring_full(ring) (rg_ring_count(ring) == (ring)->cap)

/**
 * @brief Push value to ring buffer
 * @param type Element type
 * @param ring Pointer to ring buffer
 * @param value Value to push
 * @return 1 on success, 0 if full
 */
#define rg_ring_push(type, ring, value) \
	(rg_ring_full(ring) ? 0 : ((ring)->data[(ring)->head & (ring)->mask] = (value), (ring)->head++, 1))

/**
 * @brief Push value and overwrite oldest if full
 * @param type Element type
 * @param ring Pointer to ring buffer
 * @param value Value to push
 */
#define rg_ring_push_overwrite(type, ring, value)            \
	do                                                       \
	{                                                        \
		if (rg_ring_full(ring))                              \
		{                                                    \
			(ring)->tail++;                                  \
		}                                                    \
		(ring)->data[(ring)->head & (ring)->mask] = (value); \
		(ring)->head++;                                      \
	} while (0)

/**
 * @brief Pop value from ring buffer
 * @param type Element type
 * @param ring Pointer to ring buffer
 * @param out_ptr Output pointer for popped value
 * @return 1 on success, 0 if empty
 */
#define rg_ring_pop(type, ring, out_ptr) \
	(rg_ring_empty(ring) ? 0 : (*(out_ptr) = (ring)->data[(ring)->tail & (ring)->mask], (ring)->tail++, 1))

/**
 * @brief Peek next value from ring buffer (no pop)
 * @param type Element type
 * @param ring Pointer to ring buffer
 * @param out_ptr Output pointer for value
 * @return 1 on success, 0 if empty
 */
#define rg_ring_peek(type, ring, out_ptr) \
	(rg_ring_empty(ring) ? 0 : (*(out_ptr) = (ring)->data[(ring)->tail & (ring)->mask], 1))

// =============================================================================
// PUBLIC API - Sparse Set
// =============================================================================

/**
 * @brief Initialize sparse set
 * @param set Pointer to sparse set
 * @param arena_ptr Arena for allocations
 */
#define rg_sparse_set_init(set, arena_ptr) \
	do                                     \
	{                                      \
		(set)->dense = NULL;               \
		(set)->dense_count = 0;            \
		(set)->dense_cap = 0;              \
		(set)->sparse = NULL;              \
		(set)->sparse_cap = 0;             \
		(set)->arena = (arena_ptr);        \
	} while (0)

/**
 * @brief Clear sparse set (keeps capacity)
 * @param set Pointer to sparse set
 */
#define rg_sparse_set_clear(set) \
	do                           \
	{                            \
		(set)->dense_count = 0;  \
	} while (0)

/**
 * @brief Free sparse set (no-op for arena, clears fields)
 * @param set Pointer to sparse set
 */
#define rg_sparse_set_free(set) \
	do                          \
	{                           \
		(set)->dense = NULL;    \
		(set)->dense_count = 0; \
		(set)->dense_cap = 0;   \
		(set)->sparse = NULL;   \
		(set)->sparse_cap = 0;  \
		(set)->arena = NULL;    \
	} while (0)

/**
 * @brief Reserve dense capacity
 * @param id_type ID type
 * @param set Pointer to sparse set
 * @param min_cap Minimum dense capacity
 */
#define rg_sparse_set_reserve_dense(id_type, set, min_cap) \
	do                                                     \
	{                                                      \
		if ((set)->dense_cap < (min_cap))                  \
		{                                                  \
			(set)->dense = (id_type*)rg_container_grow(    \
			    (set)->dense,                              \
			    (set)->dense_count,                        \
			    &(set)->dense_cap,                         \
			    sizeof(id_type),                           \
			    RG_ALIGNOF(id_type),                       \
			    (min_cap),                                 \
			    (set)->arena);                             \
		}                                                  \
	} while (0)

/**
 * @brief Reserve sparse capacity (max ID + 1)
 * @param set Pointer to sparse set
 * @param max_id Maximum ID value to store
 */
#define rg_sparse_set_reserve_sparse(set, max_id)                                           \
	do                                                                                      \
	{                                                                                       \
		size_t _rg_max_id = (size_t)(max_id);                                               \
		if (_rg_max_id >= (set)->sparse_cap)                                                \
		{                                                                                   \
			RG_CONTAINERS_ASSERT(_rg_max_id < SIZE_MAX);                                    \
			if (_rg_max_id < SIZE_MAX)                                                      \
			{                                                                               \
				size_t _rg_min_cap = _rg_max_id + 1;                                        \
				size_t _rg_new_cap = rg_container_next_cap((set)->sparse_cap, _rg_min_cap); \
				size_t* _rg_new_sparse = rg_container_grow_sparse(                          \
				    (set)->sparse,                                                          \
				    (set)->sparse_cap,                                                      \
				    _rg_new_cap,                                                            \
				    (set)->arena);                                                          \
				RG_CONTAINERS_ASSERT(_rg_new_sparse != NULL);                               \
				if (_rg_new_sparse != NULL)                                                 \
				{                                                                           \
					(set)->sparse = _rg_new_sparse;                                         \
					(set)->sparse_cap = _rg_new_cap;                                        \
				}                                                                           \
			}                                                                               \
		}                                                                                   \
	} while (0)

/**
 * @brief Check if ID is in sparse set
 * @param set Pointer to sparse set
 * @param id ID to check
 * @return 1 if present, 0 otherwise
 */
#define rg_sparse_set_contains(set, id)                  \
	((size_t)(id) < (set)->sparse_cap &&                 \
	 (set)->sparse[(size_t)(id)] < (set)->dense_count && \
	 (set)->dense[(set)->sparse[(size_t)(id)]] == (id))

/**
 * @brief Get dense index for ID (or RG_SPARSE_INVALID)
 * @param set Pointer to sparse set
 * @param id ID to locate
 * @return Dense index or RG_SPARSE_INVALID
 */
#define rg_sparse_set_index_of(set, id) \
	(rg_sparse_set_contains(set, id) ? (set)->sparse[(size_t)(id)] : RG_SPARSE_INVALID)

/**
 * @brief Get dense array pointer
 * @param set Pointer to sparse set
 * @return Dense array pointer
 */
#define rg_sparse_set_dense(set) ((set)->dense)

/**
 * @brief Get dense count
 * @param set Pointer to sparse set
 * @return Dense count
 */
#define rg_sparse_set_dense_count(set) ((set)->dense_count)

/**
 * @brief Get dense capacity
 * @param set Pointer to sparse set
 * @return Dense capacity
 */
#define rg_sparse_set_dense_capacity(set) ((set)->dense_cap)

/**
 * @brief Get sparse capacity (max ID + 1)
 * @param set Pointer to sparse set
 * @return Sparse capacity
 */
#define rg_sparse_set_sparse_capacity(set) ((set)->sparse_cap)

/**
 * @brief Insert ID into sparse set (no-op if already present)
 * @param id_type ID type
 * @param set Pointer to sparse set
 * @param id ID to insert
 */
#define rg_sparse_set_insert(id_type, set, id)                                 \
	do                                                                         \
	{                                                                          \
		size_t _rg_id = (size_t)(id);                                          \
		rg_sparse_set_reserve_sparse(set, _rg_id);                             \
		if (!rg_sparse_set_contains(set, id))                                  \
		{                                                                      \
			rg_sparse_set_reserve_dense(id_type, set, (set)->dense_count + 1); \
			(set)->dense[(set)->dense_count] = (id);                           \
			(set)->sparse[_rg_id] = (set)->dense_count;                        \
			(set)->dense_count++;                                              \
		}                                                                      \
	} while (0)

/**
 * @brief Remove ID from sparse set (no-op if missing)
 * @param set Pointer to sparse set
 * @param id ID to remove
 */
#define rg_sparse_set_remove(set, id)                                           \
	do                                                                          \
	{                                                                           \
		size_t _rg_id = (size_t)(id);                                           \
		if (_rg_id < (set)->sparse_cap)                                         \
		{                                                                       \
			size_t _rg_index = (set)->sparse[_rg_id];                           \
			if (_rg_index < (set)->dense_count &&                               \
			    (size_t)(set)->dense[_rg_index] == _rg_id)                      \
			{                                                                   \
				size_t _rg_last = (set)->dense_count - 1;                       \
				if (_rg_index != _rg_last)                                      \
				{                                                               \
					(set)->dense[_rg_index] = (set)->dense[_rg_last];           \
					(set)->sparse[(size_t)(set)->dense[_rg_index]] = _rg_index; \
				}                                                               \
				(set)->dense_count = _rg_last;                                  \
				(set)->sparse[_rg_id] = RG_SPARSE_INVALID;                      \
			}                                                                   \
		}                                                                       \
	} while (0)

// =============================================================================
// IMPLEMENTATION
// =============================================================================

#if RG_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4505)
#define RG_CONTAINERS_COLD static __declspec(noinline)
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
#define RG_CONTAINERS_COLD static __attribute__((noinline, unused))
#else
#define RG_CONTAINERS_COLD static
#endif

RGINLINE size_t rg_container_next_cap(size_t cap, size_t min_cap)
{
	if (cap >= min_cap)
		return cap;

	size_t new_cap = (size_t)RG_CONTAINERS_MIN_CAP;
	if (cap > 0)
		new_cap = cap <= SIZE_MAX / 2 ? cap * 2 : SIZE_MAX;
	if (new_cap < min_cap)
		new_cap = min_cap;
	return new_cap;
}

RGINLINE void* rg_container_alloc(RgArena* arena, size_t size, size_t alignment)
{
	RG_CONTAINERS_ASSERT(arena != NULL && arena->memory != NULL);
	RG_CONTAINERS_ASSERT(size > 0);
	RG_CONTAINERS_ASSERT(alignment > 0 && RG_IS_POWER_OF_2(alignment));
	if (arena == NULL || arena->memory == NULL || size == 0 ||
	    alignment == 0 || !RG_IS_POWER_OF_2(alignment))
		return NULL;

	void* ptr = rg_arena_alloc_aligned(arena, size, alignment);
	RG_CONTAINERS_ASSERT(ptr != NULL);
	return ptr;
}

RGINLINE void* rg_container_alloc_array(RgArena* arena,
                                        size_t count,
                                        size_t elem_size,
                                        size_t elem_align)
{
	RG_CONTAINERS_ASSERT(elem_size > 0);
	RG_CONTAINERS_ASSERT(count > 0 && count <= SIZE_MAX / elem_size);
	if (elem_size == 0 || count == 0 || count > SIZE_MAX / elem_size)
		return NULL;
	return rg_container_alloc(arena, count * elem_size, elem_align);
}

RG_CONTAINERS_COLD void* rg_container_grow(void* data,
                                           size_t len,
                                           size_t* cap,
                                           size_t elem_size,
                                           size_t elem_align,
                                           size_t min_cap,
                                           RgArena* arena)
{
	RG_CONTAINERS_ASSERT(cap != NULL);
	RG_CONTAINERS_ASSERT(arena != NULL);
	RG_CONTAINERS_ASSERT(elem_size > 0);
	RG_CONTAINERS_ASSERT(elem_align > 0 && RG_IS_POWER_OF_2(elem_align));
	if (cap == NULL || arena == NULL || elem_size == 0 || elem_align == 0 ||
	    !RG_IS_POWER_OF_2(elem_align))
		return data;
	if (min_cap <= *cap)
		return data;

	size_t new_cap = rg_container_next_cap(*cap, min_cap);
	RG_CONTAINERS_ASSERT(len <= *cap);
	RG_CONTAINERS_ASSERT(new_cap >= min_cap);
	RG_CONTAINERS_ASSERT(new_cap <= SIZE_MAX / elem_size);
	if (len > *cap || new_cap < min_cap || new_cap > SIZE_MAX / elem_size ||
	    len > SIZE_MAX / elem_size)
		return data;

	size_t bytes = new_cap * elem_size;
	void* new_data = rg_container_alloc(arena, bytes, elem_align);
	if (new_data == NULL)
		return data;

	if (data != NULL && len > 0)
		memcpy(new_data, data, len * elem_size);

	*cap = new_cap;
	return new_data;
}

RG_CONTAINERS_COLD size_t* rg_container_grow_sparse(const size_t* sparse,
                                                    size_t old_cap,
                                                    size_t new_cap,
                                                    RgArena* arena)
{
	RG_CONTAINERS_ASSERT(new_cap >= old_cap);
	RG_CONTAINERS_ASSERT(new_cap <= SIZE_MAX / sizeof(size_t));
	if (new_cap <= old_cap || new_cap > SIZE_MAX / sizeof(size_t))
		return NULL;

	size_t* new_sparse = (size_t*)rg_container_alloc_array(
	    arena, new_cap, sizeof(size_t), RG_ALIGNOF(size_t));
	if (new_sparse == NULL)
		return NULL;

	if (sparse != NULL && old_cap > 0)
		memcpy(new_sparse, sparse, old_cap * sizeof(size_t));

	for (size_t i = old_cap; i < new_cap; i++)
		new_sparse[i] = RG_SPARSE_INVALID;

	return new_sparse;
}

#undef RG_CONTAINERS_COLD

#if RG_COMPILER_MSVC
#pragma warning(pop)
#endif

#endif // RG_CONTAINERS_H
