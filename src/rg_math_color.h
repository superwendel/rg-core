// rg_math_color - color helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides common linear RGB color calculations.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_COLOR_H
#define RG_MATH_COLOR_H

#include "rg_math_vec.h"

RG_MATH_EXTERN_C_BEGIN

// =============================================================================
// COLOR HELPERS
// =============================================================================

/**
 * @brief Luminance from linear RGB (Rec. 709)
 * @param[in] rgb Linear RGB color
 * @return Luminance value
 */
RGINLINE f32 rg_luminance(const rg_vec3* rgb);

RG_MATH_EXTERN_C_END

// =============================================================================
// IMPLEMENTATION
// =============================================================================

RGINLINE f32 rg_luminance(const rg_vec3* rgb)
{
	return rgb->x * 0.212671f + rgb->y * 0.715160f + rgb->z * 0.072169f;
}

#endif // RG_MATH_COLOR_H
