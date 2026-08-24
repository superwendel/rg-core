# rg_random

`rg_random.h` is a single-header xoshiro256** random generator with uniform
sampling, array helpers, and common probability distributions. It depends only
on [`rg_defs.h`](../src/rg_defs.h).

It is intended for deterministic gameplay, simulation, procedural content, and
tools. It is not a cryptographically secure random generator.

## Setup

Create and seed an explicit RNG state:

```c
#include "src/rg_random.h"

RgRng rng;
rg_rng_seed(&rng, 1234);

u32 value = rg_random_u32(&rng);
f32 unit = rg_random_f32(&rng);
```

The header contains normal include guards and all functions are `static inline`,
so it can be included repeatedly and works naturally in unity builds.

## Generator state

```c
typedef struct RgRng
{
    u64 state[4];
    u32 has_spare;
    f32 spare;
} RgRng;
```

`rg_rng_seed` expands one 64-bit seed through splitmix64. A zero seed is valid.
`rg_rng_seed_state` accepts an explicit four-word state; an all-zero state is
changed to `{1, 0, 0, 0}` because xoshiro256** cannot advance from all zeroes.
Both seeding operations reset the built-in normal-distribution cache.

```c
void rg_rng_seed(RgRng* rng, u64 seed);
void rg_rng_seed_state(RgRng* rng, const u64 seed_state[4]);
u64 rg_rng_next_u64(RgRng* rng);
u32 rg_rng_next_u32(RgRng* rng);
```

## Uniform sampling

```c
u32 rg_random_u32(RgRng* rng);
u64 rg_random_u64(RgRng* rng);
f32 rg_random_f32(RgRng* rng);     // [0, 1)
f64 rg_random_f64(RgRng* rng);     // [0, 1)

u32 rg_random_bounded_u32(RgRng* rng, u32 bound);
u64 rg_random_bounded_u64(RgRng* rng, u64 bound);
```

The bounded functions sample without modulo bias and return values in
`[0, bound)`. A zero bound returns zero without consuming RNG state.

Integer ranges are inclusive at both ends:

```c
u32 rg_random_range_u32(RgRng* rng, u32 min, u32 max);
u64 rg_random_range_u64(RgRng* rng, u64 min, u64 max);
i32 rg_random_range_i32(RgRng* rng, i32 min, i32 max);
i64 rg_random_range_i64(RgRng* rng, i64 min, i64 max);
```

Floating-point ranges use the conventional half-open target interval:

```c
f32 rg_random_range_f32(RgRng* rng, f32 min, f32 max);
f64 rg_random_range_f64(RgRng* rng, f64 min, f64 max);
```

The source uniform value is below one. As with the usual multiply-and-add range
formula, floating-point rounding can produce the upper endpoint for some large
or closely spaced inputs.

Additional helpers provide random booleans, signs, Fisher-Yates shuffling, and
byte filling:

```c
int rg_random_bool(RgRng* rng);
int rg_random_sign(RgRng* rng);
void rg_random_shuffle(void* data, size_t count, size_t stride, RgRng* rng);
void rg_random_fill_bytes(void* data, size_t size, RgRng* rng);
```

## Distributions

```c
void rg_random_normal2_f32(RgRng* rng, f32 mean, f32 stddev,
                           f32* out0, f32* out1);
f32 rg_random_normal_f32(RgRng* rng, f32 mean, f32 stddev);

typedef struct RgNormalCache
{
    u32 has_spare;
    f32 spare;
} RgNormalCache;

void rg_random_normal_cache_reset(RgNormalCache* cache);
f32 rg_random_normal_f32_cached(RgRng* rng, RgNormalCache* cache,
                                f32 mean, f32 stddev);
f32 rg_random_exponential_f32(RgRng* rng, f32 lambda);
```

`rg_random_normal_f32` stores the second Box-Muller sample in `RgRng`.
`rg_random_normal_f32_cached` uses a separate cache when independent cached
streams are needed. Standard deviation and exponential lambda must be positive.

## Determinism

The xoshiro engine, integer helpers, bounded ranges, shuffling, and byte filling
produce the same sequence across supported targets for the same initial state.
The implementation uses equivalent native and portable 64-by-64-bit
multiplication paths so bounded 64-bit sampling does not change algorithms on
platforms without a native 128-bit integer.

The `f32` and `f64` uniform helpers assume ordinary IEEE-754 binary floating
point.
Normal and exponential sampling call the platform math library, so their final
floating-point bits are not guaranteed to match across different math-library
implementations.

## Configuration

Define options before including the header:

```c
#define RG_RANDOM_ASSERT(condition)       // Custom assertion macro
#define RG_RANDOM_SEED                    // Caller-facing default; default: 0
#define RG_RANDOM_FORCE_PORTABLE_MUL128   // Disable native 128-bit multiply
```

`RG_RANDOM_SEED` does not create or seed global state; it is a convenient
project-wide default for caller code.

## Thread safety

The library has no mutable global state. Separate `RgRng` instances may be used
concurrently, but one instance must not be advanced by multiple threads without
external synchronization.

## Attribution

xoshiro256** and splitmix64 were designed by David Blackman and Sebastiano
Vigna. Their reference implementations are released into the public domain.
