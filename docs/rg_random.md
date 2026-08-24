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

uint32_t value = rg_random_u32(&rng);
float unit = rg_random_f32(&rng);
```

The header contains normal include guards and all functions are `static inline`,
so it can be included repeatedly and works naturally in unity builds.

## Generator state

```c
typedef struct RgRng
{
    uint64_t state[4];
    uint32_t has_spare;
    float spare;
} RgRng;
```

`rg_rng_seed` expands one 64-bit seed through splitmix64. A zero seed is valid.
`rg_rng_seed_state` accepts an explicit four-word state; an all-zero state is
changed to `{1, 0, 0, 0}` because xoshiro256** cannot advance from all zeroes.
Both seeding operations reset the built-in normal-distribution cache.

```c
void rg_rng_seed(RgRng* rng, uint64_t seed);
void rg_rng_seed_state(RgRng* rng, const uint64_t seed_state[4]);
uint64_t rg_rng_next_u64(RgRng* rng);
uint32_t rg_rng_next_u32(RgRng* rng);
```

## Uniform sampling

```c
uint32_t rg_random_u32(RgRng* rng);
uint64_t rg_random_u64(RgRng* rng);
float rg_random_f32(RgRng* rng);     // [0, 1)
double rg_random_f64(RgRng* rng);    // [0, 1)

uint32_t rg_random_bounded_u32(RgRng* rng, uint32_t bound);
uint64_t rg_random_bounded_u64(RgRng* rng, uint64_t bound);
```

The bounded functions sample without modulo bias and return values in
`[0, bound)`. A zero bound returns zero without consuming RNG state.

Integer ranges are inclusive at both ends:

```c
uint32_t rg_random_range_u32(RgRng* rng, uint32_t min, uint32_t max);
uint64_t rg_random_range_u64(RgRng* rng, uint64_t min, uint64_t max);
int32_t rg_random_range_i32(RgRng* rng, int32_t min, int32_t max);
int64_t rg_random_range_i64(RgRng* rng, int64_t min, int64_t max);
```

Floating-point ranges use the conventional half-open target interval:

```c
float rg_random_range_f32(RgRng* rng, float min, float max);
double rg_random_range_f64(RgRng* rng, double min, double max);
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
void rg_random_normal2_f32(RgRng* rng, float mean, float stddev,
                           float* out0, float* out1);
float rg_random_normal_f32(RgRng* rng, float mean, float stddev);

typedef struct RgNormalCache
{
    uint32_t has_spare;
    float spare;
} RgNormalCache;

void rg_random_normal_cache_reset(RgNormalCache* cache);
float rg_random_normal_f32_cached(RgRng* rng, RgNormalCache* cache,
                                  float mean, float stddev);
float rg_random_exponential_f32(RgRng* rng, float lambda);
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

The float uniform helpers assume ordinary IEEE-754 binary floating point.
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
