# rg_window

`rg_window.h` provides the window operations used by Reverse Gravity
applications: descriptor-based creation, size constraints, fullscreen and
border controls, drawable sizing, content scale, and relative mouse mode.

## Setup and lifetime

Initialize SDL's video subsystem before creating a window, check the returned
pointer, and destroy the window before shutting SDL down:

```c
#include "rg_sdl.h"
#include "rg_window.h"

int main(void)
{
	if (!rg_sdl_init(SDL_INIT_VIDEO))
		return 1;

	RgWindowDesc desc = {0};
	desc.title = "Game";
	desc.width = 1280;
	desc.height = 720;
	desc.flags = SDL_WINDOW_RESIZABLE;
	desc.min_width = 640;
	desc.min_height = 360;

	SDL_Window* window = rg_window_create(&desc);
	if (!window)
	{
		rg_sdl_quit();
		return 1;
	}

	// Run the application.

	rg_window_destroy(window);
	rg_sdl_quit();
	return 0;
}
```

`rg_window_create` uses `"rg_window"` when `title` is null and clamps a
nonpositive width or height to one. It applies a minimum or maximum constraint
only when both dimensions in that pair are positive. Flags are passed directly
to `SDL_CreateWindow`.

## Window operations

| Area | Functions |
| --- | --- |
| Lifetime | `rg_window_create`, `rg_window_destroy` |
| Logical size | `rg_window_get_size`, `rg_window_set_size` |
| Constraints | `rg_window_set_min_size`, `rg_window_set_max_size` |
| Presentation | `rg_window_set_title`, `rg_window_show`, `rg_window_hide`, `rg_window_raise` |
| Display mode | `rg_window_set_fullscreen`, `rg_window_set_borderless`, `rg_window_set_resizable` |
| High DPI | `rg_window_get_pixel_size`, `rg_window_get_content_scale` |
| Mouse | `rg_window_set_relative_mouse` |

`rg_window_set_fullscreen` returns `1` on success and `0` on failure. Creation
returns null on failure. Retrieve the reason through `rg_sdl_error` or
`SDL_GetError`.

The void setter wrappers intentionally do not expose SDL's success value. Use
the underlying SDL function directly when the application must react to a
setter failure.

## Logical and pixel sizes

`rg_window_get_size` returns logical window coordinates.
`rg_window_get_pixel_size` returns drawable pixels, which may be larger on a
high-DPI display. `rg_window_get_content_scale` reports `pixel / logical` for
each axis and writes `1.0f` when a logical dimension is zero. Any output pointer
may be null.

Use pixel dimensions for swapchains and rendering targets, and logical
dimensions for window layout and input coordinates.

## Relative mouse mode

`rg_window_set_relative_mouse` changes SDL's mode without tracking input state.
When using `RgInputState`, prefer `rg_input_set_relative_mouse`; it also updates
the flag that makes `rg_input_update` read relative deltas.

## Configuration and ownership

Define `RG_WINDOW_ASSERT(condition)` before inclusion to replace the default
`assert` hook. Unless otherwise noted, functions require a non-null live
`SDL_Window*`; destroy each successfully created window exactly once.

The header has normal include guards and all wrapper functions have internal
linkage. SDL's window-thread requirements still apply.
