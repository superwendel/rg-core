# rg_bin

`rg_bin.h` provides endian-aware binary loads and stores, pointer-advancing
cursors, bounded reader and writer cursors, and variable-length integer
encoding. It depends only on [`rg_defs.h`](../src/rg_defs.h), performs no
allocation, and keeps all functions at internal linkage.

## Setup

Include the header directly:

```c
#include "rg_bin.h"

u8 bytes[8];
rg_bin_store_u32_le(bytes, UINT32_C(0x11223344));
u32 value = rg_bin_load_u32_le(bytes);
```

The header uses the `u8`, `u16`, `u32`, `u64`, `i32`, `i64`, `f32`, and `f64`
aliases from `rg_defs.h`. It has a normal include guard and can be included
repeatedly in conventional or unity builds.

## Primitive loads and stores

Unsigned 8-, 16-, 32-, and 64-bit integers can be loaded or stored in explicit
little- or big-endian byte order. The one-byte operations do not need an
endianness suffix.

```c
u16 rg_bin_load_u16_le(const void* ptr);
u16 rg_bin_load_u16_be(const void* ptr);
u32 rg_bin_load_u32_le(const void* ptr);
u32 rg_bin_load_u32_be(const void* ptr);
u64 rg_bin_load_u64_le(const void* ptr);
u64 rg_bin_load_u64_be(const void* ptr);

void rg_bin_store_u32_le(void* ptr, u32 value);
void rg_bin_store_u32_be(void* ptr, u32 value);
```

Equivalent `f32` and `f64` functions preserve the floating-point bit pattern
while converting its byte order. Byte-swap helpers are also available for
unsigned 16-, 32-, and 64-bit values.

The default implementation uses `memcpy` on a detected little-endian target
and explicit byte assembly on other targets. Both paths accept unaligned byte
addresses without violating C aliasing rules.

## Pointer-advancing cursors

The lightweight cursor helpers read or write a value and advance the supplied
pointer:

```c
u8 buffer[32];
u8* write = buffer;

rg_bin_write_u16_le(&write, UINT16_C(0x1234));
rg_bin_write_u32_be(&write, UINT32_C(0x11223344));

const u8* read = buffer;
u16 first = rg_bin_read_u16_le(&read);
u32 second = rg_bin_read_u32_be(&read);
```

`rg_bin_read_bytes` returns the current byte span before advancing.
`rg_bin_write_bytes` copies a byte span and advances. These raw cursor helpers
do not know the buffer bounds; the caller must provide enough readable or
writable memory.

## Bounded readers and writers

`RgBinReader` and `RgBinWriter` track the end of a caller-owned buffer:

```c
u8 buffer[64];
RgBinWriter writer;
rg_bin_writer_init(&writer, buffer, sizeof(buffer));

rg_bin_writer_write_u32_le(&writer, UINT32_C(0x12345678));
rg_bin_writer_write_uvarint_u32(&writer, 300u);

size_t used = sizeof(buffer) - rg_bin_writer_remaining(&writer);
RgBinReader reader;
rg_bin_reader_init(&reader, buffer, used);

u32 fixed = rg_bin_reader_read_u32_le(&reader);
u32 compact = rg_bin_reader_read_uvarint_u32(&reader);
```

The reader also provides `peek`, `take`, and `skip` operations. The writer
provides `reserve` for direct access to the next output span. Capacity failures
call `RG_BIN_ASSERT`; the API does not return recoverable error codes.

## Variable-length integers

Unsigned variable-length integers use ULEB128. Signed values are first mapped
with zigzag encoding, keeping small positive and negative magnitudes compact.

```c
u8 encoded[RG_BIN_UVARINT64_MAX_BYTES];
size_t length = rg_bin_write_svarint_s64(encoded, -123);

const u8* read = encoded;
i64 value = rg_bin_read_svarint_s64(&read);
```

Length helpers report the encoded size without writing:

```c
size_t rg_bin_uvarint_len_u32(u32 value);
size_t rg_bin_uvarint_len_u64(u64 value);
size_t rg_bin_svarint_len_s32(i32 value);
size_t rg_bin_svarint_len_s64(i64 value);
```

A `u32` uses at most `RG_BIN_UVARINT32_MAX_BYTES` bytes and a `u64` uses at
most `RG_BIN_UVARINT64_MAX_BYTES` bytes.

## Configuration

Define options before the first include:

```c
#define RG_BIN_ASSERT(condition)    // Custom assertion macro
#define RG_BIN_LITTLE_ENDIAN 1      // Override detected host endianness
#define RG_BIN_FAST_UNALIGNED 1     // Opt into direct unaligned access
#include "rg_bin.h"
```

`RG_BIN_LITTLE_ENDIAN` and `RG_BIN_FAST_UNALIGNED` accept only `0` or `1`.
Endianness is otherwise detected from compiler macros and the platform and
architecture definitions in `rg_defs.h`.

`RG_BIN_FAST_UNALIGNED` affects little-endian integer loads and stores. It uses
direct typed pointer access and should be enabled only when the target permits
unaligned access and the project accepts its effective-type and aliasing
requirements. The portable `memcpy` path remains the default.

## Input safety

The library is designed for validated binary layouts and performance-sensitive
internal formats. Bounds and malformed-varint checks are assertions; the
default C assertion disappears when `NDEBUG` is defined. Validate untrusted
input before reading it or place a fallible parser in front of these helpers.

## Build verification

From a Visual Studio Developer Command Prompt:

```bat
build.bat test_bin
```

The target runs the public API suite against the default portable path, the
opt-in direct unaligned path, the bytewise path, and a C++17 build.
