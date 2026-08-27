# rg_window

`rg_window.h` provides the window operations used by Reverse Gravity
applications: descriptor-based creation, size constraints, fullscreen and
border controls, drawable sizing, content scale, and relative mouse mode.

```c
#include "rg_window.h"

RgWindowDesc desc = {0};
desc.title = "Game";
desc.width = 1280;
desc.height = 720;
desc.flags = SDL_WINDOW_RESIZABLE;

SDL_Window* window = rg_window_create(&desc);
```

SDL initialization remains the caller's responsibility. Include `rg_sdl.h`
and initialize `SDL_INIT_VIDEO` before creating a window.
