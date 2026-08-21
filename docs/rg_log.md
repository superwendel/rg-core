# rg_log

`rg_log.h` is a small, unity-friendly logger for C. It adds severity filtering,
source locations, optional color and timestamps, and uses `rg_sprintf` for
message formatting.

## Integration

```c
#include "rg_log.h"

int main(void)
{
  rg_log_init(RG_LOG_DEBUG);
  RG_INFO("Server started on port %d", 8080);
  return 0;
}
```

The header includes `rg_sprintf_hybrid.h`. On MSVC x64 AVX2 builds, link the
Windows assembly helper described in the [rg_sprintf documentation](rg_sprintf.md),
or define `RG_SPRINTF_NO_ASM` to use the portable path.

The header intentionally has no include guard and should be included once in a
unity translation unit. Its `static` state is then shared by every source file
in that unity build. Separately compiled translation units each receive their
own logger state.

## Configuration

Define options before including the header:

```c
#define RG_LOG_TIMESTAMP        // Add an HH:MM:SS timestamp
#define RG_LOG_STRIP_EXTENSION  // Show main instead of main.c
#define RG_LOG_NO_COLOR         // Disable ANSI color sequences
#define RG_LOG_BUFFER_SIZE 1024 // Default: 512 bytes
#include "rg_log.h"
```

Messages that exceed the buffer are truncated while retaining a final newline
and null terminator.

## API

```c
void rg_log_init(RgLogLevel level);
void rg_log_set_level(RgLogLevel level);
RgLogLevel rg_log_get_level(void);

RG_DEBUG("frame=%u", frame);
RG_INFO("Loaded %s", path);
RG_WARN("Frame spike: %.2f ms", elapsed_ms);
RG_ERROR("Could not open %s", path);
RG_CRIT("Out of memory");
```

Levels range from `RG_LOG_DEBUG` through `RG_LOG_CRIT`; `RG_LOG_NONE` disables
all messages. Filtered macro calls do not evaluate their formatting arguments.

## Output

The default format is:

```text
[INFO] game.c:42: Loaded player.mesh
```

DEBUG and INFO write to buffered `stdout`. WARN, ERROR, and CRIT write to
`stderr`; ERROR and CRIT explicitly flush their stream. ANSI colors are enabled
by default and can be disabled for files or CI with `RG_LOG_NO_COLOR`.

## Assertions

Include `rg_assert.h` after `rg_log.h` to route assertion reports through
`RG_CRIT` automatically:

```c
#include "rg_log.h"
#include "rg_assert.h"

RG_ASSERT_MSG(ptr != NULL, "Allocation failed");
```
