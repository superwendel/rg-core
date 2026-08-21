# rg_assert

`rg_assert.h` provides configurable assertion, ensure, and panic helpers for
C. It is independent of the logger but can report through `rg_log` when both
headers are used.

## Behavior

- `RG_ASSERT` reports and terminates when its condition fails. It is disabled
  by default when `NDEBUG` is defined and does not evaluate its condition then.
- `RG_ENSURE` always evaluates its condition and returns `1` or `0`. Failed
  ensures report only when ensure reporting is enabled.
- `RG_PANIC` always reports and terminates.

```c
#include "rg_assert.h"

RG_ASSERT(ptr != NULL);
RG_ASSERT_MSG(ptr != NULL, "Allocation failed for %zu bytes", size);

if (!RG_ENSURE_MSG(count > 0, "Count must be positive"))
  return;

RG_PANIC_MSG("Unreachable state %d", state);
```

## Configuration

Define options before including the header:

```c
#define RG_ASSERT_ENABLED 1
#define RG_ENSURE_ENABLED 1
#define RG_ASSERT_HANDLER my_assert_handler
#define RG_ASSERT_BREAK() my_debug_break()
#define RG_ASSERT_ABORT() my_abort()
#define RG_ASSERT_MESSAGE_BUFFER_SIZE 1024
#include "rg_assert.h"
```

`RG_ASSERT_HANDLER` receives the expression, source file, line number, and
formatted message:

```c
typedef void (*RgAssertHandler)(const char* expression,
                                const char* file,
                                int line,
                                const char* message);
```

Without a custom handler, reports are written to `stderr`. Include `rg_log.h`
first to make the default handler use `RG_CRIT` instead.
