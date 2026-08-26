// rg_bin - Endian-aware binary I/O and variable-length integers for C
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header binary primitives, pointer cursors, bounded cursors, and varints.
//
// USAGE:
//   #include "rg_bin.h"
//
//   u8 buf[16];
//   rg_bin_store_u32_le(buf, 0x11223344u);
//   u32 value = rg_bin_load_u32_le(buf);
//
// OPTIONS:
//   #define RG_BIN_ASSERT(condition) - Custom assert macro (default: assert)
//   #define RG_BIN_LITTLE_ENDIAN     - Override host endianness (0 or 1)
//   #define RG_BIN_FAST_UNALIGNED    - Use direct unaligned access (default: 0)
//
// NOTES:
//   - Raw loads, stores, and pointer cursors require caller-provided capacity.
//   - RgBinReader and RgBinWriter assert when their capacity is exceeded.
//   - Varints use ULEB128 and zigzag encoding for signed values.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_BIN_H
#define RG_BIN_H

#include "rg_defs.h"

#include <stddef.h>
#include <string.h>

#if RG_COMPILER_MSVC
#include <intrin.h>
#endif

// =============================================================================
// CONFIGURATION
// =============================================================================

#ifndef RG_BIN_ASSERT
#include <assert.h>
#define RG_BIN_ASSERT(condition) assert(condition)
#endif

#ifndef RG_BIN_LITTLE_ENDIAN
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define RG_BIN_LITTLE_ENDIAN 1
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define RG_BIN_LITTLE_ENDIAN 0
#elif RG_PLATFORM_WINDOWS || RG_ARCH_X86 || RG_ARCH_X64 || RG_ARCH_ARM64
#define RG_BIN_LITTLE_ENDIAN 1
#else
#error Unable to detect host endianness; define RG_BIN_LITTLE_ENDIAN to 0 or 1
#endif
#endif

#ifndef RG_BIN_FAST_UNALIGNED
#define RG_BIN_FAST_UNALIGNED 0
#endif

#if RG_BIN_LITTLE_ENDIAN != 0 && RG_BIN_LITTLE_ENDIAN != 1
#error RG_BIN_LITTLE_ENDIAN must be 0 or 1
#endif

#if RG_BIN_FAST_UNALIGNED != 0 && RG_BIN_FAST_UNALIGNED != 1
#error RG_BIN_FAST_UNALIGNED must be 0 or 1
#endif

#define RG_BIN_UVARINT32_MAX_BYTES 5u
#define RG_BIN_UVARINT64_MAX_BYTES 10u

// =============================================================================
// PUBLIC API
// =============================================================================

/**
 * @brief Byte-swap a 16-bit value
 */
RGINLINE u16 rg_bin_swap16(u16 value);

/**
 * @brief Byte-swap a 32-bit value
 */
RGINLINE u32 rg_bin_swap32(u32 value);

/**
 * @brief Byte-swap a 64-bit value
 */
RGINLINE u64 rg_bin_swap64(u64 value);

/**
 * @brief Load unsigned 8-bit value
 */
RGINLINE u8 rg_bin_load_u8(const void* ptr);

/**
 * @brief Store unsigned 8-bit value
 */
RGINLINE void rg_bin_store_u8(void* ptr, u8 value);

/**
 * @brief Load unsigned 16-bit value (little-endian)
 */
RGINLINE u16 rg_bin_load_u16_le(const void* ptr);

/**
 * @brief Load unsigned 16-bit value (big-endian)
 */
RGINLINE u16 rg_bin_load_u16_be(const void* ptr);

/**
 * @brief Store unsigned 16-bit value (little-endian)
 */
RGINLINE void rg_bin_store_u16_le(void* ptr, u16 value);

/**
 * @brief Store unsigned 16-bit value (big-endian)
 */
RGINLINE void rg_bin_store_u16_be(void* ptr, u16 value);

/**
 * @brief Load unsigned 32-bit value (little-endian)
 */
RGINLINE u32 rg_bin_load_u32_le(const void* ptr);

/**
 * @brief Load unsigned 32-bit value (big-endian)
 */
RGINLINE u32 rg_bin_load_u32_be(const void* ptr);

/**
 * @brief Store unsigned 32-bit value (little-endian)
 */
RGINLINE void rg_bin_store_u32_le(void* ptr, u32 value);

/**
 * @brief Store unsigned 32-bit value (big-endian)
 */
RGINLINE void rg_bin_store_u32_be(void* ptr, u32 value);

/**
 * @brief Load unsigned 64-bit value (little-endian)
 */
RGINLINE u64 rg_bin_load_u64_le(const void* ptr);

/**
 * @brief Load unsigned 64-bit value (big-endian)
 */
RGINLINE u64 rg_bin_load_u64_be(const void* ptr);

/**
 * @brief Store unsigned 64-bit value (little-endian)
 */
RGINLINE void rg_bin_store_u64_le(void* ptr, u64 value);

/**
 * @brief Store unsigned 64-bit value (big-endian)
 */
RGINLINE void rg_bin_store_u64_be(void* ptr, u64 value);

/**
 * @brief Load 32-bit floating-point (little-endian)
 */
RGINLINE f32 rg_bin_load_f32_le(const void* ptr);

/**
 * @brief Load 32-bit floating-point (big-endian)
 */
RGINLINE f32 rg_bin_load_f32_be(const void* ptr);

/**
 * @brief Store 32-bit floating-point (little-endian)
 */
RGINLINE void rg_bin_store_f32_le(void* ptr, f32 value);

/**
 * @brief Store 32-bit floating-point (big-endian)
 */
RGINLINE void rg_bin_store_f32_be(void* ptr, f32 value);

/**
 * @brief Load 64-bit floating-point value (little-endian)
 */
RGINLINE f64 rg_bin_load_f64_le(const void* ptr);

/**
 * @brief Load 64-bit floating-point value (big-endian)
 */
RGINLINE f64 rg_bin_load_f64_be(const void* ptr);

/**
 * @brief Store 64-bit floating-point value (little-endian)
 */
RGINLINE void rg_bin_store_f64_le(void* ptr, f64 value);

/**
 * @brief Store 64-bit floating-point value (big-endian)
 */
RGINLINE void rg_bin_store_f64_be(void* ptr, f64 value);

/**
 * @brief Read and advance pointer by 1 byte
 */
RGINLINE u8 rg_bin_read_u8(const u8** ptr);

/**
 * @brief Write and advance pointer by 1 byte
 */
RGINLINE void rg_bin_write_u8(u8** ptr, u8 value);

/**
 * @brief Read and advance pointer by 2 bytes (little-endian)
 */
RGINLINE u16 rg_bin_read_u16_le(const u8** ptr);

/**
 * @brief Read and advance pointer by 2 bytes (big-endian)
 */
RGINLINE u16 rg_bin_read_u16_be(const u8** ptr);

/**
 * @brief Write and advance pointer by 2 bytes (little-endian)
 */
RGINLINE void rg_bin_write_u16_le(u8** ptr, u16 value);

/**
 * @brief Write and advance pointer by 2 bytes (big-endian)
 */
RGINLINE void rg_bin_write_u16_be(u8** ptr, u16 value);

/**
 * @brief Read and advance pointer by 4 bytes (little-endian)
 */
RGINLINE u32 rg_bin_read_u32_le(const u8** ptr);

/**
 * @brief Read and advance pointer by 4 bytes (big-endian)
 */
RGINLINE u32 rg_bin_read_u32_be(const u8** ptr);

/**
 * @brief Write and advance pointer by 4 bytes (little-endian)
 */
RGINLINE void rg_bin_write_u32_le(u8** ptr, u32 value);

/**
 * @brief Write and advance pointer by 4 bytes (big-endian)
 */
RGINLINE void rg_bin_write_u32_be(u8** ptr, u32 value);

/**
 * @brief Read and advance pointer by 8 bytes (little-endian)
 */
RGINLINE u64 rg_bin_read_u64_le(const u8** ptr);

/**
 * @brief Read and advance pointer by 8 bytes (big-endian)
 */
RGINLINE u64 rg_bin_read_u64_be(const u8** ptr);

/**
 * @brief Write and advance pointer by 8 bytes (little-endian)
 */
RGINLINE void rg_bin_write_u64_le(u8** ptr, u64 value);

/**
 * @brief Write and advance pointer by 8 bytes (big-endian)
 */
RGINLINE void rg_bin_write_u64_be(u8** ptr, u64 value);

/**
 * @brief Read bytes and advance pointer by size
 */
RGINLINE const u8* rg_bin_read_bytes(const u8** ptr, size_t size);

/**
 * @brief Write bytes and advance pointer by size
 */
RGINLINE void rg_bin_write_bytes(u8** ptr, const void* src, size_t size);

/**
 * @brief Zigzag encode signed 32-bit value
 */
RGINLINE u32 rg_bin_zigzag_encode_s32(i32 value);

/**
 * @brief Zigzag decode signed 32-bit value
 */
RGINLINE i32 rg_bin_zigzag_decode_s32(u32 value);

/**
 * @brief Zigzag encode signed 64-bit value
 */
RGINLINE u64 rg_bin_zigzag_encode_s64(i64 value);

/**
 * @brief Zigzag decode signed 64-bit value
 */
RGINLINE i64 rg_bin_zigzag_decode_s64(u64 value);

/**
 * @brief ULEB128 length for 32-bit value
 */
RGINLINE size_t rg_bin_uvarint_len_u32(u32 value);

/**
 * @brief ULEB128 length for 64-bit value
 */
RGINLINE size_t rg_bin_uvarint_len_u64(u64 value);

/**
 * @brief ULEB128 length for signed 32-bit value
 */
RGINLINE size_t rg_bin_svarint_len_s32(i32 value);

/**
 * @brief ULEB128 length for signed 64-bit value
 */
RGINLINE size_t rg_bin_svarint_len_s64(i64 value);

/**
 * @brief Write ULEB128 (u32). Returns bytes written.
 */
RGINLINE size_t rg_bin_write_uvarint_u32(u8* dst, u32 value);

/**
 * @brief Write ULEB128 (u64). Returns bytes written.
 */
RGINLINE size_t rg_bin_write_uvarint_u64(u8* dst, u64 value);

/**
 * @brief Read ULEB128 (u32). Advances pointer.
 */
RGINLINE u32 rg_bin_read_uvarint_u32(const u8** ptr);

/**
 * @brief Read ULEB128 (u64). Advances pointer.
 */
RGINLINE u64 rg_bin_read_uvarint_u64(const u8** ptr);

/**
 * @brief Write signed varint (s32). Returns bytes written.
 */
RGINLINE size_t rg_bin_write_svarint_s32(u8* dst, i32 value);

/**
 * @brief Write signed varint (s64). Returns bytes written.
 */
RGINLINE size_t rg_bin_write_svarint_s64(u8* dst, i64 value);

/**
 * @brief Read signed varint (s32). Advances pointer.
 */
RGINLINE i32 rg_bin_read_svarint_s32(const u8** ptr);

/**
 * @brief Read signed varint (s64). Advances pointer.
 */
RGINLINE i64 rg_bin_read_svarint_s64(const u8** ptr);

typedef struct RgBinReader
{
	const u8* cur;
	const u8* end;
} RgBinReader;

typedef struct RgBinWriter
{
	u8* cur;
	u8* end;
} RgBinWriter;

/**
 * @brief Initialize a reader over a buffer
 */
RGINLINE void rg_bin_reader_init(RgBinReader* reader, const void* data, size_t size);

/**
 * @brief Remaining bytes in reader
 */
RGINLINE size_t rg_bin_reader_remaining(const RgBinReader* reader);

/**
 * @brief Peek at the next size bytes
 */
RGINLINE const u8* rg_bin_reader_peek(const RgBinReader* reader, size_t size);

/**
 * @brief Consume size bytes and return pointer to start
 */
RGINLINE const u8* rg_bin_reader_take(RgBinReader* reader, size_t size);

/**
 * @brief Skip size bytes
 */
RGINLINE void rg_bin_reader_skip(RgBinReader* reader, size_t size);

/**
 * @brief Read u8 from reader
 */
RGINLINE u8 rg_bin_reader_read_u8(RgBinReader* reader);

/**
 * @brief Read u16 (little-endian) from reader
 */
RGINLINE u16 rg_bin_reader_read_u16_le(RgBinReader* reader);

/**
 * @brief Read u16 (big-endian) from reader
 */
RGINLINE u16 rg_bin_reader_read_u16_be(RgBinReader* reader);

/**
 * @brief Read u32 (little-endian) from reader
 */
RGINLINE u32 rg_bin_reader_read_u32_le(RgBinReader* reader);

/**
 * @brief Read u32 (big-endian) from reader
 */
RGINLINE u32 rg_bin_reader_read_u32_be(RgBinReader* reader);

/**
 * @brief Read u64 (little-endian) from reader
 */
RGINLINE u64 rg_bin_reader_read_u64_le(RgBinReader* reader);

/**
 * @brief Read u64 (big-endian) from reader
 */
RGINLINE u64 rg_bin_reader_read_u64_be(RgBinReader* reader);

/**
 * @brief Read f32 (little-endian) from reader
 */
RGINLINE f32 rg_bin_reader_read_f32_le(RgBinReader* reader);

/**
 * @brief Read f32 (big-endian) from reader
 */
RGINLINE f32 rg_bin_reader_read_f32_be(RgBinReader* reader);

/**
 * @brief Read f64 (little-endian) from reader
 */
RGINLINE f64 rg_bin_reader_read_f64_le(RgBinReader* reader);

/**
 * @brief Read f64 (big-endian) from reader
 */
RGINLINE f64 rg_bin_reader_read_f64_be(RgBinReader* reader);

/**
 * @brief Read ULEB128 (u32) from reader
 */
RGINLINE u32 rg_bin_reader_read_uvarint_u32(RgBinReader* reader);

/**
 * @brief Read ULEB128 (u64) from reader
 */
RGINLINE u64 rg_bin_reader_read_uvarint_u64(RgBinReader* reader);

/**
 * @brief Read signed varint (s32) from reader
 */
RGINLINE i32 rg_bin_reader_read_svarint_s32(RgBinReader* reader);

/**
 * @brief Read signed varint (s64) from reader
 */
RGINLINE i64 rg_bin_reader_read_svarint_s64(RgBinReader* reader);

/**
 * @brief Initialize a writer over a buffer
 */
RGINLINE void rg_bin_writer_init(RgBinWriter* writer, void* data, size_t size);

/**
 * @brief Remaining bytes in writer
 */
RGINLINE size_t rg_bin_writer_remaining(const RgBinWriter* writer);

/**
 * @brief Reserve size bytes and return pointer to start
 */
RGINLINE u8* rg_bin_writer_reserve(RgBinWriter* writer, size_t size);

/**
 * @brief Write bytes from src
 */
RGINLINE void rg_bin_writer_write_bytes(RgBinWriter* writer, const void* src, size_t size);

/**
 * @brief Write u8 to writer
 */
RGINLINE void rg_bin_writer_write_u8(RgBinWriter* writer, u8 value);

/**
 * @brief Write u16 (little-endian) to writer
 */
RGINLINE void rg_bin_writer_write_u16_le(RgBinWriter* writer, u16 value);

/**
 * @brief Write u16 (big-endian) to writer
 */
RGINLINE void rg_bin_writer_write_u16_be(RgBinWriter* writer, u16 value);

/**
 * @brief Write u32 (little-endian) to writer
 */
RGINLINE void rg_bin_writer_write_u32_le(RgBinWriter* writer, u32 value);

/**
 * @brief Write u32 (big-endian) to writer
 */
RGINLINE void rg_bin_writer_write_u32_be(RgBinWriter* writer, u32 value);

/**
 * @brief Write u64 (little-endian) to writer
 */
RGINLINE void rg_bin_writer_write_u64_le(RgBinWriter* writer, u64 value);

/**
 * @brief Write u64 (big-endian) to writer
 */
RGINLINE void rg_bin_writer_write_u64_be(RgBinWriter* writer, u64 value);

/**
 * @brief Write f32 (little-endian) to writer
 */
RGINLINE void rg_bin_writer_write_f32_le(RgBinWriter* writer, f32 value);

/**
 * @brief Write f32 (big-endian) to writer
 */
RGINLINE void rg_bin_writer_write_f32_be(RgBinWriter* writer, f32 value);

/**
 * @brief Write f64 (little-endian) to writer
 */
RGINLINE void rg_bin_writer_write_f64_le(RgBinWriter* writer, f64 value);

/**
 * @brief Write f64 (big-endian) to writer
 */
RGINLINE void rg_bin_writer_write_f64_be(RgBinWriter* writer, f64 value);

/**
 * @brief Write ULEB128 (u32) to writer
 */
RGINLINE void rg_bin_writer_write_uvarint_u32(RgBinWriter* writer, u32 value);

/**
 * @brief Write ULEB128 (u64) to writer
 */
RGINLINE void rg_bin_writer_write_uvarint_u64(RgBinWriter* writer, u64 value);

/**
 * @brief Write signed varint (s32) to writer
 */
RGINLINE void rg_bin_writer_write_svarint_s32(RgBinWriter* writer, i32 value);

/**
 * @brief Write signed varint (s64) to writer
 */
RGINLINE void rg_bin_writer_write_svarint_s64(RgBinWriter* writer, i64 value);

// =============================================================================
// IMPLEMENTATION
// =============================================================================
RGINLINE u16 rg_bin_swap16(u16 value)
{
#if RG_COMPILER_MSVC
	return _byteswap_ushort(value);
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
	return __builtin_bswap16(value);
#else
	return (u16)((value << 8u) | (value >> 8u));
#endif
}

RGINLINE u32 rg_bin_swap32(u32 value)
{
#if RG_COMPILER_MSVC
	return _byteswap_ulong(value);
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
	return __builtin_bswap32(value);
#else
	return (value << 24u) |
	       ((value << 8u) & 0x00ff0000u) |
	       ((value >> 8u) & 0x0000ff00u) |
	       (value >> 24u);
#endif
}

RGINLINE u64 rg_bin_swap64(u64 value)
{
#if RG_COMPILER_MSVC
	return _byteswap_uint64(value);
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
	return __builtin_bswap64(value);
#else
	return (value << 56u) |
	       ((value << 40u) & 0x00ff000000000000ull) |
	       ((value << 24u) & 0x0000ff0000000000ull) |
	       ((value << 8u) & 0x000000ff00000000ull) |
	       ((value >> 8u) & 0x00000000ff000000ull) |
	       ((value >> 24u) & 0x0000000000ff0000ull) |
	       ((value >> 40u) & 0x000000000000ff00ull) |
	       (value >> 56u);
#endif
}

RGINLINE u8 rg_bin_load_u8(const void* ptr)
{
	return *(const u8*)ptr;
}

RGINLINE void rg_bin_store_u8(void* ptr, u8 value)
{
	*(u8*)ptr = value;
}

#if RG_BIN_LITTLE_ENDIAN
RGINLINE u16 rg_bin_load_u16_le(const void* ptr)
{
#if RG_BIN_FAST_UNALIGNED
	return *(const u16*)ptr;
#else
	u16 value;
	memcpy(&value, ptr, sizeof(value));
	return value;
#endif
}

RGINLINE u16 rg_bin_load_u16_be(const void* ptr)
{
#if RG_BIN_FAST_UNALIGNED
	return rg_bin_swap16(*(const u16*)ptr);
#else
	u16 value;
	memcpy(&value, ptr, sizeof(value));
	return rg_bin_swap16(value);
#endif
}

RGINLINE void rg_bin_store_u16_le(void* ptr, u16 value)
{
#if RG_BIN_FAST_UNALIGNED
	*(u16*)ptr = value;
#else
	memcpy(ptr, &value, sizeof(value));
#endif
}

RGINLINE void rg_bin_store_u16_be(void* ptr, u16 value)
{
#if RG_BIN_FAST_UNALIGNED
	*(u16*)ptr = rg_bin_swap16(value);
#else
	value = rg_bin_swap16(value);
	memcpy(ptr, &value, sizeof(value));
#endif
}

RGINLINE u32 rg_bin_load_u32_le(const void* ptr)
{
#if RG_BIN_FAST_UNALIGNED
	return *(const u32*)ptr;
#else
	u32 value;
	memcpy(&value, ptr, sizeof(value));
	return value;
#endif
}

RGINLINE u32 rg_bin_load_u32_be(const void* ptr)
{
#if RG_BIN_FAST_UNALIGNED
	return rg_bin_swap32(*(const u32*)ptr);
#else
	u32 value;
	memcpy(&value, ptr, sizeof(value));
	return rg_bin_swap32(value);
#endif
}

RGINLINE void rg_bin_store_u32_le(void* ptr, u32 value)
{
#if RG_BIN_FAST_UNALIGNED
	*(u32*)ptr = value;
#else
	memcpy(ptr, &value, sizeof(value));
#endif
}

RGINLINE void rg_bin_store_u32_be(void* ptr, u32 value)
{
#if RG_BIN_FAST_UNALIGNED
	*(u32*)ptr = rg_bin_swap32(value);
#else
	value = rg_bin_swap32(value);
	memcpy(ptr, &value, sizeof(value));
#endif
}

RGINLINE u64 rg_bin_load_u64_le(const void* ptr)
{
#if RG_BIN_FAST_UNALIGNED
	return *(const u64*)ptr;
#else
	u64 value;
	memcpy(&value, ptr, sizeof(value));
	return value;
#endif
}

RGINLINE u64 rg_bin_load_u64_be(const void* ptr)
{
#if RG_BIN_FAST_UNALIGNED
	return rg_bin_swap64(*(const u64*)ptr);
#else
	u64 value;
	memcpy(&value, ptr, sizeof(value));
	return rg_bin_swap64(value);
#endif
}

RGINLINE void rg_bin_store_u64_le(void* ptr, u64 value)
{
#if RG_BIN_FAST_UNALIGNED
	*(u64*)ptr = value;
#else
	memcpy(ptr, &value, sizeof(value));
#endif
}

RGINLINE void rg_bin_store_u64_be(void* ptr, u64 value)
{
#if RG_BIN_FAST_UNALIGNED
	*(u64*)ptr = rg_bin_swap64(value);
#else
	value = rg_bin_swap64(value);
	memcpy(ptr, &value, sizeof(value));
#endif
}
#else
RGINLINE u16 rg_bin_load_u16_le(const void* ptr)
{
	const u8* data = (const u8*)ptr;
	return (u16)data[0] | ((u16)data[1] << 8u);
}

RGINLINE u16 rg_bin_load_u16_be(const void* ptr)
{
	const u8* data = (const u8*)ptr;
	return ((u16)data[0] << 8u) | (u16)data[1];
}

RGINLINE void rg_bin_store_u16_le(void* ptr, u16 value)
{
	u8* data = (u8*)ptr;
	data[0] = (u8)(value & 0xffu);
	data[1] = (u8)(value >> 8u);
}

RGINLINE void rg_bin_store_u16_be(void* ptr, u16 value)
{
	u8* data = (u8*)ptr;
	data[0] = (u8)(value >> 8u);
	data[1] = (u8)(value & 0xffu);
}

RGINLINE u32 rg_bin_load_u32_le(const void* ptr)
{
	const u8* data = (const u8*)ptr;
	return (u32)data[0] |
	       ((u32)data[1] << 8u) |
	       ((u32)data[2] << 16u) |
	       ((u32)data[3] << 24u);
}

RGINLINE u32 rg_bin_load_u32_be(const void* ptr)
{
	const u8* data = (const u8*)ptr;
	return ((u32)data[0] << 24u) |
	       ((u32)data[1] << 16u) |
	       ((u32)data[2] << 8u) |
	       (u32)data[3];
}

RGINLINE void rg_bin_store_u32_le(void* ptr, u32 value)
{
	u8* data = (u8*)ptr;
	data[0] = (u8)(value & 0xffu);
	data[1] = (u8)((value >> 8u) & 0xffu);
	data[2] = (u8)((value >> 16u) & 0xffu);
	data[3] = (u8)(value >> 24u);
}

RGINLINE void rg_bin_store_u32_be(void* ptr, u32 value)
{
	u8* data = (u8*)ptr;
	data[0] = (u8)(value >> 24u);
	data[1] = (u8)((value >> 16u) & 0xffu);
	data[2] = (u8)((value >> 8u) & 0xffu);
	data[3] = (u8)(value & 0xffu);
}

RGINLINE u64 rg_bin_load_u64_le(const void* ptr)
{
	const u8* data = (const u8*)ptr;
	return (u64)data[0] |
	       ((u64)data[1] << 8u) |
	       ((u64)data[2] << 16u) |
	       ((u64)data[3] << 24u) |
	       ((u64)data[4] << 32u) |
	       ((u64)data[5] << 40u) |
	       ((u64)data[6] << 48u) |
	       ((u64)data[7] << 56u);
}

RGINLINE u64 rg_bin_load_u64_be(const void* ptr)
{
	const u8* data = (const u8*)ptr;
	return ((u64)data[0] << 56u) |
	       ((u64)data[1] << 48u) |
	       ((u64)data[2] << 40u) |
	       ((u64)data[3] << 32u) |
	       ((u64)data[4] << 24u) |
	       ((u64)data[5] << 16u) |
	       ((u64)data[6] << 8u) |
	       (u64)data[7];
}

RGINLINE void rg_bin_store_u64_le(void* ptr, u64 value)
{
	u8* data = (u8*)ptr;
	data[0] = (u8)(value & 0xffu);
	data[1] = (u8)((value >> 8u) & 0xffu);
	data[2] = (u8)((value >> 16u) & 0xffu);
	data[3] = (u8)((value >> 24u) & 0xffu);
	data[4] = (u8)((value >> 32u) & 0xffu);
	data[5] = (u8)((value >> 40u) & 0xffu);
	data[6] = (u8)((value >> 48u) & 0xffu);
	data[7] = (u8)(value >> 56u);
}

RGINLINE void rg_bin_store_u64_be(void* ptr, u64 value)
{
	u8* data = (u8*)ptr;
	data[0] = (u8)(value >> 56u);
	data[1] = (u8)((value >> 48u) & 0xffu);
	data[2] = (u8)((value >> 40u) & 0xffu);
	data[3] = (u8)((value >> 32u) & 0xffu);
	data[4] = (u8)((value >> 24u) & 0xffu);
	data[5] = (u8)((value >> 16u) & 0xffu);
	data[6] = (u8)((value >> 8u) & 0xffu);
	data[7] = (u8)(value & 0xffu);
}
#endif

RGINLINE f32 rg_bin_load_f32_le(const void* ptr)
{
	u32 bits = rg_bin_load_u32_le(ptr);
	f32 value;
	memcpy(&value, &bits, sizeof(value));
	return value;
}

RGINLINE f32 rg_bin_load_f32_be(const void* ptr)
{
	u32 bits = rg_bin_load_u32_be(ptr);
	f32 value;
	memcpy(&value, &bits, sizeof(value));
	return value;
}

RGINLINE void rg_bin_store_f32_le(void* ptr, f32 value)
{
	u32 bits;
	memcpy(&bits, &value, sizeof(bits));
	rg_bin_store_u32_le(ptr, bits);
}

RGINLINE void rg_bin_store_f32_be(void* ptr, f32 value)
{
	u32 bits;
	memcpy(&bits, &value, sizeof(bits));
	rg_bin_store_u32_be(ptr, bits);
}

RGINLINE f64 rg_bin_load_f64_le(const void* ptr)
{
	u64 bits = rg_bin_load_u64_le(ptr);
	f64 value;
	memcpy(&value, &bits, sizeof(value));
	return value;
}

RGINLINE f64 rg_bin_load_f64_be(const void* ptr)
{
	u64 bits = rg_bin_load_u64_be(ptr);
	f64 value;
	memcpy(&value, &bits, sizeof(value));
	return value;
}

RGINLINE void rg_bin_store_f64_le(void* ptr, f64 value)
{
	u64 bits;
	memcpy(&bits, &value, sizeof(bits));
	rg_bin_store_u64_le(ptr, bits);
}

RGINLINE void rg_bin_store_f64_be(void* ptr, f64 value)
{
	u64 bits;
	memcpy(&bits, &value, sizeof(bits));
	rg_bin_store_u64_be(ptr, bits);
}
RGINLINE u8 rg_bin_read_u8(const u8** ptr)
{
	u8 value = **ptr;
	*ptr += 1;
	return value;
}

RGINLINE void rg_bin_write_u8(u8** ptr, u8 value)
{
	**ptr = value;
	*ptr += 1;
}

RGINLINE u16 rg_bin_read_u16_le(const u8** ptr)
{
	u16 value = rg_bin_load_u16_le(*ptr);
	*ptr += sizeof(u16);
	return value;
}

RGINLINE u16 rg_bin_read_u16_be(const u8** ptr)
{
	u16 value = rg_bin_load_u16_be(*ptr);
	*ptr += sizeof(u16);
	return value;
}

RGINLINE void rg_bin_write_u16_le(u8** ptr, u16 value)
{
	rg_bin_store_u16_le(*ptr, value);
	*ptr += sizeof(u16);
}

RGINLINE void rg_bin_write_u16_be(u8** ptr, u16 value)
{
	rg_bin_store_u16_be(*ptr, value);
	*ptr += sizeof(u16);
}

RGINLINE u32 rg_bin_read_u32_le(const u8** ptr)
{
	u32 value = rg_bin_load_u32_le(*ptr);
	*ptr += sizeof(u32);
	return value;
}

RGINLINE u32 rg_bin_read_u32_be(const u8** ptr)
{
	u32 value = rg_bin_load_u32_be(*ptr);
	*ptr += sizeof(u32);
	return value;
}

RGINLINE void rg_bin_write_u32_le(u8** ptr, u32 value)
{
	rg_bin_store_u32_le(*ptr, value);
	*ptr += sizeof(u32);
}

RGINLINE void rg_bin_write_u32_be(u8** ptr, u32 value)
{
	rg_bin_store_u32_be(*ptr, value);
	*ptr += sizeof(u32);
}

RGINLINE u64 rg_bin_read_u64_le(const u8** ptr)
{
	u64 value = rg_bin_load_u64_le(*ptr);
	*ptr += sizeof(u64);
	return value;
}

RGINLINE u64 rg_bin_read_u64_be(const u8** ptr)
{
	u64 value = rg_bin_load_u64_be(*ptr);
	*ptr += sizeof(u64);
	return value;
}

RGINLINE void rg_bin_write_u64_le(u8** ptr, u64 value)
{
	rg_bin_store_u64_le(*ptr, value);
	*ptr += sizeof(u64);
}

RGINLINE void rg_bin_write_u64_be(u8** ptr, u64 value)
{
	rg_bin_store_u64_be(*ptr, value);
	*ptr += sizeof(u64);
}

RGINLINE const u8* rg_bin_read_bytes(const u8** ptr, size_t size)
{
	const u8* out = *ptr;
	*ptr += size;
	return out;
}

RGINLINE void rg_bin_write_bytes(u8** ptr, const void* src, size_t size)
{
	memcpy(*ptr, src, size);
	*ptr += size;
}

RGINLINE u32 rg_bin_zigzag_encode_s32(i32 value)
{
	u32 uv = (u32)value;
	u32 mask = (u32)(value >> 31);
	return (uv << 1) ^ mask;
}

RGINLINE i32 rg_bin_zigzag_decode_s32(u32 value)
{
	return (i32)((value >> 1) ^ (u32) - (i32)(value & 1u));
}

RGINLINE u64 rg_bin_zigzag_encode_s64(i64 value)
{
	u64 uv = (u64)value;
	u64 mask = (u64)(value >> 63);
	return (uv << 1) ^ mask;
}

RGINLINE i64 rg_bin_zigzag_decode_s64(u64 value)
{
	return (i64)((value >> 1) ^ (u64) - (i64)(value & 1ull));
}

RGINLINE size_t rg_bin_uvarint_len_u32(u32 value)
{
	if (value < 0x80u)
	{
		return 1u;
	}
	if (value < 0x4000u)
	{
		return 2u;
	}
	if (value < 0x200000u)
	{
		return 3u;
	}
	if (value < 0x10000000u)
	{
		return 4u;
	}
	return 5u;
}

RGINLINE size_t rg_bin_uvarint_len_u64(u64 value)
{
	if (value < 0x80ull)
	{
		return 1u;
	}
	if (value < 0x4000ull)
	{
		return 2u;
	}
	if (value < 0x200000ull)
	{
		return 3u;
	}
	if (value < 0x10000000ull)
	{
		return 4u;
	}
	if (value < 0x800000000ull)
	{
		return 5u;
	}
	if (value < 0x40000000000ull)
	{
		return 6u;
	}
	if (value < 0x2000000000000ull)
	{
		return 7u;
	}
	if (value < 0x100000000000000ull)
	{
		return 8u;
	}
	if (value < 0x8000000000000000ull)
	{
		return 9u;
	}
	return 10u;
}

RGINLINE size_t rg_bin_svarint_len_s32(i32 value)
{
	return rg_bin_uvarint_len_u32(rg_bin_zigzag_encode_s32(value));
}

RGINLINE size_t rg_bin_svarint_len_s64(i64 value)
{
	return rg_bin_uvarint_len_u64(rg_bin_zigzag_encode_s64(value));
}

RGINLINE size_t rg_bin_write_uvarint_u32(u8* dst, u32 value)
{
	size_t count = 0;
	while (value >= 0x80u)
	{
		dst[count++] = (u8)((value & 0x7fu) | 0x80u);
		value >>= 7u;
	}
	dst[count++] = (u8)value;
	return count;
}

RGINLINE size_t rg_bin_write_uvarint_u64(u8* dst, u64 value)
{
	size_t count = 0;
	while (value >= 0x80ull)
	{
		dst[count++] = (u8)((value & 0x7full) | 0x80u);
		value >>= 7u;
	}
	dst[count++] = (u8)value;
	return count;
}

RGINLINE u32 rg_bin_read_uvarint_u32(const u8** ptr)
{
	const u8* data = *ptr;
	u32 value = 0;
	u32 shift = 0;
	for (u32 i = 0; i < RG_BIN_UVARINT32_MAX_BYTES; i++)
	{
		u8 byte = *data++;
		if (i == RG_BIN_UVARINT32_MAX_BYTES - 1u)
		{
			RG_BIN_ASSERT((byte & 0xf0u) == 0u);
		}
		value |= (u32)(byte & 0x7fu) << shift;
		if ((byte & 0x80u) == 0u)
		{
			*ptr = data;
			return value;
		}
		shift += 7u;
	}
	RG_BIN_ASSERT(0 && "rg_bin_read_uvarint_u32: malformed varint");
	*ptr = data;
	return value;
}

RGINLINE u64 rg_bin_read_uvarint_u64(const u8** ptr)
{
	const u8* data = *ptr;
	u64 value = 0;
	u32 shift = 0;
	for (u32 i = 0; i < RG_BIN_UVARINT64_MAX_BYTES; i++)
	{
		u8 byte = *data++;
		if (i == RG_BIN_UVARINT64_MAX_BYTES - 1u)
		{
			RG_BIN_ASSERT((byte & 0xfeu) == 0u);
		}
		value |= (u64)(byte & 0x7fu) << shift;
		if ((byte & 0x80u) == 0u)
		{
			*ptr = data;
			return value;
		}
		shift += 7u;
	}
	RG_BIN_ASSERT(0 && "rg_bin_read_uvarint_u64: malformed varint");
	*ptr = data;
	return value;
}

RGINLINE size_t rg_bin_write_svarint_s32(u8* dst, i32 value)
{
	return rg_bin_write_uvarint_u32(dst, rg_bin_zigzag_encode_s32(value));
}

RGINLINE size_t rg_bin_write_svarint_s64(u8* dst, i64 value)
{
	return rg_bin_write_uvarint_u64(dst, rg_bin_zigzag_encode_s64(value));
}

RGINLINE i32 rg_bin_read_svarint_s32(const u8** ptr)
{
	return rg_bin_zigzag_decode_s32(rg_bin_read_uvarint_u32(ptr));
}

RGINLINE i64 rg_bin_read_svarint_s64(const u8** ptr)
{
	return rg_bin_zigzag_decode_s64(rg_bin_read_uvarint_u64(ptr));
}
RGINLINE void rg_bin_reader_init(RgBinReader* reader, const void* data, size_t size)
{
	RG_BIN_ASSERT(reader != NULL);
	RG_BIN_ASSERT(data != NULL);
	reader->cur = (const u8*)data;
	reader->end = reader->cur + size;
}

RGINLINE size_t rg_bin_reader_remaining(const RgBinReader* reader)
{
	RG_BIN_ASSERT(reader != NULL);
	return (size_t)(reader->end - reader->cur);
}

RGINLINE const u8* rg_bin_reader_peek(const RgBinReader* reader, size_t size)
{
	RG_BIN_ASSERT(size <= rg_bin_reader_remaining(reader));
	RG_UNUSED(size);
	return reader->cur;
}

RGINLINE const u8* rg_bin_reader_take(RgBinReader* reader, size_t size)
{
	const u8* ptr = rg_bin_reader_peek(reader, size);
	reader->cur += size;
	return ptr;
}

RGINLINE void rg_bin_reader_skip(RgBinReader* reader, size_t size)
{
	RG_BIN_ASSERT(size <= rg_bin_reader_remaining(reader));
	reader->cur += size;
}

RGINLINE u8 rg_bin_reader_read_u8(RgBinReader* reader)
{
	RG_BIN_ASSERT(rg_bin_reader_remaining(reader) >= 1u);
	return rg_bin_read_u8(&reader->cur);
}

RGINLINE u16 rg_bin_reader_read_u16_le(RgBinReader* reader)
{
	const u8* ptr = rg_bin_reader_take(reader, sizeof(u16));
	return rg_bin_load_u16_le(ptr);
}

RGINLINE u16 rg_bin_reader_read_u16_be(RgBinReader* reader)
{
	const u8* ptr = rg_bin_reader_take(reader, sizeof(u16));
	return rg_bin_load_u16_be(ptr);
}

RGINLINE u32 rg_bin_reader_read_u32_le(RgBinReader* reader)
{
	const u8* ptr = rg_bin_reader_take(reader, sizeof(u32));
	return rg_bin_load_u32_le(ptr);
}

RGINLINE u32 rg_bin_reader_read_u32_be(RgBinReader* reader)
{
	const u8* ptr = rg_bin_reader_take(reader, sizeof(u32));
	return rg_bin_load_u32_be(ptr);
}

RGINLINE u64 rg_bin_reader_read_u64_le(RgBinReader* reader)
{
	const u8* ptr = rg_bin_reader_take(reader, sizeof(u64));
	return rg_bin_load_u64_le(ptr);
}

RGINLINE u64 rg_bin_reader_read_u64_be(RgBinReader* reader)
{
	const u8* ptr = rg_bin_reader_take(reader, sizeof(u64));
	return rg_bin_load_u64_be(ptr);
}

RGINLINE f32 rg_bin_reader_read_f32_le(RgBinReader* reader)
{
	const u8* ptr = rg_bin_reader_take(reader, sizeof(u32));
	return rg_bin_load_f32_le(ptr);
}

RGINLINE f32 rg_bin_reader_read_f32_be(RgBinReader* reader)
{
	const u8* ptr = rg_bin_reader_take(reader, sizeof(u32));
	return rg_bin_load_f32_be(ptr);
}

RGINLINE f64 rg_bin_reader_read_f64_le(RgBinReader* reader)
{
	const u8* ptr = rg_bin_reader_take(reader, sizeof(u64));
	return rg_bin_load_f64_le(ptr);
}

RGINLINE f64 rg_bin_reader_read_f64_be(RgBinReader* reader)
{
	const u8* ptr = rg_bin_reader_take(reader, sizeof(u64));
	return rg_bin_load_f64_be(ptr);
}

RGINLINE u32 rg_bin_reader_read_uvarint_u32(RgBinReader* reader)
{
	const u8* data = reader->cur;
	const u8* end = reader->end;
	RG_UNUSED(end);
	u32 value = 0;
	u32 shift = 0;
	for (u32 i = 0; i < RG_BIN_UVARINT32_MAX_BYTES; i++)
	{
		RG_BIN_ASSERT(data < end);
		u8 byte = *data++;
		if (i == RG_BIN_UVARINT32_MAX_BYTES - 1u)
		{
			RG_BIN_ASSERT((byte & 0xf0u) == 0u);
		}
		value |= (u32)(byte & 0x7fu) << shift;
		if ((byte & 0x80u) == 0u)
		{
			reader->cur = data;
			return value;
		}
		shift += 7u;
	}
	RG_BIN_ASSERT(0 && "rg_bin_reader_read_uvarint_u32: malformed varint");
	reader->cur = data;
	return value;
}

RGINLINE u64 rg_bin_reader_read_uvarint_u64(RgBinReader* reader)
{
	const u8* data = reader->cur;
	const u8* end = reader->end;
	RG_UNUSED(end);
	u64 value = 0;
	u32 shift = 0;
	for (u32 i = 0; i < RG_BIN_UVARINT64_MAX_BYTES; i++)
	{
		RG_BIN_ASSERT(data < end);
		u8 byte = *data++;
		if (i == RG_BIN_UVARINT64_MAX_BYTES - 1u)
		{
			RG_BIN_ASSERT((byte & 0xfeu) == 0u);
		}
		value |= (u64)(byte & 0x7fu) << shift;
		if ((byte & 0x80u) == 0u)
		{
			reader->cur = data;
			return value;
		}
		shift += 7u;
	}
	RG_BIN_ASSERT(0 && "rg_bin_reader_read_uvarint_u64: malformed varint");
	reader->cur = data;
	return value;
}

RGINLINE i32 rg_bin_reader_read_svarint_s32(RgBinReader* reader)
{
	return rg_bin_zigzag_decode_s32(rg_bin_reader_read_uvarint_u32(reader));
}

RGINLINE i64 rg_bin_reader_read_svarint_s64(RgBinReader* reader)
{
	return rg_bin_zigzag_decode_s64(rg_bin_reader_read_uvarint_u64(reader));
}

RGINLINE void rg_bin_writer_init(RgBinWriter* writer, void* data, size_t size)
{
	RG_BIN_ASSERT(writer != NULL);
	RG_BIN_ASSERT(data != NULL);
	writer->cur = (u8*)data;
	writer->end = writer->cur + size;
}

RGINLINE size_t rg_bin_writer_remaining(const RgBinWriter* writer)
{
	RG_BIN_ASSERT(writer != NULL);
	return (size_t)(writer->end - writer->cur);
}

RGINLINE u8* rg_bin_writer_reserve(RgBinWriter* writer, size_t size)
{
	RG_BIN_ASSERT(size <= rg_bin_writer_remaining(writer));
	u8* ptr = writer->cur;
	writer->cur += size;
	return ptr;
}

RGINLINE void rg_bin_writer_write_bytes(RgBinWriter* writer, const void* src, size_t size)
{
	u8* dst = rg_bin_writer_reserve(writer, size);
	memcpy(dst, src, size);
}

RGINLINE void rg_bin_writer_write_u8(RgBinWriter* writer, u8 value)
{
	RG_BIN_ASSERT(rg_bin_writer_remaining(writer) >= 1u);
	rg_bin_write_u8(&writer->cur, value);
}

RGINLINE void rg_bin_writer_write_u16_le(RgBinWriter* writer, u16 value)
{
	u8* dst = rg_bin_writer_reserve(writer, sizeof(u16));
	rg_bin_store_u16_le(dst, value);
}

RGINLINE void rg_bin_writer_write_u16_be(RgBinWriter* writer, u16 value)
{
	u8* dst = rg_bin_writer_reserve(writer, sizeof(u16));
	rg_bin_store_u16_be(dst, value);
}

RGINLINE void rg_bin_writer_write_u32_le(RgBinWriter* writer, u32 value)
{
	u8* dst = rg_bin_writer_reserve(writer, sizeof(u32));
	rg_bin_store_u32_le(dst, value);
}

RGINLINE void rg_bin_writer_write_u32_be(RgBinWriter* writer, u32 value)
{
	u8* dst = rg_bin_writer_reserve(writer, sizeof(u32));
	rg_bin_store_u32_be(dst, value);
}

RGINLINE void rg_bin_writer_write_u64_le(RgBinWriter* writer, u64 value)
{
	u8* dst = rg_bin_writer_reserve(writer, sizeof(u64));
	rg_bin_store_u64_le(dst, value);
}

RGINLINE void rg_bin_writer_write_u64_be(RgBinWriter* writer, u64 value)
{
	u8* dst = rg_bin_writer_reserve(writer, sizeof(u64));
	rg_bin_store_u64_be(dst, value);
}

RGINLINE void rg_bin_writer_write_f32_le(RgBinWriter* writer, f32 value)
{
	u8* dst = rg_bin_writer_reserve(writer, sizeof(u32));
	rg_bin_store_f32_le(dst, value);
}

RGINLINE void rg_bin_writer_write_f32_be(RgBinWriter* writer, f32 value)
{
	u8* dst = rg_bin_writer_reserve(writer, sizeof(u32));
	rg_bin_store_f32_be(dst, value);
}

RGINLINE void rg_bin_writer_write_f64_le(RgBinWriter* writer, f64 value)
{
	u8* dst = rg_bin_writer_reserve(writer, sizeof(u64));
	rg_bin_store_f64_le(dst, value);
}

RGINLINE void rg_bin_writer_write_f64_be(RgBinWriter* writer, f64 value)
{
	u8* dst = rg_bin_writer_reserve(writer, sizeof(u64));
	rg_bin_store_f64_be(dst, value);
}

RGINLINE void rg_bin_writer_write_uvarint_u32(RgBinWriter* writer, u32 value)
{
	size_t len = rg_bin_uvarint_len_u32(value);
	RG_BIN_ASSERT(rg_bin_writer_remaining(writer) >= len);
	RG_UNUSED(len);
	writer->cur += rg_bin_write_uvarint_u32(writer->cur, value);
}

RGINLINE void rg_bin_writer_write_uvarint_u64(RgBinWriter* writer, u64 value)
{
	size_t len = rg_bin_uvarint_len_u64(value);
	RG_BIN_ASSERT(rg_bin_writer_remaining(writer) >= len);
	RG_UNUSED(len);
	writer->cur += rg_bin_write_uvarint_u64(writer->cur, value);
}

RGINLINE void rg_bin_writer_write_svarint_s32(RgBinWriter* writer, i32 value)
{
	rg_bin_writer_write_uvarint_u32(writer, rg_bin_zigzag_encode_s32(value));
}

RGINLINE void rg_bin_writer_write_svarint_s64(RgBinWriter* writer, i64 value)
{
	rg_bin_writer_write_uvarint_u64(writer, rg_bin_zigzag_encode_s64(value));
}

#endif // RG_BIN_H
