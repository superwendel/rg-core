// rg_sdl - SDL3 init/teardown helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header C99 helper for SDL3 initialization.
//
// USAGE:
//   #include "rg_sdl.h"
//
//   if (!rg_sdl_init(SDL_INIT_VIDEO))
//   {
//       // handle error
//   }
//
// NOTES:
//   - All functions have internal linkage and work in unity builds.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_SDL_H
#define RG_SDL_H

#include "rg_defs.h"

#include <SDL3/SDL.h>

// =============================================================================
// PUBLIC API
// =============================================================================

/**
 * @brief Initialize SDL3
 * @param flags SDL init flags (e.g. SDL_INIT_VIDEO)
 * @return 1 on success, 0 on failure
 */
RGINLINE int rg_sdl_init(u32 flags);

/**
 * @brief Shutdown SDL3
 */
RGINLINE void rg_sdl_quit(void);

/**
 * @brief Return last SDL error string
 * @return Error string
 */
RGINLINE const char* rg_sdl_error(void);

/**
 * @brief Clear SDL error string
 */
RGINLINE void rg_sdl_clear_error(void);

// =============================================================================
// IMPLEMENTATION
// =============================================================================

RGINLINE int rg_sdl_init(u32 flags)
{
	if (!SDL_Init(flags))
	{
		return 0;
	}

	return 1;
}

RGINLINE void rg_sdl_quit(void)
{
	SDL_Quit();
}

RGINLINE const char* rg_sdl_error(void)
{
	return SDL_GetError();
}

RGINLINE void rg_sdl_clear_error(void)
{
	SDL_ClearError();
}

#endif // RG_SDL_H
