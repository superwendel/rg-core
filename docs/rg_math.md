# rg_math

`rg_math` is a header-only C99 math library for game and realtime code. It
provides scalar helpers, vectors, matrices, quaternions, camera transforms,
curves, geometry queries, debug printing, and Perlin noise.

## Performance

![rg_math hot-path benchmark results](benchmarks/rg_math-hot-path.svg)

The upper panel compares `rg_math` with [cglm 0.9.6](https://github.com/recp/cglm)
across representative vector, matrix, and quaternion operations. Each value is
the median elapsed time in milliseconds for 1,000,000 calls, so lower is
better.

| Operation | rg_math | cglm |
| --- | ---: | ---: |
| `vec3_add` | 0.39 ms | 0.39 ms |
| `vec3_dot` | 0.92 ms | 1.24 ms |
| `vec3_length` | 1.27 ms | 1.30 ms |
| `vec3_normalize` | 2.05 ms | 2.53 ms |
| `vec4_add` | 1.01 ms | 1.00 ms |
| `mat4_mul` | 0.38 ms | 2.56 ms |
| `mat4_mulv4` | 2.20 ms | 2.21 ms |
| `quat_mul` | 0.40 ms | 0.40 ms |
| `quat_normalize` | 2.57 ms | 3.51 ms |

The lower panel summarizes a CPU-side 3D-engine profile covering transform
updates, animation, physics, and culling. It is a weighted model rather than a
trace from one application:

| Area | Operations and weights |
| --- | --- |
| Vec3 (45) | add 8, scale 8, dot 10, length 7, normalize 10, cross 2 |
| Mat4 (20) | multiply 8, matrix-vector multiply 12 |
| Quaternion (15) | multiply 9, normalize 6 |
| Vec4 (10) | add 3, scale 3, dot 2, length 1, normalize 1 |

The weighted score is the sum of each median multiplied by its weight, divided
by the 90-point shared-profile total. Only libraries with a measurement for
every operation are shown:

| Library | Weighted median | Coverage |
| --- | ---: | ---: |
| `rg_math` | 1.14 ms | 100% |
| [DirectXMath 3.19](https://github.com/microsoft/DirectXMath) | 1.48 ms | 100% |
| [cglm 0.9.6](https://github.com/recp/cglm) | 1.50 ms | 100% |
| [GLM 1.1.0](https://github.com/g-truc/glm) | 3.65 ms | 100% |

The current public headers and all comparison libraries were built into the
same C++17 benchmark executable with MSVC 19.44.35217 for x64 using `/O2 /Ob3
/Oi /Ot /Oy /GL /LTCG /arch:AVX2 /fp:fast /GS- /DNDEBUG`. The default
`RG_MATH_MAX_PERF=1`, `RG_MATH_USE_LIBC=1`, and `RG_MATH_LIBC_ALIASES=1`
configuration was used. Each benchmark group ran in a fresh process; the
figure reports the median of seven runs after a 10,000-call warmup per
operation.

Measurements were taken on an AMD Ryzen 9 4900HS on Windows build 26200.9168.
HandmadeMath is not shown because the development harness intentionally
configures it without SIMD. The development harness and comparison-library
sources are not distributed in this repository. Results are machine-specific
and should not be treated as a performance guarantee.

## Usage

Include the umbrella header for the complete library:

```c
#include "src/rg_math.h"

rg_vec3 position;
rg_vec3 velocity;
rg_vec3 next;

rg_vec3_set(&position, 1.0f, 2.0f, 3.0f);
rg_vec3_set(&velocity, 4.0f, 0.0f, -1.0f);
rg_vec3_add(&position, &velocity, &next);
```

All functions have internal linkage, so there is no implementation macro or
separate library to link. The headers use the `f32`, `f64`, and fixed-width
integer aliases from `rg_defs.h`.

## Modules

The umbrella always includes the scalar, vector, and matrix base. The remaining
modules can also be included directly:

- `rg_math_scalar.h` — scalar math, conversions, interpolation, and approximations.
- `rg_math_vec.h` — floating-point and integer vectors.
- `rg_math_mat.h` — column-major matrices, transforms, projection, and decomposition.
- `rg_math_cam.h` — camera, projection, and frustum helpers.
- `rg_math_quat.h` — quaternion operations and conversions.
- `rg_math_euler.h` — ordered Euler-angle conversions.
- `rg_math_color.h` — linear RGB calculations.
- `rg_math_curve.h` — Bezier, spline, and easing helpers.
- `rg_math_geom.h` — geometric primitives, bounds, and intersections.
- `rg_math_io.h` — configurable debug printing.
- `rg_math_noise.h` — periodic and non-periodic Perlin noise.

To trim the umbrella, define any of these before including `rg_math.h`:

```c
#define RG_MATH_NO_CAM
#define RG_MATH_NO_QUAT
#define RG_MATH_NO_EULER
#define RG_MATH_NO_COLOR
#define RG_MATH_NO_CURVE
#define RG_MATH_NO_GEOM
#define RG_MATH_NO_IO
#define RG_MATH_NO_NOISE
#include "src/rg_math.h"
```

`RG_MATH_NO_QUAT` also omits Euler helpers, which depend on quaternions.
`RG_MATH_NO_GEOM` also omits IO helpers, whose public API includes geometry
types. These switches affect only the umbrella header; direct module includes
still include their required dependencies.

## SIMD and portability

On x86 and x64, compiler-enabled SSE, SSE4.1, FMA, and AVX paths are selected at
compile time. The library does not perform runtime CPU dispatch, so the emitted
instructions follow the compiler target. For example, `/arch:AVX2` enables the
corresponding optimized paths in an MSVC build and requires an AVX2-capable
machine at runtime.

Other architectures use the scalar implementation. Define
`RG_MATH_NO_SIMD` to force that implementation on any target:

```c
#define RG_MATH_NO_SIMD
#include "src/rg_math.h"
```

SIMD vector, quaternion, and matrix types are 16-byte aligned. The explicitly
named `*_fast` matrix functions may impose stronger alignment or input
preconditions; see their declarations before using them with external storage.

## Performance and safety modes

`RG_MATH_MAX_PERF` defaults to `1`. This selects the library's fastest paths,
including approximate reciprocal square root for normalization, and removes
some zero-length and validity checks. In this mode:

- Pass normalized axes, normals, and quaternions to APIs that document that requirement.
- Initialize `rg_vec3` values with `rg_vec3_set` or explicitly initialize `_pad`
  before relying on SIMD loads.
- Treat `*_fast` functions as unchecked operations with documented preconditions.
- Do not use `rg_vec*_isnan`, `rg_vec*_isinf`, or `rg_vec*_isvalid` as validation;
  max-performance builds intentionally avoid those checks.

For checked normalization and validity paths, configure the library before its
first include:

```c
#define RG_MATH_MAX_PERF 0
#include "src/rg_math.h"
```

The setting is translation-unit-wide and must be consistent wherever math
types and inline functions are used.

Additional configuration:

- `RG_MATH_ASSERT(expression)` replaces the assertion hook.
- `RG_MATH_USE_LIBC` controls use of `<math.h>` fallbacks.
- `RG_MATH_LIBC_ALIASES` allows direct macro wrappers when libc is enabled.
- `RG_MATH_FAST_MATH`, `RG_MATH_FAST_TRIG`, and `RG_MATH_FAST_EXP` opt into
  lower-precision approximations for the corresponding scalar wrappers.
- `RG_MATH_RSQRT_MODE` selects reciprocal-square-root implementation details.
- `RG_MATH_VEC3_PLAIN` and `RG_MATH_VEC4_PLAIN` omit the `__m128` union member
  while retaining an aligned four-component array layout.

Boolean numeric options accept only `0` or `1`; invalid configuration values
produce a compile-time error.

## Coordinate conventions

Matrices are column-major. Projection helpers default to right-handed clip
space with an NDC depth range of `[-1, 1]`:

```c
#define RG_MATH_CLIP_CONTROL RG_MATH_CLIP_CONTROL_RH_NO
#include "src/rg_math.h"
```

The other supported values are `RG_MATH_CLIP_CONTROL_RH_ZO`,
`RG_MATH_CLIP_CONTROL_LH_NO`, and `RG_MATH_CLIP_CONTROL_LH_ZO`. Explicit
`_rh_no`, `_rh_zo`, `_lh_no`, and `_lh_zo` variants are available when a call
site should not depend on the translation-unit default.

```c
rg_vec3 eye = rg_vec3(0.0f, 2.0f, 5.0f);
rg_vec3 target = rg_vec3(0.0f, 0.0f, 0.0f);
rg_vec3 up = rg_vec3(0.0f, 1.0f, 0.0f);
rg_mat4 view;
rg_mat4 projection;

rg_mat4_look_at(&eye, &target, &up, &view);
rg_mat4_perspective(RG_PI / 3.0f, 16.0f / 9.0f, 0.1f, 1000.0f,
                    &projection);
```

## Debug printing

`rg_math_io.h` supplies print helpers for math and geometry types. Printing is
enabled in debug builds and compiles to no-ops when `NDEBUG` is defined. Define
`RG_MATH_DEFINE_PRINTS` to enable it in a release build or
`RG_MATH_PRINT_NO_COLOR` to disable ANSI color sequences.

When printing is enabled, `rg_math_io.h` uses `rg_sprintf_hybrid.h`. Define
`RG_MATH_IO_NO_SPRINTF_INCLUDE` only when a compatible `rg_sprintf` API was
already included earlier in the translation unit.

## Build verification

From a Visual Studio Developer Command Prompt:

```bat
build.bat test_math
```

The target checks baseline x64 SIMD, AVX2, checked SIMD and scalar,
plain-layout, C++17, and reduced-umbrella configurations. Tests exercise
representative functionality from every module. Published benchmark results
are documentation artifacts and are not part of the test target.
