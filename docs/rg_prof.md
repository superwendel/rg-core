# rg_prof

`rg_prof.h` records CPU scopes, instant events, and frame markers into fixed
per-thread buffers. It is intended for always-available development
instrumentation in a unity-built game, with all recording compiled away unless
`RG_PROF_ENABLED` is `1`.

It depends on [`rg_mem.h`](rg_mem.md) for arena storage and
[`rg_time.h`](rg_time.md) for the monotonic platform timer.

## Setup

Define the switch before the first include:

```c
#define RG_PROF_ENABLED 1
#include "rg_prof.h"
```

Create the profiler and register thread handles during setup. Registration is
not concurrent and deliberately touches each event buffer once so the first
captured frame does not absorb demand-zero page costs.

```c
RgArena prof_arena = rg_arena_create(MB(8));

RgProf prof;
if (!rg_prof_init(&prof, &prof_arena, 4, 16384))
{
  // Not enough arena memory or no timer frequency.
}

RgProfThread* main_thread = rg_prof_register_thread(&prof, "main");
rg_prof_set_thread(main_thread);
```

`rg_prof_init_default` uses eight thread slots and 16,384 events for each
thread that is actually registered. Names are borrowed pointers, so thread,
scope, event, and history-section names must outlive their recorded data.

## Recording

The usual path stores the current `RgProfThread` in thread-local storage:

```c
rg_prof_thread_reset(main_thread);
RG_PROF_FRAME_BEGIN("Frame");

RG_PROF_SCOPE("Update")
{
  update_game();
}

RG_PROF_EVENT("Enemy spawned");
RG_PROF_FRAME_END("Frame");
```

Each OS thread calls `rg_prof_set_thread` once with its own registered handle.
After that, recording writes only that thread's state and event buffer; there
are no locks or atomics on the hot path. Thread states occupy separate cache
lines so workers do not bounce their event counters through the same line.

The `_ON` forms take a thread handle directly and avoid the TLS lookup:

```c
RG_PROF_SCOPE_ON(worker_prof, "Physics")
{
  simulate_physics();
}

RG_PROF_EVENT_ON(worker_prof, "Island complete");
```

The direct forms are useful inside an especially hot worker loop. The regular
macros are normally clearer, and the timer calls dominate a timed scope.

`RG_PROF_SCOPE` and `RG_PROF_SCOPE_ON` close correctly when their body uses
`break` or `continue`, but those statements apply to the scope macro's loop;
they do not break or continue an enclosing application loop. Portable C cannot
provide automatic cleanup across `return` or `goto`. When any of those exits
must target surrounding control flow, use `rg_prof_scope_begin_on` and call
`rg_prof_scope_end` explicitly before leaving.

## Capture modes

For a live overlay or rolling statistics, reset a thread before each frame,
record one frame, then consume it while that thread is idle. Reset only rewinds
the counters; it does not clear the event memory.

For a multi-frame trace, do not reset every frame. Record until the fixed
buffer is full, stop the producing threads, consume or export the events, and
then reset. Once full, new events are skipped and `thread->dropped` is
incremented.

`rg_prof_iterate_events` visits registered threads in registration order and
events in recording order. The caller is responsible for synchronizing reset
and consumption with the producing threads.

## Optional frame history

`RgProfHistory` adds latest-frame and rolling-average telemetry without adding
fields or branches to an event record. It is an optional post-frame consumer.
The caller chooses the scope names to aggregate:

```c
const char* sections[] = {"Simulation", "Render", "GPU wait"};
RgProfHistory history;

rg_prof_history_init(&history, &prof_arena, 4096,
                     sections, RG_ARRAY_COUNT(sections));

// After RG_PROF_FRAME_END, while main_thread is idle:
rg_prof_history_capture(&history, main_thread);

const RgProfFrameSample* latest = rg_prof_history_latest(&history);
f32 average_frame_ms = rg_prof_history_average_frame_ms(&history, 60);
f32 average_render_ms = rg_prof_history_average_section_ms(&history, 1, 60);
```

Capture finds the most recent complete frame-marker pair in the thread buffer.
It returns zero when there is no new complete frame, so an unfinished frame or
repeated call cannot duplicate the previous sample.
Matching scope durations are summed into the configured section. Matching is
pointer-fast when names are shared and falls back to string comparison during
the post-frame scan. A sample count of zero means all retained samples.

The history borrows the section-name array and strings. Its frame and section
storage comes from the supplied arena. A 4,096-frame history with four sections
uses about 128 KiB. Profiler event buffers remain separately sized for trace
detail.

## Disabled builds

With the default `RG_PROF_ENABLED 0`, recording macros do not evaluate their
arguments and scoped bodies still execute exactly once. Setup functions remain
as inexpensive stubs, which lets application code keep profiler setup in all
build configurations.

## Performance

The public-release pass reduced `RgProfEvent` from 40 bytes to 32 bytes on x64
by moving frame numbering into the optional history. A default registered
thread therefore uses 512 KiB for events instead of 640 KiB. Thread state is
cache-line isolated, and event storage is pre-touched during registration.

Seven fresh-process runs of the optimized MSVC x64 lab benchmark produced
these representative medians:

| Operation | Median time |
| --- | ---: |
| Timer call | 21.35 ns |
| Instant event through TLS | 22.38 ns |
| Timed scope through TLS | 43.86 ns |
| Fixed-section baseline | 44.81 ns |
| `rg_prof` empty frame | 43.82 ns |
| Fixed four-section frame baseline | 52.73 ns |
| Post-frame history capture | 12.65 ns |
| Two-worker events, isolated state | 12.37 ns/event |
| Two-worker events, packed state | 24.70 ns/event |

The fixed-section comparison uses the same `rg_time`/QPC clock so the test
measures profiler design rather than timer-wrapper differences. It directly
accumulates one of four fixed section totals, while `rg_prof` records a named,
nested event suitable for trace export. The packed-state comparison uses the
same event recorder with adjacent 48-byte worker states and demonstrates the
false-sharing cost that the cache-line slots avoid.

These are microbenchmarks, not guarantees. Timer latency and scheduling vary
by processor and operating system; the important result is that the richer
event path did not regress from the lab version and remains slightly faster
than the fixed-section path in the same executable.
