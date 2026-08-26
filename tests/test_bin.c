// rg_bin public API correctness tests

#include "../src/rg_bin.h"
#include "../src/rg_bin.h"

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

static void test_swaps_and_primitives(void)
{
	u8 storage[32] = {0};
	u8* buffer = storage + 1;
	f32 f32_value = -13.25f;
	f64 f64_value = 12345.5;
	f32 f32_roundtrip;
	f64 f64_roundtrip;

	CHECK(rg_bin_swap16(UINT16_C(0x1122)) == UINT16_C(0x2211));
	CHECK(rg_bin_swap32(UINT32_C(0x11223344)) == UINT32_C(0x44332211));
	CHECK(rg_bin_swap64(UINT64_C(0x1122334455667788)) == UINT64_C(0x8877665544332211));

	rg_bin_store_u8(buffer, UINT8_C(0xa5));
	CHECK(rg_bin_load_u8(buffer) == UINT8_C(0xa5));

	rg_bin_store_u16_le(buffer, UINT16_C(0x1122));
	CHECK(buffer[0] == UINT8_C(0x22) && buffer[1] == UINT8_C(0x11));
	CHECK(rg_bin_load_u16_le(buffer) == UINT16_C(0x1122));
	CHECK(rg_bin_load_u16_be(buffer) == UINT16_C(0x2211));

	rg_bin_store_u16_be(buffer, UINT16_C(0x1122));
	CHECK(buffer[0] == UINT8_C(0x11) && buffer[1] == UINT8_C(0x22));
	CHECK(rg_bin_load_u16_be(buffer) == UINT16_C(0x1122));

	rg_bin_store_u32_le(buffer, UINT32_C(0x11223344));
	CHECK(buffer[0] == UINT8_C(0x44) && buffer[1] == UINT8_C(0x33) &&
	      buffer[2] == UINT8_C(0x22) && buffer[3] == UINT8_C(0x11));
	CHECK(rg_bin_load_u32_le(buffer) == UINT32_C(0x11223344));
	CHECK(rg_bin_load_u32_be(buffer) == UINT32_C(0x44332211));

	rg_bin_store_u32_be(buffer, UINT32_C(0x11223344));
	CHECK(buffer[0] == UINT8_C(0x11) && buffer[1] == UINT8_C(0x22) &&
	      buffer[2] == UINT8_C(0x33) && buffer[3] == UINT8_C(0x44));
	CHECK(rg_bin_load_u32_be(buffer) == UINT32_C(0x11223344));

	rg_bin_store_u64_le(buffer, UINT64_C(0x1122334455667788));
	CHECK(buffer[0] == UINT8_C(0x88) && buffer[7] == UINT8_C(0x11));
	CHECK(rg_bin_load_u64_le(buffer) == UINT64_C(0x1122334455667788));
	CHECK(rg_bin_load_u64_be(buffer) == UINT64_C(0x8877665544332211));

	rg_bin_store_u64_be(buffer, UINT64_C(0x1122334455667788));
	CHECK(buffer[0] == UINT8_C(0x11) && buffer[7] == UINT8_C(0x88));
	CHECK(rg_bin_load_u64_be(buffer) == UINT64_C(0x1122334455667788));

	rg_bin_store_f32_le(buffer, f32_value);
	f32_roundtrip = rg_bin_load_f32_le(buffer);
	CHECK(memcmp(&f32_roundtrip, &f32_value, sizeof(f32_value)) == 0);
	rg_bin_store_f32_be(buffer, f32_value);
	f32_roundtrip = rg_bin_load_f32_be(buffer);
	CHECK(memcmp(&f32_roundtrip, &f32_value, sizeof(f32_value)) == 0);

	rg_bin_store_f64_le(buffer, f64_value);
	f64_roundtrip = rg_bin_load_f64_le(buffer);
	CHECK(memcmp(&f64_roundtrip, &f64_value, sizeof(f64_value)) == 0);
	rg_bin_store_f64_be(buffer, f64_value);
	f64_roundtrip = rg_bin_load_f64_be(buffer);
	CHECK(memcmp(&f64_roundtrip, &f64_value, sizeof(f64_value)) == 0);
}

static void test_pointer_cursors(void)
{
	static const u8 payload[] = {UINT8_C(0xde), UINT8_C(0xad), UINT8_C(0xbe)};
	u8 buffer[64] = {0};
	u8* writer = buffer;
	const u8* reader;
	const u8* bytes;

	rg_bin_write_u8(&writer, UINT8_C(0x5a));
	rg_bin_write_u16_le(&writer, UINT16_C(0x1234));
	rg_bin_write_u16_be(&writer, UINT16_C(0x5678));
	rg_bin_write_u32_le(&writer, UINT32_C(0x11223344));
	rg_bin_write_u32_be(&writer, UINT32_C(0x55667788));
	rg_bin_write_u64_le(&writer, UINT64_C(0x0123456789abcdef));
	rg_bin_write_u64_be(&writer, UINT64_C(0xfedcba9876543210));
	rg_bin_write_bytes(&writer, payload, sizeof(payload));

	reader = buffer;
	CHECK(rg_bin_read_u8(&reader) == UINT8_C(0x5a));
	CHECK(rg_bin_read_u16_le(&reader) == UINT16_C(0x1234));
	CHECK(rg_bin_read_u16_be(&reader) == UINT16_C(0x5678));
	CHECK(rg_bin_read_u32_le(&reader) == UINT32_C(0x11223344));
	CHECK(rg_bin_read_u32_be(&reader) == UINT32_C(0x55667788));
	CHECK(rg_bin_read_u64_le(&reader) == UINT64_C(0x0123456789abcdef));
	CHECK(rg_bin_read_u64_be(&reader) == UINT64_C(0xfedcba9876543210));
	bytes = rg_bin_read_bytes(&reader, sizeof(payload));
	CHECK(memcmp(bytes, payload, sizeof(payload)) == 0);
	CHECK((size_t)(reader - buffer) == (size_t)(writer - buffer));
}

static void test_bounded_cursors(void)
{
	static const u8 payload[] = {1u, 2u, 3u, 4u};
	u8 buffer[128] = {0};
	RgBinWriter writer;
	RgBinReader reader;
	u8* reserved;
	size_t used;

	rg_bin_writer_init(&writer, buffer, sizeof(buffer));
	CHECK(rg_bin_writer_remaining(&writer) == sizeof(buffer));
	reserved = rg_bin_writer_reserve(&writer, 2u);
	reserved[0] = UINT8_C(0xaa);
	reserved[1] = UINT8_C(0xbb);
	rg_bin_writer_write_u8(&writer, UINT8_C(0x7f));
	rg_bin_writer_write_u16_le(&writer, UINT16_C(0x1234));
	rg_bin_writer_write_u16_be(&writer, UINT16_C(0x5678));
	rg_bin_writer_write_u32_le(&writer, UINT32_C(0x11223344));
	rg_bin_writer_write_u32_be(&writer, UINT32_C(0x55667788));
	rg_bin_writer_write_u64_le(&writer, UINT64_C(0x0123456789abcdef));
	rg_bin_writer_write_u64_be(&writer, UINT64_C(0xfedcba9876543210));
	rg_bin_writer_write_f32_le(&writer, 1.5f);
	rg_bin_writer_write_f32_be(&writer, -2.25f);
	rg_bin_writer_write_f64_le(&writer, 3.5);
	rg_bin_writer_write_f64_be(&writer, -4.75);
	rg_bin_writer_write_uvarint_u32(&writer, 300u);
	rg_bin_writer_write_uvarint_u64(&writer, UINT64_C(0x1fffffffffffff));
	rg_bin_writer_write_svarint_s32(&writer, -12345);
	rg_bin_writer_write_svarint_s64(&writer, INT64_C(-1234567890123));
	rg_bin_writer_write_bytes(&writer, payload, sizeof(payload));
	used = sizeof(buffer) - rg_bin_writer_remaining(&writer);

	rg_bin_reader_init(&reader, buffer, used);
	CHECK(rg_bin_reader_remaining(&reader) == used);
	CHECK(rg_bin_reader_peek(&reader, 2u) == buffer);
	CHECK(rg_bin_reader_remaining(&reader) == used);
	CHECK(rg_bin_reader_take(&reader, 1u)[0] == UINT8_C(0xaa));
	rg_bin_reader_skip(&reader, 1u);
	CHECK(rg_bin_reader_read_u8(&reader) == UINT8_C(0x7f));
	CHECK(rg_bin_reader_read_u16_le(&reader) == UINT16_C(0x1234));
	CHECK(rg_bin_reader_read_u16_be(&reader) == UINT16_C(0x5678));
	CHECK(rg_bin_reader_read_u32_le(&reader) == UINT32_C(0x11223344));
	CHECK(rg_bin_reader_read_u32_be(&reader) == UINT32_C(0x55667788));
	CHECK(rg_bin_reader_read_u64_le(&reader) == UINT64_C(0x0123456789abcdef));
	CHECK(rg_bin_reader_read_u64_be(&reader) == UINT64_C(0xfedcba9876543210));
	CHECK(rg_bin_reader_read_f32_le(&reader) == 1.5f);
	CHECK(rg_bin_reader_read_f32_be(&reader) == -2.25f);
	CHECK(rg_bin_reader_read_f64_le(&reader) == 3.5);
	CHECK(rg_bin_reader_read_f64_be(&reader) == -4.75);
	CHECK(rg_bin_reader_read_uvarint_u32(&reader) == 300u);
	CHECK(rg_bin_reader_read_uvarint_u64(&reader) == UINT64_C(0x1fffffffffffff));
	CHECK(rg_bin_reader_read_svarint_s32(&reader) == -12345);
	CHECK(rg_bin_reader_read_svarint_s64(&reader) == INT64_C(-1234567890123));
	CHECK(memcmp(rg_bin_reader_take(&reader, sizeof(payload)), payload, sizeof(payload)) == 0);
	CHECK(rg_bin_reader_remaining(&reader) == 0u);
}

static void test_zigzag(void)
{
	static const i32 s32_values[] = {0, 1, -1, 2, -2, 12345, -12345, INT32_MIN, INT32_MAX};
	static const i64 s64_values[] = {0, 1, -1, 2, -2, INT64_C(1234567890123),
	                                 INT64_C(-1234567890123), INT64_MIN, INT64_MAX};

	CHECK(rg_bin_zigzag_encode_s32(0) == 0u);
	CHECK(rg_bin_zigzag_encode_s32(-1) == 1u);
	CHECK(rg_bin_zigzag_encode_s32(1) == 2u);
	CHECK(rg_bin_zigzag_encode_s32(INT32_MIN) == UINT32_MAX);
	CHECK(rg_bin_zigzag_encode_s32(INT32_MAX) == UINT32_MAX - 1u);
	CHECK(rg_bin_zigzag_encode_s64(INT64_MIN) == UINT64_MAX);
	CHECK(rg_bin_zigzag_encode_s64(INT64_MAX) == UINT64_MAX - 1u);

	for (size_t i = 0; i < RG_ARRAY_COUNT(s32_values); i++)
	{
		CHECK(rg_bin_zigzag_decode_s32(rg_bin_zigzag_encode_s32(s32_values[i])) == s32_values[i]);
	}
	for (size_t i = 0; i < RG_ARRAY_COUNT(s64_values); i++)
	{
		CHECK(rg_bin_zigzag_decode_s64(rg_bin_zigzag_encode_s64(s64_values[i])) == s64_values[i]);
	}
}

static void test_unsigned_varints(void)
{
	static const u32 u32_values[] = {
	    0u, 1u, 127u, 128u, 16383u, 16384u, UINT32_C(0x1fffff), UINT32_C(0x200000),
	    UINT32_C(0x0fffffff), UINT32_C(0x10000000), UINT32_MAX};
	static const u64 u64_values[] = {
	    0u, 1u, 127u, 128u, UINT64_C(0xffffffff), UINT64_C(0x1fffffffffffff),
	    UINT64_C(0x7fffffffffffffff), UINT64_MAX};
	u8 encoded[RG_BIN_UVARINT64_MAX_BYTES];

	CHECK(rg_bin_uvarint_len_u32(127u) == 1u);
	CHECK(rg_bin_uvarint_len_u32(128u) == 2u);
	CHECK(rg_bin_uvarint_len_u64(UINT64_MAX) == 10u);
	CHECK(rg_bin_write_uvarint_u32(encoded, 300u) == 2u);
	CHECK(encoded[0] == UINT8_C(0xac) && encoded[1] == UINT8_C(0x02));

	for (size_t i = 0; i < RG_ARRAY_COUNT(u32_values); i++)
	{
		const u8* reader;
		size_t len = rg_bin_write_uvarint_u32(encoded, u32_values[i]);
		reader = encoded;
		CHECK(len == rg_bin_uvarint_len_u32(u32_values[i]));
		CHECK((encoded[len - 1u] & 0x80u) == 0u);
		CHECK(rg_bin_read_uvarint_u32(&reader) == u32_values[i]);
		CHECK((size_t)(reader - encoded) == len);
	}

	for (size_t i = 0; i < RG_ARRAY_COUNT(u64_values); i++)
	{
		const u8* reader;
		size_t len = rg_bin_write_uvarint_u64(encoded, u64_values[i]);
		reader = encoded;
		CHECK(len == rg_bin_uvarint_len_u64(u64_values[i]));
		CHECK((encoded[len - 1u] & 0x80u) == 0u);
		CHECK(rg_bin_read_uvarint_u64(&reader) == u64_values[i]);
		CHECK((size_t)(reader - encoded) == len);
	}
}

static void test_signed_varints(void)
{
	static const i32 s32_values[] = {0, 1, -1, 12345, -12345, INT32_MIN, INT32_MAX};
	static const i64 s64_values[] = {
	    0, 1, -1, INT64_C(1234567890123), INT64_C(-1234567890123), INT64_MIN, INT64_MAX};
	u8 encoded[RG_BIN_UVARINT64_MAX_BYTES];

	for (size_t i = 0; i < RG_ARRAY_COUNT(s32_values); i++)
	{
		const u8* reader;
		size_t len = rg_bin_write_svarint_s32(encoded, s32_values[i]);
		reader = encoded;
		CHECK(len == rg_bin_svarint_len_s32(s32_values[i]));
		CHECK(rg_bin_read_svarint_s32(&reader) == s32_values[i]);
		CHECK((size_t)(reader - encoded) == len);
	}

	for (size_t i = 0; i < RG_ARRAY_COUNT(s64_values); i++)
	{
		const u8* reader;
		size_t len = rg_bin_write_svarint_s64(encoded, s64_values[i]);
		reader = encoded;
		CHECK(len == rg_bin_svarint_len_s64(s64_values[i]));
		CHECK(rg_bin_read_svarint_s64(&reader) == s64_values[i]);
		CHECK((size_t)(reader - encoded) == len);
	}
}

int main(void)
{
	test_swaps_and_primitives();
	test_pointer_cursors();
	test_bounded_cursors();
	test_zigzag();
	test_unsigned_varints();
	test_signed_varints();

	printf("rg_bin: %d checks, %d failures\n", tests_run, tests_failed);
	return tests_failed ? 1 : 0;
}
