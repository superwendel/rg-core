// rg_input test suite
//
// Usage:
//   test_input.exe

#include "../src/rg_input.h"

#include <stdio.h>
#include <string.h>

static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(condition, message)                          \
	do {                                                         \
		if (!(condition))                                        \
		{                                                        \
			printf("  FAIL: %s (line %d)\n", message, __LINE__); \
			g_tests_failed++;                                    \
			return;                                              \
		}                                                        \
	} while (0)

#define TEST_PASS()       \
	do {                  \
		g_tests_passed++; \
	} while (0)

static void test_legacy_event_behavior(void)
{
	RgInputState input;
	rg_input_init(&input);

	SDL_Event event;
	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_TEXT_INPUT;
	event.text.text = "legacy text";
	rg_input_process_event(&input, &event);

	TEST_ASSERT(input.has_text_input, "legacy text event flag");
	TEST_ASSERT(strcmp(input.text_input_buffer, "legacy text") == 0,
	            "legacy text event copy");

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_MOUSE_WHEEL;
	event.wheel.y = 1.25f;
	rg_input_process_event(&input, &event);
	event.wheel.y = -0.5f;
	rg_input_process_event_ex(&input, &event, NULL);

	TEST_ASSERT(input.mouse_scroll_y == 0.75f, "legacy wheel accumulation");

	TEST_PASS();
	printf("  PASS: legacy event behavior\n");
}

static void test_order_and_event_data(void)
{
	RgInputState input;
	RgInputEvent events[16];
	char text_buffer[64];
	RgInputEventQueue queue;
	SDL_Event event;
	char source_text[8] = "abc";
	char preedit_source[8] = "\xE3\x81\x82"
	                         "x";

	rg_input_init(&input);
	rg_input_event_queue_init(&queue,
	                          events,
	                          sizeof(events) / sizeof(events[0]),
	                          text_buffer,
	                          sizeof(text_buffer));

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_KEY_DOWN;
	event.key.timestamp = 11;
	event.key.windowID = 7;
	event.key.which = 3;
	event.key.scancode = SDL_SCANCODE_A;
	event.key.key = SDLK_A;
	event.key.mod = SDL_KMOD_LSHIFT;
	event.key.raw = 42;
	event.key.repeat = true;
	rg_input_process_event_ex(&input, &event, &queue);

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_TEXT_INPUT;
	event.text.timestamp = 12;
	event.text.windowID = 7;
	event.text.text = source_text;
	rg_input_process_event_ex(&input, &event, &queue);
	source_text[0] = 'z';

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_TEXT_EDITING;
	event.edit.timestamp = 13;
	event.edit.windowID = 7;
	event.edit.text = preedit_source;
	event.edit.start = -1;
	event.edit.length = 1;
	rg_input_process_event_ex(&input, &event, &queue);
	preedit_source[0] = 'z';

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_MOUSE_MOTION;
	event.motion.timestamp = 14;
	event.motion.windowID = 7;
	event.motion.which = 5;
	event.motion.state = SDL_BUTTON_LMASK | SDL_BUTTON_RMASK;
	event.motion.x = 10.5f;
	event.motion.y = 20.25f;
	event.motion.xrel = -1.5f;
	event.motion.yrel = 2.75f;
	rg_input_process_event_ex(&input, &event, &queue);

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
	event.button.timestamp = 15;
	event.button.windowID = 7;
	event.button.which = 5;
	event.button.button = SDL_BUTTON_LEFT;
	event.button.clicks = 2;
	event.button.x = 30.5f;
	event.button.y = 40.75f;
	rg_input_process_event_ex(&input, &event, &queue);

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_MOUSE_WHEEL;
	event.wheel.timestamp = 16;
	event.wheel.windowID = 7;
	event.wheel.which = 5;
	event.wheel.x = -0.25f;
	event.wheel.y = 1.5f;
	event.wheel.mouse_x = 31.5f;
	event.wheel.mouse_y = 41.75f;
	event.wheel.direction = SDL_MOUSEWHEEL_FLIPPED;
	rg_input_process_event_ex(&input, &event, &queue);

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_KEY_UP;
	event.key.timestamp = 17;
	event.key.windowID = 7;
	event.key.which = 3;
	event.key.scancode = SDL_SCANCODE_A;
	event.key.key = SDLK_A;
	event.key.mod = SDL_KMOD_NONE;
	event.key.raw = 42;
	rg_input_process_event_ex(&input, &event, &queue);

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_MOUSE_BUTTON_UP;
	event.button.timestamp = 18;
	event.button.windowID = 7;
	event.button.which = 5;
	event.button.button = SDL_BUTTON_LEFT;
	event.button.clicks = 2;
	event.button.x = 30.5f;
	event.button.y = 40.75f;
	rg_input_process_event_ex(&input, &event, &queue);

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_WINDOW_FOCUS_GAINED;
	event.window.timestamp = 19;
	event.window.windowID = 7;
	rg_input_process_event_ex(&input, &event, &queue);

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_WINDOW_FOCUS_LOST;
	event.window.timestamp = 20;
	event.window.windowID = 7;
	rg_input_process_event_ex(&input, &event, &queue);

	TEST_ASSERT(queue.count == 10, "all supported events queued in order");
	TEST_ASSERT(events[0].kind == RG_INPUT_EVENT_KEY_DOWN, "key down kind");
	TEST_ASSERT(events[0].timestamp_ns == 11 && events[0].window_id == 7,
	            "key common fields");
	TEST_ASSERT(events[0].modifiers == SDL_KMOD_LSHIFT,
	            "key modifier snapshot");
	TEST_ASSERT(events[0].data.key.keyboard_id == 3 &&
	                events[0].data.key.scancode == SDL_SCANCODE_A &&
	                events[0].data.key.keycode == SDLK_A &&
	                events[0].data.key.raw == 42 && events[0].data.key.repeat,
	            "key payload");

	TEST_ASSERT(events[1].kind == RG_INPUT_EVENT_TEXT_INPUT, "text kind");
	TEST_ASSERT(events[1].modifiers == SDL_KMOD_LSHIFT,
	            "ordered text modifier snapshot");
	TEST_ASSERT(events[1].data.text.text.length == 3 &&
	                strcmp(events[1].data.text.text.data, "abc") == 0,
	            "text deep copy");

	TEST_ASSERT(events[2].kind == RG_INPUT_EVENT_TEXT_EDITING,
	            "editing kind");
	TEST_ASSERT(events[2].data.text.text.length == 4 &&
	                strcmp(events[2].data.text.text.data,
	                       "\xE3\x81\x82"
	                       "x") == 0 &&
	                events[2].data.text.start == -1 &&
	                events[2].data.text.length == 1,
	            "editing deep copy, byte length, and character range");

	TEST_ASSERT(events[3].kind == RG_INPUT_EVENT_MOUSE_MOVE,
	            "mouse motion kind");
	TEST_ASSERT(events[3].data.motion.mouse_id == 5 &&
	                events[3].data.motion.buttons == (SDL_BUTTON_LMASK | SDL_BUTTON_RMASK) &&
	                events[3].data.motion.x == 10.5f &&
	                events[3].data.motion.y == 20.25f &&
	                events[3].data.motion.dx == -1.5f &&
	                events[3].data.motion.dy == 2.75f,
	            "mouse motion raw floats");
	TEST_ASSERT(events[3].modifiers == SDL_KMOD_LSHIFT,
	            "ordered mouse modifier snapshot");

	TEST_ASSERT(events[4].kind == RG_INPUT_EVENT_MOUSE_BUTTON_DOWN &&
	                events[4].data.button.button == RG_MOUSE_BUTTON_LEFT &&
	                events[4].data.button.clicks == 2 &&
	                events[4].data.button.x == 30.5f &&
	                events[4].data.button.y == 40.75f,
	            "mouse button down payload");

	TEST_ASSERT(events[5].kind == RG_INPUT_EVENT_MOUSE_WHEEL &&
	                events[5].data.wheel.mouse_id == 5 &&
	                events[5].data.wheel.x == -0.25f &&
	                events[5].data.wheel.y == 1.5f &&
	                events[5].data.wheel.mouse_x == 31.5f &&
	                events[5].data.wheel.mouse_y == 41.75f &&
	                events[5].data.wheel.direction == SDL_MOUSEWHEEL_FLIPPED,
	            "mouse wheel payload");

	TEST_ASSERT(events[6].kind == RG_INPUT_EVENT_KEY_UP &&
	                events[6].modifiers == SDL_KMOD_NONE,
	            "key up and modifier update");
	TEST_ASSERT(events[7].kind == RG_INPUT_EVENT_MOUSE_BUTTON_UP &&
	                events[7].modifiers == SDL_KMOD_NONE,
	            "mouse button up after modifier update");
	TEST_ASSERT(events[8].kind == RG_INPUT_EVENT_WINDOW_FOCUS_GAINED &&
	                events[9].kind == RG_INPUT_EVENT_WINDOW_FOCUS_LOST,
	            "window focus events");
	TEST_ASSERT(queue.modifiers == SDL_KMOD_NONE,
	            "focus loss clears tracked modifiers");
	TEST_ASSERT(queue.dropped_event_count == 0 &&
	                queue.dropped_text_byte_count == 0,
	            "no unexpected queue drops");

	const RgInputEventQueue* readonly_queue = &queue;
	TEST_ASSERT(readonly_queue->events[0].kind == RG_INPUT_EVENT_KEY_DOWN,
	            "queue supports const consumers");

	TEST_PASS();
	printf("  PASS: ordered event data\n");
}

static void test_reset_and_modifier_continuity(void)
{
	RgInputState input;
	RgInputEvent events[2];
	char text_buffer[8];
	RgInputEventQueue queue;
	SDL_Event event;

	rg_input_init(&input);
	rg_input_event_queue_init(&queue, events, 2, text_buffer, sizeof(text_buffer));

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_KEY_DOWN;
	event.key.mod = SDL_KMOD_LCTRL;
	rg_input_process_event_ex(&input, &event, &queue);

	rg_input_event_queue_reset(&queue, SDL_KMOD_RALT);
	TEST_ASSERT(queue.count == 0 && queue.text_used == 0,
	            "reset queue contents");
	TEST_ASSERT(queue.dropped_event_count == 0 &&
	                queue.dropped_text_byte_count == 0,
	            "reset overflow telemetry");
	TEST_ASSERT(queue.modifiers == SDL_KMOD_RALT,
	            "reset applies explicit modifier state");

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_MOUSE_MOTION;
	rg_input_process_event_ex(&input, &event, &queue);
	TEST_ASSERT(events[0].modifiers == SDL_KMOD_RALT,
	            "explicit modifier state starts new event stream");

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_KEY_DOWN;
	event.key.mod = SDL_KMOD_LALT;
	rg_input_process_event_ex(&input, &event, &queue);

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_WINDOW_FOCUS_LOST;
	rg_input_process_event_ex(&input, &event, &queue);
	TEST_ASSERT(queue.count == 2 && queue.dropped_event_count == 1,
	            "focus event reports full event buffer");
	TEST_ASSERT(queue.modifiers == SDL_KMOD_NONE,
	            "overflowed focus loss still clears producer modifiers");

	TEST_PASS();
	printf("  PASS: reset and modifier continuity\n");
}

static void test_atomic_overflow(void)
{
	RgInputState input;
	RgInputEvent events[2];
	char text_buffer[4];
	RgInputEventQueue queue;
	SDL_Event event;

	rg_input_init(&input);
	rg_input_event_queue_init(&queue, events, 1, text_buffer, sizeof(text_buffer));

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_KEY_DOWN;
	event.key.mod = SDL_KMOD_LALT;
	rg_input_process_event_ex(&input, &event, &queue);

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_TEXT_INPUT;
	event.text.text = "ab";
	rg_input_process_event_ex(&input, &event, &queue);
	TEST_ASSERT(queue.count == 1 && queue.text_used == 0,
	            "event-capacity drop is atomic");
	TEST_ASSERT(queue.dropped_event_count == 1 &&
	                queue.dropped_text_byte_count == 0,
	            "event-capacity telemetry");
	TEST_ASSERT(input.has_text_input && strcmp(input.text_input_buffer, "ab") == 0,
	            "legacy snapshot updates when queue is full");

	rg_input_event_queue_init(&queue, events, 2, text_buffer, 3);
	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_TEXT_INPUT;
	event.text.text = "abcd";
	rg_input_process_event_ex(&input, &event, &queue);
	TEST_ASSERT(queue.count == 0 && queue.text_used == 0,
	            "text-capacity drop is atomic");
	TEST_ASSERT(queue.dropped_event_count == 1 &&
	                queue.dropped_text_byte_count == 5,
	            "text-capacity telemetry includes terminator");

	event.text.text = "ab";
	rg_input_process_event_ex(&input, &event, &queue);
	TEST_ASSERT(queue.count == 1 && queue.text_used == 3,
	            "exact-fit event follows dropped event");
	TEST_ASSERT(strcmp(events[0].data.text.text.data, "ab") == 0,
	            "valid prefix remains intact");

	event.text.text = "c";
	rg_input_process_event_ex(&input, &event, &queue);
	TEST_ASSERT(queue.count == 1 && queue.text_used == 3,
	            "second text-capacity drop keeps valid prefix");
	TEST_ASSERT(queue.dropped_event_count == 2 &&
	                queue.dropped_text_byte_count == 7,
	            "cumulative text overflow telemetry");
	TEST_ASSERT(strcmp(events[0].data.text.text.data, "ab") == 0,
	            "overflow does not overwrite copied text");

	TEST_PASS();
	printf("  PASS: atomic overflow\n");
}

static void test_mouse_button_normalization(void)
{
	RgInputState input;
	RgInputEvent events[6];
	RgInputEventQueue queue;
	SDL_Event event;
	const uint8_t sdl_buttons[6] = {
	    SDL_BUTTON_LEFT,
	    SDL_BUTTON_MIDDLE,
	    SDL_BUTTON_RIGHT,
	    SDL_BUTTON_X1,
	    SDL_BUTTON_X2,
	    99};
	const int8_t expected[6] = {
	    RG_MOUSE_BUTTON_LEFT,
	    RG_MOUSE_BUTTON_MIDDLE,
	    RG_MOUSE_BUTTON_RIGHT,
	    RG_MOUSE_BUTTON_X1,
	    RG_MOUSE_BUTTON_X2,
	    -1};

	rg_input_init(&input);
	rg_input_event_queue_init(&queue, events, 6, NULL, 0);

	for (size_t i = 0; i < 6; i++)
	{
		memset(&event, 0, sizeof(event));
		event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
		event.button.button = sdl_buttons[i];
		rg_input_process_event_ex(&input, &event, &queue);
	}

	TEST_ASSERT(queue.count == 6, "all button events queued");
	for (size_t i = 0; i < 6; i++)
	{
		TEST_ASSERT(events[i].data.button.button == expected[i],
		            "SDL button normalized to rg index");
	}

	TEST_PASS();
	printf("  PASS: mouse button normalization\n");
}

static void test_unsupported_event_is_ignored(void)
{
	RgInputState input;
	RgInputEvent events[1];
	RgInputEventQueue queue;
	SDL_Event event;

	rg_input_init(&input);
	rg_input_event_queue_init(&queue, events, 1, NULL, 0);

	memset(&event, 0, sizeof(event));
	event.type = SDL_EVENT_QUIT;
	rg_input_process_event_ex(&input, &event, &queue);

	TEST_ASSERT(queue.count == 0 && queue.dropped_event_count == 0,
	            "unsupported event ignored without consuming capacity");

	TEST_PASS();
	printf("  PASS: unsupported event ignored\n");
}

int main(void)
{
	printf("rg_input tests\n");
	printf("==============\n");

	test_legacy_event_behavior();
	test_order_and_event_data();
	test_reset_and_modifier_continuity();
	test_atomic_overflow();
	test_mouse_button_normalization();
	test_unsupported_event_is_ignored();

	printf("\n%d passed, %d failed\n", g_tests_passed, g_tests_failed);
	return g_tests_failed == 0 ? 0 : 1;
}
