// rg_hash - Hash functions and typed robin-hood hash tables for C
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header hashes plus arena-backed, macro-generated hash maps and sets.
//
// USAGE:
//   #include "rg_hash.h"
//
//   RG_HASH_MAP_DEFINE(uint32_t, float, U32FloatMap, rg_hash_u32, rg_hash_eq_u32);
//   U32FloatMap map;
//   rg_hash_map_init(U32FloatMap, &map, &arena);
//   rg_hash_map_put(U32FloatMap, &map, 42, 3.14f, NULL);
//
// OPTIONS:
//   #define RG_HASH_ASSERT(x)       - Custom assert macro
//   #define RG_HASH_MIN_CAP         - Minimum table capacity (default: 8)
//   #define RG_HASH_MAX_LOAD_NUM    - Load factor numerator (default: 7)
//   #define RG_HASH_MAX_LOAD_DEN    - Load factor denominator (default: 10)
//   #define RG_HASH_SEED            - Default hash seed (default: 0)
//
// NOTES:
//   - Hash tables are arena-backed; growth leaves old storage in the arena.
//   - All slots use hash == 0 as empty; hashes are forced non-zero.
//   - Maps and sets are not safe for concurrent mutation.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_HASH_H
#define RG_HASH_H

#include "rg_mem.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

// =============================================================================
// CONFIGURATION
// =============================================================================

#ifndef RG_HASH_ASSERT
#include <assert.h>
#define RG_HASH_ASSERT(condition) assert(condition)
#endif

#ifndef RG_HASH_MIN_CAP
#define RG_HASH_MIN_CAP 8
#endif

#ifndef RG_HASH_MAX_LOAD_NUM
#define RG_HASH_MAX_LOAD_NUM 7
#endif

#ifndef RG_HASH_MAX_LOAD_DEN
#define RG_HASH_MAX_LOAD_DEN 10
#endif

#ifndef RG_HASH_SEED
#define RG_HASH_SEED 0
#endif

#ifndef RG_HASH_INVALID
#define RG_HASH_INVALID SIZE_MAX
#endif

#if RG_HASH_MIN_CAP == 0
#error RG_HASH_MIN_CAP must be greater than zero
#endif

#if RG_HASH_MAX_LOAD_NUM == 0 || RG_HASH_MAX_LOAD_DEN == 0 || \
    RG_HASH_MAX_LOAD_NUM >= RG_HASH_MAX_LOAD_DEN
#error RG_HASH_MAX_LOAD_NUM / RG_HASH_MAX_LOAD_DEN must be between zero and one
#endif

// =============================================================================
// PUBLIC API - Hash Functions
// =============================================================================

/**
 * @brief Hash a byte buffer (xxhash64)
 * @param data Pointer to bytes
 * @param len Number of bytes
 * @param seed Hash seed
 * @return 64-bit hash
 */
RGINLINE uint64_t rg_hash_bytes(const void* data, size_t len, uint64_t seed);

/**
 * @brief Hash a null-terminated string using RG_HASH_SEED
 * @param str String to hash
 * @return 64-bit hash
 */
RGINLINE uint64_t rg_hash_str(const char* str);

/**
 * @brief Hash a 32-bit integer using a fast mix
 * @param value Input value
 * @return 64-bit hash
 */
RGINLINE uint64_t rg_hash_u32(uint32_t value);

/**
 * @brief Hash a 64-bit integer using a fast mix
 * @param value Input value
 * @return 64-bit hash
 */
RGINLINE uint64_t rg_hash_u64(uint64_t value);

/**
 * @brief Hash a pointer value
 * @param ptr Pointer to hash
 * @return 64-bit hash
 */
RGINLINE uint64_t rg_hash_ptr(const void* ptr);

/**
 * @brief Equality helpers for common key types
 */
RGINLINE int rg_hash_eq_u32(uint32_t a, uint32_t b);
RGINLINE int rg_hash_eq_u64(uint64_t a, uint64_t b);
RGINLINE int rg_hash_eq_ptr(const void* a, const void* b);
RGINLINE int rg_hash_eq_str(const char* a, const char* b);

// =============================================================================
// PUBLIC API - Hash Tables
// =============================================================================

/**
 * @brief Define a typed robin-hood hash map
 * @param key_type Key type
 * @param value_type Value type
 * @param name Map type name
 * @param hash_fn Hash function (uint64_t fn(key_type))
 * @param eq_fn Equality function (int fn(key_type a, key_type b))
 */
#define RG_HASH_MAP_DEFINE(key_type, value_type, name, hash_fn, eq_fn) \
	typedef key_type name##_Key; \
	typedef value_type name##_Value; \
	typedef struct name##_Entry \
	{ \
		uint64_t hash; \
		key_type key; \
		value_type value; \
	} name##_Entry; \
	typedef struct name \
	{ \
		name##_Entry* entries; \
		size_t cap; \
		size_t count; \
		RgArena* arena; \
	} name; \
	RGINLINE uint64_t rg_hash_map_hash_##name(name##_Key key) \
	{ \
		return (hash_fn)(key); \
	} \
	RGINLINE int rg_hash_map_eq_##name(name##_Key a, name##_Key b) \
	{ \
		return (eq_fn)(a, b); \
	} \
	RGINLINE void rg_hash_map_init_##name(name* map, RgArena* arena) \
	{ \
		map->entries = NULL; \
		map->cap = 0; \
		map->count = 0; \
		map->arena = arena; \
	} \
	RGINLINE void rg_hash_map_clear_##name(name* map) \
	{ \
		if (map->entries != NULL) \
		{ \
			memset(map->entries, 0, map->cap * sizeof(name##_Entry)); \
		} \
		map->count = 0; \
	} \
	RGINLINE void rg_hash_map_free_##name(name* map) \
	{ \
		map->entries = NULL; \
		map->cap = 0; \
		map->count = 0; \
		map->arena = NULL; \
	} \
	RGINLINE size_t rg_hash_map_find_index_##name(const name* map, name##_Key key) \
	{ \
		if (map->cap == 0) \
		{ \
			return RG_HASH_INVALID; \
		} \
		uint64_t hash = rg_hash_prepare(rg_hash_map_hash_##name(key)); \
		size_t mask = map->cap - 1; \
		size_t idx = (size_t)(hash & mask); \
		size_t dist = 0; \
		for (;;) \
		{ \
			const name##_Entry* entry = &map->entries[idx]; \
			if (entry->hash == 0) \
			{ \
				return RG_HASH_INVALID; \
			} \
			size_t entry_dist = rg_hash_probe_distance(entry->hash, idx, mask); \
			if (entry_dist < dist) \
			{ \
				return RG_HASH_INVALID; \
			} \
			if (entry->hash == hash && rg_hash_map_eq_##name(entry->key, key)) \
			{ \
				return idx; \
			} \
			idx = (idx + 1) & mask; \
			dist++; \
		} \
	} \
	RGINLINE name##_Value* rg_hash_map_get_ptr_##name(name* map, name##_Key key) \
	{ \
		size_t idx = rg_hash_map_find_index_##name(map, key); \
		if (idx == RG_HASH_INVALID) \
		{ \
			return NULL; \
		} \
		return &map->entries[idx].value; \
	} \
	RGINLINE const name##_Value* rg_hash_map_get_ptr_const_##name(const name* map, name##_Key key) \
	{ \
		size_t idx = rg_hash_map_find_index_##name(map, key); \
		if (idx == RG_HASH_INVALID) \
		{ \
			return NULL; \
		} \
		return &map->entries[idx].value; \
	} \
	RGINLINE void rg_hash_map_insert_entry_##name(name* map, name##_Entry entry) \
	{ \
		size_t mask = map->cap - 1; \
		size_t idx = (size_t)(entry.hash & mask); \
		size_t dist = 0; \
		for (;;) \
		{ \
			name##_Entry* slot = &map->entries[idx]; \
			if (slot->hash == 0) \
			{ \
				*slot = entry; \
				map->count++; \
				return; \
			} \
			size_t slot_dist = rg_hash_probe_distance(slot->hash, idx, mask); \
			if (slot_dist < dist) \
			{ \
				name##_Entry tmp = *slot; \
				*slot = entry; \
				entry = tmp; \
				dist = slot_dist; \
			} \
			idx = (idx + 1) & mask; \
			dist++; \
		} \
	} \
	RGINLINE int rg_hash_map_rehash_##name(name* map, size_t new_cap) \
	{ \
		name##_Entry* old_entries = map->entries; \
		size_t old_cap = map->cap; \
		name##_Entry* new_entries = (name##_Entry*)rg_arena_alloc_array( \
		    map->arena, sizeof(name##_Entry), new_cap, RG_ALIGNOF(name##_Entry)); \
		RG_HASH_ASSERT(new_entries != NULL); \
		if (new_entries == NULL) \
		{ \
			return 0; \
		} \
		map->entries = new_entries; \
		map->cap = new_cap; \
		map->count = 0; \
		memset(map->entries, 0, new_cap * sizeof(name##_Entry)); \
		if (old_entries != NULL) \
		{ \
			for (size_t i = 0; i < old_cap; i++) \
			{ \
				if (old_entries[i].hash != 0) \
				{ \
					rg_hash_map_insert_entry_##name(map, old_entries[i]); \
				} \
			} \
		} \
		return 1; \
	} \
	RGINLINE int rg_hash_map_reserve_##name(name* map, size_t min_count) \
	{ \
		RG_HASH_ASSERT(map->arena != NULL); \
		if (map->arena == NULL) \
		{ \
			return 0; \
		} \
		size_t new_cap = rg_hash_capacity_for(min_count); \
		if (new_cap == 0) \
		{ \
			return min_count == 0; \
		} \
		if (new_cap <= map->cap) \
		{ \
			return 1; \
		} \
		return rg_hash_map_rehash_##name(map, new_cap); \
	} \
	RGINLINE int rg_hash_map_put_##name(name* map, name##_Key key, name##_Value value) \
	{ \
		RG_HASH_ASSERT(map->arena != NULL); \
		if (map->cap == 0) \
		{ \
			if (!rg_hash_map_reserve_##name(map, 1)) \
			{ \
				return -1; \
			} \
		} \
		size_t idx = rg_hash_map_find_index_##name(map, key); \
		if (idx != RG_HASH_INVALID) \
		{ \
			map->entries[idx].value = value; \
			return 0; \
		} \
		if (rg_hash_should_grow(map->count, map->cap)) \
		{ \
			if (map->count == SIZE_MAX || \
			    !rg_hash_map_reserve_##name(map, map->count + 1)) \
			{ \
				return -1; \
			} \
		} \
		name##_Entry entry; \
		entry.hash = rg_hash_prepare(rg_hash_map_hash_##name(key)); \
		entry.key = key; \
		entry.value = value; \
		rg_hash_map_insert_entry_##name(map, entry); \
		return 1; \
	} \
	RGINLINE int rg_hash_map_remove_##name(name* map, name##_Key key) \
	{ \
		size_t idx = rg_hash_map_find_index_##name(map, key); \
		if (idx == RG_HASH_INVALID) \
		{ \
			return 0; \
		} \
		size_t mask = map->cap - 1; \
		size_t next = (idx + 1) & mask; \
		while (map->entries[next].hash != 0 && \
		    rg_hash_probe_distance(map->entries[next].hash, next, mask) > 0) \
		{ \
			map->entries[idx] = map->entries[next]; \
			idx = next; \
			next = (next + 1) & mask; \
		} \
		map->entries[idx].hash = 0; \
		map->count--; \
		return 1; \
	}

/**
 * @brief Define a typed robin-hood hash set
 * @param key_type Key type
 * @param name Set type name
 * @param hash_fn Hash function (uint64_t fn(key_type))
 * @param eq_fn Equality function (int fn(key_type a, key_type b))
 */
#define RG_HASH_SET_DEFINE(key_type, name, hash_fn, eq_fn) \
	typedef key_type name##_Key; \
	typedef struct name##_Entry \
	{ \
		uint64_t hash; \
		key_type key; \
	} name##_Entry; \
	typedef struct name \
	{ \
		name##_Entry* entries; \
		size_t cap; \
		size_t count; \
		RgArena* arena; \
	} name; \
	RGINLINE uint64_t rg_hash_set_hash_##name(name##_Key key) \
	{ \
		return (hash_fn)(key); \
	} \
	RGINLINE int rg_hash_set_eq_##name(name##_Key a, name##_Key b) \
	{ \
		return (eq_fn)(a, b); \
	} \
	RGINLINE void rg_hash_set_init_##name(name* set, RgArena* arena) \
	{ \
		set->entries = NULL; \
		set->cap = 0; \
		set->count = 0; \
		set->arena = arena; \
	} \
	RGINLINE void rg_hash_set_clear_##name(name* set) \
	{ \
		if (set->entries != NULL) \
		{ \
			memset(set->entries, 0, set->cap * sizeof(name##_Entry)); \
		} \
		set->count = 0; \
	} \
	RGINLINE void rg_hash_set_free_##name(name* set) \
	{ \
		set->entries = NULL; \
		set->cap = 0; \
		set->count = 0; \
		set->arena = NULL; \
	} \
	RGINLINE size_t rg_hash_set_find_index_##name(const name* set, name##_Key key) \
	{ \
		if (set->cap == 0) \
		{ \
			return RG_HASH_INVALID; \
		} \
		uint64_t hash = rg_hash_prepare(rg_hash_set_hash_##name(key)); \
		size_t mask = set->cap - 1; \
		size_t idx = (size_t)(hash & mask); \
		size_t dist = 0; \
		for (;;) \
		{ \
			const name##_Entry* entry = &set->entries[idx]; \
			if (entry->hash == 0) \
			{ \
				return RG_HASH_INVALID; \
			} \
			size_t entry_dist = rg_hash_probe_distance(entry->hash, idx, mask); \
			if (entry_dist < dist) \
			{ \
				return RG_HASH_INVALID; \
			} \
			if (entry->hash == hash && rg_hash_set_eq_##name(entry->key, key)) \
			{ \
				return idx; \
			} \
			idx = (idx + 1) & mask; \
			dist++; \
		} \
	} \
	RGINLINE int rg_hash_set_contains_##name(const name* set, name##_Key key) \
	{ \
		return rg_hash_set_find_index_##name(set, key) != RG_HASH_INVALID; \
	} \
	RGINLINE void rg_hash_set_insert_entry_##name(name* set, name##_Entry entry) \
	{ \
		size_t mask = set->cap - 1; \
		size_t idx = (size_t)(entry.hash & mask); \
		size_t dist = 0; \
		for (;;) \
		{ \
			name##_Entry* slot = &set->entries[idx]; \
			if (slot->hash == 0) \
			{ \
				*slot = entry; \
				set->count++; \
				return; \
			} \
			size_t slot_dist = rg_hash_probe_distance(slot->hash, idx, mask); \
			if (slot_dist < dist) \
			{ \
				name##_Entry tmp = *slot; \
				*slot = entry; \
				entry = tmp; \
				dist = slot_dist; \
			} \
			idx = (idx + 1) & mask; \
			dist++; \
		} \
	} \
	RGINLINE int rg_hash_set_rehash_##name(name* set, size_t new_cap) \
	{ \
		name##_Entry* old_entries = set->entries; \
		size_t old_cap = set->cap; \
		name##_Entry* new_entries = (name##_Entry*)rg_arena_alloc_array( \
		    set->arena, sizeof(name##_Entry), new_cap, RG_ALIGNOF(name##_Entry)); \
		RG_HASH_ASSERT(new_entries != NULL); \
		if (new_entries == NULL) \
		{ \
			return 0; \
		} \
		set->entries = new_entries; \
		set->cap = new_cap; \
		set->count = 0; \
		memset(set->entries, 0, new_cap * sizeof(name##_Entry)); \
		if (old_entries != NULL) \
		{ \
			for (size_t i = 0; i < old_cap; i++) \
			{ \
				if (old_entries[i].hash != 0) \
				{ \
					rg_hash_set_insert_entry_##name(set, old_entries[i]); \
				} \
			} \
		} \
		return 1; \
	} \
	RGINLINE int rg_hash_set_reserve_##name(name* set, size_t min_count) \
	{ \
		RG_HASH_ASSERT(set->arena != NULL); \
		if (set->arena == NULL) \
		{ \
			return 0; \
		} \
		size_t new_cap = rg_hash_capacity_for(min_count); \
		if (new_cap == 0) \
		{ \
			return min_count == 0; \
		} \
		if (new_cap <= set->cap) \
		{ \
			return 1; \
		} \
		return rg_hash_set_rehash_##name(set, new_cap); \
	} \
	RGINLINE int rg_hash_set_insert_##name(name* set, name##_Key key) \
	{ \
		RG_HASH_ASSERT(set->arena != NULL); \
		if (set->cap == 0) \
		{ \
			if (!rg_hash_set_reserve_##name(set, 1)) \
			{ \
				return -1; \
			} \
		} \
		if (rg_hash_set_contains_##name(set, key)) \
		{ \
			return 0; \
		} \
		if (rg_hash_should_grow(set->count, set->cap)) \
		{ \
			if (set->count == SIZE_MAX || \
			    !rg_hash_set_reserve_##name(set, set->count + 1)) \
			{ \
				return -1; \
			} \
		} \
		name##_Entry entry; \
		entry.hash = rg_hash_prepare(rg_hash_set_hash_##name(key)); \
		entry.key = key; \
		rg_hash_set_insert_entry_##name(set, entry); \
		return 1; \
	} \
	RGINLINE int rg_hash_set_remove_##name(name* set, name##_Key key) \
	{ \
		size_t idx = rg_hash_set_find_index_##name(set, key); \
		if (idx == RG_HASH_INVALID) \
		{ \
			return 0; \
		} \
		size_t mask = set->cap - 1; \
		size_t next = (idx + 1) & mask; \
		while (set->entries[next].hash != 0 && \
		    rg_hash_probe_distance(set->entries[next].hash, next, mask) > 0) \
		{ \
			set->entries[idx] = set->entries[next]; \
			idx = next; \
			next = (next + 1) & mask; \
		} \
		set->entries[idx].hash = 0; \
		set->count--; \
		return 1; \
	}

// Convenience wrappers (use with RG_HASH_MAP_DEFINE / RG_HASH_SET_DEFINE types)
#define rg_hash_map_init(name, map, arena) rg_hash_map_init_##name((map), (arena))
#define rg_hash_map_reserve(name, map, min_count) rg_hash_map_reserve_##name((map), (min_count))
#define rg_hash_map_clear(name, map) rg_hash_map_clear_##name((map))
#define rg_hash_map_free(name, map) rg_hash_map_free_##name((map))
#define rg_hash_map_try_put(name, map, key, value) rg_hash_map_put_##name((map), (key), (value))
#define rg_hash_map_put(name, map, key, value, out_is_new) \
	do \
	{ \
		int _rg_put_result = rg_hash_map_try_put(name, (map), (key), (value)); \
		int* _rg_out = (int*)(out_is_new); \
		if (_rg_out != NULL) \
		{ \
			*_rg_out = _rg_put_result > 0; \
		} \
	} while (0)
#define rg_hash_map_remove(name, map, key, out_removed) \
	do \
	{ \
		int _rg_removed = rg_hash_map_remove_##name((map), (key)); \
		int* _rg_out = (int*)(out_removed); \
		if (_rg_out != NULL) \
		{ \
			*_rg_out = _rg_removed; \
		} \
	} while (0)
#define rg_hash_map_get_ptr(name, map, key) rg_hash_map_get_ptr_##name((map), (key))
#define rg_hash_map_get_ptr_const(name, map, key) rg_hash_map_get_ptr_const_##name((map), (key))
#define rg_hash_map_contains(name, map, key) (rg_hash_map_get_ptr_##name((map), (key)) != NULL)
#define rg_hash_map_count(map) ((map)->count)
#define rg_hash_map_capacity(map) ((map)->cap)
#define rg_hash_map_is_empty(map) ((map)->count == 0)

#define rg_hash_set_init(name, set, arena) rg_hash_set_init_##name((set), (arena))
#define rg_hash_set_reserve(name, set, min_count) rg_hash_set_reserve_##name((set), (min_count))
#define rg_hash_set_clear(name, set) rg_hash_set_clear_##name((set))
#define rg_hash_set_free(name, set) rg_hash_set_free_##name((set))
#define rg_hash_set_try_insert(name, set, key) rg_hash_set_insert_##name((set), (key))
#define rg_hash_set_insert(name, set, key, out_is_new) \
	do \
	{ \
		int _rg_insert_result = rg_hash_set_try_insert(name, (set), (key)); \
		int* _rg_out = (int*)(out_is_new); \
		if (_rg_out != NULL) \
		{ \
			*_rg_out = _rg_insert_result > 0; \
		} \
	} while (0)
#define rg_hash_set_remove(name, set, key, out_removed) \
	do \
	{ \
		int _rg_removed = rg_hash_set_remove_##name((set), (key)); \
		int* _rg_out = (int*)(out_removed); \
		if (_rg_out != NULL) \
		{ \
			*_rg_out = _rg_removed; \
		} \
	} while (0)
#define rg_hash_set_contains(name, set, key) rg_hash_set_contains_##name((set), (key))
#define rg_hash_set_count(set) ((set)->count)
#define rg_hash_set_capacity(set) ((set)->cap)
#define rg_hash_set_is_empty(set) ((set)->count == 0)

// =============================================================================
// IMPLEMENTATION
// =============================================================================

#define RG_HASH_PRIME1 11400714785074694791ULL
#define RG_HASH_PRIME2 14029467366897019727ULL
#define RG_HASH_PRIME3  1609587929392839161ULL
#define RG_HASH_PRIME4  9650029242287828579ULL
#define RG_HASH_PRIME5  2870177450012600261ULL

RGINLINE uint64_t rg_hash_rotl64(uint64_t value, int r)
{
	return (value << r) | (value >> (64 - r));
}

RGINLINE int rg_hash_is_big_endian(void)
{
#if defined(_WIN32)
	return 0;
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && \
    defined(__ORDER_LITTLE_ENDIAN__)
	return __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__;
#else
	const uint16_t value = 1;
	return ((const uint8_t*)&value)[0] == 0;
#endif
}

RGINLINE uint64_t rg_hash_bswap64(uint64_t value)
{
	return ((value & UINT64_C(0x00000000000000ff)) << 56) |
	       ((value & UINT64_C(0x000000000000ff00)) << 40) |
	       ((value & UINT64_C(0x0000000000ff0000)) << 24) |
	       ((value & UINT64_C(0x00000000ff000000)) << 8) |
	       ((value & UINT64_C(0x000000ff00000000)) >> 8) |
	       ((value & UINT64_C(0x0000ff0000000000)) >> 24) |
	       ((value & UINT64_C(0x00ff000000000000)) >> 40) |
	       ((value & UINT64_C(0xff00000000000000)) >> 56);
}

RGINLINE uint32_t rg_hash_bswap32(uint32_t value)
{
	return ((value & UINT32_C(0x000000ff)) << 24) |
	       ((value & UINT32_C(0x0000ff00)) << 8) |
	       ((value & UINT32_C(0x00ff0000)) >> 8) |
	       ((value & UINT32_C(0xff000000)) >> 24);
}

RGINLINE uint64_t rg_hash_read64(const void* ptr)
{
	uint64_t value;
	memcpy(&value, ptr, sizeof(value));
	return rg_hash_is_big_endian() ? rg_hash_bswap64(value) : value;
}

RGINLINE uint32_t rg_hash_read32(const void* ptr)
{
	uint32_t value;
	memcpy(&value, ptr, sizeof(value));
	return rg_hash_is_big_endian() ? rg_hash_bswap32(value) : value;
}

RGINLINE uint64_t rg_hash_round(uint64_t acc, uint64_t input)
{
	acc += input * RG_HASH_PRIME2;
	acc = rg_hash_rotl64(acc, 31);
	acc *= RG_HASH_PRIME1;
	return acc;
}

RGINLINE uint64_t rg_hash_merge_round(uint64_t acc, uint64_t val)
{
	val = rg_hash_round(0, val);
	acc ^= val;
	acc = acc * RG_HASH_PRIME1 + RG_HASH_PRIME4;
	return acc;
}

RGINLINE uint64_t rg_hash_avalanche(uint64_t hash)
{
	hash ^= hash >> 33;
	hash *= RG_HASH_PRIME2;
	hash ^= hash >> 29;
	hash *= RG_HASH_PRIME3;
	hash ^= hash >> 32;
	return hash;
}

RGINLINE uint64_t rg_hash_bytes(const void* data, size_t len, uint64_t seed)
{
	if (len == 0)
	{
		return rg_hash_avalanche(seed + RG_HASH_PRIME5);
	}
	RG_HASH_ASSERT(data != NULL);
	if (data == NULL)
	{
		return 0;
	}

	const uint8_t* ptr = (const uint8_t*)data;
	const uint8_t* end = ptr + len;
	uint64_t hash;

	if (len >= 32)
	{
		const uint8_t* limit = end - 32;
		uint64_t v1 = seed + RG_HASH_PRIME1 + RG_HASH_PRIME2;
		uint64_t v2 = seed + RG_HASH_PRIME2;
		uint64_t v3 = seed + 0;
		uint64_t v4 = seed - RG_HASH_PRIME1;

		do
		{
			v1 = rg_hash_round(v1, rg_hash_read64(ptr));
			ptr += 8;
			v2 = rg_hash_round(v2, rg_hash_read64(ptr));
			ptr += 8;
			v3 = rg_hash_round(v3, rg_hash_read64(ptr));
			ptr += 8;
			v4 = rg_hash_round(v4, rg_hash_read64(ptr));
			ptr += 8;
		} while (ptr <= limit);

		hash = rg_hash_rotl64(v1, 1) +
		       rg_hash_rotl64(v2, 7) +
		       rg_hash_rotl64(v3, 12) +
		       rg_hash_rotl64(v4, 18);

		hash = rg_hash_merge_round(hash, v1);
		hash = rg_hash_merge_round(hash, v2);
		hash = rg_hash_merge_round(hash, v3);
		hash = rg_hash_merge_round(hash, v4);
	}
	else
	{
		hash = seed + RG_HASH_PRIME5;
	}

	hash += len;

	while ((size_t)(end - ptr) >= 8)
	{
		uint64_t k1 = rg_hash_round(0, rg_hash_read64(ptr));
		hash ^= k1;
		hash = rg_hash_rotl64(hash, 27) * RG_HASH_PRIME1 + RG_HASH_PRIME4;
		ptr += 8;
	}

	if ((size_t)(end - ptr) >= 4)
	{
		hash ^= (uint64_t)rg_hash_read32(ptr) * RG_HASH_PRIME1;
		hash = rg_hash_rotl64(hash, 23) * RG_HASH_PRIME2 + RG_HASH_PRIME3;
		ptr += 4;
	}

	while (ptr < end)
	{
		hash ^= (*ptr) * RG_HASH_PRIME5;
		hash = rg_hash_rotl64(hash, 11) * RG_HASH_PRIME1;
		ptr++;
	}

	return rg_hash_avalanche(hash);
}

RGINLINE uint64_t rg_hash_str(const char* str)
{
	return rg_hash_bytes(str, strlen(str), RG_HASH_SEED);
}

RGINLINE uint64_t rg_hash_u32(uint32_t value)
{
	return rg_hash_avalanche(((uint64_t)value) + RG_HASH_SEED);
}

RGINLINE uint64_t rg_hash_u64(uint64_t value)
{
	return rg_hash_avalanche(value + RG_HASH_SEED);
}

RGINLINE uint64_t rg_hash_ptr(const void* ptr)
{
	return rg_hash_u64((uint64_t)(uintptr_t)ptr);
}

RGINLINE int rg_hash_eq_u32(uint32_t a, uint32_t b)
{
	return a == b;
}

RGINLINE int rg_hash_eq_u64(uint64_t a, uint64_t b)
{
	return a == b;
}

RGINLINE int rg_hash_eq_ptr(const void* a, const void* b)
{
	return a == b;
}

RGINLINE int rg_hash_eq_str(const char* a, const char* b)
{
	return strcmp(a, b) == 0;
}

RGINLINE uint64_t rg_hash_prepare(uint64_t hash)
{
	return hash == 0 ? 1 : hash;
}

RGINLINE size_t rg_hash_probe_distance(uint64_t hash, size_t index, size_t mask)
{
	return (index - (hash & mask)) & mask;
}

RGINLINE size_t rg_hash_pow2_ceil(size_t value)
{
	if (value <= 1)
	{
		return 1;
	}

	value--;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
#if SIZE_MAX > UINT32_MAX
	value |= value >> 32;
#endif
	return value + 1;
}

RGINLINE size_t rg_hash_capacity_for(size_t count)
{
	if (count == 0)
	{
		return 0;
	}

	const size_t numerator = (size_t)RG_HASH_MAX_LOAD_NUM;
	const size_t denominator = (size_t)RG_HASH_MAX_LOAD_DEN;
	if (count > (SIZE_MAX - (numerator - 1)) / denominator)
	{
		return 0;
	}

	size_t needed = (count * denominator + numerator - 1) / numerator;
	if (needed < (size_t)RG_HASH_MIN_CAP)
	{
		needed = (size_t)RG_HASH_MIN_CAP;
	}
	return rg_hash_pow2_ceil(needed);
}

RGINLINE int rg_hash_should_grow(size_t count, size_t cap)
{
	if (cap == 0 || count == SIZE_MAX)
	{
		return 1;
	}

	const size_t next_count = count + 1;
	const size_t numerator = (size_t)RG_HASH_MAX_LOAD_NUM;
	const size_t denominator = (size_t)RG_HASH_MAX_LOAD_DEN;
	if (next_count > SIZE_MAX / denominator || cap > SIZE_MAX / numerator)
	{
		size_t needed = rg_hash_capacity_for(next_count);
		return needed == 0 || needed > cap;
	}
	return next_count * denominator > cap * numerator;
}

#endif // RG_HASH_H
