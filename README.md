# rg-core

Small, portable C libraries for game and realtime applications.

## Available libraries

- `rg_log.h` — lightweight severity-based logging with source locations,
  optional color and timestamps, and `rg_sprintf` formatting.
- `rg_assert.h` — configurable assertion, ensure, and panic helpers with
  optional `rg_log` integration.
- `rg_sprintf_hybrid.h` — selects the assembly-accelerated formatter when its
  platform helper is linked and otherwise uses the portable implementation.
- `rg_sprintf.h` — the portable formatter, covering the common `printf` subset
  used by games plus direct numeric conversion, hexadecimal conversion,
  callback output, and a bounded string builder.

## Inspiration

`rg_sprintf` was inspired by Sean Barrett's excellent
[`stb_sprintf`](https://github.com/nothings/stb). I set out to build a formatter
that could go faster on common game and realtime workloads, with a portable C
fallback and optional x64 assembly acceleration.

## Performance

![rg_sprintf benchmark results](docs/benchmarks/rg_sprintf-vs-stb.svg)

These measurements compare `stb_sprintf` 1.10 with the portable C and x64
assembly implementations of `rg_sprintf`. See the
[benchmark methodology](docs/rg_sprintf.md#performance) for the test cases,
build configuration, and hardware details. Results are machine-specific.

## Build and test

From a Visual Studio Developer Command Prompt:

```bat
build.bat test
```

The test target builds portable AVX2, scalar, hybrid assembly, hybrid C
fallback, and secure formatter configurations, then runs the logging and
assertion suites. On Windows x64, it assembles and links the included MASM
helper automatically.

## Usage

```c
#include "src/rg_sprintf_hybrid.h"

char buffer[128];
rg_snprintf(buffer, sizeof(buffer), "entity=%u position=(%.2f, %.2f)",
            entity_id, x, y);
```

All functions are `static`, making the headers safe to include in unity builds
without an implementation toggle.
