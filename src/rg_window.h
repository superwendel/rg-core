// rg_window - SDL3 window helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header C99 helper for SDL3 window creation and basic settings.
//
// USAGE:
//   #include "rg_window.h"
//
//   RgWindowDesc desc = {};
//   desc.title = "My Game";
//   desc.width = 1280;
//   desc.height = 720;
//   desc.flags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE;
//
//   SDL_Window* window = rg_window_create(&desc);
//
// OPTIONS:
//   #define RG_WINDOW_ASSERT(x)  - Custom assert macro (default: assert)
//
// NOTES:
//   - All functions have internal linkage and work in unity builds.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_WINDOW_H
#define RG_WINDOW_H

#include "rg_defs.h"

#include <SDL3/SDL.h>

// =============================================================================
// CONFIGURATION
// =============================================================================

#ifndef RG_WINDOW_ASSERT
#include <assert.h>
#define RG_WINDOW_ASSERT(x) assert(x)
#endif

// =============================================================================
// TYPES
// =============================================================================

typedef struct RgWindowDesc
{
	const char* title;
	int width;
	int height;
	u32 flags;
	int min_width;
	int min_height;
	int max_width;
	int max_height;
} RgWindowDesc;

// =============================================================================
// PUBLIC API
// =============================================================================

/**
 * @brief Create an SDL window from a descriptor
 * @param desc Window descriptor
 * @return SDL_Window* or NULL on failure
 */
RGINLINE SDL_Window* rg_window_create(const RgWindowDesc* desc);

/**
 * @brief Destroy an SDL window
 * @param window Window handle
 */
RGINLINE void rg_window_destroy(SDL_Window* window);

/**
 * @brief Get window size in logical pixels
 * @param window Window handle
 * @param width Output width
 * @param height Output height
 */
RGINLINE void rg_window_get_size(SDL_Window* window, int* width, int* height);

/**
 * @brief Set window title
 * @param window Window handle
 * @param title New title
 */
RGINLINE void rg_window_set_title(SDL_Window* window, const char* title);

/**
 * @brief Set window size
 * @param window Window handle
 * @param width Width in logical pixels
 * @param height Height in logical pixels
 */
RGINLINE void rg_window_set_size(SDL_Window* window, int width, int height);

/**
 * @brief Set window minimum size
 * @param window Window handle
 * @param width Minimum width
 * @param height Minimum height
 */
RGINLINE void rg_window_set_min_size(SDL_Window* window, int width, int height);

/**
 * @brief Set window maximum size
 * @param window Window handle
 * @param width Maximum width
 * @param height Maximum height
 */
RGINLINE void rg_window_set_max_size(SDL_Window* window, int width, int height);

/**
 * @brief Show the window
 * @param window Window handle
 */
RGINLINE void rg_window_show(SDL_Window* window);

/**
 * @brief Hide the window
 * @param window Window handle
 */
RGINLINE void rg_window_hide(SDL_Window* window);

/**
 * @brief Raise the window to the front
 * @param window Window handle
 */
RGINLINE void rg_window_raise(SDL_Window* window);

/**
 * @brief Enable or disable fullscreen mode
 * @param window Window handle
 * @param enabled Non-zero to enable
 * @return 1 on success, 0 on failure
 */
RGINLINE int rg_window_set_fullscreen(SDL_Window* window, int enabled);

/**
 * @brief Enable or disable borderless mode
 * @param window Window handle
 * @param borderless Non-zero to enable borderless
 */
RGINLINE void rg_window_set_borderless(SDL_Window* window, int borderless);

/**
 * @brief Enable or disable window resizing
 * @param window Window handle
 * @param resizable Non-zero to enable
 */
RGINLINE void rg_window_set_resizable(SDL_Window* window, int resizable);

/**
 * @brief Get window size in pixels (drawable size)
 * @param window Window handle
 * @param width Output width in pixels
 * @param height Output height in pixels
 */
RGINLINE void rg_window_get_pixel_size(SDL_Window* window, int* width, int* height);

/**
 * @brief Compute content scale (pixels / logical size)
 * @param window Window handle
 * @param scale_x Output scale X
 * @param scale_y Output scale Y
 */
RGINLINE void rg_window_get_content_scale(SDL_Window* window, f32* scale_x, f32* scale_y);

/**
 * @brief Enable or disable relative mouse mode for a window
 * @param window Window handle
 * @param enabled Non-zero to enable
 */
RGINLINE void rg_window_set_relative_mouse(SDL_Window* window, int enabled);

// =============================================================================
// IMPLEMENTATION
// =============================================================================

RGINLINE SDL_Window* rg_window_create(const RgWindowDesc* desc)
{
	RG_WINDOW_ASSERT(desc != NULL);

	const char* title = desc->title ? desc->title : "rg_window";
	int width = desc->width > 0 ? desc->width : 1;
	int height = desc->height > 0 ? desc->height : 1;

	SDL_Window* window = SDL_CreateWindow(title, width, height, (SDL_WindowFlags)desc->flags);
	if (!window)
	{
		return NULL;
	}

	if (desc->min_width > 0 && desc->min_height > 0)
	{
		SDL_SetWindowMinimumSize(window, desc->min_width, desc->min_height);
	}

	if (desc->max_width > 0 && desc->max_height > 0)
	{
		SDL_SetWindowMaximumSize(window, desc->max_width, desc->max_height);
	}

	return window;
}

RGINLINE void rg_window_destroy(SDL_Window* window)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_DestroyWindow(window);
}

RGINLINE void rg_window_get_size(SDL_Window* window, int* width, int* height)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_GetWindowSize(window, width, height);
}

RGINLINE void rg_window_set_title(SDL_Window* window, const char* title)
{
	RG_WINDOW_ASSERT(window != NULL);
	RG_WINDOW_ASSERT(title != NULL);
	SDL_SetWindowTitle(window, title);
}

RGINLINE void rg_window_set_size(SDL_Window* window, int width, int height)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_SetWindowSize(window, width, height);
}

RGINLINE void rg_window_set_min_size(SDL_Window* window, int width, int height)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_SetWindowMinimumSize(window, width, height);
}

RGINLINE void rg_window_set_max_size(SDL_Window* window, int width, int height)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_SetWindowMaximumSize(window, width, height);
}

RGINLINE void rg_window_show(SDL_Window* window)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_ShowWindow(window);
}

RGINLINE void rg_window_hide(SDL_Window* window)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_HideWindow(window);
}

RGINLINE void rg_window_raise(SDL_Window* window)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_RaiseWindow(window);
}

RGINLINE int rg_window_set_fullscreen(SDL_Window* window, int enabled)
{
	RG_WINDOW_ASSERT(window != NULL);
	return SDL_SetWindowFullscreen(window, enabled != 0) ? 1 : 0;
}

RGINLINE void rg_window_set_borderless(SDL_Window* window, int borderless)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_SetWindowBordered(window, borderless ? 0 : 1);
}

RGINLINE void rg_window_set_resizable(SDL_Window* window, int resizable)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_SetWindowResizable(window, resizable ? 1 : 0);
}

RGINLINE void rg_window_get_pixel_size(SDL_Window* window, int* width, int* height)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_GetWindowSizeInPixels(window, width, height);
}

RGINLINE void rg_window_get_content_scale(SDL_Window* window, f32* scale_x, f32* scale_y)
{
	RG_WINDOW_ASSERT(window != NULL);

	int logical_w = 0;
	int logical_h = 0;
	int pixel_w = 0;
	int pixel_h = 0;

	SDL_GetWindowSize(window, &logical_w, &logical_h);
	SDL_GetWindowSizeInPixels(window, &pixel_w, &pixel_h);

	if (scale_x)
	{
		*scale_x = (logical_w > 0) ? ((f32)pixel_w / (f32)logical_w) : 1.0f;
	}
	if (scale_y)
	{
		*scale_y = (logical_h > 0) ? ((f32)pixel_h / (f32)logical_h) : 1.0f;
	}
}

RGINLINE void rg_window_set_relative_mouse(SDL_Window* window, int enabled)
{
	RG_WINDOW_ASSERT(window != NULL);
	SDL_SetWindowRelativeMouseMode(window, enabled != 0);
}

#endif // RG_WINDOW_H
