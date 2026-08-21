// rg_sprintf public API correctness tests

#if defined(RG_SPRINTF_TEST_HYBRID)
#include "../src/rg_sprintf_hybrid.h"
#elif defined(RG_SPRINTF_TEST_ASM)
#include "../src/rg_sprintf_asm.h"
#else
#include "../src/rg_sprintf.h"
#endif

#include <limits.h>
#include <stdint.h>
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

#define CHECK_FORMAT(expected, format, ...)                                \
	do                                                                     \
	{                                                                      \
		char actual_buffer[256];                                           \
		int actual_count = rg_sprintf(actual_buffer, format, __VA_ARGS__); \
		CHECK(strcmp(actual_buffer, expected) == 0);                       \
		CHECK(actual_count == (int)strlen(expected));                      \
	} while (0)

#define CHECK_LIBC(format, ...)                                                            \
	do                                                                                     \
	{                                                                                      \
		char expected_buffer[256];                                                         \
		char actual_buffer[256];                                                           \
		int expected_count = sprintf(expected_buffer, format, __VA_ARGS__);                \
		int actual_count = rg_sprintf(actual_buffer, format, __VA_ARGS__);                 \
		tests_run += 2;                                                                    \
		if (strcmp(actual_buffer, expected_buffer) != 0 || actual_count != expected_count) \
		{                                                                                  \
			printf("FAIL %s:%d: format %s\n", __FILE__, __LINE__, format);                 \
			printf("  libc: %s (%d)\n", expected_buffer, expected_count);                  \
			printf("  rg:   %s (%d)\n", actual_buffer, actual_count);                      \
			if (strcmp(actual_buffer, expected_buffer) != 0) tests_failed++;               \
			if (actual_count != expected_count) tests_failed++;                            \
		}                                                                                  \
	} while (0)

typedef struct CallbackBuffer
{
	char data[2048];
	size_t len;
	int calls;
} CallbackBuffer;

static void capture_callback(const char* buf, void* user, int len)
{
	CallbackBuffer* capture = (CallbackBuffer*)user;
	size_t available = sizeof(capture->data) - capture->len - 1;
	size_t copy_len = (size_t)len < available ? (size_t)len : available;

	memcpy(capture->data + capture->len, buf, copy_len);
	capture->len += copy_len;
	capture->data[capture->len] = '\0';
	capture->calls++;
}

static void test_basic_formatting(void)
{
	char buffer[64];
	int count = rg_sprintf(buffer, "plain text");

	CHECK(strcmp(buffer, "plain text") == 0);
	CHECK(count == 10);

	CHECK_FORMAT("value=-42", "value=%d", -42);
	CHECK_FORMAT("42", "%i", 42);
	CHECK_FORMAT("4294967295", "%u", UINT32_MAX);
	CHECK_FORMAT("deadbeef", "%x", 0xDEADBEEFu);
	CHECK_FORMAT("DEADBEEF", "%X", 0xDEADBEEFu);
	CHECK_FORMAT("755", "%o", 0755u);
	CHECK_FORMAT("100%", "%d%%", 100);
	CHECK_FORMAT("A", "%c", 'A');
	CHECK_FORMAT("hello", "%s", "hello");
}

static void test_width_precision_and_flags(void)
{
	CHECK_LIBC("%8d", 42);
	CHECK_LIBC("%-8d", 42);
	CHECK_LIBC("%08d", -42);
	CHECK_LIBC("%+d", 42);
	CHECK_LIBC("% d", 42);
	CHECK_LIBC("%.5d", 42);
	CHECK_LIBC("%#x", 0x2au);
	CHECK_LIBC("%#X", 0x2au);
	CHECK_LIBC("%#o", 0755u);
	CHECK_LIBC("%10s", "hello");
	CHECK_LIBC("%-10s", "hello");
	CHECK_LIBC("%.3s", "hello");
	CHECK_LIBC("%*.*f", 9, 3, 1.25);
}

static void test_integer_limits(void)
{
	CHECK_LIBC("%d", INT_MIN);
	CHECK_LIBC("%d", INT_MAX);
	CHECK_LIBC("%lld", (long long)INT64_MIN);
	CHECK_LIBC("%lld", (long long)INT64_MAX);
	CHECK_LIBC("%llu", (unsigned long long)UINT64_MAX);
	CHECK_LIBC("%zu", (size_t)1234567);
}

static void test_floating_point(void)
{
	CHECK_LIBC("%.2f", 3.25);
	CHECK_LIBC("%.3f", -1.125);
	CHECK_LIBC("%.6e", 0.000001);
	CHECK_LIBC("%.4g", 12.5);
	CHECK_LIBC("%+.1f", 2.0);
}

static void test_snprintf_bounds(void)
{
	char buffer[8];
	char one[1] = {'X'};
	int count;

	memset(buffer, 'X', sizeof(buffer));
	count = rg_snprintf(buffer, sizeof(buffer), "value=%d", 1234);
	CHECK(count == 10);
	CHECK(strcmp(buffer, "value=1") == 0);

	count = rg_snprintf(one, sizeof(one), "abc");
	CHECK(count == 3);
	CHECK(one[0] == '\0');

	count = rg_snprintf(NULL, 0, "value=%d", 1234);
	CHECK(count == 10);
}

static void test_direct_conversions(void)
{
	char buffer[64];
	char* end;

	end = rg_itoa(INT32_MIN, buffer);
	CHECK(strcmp(buffer, "-2147483648") == 0);
	CHECK(end == buffer + strlen(buffer));

	end = rg_utoa(UINT32_MAX, buffer);
	CHECK(strcmp(buffer, "4294967295") == 0);
	CHECK(end == buffer + strlen(buffer));

	end = rg_i64toa(INT64_MIN, buffer);
	CHECK(strcmp(buffer, "-9223372036854775808") == 0);
	CHECK(end == buffer + strlen(buffer));

	end = rg_u64toa(UINT64_MAX, buffer);
	CHECK(strcmp(buffer, "18446744073709551615") == 0);
	CHECK(end == buffer + strlen(buffer));

	end = rg_dtoa(12.5, buffer, 3);
	CHECK(strcmp(buffer, "12.500") == 0);
	CHECK(end == buffer + strlen(buffer));

	end = rg_ftoa(-0.25f, buffer, 2);
	CHECK(strcmp(buffer, "-0.25") == 0);
	CHECK(end == buffer + strlen(buffer));
}

static void test_hex_conversion(void)
{
	static const uint8_t input[] = {0x00, 0x12, 0xAB, 0xFF};
	uint8_t output[sizeof(input)] = {0};
	char hex[sizeof(input) * 2 + 1];
	char* end;
	int count;

	end = rg_to_hex(input, sizeof(input), hex, 0);
	CHECK(strcmp(hex, "0012abff") == 0);
	CHECK(end == hex + 8);

	rg_to_hex(input, sizeof(input), hex, 1);
	CHECK(strcmp(hex, "0012ABFF") == 0);

	count = rg_from_hex(hex, strlen(hex), output);
	CHECK(count == (int)sizeof(input));
	CHECK(memcmp(input, output, sizeof(input)) == 0);
	CHECK(rg_from_hex("abc", 3, output) == -1);
	CHECK(rg_from_hex("zz", 2, output) == -1);
}

static void test_callback_output(void)
{
	CallbackBuffer capture = {{0}, 0, 0};
	char payload[1500];
	int count;

	memset(payload, 'q', sizeof(payload) - 1);
	payload[sizeof(payload) - 1] = '\0';

	count = rg_sprintf_cb(capture_callback, &capture, "prefix:%s:%d", payload, 42);
	CHECK(count == 1509);
	CHECK(capture.len == 1509);
	CHECK(capture.calls >= 2);
	CHECK(strncmp(capture.data, "prefix:", 7) == 0);
	CHECK(strcmp(capture.data + capture.len - 3, ":42") == 0);
}

static void test_builder(void)
{
	static const uint8_t bytes[] = {0x12, 0xAB};
	char buffer[64];
	RgBuilder builder;
	size_t len;

	rg_builder_init(&builder, buffer, sizeof(buffer));
	rg_builder_append(&builder, "id=");
	rg_builder_append_uint(&builder, 42);
	rg_builder_append_char(&builder, ' ');
	rg_builder_appendf(&builder, "name=%s ", "hero");
	rg_builder_append_hex(&builder, bytes, sizeof(bytes));
	len = rg_builder_finish(&builder);

	CHECK(strcmp(buffer, "id=42 name=hero 12ab") == 0);
	CHECK(len == strlen(buffer));

	rg_builder_reset(&builder);
	rg_builder_append_float(&builder, 1.25, 2);
	CHECK(strcmp(buffer, "1.25") == 0);

	{
		char small[5];
		rg_builder_init(&builder, small, sizeof(small));
		rg_builder_append(&builder, "abcdef");
		CHECK(strcmp(small, "abcd") == 0);
		CHECK(rg_builder_finish(&builder) == 4);
	}
}

static void test_former_benchmark_templates(void)
{
	// These must remain correct through the generic formatter. Their exact
	// wording must not be recognized by special-case dispatch in the library.
	CHECK_LIBC("[frame %06u] dt=%.3fms fps=%u jobs=%u", 7u, 16.125, 60u, 12u);
	CHECK_LIBC("asset load %s/%s_%04u.rgi %.1f%%", "tiles", "jungle", 7u, 42.0);
	CHECK_LIBC("job[%02u] %s worker=%u time=%.3fms", 7u, "visibility", 3u, 0.020);
	CHECK_LIBC("warn %s:%d %s code=%08x", "renderer.c", 120, "resized", 0xC0000000u);
	CHECK_LIBC("net peer=%s seq=%u ack=%u ping=%d loss=%g", "east-2", 10u, 9u, 18, 0.25);
	CHECK_LIBC("prof %-10s %lld ticks %.6e", "physics", (long long)123456789, 0.000001);
	CHECK_LIBC("rect=(%d,%d %dx%d)", 32, 48, 96, 24);
	CHECK_LIBC("save slot=%u scene=%s version=%u bytes=%u", 1u, "dungeon", 42u, 1024u);
}

int main(void)
{
	test_basic_formatting();
	test_width_precision_and_flags();
	test_integer_limits();
	test_floating_point();
	test_snprintf_bounds();
	test_direct_conversions();
	test_hex_conversion();
	test_callback_output();
	test_builder();
	test_former_benchmark_templates();

	if (tests_failed != 0)
	{
		printf("%d of %d checks failed\n", tests_failed, tests_run);
		return 1;
	}

	printf("All %d rg_sprintf checks passed\n", tests_run);
	return 0;
}
