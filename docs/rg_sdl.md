# rg_sdl

`rg_sdl.h` is the small SDL3 lifetime layer used by the other SDL-facing
`rg_core` modules. It initializes SDL, shuts it down, and exposes SDL's error
string without adding another error system.

## Setup and lifetime

Initialize the subsystems the application needs before creating windows,
reading input, or creating a GPU device:

```c
#include "rg_sdl.h"

int main(void)
{
	if (!rg_sdl_init(SDL_INIT_VIDEO))
	{
		// rg_sdl_error() describes the failure.
		return 1;
	}

	// Create SDL resources and run the application.

	rg_sdl_quit();
	return 0;
}
```

Destroy windows, GPU devices, and other SDL resources before calling
`rg_sdl_quit`. The wrapper owns no resources or error storage of its own; it
delegates to SDL's process-wide lifetime and error state.

## API

| Function | Behavior |
| --- | --- |
| `rg_sdl_init(flags)` | Calls `SDL_Init` and returns `1` on success or `0` on failure. |
| `rg_sdl_quit()` | Calls `SDL_Quit`. |
| `rg_sdl_error()` | Returns SDL's current error string. The pointer is owned by SDL. |
| `rg_sdl_clear_error()` | Clears SDL's current error string. |

Call `rg_sdl_error` immediately after a failed SDL or `rg_core` SDL-wrapper
operation. A successful call does not necessarily clear an older error; use
`rg_sdl_clear_error` when code needs to distinguish newly reported errors.

## Building with SDL3

SDL3 is an external dependency. Add both `rg_core/src` and the SDL3 include
directory to the compiler search path, link SDL3, and make its shared library
available when the application starts. The repository
[quickstart](../README.md#quick-start) contains complete MSVC and GCC/Clang
commands.

On Windows, `build.bat test_sdl` and `build.bat test_input` look for an SDL3
development package through `SDL3_DIR`. The directory must contain
`include/SDL3/SDL.h` and `lib/x64/SDL3.lib`; `SDL3.dll` must also be discoverable
when the tests run.

All wrapper functions have internal linkage. The SDL library itself still owns
its global lifetime, so initialize and shut it down from the application's
coordinating thread.
