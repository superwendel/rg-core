# rg_input

`rg_input.h` maintains immediate keyboard, mouse, wheel, and text-input state
from SDL3 events. It can also normalize those events into a caller-owned,
ordered queue for systems that need exact event order or IME data.

Call `rg_input_update` before polling events each frame, then pass every event
to `rg_input_process_event` or `rg_input_process_event_ex`.

```c
#include "rg_input.h"

RgInputState input = {0};
rg_input_init(&input);

while (running)
{
	rg_input_update(&input);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		rg_input_process_event(&input, &event);
	}

	if (rg_input_is_key_pressed(&input, SDL_SCANCODE_ESCAPE))
	{
		running = 0;
	}
}
```

The ordered queue never allocates. Event and UTF-8 text storage are supplied
by the caller, and overflow counters make dropped input visible.
