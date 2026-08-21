// rg_log public API correctness tests

#define RG_LOG_NO_COLOR
#define RG_ASSERT_ABORT() ((void)0)
#define RG_ASSERT_BREAK() ((void)0)

#include "../src/rg_log.h"
#include "../src/rg_assert.h"

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

static size_t capture_read(FILE* file, char* buffer, size_t capacity)
{
	if (capacity == 0)
		return 0;
	fflush(file);
	fseek(file, 0, SEEK_SET);
	size_t count = fread(buffer, 1, capacity - 1, file);
	buffer[count] = '\0';
	return count;
}

static int side_effect_count;

static int side_effect(void)
{
	side_effect_count++;
	return 42;
}

static void test_levels(void)
{
	rg_log_init(RG_LOG_DEBUG);
	CHECK(rg_log_get_level() == RG_LOG_DEBUG);
	rg_log_set_level(RG_LOG_WARN);
	CHECK(rg_log_get_level() == RG_LOG_WARN);
	rg_log_set_level(RG_LOG_NONE);
	CHECK(rg_log_get_level() == RG_LOG_NONE);
}

static void test_output(void)
{
	FILE* capture = tmpfile();
	CHECK(capture != NULL);
	if (capture == NULL)
		return;

	rg_log_init(RG_LOG_DEBUG);
	FILE* original = rg_log_level_table[RG_LOG_INFO].stream;
	rg_log_level_table[RG_LOG_INFO].stream = capture;
	rg_log(RG_LOG_INFO, "C:\\project\\game.c", 42, "value=%d", 7);

	char output[256];
	capture_read(capture, output, sizeof(output));
	CHECK(strcmp(output, "[INFO] game.c:42: value=7\n") == 0);

	rg_log_level_table[RG_LOG_INFO].stream = original;
	fclose(capture);
}

static void test_filtering(void)
{
	FILE* capture = tmpfile();
	CHECK(capture != NULL);
	if (capture == NULL)
		return;

	rg_log_set_level(RG_LOG_ERROR);
	FILE* original = rg_log_level_table[RG_LOG_ERROR].stream;
	rg_log_level_table[RG_LOG_ERROR].stream = capture;
	side_effect_count = 0;
	RG_DEBUG("filtered=%d", side_effect());
	CHECK(side_effect_count == 0);
	RG_ERROR("visible=%d", side_effect());
	CHECK(side_effect_count == 1);

	char output[256];
	capture_read(capture, output, sizeof(output));
	CHECK(strstr(output, "visible=42") != NULL);
	CHECK(strstr(output, "filtered") == NULL);

	rg_log_set_level(RG_LOG_NONE);
	RG_ERROR("filtered=%d", side_effect());
	CHECK(side_effect_count == 1);
	rg_log_level_table[RG_LOG_ERROR].stream = original;
	fclose(capture);
}

static void test_truncation(void)
{
	FILE* capture = tmpfile();
	CHECK(capture != NULL);
	if (capture == NULL)
		return;

	char message[RG_LOG_BUFFER_SIZE * 3];
	memset(message, 'x', sizeof(message) - 1);
	message[sizeof(message) - 1] = '\0';

	rg_log_set_level(RG_LOG_INFO);
	FILE* original = rg_log_level_table[RG_LOG_INFO].stream;
	rg_log_level_table[RG_LOG_INFO].stream = capture;
	rg_log(RG_LOG_INFO, "long_file_name.c", 12345, "%s", message);

	char output[RG_LOG_BUFFER_SIZE + 1];
	size_t count = capture_read(capture, output, sizeof(output));
	CHECK(count > 0);
	CHECK(count <= RG_LOG_BUFFER_SIZE - 1);
	CHECK(output[count - 1] == '\n');

	rg_log_level_table[RG_LOG_INFO].stream = original;
	fclose(capture);
}

static void test_assert_integration(void)
{
	FILE* capture = tmpfile();
	CHECK(capture != NULL);
	if (capture == NULL)
		return;

	rg_log_set_level(RG_LOG_DEBUG);
	FILE* original = rg_log_level_table[RG_LOG_CRIT].stream;
	rg_log_level_table[RG_LOG_CRIT].stream = capture;
	int result = RG_ENSURE_MSG(0, "missing value %d", 9);
	CHECK(result == 0);

	char output[512];
	capture_read(capture, output, sizeof(output));
	CHECK(strstr(output, "[CRIT]") != NULL);
	CHECK(strstr(output, "ASSERT FAIL: (0) - missing value 9") != NULL);

	rg_log_level_table[RG_LOG_CRIT].stream = original;
	fclose(capture);
}

int main(void)
{
	test_levels();
	test_output();
	test_filtering();
	test_truncation();
	test_assert_integration();

	if (tests_failed == 0)
		printf("All %d rg_log checks passed\n", tests_run);
	else
		printf("%d of %d rg_log checks failed\n", tests_failed, tests_run);
	return tests_failed == 0 ? 0 : 1;
}
