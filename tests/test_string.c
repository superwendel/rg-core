// rg_string public API correctness tests

#define RG_STRING_ASSERT(condition) ((void)sizeof(condition))

#include "../src/rg_string.h"
#include "../src/rg_string.h"

#include <stdio.h>
#include <string.h>

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

static void test_trim_and_case(void)
{
	char both[] = " \t\r\n hello \v\f";
	char left[] = "\t  left";
	char right[] = "right \n\t";
	char whitespace[] = " \t\r\n\v\f";
	char empty[] = "";
	char lower[] = "AbC-123";
	char upper[] = "aBc-123";
	char high[] = "\xc0" "A";
	char reverse[] = "abcdef";

	CHECK(strcmp(rg_trim(both), "hello") == 0);
	CHECK(strcmp(rg_ltrim(left), "left") == 0);
	CHECK(strcmp(rg_rtrim(right), "right") == 0);
	CHECK(strcmp(rg_rtrim(whitespace), "") == 0);
	CHECK(rg_rtrim(empty) == empty && empty[0] == '\0');
	CHECK(strcmp(rg_strlower(lower), "abc-123") == 0);
	CHECK(strcmp(rg_strupper(upper), "ABC-123") == 0);
	CHECK((u8)rg_strlower(high)[0] == UINT8_C(0xc0) && high[1] == 'a');
	CHECK(strcmp(rg_strrev(reverse), "fedcba") == 0);
	CHECK(strcmp(rg_strrev(empty), "") == 0);
}

static void test_queries(void)
{
	CHECK(rg_startswith("render/entity", "render"));
	CHECK(rg_startswith("render", ""));
	CHECK(!rg_startswith("render", "renderer"));
	CHECK(rg_endswith("texture.dds", ".dds"));
	CHECK(rg_endswith("texture.dds", ""));
	CHECK(!rg_endswith("texture.dds", ".png"));
	CHECK(rg_strcount("aaaaa", "aa") == 2);
	CHECK(rg_strcount("one two one", "one") == 2);
	CHECK(rg_strcount("abc", "") == 0);
	CHECK(rg_strcount("abc", "z") == 0);
}

static void test_replace(void)
{
	char buffer[64];
	char small[5];
	char untouched = 'x';

	CHECK(rg_replace(buffer, sizeof(buffer), "foo bar foo", "foo", "x") == 9);
	CHECK(strcmp(buffer, "x bar foo") == 0);
	CHECK(rg_replace(small, sizeof(small), "foo bar foo", "foo", "x") == 9);
	CHECK(strcmp(small, "x ba") == 0);
	CHECK(rg_replace(NULL, 0, "foo bar foo", "foo", "long") == 12);
	CHECK(rg_replace(&untouched, 0, "abc", "b", "xyz") == 5);
	CHECK(untouched == 'x');
	CHECK(rg_replace(buffer, sizeof(buffer), "abc", "z", "x") == 3);
	CHECK(strcmp(buffer, "abc") == 0);
	CHECK(rg_replace(buffer, sizeof(buffer), "abc", "", "x") == 3);
	CHECK(strcmp(buffer, "abc") == 0);

	CHECK(rg_replace_all(buffer, sizeof(buffer), "foo bar foo", "foo", "x") == 7);
	CHECK(strcmp(buffer, "x bar x") == 0);
	CHECK(rg_replace_all(buffer, 6, "a-a", "a", "XYZ") == 7);
	CHECK(strcmp(buffer, "XYZ-X") == 0);
	CHECK(rg_replace_all(NULL, 0, "a-a", "a", "XYZ") == 7);
	CHECK(rg_replace_all(buffer, sizeof(buffer), "abc", "", "x") == 3);
	CHECK(strcmp(buffer, "abc") == 0);
}

static void test_split_and_join(void)
{
	char fields[] = ",alpha,,beta,";
	char capped[] = "a,b,c";
	char empty[] = "";
	char* parts[4] = {0};

	CHECK(rg_split(fields, ',', parts, RG_ARRAY_COUNT(parts)) == 2);
	CHECK(strcmp(parts[0], "alpha") == 0);
	CHECK(strcmp(parts[1], "beta") == 0);
	CHECK(rg_split(capped, ',', parts, 1) == 1);
	CHECK(strcmp(parts[0], "a") == 0);
	CHECK(capped[1] == '\0' && capped[3] == '\0');
	CHECK(rg_split(empty, ',', parts, RG_ARRAY_COUNT(parts)) == 0);

	const char* joined_parts[] = {"alpha", NULL, "beta"};
	char joined[32];
	char small[4];
	CHECK(rg_join(joined, sizeof(joined), joined_parts,
	              RG_ARRAY_COUNT(joined_parts), "/") == 11);
	CHECK(strcmp(joined, "alpha//beta") == 0);
	CHECK(rg_join(small, sizeof(small), joined_parts,
	              RG_ARRAY_COUNT(joined_parts), "/") == 11);
	CHECK(strcmp(small, "alp") == 0);
	CHECK(rg_join(NULL, 0, joined_parts, RG_ARRAY_COUNT(joined_parts), "/") == 11);
	CHECK(rg_join(joined, sizeof(joined), NULL, 0, NULL) == 0);
	CHECK(strcmp(joined, "") == 0);
}

static void test_utf8(void)
{
	static const char valid[] = "\x24\xc2\xa2\xe2\x82\xac\xf0\x90\x8d\x88";
	static const u32 expected[] = {
		UINT32_C(0x24),
		UINT32_C(0xa2),
		UINT32_C(0x20ac),
		UINT32_C(0x10348),
	};
	static const char overlong[] = "\xc0\x80";
	static const char surrogate[] = "\xed\xa0\x80";
	static const char too_large[] = "\xf4\x90\x80\x80";
	static const char truncated[] = "\xe2\x82";
	static const char continuation[] = "\x80";
	char invalid_encoding[4] = {0};

	CHECK(rg_utf8_len(valid) == RG_ARRAY_COUNT(expected));
	CHECK(rg_utf8_valid(valid));
	const char* cursor = valid;
	for (size_t i = 0; i < RG_ARRAY_COUNT(expected); i++)
	{
		u32 codepoint = 0;
		int bytes = rg_utf8_decode(cursor, &codepoint);
		CHECK(bytes >= 1 && bytes <= 4);
		CHECK(codepoint == expected[i]);

		char encoded[4] = {0};
		CHECK(rg_utf8_encode(encoded, codepoint) == bytes);
		CHECK(memcmp(encoded, cursor, (size_t)bytes) == 0);
		cursor += bytes;
	}
	CHECK(*cursor == '\0');
	CHECK(!rg_utf8_valid(overlong));
	CHECK(!rg_utf8_valid(surrogate));
	CHECK(!rg_utf8_valid(too_large));
	CHECK(!rg_utf8_valid(truncated));
	CHECK(!rg_utf8_valid(continuation));
	CHECK(rg_utf8_encode(invalid_encoding, UINT32_C(0xd800)) == 0);
	CHECK(rg_utf8_encode(invalid_encoding, UINT32_C(0x110000)) == 0);
}

static void test_rgstring(RgArena* arena)
{
	RgString text;
	rgs_init(&text, arena);
	CHECK(rgs_len(&text) == 0);
	CHECK(strcmp(rgs_data(&text), "") == 0);
	CHECK(text.cap == 0);

	rgs_copy(&text, "hello");
	CHECK(rgs_len(&text) == 5);
	CHECK(strcmp(rgs_data(&text), "hello") == 0);
	rgs_reserve(&text, 32);
	CHECK(text.cap >= 32);
	CHECK(strcmp(rgs_data(&text), "hello") == 0);
	rgs_cat(&text, " world");
	CHECK(rgs_len(&text) == 11);
	CHECK(strcmp(rgs_data(&text), "hello world") == 0);

	RgString alias;
	rgs_init_with(&alias, arena, "abc");
	rgs_cat(&alias, rgs_data(&alias));
	CHECK(strcmp(rgs_data(&alias), "abcabc") == 0);
	rgs_copy_n(&alias, rgs_data(&alias) + 2, 4);
	CHECK(alias.len == 4 && strcmp(rgs_data(&alias), "cabc") == 0);

	static const char binary[] = {'a', '\0', 'b'};
	RgString bytes;
	rgs_init_with_n(&bytes, arena, binary, sizeof(binary));
	CHECK(bytes.len == sizeof(binary));
	CHECK(memcmp(bytes.data, binary, sizeof(binary)) == 0);
	CHECK(bytes.data[sizeof(binary)] == '\0');

	RgString greater;
	rgs_init_with(&greater, arena, "cabd");
	CHECK(rgs_cmp(&alias, &greater) < 0);
	CHECK(rgs_cmp(&alias, &alias) == 0);

	size_t capacity = text.cap;
	rgs_clear(&text);
	CHECK(text.len == 0 && text.cap == capacity);
	CHECK(strcmp(rgs_data(&text), "") == 0);
	rgs_reserve(&text, SIZE_MAX);
	CHECK(text.cap == capacity);

	char byte = 0;
	RgArena full = {0};
	full.memory = &byte;
	full.capacity = 1;
	full.used = 1;
	full.committed = 1;
	RgString failed;
	rgs_init(&failed, &full);
	rgs_copy(&failed, "this allocation cannot fit");
	CHECK(failed.len == 0 && failed.cap == 0);
	CHECK(strcmp(rgs_data(&failed), "") == 0);

	rgs_free(&text);
	CHECK(text.arena == NULL && text.len == 0 && text.cap == 0);
	CHECK(strcmp(rgs_data(&text), "") == 0);
}

#ifdef RG_STRING_SECURE
static void test_secure_arguments(void)
{
	CHECK(rg_trim(NULL) == NULL);
	CHECK(rg_strlower(NULL) == NULL);
	CHECK(!rg_startswith(NULL, "x"));
	CHECK(!rg_endswith("x", NULL));
	CHECK(rg_strcount(NULL, "x") == 0);
	CHECK(rg_replace(NULL, 1, "x", "x", "y") == 0);
	CHECK(rg_replace(NULL, 0, NULL, "x", "y") == 0);
	CHECK(rg_split(NULL, ',', NULL, 0) == 0);
	CHECK(rg_join(NULL, 1, NULL, 0, NULL) == 0);
	CHECK(rg_utf8_len(NULL) == 0);
	CHECK(rg_utf8_decode(NULL, NULL) == 0);
	CHECK(rg_utf8_encode(NULL, 0) == 0);
	CHECK(!rg_utf8_valid(NULL));
	CHECK(strcmp(rgs_data(NULL), "") == 0);
	CHECK(rgs_len(NULL) == 0);
	rgs_free(NULL);
	rgs_copy(NULL, "x");
	rgs_cat(NULL, "x");
	rgs_clear(NULL);
}
#endif

int main(void)
{
	if (rg_malloc(MB(8)) != 0)
	{
		printf("Failed to initialize the memory pool.\n");
		return 1;
	}
	CHECK(rg_total() >= MB(8));
	CHECK(rg_used() == 0);
	CHECK(rg_remaining() == rg_total());

	RgArena arena = rg_arena_create(MB(4));
	if (arena.memory == NULL)
	{
		printf("Failed to create the test arena.\n");
		rg_free();
		return 1;
	}

	test_trim_and_case();
	test_queries();
	test_replace();
	test_split_and_join();
	test_utf8();
	test_rgstring(&arena);
#ifdef RG_STRING_SECURE
	test_secure_arguments();
#endif

	rg_arena_free(&arena);
	rg_free();
	printf("rg_string: %d checks, %d failures\n", tests_run, tests_failed);
	return tests_failed == 0 ? 0 : 1;
}
