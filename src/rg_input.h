// rg_input - SDL3 input helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header C99 helper for keyboard, mouse, and text input state.
//
// USAGE:
//   #include "rg_input.h"
//
//   RgInputState input = {};
//   rg_input_init(&input);
//
//   // Per frame:
//   rg_input_update(&input); // call before processing events
//   SDL_Event event;
//   while (SDL_PollEvent(&event))
//   {
//       rg_input_process_event(&input, &event);
//   }
//
// OPTIONS:
//   #define RG_INPUT_TEXT_BUFFER_SIZE  - Text input buffer size (default: 32)
//   #define RG_INPUT_ASSERT(x)         - Custom assert macro (default: assert)
//
// NOTES:
//   - All functions have internal linkage and work in unity builds.
//   - Call rg_input_update before SDL_PollEvent to reset per-frame state.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_INPUT_H
#define RG_INPUT_H

#include "rg_defs.h"

#include <SDL3/SDL.h>

#include <stdbool.h>
#include <string.h>

// =============================================================================
// CONFIGURATION
// =============================================================================

#ifndef RG_INPUT_TEXT_BUFFER_SIZE
#define RG_INPUT_TEXT_BUFFER_SIZE 32u
#endif

#ifndef RG_INPUT_ASSERT
#include <assert.h>
#define RG_INPUT_ASSERT(x) assert(x)
#endif

#ifndef RG_INPUT_UNUSED
#define RG_INPUT_UNUSED(x) (void)(x)
#endif

// Mouse button indices (for rg_input_is_mouse_button_*)
#define RG_MOUSE_BUTTON_LEFT 0
#define RG_MOUSE_BUTTON_MIDDLE 1
#define RG_MOUSE_BUTTON_RIGHT 2
#define RG_MOUSE_BUTTON_X1 3
#define RG_MOUSE_BUTTON_X2 4
#define RG_MOUSE_BUTTON_COUNT 5

// =============================================================================
// TYPES
// =============================================================================

typedef enum RgInputEventKind
{
	RG_INPUT_EVENT_NONE = 0,
	RG_INPUT_EVENT_KEY_DOWN,
	RG_INPUT_EVENT_KEY_UP,
	RG_INPUT_EVENT_TEXT_INPUT,
	RG_INPUT_EVENT_TEXT_EDITING,
	RG_INPUT_EVENT_MOUSE_MOVE,
	RG_INPUT_EVENT_MOUSE_BUTTON_DOWN,
	RG_INPUT_EVENT_MOUSE_BUTTON_UP,
	RG_INPUT_EVENT_MOUSE_WHEEL,
	RG_INPUT_EVENT_WINDOW_FOCUS_GAINED,
	RG_INPUT_EVENT_WINDOW_FOCUS_LOST,
	RG_INPUT_EVENT_COUNT
} RgInputEventKind;

typedef struct RgInputStringView
{
	const char* data;
	size_t length; // UTF-8 bytes, excluding the stored terminating NUL.
} RgInputStringView;

typedef struct RgInputKeyEvent
{
	SDL_KeyboardID keyboard_id;
	SDL_Scancode scancode;
	SDL_Keycode keycode;
	u16 raw;
	bool repeat;
} RgInputKeyEvent;

typedef struct RgInputTextEvent
{
	RgInputStringView text;
	i32 start;  // SDL IME character offset; may be -1.
	i32 length; // SDL IME character count, not a byte count.
} RgInputTextEvent;

typedef struct RgInputMouseMotionEvent
{
	SDL_MouseID mouse_id;
	SDL_MouseButtonFlags buttons;
	f32 x;
	f32 y;
	f32 dx;
	f32 dy;
} RgInputMouseMotionEvent;

typedef struct RgInputMouseButtonEvent
{
	SDL_MouseID mouse_id;
	i8 button; // Zero-based RG_MOUSE_BUTTON_* or -1 when unknown.
	u8 clicks;
	f32 x;
	f32 y;
} RgInputMouseButtonEvent;

typedef struct RgInputMouseWheelEvent
{
	SDL_MouseID mouse_id;
	f32 x;
	f32 y;
	f32 mouse_x;
	f32 mouse_y;
	SDL_MouseWheelDirection direction;
} RgInputMouseWheelEvent;

typedef struct RgInputEvent
{
	RgInputEventKind kind;
	u64 timestamp_ns;
	SDL_WindowID window_id;
	SDL_Keymod modifiers;
	union
	{
		RgInputKeyEvent key;
		RgInputTextEvent text;
		RgInputMouseMotionEvent motion;
		RgInputMouseButtonEvent button;
		RgInputMouseWheelEvent wheel;
	} data;
} RgInputEvent;

/**
 * Optional caller-owned ordered event queue.
 *
 * Consumers should treat the queue and its buffers as const. The modifiers
 * field tracks modifier changes in event order. Supply the starting modifier
 * state explicitly when resetting the queue each frame.
 */
typedef struct RgInputEventQueue
{
	RgInputEvent* events;
	size_t count;
	size_t capacity;

	char* text_buffer;
	size_t text_used;
	size_t text_capacity;

	size_t dropped_event_count;
	size_t dropped_text_byte_count;
	SDL_Keymod modifiers;
} RgInputEventQueue;

typedef struct RgInputState
{
	bool current_keyboard[SDL_SCANCODE_COUNT];
	bool previous_keyboard[SDL_SCANCODE_COUNT];

	int mouse_x;
	int mouse_y;
	int mouse_delta_x;
	int mouse_delta_y;
	f32 mouse_scroll_y;
	bool current_mouse[RG_MOUSE_BUTTON_COUNT];
	bool previous_mouse[RG_MOUSE_BUTTON_COUNT];
	int relative_mouse_enabled;

	bool text_input_active;
	char text_input_buffer[RG_INPUT_TEXT_BUFFER_SIZE];
	bool has_text_input;
} RgInputState;

// =============================================================================
// PUBLIC API
// =============================================================================

/**
 * @brief Initialize input state
 * @param input Input state
 */
RGINLINE void rg_input_init(RgInputState* input);

/**
 * @brief Update input state from SDL (call before event processing)
 * @param input Input state
 */
RGINLINE void rg_input_update(RgInputState* input);

/**
 * @brief Initialize an optional ordered event queue with caller-owned storage
 * @param queue Event queue
 * @param events Event storage (may be NULL when event_capacity is zero)
 * @param event_capacity Number of events available in events
 * @param text_buffer Text storage (may be NULL when text_capacity is zero)
 * @param text_capacity Number of bytes available in text_buffer
 */
RGINLINE void rg_input_event_queue_init(RgInputEventQueue* queue,
                                        RgInputEvent* events,
                                        size_t event_capacity,
                                        char* text_buffer,
                                        size_t text_capacity);

/**
 * @brief Reset per-frame queue contents, telemetry, and initial modifiers
 * @param queue Event queue
 * @param initial_modifiers Modifier state at the start of the event stream
 */
RGINLINE void rg_input_event_queue_reset(RgInputEventQueue* queue,
                                         SDL_Keymod initial_modifiers);

/**
 * @brief Process an SDL event and optionally append its normalized form
 * @param input Input state
 * @param event SDL event
 * @param queue Optional ordered event queue
 */
RGINLINE void rg_input_process_event_ex(RgInputState* input,
                                        const SDL_Event* event,
                                        RgInputEventQueue* queue);

/**
 * @brief Process SDL events that affect input state
 * @param input Input state
 * @param event SDL event
 */
RGINLINE void rg_input_process_event(RgInputState* input, const SDL_Event* event);

/**
 * @brief Enable or disable relative mouse mode for a window
 * @param input Input state
 * @param window Window handle (optional, uses keyboard focus if NULL)
 * @param enabled Non-zero to enable
 */
RGINLINE void rg_input_set_relative_mouse(RgInputState* input, SDL_Window* window, int enabled);

/**
 * @brief Enable or disable text input for a window
 * @param input Input state
 * @param window Window handle (optional, uses keyboard focus if NULL)
 * @param enabled Non-zero to enable
 */
RGINLINE void rg_input_set_text_input(RgInputState* input, SDL_Window* window, int enabled);

/**
 * @brief Return 1 if key is currently held
 * @param input Input state
 * @param scancode SDL scancode
 * @return 1 if down, 0 otherwise
 */
RGINLINE int rg_input_is_key_down(const RgInputState* input, SDL_Scancode scancode);

/**
 * @brief Return 1 if key was pressed this frame
 * @param input Input state
 * @param scancode SDL scancode
 * @return 1 if pressed, 0 otherwise
 */
RGINLINE int rg_input_is_key_pressed(const RgInputState* input, SDL_Scancode scancode);

/**
 * @brief Return 1 if key was released this frame
 * @param input Input state
 * @param scancode SDL scancode
 * @return 1 if released, 0 otherwise
 */
RGINLINE int rg_input_is_key_released(const RgInputState* input, SDL_Scancode scancode);

/**
 * @brief Return 1 if mouse button is held
 * @param input Input state
 * @param button Mouse button index (RG_MOUSE_BUTTON_*)
 * @return 1 if down, 0 otherwise
 */
RGINLINE int rg_input_is_mouse_button_down(const RgInputState* input, int button);

/**
 * @brief Return 1 if mouse button was pressed this frame
 * @param input Input state
 * @param button Mouse button index (RG_MOUSE_BUTTON_*)
 * @return 1 if pressed, 0 otherwise
 */
RGINLINE int rg_input_is_mouse_button_pressed(const RgInputState* input, int button);

/**
 * @brief Return 1 if mouse button was released this frame
 * @param input Input state
 * @param button Mouse button index (RG_MOUSE_BUTTON_*)
 * @return 1 if released, 0 otherwise
 */
RGINLINE int rg_input_is_mouse_button_released(const RgInputState* input, int button);

/**
 * @brief Get mouse delta for this frame
 * @param input Input state
 * @param dx Output delta x
 * @param dy Output delta y
 */
RGINLINE void rg_input_get_mouse_delta(const RgInputState* input, int* dx, int* dy);

/**
 * @brief Get mouse position
 * @param input Input state
 * @param x Output x
 * @param y Output y
 */
RGINLINE void rg_input_get_mouse_position(const RgInputState* input, int* x, int* y);

// =============================================================================
// IMPLEMENTATION
// =============================================================================

RGINLINE void rg_input_init(RgInputState* input)
{
	RG_INPUT_ASSERT(input != NULL);
	memset(input, 0, sizeof(*input));
}

RGINLINE void rg_input_update(RgInputState* input)
{
	RG_INPUT_ASSERT(input != NULL);

	memcpy(input->previous_keyboard, input->current_keyboard, sizeof(input->current_keyboard));
	memcpy(input->previous_mouse, input->current_mouse, sizeof(input->current_mouse));

	const bool* keyboard = SDL_GetKeyboardState(NULL);
	memcpy(input->current_keyboard, keyboard, sizeof(input->current_keyboard));

	if (input->relative_mouse_enabled)
	{
		f32 dx = 0.0f;
		f32 dy = 0.0f;
		SDL_GetRelativeMouseState(&dx, &dy);
		input->mouse_delta_x = (int)dx;
		input->mouse_delta_y = (int)dy;
	}
	else
	{
		f32 mx = 0.0f;
		f32 my = 0.0f;
		SDL_GetMouseState(&mx, &my);
		input->mouse_delta_x = (int)mx - input->mouse_x;
		input->mouse_delta_y = (int)my - input->mouse_y;
		input->mouse_x = (int)mx;
		input->mouse_y = (int)my;
	}

	SDL_MouseButtonFlags mouse_state = SDL_GetMouseState(NULL, NULL);
	input->current_mouse[RG_MOUSE_BUTTON_LEFT] = (mouse_state & SDL_BUTTON_LMASK) != 0;
	input->current_mouse[RG_MOUSE_BUTTON_MIDDLE] = (mouse_state & SDL_BUTTON_MMASK) != 0;
	input->current_mouse[RG_MOUSE_BUTTON_RIGHT] = (mouse_state & SDL_BUTTON_RMASK) != 0;
	input->current_mouse[RG_MOUSE_BUTTON_X1] = (mouse_state & SDL_BUTTON_X1MASK) != 0;
	input->current_mouse[RG_MOUSE_BUTTON_X2] = (mouse_state & SDL_BUTTON_X2MASK) != 0;

	input->has_text_input = false;
	input->text_input_buffer[0] = '\0';
	input->mouse_scroll_y = 0.0f;
}

RGINLINE void rg_input_event_queue_init(RgInputEventQueue* queue,
                                        RgInputEvent* events,
                                        size_t event_capacity,
                                        char* text_buffer,
                                        size_t text_capacity)
{
	RG_INPUT_ASSERT(queue != NULL);
	RG_INPUT_ASSERT(events != NULL || event_capacity == 0);
	RG_INPUT_ASSERT(text_buffer != NULL || text_capacity == 0);

	memset(queue, 0, sizeof(*queue));
	queue->events = events;
	queue->capacity = event_capacity;
	queue->text_buffer = text_buffer;
	queue->text_capacity = text_capacity;
	queue->modifiers = SDL_KMOD_NONE;

	if (text_buffer && text_capacity > 0)
	{
		text_buffer[0] = '\0';
	}
}

RGINLINE void rg_input_event_queue_reset(RgInputEventQueue* queue,
                                         SDL_Keymod initial_modifiers)
{
	RG_INPUT_ASSERT(queue != NULL);

	queue->count = 0;
	queue->text_used = 0;
	queue->dropped_event_count = 0;
	queue->dropped_text_byte_count = 0;
	queue->modifiers = initial_modifiers;

	if (queue->text_buffer && queue->text_capacity > 0)
	{
		queue->text_buffer[0] = '\0';
	}
}

RGINLINE RgInputEvent* rg_input_event_queue_push(RgInputEventQueue* queue)
{
	if (queue->count >= queue->capacity)
	{
		queue->dropped_event_count++;
		return NULL;
	}

	RgInputEvent* result = &queue->events[queue->count++];
	memset(result, 0, sizeof(*result));
	return result;
}

RGINLINE RgInputEvent* rg_input_event_queue_push_text(RgInputEventQueue* queue,
                                                      const char* text)
{
	const size_t length = strlen(text);
	const size_t required = length + 1;

	if (queue->count >= queue->capacity)
	{
		queue->dropped_event_count++;
		return NULL;
	}

	if (required > queue->text_capacity - queue->text_used)
	{
		queue->dropped_event_count++;
		queue->dropped_text_byte_count += required;
		return NULL;
	}

	char* copied_text = queue->text_buffer + queue->text_used;
	memcpy(copied_text, text, required);
	queue->text_used += required;

	RgInputEvent* result = &queue->events[queue->count++];
	memset(result, 0, sizeof(*result));
	result->data.text.text.data = copied_text;
	result->data.text.text.length = length;
	return result;
}

RGINLINE i8 rg_input_mouse_button_from_sdl(u8 button)
{
	switch (button)
	{
		case SDL_BUTTON_LEFT: return RG_MOUSE_BUTTON_LEFT;
		case SDL_BUTTON_MIDDLE: return RG_MOUSE_BUTTON_MIDDLE;
		case SDL_BUTTON_RIGHT: return RG_MOUSE_BUTTON_RIGHT;
		case SDL_BUTTON_X1: return RG_MOUSE_BUTTON_X1;
		case SDL_BUTTON_X2: return RG_MOUSE_BUTTON_X2;
		default: return -1;
	}
}

RGINLINE void rg_input_process_event_ex(RgInputState* input,
                                        const SDL_Event* event,
                                        RgInputEventQueue* queue)
{
	RG_INPUT_ASSERT(input != NULL);
	RG_INPUT_ASSERT(event != NULL);

	if (event->type == SDL_EVENT_TEXT_INPUT)
	{
		SDL_strlcpy(input->text_input_buffer, event->text.text, (size_t)RG_INPUT_TEXT_BUFFER_SIZE);
		input->has_text_input = true;
	}
	else if (event->type == SDL_EVENT_MOUSE_WHEEL)
	{
		input->mouse_scroll_y += event->wheel.y;
	}

	if (!queue)
	{
		return;
	}

	RgInputEvent* queued = NULL;
	switch (event->type)
	{
		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP:
		{
			queue->modifiers = event->key.mod;
			queued = rg_input_event_queue_push(queue);
			if (queued)
			{
				queued->kind = event->type == SDL_EVENT_KEY_DOWN
				                   ? RG_INPUT_EVENT_KEY_DOWN
				                   : RG_INPUT_EVENT_KEY_UP;
				queued->timestamp_ns = event->key.timestamp;
				queued->window_id = event->key.windowID;
				queued->modifiers = event->key.mod;
				queued->data.key.keyboard_id = event->key.which;
				queued->data.key.scancode = event->key.scancode;
				queued->data.key.keycode = event->key.key;
				queued->data.key.raw = event->key.raw;
				queued->data.key.repeat = event->key.repeat;
			}
		}
		break;

		case SDL_EVENT_TEXT_INPUT:
		{
			queued = rg_input_event_queue_push_text(queue, event->text.text);
			if (queued)
			{
				queued->kind = RG_INPUT_EVENT_TEXT_INPUT;
				queued->timestamp_ns = event->text.timestamp;
				queued->window_id = event->text.windowID;
				queued->modifiers = queue->modifiers;
			}
		}
		break;

		case SDL_EVENT_TEXT_EDITING:
		{
			queued = rg_input_event_queue_push_text(queue, event->edit.text);
			if (queued)
			{
				queued->kind = RG_INPUT_EVENT_TEXT_EDITING;
				queued->timestamp_ns = event->edit.timestamp;
				queued->window_id = event->edit.windowID;
				queued->modifiers = queue->modifiers;
				queued->data.text.start = event->edit.start;
				queued->data.text.length = event->edit.length;
			}
		}
		break;

		case SDL_EVENT_MOUSE_MOTION:
		{
			queued = rg_input_event_queue_push(queue);
			if (queued)
			{
				queued->kind = RG_INPUT_EVENT_MOUSE_MOVE;
				queued->timestamp_ns = event->motion.timestamp;
				queued->window_id = event->motion.windowID;
				queued->modifiers = queue->modifiers;
				queued->data.motion.mouse_id = event->motion.which;
				queued->data.motion.buttons = event->motion.state;
				queued->data.motion.x = event->motion.x;
				queued->data.motion.y = event->motion.y;
				queued->data.motion.dx = event->motion.xrel;
				queued->data.motion.dy = event->motion.yrel;
			}
		}
		break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
		{
			queued = rg_input_event_queue_push(queue);
			if (queued)
			{
				queued->kind = event->type == SDL_EVENT_MOUSE_BUTTON_DOWN
				                   ? RG_INPUT_EVENT_MOUSE_BUTTON_DOWN
				                   : RG_INPUT_EVENT_MOUSE_BUTTON_UP;
				queued->timestamp_ns = event->button.timestamp;
				queued->window_id = event->button.windowID;
				queued->modifiers = queue->modifiers;
				queued->data.button.mouse_id = event->button.which;
				queued->data.button.button = rg_input_mouse_button_from_sdl(
				    event->button.button);
				queued->data.button.clicks = event->button.clicks;
				queued->data.button.x = event->button.x;
				queued->data.button.y = event->button.y;
			}
		}
		break;

		case SDL_EVENT_MOUSE_WHEEL:
		{
			queued = rg_input_event_queue_push(queue);
			if (queued)
			{
				queued->kind = RG_INPUT_EVENT_MOUSE_WHEEL;
				queued->timestamp_ns = event->wheel.timestamp;
				queued->window_id = event->wheel.windowID;
				queued->modifiers = queue->modifiers;
				queued->data.wheel.mouse_id = event->wheel.which;
				queued->data.wheel.x = event->wheel.x;
				queued->data.wheel.y = event->wheel.y;
				queued->data.wheel.mouse_x = event->wheel.mouse_x;
				queued->data.wheel.mouse_y = event->wheel.mouse_y;
				queued->data.wheel.direction = event->wheel.direction;
			}
		}
		break;

		case SDL_EVENT_WINDOW_FOCUS_GAINED:
		case SDL_EVENT_WINDOW_FOCUS_LOST:
		{
			queued = rg_input_event_queue_push(queue);
			if (queued)
			{
				queued->kind = event->type == SDL_EVENT_WINDOW_FOCUS_GAINED
				                   ? RG_INPUT_EVENT_WINDOW_FOCUS_GAINED
				                   : RG_INPUT_EVENT_WINDOW_FOCUS_LOST;
				queued->timestamp_ns = event->window.timestamp;
				queued->window_id = event->window.windowID;
				queued->modifiers = queue->modifiers;
			}
			if (event->type == SDL_EVENT_WINDOW_FOCUS_LOST)
			{
				queue->modifiers = SDL_KMOD_NONE;
			}
		}
		break;

		default: break;
	}
}

RGINLINE void rg_input_process_event(RgInputState* input, const SDL_Event* event)
{
	rg_input_process_event_ex(input, event, NULL);
}

RGINLINE void rg_input_set_relative_mouse(RgInputState* input, SDL_Window* window, int enabled)
{
	RG_INPUT_ASSERT(input != NULL);
	input->relative_mouse_enabled = enabled != 0;
	SDL_SetWindowRelativeMouseMode(window ? window : SDL_GetKeyboardFocus(), enabled != 0);
}

RGINLINE void rg_input_set_text_input(RgInputState* input, SDL_Window* window, int enabled)
{
	RG_INPUT_ASSERT(input != NULL);
	if (enabled)
	{
		SDL_StartTextInput(window ? window : SDL_GetKeyboardFocus());
	}
	else
	{
		SDL_StopTextInput(window ? window : SDL_GetKeyboardFocus());
	}
	input->text_input_active = enabled != 0;
}

RGINLINE int rg_input_is_key_down(const RgInputState* input, SDL_Scancode scancode)
{
	return input->current_keyboard[scancode] ? 1 : 0;
}

RGINLINE int rg_input_is_key_pressed(const RgInputState* input, SDL_Scancode scancode)
{
	return (input->current_keyboard[scancode] && !input->previous_keyboard[scancode]) ? 1 : 0;
}

RGINLINE int rg_input_is_key_released(const RgInputState* input, SDL_Scancode scancode)
{
	return (!input->current_keyboard[scancode] && input->previous_keyboard[scancode]) ? 1 : 0;
}

RGINLINE int rg_input_is_mouse_button_down(const RgInputState* input, int button)
{
	if (button < 0 || button >= RG_MOUSE_BUTTON_COUNT)
	{
		return 0;
	}
	return input->current_mouse[button] ? 1 : 0;
}

RGINLINE int rg_input_is_mouse_button_pressed(const RgInputState* input, int button)
{
	if (button < 0 || button >= RG_MOUSE_BUTTON_COUNT)
	{
		return 0;
	}
	return (input->current_mouse[button] && !input->previous_mouse[button]) ? 1 : 0;
}

RGINLINE int rg_input_is_mouse_button_released(const RgInputState* input, int button)
{
	if (button < 0 || button >= RG_MOUSE_BUTTON_COUNT)
	{
		return 0;
	}
	return (!input->current_mouse[button] && input->previous_mouse[button]) ? 1 : 0;
}

RGINLINE void rg_input_get_mouse_delta(const RgInputState* input, int* dx, int* dy)
{
	if (dx)
	{
		*dx = input->mouse_delta_x;
	}
	if (dy)
	{
		*dy = input->mouse_delta_y;
	}
}

RGINLINE void rg_input_get_mouse_position(const RgInputState* input, int* x, int* y)
{
	if (x)
	{
		*x = input->mouse_x;
	}
	if (y)
	{
		*y = input->mouse_y;
	}
}

#endif // RG_INPUT_H
