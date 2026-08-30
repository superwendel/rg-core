# rg_time

`rg_time.h` provides high-resolution monotonic timestamps, duration
conversions, sleeping, and thread yielding. It depends only on
[`rg_defs.h`](rg_defs.md).

## Setup

Include the header wherever timing helpers are used:

```c
#include "rg_time.h"

int main(void)
{
  rg_time_init();
  // Start worker threads and run the application.
}
```

All functions have internal linkage. The header can be included directly in
unity builds or separately compiled C and C++ translation units.

Native backends are included for Windows, Linux, and macOS. Other platforms
can provide the four custom hooks described below.

Call `rg_time_init` once before starting worker threads. It caches the invariant
platform frequency so subsequent conversions are arithmetic-only. Because the
header has internal linkage, separately compiled translation units that need
the cached path must each call `rg_time_init`; a unity build has one cache.
Calls made without initialization remain correct but query the platform
frequency each time.

## Measuring time

Use raw ticks when measuring a duration, then convert the difference:

```c
u64 start = rg_time_ticks();

update_world();

u64 elapsed_ticks = rg_time_ticks() - start;
f64 elapsed_ms = rg_time_ticks_to_ms(elapsed_ticks);
```

`rg_time_ticks_per_second` returns the platform frequency. Conversion helpers
are available for seconds, milliseconds, and microseconds in both directions.
If a backend reports a zero frequency, conversions return zero.

Floating-point durations that are negative, zero, or NaN convert to zero
ticks. Values beyond the `u64` tick range saturate to `UINT64_MAX`.

The convenience functions `rg_time_seconds`, `rg_time_ms`, and `rg_time_us`
return the current monotonic time in the requested unit. These are elapsed-time
values with an unspecified origin, not civil or Unix timestamps.

## Sleeping and yielding

```c
rg_time_sleep_ns(500000);
rg_time_sleep_us(500);
rg_time_sleep_ms(1);
rg_time_yield();
```

Sleep durations are requests to the operating system and may complete later
because of scheduler granularity. The Windows backend rounds nonzero
nanosecond requests up to the next millisecond. Linux and macOS retry an
interrupted `nanosleep` with its remaining duration.

Microsecond-to-nanosecond conversion saturates rather than wrapping. A zero
duration returns without calling the backend.

## Custom backend

Define `RG_TIME_CUSTOM` as `1`, provide functions matching the native hook
signatures, and name them before including the header:

```c
#include "rg_defs.h"

static u64 platform_ticks(void);
static u64 platform_frequency(void);
static void platform_sleep_ns(u64 ns);
static void platform_yield(void);

#define RG_TIME_CUSTOM 1
#define RG_TIME_PLATFORM_TICKS platform_ticks
#define RG_TIME_PLATFORM_FREQUENCY platform_frequency
#define RG_TIME_PLATFORM_SLEEP_NS platform_sleep_ns
#define RG_TIME_PLATFORM_YIELD platform_yield
#include "rg_time.h"
```

The frequency is the number of custom ticks per second. Custom timestamp and
frequency functions must return `u64`; sleep accepts nanoseconds, and yield
takes no arguments.

## Thread safety

The native implementation has one translation-unit-local frequency cache.
Timing, sleep, and yield functions can be called concurrently after
`rg_time_init` completes. The cache is read-only after initialization; do not
initialize it concurrently with conversion calls. Calls that never initialize
the cache remain thread-safe and query the native backend each time. A custom
backend is responsible for providing equivalent synchronization guarantees.
