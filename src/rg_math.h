// rg_math - Fast vector, matrix, geometry, and scalar math
//
// Part of the Reverse Gravity (rg_) core libraries.
// Header-only C99 umbrella for the rg_math modules. Individual module headers
// can be included directly when a smaller surface is useful.
//
// USAGE:
//   #include "rg_math.h"
//
//   rg_vec3 a, b, c;
//   rg_vec3_set(&a, 1, 2, 3);
//   rg_vec3_set(&b, 4, 5, 6);
//   rg_vec3_add(&a, &b, &c);
//   f32 d = rg_vec3_dot(&a, &b);
//
// OPTIONAL INCLUDES:
//   #include "rg_math_scalar.h"  // scalar math helpers
//   #include "rg_math_vec.h"     // vector ops + integer vectors
//   #include "rg_math_mat.h"     // matrix ops
//   #include "rg_math_cam.h"     // camera/projection helpers
//   #include "rg_math_quat.h"    // quaternion ops
//   #include "rg_math_euler.h"   // euler helpers
//   #include "rg_math_color.h"   // color helpers
//   #include "rg_math_curve.h"   // curves + easing helpers
//   #include "rg_math_geom.h"    // geometry + intersections
//   #include "rg_math_io.h"      // debug print helpers
//   #include "rg_math_noise.h"   // perlin noise helpers
//
// OPTIONS:
//   #define RG_MATH_NO_SIMD      - Disable SIMD, use scalar fallback
//   #define RG_MATH_ASSERT(x)    - Custom assert macro
//   #define RG_MATH_MAX_PERF 1   - Default: 1 (enables fastest normalize/length paths; skips sqrt safety checks; uses fast rsqrt)
//   #define RG_MATH_VEC3_PLAIN   - Use f32[4] vec3 layout without __m128 union
//   #define RG_MATH_VEC4_PLAIN   - Use f32[4] vec4 layout without __m128 union
//   #define RG_MATH_USE_LIBC     - Use math.h fallbacks for scalar wrappers (default: 1 when RG_MATH_MAX_PERF)
//   #define RG_MATH_LIBC_ALIASES - Use direct scalar wrappers/macros instead of function-pointer-friendly functions (default: 1 when RG_MATH_USE_LIBC)
//   #define RG_MATH_UNSAFE_NORMALIZE - Skip zero-length checks in normalize for speed
//   #define RG_MATH_FAST_NORMALIZE   - Use faster (less accurate) rsqrt in normalize
//   #define RG_MATH_RESTRICT         - Override restrict keyword for pointer params
//   #define RG_MATH_ASSUME_ALIGNED(ptr, alignment) - Override alignment hints
//   #define RG_MATH_FAST_MATH    - Optional: use faster (less accurate) sqrt; affects rg_sqrtf and rg_vec*_len.
//   #define RG_MATH_FAST_EXP     - Use fast exp/log/pow approximations.
//   #define RG_MATH_FAST_TRIG    - Use fast sin/cos/asin/acos/atan/atan2 approximations for scalar wrapper names.
//   #define RG_MATH_RSQRT_MODE   - Override rsqrt mode (see RG_MATH_RSQRT_MODE_* below)
//   #define RG_MATH_CLIP_CONTROL - Override clip-space/handedness (default: RG_MATH_CLIP_CONTROL_RH_NO)
//   #define RG_MATH_DEFINE_PRINTS     - Force-enable debug printing helpers in release builds
//   #define RG_MATH_PRINT_PRECISION   - Print precision for rg_math_io.h (default: 5)
//   #define RG_MATH_PRINT_MAX_TO_SHORT - Switch to %g above this value (default: 1e5f)
//   #define RG_MATH_PRINT_COLOR       - ANSI color prefix for prints (default: "\033[36m")
//   #define RG_MATH_PRINT_COLOR_RESET - ANSI color reset (default: "\033[0m")
//   #define RG_MATH_PRINT_NO_COLOR    - Disable ANSI coloring for rg_math_io.h output
//   #define RG_MATH_NO_PRINTS_NOOP    - Legacy compat: enable prints even in release builds
//   #define RG_MATH_IO_NO_SPRINTF_INCLUDE - Skip rg_sprintf_hybrid.h include in rg_math_io.h (if included elsewhere)
//   #define RG_MATH_NO_CAM      - Umbrella only: skip camera/projection helpers
//   #define RG_MATH_NO_QUAT     - Umbrella only: skip quaternion helpers
//   #define RG_MATH_NO_EULER    - Umbrella only: skip euler helpers
//   #define RG_MATH_NO_COLOR    - Umbrella only: skip color helpers
//   #define RG_MATH_NO_CURVE    - Umbrella only: skip curves/easing
//   #define RG_MATH_NO_GEOM     - Umbrella only: skip geometry/intersections
//   #define RG_MATH_NO_IO       - Umbrella only: skip debug print helpers
//   #define RG_MATH_NO_NOISE    - Umbrella only: skip noise helpers
//   NOTE: RG_MATH_MAX_PERF does not maintain rg_vec3 padding; use rg_vec3_set or set _pad when writing fields directly if you rely on SIMD loads.
//   NOTE: Trig wrappers rg_sinf/rg_cosf and inverse trig wrappers call math.h directly by default; use RG_MATH_FAST_TRIG or the explicit rg_sinf_fast/rg_cosf_fast/rg_asinf_fast/rg_atan2f_fast APIs for approximations.
//
// FEATURES:
//   - SSE, SSE4.1, FMA, and AVX optimizations when enabled by the x86/x64 compiler target
//   - Pointer-based API for maximum performance
//   - Optional math.h dependency for scalar fallback wrappers
//   - 16-byte aligned types for SIMD
//   - Explicit *_fast APIs trade precision/safety for speed; fast easing uses approximate trig and rg_mat4_decompose_fast uses approximate reciprocal sqrt.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_H
#define RG_MATH_H

#include "rg_math_scalar.h"
#include "rg_math_vec.h"
#include "rg_math_mat.h"

#ifndef RG_MATH_NO_CAM
#include "rg_math_cam.h"
#endif
#ifndef RG_MATH_NO_QUAT
#include "rg_math_quat.h"
#endif
#if !defined(RG_MATH_NO_EULER) && !defined(RG_MATH_NO_QUAT)
#include "rg_math_euler.h"
#endif
#ifndef RG_MATH_NO_COLOR
#include "rg_math_color.h"
#endif
#ifndef RG_MATH_NO_CURVE
#include "rg_math_curve.h"
#endif
#ifndef RG_MATH_NO_GEOM
#include "rg_math_geom.h"
#endif
#if !defined(RG_MATH_NO_IO) && !defined(RG_MATH_NO_GEOM)
#include "rg_math_io.h"
#endif
#ifndef RG_MATH_NO_NOISE
#include "rg_math_noise.h"
#endif

#endif // RG_MATH_H
