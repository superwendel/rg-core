# rg_defs

`rg_defs.h` supplies the primitive aliases, compiler/platform/architecture
detection, alignment utilities, branch hints, size helpers, and portability
macros used throughout `rg_core`. It has no state and depends only on
`<stddef.h>` and `<stdint.h>`.

```c
#include "rg_defs.h"

u32 entity_count = 0u;
f32 delta_seconds = 1.0f / 60.0f;
size_t arena_size = MB(64);
```

Other modules include `rg_defs.h` as needed, which is why their examples can
use aliases such as `u32`, `f32`, and helpers such as `MB()` directly.

## Primitive aliases

| Alias | Definition |
| --- | --- |
| `u8`, `u16`, `u32`, `u64` | Exact-width unsigned integers from `<stdint.h>` |
| `i8`, `i16`, `i32`, `i64` | Exact-width signed integers from `<stdint.h>` |
| `f32`, `f64` | `float` and `double` |
| `b32` | A 32-bit signed integer used for zero/nonzero boolean values |

The exact-width integer aliases are available only where the corresponding
standard `<stdint.h>` types exist.

## Compiler detection

Exactly one recognized compiler macro is `1`; the others are `0`:

| Macro | Detected compiler |
| --- | --- |
| `RG_COMPILER_MSVC` | Microsoft Visual C/C++ |
| `RG_COMPILER_GCC` | GCC |
| `RG_COMPILER_CLANG` | Clang, including clang-cl |

Clang is tested before GCC and MSVC because it can define compatibility macros
for either frontend. On an unrecognized compiler, all three macros are `0`.

## Platform and architecture detection

The platform macros are `RG_PLATFORM_WINDOWS`, `RG_PLATFORM_LINUX`, and
`RG_PLATFORM_MACOS`. The architecture macros are `RG_ARCH_X64`, `RG_ARCH_X86`,
and `RG_ARCH_ARM64`. A recognized target sets one macro in its group to `1` and
the rest to `0`. An unknown platform or architecture leaves every macro in that
group at `0`.

These are compile-time target checks, not runtime operating-system or CPU
feature detection. Individual modules may support fewer targets or require a
custom backend even though `rg_defs.h` itself compiles.

## Internal linkage and inlining

`RGINLINE` expands to a force-inline request with `static` linkage on MSVC,
GCC, and Clang. On an unknown compiler it becomes standard `static inline`.
The compiler may still choose not to inline a function; the important API
guarantee is internal linkage.

`RG_NOINLINE` requests that a cold function not be inlined on recognized
compilers and expands to nothing elsewhere.

## Alignment

| Macro | Purpose |
| --- | --- |
| `RG_ALIGNOF(type)` | Required alignment of a type |
| `RG_ALIGN_UP(value, align)` | Round a byte value upward |
| `RG_ALIGN_DOWN(value, align)` | Round a byte value downward |
| `RG_IS_POWER_OF_2(value)` | Test for a nonzero power of two |
| `RG_IS_ALIGNED(ptr, align)` | Test pointer alignment |

`RG_ALIGN_UP`, `RG_ALIGN_DOWN`, and `RG_IS_ALIGNED` require a nonzero,
power-of-two alignment. `RG_ALIGN_UP` additionally requires that adding
`align - 1` does not overflow `size_t`. Treat their arguments as expressions
without side effects because macros may evaluate an argument more than once.

`RG_ALIGNOF` uses C11 `_Alignof` when available, otherwise the recognized
compiler extension. Its unknown-compiler fallback is `sizeof(type)`, which is
not a reliable report of the type's actual alignment and may not be a power of
two. Define `RG_ALIGNOF` before inclusion when porting the alignment utilities
to an unrecognized compiler.

## Size helpers

`KB(x)`, `MB(x)`, and `GB(x)` convert binary kibibyte, mebibyte, and gibibyte
counts to `size_t` using multipliers of 1024, 1024 squared, and 1024 cubed.
For example, `MB(64)` is suitable for a 64 MiB arena size. The result must fit
both `unsigned long long` during multiplication and the target `size_t`.

`RG_CACHE_LINE_SIZE` defaults to 64 bytes and can be defined before inclusion
when a project has a different compile-time assumption. It is not detected at
runtime.

## Branch prediction

`RG_LIKELY(condition)` and `RG_UNLIKELY(condition)` provide branch-probability
hints on GCC and Clang. They evaluate to the condition without a hint on MSVC
and unknown compilers. Define both macros before inclusion to provide a custom
implementation.

## Thread-local and restrict behavior

`RG_THREAD_LOCAL` selects the compiler's thread-local storage extension on
MSVC, GCC, and Clang. On an unknown compiler it expands to nothing, so a
declaration becomes ordinary storage rather than thread-local storage.

`RG_RESTRICT` selects the recognized compiler's restrict extension. It expands
to nothing on an unknown compiler. As with standard `restrict`, using it is a
promise that the relevant accesses do not alias; violating that promise can
produce undefined behavior after optimization.

## General utilities

- `RG_ARRAY_COUNT(array)` returns the number of elements in an actual array.
  Passing a pointer produces a size ratio, not an element count.
- `RG_MIN`, `RG_MAX`, and `RG_CLAMP` evaluate arguments once on GCC and Clang
  by using statement expressions. The portable/MSVC forms can evaluate an
  argument more than once, so do not pass expressions with side effects.
- `RG_UNUSED(value)` suppresses unused-variable and unused-parameter warnings.
- `RG_DEBUG_BREAK()` emits a debugger trap on recognized compilers and becomes
  a no-op on an unknown compiler.

Most utility macros are guarded with `#ifndef` and may be customized before
including the header. Compiler, platform, and architecture detection macros
are reset by the header and reflect the detected build target.

## Unknown environments

On an unknown compiler, platform, or architecture, detection macros are all
zero and `rg_defs.h` favors compilable fallbacks: ordinary `static inline`, no
branch or no-inline hints, no thread-local or restrict qualifier, a no-op debug
break, and the alignment fallback described above. This does not imply that
every other `rg_core` module supports that environment. Check the module's
platform requirements and provide its documented custom backend where one is
available.
