# rg_sprintf

`rg_sprintf.h` is a single-header C formatter intended for common game and
realtime workloads. It is not a promise of complete libc `printf`
compatibility for every locale or formatting edge case.

## Integration

For the fastest supported implementation, include the hybrid header:

```c
#include "rg_sprintf_hybrid.h"
```

On MSVC x64 AVX2 builds, the hybrid header selects `rg_sprintf_asm.h`. Compile
and link `src/asm/sprintf/win_x64/rg_sprintf_asm_x64.asm` in that configuration.
Linux x64 builds can compile `src/asm/sprintf/linux_x64/rg_sprintf_asm_x64.S`
and define `RG_SPRINTF_HAS_ASM`.

Use the portable implementation directly when no assembly object is desired:

```c
#include "rg_sprintf.h"
```

Define configuration macros before inclusion when needed:

```c
#define RG_SPRINTF_NO_SIMD  // Disable SIMD hexadecimal encoding
#define RG_SPRINTF_NO_ASM   // Force the hybrid header to select portable C
#define RG_SPRINTF_SECURE   // Enable argument checks and parser limits
#define RG_SPRINTF_ASSERT(x) custom_assert(x)
```

`RG_SPRINTF_HYBRID_FORCE_C` and `RG_SPRINTF_HYBRID_FORCE_ASM` provide explicit
selection. A forced assembly build must link the appropriate platform helper.

## Formatting API

```c
int rg_sprintf(char* buf, const char* fmt, ...);
int rg_snprintf(char* buf, size_t count, const char* fmt, ...);
int rg_vsprintf(char* buf, const char* fmt, va_list args);
int rg_vsnprintf(char* buf, size_t count, const char* fmt, va_list args);
```

`rg_snprintf` and `rg_vsnprintf` return the number of characters that would
have been written, excluding the terminator.

Callback output is available through `rg_sprintf_cb` and `rg_vsprintf_cb`.
Callbacks receive temporary chunks and must consume or copy them before
returning.

## Direct conversion API

- `rg_itoa`, `rg_utoa`, `rg_i64toa`, and `rg_u64toa`
- `rg_ftoa` and `rg_dtoa`
- `rg_to_hex` and `rg_from_hex`

The numeric-to-string functions return a pointer to the terminating null byte.

## String builder

`RgBuilder` provides bounded append operations for strings, characters,
integers, floats, hexadecimal data, and formatted text. Appends truncate to the
provided capacity while preserving null termination when capacity is nonzero.

## Performance

![rg_sprintf benchmark results](benchmarks/rg_sprintf-vs-stb.svg)

The figure compares [`stb_sprintf` 1.10](https://github.com/nothings/stb/blob/master/stb_sprintf.h),
the portable `rg_sprintf` header, and `rg_sprintf_asm.h` linked with the MASM
x64 helper. It reports the median of seven process runs. Each process was
pinned to one logical CPU and raised to high priority, warmed each
implementation with 1,000,000 calls per case, and then took the median of
three 10,000,000-call samples.

All three implementations were built into the same binary with MSVC
19.44.35217 for x64 using `/O2 /Ob3 /Oi /Ot /Oy /GL /LTCG /arch:AVX2
/fp:fast /GS- /DNDEBUG`. Measurements were taken on an AMD Ryzen 9 4900HS on
Windows build 26200.9168 on August 21, 2026, using the public source snapshot
in this repository.

The cases cover `%d`, `%08u`, `%x`, `%lld`, `%.6f`, `%.6e`, `%g`, `%s`, and a
mixed game-style status string containing a name, two integers, and two
floating-point values. They do not use the experimental corpus from
development, and the library contains no complete-format dispatch for these
cases. Values are nanoseconds per call, so lower is better. Results are
machine-specific and should not be treated as a performance guarantee.

## Optimization approach

`rg_sprintf` optimizes general formatting operations such as integer,
floating-point, and string conversion. It does not contain special cases for
specific application messages or benchmark format strings.
