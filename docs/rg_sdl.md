# rg_sdl

`rg_sdl.h` is the small SDL3 lifetime layer used by the rest of the Reverse
Gravity ecosystem. It initializes SDL, shuts it down, and exposes SDL's error
string without adding another error system.

```c
#include "rg_sdl.h"

if (!rg_sdl_init(SDL_INIT_VIDEO))
{
	return 1;
}

// Application lifetime...

rg_sdl_quit();
```

SDL3 is an external dependency. Set `SDL3_DIR` to the root of an SDL3
development package when running the SDL test targets on Windows.
