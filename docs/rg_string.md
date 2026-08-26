# rg_string

`rg_string.h` provides in-place ASCII string operations, replacement,
splitting and joining, UTF-8 helpers, and an arena-backed `RgString`. It depends
on [`rg_mem.h`](../src/rg_mem.h), which supplies the shared primitive types and
arena allocator.

## Setup

Include the header wherever the functions are used:

```c
#include "src/rg_string.h"
```

All functions have internal linkage. The header can be included directly in
unity builds or separately compiled C and C++ translation units.

`RgString` additionally requires an initialized `rg_mem` pool and an arena:

```c
rg_malloc(MB(16));
RgArena strings = rg_arena_create(MB(8));

RgString message;
rgs_init_with(&message, &strings, "entity");
rgs_cat(&message, " spawned");

rg_arena_free(&strings);
rg_free();
```

The arena must outlive every `RgString` backed by it.

## String utilities

| Operation | Function |
| --- | --- |
| Trim ASCII whitespace | `rg_trim`, `rg_ltrim`, `rg_rtrim` |
| Convert ASCII case | `rg_strlower`, `rg_strlower_n`, `rg_strupper` |
| Reverse bytes | `rg_strrev` |
| Test prefix or suffix | `rg_startswith`, `rg_endswith` |
| Count non-overlapping matches | `rg_strcount` |
| Replace the first or every match | `rg_replace`, `rg_replace_all` |
| Split or join fields | `rg_split`, `rg_join` |

Trim, case conversion, reversal, and splitting modify their input buffers.
Whitespace follows the ASCII set: space, tab, newline, carriage return,
vertical tab, and form feed. Case conversion changes only `A-Z` or `a-z`;
bytes from 128 through 255 pass through unchanged.

`rg_strlower_n` processes exactly the supplied number of bytes, so it avoids a
terminator scan and continues through embedded null bytes. It does not append a
terminator. When AVX2 is enabled for the compilation target, ranges of 32 bytes
or more use the vector path; shorter ranges and portable builds use the scalar
path.

`rg_split` replaces every delimiter with a null terminator and stores pointers
to non-empty fields. Empty fields, including leading and trailing fields, are
skipped. Once `max_parts` pointers have been stored, remaining delimiters are
still replaced.

## Bounded output and length queries

`rg_replace`, `rg_replace_all`, and `rg_join` always return the required result
length excluding the null terminator. When the destination has space, output is
null-terminated; truncation does not change the returned length.

Pass a null destination and a size of zero to query the required capacity:

```c
size_t len = rg_replace_all(NULL, 0, source, "${name}", player_name);
char* result = RG_ARENA_PUSH_ARRAY(&scratch, char, len + 1);
rg_replace_all(result, len + 1, source, "${name}", player_name);
```

Destination buffers must not overlap the source, match, replacement,
separator, or part strings. `rg_join` treats a null separator or null part
pointer as an empty string.

## UTF-8 helpers

- `rg_utf8_len` counts code-point lead bytes. It does not validate the input.
- `rg_utf8_len_n` performs the same count over an explicit byte length. It
  processes embedded null bytes and does not validate the input.
- `rg_utf8_decode` validates and decodes one code point, returning the number
  of consumed bytes or zero for an invalid sequence or the null terminator.
- `rg_utf8_encode` writes one code point to a caller-provided four-byte buffer.
  It returns the number of bytes written and does not add a terminator.
- `rg_utf8_valid` validates a complete null-terminated string, rejecting
  overlong encodings, surrogate code points, truncated input, and values above
  `U+10FFFF`.

`rg_strrev`, `rg_strlower`, and `rg_strupper` operate on bytes, not Unicode
graphemes or locale-aware text.

## RgString

`RgString` stores a null-terminated buffer plus its length and capacity:

```c
typedef struct RgString
{
    RgArena* arena;
    char* data;
    size_t len;
    size_t cap;
} RgString;
```

Use `rgs_init` or `rgs_init_with` before other operations. `rgs_copy_n` and
`rgs_cat_n` avoid a source-length scan when the byte length is already known.
Their length-aware input may contain embedded null bytes; an additional null is
always written at `data[len]`.

Growth uses a roughly 1.5x capacity policy with a minimum allocation of 16
bytes. Because storage comes from a bump arena, growth allocates a new buffer
and leaves the old one in the arena. This also makes self-copy and self-append
safe. Reserve suitable capacity up front when repeatedly extending a string.

Length-aware operations can use the stored fields directly without rescanning:

```c
rg_strlower_n(message.data, message.len);
size_t codepoints = rg_utf8_len_n(message.data, message.len);
```

`rgs_clear` keeps capacity for reuse. `rgs_free` only releases the handle and
does not reclaim arena storage; call `rg_arena_reset` or `rg_arena_free` when
all strings using that arena are no longer needed. Reinitialize a handle after
`rgs_free` before using it again.

Allocation failure leaves the existing string unchanged.

## Configuration

Define options before including the header:

```c
#define RG_STRING_SECURE
#define RG_STRING_ASSERT(condition) my_assert(condition)
#include "src/rg_string.h"
```

`RG_STRING_SECURE` adds null-argument handling. Without it, null arguments are
outside the API contract except for documented length queries, null join
separators and parts, and optional null sources accepted by `RgString`
copy/append operations.

The explicit-length lowercase and UTF-8 length functions automatically use
AVX2 intrinsics when the compiler target defines AVX2 support, such as MSVC's
`/arch:AVX2` or GCC and Clang's `-mavx2`. The vector loop begins at 32 bytes,
the crossover selected by repeated scalar-versus-AVX2 benchmarks. Define
`RG_STRING_NO_SIMD` to force the portable scalar implementation. Selection is
compile-time; the header does not perform runtime CPU detection.

## Thread safety

The lookup tables and empty-string sentinel are translation-unit-local and are
not mutated by normal API use. Independent buffers and arenas can be used on
different threads. Sharing a mutable buffer, `RgString`, or arena requires
caller synchronization.
