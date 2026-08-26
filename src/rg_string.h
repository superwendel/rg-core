// rg_string - String utilities, UTF-8 helpers, and arena-backed strings
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header C99 library for common string operations, UTF-8 conversion and
// validation, and length-aware strings backed by rg_mem arenas.
//
// USAGE:
//   #include "rg_string.h"
//
//   char text[] = "  hello  ";
//   char* trimmed = rg_trim(text);
//
//   rg_malloc(MB(1));
//   RgArena arena = rg_arena_create(KB(64));
//   RgString message;
//   rgs_init_with(&message, &arena, trimmed);
//   rg_arena_free(&arena);
//   rg_free();
//
// OPTIONS:
//   #define RG_STRING_SECURE       - Return safely for null arguments
//   #define RG_STRING_ASSERT(x)    - Custom assertion macro (default: assert)
//
// NOTES:
//   - ASCII case conversion leaves bytes 128-255 unchanged.
//   - Replace and join destinations must not overlap their inputs.
//   - RgString growth consumes arena space; old buffers remain until reset.
//   - All functions have internal linkage and work in unity builds.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_STRING_H
#define RG_STRING_H

#include "rg_mem.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

// =============================================================================
// PUBLIC API - Extended Functions
// =============================================================================

/**
 * @brief Trim whitespace from both ends of string (in-place)
 * @param str String to trim (modified in place)
 * @return Pointer to trimmed string (may be offset from original)
 */
RGINLINE char* rg_trim(char* str);

/**
 * @brief Trim whitespace from left side of string
 * @param str String to trim
 * @return Pointer to first non-whitespace character
 */
RGINLINE char* rg_ltrim(char* str);

/**
 * @brief Trim whitespace from right side of string (in-place)
 * @param str String to trim (modified in place)
 * @return Pointer to string
 */
RGINLINE char* rg_rtrim(char* str);

/**
 * @brief Convert string to lowercase (in-place)
 * @param str String to convert
 * @return Pointer to string
 * @note ASCII only (A-Z to a-z). Non-ASCII bytes pass through unchanged.
 */
RGINLINE char* rg_strlower(char* str);

/**
 * @brief Convert string to uppercase (in-place)
 * @param str String to convert
 * @return Pointer to string
 * @note ASCII only (a-z to A-Z). Non-ASCII bytes pass through unchanged.
 */
RGINLINE char* rg_strupper(char* str);

/**
 * @brief Reverse string (in-place)
 * @param str String to reverse
 * @return Pointer to string
 */
RGINLINE char* rg_strrev(char* str);

// =============================================================================
// PUBLIC API - Utility Functions
// =============================================================================

/**
 * @brief Check if string starts with prefix
 * @param str String to check
 * @param prefix Prefix to look for
 * @return 1 if str starts with prefix, 0 otherwise
 */
RGINLINE int rg_startswith(const char* str, const char* prefix);

/**
 * @brief Check if string ends with suffix
 * @param str String to check
 * @param suffix Suffix to look for
 * @return 1 if str ends with suffix, 0 otherwise
 */
RGINLINE int rg_endswith(const char* str, const char* suffix);

/**
 * @brief Count occurrences of substring in string
 * @param str String to search
 * @param needle Substring to count
 * @return Number of non-overlapping occurrences
 */
RGINLINE size_t rg_strcount(const char* str, const char* needle);

/**
 * @brief Replace first occurrence of substring
 * @param dst Destination buffer
 * @param dst_size Size of destination buffer
 * @param src Source string
 * @param find Substring to find
 * @param replace Replacement string
 * @return Required result length excluding the null terminator
 * @note Pass NULL and zero to query the required length. Destination and
 *       inputs must not overlap.
 */
RGINLINE size_t rg_replace(char* dst, size_t dst_size, const char* src,
                           const char* find, const char* replace);

/**
 * @brief Replace all occurrences of substring
 * @param dst Destination buffer
 * @param dst_size Size of destination buffer
 * @param src Source string
 * @param find Substring to find
 * @param replace Replacement string
 * @return Required result length excluding the null terminator
 * @note Pass NULL and zero to query the required length. Destination and
 *       inputs must not overlap.
 */
RGINLINE size_t rg_replace_all(char* dst, size_t dst_size, const char* src,
                               const char* find, const char* replace);

/**
 * @brief Split string by delimiter character
 * @param str String to split (will be modified - nulls inserted)
 * @param delim Delimiter character
 * @param parts Array to fill with pointers to parts
 * @param max_parts Maximum number of parts
 * @return Number of non-empty parts stored
 * @note Empty fields are skipped. All delimiters are replaced with nulls even
 *       when max_parts is reached.
 */
RGINLINE size_t rg_split(char* str, char delim, char** parts, size_t max_parts);

/**
 * @brief Join strings with separator
 * @param dst Destination buffer
 * @param dst_size Size of destination buffer
 * @param parts Array of string pointers
 * @param count Number of strings
 * @param sep Separator string
 * @return Required result length excluding the null terminator
 * @note Pass NULL and zero to query the required length. Destination and
 *       inputs must not overlap. Null part pointers are treated as empty.
 */
RGINLINE size_t rg_join(char* dst, size_t dst_size, const char** parts,
                        size_t count, const char* sep);

// =============================================================================
// PUBLIC API - UTF-8 Functions
// =============================================================================

/**
 * @brief Count UTF-8 codepoints (characters) in a string
 * @param str UTF-8 encoded string
 * @return Number of codepoints (not bytes)
 * @note "hello" = 5, "\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E" = 3
 * @warning Does not validate UTF-8. For invalid sequences, the count may be
 *          incorrect. Use rg_utf8_valid() first if validation is needed.
 */
RGINLINE size_t rg_utf8_len(const char* str);

/**
 * @brief Decode one UTF-8 codepoint from string
 * @param str Pointer to UTF-8 bytes
 * @param codepoint Output: decoded Unicode codepoint (0-0x10FFFF)
 * @return Bytes consumed (1-4), or 0 on invalid sequence
 */
RGINLINE int rg_utf8_decode(const char* str, u32* codepoint);

/**
 * @brief Encode one codepoint to UTF-8
 * @param dst Destination buffer (must have 4 bytes available)
 * @param codepoint Unicode codepoint to encode (0-0x10FFFF)
 * @return Bytes written (1-4), or 0 if codepoint is invalid
 */
RGINLINE int rg_utf8_encode(char* dst, u32 codepoint);

/**
 * @brief Validate UTF-8 string
 * @param str String to validate
 * @return 1 if valid UTF-8, 0 if invalid
 */
RGINLINE int rg_utf8_valid(const char* str);

// =============================================================================
// PUBLIC API - RgString (Length-aware string for fast operations)
// =============================================================================

/**
 * @brief Length-aware string struct for O(1) length and fast byte operations
 *
 * Unlike C-strings which require scanning for null terminator, RgString stores
 * the length, enabling fast copy and concatenation without rescanning it.
 *
 * Storage comes from an arena; growth allocates new buffers without freeing
 * old memory until the arena is reset.
 */
typedef struct RgString
{
	RgArena* arena; // Arena backing storage (must outlive the string)
	char* data;     // Pointer to string data (always null-terminated)
	size_t len;     // Length excluding null terminator
	size_t cap;     // Capacity excluding null terminator
} RgString;

/**
 * @brief Initialize an empty RgString
 * @param s String to initialize
 * @param arena Arena for allocations
 */
RGINLINE void rgs_init(RgString* s, RgArena* arena);

/**
 * @brief Initialize RgString with a C-string
 * @param s String to initialize
 * @param arena Arena for allocations
 * @param str Source C-string to copy
 */
RGINLINE void rgs_init_with(RgString* s, RgArena* arena, const char* str);

/**
 * @brief Initialize RgString with a C-string and known length
 * @param s String to initialize
 * @param arena Arena for allocations
 * @param str Source C-string to copy
 * @param len Length of source string
 */
RGINLINE void rgs_init_with_n(RgString* s, RgArena* arena, const char* str, size_t len);

/**
 * @brief Release an RgString handle
 * @param s String to release
 * @note This does not reclaim arena memory. Reset or free the arena to reclaim
 *       storage. Reinitialize the handle before using it again.
 */
RGINLINE void rgs_free(RgString* s);

/**
 * @brief Get pointer to string data (null-terminated)
 * @param s Source string
 * @return Pointer to string data
 */
RGINLINE const char* rgs_data(const RgString* s);

/**
 * @brief Get string length - O(1)
 * @param s Source string
 * @return Length excluding null terminator
 */
RGINLINE size_t rgs_len(const RgString* s);

/**
 * @brief Copy C-string into RgString
 * @param dst Destination RgString
 * @param src Source C-string
 */
RGINLINE void rgs_copy(RgString* dst, const char* src);

/**
 * @brief Copy bytes with known length into RgString
 * @param dst Destination RgString
 * @param src Source C-string
 * @param len Length of source string
 */
RGINLINE void rgs_copy_n(RgString* dst, const char* src, size_t len);

/**
 * @brief Concatenate C-string to RgString
 * @param dst Destination RgString
 * @param src Source C-string to append
 */
RGINLINE void rgs_cat(RgString* dst, const char* src);

/**
 * @brief Concatenate bytes with known length to RgString
 * @param dst Destination RgString
 * @param src Source C-string to append
 * @param len Length of source string
 */
RGINLINE void rgs_cat_n(RgString* dst, const char* src, size_t len);

/**
 * @brief Compare two RgStrings
 * @param a First string
 * @param b Second string
 * @return <0 if a<b, 0 if equal, >0 if a>b
 */
RGINLINE int rgs_cmp(const RgString* a, const RgString* b);

/**
 * @brief Clear string contents (keeps capacity)
 * @param s String to clear
 */
RGINLINE void rgs_clear(RgString* s);

/**
 * @brief Reserve capacity for future growth
 * @param s String to reserve capacity for
 * @param cap Minimum capacity to reserve
 * @note Growth allocates from the arena; old buffers are not freed.
 */
RGINLINE void rgs_reserve(RgString* s, size_t cap);

// =============================================================================
// IMPLEMENTATION
// =============================================================================

#ifndef RG_STRING_ASSERT
#include <assert.h>
#define RG_STRING_ASSERT(x) assert(x)
#endif

// -----------------------------------------------------------------------------
// Lookup Tables
// -----------------------------------------------------------------------------

// Whitespace lookup table (space, tab, newline, carriage return, vertical tab, form feed)
static const char rg_whitespace[256] = {
	0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// Case conversion tables (ASCII only: A-Z and a-z, bytes 128-255 pass through unchanged)
static const u8 rg_tolower_table[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
	64,'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',91,92,93,94,95,
	96,'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
};

static const u8 rg_toupper_table[256] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
	64,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',91,92,93,94,95,
	96,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
};

// -----------------------------------------------------------------------------
// Extended Functions Implementation
// -----------------------------------------------------------------------------

RGINLINE char* rg_ltrim(char* str)
{
#ifdef RG_STRING_SECURE
	if (!str) return NULL;
#endif
	while (rg_whitespace[(u8)*str]) str++;
	return str;
}

RGINLINE char* rg_rtrim(char* str)
{
#ifdef RG_STRING_SECURE
	if (!str) return str;
#endif
	char* end = str + strlen(str);
	while (end > str && rg_whitespace[(u8)end[-1]])
	{
		end--;
		*end = '\0';
	}
	return str;
}

RGINLINE char* rg_trim(char* str)
{
#ifdef RG_STRING_SECURE
	if (!str) return NULL;
#endif
	str = rg_ltrim(str);
	return rg_rtrim(str);
}

RGINLINE char* rg_strlower(char* str)
{
#ifdef RG_STRING_SECURE
	if (!str) return NULL;
#endif
	u8* s = (u8*)str;
	while (*s)
	{
		*s = rg_tolower_table[*s];
		s++;
	}
	return str;
}

RGINLINE char* rg_strupper(char* str)
{
#ifdef RG_STRING_SECURE
	if (!str) return NULL;
#endif
	u8* s = (u8*)str;
	while (*s)
	{
		*s = rg_toupper_table[*s];
		s++;
	}
	return str;
}

RGINLINE char* rg_strrev(char* str)
{
#ifdef RG_STRING_SECURE
	if (!str) return NULL;
#endif
	if (*str == '\0') return str;

	char* start = str;
	char* end = str + strlen(str) - 1;

	while (start < end)
	{
		char tmp = *start;
		*start = *end;
		*end = tmp;
		start++;
		end--;
	}
	return str;
}

// -----------------------------------------------------------------------------
// Utility Functions Implementation
// -----------------------------------------------------------------------------

RGINLINE int rg_startswith(const char* str, const char* prefix)
{
#ifdef RG_STRING_SECURE
	if (!str || !prefix) return 0;
#endif
	while (*prefix)
	{
		if (*str == '\0' || *str != *prefix) return 0;
		str++;
		prefix++;
	}
	return 1;
}

RGINLINE int rg_endswith(const char* str, const char* suffix)
{
#ifdef RG_STRING_SECURE
	if (!str || !suffix) return 0;
#endif
	size_t str_len = strlen(str);
	size_t suffix_len = strlen(suffix);

	if (suffix_len > str_len) return 0;

	return strcmp(str + str_len - suffix_len, suffix) == 0;
}

RGINLINE size_t rg_strcount(const char* str, const char* needle)
{
#ifdef RG_STRING_SECURE
	if (!str || !needle) return 0;
#endif
	if (*needle == '\0') return 0;

	size_t needle_len = strlen(needle);
	if (needle_len == 1)
	{
		size_t count = 0;
		const char* pos = str;
		char ch = needle[0];
		while ((pos = strchr(pos, ch)) != NULL)
		{
			count++;
			pos++;
		}
		return count;
	}

	size_t count = 0;
	const char* pos = str;
	while ((pos = strstr(pos, needle)) != NULL)
	{
		count++;
		pos += needle_len;
	}
	return count;
}

RGINLINE void rg_string_copy_bounded(char* dst, size_t dst_size, size_t* written,
                                     const char* src, size_t len)
{
	if (dst_size == 0 || *written >= dst_size - 1) return;

	size_t available = dst_size - 1 - *written;
	size_t copy_len = len < available ? len : available;
	if (copy_len > 0)
	{
		memcpy(dst + *written, src, copy_len);
		*written += copy_len;
	}
}

RGINLINE int rg_string_add_length(size_t* total, size_t amount)
{
	if (amount > SIZE_MAX - *total)
	{
		*total = SIZE_MAX;
		return 0;
	}
	*total += amount;
	return 1;
}

RGINLINE size_t rg_replace(char* dst, size_t dst_size, const char* src,
                           const char* find, const char* replace)
{
#ifdef RG_STRING_SECURE
	if (!src || !find || !replace || (dst_size > 0 && !dst)) return 0;
#endif
	size_t written = 0;
	if (dst_size > 0) dst[0] = '\0';

	size_t find_len = strlen(find);
	size_t replace_len = strlen(replace);
	const char* match = find_len > 0 ? strstr(src, find) : NULL;
	if (!match)
	{
		size_t src_len = strlen(src);
		rg_string_copy_bounded(dst, dst_size, &written, src, src_len);
		if (dst_size > 0) dst[written] = '\0';
		return src_len;
	}

	size_t before_len = (size_t)(match - src);
	const char* tail = match + find_len;
	size_t tail_len = strlen(tail);
	size_t result_len = 0;
	if (!rg_string_add_length(&result_len, before_len) ||
	    !rg_string_add_length(&result_len, replace_len) ||
	    !rg_string_add_length(&result_len, tail_len))
		return SIZE_MAX;

	rg_string_copy_bounded(dst, dst_size, &written, src, before_len);
	rg_string_copy_bounded(dst, dst_size, &written, replace, replace_len);
	rg_string_copy_bounded(dst, dst_size, &written, tail, tail_len);
	if (dst_size > 0) dst[written] = '\0';
	return result_len;
}

RGINLINE size_t rg_replace_all(char* dst, size_t dst_size, const char* src,
                               const char* find, const char* replace)
{
#ifdef RG_STRING_SECURE
	if (!src || !find || !replace || (dst_size > 0 && !dst)) return 0;
#endif
	size_t written = 0;
	size_t result_len = 0;
	if (dst_size > 0) dst[0] = '\0';

	size_t find_len = strlen(find);
	size_t replace_len = strlen(replace);
	if (find_len == 0)
	{
		size_t src_len = strlen(src);
		rg_string_copy_bounded(dst, dst_size, &written, src, src_len);
		if (dst_size > 0) dst[written] = '\0';
		return src_len;
	}

	const char* cursor = src;
	const char* match;
	while ((match = strstr(cursor, find)) != NULL)
	{
		size_t before_len = (size_t)(match - cursor);
		if (!rg_string_add_length(&result_len, before_len) ||
		    !rg_string_add_length(&result_len, replace_len))
		{
			if (dst_size > 0) dst[written] = '\0';
			return SIZE_MAX;
		}

		rg_string_copy_bounded(dst, dst_size, &written, cursor, before_len);
		rg_string_copy_bounded(dst, dst_size, &written, replace, replace_len);
		cursor = match + find_len;
	}

	size_t tail_len = strlen(cursor);
	if (!rg_string_add_length(&result_len, tail_len))
	{
		if (dst_size > 0) dst[written] = '\0';
		return SIZE_MAX;
	}
	rg_string_copy_bounded(dst, dst_size, &written, cursor, tail_len);
	if (dst_size > 0) dst[written] = '\0';
	return result_len;
}

RGINLINE size_t rg_split(char* str, char delim, char** parts, size_t max_parts)
{
#ifdef RG_STRING_SECURE
	if (!str || (max_parts > 0 && !parts)) return 0;
#endif
	size_t count = 0;
	char* field = str;
	char* cursor = str;
	while (*cursor)
	{
		if (*cursor == delim)
		{
			if (cursor > field && count < max_parts) parts[count++] = field;
			*cursor = '\0';
			field = cursor + 1;
		}
		cursor++;
	}
	if (cursor > field && count < max_parts) parts[count++] = field;

	return count;
}

RGINLINE size_t rg_join(char* dst, size_t dst_size, const char** parts,
                        size_t count, const char* sep)
{
#ifdef RG_STRING_SECURE
	if ((dst_size > 0 && !dst) || (count > 0 && !parts)) return 0;
#endif
	if (!sep) sep = "";

	size_t sep_len = strlen(sep);
	size_t written = 0;
	size_t result_len = 0;
	if (dst_size > 0) dst[0] = '\0';

	for (size_t i = 0; i < count; i++)
	{
		if (i > 0)
		{
			if (!rg_string_add_length(&result_len, sep_len))
			{
				if (dst_size > 0) dst[written] = '\0';
				return SIZE_MAX;
			}
			rg_string_copy_bounded(dst, dst_size, &written, sep, sep_len);
		}

		const char* part = parts[i] ? parts[i] : "";
		size_t part_len = strlen(part);
		if (!rg_string_add_length(&result_len, part_len))
		{
			if (dst_size > 0) dst[written] = '\0';
			return SIZE_MAX;
		}
		rg_string_copy_bounded(dst, dst_size, &written, part, part_len);
	}

	if (dst_size > 0) dst[written] = '\0';
	return result_len;
}

// -----------------------------------------------------------------------------
// UTF-8 Functions Implementation
// -----------------------------------------------------------------------------

RGINLINE size_t rg_utf8_len(const char* str)
{
#ifdef RG_STRING_SECURE
	if (!str) return 0;
#endif
	size_t count = 0;
	while (*str)
	{
		// Count bytes that are NOT continuation bytes (10xxxxxx)
		// Lead bytes are 0xxxxxxx, 110xxxxx, 1110xxxx, 11110xxx
		if (((u8)*str & 0xC0) != 0x80)
		{
			count++;
		}
		str++;
	}
	return count;
}

RGINLINE int rg_utf8_decode(const char* str, u32* codepoint)
{
#ifdef RG_STRING_SECURE
	if (!str || !codepoint) return 0;
#endif
	u8 c = (u8)*str;

	// ASCII (0xxxxxxx)
	if (c < 0x80)
	{
		*codepoint = c;
		return c ? 1 : 0;  // Return 0 for null terminator
	}

	// 2-byte sequence (110xxxxx 10xxxxxx)
	if ((c & 0xE0) == 0xC0)
	{
		u8 c1 = (u8)str[1];
		if ((c1 & 0xC0) != 0x80) return 0;  // Invalid or truncated

		*codepoint = ((c & 0x1F) << 6) | (c1 & 0x3F);
		if (*codepoint < 0x80) return 0;  // Overlong encoding
		return 2;
	}

	// 3-byte sequence (1110xxxx 10xxxxxx 10xxxxxx)
	if ((c & 0xF0) == 0xE0)
	{
		// Check each continuation byte before reading the next (avoid reading past buffer)
		u8 c1 = (u8)str[1];
		if ((c1 & 0xC0) != 0x80) return 0;  // Invalid or truncated
		u8 c2 = (u8)str[2];
		if ((c2 & 0xC0) != 0x80) return 0;  // Invalid or truncated

		*codepoint = ((c & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
		if (*codepoint < 0x800) return 0;  // Overlong
		if (*codepoint >= 0xD800 && *codepoint <= 0xDFFF) return 0;  // Surrogate
		return 3;
	}

	// 4-byte sequence (11110xxx 10xxxxxx 10xxxxxx 10xxxxxx)
	if ((c & 0xF8) == 0xF0)
	{
		// Check each continuation byte before reading the next (avoid reading past buffer)
		u8 c1 = (u8)str[1];
		if ((c1 & 0xC0) != 0x80) return 0;  // Invalid or truncated
		u8 c2 = (u8)str[2];
		if ((c2 & 0xC0) != 0x80) return 0;  // Invalid or truncated
		u8 c3 = (u8)str[3];
		if ((c3 & 0xC0) != 0x80) return 0;  // Invalid or truncated

		*codepoint = ((c & 0x07) << 18) | ((c1 & 0x3F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
		if (*codepoint < 0x10000 || *codepoint > 0x10FFFF) return 0;  // Overlong or out of range
		return 4;
	}

	return 0;  // Invalid lead byte
}

RGINLINE int rg_utf8_encode(char* dst, u32 codepoint)
{
#ifdef RG_STRING_SECURE
	if (!dst) return 0;
#endif
	// ASCII
	if (codepoint < 0x80)
	{
		dst[0] = (char)codepoint;
		return 1;
	}

	// 2-byte sequence
	if (codepoint < 0x800)
	{
		dst[0] = (char)(0xC0 | (codepoint >> 6));
		dst[1] = (char)(0x80 | (codepoint & 0x3F));
		return 2;
	}

	// 3-byte sequence (reject surrogates)
	if (codepoint < 0x10000)
	{
		if (codepoint >= 0xD800 && codepoint <= 0xDFFF) return 0;  // Surrogate
		dst[0] = (char)(0xE0 | (codepoint >> 12));
		dst[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
		dst[2] = (char)(0x80 | (codepoint & 0x3F));
		return 3;
	}

	// 4-byte sequence
	if (codepoint <= 0x10FFFF)
	{
		dst[0] = (char)(0xF0 | (codepoint >> 18));
		dst[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
		dst[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
		dst[3] = (char)(0x80 | (codepoint & 0x3F));
		return 4;
	}

	return 0;  // Invalid codepoint
}

RGINLINE int rg_utf8_valid(const char* str)
{
#ifdef RG_STRING_SECURE
	if (!str) return 0;
#endif
	while (*str)
	{
		u32 codepoint;
		int bytes = rg_utf8_decode(str, &codepoint);
		if (bytes == 0) return 0;  // Invalid sequence
		str += bytes;
	}
	return 1;
}

// -----------------------------------------------------------------------------
// RgString Functions Implementation
// -----------------------------------------------------------------------------

// Empty string singleton (avoids null checks)
static char rgs_empty_str[1] = {'\0'};

RGINLINE void rgs_init(RgString* s, RgArena* arena)
{
#ifdef RG_STRING_SECURE
	if (!s || !arena) return;
#endif
	RG_STRING_ASSERT(s != NULL && arena != NULL);
	if (!s || !arena) return;
	s->arena = arena;
	s->data = rgs_empty_str;
	s->len = 0;
	s->cap = 0;
}

RGINLINE void rgs_init_with(RgString* s, RgArena* arena, const char* str)
{
#ifdef RG_STRING_SECURE
	if (!s || !arena) return;
#endif
	if (!str || !*str)
	{
		rgs_init(s, arena);
		return;
	}

	size_t len = strlen(str);
	rgs_init_with_n(s, arena, str, len);
}

RGINLINE void rgs_init_with_n(RgString* s, RgArena* arena, const char* str, size_t len)
{
#ifdef RG_STRING_SECURE
	if (!s || !arena) return;
#endif
	rgs_init(s, arena);
	if (!str || len == 0 || len == SIZE_MAX)
	{
		return;
	}

	char* data = (char*)rg_arena_alloc_aligned(arena, len + 1, RG_ALIGNOF(char));
	if (!data)
	{
		return;
	}

	memmove(data, str, len);
	data[len] = '\0';
	s->data = data;
	s->len = len;
	s->cap = len;
}

RGINLINE void rgs_free(RgString* s)
{
#ifdef RG_STRING_SECURE
	if (!s) return;
#endif
	s->arena = NULL;
	s->data = rgs_empty_str;
	s->len = 0;
	s->cap = 0;
}

RGINLINE const char* rgs_data(const RgString* s)
{
#ifdef RG_STRING_SECURE
	if (!s) return rgs_empty_str;
#endif
	return s->data;
}

RGINLINE size_t rgs_len(const RgString* s)
{
#ifdef RG_STRING_SECURE
	if (!s) return 0;
#endif
	return s->len;
}

RGINLINE void rgs_reserve(RgString* s, size_t cap)
{
#ifdef RG_STRING_SECURE
	if (!s || !s->arena) return;
#endif
	RG_STRING_ASSERT(s != NULL && s->arena != NULL);
	if (!s || !s->arena) return;
	if (cap <= s->cap) return;

	if (cap == SIZE_MAX) return;

	char* new_data = (char*)rg_arena_alloc_aligned(s->arena, cap + 1, RG_ALIGNOF(char));
	if (!new_data) return;

	if (s->len > 0)
	{
		memmove(new_data, s->data, s->len);
	}
	new_data[s->len] = '\0';

	s->data = new_data;
	s->cap = cap;
}

RGINLINE void rgs_copy(RgString* dst, const char* src)
{
#ifdef RG_STRING_SECURE
	if (!dst) return;
#endif
	if (!src || !*src)
	{
		rgs_clear(dst);
		return;
	}

	size_t len = strlen(src);
	rgs_copy_n(dst, src, len);
}

RGINLINE void rgs_copy_n(RgString* dst, const char* src, size_t len)
{
#ifdef RG_STRING_SECURE
	if (!dst) return;
#endif
	RG_STRING_ASSERT(dst != NULL);
	if (!dst) return;
	if (!src || len == 0)
	{
		rgs_clear(dst);
		return;
	}

	if (len > dst->cap)
	{
		rgs_reserve(dst, len);
		if (len > dst->cap) return;
	}

	memmove(dst->data, src, len);
	dst->data[len] = '\0';
	dst->len = len;
}

RGINLINE void rgs_cat(RgString* dst, const char* src)
{
#ifdef RG_STRING_SECURE
	if (!dst) return;
#endif
	if (!src || !*src) return;

	size_t len = strlen(src);
	rgs_cat_n(dst, src, len);
}

RGINLINE void rgs_cat_n(RgString* dst, const char* src, size_t len)
{
#ifdef RG_STRING_SECURE
	if (!dst) return;
#endif
	RG_STRING_ASSERT(dst != NULL);
	if (!dst) return;
	if (!src || len == 0) return;
	if (len > SIZE_MAX - dst->len) return;

	size_t new_len = dst->len + len;

	if (new_len > dst->cap)
	{
		size_t grow = dst->cap + (dst->cap / 2);
		if (grow < dst->cap) grow = SIZE_MAX;
		size_t new_cap = new_len > grow ? new_len : grow;
		if (new_cap < 16) new_cap = 16;
		rgs_reserve(dst, new_cap);
		if (new_len > dst->cap) return;
	}

	memmove(dst->data + dst->len, src, len);
	dst->len = new_len;
	dst->data[dst->len] = '\0';
}

RGINLINE int rgs_cmp(const RgString* a, const RgString* b)
{
#ifdef RG_STRING_SECURE
	if (!a || !b) return a ? 1 : (b ? -1 : 0);
#endif
	// Fast path: if lengths differ and one is prefix of other, use length
	size_t min_len = a->len < b->len ? a->len : b->len;

	// Compare contents
	int result = memcmp(a->data, b->data, min_len);
	if (result != 0) return result;

	// Contents equal up to min_len, shorter string is "less"
	if (a->len < b->len) return -1;
	if (a->len > b->len) return 1;
	return 0;
}

RGINLINE void rgs_clear(RgString* s)
{
#ifdef RG_STRING_SECURE
	if (!s) return;
#endif
	s->len = 0;
	if (s->data && s->data != rgs_empty_str)
	{
		s->data[0] = '\0';
	}
}

#endif // RG_STRING_H
