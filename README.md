# rg_core by Reverse Gravity

`rg_core` is a set of header-only C modules designed for unity builds. It
provides memory arenas, containers, hashing, sorting, binary I/O, strings,
math, timing, logging, assertions, formatting, and a small SDL3 foundation.
These are the foundational modules I use to make games at Reverse Gravity.

`rg_core` is not intended to be a general-purpose answer for every use case. It
is the code I use to make games, released because its focused approach
outperformed many popular libraries in the workloads I measured.

Functions use internal linkage, so there is no implementation macro or
separate C library target. Stateful modules have one state per translation
unit. A unity build therefore gives the program one shared state;
conventionally compiled translation units each receive their own state.

Add `rg_core/src` to the compiler include path and include only the modules you
use. Most modules depend only on the C standard library. `rg_sdl.h`,
`rg_window.h`, `rg_input.h`, and `rg_gpu.h` require SDL3. The hybrid formatter
can optionally use an x64 assembly object.

## Quick start

This example opens a resizable SDL3 window and exits when you close it or press
Escape. Install an [SDL3 development package](https://github.com/libsdl-org/SDL/releases)
before building it. SDL3 is only required by the SDL modules; the other modules
remain independently usable.

```c
#include "rg_input.h"
#include "rg_log.h"
#include "rg_sdl.h"
#include "rg_window.h"

int main(void)
{
	rg_log_init(RG_LOG_INFO);

	if (!rg_sdl_init(SDL_INIT_VIDEO))
	{
		RG_ERROR("SDL initialization failed: %s", rg_sdl_error());
		return 1;
	}

	RgWindowDesc desc = {0};
	desc.title = "rg_core quickstart";
	desc.width = 1280;
	desc.height = 720;
	desc.flags = SDL_WINDOW_RESIZABLE;

	SDL_Window* window = rg_window_create(&desc);
	if (!window)
	{
		RG_ERROR("Window creation failed: %s", rg_sdl_error());
		rg_sdl_quit();
		return 1;
	}

	RgInputState input;
	rg_input_init(&input);

	int running = 1;
	while (running)
	{
		rg_input_update(&input);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			rg_input_process_event(&input, &event);

			if (event.type == SDL_EVENT_QUIT ||
			    event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
			{
				running = 0;
			}
		}

		if (rg_input_is_key_pressed(&input, SDL_SCANCODE_ESCAPE))
		{
			running = 0;
		}

		// Update and render your game here.
	}

	rg_window_destroy(window);
	rg_sdl_quit();
	return 0;
}
```

MSVC (from a Visual Studio Developer Command Prompt):

```bat
cl /nologo /std:c11 /W4 /O2 ^
   /I path\to\rg_core\src /I "%SDL3_DIR%\include" example.c ^
   /link /LIBPATH:"%SDL3_DIR%\lib\x64" SDL3.lib
set "PATH=%SDL3_DIR%\lib\x64;%PATH%"
example.exe
```

Set `SDL3_DIR` to the SDL3 development package root before building. Its
`lib\x64` directory must contain both `SDL3.lib` and `SDL3.dll`.

GCC or Clang (with SDL3 available through `pkg-config`):

```sh
cc -std=c99 -Wall -Wextra -O2 \
   -Ipath/to/rg_core/src $(pkg-config --cflags sdl3) \
   example.c -o example $(pkg-config --libs sdl3)
./example
```

See the [`rg_sdl`](docs/rg_sdl.md), [`rg_window`](docs/rg_window.md),
[`rg_input`](docs/rg_input.md), and [`rg_log`](docs/rg_log.md) guides for the
complete APIs.

## Modules

### Foundation

- [`rg_defs.h`](docs/rg_defs.md) — primitive aliases, compiler/platform
  detection, alignment utilities, branch hints, and shared definitions.
- [`rg_assert.h`](docs/rg_assert.md) — configurable assertions, ensures, and
  panic helpers with optional logging integration.
- [`rg_mem.h`](docs/rg_mem.md) — OS-backed global memory pool and typed bump
  arenas with alignment, reset, and optional secure clearing.
- [`rg_time.h`](docs/rg_time.md) — monotonic timing, tick conversion, sleeping,
  and yielding with native and custom platform backends.
- [`rg_prof.h`](docs/rg_prof.md) — lock-free per-thread CPU scopes and events,
  with optional rolling frame and named-section history.
- [`rg_log.h`](docs/rg_log.md) — severity-based logging with source locations,
  optional color and timestamps, and `rg_sprintf` formatting.

### Data and algorithms

- [`rg_bin.h`](docs/rg_bin.md) — endian-aware loads and stores, bounded cursors,
  and ULEB128/zigzag variable-length integers.
- [`rg_string.h`](docs/rg_string.md) — in-place string utilities, bounded
  replacement and joining, UTF-8 helpers, and arena-backed strings.
- [`rg_containers.h`](docs/rg_containers.md) — typed dynamic arrays, inline
  small vectors, power-of-two ring buffers, and sparse sets.
- [`rg_hash.h`](docs/rg_hash.md) — deterministic 64-bit hashing and typed
  robin-hood hash maps and sets.
- [`rg_algo.h`](docs/rg_algo.md) — typed sorting, selection, searching, min/max,
  and stable 32/64-bit radix sorting.
- [`rg_random.h`](docs/rg_random.md) — deterministic xoshiro256** generation,
  unbiased ranges, shuffling, byte filling, and probability distributions.

### Math

- [`rg_math.h`](docs/rg_math.md) — SIMD-accelerated scalar, vector, matrix,
  quaternion, camera, curve, geometry, color, and noise helpers with scalar
  fallbacks.

### Formatting

- [`rg_sprintf.h`](docs/rg_sprintf.md) — portable formatting, direct numeric
  conversion, callback output, and a bounded string builder.
- [`rg_sprintf_hybrid.h`](docs/rg_sprintf.md#integration) — selects the optional
  x64 assembly implementation when its platform helper is linked.

### SDL3

- [`rg_sdl.h`](docs/rg_sdl.md) — SDL application lifetime and error access.
- [`rg_window.h`](docs/rg_window.md) — window creation and display controls.
- [`rg_input.h`](docs/rg_input.md) — immediate input state and ordered events.
- [`rg_gpu.h`](docs/rg_gpu.md) — GPU device setup, uploads, and compiled shaders.

## Performance

| Workload | rg_core time | Compared with | Comparison time | Speedup |
| --- | ---: | --- | ---: | ---: |
| Mixed formatting | 109.42 ns/call | `stb_sprintf` 1.10 | 306.33 ns/call | **2.8x** |
| Shared 3D hot-path model | 1.14 ms | cglm 0.9.6 | 1.50 ms | **1.3x** |
| One million random `int32`, radix sort | 15.38 ms | `std::sort` | 89.63 ms | **5.8x** |
| 500,000-entry reserved map insert | 25.81 ms | `std::unordered_map` | 79.91 ms | **3.1x** |
| 200,000 sparse-set inserts | 0.38 ms | EnTT 3.13.2 | 2.01 ms | **5.3x** |
| 200,000 sparse-set removals | 0.20 ms | EnTT 3.13.2 | 1.70 ms | **8.5x** |

Lower time is better and speedup is comparison time divided by `rg_core` time;
results are machine- and workload-specific, so see the complete
[benchmark reports](docs/benchmarks/README.md) for charts and methodology.

## Build, test, and benchmark

From a Visual Studio Developer Command Prompt:

```bat
build.bat test
```

The aggregate target exercises the portable, SIMD, assembly, secure, custom,
configured, and C++ compatibility builds. When SDL3 is available, it also runs
the SDL foundation and input suites; set `SDL3_DIR` to select an SDL3
development package explicitly.

Reproduce the documented three-process benchmark medians with:

```bat
build.bat bench_median
```

Use `build.bat bench` for a quicker one-process diagnostic pass. Set
`RG_BENCH_DEPS` to enable the optional quadsort, crumsort, `stb_ds`, and EnTT
comparisons described in the benchmark reports.

## Inspiration

The [Handmade community](https://handmade.network/) has shaped this project
through its emphasis on simple, direct, understandable software and deliberate
control over the systems a game depends on.

`rg_sprintf` was inspired by Sean Barrett's
[`stb_sprintf`](https://github.com/nothings/stb). It targets the common game and
real-time formatting workloads where a smaller focused formatter can go faster,
with a portable C fallback and optional x64 assembly acceleration.

## License and trademark

The software and documentation are available under the [MIT License](LICENSE).
Reverse Gravity is a registered trademark of Steven Wendel in the United
States. The license grants rights to the software and documentation, but not
to the Reverse Gravity name or trademark except to identify the origin of this
software. See the
[USPTO trademark record](https://tmsearch.uspto.gov/search/search-results/86371513)
(Serial No. `86371513`, Registration No. `4805325`).
