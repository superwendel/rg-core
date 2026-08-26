// rg_math_vec - vector types and operations
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides floating-point and integer vector operations with SIMD fast paths.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_VEC_H
#define RG_MATH_VEC_H

#include "rg_math_scalar.h"

#ifndef RG_MATH_SHUFFLE2
#define RG_MATH_SHUFFLE4(z, y, x, w) (((z) << 6) | ((y) << 4) | ((x) << 2) | (w))
#define RG_MATH_SHUFFLE3(z, y, x) (((z) << 4) | ((y) << 2) | (x))
#define RG_MATH_SHUFFLE2(y, x) (((y) << 2) | (x))
#endif

RG_MATH_EXTERN_C_BEGIN

// =============================================================================
// VECTOR OPERATIONS - POINTER-BASED API
// =============================================================================

// --- Vec2 operations ---

RGINLINE void rg_vec2_set(rg_vec2* v, f32 x, f32 y);
RGINLINE void rg_vec2_add(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE rg_vec2 rg_vec2_add_fast(rg_vec2 a, rg_vec2 b);
RGINLINE void rg_vec2_sub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE rg_vec2 rg_vec2_sub_fast(rg_vec2 a, rg_vec2 b);
RGINLINE void rg_vec2_scale(const rg_vec2* v, f32 s, rg_vec2* out);
RGINLINE rg_vec2 rg_vec2_scale_fast(rg_vec2 v, f32 s);
RGINLINE void rg_vec2_div(const rg_vec2* v, f32 s, rg_vec2* out);
RGINLINE f32 rg_vec2_dot(const rg_vec2* a, const rg_vec2* b);
RGINLINE f32 rg_vec2_dot_fast(rg_vec2 a, rg_vec2 b);
RGINLINE f32 rg_vec2_len(const rg_vec2* v);
RGINLINE f32 rg_vec2_len_fast(rg_vec2 v);
RGINLINE f32 rg_vec2_distance2(const rg_vec2* a, const rg_vec2* b);
RGINLINE f32 rg_vec2_distance(const rg_vec2* a, const rg_vec2* b);
RGINLINE void rg_vec2_min(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_max(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_clamp(const rg_vec2* v, f32 min_val, f32 max_val, rg_vec2* out);
RGINLINE void rg_vec2_lerp(const rg_vec2* a, const rg_vec2* b, f32 t, rg_vec2* out);
RGINLINE void rg_vec2_proj(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE rg_vec2 rg_vec2_proj_fast(const rg_vec2* a, const rg_vec2* b);
RGINLINE void rg_vec2_reflect(const rg_vec2* v, const rg_vec2* n, rg_vec2* out);
RGINLINE int rg_vec2_refract(const rg_vec2* v, const rg_vec2* n, f32 eta, rg_vec2* out);
RGINLINE void rg_vec2_normalize(const rg_vec2* v, rg_vec2* out);
RGINLINE rg_vec2 rg_vec2_normalize_fast(rg_vec2 v);
RGINLINE void rg_vec2_fill(rg_vec2* v, f32 val);
RGINLINE int rg_vec2_eq(const rg_vec2* v, f32 val);
RGINLINE int rg_vec2_eq_eps(const rg_vec2* v, f32 val);
RGINLINE int rg_vec2_eq_all(const rg_vec2* v);
RGINLINE int rg_vec2_eqv(const rg_vec2* a, const rg_vec2* b);
RGINLINE int rg_vec2_eqv_eps(const rg_vec2* a, const rg_vec2* b);
RGINLINE f32 rg_vec2_max_component(const rg_vec2* v);
RGINLINE f32 rg_vec2_min_component(const rg_vec2* v);
RGINLINE int rg_vec2_isnan(const rg_vec2* v);
RGINLINE int rg_vec2_isinf(const rg_vec2* v);
RGINLINE int rg_vec2_isvalid(const rg_vec2* v);
RGINLINE void rg_vec2_sign(const rg_vec2* v, rg_vec2* out);
RGINLINE void rg_vec2_abs(const rg_vec2* v, rg_vec2* out);
RGINLINE void rg_vec2_fract(const rg_vec2* v, rg_vec2* out);
RGINLINE void rg_vec2_floor(const rg_vec2* v, rg_vec2* out);
RGINLINE void rg_vec2_mods(const rg_vec2* v, f32 s, rg_vec2* out);
RGINLINE void rg_vec2_steps(f32 edge, const rg_vec2* v, rg_vec2* out);
RGINLINE void rg_vec2_stepr(const rg_vec2* edge, f32 v, rg_vec2* out);
RGINLINE void rg_vec2_sqrt(const rg_vec2* v, rg_vec2* out);
RGINLINE void rg_vec2_complex_mul(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_complex_div(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_complex_conjugate(const rg_vec2* a, rg_vec2* out);
RGINLINE void rg_vec2_copy(const rg_vec2* a, rg_vec2* out);
RGINLINE void rg_vec2_zero(rg_vec2* v);
RGINLINE void rg_vec2_one(rg_vec2* v);
RGINLINE void rg_vec2_make(const f32* src, rg_vec2* out);
RGINLINE void rg_vec2_adds(const rg_vec2* v, f32 s, rg_vec2* out);
RGINLINE void rg_vec2_subs(const rg_vec2* v, f32 s, rg_vec2* out);
RGINLINE void rg_vec2_mul(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_scale_as(const rg_vec2* v, f32 s, rg_vec2* out);
RGINLINE void rg_vec2_divv(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_divs(const rg_vec2* v, f32 s, rg_vec2* out);
RGINLINE void rg_vec2_addadd(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_subadd(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_muladd(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_muladds(const rg_vec2* a, f32 s, rg_vec2* out);
RGINLINE void rg_vec2_maxadd(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_minadd(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_subsub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_addsub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_mulsub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_mulsubs(const rg_vec2* a, f32 s, rg_vec2* out);
RGINLINE void rg_vec2_maxsub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_minsub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE f32 rg_vec2_cross(const rg_vec2* a, const rg_vec2* b);
RGINLINE f32 rg_vec2_norm2(const rg_vec2* v);
RGINLINE f32 rg_vec2_norm(const rg_vec2* v);
RGINLINE void rg_vec2_minv(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_maxv(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_center(const rg_vec2* a, const rg_vec2* b, rg_vec2* out);
RGINLINE void rg_vec2_rotate(const rg_vec2* v, f32 angle, rg_vec2* out);
RGINLINE void rg_vec2_rotate_sincos(const rg_vec2* v, f32 s, f32 c, rg_vec2* out);
RGINLINE void rg_vec2_negate_to(const rg_vec2* v, rg_vec2* out);
RGINLINE void rg_vec2_negate(rg_vec2* v);
RGINLINE void rg_vec2_normalize_to(const rg_vec2* v, rg_vec2* out);
RGINLINE void rg_vec2_swizzle(const rg_vec2* v, int mask, rg_vec2* out);
RGINLINE void rg_vec2_step(const rg_vec2* edge, const rg_vec2* v, rg_vec2* out);

// --- Vec3 operations (SIMD optimized) ---

RGINLINE void rg_vec3_set(rg_vec3* v, f32 x, f32 y, f32 z);
RGINLINE void rg_vec3_add(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_sub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_scale(const rg_vec3* v, f32 s, rg_vec3* out);
RGINLINE void rg_vec3_div(const rg_vec3* v, f32 s, rg_vec3* out);
RGINLINE f32 rg_vec3_dot(const rg_vec3* a, const rg_vec3* b);
RGINLINE void rg_vec3_cross(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE f32 rg_vec3_len(const rg_vec3* v);
RGINLINE f32 rg_vec3_distance2(const rg_vec3* a, const rg_vec3* b);
RGINLINE f32 rg_vec3_distance(const rg_vec3* a, const rg_vec3* b);
RGINLINE void rg_vec3_min(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_max(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_clamp(const rg_vec3* v, f32 min_val, f32 max_val, rg_vec3* out);
RGINLINE void rg_vec3_lerp(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out);
RGINLINE void rg_vec3_center(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_proj(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE rg_vec3 rg_vec3_proj_fast(const rg_vec3* a, const rg_vec3* b);
RGINLINE void rg_vec3_reflect(const rg_vec3* v, const rg_vec3* n, rg_vec3* out);
RGINLINE int rg_vec3_refract(const rg_vec3* v, const rg_vec3* n, f32 eta, rg_vec3* out);
RGINLINE void rg_vec3_normalize(const rg_vec3* v, rg_vec3* out);
#ifdef RG_MATH_SSE
RGINLINE void rg_vec3_scale_simd(const rg_vec3* v, __m128 scalar, rg_vec3* out);
RGINLINE void rg_vec3_div_simd(const rg_vec3* v, __m128 inv, rg_vec3* out);
#endif

RGINLINE void rg_vec3_broadcast(f32 val, rg_vec3* out);
RGINLINE void rg_vec3_fill(rg_vec3* v, f32 val);
RGINLINE int rg_vec3_eq(const rg_vec3* v, f32 val);
RGINLINE int rg_vec3_eq_eps(const rg_vec3* v, f32 val);
RGINLINE int rg_vec3_eq_all(const rg_vec3* v);
RGINLINE int rg_vec3_eqv(const rg_vec3* a, const rg_vec3* b);
RGINLINE int rg_vec3_eqv_eps(const rg_vec3* a, const rg_vec3* b);
RGINLINE f32 rg_vec3_max_component(const rg_vec3* v);
RGINLINE f32 rg_vec3_min_component(const rg_vec3* v);
RGINLINE int rg_vec3_isnan(const rg_vec3* v);
RGINLINE int rg_vec3_isinf(const rg_vec3* v);
RGINLINE int rg_vec3_isvalid(const rg_vec3* v);
RGINLINE void rg_vec3_sign(const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_abs(const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_fract(const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_floor(const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_mods(const rg_vec3* v, f32 s, rg_vec3* out);
RGINLINE void rg_vec3_steps(f32 edge, const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_stepr(const rg_vec3* edge, f32 v, rg_vec3* out);
RGINLINE f32 rg_vec3_hadd(const rg_vec3* v);
RGINLINE void rg_vec3_sqrt(const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_copy(const rg_vec3* a, rg_vec3* out);
RGINLINE void rg_vec3_zero(rg_vec3* v);
RGINLINE void rg_vec3_one(rg_vec3* v);
RGINLINE void rg_vec3_make(const f32* src, rg_vec3* out);
RGINLINE void rg_vec3_adds(const rg_vec3* v, f32 s, rg_vec3* out);
RGINLINE void rg_vec3_subs(const rg_vec3* v, f32 s, rg_vec3* out);
RGINLINE void rg_vec3_mul(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_scale_as(const rg_vec3* v, f32 s, rg_vec3* out);
RGINLINE void rg_vec3_divv(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_divs(const rg_vec3* v, f32 s, rg_vec3* out);
RGINLINE void rg_vec3_addadd(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_subadd(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_muladd(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_muladds(const rg_vec3* a, f32 s, rg_vec3* out);
RGINLINE void rg_vec3_maxadd(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_minadd(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_subsub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_addsub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_mulsub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_mulsubs(const rg_vec3* a, f32 s, rg_vec3* out);
RGINLINE void rg_vec3_maxsub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_minsub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_negate_to(const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_negate(rg_vec3* v);
RGINLINE void rg_vec3_inv(rg_vec3* v);
RGINLINE void rg_vec3_inv_to(const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_normalize_to(const rg_vec3* v, rg_vec3* out);
RGINLINE f32 rg_vec3_norm2(const rg_vec3* v);
RGINLINE f32 rg_vec3_norm(const rg_vec3* v);
RGINLINE f32 rg_vec3_norm_one(const rg_vec3* v);
RGINLINE f32 rg_vec3_norm_inf(const rg_vec3* v);
RGINLINE void rg_vec3_crossn(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE f32 rg_vec3_angle(const rg_vec3* a, const rg_vec3* b);
RGINLINE void rg_vec3_rotate(const rg_vec3* v, f32 angle, const rg_vec3* axis, rg_vec3* out);
RGINLINE void rg_vec3_rotate_unit_axis(const rg_vec3* v, f32 angle, const rg_vec3* axis_unit, rg_vec3* out);
RGINLINE void rg_vec3_rotate_unit_axis_sincos(const rg_vec3* v, f32 s, f32 c, const rg_vec3* axis_unit, rg_vec3* out);
RGINLINE void rg_vec3_rotate_m4(const rg_mat4* m, const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_rotate_m3(const rg_mat3* m, const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_rotate_m4_fast(const rg_mat4* m, const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_rotate_m3_fast(const rg_mat3* m, const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_minv(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_maxv(const rg_vec3* a, const rg_vec3* b, rg_vec3* out);
RGINLINE void rg_vec3_ortho(const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_lerpc(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out);
RGINLINE void rg_vec3_mix(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out);
RGINLINE void rg_vec3_mixc(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out);
RGINLINE void rg_vec3_step(const rg_vec3* edge, const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_smoothstep_uni(f32 edge0, f32 edge1, const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_smoothstep(const rg_vec3* edge0, const rg_vec3* edge1, const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_vec3_smoothinterp(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out);
RGINLINE void rg_vec3_smoothinterpc(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out);
RGINLINE void rg_vec3_swizzle(const rg_vec3* v, int mask, rg_vec3* out);
RGINLINE void rg_vec3_faceforward(const rg_vec3* n, const rg_vec3* v, const rg_vec3* nref, rg_vec3* out);

// --- Vec4 operations (SIMD optimized) ---

RGINLINE void rg_vec4_set(rg_vec4* v, f32 x, f32 y, f32 z, f32 w);
RGINLINE void rg_vec4_add(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_sub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_scale(const rg_vec4* v, f32 s, rg_vec4* out);
RGINLINE void rg_vec4_div(const rg_vec4* v, f32 s, rg_vec4* out);
RGINLINE f32 rg_vec4_dot(const rg_vec4* a, const rg_vec4* b);
RGINLINE f32 rg_vec4_len(const rg_vec4* v);
RGINLINE f32 rg_vec4_distance2(const rg_vec4* a, const rg_vec4* b);
RGINLINE f32 rg_vec4_distance(const rg_vec4* a, const rg_vec4* b);
RGINLINE void rg_vec4_min(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_max(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_clamp(const rg_vec4* v, f32 min_val, f32 max_val, rg_vec4* out);
RGINLINE void rg_vec4_lerp(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out);
RGINLINE void rg_vec4_proj(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_reflect(const rg_vec4* v, const rg_vec4* n, rg_vec4* out);
RGINLINE int rg_vec4_refract(const rg_vec4* v, const rg_vec4* n, f32 eta, rg_vec4* out);
RGINLINE void rg_vec4_normalize(const rg_vec4* v, rg_vec4* out);
#ifdef RG_MATH_SSE
RGINLINE void rg_vec4_scale_simd(const rg_vec4* v, __m128 scalar, rg_vec4* out);
#endif

RGINLINE void rg_vec4_broadcast(f32 val, rg_vec4* out);
RGINLINE void rg_vec4_fill(rg_vec4* v, f32 val);
RGINLINE int rg_vec4_eq(const rg_vec4* v, f32 val);
RGINLINE int rg_vec4_eq_eps(const rg_vec4* v, f32 val);
RGINLINE int rg_vec4_eq_all(const rg_vec4* v);
RGINLINE int rg_vec4_eqv(const rg_vec4* a, const rg_vec4* b);
RGINLINE int rg_vec4_eqv_eps(const rg_vec4* a, const rg_vec4* b);
RGINLINE f32 rg_vec4_max_component(const rg_vec4* v);
RGINLINE f32 rg_vec4_min_component(const rg_vec4* v);
RGINLINE int rg_vec4_isnan(const rg_vec4* v);
RGINLINE int rg_vec4_isinf(const rg_vec4* v);
RGINLINE int rg_vec4_isvalid(const rg_vec4* v);
RGINLINE void rg_vec4_sign(const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_abs(const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_fract(const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_floor(const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_mods(const rg_vec4* v, f32 s, rg_vec4* out);
RGINLINE void rg_vec4_steps(f32 edge, const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_stepr(const rg_vec4* edge, f32 v, rg_vec4* out);
RGINLINE f32 rg_vec4_hadd(const rg_vec4* v);
RGINLINE void rg_vec4_sqrt(const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_cubic(f32 s, rg_vec4* out);
RGINLINE void rg_vec4_copy3(const rg_vec4* v, rg_vec3* out);
RGINLINE void rg_vec4_copy(const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_ucopy(const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_zero(rg_vec4* v);
RGINLINE void rg_vec4_one(rg_vec4* v);
RGINLINE void rg_vec4_make(const f32* src, rg_vec4* out);
RGINLINE void rg_vec4_adds(const rg_vec4* v, f32 s, rg_vec4* out);
RGINLINE void rg_vec4_subs(const rg_vec4* v, f32 s, rg_vec4* out);
RGINLINE void rg_vec4_mul(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_scale_as(const rg_vec4* v, f32 s, rg_vec4* out);
RGINLINE void rg_vec4_divv(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_divs(const rg_vec4* v, f32 s, rg_vec4* out);
RGINLINE void rg_vec4_addadd(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_subadd(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_muladd(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_muladds(const rg_vec4* a, f32 s, rg_vec4* out);
RGINLINE void rg_vec4_maxadd(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_minadd(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_subsub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_addsub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_mulsub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_mulsubs(const rg_vec4* a, f32 s, rg_vec4* out);
RGINLINE void rg_vec4_maxsub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_minsub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_negate_to(const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_negate(rg_vec4* v);
RGINLINE void rg_vec4_inv(rg_vec4* v);
RGINLINE void rg_vec4_inv_to(const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_normalize_to(const rg_vec4* v, rg_vec4* out);
RGINLINE f32 rg_vec4_norm2(const rg_vec4* v);
RGINLINE f32 rg_vec4_norm(const rg_vec4* v);
RGINLINE f32 rg_vec4_norm_one(const rg_vec4* v);
RGINLINE f32 rg_vec4_norm_inf(const rg_vec4* v);
RGINLINE void rg_vec4_maxv(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_minv(const rg_vec4* a, const rg_vec4* b, rg_vec4* out);
RGINLINE void rg_vec4_lerpc(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out);
RGINLINE void rg_vec4_mix(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out);
RGINLINE void rg_vec4_mixc(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out);
RGINLINE void rg_vec4_step(const rg_vec4* edge, const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_smoothstep_uni(f32 edge0, f32 edge1, const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_smoothstep(const rg_vec4* edge0, const rg_vec4* edge1, const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_vec4_smoothinterp(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out);
RGINLINE void rg_vec4_smoothinterpc(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out);
RGINLINE void rg_vec4_swizzle(const rg_vec4* v, int mask, rg_vec4* out);

// --- Integer vector operations ---

RGINLINE void rg_vec2i_set(rg_vec2i* v, i32 x, i32 y);
RGINLINE void rg_vec2i_copy(const rg_vec2i* a, rg_vec2i* out);
RGINLINE void rg_vec2i_zero(rg_vec2i* v);
RGINLINE void rg_vec2i_one(rg_vec2i* v);
RGINLINE void rg_vec2i_fill(rg_vec2i* v, i32 val);
RGINLINE i32 rg_vec2i_dot(const rg_vec2i* a, const rg_vec2i* b);
RGINLINE i32 rg_vec2i_cross(const rg_vec2i* a, const rg_vec2i* b);
RGINLINE void rg_vec2i_add(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_adds(const rg_vec2i* v, i32 s, rg_vec2i* out);
RGINLINE void rg_vec2i_sub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_subs(const rg_vec2i* v, i32 s, rg_vec2i* out);
RGINLINE void rg_vec2i_mul(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_scale(const rg_vec2i* v, i32 s, rg_vec2i* out);
RGINLINE void rg_vec2i_div(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_divs(const rg_vec2i* v, i32 s, rg_vec2i* out);
RGINLINE void rg_vec2i_mod(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_addadd(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_addadds(const rg_vec2i* a, i32 s, rg_vec2i* out);
RGINLINE void rg_vec2i_subadd(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_subadds(const rg_vec2i* a, i32 s, rg_vec2i* out);
RGINLINE void rg_vec2i_muladd(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_muladds(const rg_vec2i* a, i32 s, rg_vec2i* out);
RGINLINE void rg_vec2i_maxadd(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_minadd(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_subsub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_subsubs(const rg_vec2i* a, i32 s, rg_vec2i* out);
RGINLINE void rg_vec2i_addsub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_addsubs(const rg_vec2i* a, i32 s, rg_vec2i* out);
RGINLINE void rg_vec2i_mulsub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_mulsubs(const rg_vec2i* a, i32 s, rg_vec2i* out);
RGINLINE void rg_vec2i_maxsub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_minsub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE i32 rg_vec2i_distance2(const rg_vec2i* a, const rg_vec2i* b);
RGINLINE f32 rg_vec2i_distance(const rg_vec2i* a, const rg_vec2i* b);
RGINLINE int rg_vec2i_eq(const rg_vec2i* v, i32 val);
RGINLINE int rg_vec2i_eqv(const rg_vec2i* a, const rg_vec2i* b);
RGINLINE void rg_vec2i_maxv(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_minv(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out);
RGINLINE void rg_vec2i_clamp(rg_vec2i* v, i32 min_val, i32 max_val);
RGINLINE void rg_vec2i_abs(const rg_vec2i* v, rg_vec2i* out);

RGINLINE void rg_vec3i_set(rg_vec3i* v, i32 x, i32 y, i32 z);
RGINLINE void rg_vec3i_copy(const rg_vec3i* a, rg_vec3i* out);
RGINLINE void rg_vec3i_zero(rg_vec3i* v);
RGINLINE void rg_vec3i_one(rg_vec3i* v);
RGINLINE void rg_vec3i_fill(rg_vec3i* v, i32 val);
RGINLINE i32 rg_vec3i_dot(const rg_vec3i* a, const rg_vec3i* b);
RGINLINE i32 rg_vec3i_norm2(const rg_vec3i* v);
RGINLINE i32 rg_vec3i_norm(const rg_vec3i* v);
RGINLINE void rg_vec3i_add(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_adds(const rg_vec3i* v, i32 s, rg_vec3i* out);
RGINLINE void rg_vec3i_sub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_subs(const rg_vec3i* v, i32 s, rg_vec3i* out);
RGINLINE void rg_vec3i_mul(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_scale(const rg_vec3i* v, i32 s, rg_vec3i* out);
RGINLINE void rg_vec3i_div(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_divs(const rg_vec3i* v, i32 s, rg_vec3i* out);
RGINLINE void rg_vec3i_mod(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_addadd(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_addadds(const rg_vec3i* a, i32 s, rg_vec3i* out);
RGINLINE void rg_vec3i_subadd(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_subadds(const rg_vec3i* a, i32 s, rg_vec3i* out);
RGINLINE void rg_vec3i_muladd(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_muladds(const rg_vec3i* a, i32 s, rg_vec3i* out);
RGINLINE void rg_vec3i_maxadd(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_minadd(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_subsub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_subsubs(const rg_vec3i* a, i32 s, rg_vec3i* out);
RGINLINE void rg_vec3i_addsub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_addsubs(const rg_vec3i* a, i32 s, rg_vec3i* out);
RGINLINE void rg_vec3i_mulsub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_mulsubs(const rg_vec3i* a, i32 s, rg_vec3i* out);
RGINLINE void rg_vec3i_maxsub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_minsub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE i32 rg_vec3i_distance2(const rg_vec3i* a, const rg_vec3i* b);
RGINLINE f32 rg_vec3i_distance(const rg_vec3i* a, const rg_vec3i* b);
RGINLINE int rg_vec3i_eq(const rg_vec3i* v, i32 val);
RGINLINE int rg_vec3i_eqv(const rg_vec3i* a, const rg_vec3i* b);
RGINLINE void rg_vec3i_maxv(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_minv(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out);
RGINLINE void rg_vec3i_clamp(rg_vec3i* v, i32 min_val, i32 max_val);
RGINLINE void rg_vec3i_abs(const rg_vec3i* v, rg_vec3i* out);

RGINLINE void rg_vec4i_set(rg_vec4i* v, i32 x, i32 y, i32 z, i32 w);
RGINLINE void rg_vec4i_copy(const rg_vec4i* a, rg_vec4i* out);
RGINLINE void rg_vec4i_zero(rg_vec4i* v);
RGINLINE void rg_vec4i_one(rg_vec4i* v);
RGINLINE void rg_vec4i_fill(rg_vec4i* v, i32 val);
RGINLINE i32 rg_vec4i_dot(const rg_vec4i* a, const rg_vec4i* b);
RGINLINE void rg_vec4i_add(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_adds(const rg_vec4i* v, i32 s, rg_vec4i* out);
RGINLINE void rg_vec4i_sub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_subs(const rg_vec4i* v, i32 s, rg_vec4i* out);
RGINLINE void rg_vec4i_mul(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_scale(const rg_vec4i* v, i32 s, rg_vec4i* out);
RGINLINE void rg_vec4i_div(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_divs(const rg_vec4i* v, i32 s, rg_vec4i* out);
RGINLINE void rg_vec4i_mod(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_addadd(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_addadds(const rg_vec4i* a, i32 s, rg_vec4i* out);
RGINLINE void rg_vec4i_subadd(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_subadds(const rg_vec4i* a, i32 s, rg_vec4i* out);
RGINLINE void rg_vec4i_muladd(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_muladds(const rg_vec4i* a, i32 s, rg_vec4i* out);
RGINLINE void rg_vec4i_maxadd(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_minadd(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_subsub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_subsubs(const rg_vec4i* a, i32 s, rg_vec4i* out);
RGINLINE void rg_vec4i_addsub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_addsubs(const rg_vec4i* a, i32 s, rg_vec4i* out);
RGINLINE void rg_vec4i_mulsub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_mulsubs(const rg_vec4i* a, i32 s, rg_vec4i* out);
RGINLINE void rg_vec4i_maxsub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_minsub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE i32 rg_vec4i_distance2(const rg_vec4i* a, const rg_vec4i* b);
RGINLINE f32 rg_vec4i_distance(const rg_vec4i* a, const rg_vec4i* b);
RGINLINE int rg_vec4i_eq(const rg_vec4i* v, i32 val);
RGINLINE int rg_vec4i_eqv(const rg_vec4i* a, const rg_vec4i* b);
RGINLINE void rg_vec4i_maxv(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_minv(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out);
RGINLINE void rg_vec4i_clamp(rg_vec4i* v, i32 min_val, i32 max_val);
RGINLINE void rg_vec4i_abs(const rg_vec4i* v, rg_vec4i* out);

// =============================================================================
// VALUE-BASED CONSTRUCTORS (convenience API)
// =============================================================================

RGINLINE rg_vec2 rg_make_vec2(f32 x, f32 y);
RGINLINE rg_vec3 rg_make_vec3(f32 x, f32 y, f32 z);
RGINLINE rg_vec4 rg_make_vec4(f32 x, f32 y, f32 z, f32 w);
RGINLINE rg_vec2i rg_make_vec2i(i32 x, i32 y);
RGINLINE rg_vec3i rg_make_vec3i(i32 x, i32 y, i32 z);
RGINLINE rg_vec4i rg_make_vec4i(i32 x, i32 y, i32 z, i32 w);

// Shorthand macros for value constructors
#define rg_vec2(x, y) rg_make_vec2((x), (y))
#define rg_vec3(x, y, z) rg_make_vec3((x), (y), (z))
#define rg_vec4(x, y, z, w) rg_make_vec4((x), (y), (z), (w))
#define rg_vec2i(x, y) rg_make_vec2i((x), (y))
#define rg_vec3i(x, y, z) rg_make_vec3i((x), (y), (z))
#define rg_vec4i(x, y, z, w) rg_make_vec4i((x), (y), (z), (w))

// Compatibility aliases (cglm-style)
#ifndef rg_vec3_step_uni
#define rg_vec3_step_uni(edge, v, out) rg_vec3_steps((edge), (v), (out))
#endif
#ifndef rg_vec4_step_uni
#define rg_vec4_step_uni(edge, v, out) rg_vec4_steps((edge), (v), (out))
#endif
#ifndef rg_vec4_dup3
#define rg_vec4_dup3(v, out) rg_vec4_copy3((v), (out))
#endif
#ifndef rg_vec4_dup
#define rg_vec4_dup(v, out) rg_vec4_copy((v), (out))
#endif
#ifndef rg_vec4_flipsign
#define rg_vec4_flipsign(v) rg_vec4_negate((v))
#endif
#ifndef rg_vec4_flipsign_to
#define rg_vec4_flipsign_to(v, out) rg_vec4_negate_to((v), (out))
#endif

RG_MATH_EXTERN_C_END

#if defined(RG_MATH_SSE)
RGINLINE __m128 rg_math_vec_dot3_ps(__m128 a, __m128 b)
{
	__m128 mul = _mm_mul_ps(a, b);
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 1, 2, 1));
	__m128 sums = _mm_add_ss(mul, shuf);
	shuf = _mm_movehl_ps(shuf, mul);
	sums = _mm_add_ss(sums, shuf);
	return _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(0, 0, 0, 0));
}
#endif

// =============================================================================
// Vec2 Implementation
// =============================================================================

RGINLINE void rg_vec2_set(rg_vec2* v, f32 x, f32 y)
{
	v->x = x;
	v->y = y;
}

RGINLINE void rg_vec2_add(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
#if defined(RG_MATH_SSE2) && RG_MATH_MAX_PERF
	__m128 av = _mm_castsi128_ps(_mm_loadl_epi64((const __m128i*)a->data));
	__m128 bv = _mm_castsi128_ps(_mm_loadl_epi64((const __m128i*)b->data));
	__m128 sum = _mm_add_ps(av, bv);
	_mm_storel_epi64((__m128i*)out->data, _mm_castps_si128(sum));
#else
	f32 ax = a->x;
	f32 ay = a->y;
	f32 bx = b->x;
	f32 by = b->y;
	out->x = ax + bx;
	out->y = ay + by;
#endif
}

RGINLINE rg_vec2 rg_vec2_add_fast(rg_vec2 a, rg_vec2 b)
{
	rg_vec2 out;
	out.x = a.x + b.x;
	out.y = a.y + b.y;
	return out;
}

RGINLINE void rg_vec2_sub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
}

RGINLINE rg_vec2 rg_vec2_sub_fast(rg_vec2 a, rg_vec2 b)
{
	rg_vec2 out;
	out.x = a.x - b.x;
	out.y = a.y - b.y;
	return out;
}

RGINLINE void rg_vec2_scale(const rg_vec2* v, f32 s, rg_vec2* out)
{
#if defined(RG_MATH_SSE2) && RG_MATH_MAX_PERF
	__m128 vv = _mm_castsi128_ps(_mm_loadl_epi64((const __m128i*)v->data));
	__m128 scaled = _mm_mul_ps(vv, _mm_set1_ps(s));
	_mm_storel_epi64((__m128i*)out->data, _mm_castps_si128(scaled));
#else
	f32 x = v->x;
	f32 y = v->y;
	out->x = x * s;
	out->y = y * s;
#endif
}

RGINLINE rg_vec2 rg_vec2_scale_fast(rg_vec2 v, f32 s)
{
	rg_vec2 out;
	out.x = v.x * s;
	out.y = v.y * s;
	return out;
}

RGINLINE void rg_vec2_div(const rg_vec2* v, f32 s, rg_vec2* out)
{
	RG_MATH_ASSERT(s != 0.0f);
	f32 inv = 1.0f / s;
	out->x = v->x * inv;
	out->y = v->y * inv;
}

RGINLINE f32 rg_vec2_dot(const rg_vec2* a, const rg_vec2* b)
{
	return a->x * b->x + a->y * b->y;
}

RGINLINE f32 rg_vec2_dot_fast(rg_vec2 a, rg_vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

RGINLINE f32 rg_vec2_len(const rg_vec2* v)
{
#if RG_MATH_MAX_PERF
	f32 x = v->x;
	f32 y = v->y;
	f32 len_sq = x * x + y * y;
	return rg_sqrtf(len_sq);
#else
	RG_UNUSED(v);
	return rg_sqrtf(rg_vec2_dot(v, v));
#endif
}

RGINLINE f32 rg_vec2_len_fast(rg_vec2 v)
{
	f32 len_sq = v.x * v.x + v.y * v.y;
	return rg_sqrtf(len_sq);
}

RGINLINE f32 rg_vec2_distance2(const rg_vec2* a, const rg_vec2* b)
{
	f32 dx = a->x - b->x;
	f32 dy = a->y - b->y;
	return dx * dx + dy * dy;
}

RGINLINE f32 rg_vec2_distance(const rg_vec2* a, const rg_vec2* b)
{
	return rg_sqrtf(rg_vec2_distance2(a, b));
}

RGINLINE void rg_vec2_min(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x = rg_minf(a->x, b->x);
	out->y = rg_minf(a->y, b->y);
}

RGINLINE void rg_vec2_max(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x = rg_maxf(a->x, b->x);
	out->y = rg_maxf(a->y, b->y);
}

RGINLINE void rg_vec2_clamp(const rg_vec2* v, f32 min_val, f32 max_val, rg_vec2* out)
{
	out->x = rg_minf(rg_maxf(v->x, min_val), max_val);
	out->y = rg_minf(rg_maxf(v->y, min_val), max_val);
}

RGINLINE void rg_vec2_lerp(const rg_vec2* a, const rg_vec2* b, f32 t, rg_vec2* out)
{
	out->x = rg_lerpf(a->x, b->x, t);
	out->y = rg_lerpf(a->y, b->y, t);
}

RGINLINE void rg_vec2_proj(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
#if RG_MATH_MAX_PERF
	*out = rg_vec2_proj_fast(a, b);
#else
	f32 ax = a->x;
	f32 ay = a->y;
	f32 bx = b->x;
	f32 by = b->y;
	f32 denom = bx * bx + by * by;
	if (denom < RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		return;
	}
	f32 scale = (ax * bx + ay * by) / denom;
	out->x = bx * scale;
	out->y = by * scale;
#endif
}

RGINLINE rg_vec2 rg_vec2_proj_fast(const rg_vec2* a, const rg_vec2* b)
{
	f32 denom = b->x * b->x + b->y * b->y;
	f32 scale = (a->x * b->x + a->y * b->y) / denom;
	rg_vec2 out;
	out.x = b->x * scale;
	out.y = b->y * scale;
	return out;
}

RGINLINE void rg_vec2_reflect(const rg_vec2* v, const rg_vec2* n, rg_vec2* out)
{
	f32 ndot = v->x * n->x + v->y * n->y;
	f32 scale = 2.0f * ndot;
	out->x = v->x - n->x * scale;
	out->y = v->y - n->y * scale;
}

RGINLINE int rg_vec2_refract(const rg_vec2* v, const rg_vec2* n, f32 eta, rg_vec2* out)
{
	f32 ndi = v->x * n->x + v->y * n->y;
	f32 eni = eta * ndi;
	f32 k = 1.0f - eta * eta + eni * eni;
	if (k < 0.0f)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		return 0;
	}
	f32 scale = eni + rg_sqrtf(k);
	out->x = v->x * eta - n->x * scale;
	out->y = v->y * eta - n->y * scale;
	return 1;
}

RGINLINE void rg_vec2_normalize(const rg_vec2* v, rg_vec2* out)
{
#if defined(RG_MATH_SSE2) && RG_MATH_MAX_PERF
	__m128 vv = _mm_castsi128_ps(_mm_loadl_epi64((const __m128i*)v->data));
	f32 x = v->x;
	f32 y = v->y;
	f32 len_sq = x * x + y * y;
	__m128 normalized = _mm_mul_ps(vv, _mm_set1_ps(rg_rsqrtf_fast(len_sq)));
	_mm_storel_epi64((__m128i*)out->data, _mm_castps_si128(normalized));
#else
	f32 x = v->x;
	f32 y = v->y;
	f32 len_sq = x * x + y * y;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		return;
	}
#endif
#ifdef RG_MATH_FAST_NORMALIZE
	f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
	RG_UNUSED(v);
	f32 inv_len = rg_rsqrtf(len_sq);
#endif
	out->x = x * inv_len;
	out->y = y * inv_len;
#endif
}

RGINLINE rg_vec2 rg_vec2_normalize_fast(rg_vec2 v)
{
	f32 inv_len = rg_rsqrtf_fast(v.x * v.x + v.y * v.y);
	rg_vec2 out;
	out.x = v.x * inv_len;
	out.y = v.y * inv_len;
	return out;
}

RGINLINE void rg_vec2_fill(rg_vec2* v, f32 val)
{
	v->x = val;
	v->y = val;
}

RGINLINE int rg_vec2_eq(const rg_vec2* v, f32 val)
{
	return v->x == val && v->y == val;
}

RGINLINE int rg_vec2_eq_eps(const rg_vec2* v, f32 val)
{
	return rg_absf(v->x - val) <= RG_EPSILON && rg_absf(v->y - val) <= RG_EPSILON;
}

RGINLINE int rg_vec2_eq_all(const rg_vec2* v)
{
	f32 x = v->x;
	rg_float_bits xb;
	rg_float_bits yb;
	xb.f = x;
	yb.f = v->y;
	if ((xb.u & 0x7FFFFFFFu) >= 0x7F800000u || (yb.u & 0x7FFFFFFFu) >= 0x7F800000u)
	{
		return 0;
	}
	return rg_absf(v->y - x) <= RG_EPSILON;
}

RGINLINE int rg_vec2_eqv(const rg_vec2* a, const rg_vec2* b)
{
	return a->x == b->x && a->y == b->y;
}

RGINLINE int rg_vec2_eqv_eps(const rg_vec2* a, const rg_vec2* b)
{
	return rg_absf(a->x - b->x) <= RG_EPSILON && rg_absf(a->y - b->y) <= RG_EPSILON;
}

RGINLINE f32 rg_vec2_max_component(const rg_vec2* v)
{
	return rg_maxf(v->x, v->y);
}

RGINLINE f32 rg_vec2_min_component(const rg_vec2* v)
{
	return rg_minf(v->x, v->y);
}

RGINLINE int rg_vec2_isnan(const rg_vec2* v)
{
#if !RG_MATH_MAX_PERF
	return isnan(v->x) || isnan(v->y);
#else
	RG_UNUSED(v);
	return 0;
#endif
}

RGINLINE int rg_vec2_isinf(const rg_vec2* v)
{
#if !RG_MATH_MAX_PERF
	return isinf(v->x) || isinf(v->y);
#else
	RG_UNUSED(v);
	return 0;
#endif
}

RGINLINE int rg_vec2_isvalid(const rg_vec2* v)
{
	return !rg_vec2_isnan(v) && !rg_vec2_isinf(v);
}

RGINLINE void rg_vec2_sign(const rg_vec2* v, rg_vec2* out)
{
	out->x = rg_signf(v->x);
	out->y = rg_signf(v->y);
}

RGINLINE void rg_vec2_abs(const rg_vec2* v, rg_vec2* out)
{
#if defined(RG_MATH_SSE2) && RG_MATH_MAX_PERF
	__m128i bits = _mm_loadl_epi64((const __m128i*)v->data);
	bits = _mm_and_si128(bits, _mm_set1_epi32(0x7FFFFFFF));
	_mm_storel_epi64((__m128i*)out->data, bits);
#else
	out->x = rg_absf(v->x);
	out->y = rg_absf(v->y);
#endif
}

RGINLINE void rg_vec2_fract(const rg_vec2* v, rg_vec2* out)
{
	const f32 max_fract = 0.99999994f;
	out->x = rg_minf(rg_fracf(v->x), max_fract);
	out->y = rg_minf(rg_fracf(v->y), max_fract);
}

RGINLINE void rg_vec2_floor(const rg_vec2* v, rg_vec2* out)
{
	out->x = rg_floorf(v->x);
	out->y = rg_floorf(v->y);
}

RGINLINE void rg_vec2_mods(const rg_vec2* v, f32 s, rg_vec2* out)
{
	out->x = rg_fmodf(v->x, s);
	out->y = rg_fmodf(v->y, s);
}

RGINLINE void rg_vec2_steps(f32 edge, const rg_vec2* v, rg_vec2* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 vv = _mm_castsi128_ps(_mm_loadl_epi64((const __m128i*)v));
	__m128 cmp = _mm_cmplt_ps(vv, _mm_set1_ps(edge));
	__m128 r = _mm_andnot_ps(cmp, _mm_set1_ps(1.0f));
	_mm_storel_epi64((__m128i*)out, _mm_castps_si128(r));
#else
	out->x = v->x < edge ? 0.0f : 1.0f;
	out->y = v->y < edge ? 0.0f : 1.0f;
#endif
}

RGINLINE void rg_vec2_stepr(const rg_vec2* edge, f32 v, rg_vec2* out)
{
	out->x = v < edge->x ? 0.0f : 1.0f;
	out->y = v < edge->y ? 0.0f : 1.0f;
}

RGINLINE void rg_vec2_sqrt(const rg_vec2* v, rg_vec2* out)
{
	out->x = rg_sqrtf(v->x);
	out->y = rg_sqrtf(v->y);
}

RGINLINE void rg_vec2_complex_mul(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	f32 tr = a->x * b->x - a->y * b->y;
	f32 ti = a->x * b->y + a->y * b->x;
	out->x = tr;
	out->y = ti;
}

RGINLINE void rg_vec2_complex_div(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	f32 denom = b->x * b->x + b->y * b->y;
	RG_MATH_ASSERT(denom != 0.0f);
	f32 inv = 1.0f / denom;
	f32 tr = inv * (a->x * b->x + a->y * b->y);
	f32 ti = inv * (a->y * b->x - a->x * b->y);
	out->x = tr;
	out->y = ti;
}

RGINLINE void rg_vec2_complex_conjugate(const rg_vec2* a, rg_vec2* out)
{
	out->x = a->x;
	out->y = -a->y;
}

RGINLINE void rg_vec2_copy(const rg_vec2* a, rg_vec2* out)
{
	out->x = a->x;
	out->y = a->y;
}

RGINLINE void rg_vec2_zero(rg_vec2* v)
{
	v->x = 0.0f;
	v->y = 0.0f;
}

RGINLINE void rg_vec2_one(rg_vec2* v)
{
	v->x = 1.0f;
	v->y = 1.0f;
}

RGINLINE void rg_vec2_make(const f32* src, rg_vec2* out)
{
	out->x = src[0];
	out->y = src[1];
}

RGINLINE void rg_vec2_adds(const rg_vec2* v, f32 s, rg_vec2* out)
{
	out->x = v->x + s;
	out->y = v->y + s;
}

RGINLINE void rg_vec2_subs(const rg_vec2* v, f32 s, rg_vec2* out)
{
	out->x = v->x - s;
	out->y = v->y - s;
}

RGINLINE void rg_vec2_mul(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x = a->x * b->x;
	out->y = a->y * b->y;
}

RGINLINE void rg_vec2_scale_as(const rg_vec2* v, f32 s, rg_vec2* out)
{
#if RG_MATH_MAX_PERF
#if defined(RG_MATH_SSE)
	__m128 vv = _mm_castsi128_ps(_mm_loadl_epi64((const __m128i*)v));
	__m128 sq = _mm_mul_ps(vv, vv);
	__m128 len_sq = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, _MM_SHUFFLE(1, 1, 1, 1)));
	__m128 scale = _mm_mul_ss(_mm_set_ss(s), _mm_rsqrt_ss(len_sq));
	scale = _mm_shuffle_ps(scale, scale, _MM_SHUFFLE(0, 0, 0, 0));
	_mm_storel_epi64((__m128i*)out, _mm_castps_si128(_mm_mul_ps(vv, scale)));
#else
	f32 len_sq = v->x * v->x + v->y * v->y;
	f32 scale = s * rg_rsqrtf_fast(len_sq);
	out->x = v->x * scale;
	out->y = v->y * scale;
#endif
#else
	f32 len = rg_vec2_len(v);
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len < RG_EPSILON)
	{
		rg_vec2_zero(out);
		return;
	}
#endif
	f32 scale = s / len;
	out->x = v->x * scale;
	out->y = v->y * scale;
#endif
}

RGINLINE void rg_vec2_divv(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	RG_MATH_ASSERT(b->x != 0.0f && b->y != 0.0f);
	out->x = a->x / b->x;
	out->y = a->y / b->y;
}

RGINLINE void rg_vec2_divs(const rg_vec2* v, f32 s, rg_vec2* out)
{
	rg_vec2_div(v, s, out);
}

RGINLINE void rg_vec2_addadd(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x += a->x + b->x;
	out->y += a->y + b->y;
}

RGINLINE void rg_vec2_subadd(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x += a->x - b->x;
	out->y += a->y - b->y;
}

RGINLINE void rg_vec2_muladd(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x += a->x * b->x;
	out->y += a->y * b->y;
}

RGINLINE void rg_vec2_muladds(const rg_vec2* a, f32 s, rg_vec2* out)
{
	out->x += a->x * s;
	out->y += a->y * s;
}

RGINLINE void rg_vec2_maxadd(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x += rg_maxf(a->x, b->x);
	out->y += rg_maxf(a->y, b->y);
}

RGINLINE void rg_vec2_minadd(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x += rg_minf(a->x, b->x);
	out->y += rg_minf(a->y, b->y);
}

RGINLINE void rg_vec2_subsub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x -= a->x - b->x;
	out->y -= a->y - b->y;
}

RGINLINE void rg_vec2_addsub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x -= a->x + b->x;
	out->y -= a->y + b->y;
}

RGINLINE void rg_vec2_mulsub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x -= a->x * b->x;
	out->y -= a->y * b->y;
}

RGINLINE void rg_vec2_mulsubs(const rg_vec2* a, f32 s, rg_vec2* out)
{
	out->x -= a->x * s;
	out->y -= a->y * s;
}

RGINLINE void rg_vec2_maxsub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x -= rg_maxf(a->x, b->x);
	out->y -= rg_maxf(a->y, b->y);
}

RGINLINE void rg_vec2_minsub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x -= rg_minf(a->x, b->x);
	out->y -= rg_minf(a->y, b->y);
}

RGINLINE f32 rg_vec2_cross(const rg_vec2* a, const rg_vec2* b)
{
	return a->x * b->y - a->y * b->x;
}

RGINLINE f32 rg_vec2_norm2(const rg_vec2* v)
{
	return rg_vec2_dot(v, v);
}

RGINLINE f32 rg_vec2_norm(const rg_vec2* v)
{
	return rg_vec2_len(v);
}

RGINLINE void rg_vec2_minv(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	rg_vec2_min(a, b, out);
}

RGINLINE void rg_vec2_maxv(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	rg_vec2_max(a, b, out);
}

RGINLINE void rg_vec2_center(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x = (a->x + b->x) * 0.5f;
	out->y = (a->y + b->y) * 0.5f;
}

RGINLINE void rg_vec2_rotate(const rg_vec2* v, f32 angle, rg_vec2* out)
{
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);
	f32 x = v->x;
	f32 y = v->y;

	out->x = c * x - s * y;
	out->y = s * x + c * y;
}

RGINLINE void rg_vec2_rotate_sincos(const rg_vec2* v, f32 s, f32 c, rg_vec2* out)
{
	f32 x = v->x;
	f32 y = v->y;
	out->x = c * x - s * y;
	out->y = s * x + c * y;
}

RGINLINE void rg_vec2_negate_to(const rg_vec2* v, rg_vec2* out)
{
	out->x = -v->x;
	out->y = -v->y;
}

RGINLINE void rg_vec2_negate(rg_vec2* v)
{
	rg_vec2_negate_to(v, v);
}

RGINLINE void rg_vec2_normalize_to(const rg_vec2* v, rg_vec2* out)
{
	rg_vec2_normalize(v, out);
}

RGINLINE void rg_vec2_swizzle(const rg_vec2* v, int mask, rg_vec2* out)
{
	rg_vec2 t;
	t.x = v->data[(mask >> 2) & 0x3];
	t.y = v->data[mask & 0x3];
	out->x = t.x;
	out->y = t.y;
}

RGINLINE void rg_vec2_step(const rg_vec2* edge, const rg_vec2* v, rg_vec2* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 ev = _mm_castsi128_ps(_mm_loadl_epi64((const __m128i*)edge));
	__m128 vv = _mm_castsi128_ps(_mm_loadl_epi64((const __m128i*)v));
	__m128 cmp = _mm_cmplt_ps(vv, ev);
	__m128 r = _mm_andnot_ps(cmp, _mm_set1_ps(1.0f));
	_mm_storel_epi64((__m128i*)out, _mm_castps_si128(r));
#else
	out->x = (f32) !(v->x < edge->x);
	out->y = (f32) !(v->y < edge->y);
#endif
}

// =============================================================================
// Vec3 Implementation (SIMD optimized)
// =============================================================================

RGINLINE void rg_vec3_set(rg_vec3* v, f32 x, f32 y, f32 z)
{
#ifdef RG_MATH_SSE
	RG_VEC3_STORE(v, _mm_setr_ps(x, y, z, 0.0f));
#else
	RG_UNUSED(v);
	v->x = x;
	v->y = y;
	v->z = z;
	v->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_add(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_add_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b)));
#else
	const rg_vec3* a_aligned = RG_MATH_ASSUME_ALIGNED(a, 16);
	const rg_vec3* b_aligned = RG_MATH_ASSUME_ALIGNED(b, 16);
	rg_vec3* out_aligned = RG_MATH_ASSUME_ALIGNED(out, 16);
	out_aligned->x = a_aligned->x + b_aligned->x;
	out_aligned->y = a_aligned->y + b_aligned->y;
	out_aligned->z = a_aligned->z + b_aligned->z;
#if !RG_MATH_MAX_PERF
	out_aligned->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_sub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#ifdef RG_MATH_SSE
	RG_VEC3_STORE(out, _mm_sub_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b)));
#else
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
#endif
}

RGINLINE void rg_vec3_scale(const rg_vec3* v, f32 s, rg_vec3* out)
{
#if RG_MATH_MAX_PERF
	out->x = v->x * s;
	out->y = v->y * s;
	out->z = v->z * s;
#else
#ifdef RG_MATH_SSE
	__m128 scalar = _mm_set1_ps(s);
	RG_VEC3_STORE(out, _mm_mul_ps(RG_VEC3_LOAD(v), scalar));
#else
	out->x = v->x * s;
	out->y = v->y * s;
	out->z = v->z * s;
#endif
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_div(const rg_vec3* v, f32 s, rg_vec3* out)
{
	RG_MATH_ASSERT(s != 0.0f);
	f32 inv = 1.0f / s;
	out->x = v->x * inv;
	out->y = v->y * inv;
	out->z = v->z * inv;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE f32 rg_vec3_dot(const rg_vec3* a, const rg_vec3* b)
{
	const rg_vec3* a_aligned = RG_MATH_ASSUME_ALIGNED(a, 16);
	const rg_vec3* b_aligned = RG_MATH_ASSUME_ALIGNED(b, 16);
	return a_aligned->x * b_aligned->x + a_aligned->y * b_aligned->y + a_aligned->z * b_aligned->z;
}

RGINLINE void rg_vec3_cross(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = RG_VEC3_LOAD(a);
	__m128 bv = RG_VEC3_LOAD(b);
	__m128 ayzx = _mm_shuffle_ps(av, av, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 azxy = _mm_shuffle_ps(av, av, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 byzx = _mm_shuffle_ps(bv, bv, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 bzxy = _mm_shuffle_ps(bv, bv, _MM_SHUFFLE(3, 1, 0, 2));
	RG_VEC3_STORE(out, _mm_sub_ps(_mm_mul_ps(ayzx, bzxy), _mm_mul_ps(azxy, byzx)));
#else
	// Scalar is faster for vec3 - SIMD shuffle overhead not worth it for 6 muls + 3 subs
	f32 rx = a->y * b->z - a->z * b->y;
	f32 ry = a->z * b->x - a->x * b->z;
	f32 rz = a->x * b->y - a->y * b->x;
	out->x = rx;
	out->y = ry;
	out->z = rz;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE f32 rg_vec3_len(const rg_vec3* v)
{
#if RG_MATH_MAX_PERF
#if defined(RG_MATH_SSE41)
	__m128 vv = RG_VEC3_LOAD(v);
	return rg_sqrtf(_mm_cvtss_f32(_mm_dp_ps(vv, vv, 0x71)));
#else
	f32 len_sq = v->x * v->x + v->y * v->y + v->z * v->z;
	return rg_sqrtf(len_sq);
#endif
#else
	return rg_sqrtf(rg_vec3_dot(v, v));
#endif
}

RGINLINE f32 rg_vec3_distance2(const rg_vec3* a, const rg_vec3* b)
{
	const rg_vec3* a_aligned = RG_MATH_ASSUME_ALIGNED(a, 16);
	const rg_vec3* b_aligned = RG_MATH_ASSUME_ALIGNED(b, 16);
	f32 dx = a_aligned->x - b_aligned->x;
	f32 dy = a_aligned->y - b_aligned->y;
	f32 dz = a_aligned->z - b_aligned->z;
	return dx * dx + dy * dy + dz * dz;
}

RGINLINE f32 rg_vec3_distance(const rg_vec3* a, const rg_vec3* b)
{
	return rg_sqrtf(rg_vec3_distance2(a, b));
}

RGINLINE void rg_vec3_min(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_min_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b)));
#else
	out->x = rg_minf(a->x, b->x);
	out->y = rg_minf(a->y, b->y);
	out->z = rg_minf(a->z, b->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_max(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_max_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b)));
#else
	out->x = rg_maxf(a->x, b->x);
	out->y = rg_maxf(a->y, b->y);
	out->z = rg_maxf(a->z, b->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_clamp(const rg_vec3* v, f32 min_val, f32 max_val, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 v3 = RG_VEC3_LOAD(v);
	__m128 vmin = _mm_set1_ps(min_val);
	__m128 vmax = _mm_set1_ps(max_val);
	RG_VEC3_STORE(out, _mm_min_ps(_mm_max_ps(v3, vmin), vmax));
#else
	out->x = rg_clampf(v->x, min_val, max_val);
	out->y = rg_clampf(v->y, min_val, max_val);
	out->z = rg_clampf(v->z, min_val, max_val);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_lerp(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = RG_VEC3_LOAD(a);
	__m128 bv = RG_VEC3_LOAD(b);
	__m128 tv = _mm_set1_ps(t);
#if defined(RG_MATH_FMA)
	RG_VEC3_STORE(out, _mm_fmadd_ps(_mm_sub_ps(bv, av), tv, av));
#else
	RG_VEC3_STORE(out, _mm_add_ps(av, _mm_mul_ps(_mm_sub_ps(bv, av), tv)));
#endif
#else
	out->x = rg_lerpf(a->x, b->x, t);
	out->y = rg_lerpf(a->y, b->y, t);
	out->z = rg_lerpf(a->z, b->z, t);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_center(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
	out->x = (a->x + b->x) * 0.5f;
	out->y = (a->y + b->y) * 0.5f;
	out->z = (a->z + b->z) * 0.5f;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_proj(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
	f32 ax = a->x;
	f32 ay = a->y;
	f32 az = a->z;
	f32 bx = b->x;
	f32 by = b->y;
	f32 bz = b->z;
	f32 denom = bx * bx + by * by + bz * bz;
#if !RG_MATH_MAX_PERF
	if (denom < RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->_pad = 0.0f;
		return;
	}
#endif
	f32 scale = (ax * bx + ay * by + az * bz) / denom;
	out->x = bx * scale;
	out->y = by * scale;
	out->z = bz * scale;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE rg_vec3 rg_vec3_proj_fast(const rg_vec3* a, const rg_vec3* b)
{
	f32 bx = b->x;
	f32 by = b->y;
	f32 bz = b->z;
	f32 denom = bx * bx + by * by + bz * bz;
	f32 dot = a->x * bx + a->y * by + a->z * bz;
#if defined(RG_MATH_SSE)
	__m128 denom_v = _mm_set_ss(denom);
	__m128 inv = _mm_rcp_ss(denom_v);
	inv = _mm_mul_ss(inv, _mm_sub_ss(_mm_set_ss(2.0f), _mm_mul_ss(denom_v, inv)));
	f32 scale = dot * _mm_cvtss_f32(inv);
#else
	f32 scale = dot / denom;
#endif
	rg_vec3 out;
	out.x = bx * scale;
	out.y = by * scale;
	out.z = bz * scale;
	out._pad = 0.0f;
	return out;
}

RGINLINE void rg_vec3_reflect(const rg_vec3* v, const rg_vec3* n, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 vv = RG_VEC3_LOAD(v);
	__m128 nn = RG_VEC3_LOAD(n);
	__m128 dot = rg_math_vec_dot3_ps(vv, nn);
	__m128 scale = _mm_add_ps(dot, dot);
	RG_VEC3_STORE(out, _mm_sub_ps(vv, _mm_mul_ps(nn, scale)));
#else
	f32 ndot = v->x * n->x + v->y * n->y + v->z * n->z;
	f32 scale = 2.0f * ndot;
	out->x = v->x - n->x * scale;
	out->y = v->y - n->y * scale;
	out->z = v->z - n->z * scale;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE int rg_vec3_refract(const rg_vec3* v, const rg_vec3* n, f32 eta, rg_vec3* out)
{
	f32 ndi = v->x * n->x + v->y * n->y + v->z * n->z;
	f32 eni = eta * ndi;
	f32 k = 1.0f - eta * eta + eni * eni;
	if (k < 0.0f)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
#if !RG_MATH_MAX_PERF
		out->_pad = 0.0f;
#endif
		return 0;
	}
	f32 scale = eni + rg_sqrtf(k);
	out->x = v->x * eta - n->x * scale;
	out->y = v->y * eta - n->y * scale;
	out->z = v->z * eta - n->z * scale;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
	return 1;
}

RGINLINE void rg_vec3_normalize(const rg_vec3* v, rg_vec3* out)
{
#if defined(RG_MATH_SSE41) && defined(RG_MATH_FAST_NORMALIZE)
	__m128 vv = RG_VEC3_LOAD(v);
	__m128 len_sq = _mm_dp_ps(vv, vv, 0x71);
	__m128 inv_len = _mm_rsqrt_ss(len_sq);
	inv_len = _mm_shuffle_ps(inv_len, inv_len, _MM_SHUFFLE(0, 0, 0, 0));
	RG_VEC3_STORE(out, _mm_mul_ps(vv, inv_len));
#elif defined(RG_MATH_SSE41) && RG_MATH_MAX_PERF
	__m128 vv = RG_VEC3_LOAD(v);
	__m128 len_sqv = _mm_dp_ps(vv, vv, 0x71);
	f32 len_sq = _mm_cvtss_f32(len_sqv);
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		return;
	}
#endif
	RG_VEC3_STORE(out, _mm_mul_ps(vv, _mm_set1_ps(rg_rsqrtf(len_sq))));
#else
	f32 len_sq = v->x * v->x + v->y * v->y + v->z * v->z;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		return;
	}
#endif
#ifdef RG_MATH_FAST_NORMALIZE
	f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
	f32 inv_len = rg_rsqrtf(len_sq);
#endif
	out->x = v->x * inv_len;
	out->y = v->y * inv_len;
	out->z = v->z * inv_len;
#endif
}

#ifdef RG_MATH_SSE
RGINLINE void rg_vec3_scale_simd(const rg_vec3* v, __m128 scalar, rg_vec3* out)
{
	RG_VEC3_STORE(out, _mm_mul_ps(RG_VEC3_LOAD(v), scalar));
}

RGINLINE void rg_vec3_div_simd(const rg_vec3* v, __m128 inv, rg_vec3* out)
{
	RG_VEC3_STORE(out, _mm_mul_ps(RG_VEC3_LOAD(v), inv));
}
#endif

RGINLINE void rg_vec3_broadcast(f32 val, rg_vec3* out)
{
	out->x = val;
	out->y = val;
	out->z = val;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_fill(rg_vec3* v, f32 val)
{
	rg_vec3_broadcast(val, v);
}

RGINLINE int rg_vec3_eq(const rg_vec3* v, f32 val)
{
	return v->x == val && v->y == val && v->z == val;
}

RGINLINE int rg_vec3_eq_eps(const rg_vec3* v, f32 val)
{
	return rg_absf(v->x - val) <= RG_EPSILON && rg_absf(v->y - val) <= RG_EPSILON && rg_absf(v->z - val) <= RG_EPSILON;
}

RGINLINE int rg_vec3_eq_all(const rg_vec3* v)
{
	f32 x = v->x;
	rg_float_bits xb;
	rg_float_bits yb;
	rg_float_bits zb;
	xb.f = x;
	yb.f = v->y;
	zb.f = v->z;
	if ((xb.u & 0x7FFFFFFFu) >= 0x7F800000u ||
	    (yb.u & 0x7FFFFFFFu) >= 0x7F800000u ||
	    (zb.u & 0x7FFFFFFFu) >= 0x7F800000u)
	{
		return 0;
	}
	return rg_absf(v->y - x) <= RG_EPSILON && rg_absf(v->z - x) <= RG_EPSILON;
}

RGINLINE int rg_vec3_eqv(const rg_vec3* a, const rg_vec3* b)
{
	return a->x == b->x && a->y == b->y && a->z == b->z;
}

RGINLINE int rg_vec3_eqv_eps(const rg_vec3* a, const rg_vec3* b)
{
	return rg_absf(a->x - b->x) <= RG_EPSILON && rg_absf(a->y - b->y) <= RG_EPSILON && rg_absf(a->z - b->z) <= RG_EPSILON;
}

RGINLINE f32 rg_vec3_max_component(const rg_vec3* v)
{
	f32 max_val = v->x;
	if (v->y > max_val) max_val = v->y;
	if (v->z > max_val) max_val = v->z;
	return max_val;
}

RGINLINE f32 rg_vec3_min_component(const rg_vec3* v)
{
	f32 min_val = v->x;
	if (v->y < min_val) min_val = v->y;
	if (v->z < min_val) min_val = v->z;
	return min_val;
}

RGINLINE int rg_vec3_isnan(const rg_vec3* v)
{
#if !RG_MATH_MAX_PERF
	return isnan(v->x) || isnan(v->y) || isnan(v->z);
#else
	RG_UNUSED(v);
	return 0;
#endif
}

RGINLINE int rg_vec3_isinf(const rg_vec3* v)
{
#if !RG_MATH_MAX_PERF
	return isinf(v->x) || isinf(v->y) || isinf(v->z);
#else
	RG_UNUSED(v);
	return 0;
#endif
}

RGINLINE int rg_vec3_isvalid(const rg_vec3* v)
{
	return !rg_vec3_isnan(v) && !rg_vec3_isinf(v);
}

RGINLINE void rg_vec3_sign(const rg_vec3* v, rg_vec3* out)
{
	out->x = rg_signf(v->x);
	out->y = rg_signf(v->y);
	out->z = rg_signf(v->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_abs(const rg_vec3* v, rg_vec3* out)
{
#if defined(RG_MATH_SSE2) && RG_MATH_MAX_PERF
	const __m128 mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));
	RG_VEC3_STORE(out, _mm_and_ps(RG_VEC3_LOAD(v), mask));
#else
	out->x = rg_absf(v->x);
	out->y = rg_absf(v->y);
	out->z = rg_absf(v->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_fract(const rg_vec3* v, rg_vec3* out)
{
	const f32 max_fract = 0.99999994f;
#if defined(RG_MATH_SSE41) && RG_MATH_MAX_PERF
	const __m128 vv = RG_VEC3_LOAD(v);
	const __m128 fract = _mm_sub_ps(vv, _mm_floor_ps(vv));
	RG_VEC3_STORE(out, _mm_min_ps(fract, _mm_set1_ps(max_fract)));
#else
	out->x = rg_minf(rg_fracf(v->x), max_fract);
	out->y = rg_minf(rg_fracf(v->y), max_fract);
	out->z = rg_minf(rg_fracf(v->z), max_fract);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_floor(const rg_vec3* v, rg_vec3* out)
{
#if defined(RG_MATH_SSE41) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_floor_ps(RG_VEC3_LOAD(v)));
#else
	out->x = rg_floorf(v->x);
	out->y = rg_floorf(v->y);
	out->z = rg_floorf(v->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_mods(const rg_vec3* v, f32 s, rg_vec3* out)
{
	out->x = rg_fmodf(v->x, s);
	out->y = rg_fmodf(v->y, s);
	out->z = rg_fmodf(v->z, s);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_steps(f32 edge, const rg_vec3* v, rg_vec3* out)
{
	out->x = (f32) !(v->x < edge);
	out->y = (f32) !(v->y < edge);
	out->z = (f32) !(v->z < edge);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_stepr(const rg_vec3* edge, f32 v, rg_vec3* out)
{
	out->x = (f32) !(v < edge->x);
	out->y = (f32) !(v < edge->y);
	out->z = (f32) !(v < edge->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE f32 rg_vec3_hadd(const rg_vec3* v)
{
	return v->x + v->y + v->z;
}

RGINLINE void rg_vec3_sqrt(const rg_vec3* v, rg_vec3* out)
{
	out->x = rg_sqrtf(v->x);
	out->y = rg_sqrtf(v->y);
	out->z = rg_sqrtf(v->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_copy(const rg_vec3* a, rg_vec3* out)
{
#if RG_MATH_MAX_PERF
	*out = *a;
#else
	out->x = a->x;
	out->y = a->y;
	out->z = a->z;
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_zero(rg_vec3* v)
{
	v->x = 0.0f;
	v->y = 0.0f;
	v->z = 0.0f;
#if !RG_MATH_MAX_PERF
	v->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_one(rg_vec3* v)
{
	v->x = 1.0f;
	v->y = 1.0f;
	v->z = 1.0f;
#if !RG_MATH_MAX_PERF
	v->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_make(const f32* src, rg_vec3* out)
{
	out->x = src[0];
	out->y = src[1];
	out->z = src[2];
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_adds(const rg_vec3* v, f32 s, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_add_ps(RG_VEC3_LOAD(v), _mm_set1_ps(s)));
#else
	out->x = v->x + s;
	out->y = v->y + s;
	out->z = v->z + s;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_subs(const rg_vec3* v, f32 s, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_sub_ps(RG_VEC3_LOAD(v), _mm_set1_ps(s)));
#else
	out->x = v->x - s;
	out->y = v->y - s;
	out->z = v->z - s;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_mul(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_mul_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b)));
#else
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_scale_as(const rg_vec3* v, f32 s, rg_vec3* out)
{
#if RG_MATH_MAX_PERF
	f32 len_sq = v->x * v->x + v->y * v->y + v->z * v->z;
	f32 scale = s * rg_rsqrtf(len_sq);
	out->x = v->x * scale;
	out->y = v->y * scale;
	out->z = v->z * scale;
#else
	f32 len = rg_vec3_len(v);
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len < RG_EPSILON)
	{
		rg_vec3_zero(out);
		return;
	}
#endif
	f32 scale = s / len;
	out->x = v->x * scale;
	out->y = v->y * scale;
	out->z = v->z * scale;
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_divv(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
	RG_MATH_ASSERT(b->x != 0.0f && b->y != 0.0f && b->z != 0.0f);
	out->x = a->x / b->x;
	out->y = a->y / b->y;
	out->z = a->z / b->z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_divs(const rg_vec3* v, f32 s, rg_vec3* out)
{
	rg_vec3_div(v, s, out);
}

RGINLINE void rg_vec3_addadd(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_add_ps(RG_VEC3_LOAD(out), _mm_add_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b))));
#else
	out->x += a->x + b->x;
	out->y += a->y + b->y;
	out->z += a->z + b->z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_subadd(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_add_ps(RG_VEC3_LOAD(out), _mm_sub_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b))));
#else
	out->x += a->x - b->x;
	out->y += a->y - b->y;
	out->z += a->z - b->z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_muladd(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = RG_VEC3_LOAD(a);
	__m128 bv = RG_VEC3_LOAD(b);
	__m128 ov = RG_VEC3_LOAD(out);
#if defined(RG_MATH_FMA)
	RG_VEC3_STORE(out, _mm_fmadd_ps(av, bv, ov));
#else
	RG_VEC3_STORE(out, _mm_add_ps(ov, _mm_mul_ps(av, bv)));
#endif
#else
	out->x += a->x * b->x;
	out->y += a->y * b->y;
	out->z += a->z * b->z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_muladds(const rg_vec3* a, f32 s, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = RG_VEC3_LOAD(a);
	__m128 sv = _mm_set1_ps(s);
	__m128 ov = RG_VEC3_LOAD(out);
#if defined(RG_MATH_FMA)
	RG_VEC3_STORE(out, _mm_fmadd_ps(av, sv, ov));
#else
	RG_VEC3_STORE(out, _mm_add_ps(ov, _mm_mul_ps(av, sv)));
#endif
#else
	out->x += a->x * s;
	out->y += a->y * s;
	out->z += a->z * s;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_maxadd(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_add_ps(RG_VEC3_LOAD(out), _mm_max_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b))));
#else
	out->x += rg_maxf(a->x, b->x);
	out->y += rg_maxf(a->y, b->y);
	out->z += rg_maxf(a->z, b->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_minadd(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_add_ps(RG_VEC3_LOAD(out), _mm_min_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b))));
#else
	out->x += rg_minf(a->x, b->x);
	out->y += rg_minf(a->y, b->y);
	out->z += rg_minf(a->z, b->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_subsub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_sub_ps(RG_VEC3_LOAD(out), _mm_sub_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b))));
#else
	out->x -= a->x - b->x;
	out->y -= a->y - b->y;
	out->z -= a->z - b->z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_addsub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_sub_ps(RG_VEC3_LOAD(out), _mm_add_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b))));
#else
	out->x -= a->x + b->x;
	out->y -= a->y + b->y;
	out->z -= a->z + b->z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_mulsub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, RG_MATH_FNMADD_PS(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b), RG_VEC3_LOAD(out)));
#else
	out->x -= a->x * b->x;
	out->y -= a->y * b->y;
	out->z -= a->z * b->z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_mulsubs(const rg_vec3* a, f32 s, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, RG_MATH_FNMADD_PS(RG_VEC3_LOAD(a), _mm_set1_ps(s), RG_VEC3_LOAD(out)));
#else
	out->x -= a->x * s;
	out->y -= a->y * s;
	out->z -= a->z * s;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_maxsub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_sub_ps(RG_VEC3_LOAD(out), _mm_max_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b))));
#else
	out->x -= rg_maxf(a->x, b->x);
	out->y -= rg_maxf(a->y, b->y);
	out->z -= rg_maxf(a->z, b->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_minsub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, _mm_sub_ps(RG_VEC3_LOAD(out), _mm_min_ps(RG_VEC3_LOAD(a), RG_VEC3_LOAD(b))));
#else
	out->x -= rg_minf(a->x, b->x);
	out->y -= rg_minf(a->y, b->y);
	out->z -= rg_minf(a->z, b->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_negate_to(const rg_vec3* v, rg_vec3* out)
{
	out->x = -v->x;
	out->y = -v->y;
	out->z = -v->z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_negate(rg_vec3* v)
{
	rg_vec3_negate_to(v, v);
}

RGINLINE void rg_vec3_inv(rg_vec3* v)
{
	rg_vec3_negate_to(v, v);
}

RGINLINE void rg_vec3_inv_to(const rg_vec3* v, rg_vec3* out)
{
	rg_vec3_negate_to(v, out);
}

RGINLINE void rg_vec3_normalize_to(const rg_vec3* v, rg_vec3* out)
{
	rg_vec3_normalize(v, out);
}

RGINLINE f32 rg_vec3_norm2(const rg_vec3* v)
{
	return rg_vec3_dot(v, v);
}

RGINLINE f32 rg_vec3_norm(const rg_vec3* v)
{
	return rg_vec3_len(v);
}

RGINLINE f32 rg_vec3_norm_one(const rg_vec3* v)
{
	return rg_absf(v->x) + rg_absf(v->y) + rg_absf(v->z);
}

RGINLINE f32 rg_vec3_norm_inf(const rg_vec3* v)
{
	f32 ax = rg_absf(v->x);
	f32 ay = rg_absf(v->y);
	f32 az = rg_absf(v->z);
	return rg_maxf(ax, rg_maxf(ay, az));
}

RGINLINE void rg_vec3_crossn(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = RG_VEC3_LOAD(a);
	__m128 bv = RG_VEC3_LOAD(b);
	__m128 a_yzx = _mm_shuffle_ps(av, av, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 a_zxy = _mm_shuffle_ps(av, av, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 b_yzx = _mm_shuffle_ps(bv, bv, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 b_zxy = _mm_shuffle_ps(bv, bv, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 cv = _mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), _mm_mul_ps(a_zxy, b_yzx));
#if defined(RG_MATH_SSE41)
	__m128 len_sq = _mm_dp_ps(cv, cv, 0x71);
#else
	__m128 mul = _mm_mul_ps(cv, cv);
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 1, 2, 1));
	__m128 sums = _mm_add_ss(mul, shuf);
	shuf = _mm_movehl_ps(shuf, mul);
	__m128 len_sq = _mm_add_ss(sums, shuf);
#endif
	__m128 inv_len = _mm_rsqrt_ss(len_sq);
	inv_len = _mm_shuffle_ps(inv_len, inv_len, _MM_SHUFFLE(0, 0, 0, 0));
	RG_VEC3_STORE(out, _mm_mul_ps(cv, inv_len));
#else
	f32 cx = a->y * b->z - a->z * b->y;
	f32 cy = a->z * b->x - a->x * b->z;
	f32 cz = a->x * b->y - a->y * b->x;
	f32 len_sq = cx * cx + cy * cy + cz * cz;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		return;
	}
#endif
#ifdef RG_MATH_FAST_NORMALIZE
	f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
	f32 inv_len = rg_rsqrtf(len_sq);
#endif
	out->x = cx * inv_len;
	out->y = cy * inv_len;
	out->z = cz * inv_len;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE f32 rg_vec3_angle(const rg_vec3* a, const rg_vec3* b)
{
	f32 ax = a->x;
	f32 ay = a->y;
	f32 az = a->z;
	f32 bx = b->x;
	f32 by = b->y;
	f32 bz = b->z;
	f32 len_a_sq = ax * ax + ay * ay + az * az;
	f32 len_b_sq = bx * bx + by * by + bz * bz;
#if RG_MATH_MAX_PERF
	f32 denom = rg_sqrtf(len_a_sq * len_b_sq);
#else
	f32 denom = rg_sqrtf(len_a_sq) * rg_sqrtf(len_b_sq);
	if (denom < RG_EPSILON)
	{
		return 0.0f;
	}
#endif
	f32 cosv = (ax * bx + ay * by + az * bz) * (1.0f / denom);
	if (!(cosv >= -1.0f))
	{
		return RG_PI;
	}
	if (cosv > 1.0f)
	{
		return 0.0f;
	}
	return rg_acosf(cosv);
}

RGINLINE void rg_vec3_rotate(const rg_vec3* v, f32 angle, const rg_vec3* axis, rg_vec3* out)
{
	f32 vx = v->x;
	f32 vy = v->y;
	f32 vz = v->z;
	f32 kx = axis->x;
	f32 ky = axis->y;
	f32 kz = axis->z;
	f32 len_sq = kx * kx + ky * ky + kz * kz;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		kx = 0.0f;
		ky = 0.0f;
		kz = 0.0f;
	}
	else
#endif
	{
#ifdef RG_MATH_FAST_NORMALIZE
		f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
		f32 inv_len = rg_rsqrtf(len_sq);
#endif
		kx *= inv_len;
		ky *= inv_len;
		kz *= inv_len;
	}
	f32 c = rg_cosf(angle);
	f32 s = rg_sinf(angle);
	f32 t = 1.0f - c;
	f32 kdot = kx * vx + ky * vy + kz * vz;
	f32 kt = kdot * t;

	out->x = vx * c + (ky * vz - kz * vy) * s + kx * kt;
	out->y = vy * c + (kz * vx - kx * vz) * s + ky * kt;
	out->z = vz * c + (kx * vy - ky * vx) * s + kz * kt;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_rotate_unit_axis_sincos(const rg_vec3* v, f32 s, f32 c, const rg_vec3* axis_unit, rg_vec3* out)
{
	f32 vx = v->x;
	f32 vy = v->y;
	f32 vz = v->z;
	f32 kx = axis_unit->x;
	f32 ky = axis_unit->y;
	f32 kz = axis_unit->z;
	f32 t = 1.0f - c;
	f32 kdot = kx * vx + ky * vy + kz * vz;
	f32 kt = kdot * t;

	out->x = vx * c + (ky * vz - kz * vy) * s + kx * kt;
	out->y = vy * c + (kz * vx - kx * vz) * s + ky * kt;
	out->z = vz * c + (kx * vy - ky * vx) * s + kz * kt;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_rotate_unit_axis(const rg_vec3* v, f32 angle, const rg_vec3* axis_unit, rg_vec3* out)
{
	rg_vec3_rotate_unit_axis_sincos(v, rg_sinf(angle), rg_cosf(angle), axis_unit, out);
}

RGINLINE void rgi_vec3_rotate_padded_cols3(const f32* m, const rg_vec3* v, rg_vec3* out)
{
	f32 vx = v->x;
	f32 vy = v->y;
	f32 vz = v->z;

#if RG_MATH_MAX_PERF
	{
		f32 x0 = m[0], x1 = m[1], x2 = m[2];
		f32 y0 = m[4], y1 = m[5], y2 = m[6];
		f32 z0 = m[8], z1 = m[9], z2 = m[10];
		f32 x_len_sq = x0 * x0 + x1 * x1 + x2 * x2;
		f32 y_len_sq = y0 * y0 + y1 * y1 + y2 * y2;
		f32 z_len_sq = z0 * z0 + z1 * z1 + z2 * z2;

		if (rg_absf(x_len_sq - 1.0f) <= 1e-5f &&
		    rg_absf(y_len_sq - 1.0f) <= 1e-5f &&
		    rg_absf(z_len_sq - 1.0f) <= 1e-5f)
		{
			out->x = x0 * vx + y0 * vy + z0 * vz;
			out->y = x1 * vx + y1 * vy + z1 * vz;
			out->z = x2 * vx + y2 * vy + z2 * vz;
			return;
		}
	}
#endif

#if defined(RG_MATH_SSE41) && defined(RG_MATH_FAST_NORMALIZE)
	__m128 zero = _mm_setzero_ps();
	__m128 x = _mm_blend_ps(_mm_loadu_ps(m + 0), zero, 0x8);
	__m128 y = _mm_blend_ps(_mm_loadu_ps(m + 4), zero, 0x8);
	__m128 z = _mm_blend_ps(_mm_loadu_ps(m + 8), zero, 0x8);

	__m128 inv_x = _mm_rsqrt_ss(_mm_dp_ps(x, x, 0x71));
	__m128 inv_y = _mm_rsqrt_ss(_mm_dp_ps(y, y, 0x71));
	__m128 inv_z = _mm_rsqrt_ss(_mm_dp_ps(z, z, 0x71));
	inv_x = _mm_shuffle_ps(inv_x, inv_x, _MM_SHUFFLE(0, 0, 0, 0));
	inv_y = _mm_shuffle_ps(inv_y, inv_y, _MM_SHUFFLE(0, 0, 0, 0));
	inv_z = _mm_shuffle_ps(inv_z, inv_z, _MM_SHUFFLE(0, 0, 0, 0));

	x = _mm_mul_ps(x, inv_x);
	y = _mm_mul_ps(y, inv_y);
	z = _mm_mul_ps(z, inv_z);

	__m128 r = _mm_mul_ps(x, _mm_set1_ps(vx));
	r = _mm_add_ps(r, _mm_mul_ps(y, _mm_set1_ps(vy)));
	r = _mm_add_ps(r, _mm_mul_ps(z, _mm_set1_ps(vz)));
	RG_VEC3_STORE(out, r);
#else
	f32 x0 = m[0], x1 = m[1], x2 = m[2];
	f32 y0 = m[4], y1 = m[5], y2 = m[6];
	f32 z0 = m[8], z1 = m[9], z2 = m[10];

	f32 x_len_sq = x0 * x0 + x1 * x1 + x2 * x2;
	f32 y_len_sq = y0 * y0 + y1 * y1 + y2 * y2;
	f32 z_len_sq = z0 * z0 + z1 * z1 + z2 * z2;

#ifndef RG_MATH_UNSAFE_NORMALIZE
	f32 x_inv = (x_len_sq < RG_EPSILON * RG_EPSILON) ? 0.0f :
#ifdef RG_MATH_FAST_NORMALIZE
	                                                 rg_rsqrtf_fast(x_len_sq);
#else
	                                                 rg_rsqrtf(x_len_sq);
#endif
	f32 y_inv = (y_len_sq < RG_EPSILON * RG_EPSILON) ? 0.0f :
#ifdef RG_MATH_FAST_NORMALIZE
	                                                 rg_rsqrtf_fast(y_len_sq);
#else
	                                                 rg_rsqrtf(y_len_sq);
#endif
	f32 z_inv = (z_len_sq < RG_EPSILON * RG_EPSILON) ? 0.0f :
#ifdef RG_MATH_FAST_NORMALIZE
	                                                 rg_rsqrtf_fast(z_len_sq);
#else
	                                                 rg_rsqrtf(z_len_sq);
#endif
#else
#ifdef RG_MATH_FAST_NORMALIZE
	f32 x_inv = rg_rsqrtf_fast(x_len_sq);
	f32 y_inv = rg_rsqrtf_fast(y_len_sq);
	f32 z_inv = rg_rsqrtf_fast(z_len_sq);
#else
	f32 x_inv = rg_rsqrtf(x_len_sq);
	f32 y_inv = rg_rsqrtf(y_len_sq);
	f32 z_inv = rg_rsqrtf(z_len_sq);
#endif
#endif

	x0 *= x_inv;
	x1 *= x_inv;
	x2 *= x_inv;
	y0 *= y_inv;
	y1 *= y_inv;
	y2 *= y_inv;
	z0 *= z_inv;
	z1 *= z_inv;
	z2 *= z_inv;

	out->x = x0 * vx + y0 * vy + z0 * vz;
	out->y = x1 * vx + y1 * vy + z1 * vz;
	out->z = x2 * vx + y2 * vy + z2 * vz;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_rotate_m4(const rg_mat4* m, const rg_vec3* v, rg_vec3* out)
{
	rgi_vec3_rotate_padded_cols3(m->m, v, out);
}

RGINLINE void rg_vec3_rotate_m3(const rg_mat3* m, const rg_vec3* v, rg_vec3* out)
{
	rgi_vec3_rotate_padded_cols3(m->m, v, out);
}

RGINLINE void rgi_vec3_rotate_padded_cols3_fast(const f32* m, const rg_vec3* v, rg_vec3* out)
{
	f32 vx = v->x;
	f32 vy = v->y;
	f32 vz = v->z;

	out->x = m[0] * vx + m[4] * vy + m[8] * vz;
	out->y = m[1] * vx + m[5] * vy + m[9] * vz;
	out->z = m[2] * vx + m[6] * vy + m[10] * vz;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_rotate_m4_fast(const rg_mat4* m, const rg_vec3* v, rg_vec3* out)
{
	rgi_vec3_rotate_padded_cols3_fast(m->m, v, out);
}

RGINLINE void rg_vec3_rotate_m3_fast(const rg_mat3* m, const rg_vec3* v, rg_vec3* out)
{
	rgi_vec3_rotate_padded_cols3_fast(m->m, v, out);
}

RGINLINE void rg_vec3_minv(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
	rg_vec3_min(a, b, out);
}

RGINLINE void rg_vec3_maxv(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
	rg_vec3_max(a, b, out);
}

RGINLINE void rg_vec3_ortho(const rg_vec3* v, rg_vec3* out)
{
	f32 ignore;
	f32 f = modff(rg_absf(v->x) + 0.5f, &ignore);
	out->x = -v->y;
	out->y = v->x - f * v->z;
	out->z = f * v->y;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_lerpc(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out)
{
	rg_vec3_lerp(a, b, rg_clampf(t, 0.0f, 1.0f), out);
}

RGINLINE void rg_vec3_mix(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out)
{
	rg_vec3_lerp(a, b, t, out);
}

RGINLINE void rg_vec3_mixc(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out)
{
	rg_vec3_lerpc(a, b, t, out);
}

RGINLINE void rg_vec3_step(const rg_vec3* edge, const rg_vec3* v, rg_vec3* out)
{
	out->x = (f32) !(v->x < edge->x);
	out->y = (f32) !(v->y < edge->y);
	out->z = (f32) !(v->z < edge->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_smoothstep_uni(f32 edge0, f32 edge1, const rg_vec3* v, rg_vec3* out)
{
	out->x = rg_smoothstepf(edge0, edge1, v->x);
	out->y = rg_smoothstepf(edge0, edge1, v->y);
	out->z = rg_smoothstepf(edge0, edge1, v->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_smoothstep(const rg_vec3* edge0, const rg_vec3* edge1, const rg_vec3* v, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 edge0v = RG_VEC3_LOAD(edge0);
	__m128 edge1v = RG_VEC3_LOAD(edge1);
	__m128 vv = RG_VEC3_LOAD(v);
	__m128 zero = _mm_setzero_ps();
	__m128 one = _mm_set1_ps(1.0f);
	__m128 t = _mm_div_ps(_mm_sub_ps(vv, edge0v), _mm_sub_ps(edge1v, edge0v));
	t = _mm_min_ps(_mm_max_ps(t, zero), one);
	__m128 smooth = _mm_mul_ps(_mm_mul_ps(t, t), _mm_sub_ps(_mm_set1_ps(3.0f), _mm_add_ps(t, t)));
	__m128 degenerate = _mm_cmple_ps(edge1v, edge0v);
	__m128 step = _mm_andnot_ps(_mm_cmplt_ps(vv, edge0v), one);
	RG_VEC3_STORE(out, _mm_or_ps(_mm_and_ps(degenerate, step), _mm_andnot_ps(degenerate, smooth)));
#else
	out->x = rg_smoothstepf(edge0->x, edge1->x, v->x);
	out->y = rg_smoothstepf(edge0->y, edge1->y, v->y);
	out->z = rg_smoothstepf(edge0->z, edge1->z, v->z);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_vec3_smoothinterp(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out)
{
	f32 s = t * t * (3.0f - 2.0f * t);
	rg_vec3_lerp(a, b, s, out);
}

RGINLINE void rg_vec3_smoothinterpc(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out)
{
	rg_vec3_smoothinterp(a, b, rg_clampf(t, 0.0f, 1.0f), out);
}

RGINLINE void rg_vec3_swizzle(const rg_vec3* v, int mask, rg_vec3* out)
{
	rg_vec3 t;
	t.x = v->data[(mask >> 4) & 0x3];
	t.y = v->data[(mask >> 2) & 0x3];
	t.z = v->data[mask & 0x3];
	out->x = t.x;
	out->y = t.y;
	out->z = t.z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_vec3_faceforward(const rg_vec3* n, const rg_vec3* v, const rg_vec3* nref, rg_vec3* out)
{
	if (rg_vec3_dot(v, nref) < 0.0f)
	{
		rg_vec3_copy(n, out);
	}
	else
	{
		rg_vec3_negate_to(n, out);
	}
}

// =============================================================================
// Vec4 Implementation (SIMD optimized)
// =============================================================================

RGINLINE void rg_vec4_set(rg_vec4* v, f32 x, f32 y, f32 z, f32 w)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(v, _mm_setr_ps(x, y, z, w));
#else
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
#endif
}

RGINLINE void rg_vec4_add(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, _mm_add_ps(RG_VEC4_LOAD(a), RG_VEC4_LOAD(b)));
#else
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
	out->w = a->w + b->w;
#endif
}

RGINLINE void rg_vec4_sub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, _mm_sub_ps(RG_VEC4_LOAD(a), RG_VEC4_LOAD(b)));
#else
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
	out->w = a->w - b->w;
#endif
}

RGINLINE void rg_vec4_scale(const rg_vec4* v, f32 s, rg_vec4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 scalar = _mm_set1_ps(s);
	RG_VEC4_STORE(out, _mm_mul_ps(RG_VEC4_LOAD(v), scalar));
#else
	const rg_vec4* v_aligned = RG_MATH_ASSUME_ALIGNED(v, 16);
	rg_vec4* out_aligned = RG_MATH_ASSUME_ALIGNED(out, 16);
	out_aligned->x = v_aligned->x * s;
	out_aligned->y = v_aligned->y * s;
	out_aligned->z = v_aligned->z * s;
	out_aligned->w = v_aligned->w * s;
#endif
}

#ifdef RG_MATH_SSE
RGINLINE void rg_vec4_scale_simd(const rg_vec4* v, __m128 scalar, rg_vec4* out)
{
	RG_VEC4_STORE(out, _mm_mul_ps(RG_VEC4_LOAD(v), scalar));
}
#endif

RGINLINE void rg_vec4_div(const rg_vec4* v, f32 s, rg_vec4* out)
{
	RG_MATH_ASSERT(s != 0.0f);
#ifdef RG_MATH_SSE
	__m128 inv = _mm_set1_ps(1.0f / s);
	RG_VEC4_STORE(out, _mm_mul_ps(RG_VEC4_LOAD(v), inv));
#else
	f32 inv = 1.0f / s;
	out->x = v->x * inv;
	out->y = v->y * inv;
	out->z = v->z * inv;
	out->w = v->w * inv;
#endif
}

RGINLINE f32 rg_vec4_dot(const rg_vec4* a, const rg_vec4* b)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
#if defined(RG_MATH_SSE41)
	return _mm_cvtss_f32(_mm_dp_ps(RG_VEC4_LOAD(a), RG_VEC4_LOAD(b), 0xFF));
#else
	__m128 mul = _mm_mul_ps(RG_VEC4_LOAD(a), RG_VEC4_LOAD(b));
	__m128 sums = _mm_add_ps(mul, _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 3, 2)));
	sums = _mm_add_ps(sums, _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(0, 1, 0, 1)));
	return _mm_cvtss_f32(sums);
#endif
#else
	return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
#endif
}

RGINLINE f32 rg_vec4_len(const rg_vec4* v)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 v4 = RG_VEC4_LOAD(v);
#if defined(RG_MATH_SSE41)
	__m128 dot = _mm_dp_ps(v4, v4, 0xFF);
#else
	__m128 mul = _mm_mul_ps(v4, v4);
	__m128 dot = _mm_add_ps(mul, _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 3, 2)));
	dot = _mm_add_ps(dot, _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 1, 0, 1)));
#endif
	return _mm_cvtss_f32(_mm_sqrt_ss(dot));
#elif RG_MATH_MAX_PERF
	f32 len_sq = v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
	return rg_sqrtf(len_sq);
#else
	return rg_sqrtf(rg_vec4_dot(v, v));
#endif
}

RGINLINE f32 rg_vec4_distance2(const rg_vec4* a, const rg_vec4* b)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 d = _mm_sub_ps(RG_VEC4_LOAD(a), RG_VEC4_LOAD(b));
#if defined(RG_MATH_SSE41)
	return _mm_cvtss_f32(_mm_dp_ps(d, d, 0xFF));
#else
	__m128 mul = _mm_mul_ps(d, d);
	__m128 sums = _mm_add_ps(mul, _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 3, 2)));
	sums = _mm_add_ps(sums, _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(0, 1, 0, 1)));
	return _mm_cvtss_f32(sums);
#endif
#else
	f32 dx = a->x - b->x;
	f32 dy = a->y - b->y;
	f32 dz = a->z - b->z;
	f32 dw = a->w - b->w;
	return dx * dx + dy * dy + dz * dz + dw * dw;
#endif
}

RGINLINE f32 rg_vec4_distance(const rg_vec4* a, const rg_vec4* b)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 d = _mm_sub_ps(RG_VEC4_LOAD(a), RG_VEC4_LOAD(b));
#if defined(RG_MATH_SSE41)
	__m128 dot = _mm_dp_ps(d, d, 0xFF);
#else
	__m128 mul = _mm_mul_ps(d, d);
	__m128 dot = _mm_add_ps(mul, _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 3, 2)));
	dot = _mm_add_ps(dot, _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 1, 0, 1)));
#endif
	return _mm_cvtss_f32(_mm_sqrt_ss(dot));
#else
	return rg_sqrtf(rg_vec4_distance2(a, b));
#endif
}

RGINLINE void rg_vec4_min(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, _mm_min_ps(RG_VEC4_LOAD(a), RG_VEC4_LOAD(b)));
#else
	out->x = rg_minf(a->x, b->x);
	out->y = rg_minf(a->y, b->y);
	out->z = rg_minf(a->z, b->z);
	out->w = rg_minf(a->w, b->w);
#endif
}

RGINLINE void rg_vec4_max(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, _mm_max_ps(RG_VEC4_LOAD(a), RG_VEC4_LOAD(b)));
#else
	out->x = rg_maxf(a->x, b->x);
	out->y = rg_maxf(a->y, b->y);
	out->z = rg_maxf(a->z, b->z);
	out->w = rg_maxf(a->w, b->w);
#endif
}

RGINLINE void rg_vec4_clamp(const rg_vec4* v, f32 min_val, f32 max_val, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 v4 = RG_VEC4_LOAD(v);
	__m128 vmin = _mm_set1_ps(min_val);
	__m128 vmax = _mm_set1_ps(max_val);
	RG_VEC4_STORE(out, _mm_min_ps(_mm_max_ps(v4, vmin), vmax));
#else
	out->x = rg_clampf(v->x, min_val, max_val);
	out->y = rg_clampf(v->y, min_val, max_val);
	out->z = rg_clampf(v->z, min_val, max_val);
	out->w = rg_clampf(v->w, min_val, max_val);
#endif
}

RGINLINE void rg_vec4_lerp(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 av = RG_VEC4_LOAD(a);
	__m128 bv = RG_VEC4_LOAD(b);
	__m128 tv = _mm_set1_ps(t);
#if defined(RG_MATH_FMA)
	RG_VEC4_STORE(out, _mm_fmadd_ps(_mm_sub_ps(bv, av), tv, av));
#else
	RG_VEC4_STORE(out, _mm_add_ps(av, _mm_mul_ps(_mm_sub_ps(bv, av), tv)));
#endif
#else
	out->x = rg_lerpf(a->x, b->x, t);
	out->y = rg_lerpf(a->y, b->y, t);
	out->z = rg_lerpf(a->z, b->z, t);
	out->w = rg_lerpf(a->w, b->w, t);
#endif
}

RGINLINE void rg_vec4_proj(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	f32 denom = b->x * b->x + b->y * b->y + b->z * b->z + b->w * b->w;
#if !RG_MATH_MAX_PERF
	if (denom < RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->w = 0.0f;
		return;
	}
#endif
	f32 scale = (a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w) / denom;
	out->x = b->x * scale;
	out->y = b->y * scale;
	out->z = b->z * scale;
	out->w = b->w * scale;
}

RGINLINE void rg_vec4_reflect(const rg_vec4* v, const rg_vec4* n, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 vv = RG_VEC4_LOAD(v);
	__m128 nn = RG_VEC4_LOAD(n);
#if defined(RG_MATH_SSE41)
	__m128 dot = _mm_dp_ps(vv, nn, 0xFF);
#else
	__m128 mul = _mm_mul_ps(vv, nn);
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuf);
	shuf = _mm_movehl_ps(shuf, sums);
	sums = _mm_add_ss(sums, shuf);
	__m128 dot = _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(0, 0, 0, 0));
#endif
	__m128 scale = _mm_add_ps(dot, dot);
	RG_VEC4_STORE(out, _mm_sub_ps(vv, _mm_mul_ps(nn, scale)));
#else
	f32 ndot = v->x * n->x + v->y * n->y + v->z * n->z + v->w * n->w;
	f32 scale = 2.0f * ndot;
	out->x = v->x - n->x * scale;
	out->y = v->y - n->y * scale;
	out->z = v->z - n->z * scale;
	out->w = v->w - n->w * scale;
#endif
}

RGINLINE int rg_vec4_refract(const rg_vec4* v, const rg_vec4* n, f32 eta, rg_vec4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 vv = RG_VEC4_LOAD(v);
	__m128 nn = RG_VEC4_LOAD(n);
#if defined(RG_MATH_SSE41)
	f32 ndi = _mm_cvtss_f32(_mm_dp_ps(vv, nn, 0xFF));
#else
	__m128 mul = _mm_mul_ps(vv, nn);
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuf);
	shuf = _mm_movehl_ps(shuf, sums);
	sums = _mm_add_ss(sums, shuf);
	f32 ndi = _mm_cvtss_f32(sums);
#endif
	f32 eni = eta * ndi;
	f32 k = 1.0f - eta * eta + eni * eni;
	if (k < 0.0f)
	{
		RG_VEC4_STORE(out, _mm_setzero_ps());
		return 0;
	}
	__m128 result = _mm_sub_ps(
	    _mm_mul_ps(vv, _mm_set1_ps(eta)),
	    _mm_mul_ps(nn, _mm_set1_ps(eni + rg_sqrtf(k))));
	RG_VEC4_STORE(out, result);
	return 1;
#else
	f32 ndi = v->x * n->x + v->y * n->y + v->z * n->z + v->w * n->w;
	f32 eni = eta * ndi;
	f32 k = 1.0f - eta * eta + eni * eni;
	if (k < 0.0f)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->w = 0.0f;
		return 0;
	}
	f32 scale = eni + rg_sqrtf(k);
	out->x = v->x * eta - n->x * scale;
	out->y = v->y * eta - n->y * scale;
	out->z = v->z * eta - n->z * scale;
	out->w = v->w * eta - n->w * scale;
	return 1;
#endif
}

RGINLINE void rg_vec4_normalize(const rg_vec4* v, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 v4 = RG_VEC4_LOAD(v);
#if defined(RG_MATH_SSE41)
	__m128 dot = _mm_dp_ps(v4, v4, 0xFF);
#else
	__m128 mul = _mm_mul_ps(v4, v4);
	__m128 dot = _mm_add_ps(mul, _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(1, 0, 3, 2)));
	dot = _mm_add_ps(dot, _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 1, 0, 1)));
#endif
#ifndef RG_MATH_UNSAFE_NORMALIZE
	f32 len_sq = _mm_cvtss_f32(dot);
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		RG_VEC4_STORE(out, _mm_setzero_ps());
		return;
	}
#ifdef RG_MATH_FAST_NORMALIZE
	__m128 inv_len = _mm_rsqrt_ps(dot);
#else
	__m128 inv_len = _mm_set1_ps(rg_rsqrtf(len_sq));
#endif
#else
#ifdef RG_MATH_FAST_NORMALIZE
	__m128 inv_len = _mm_rsqrt_ps(dot);
#else
	f32 len_sq = _mm_cvtss_f32(dot);
	__m128 inv_len = _mm_set1_ps(rg_rsqrtf(len_sq));
#endif
#endif
	RG_VEC4_STORE(out, _mm_mul_ps(v4, inv_len));
#else
	f32 len_sq = rg_vec4_dot(v, v);
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->w = 0.0f;
		return;
	}
#endif
#ifdef RG_MATH_FAST_NORMALIZE
	f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
	f32 inv_len = rg_rsqrtf(len_sq);
#endif
	out->x = v->x * inv_len;
	out->y = v->y * inv_len;
	out->z = v->z * inv_len;
	out->w = v->w * inv_len;
#endif
}

RGINLINE void rg_vec4_broadcast(f32 val, rg_vec4* out)
{
	rg_vec4_set(out, val, val, val, val);
}

RGINLINE void rg_vec4_fill(rg_vec4* v, f32 val)
{
	rg_vec4_set(v, val, val, val, val);
}

RGINLINE int rg_vec4_eq(const rg_vec4* v, f32 val)
{
	return v->x == val && v->y == val && v->z == val && v->w == val;
}

RGINLINE int rg_vec4_eq_eps(const rg_vec4* v, f32 val)
{
	return rg_absf(v->x - val) <= RG_EPSILON && rg_absf(v->y - val) <= RG_EPSILON && rg_absf(v->z - val) <= RG_EPSILON && rg_absf(v->w - val) <= RG_EPSILON;
}

RGINLINE int rg_vec4_eq_all(const rg_vec4* v)
{
	f32 x = v->x;
	rg_float_bits xb;
	rg_float_bits yb;
	rg_float_bits zb;
	rg_float_bits wb;
	xb.f = x;
	yb.f = v->y;
	zb.f = v->z;
	wb.f = v->w;
	if ((xb.u & 0x7FFFFFFFu) >= 0x7F800000u ||
	    (yb.u & 0x7FFFFFFFu) >= 0x7F800000u ||
	    (zb.u & 0x7FFFFFFFu) >= 0x7F800000u ||
	    (wb.u & 0x7FFFFFFFu) >= 0x7F800000u)
	{
		return 0;
	}
	return rg_absf(v->y - x) <= RG_EPSILON && rg_absf(v->z - x) <= RG_EPSILON && rg_absf(v->w - x) <= RG_EPSILON;
}

RGINLINE int rg_vec4_eqv(const rg_vec4* a, const rg_vec4* b)
{
	return a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w;
}

RGINLINE int rg_vec4_eqv_eps(const rg_vec4* a, const rg_vec4* b)
{
	return rg_absf(a->x - b->x) <= RG_EPSILON && rg_absf(a->y - b->y) <= RG_EPSILON && rg_absf(a->z - b->z) <= RG_EPSILON && rg_absf(a->w - b->w) <= RG_EPSILON;
}

RGINLINE f32 rg_vec4_max_component(const rg_vec4* v)
{
	f32 max_val = v->x;
	if (v->y > max_val) max_val = v->y;
	if (v->z > max_val) max_val = v->z;
	if (v->w > max_val) max_val = v->w;
	return max_val;
}

RGINLINE f32 rg_vec4_min_component(const rg_vec4* v)
{
	f32 min_val = v->x;
	if (v->y < min_val) min_val = v->y;
	if (v->z < min_val) min_val = v->z;
	if (v->w < min_val) min_val = v->w;
	return min_val;
}

RGINLINE int rg_vec4_isnan(const rg_vec4* v)
{
#if !RG_MATH_MAX_PERF
	return isnan(v->x) || isnan(v->y) || isnan(v->z) || isnan(v->w);
#else
	RG_UNUSED(v);
	return 0;
#endif
}

RGINLINE int rg_vec4_isinf(const rg_vec4* v)
{
#if !RG_MATH_MAX_PERF
	return isinf(v->x) || isinf(v->y) || isinf(v->z) || isinf(v->w);
#else
	RG_UNUSED(v);
	return 0;
#endif
}

RGINLINE int rg_vec4_isvalid(const rg_vec4* v)
{
	return !rg_vec4_isnan(v) && !rg_vec4_isinf(v);
}

RGINLINE void rg_vec4_sign(const rg_vec4* v, rg_vec4* out)
{
	out->x = rg_signf(v->x);
	out->y = rg_signf(v->y);
	out->z = rg_signf(v->z);
	out->w = rg_signf(v->w);
}

RGINLINE void rg_vec4_abs(const rg_vec4* v, rg_vec4* out)
{
	out->x = rg_absf(v->x);
	out->y = rg_absf(v->y);
	out->z = rg_absf(v->z);
	out->w = rg_absf(v->w);
}

RGINLINE void rg_vec4_fract(const rg_vec4* v, rg_vec4* out)
{
	const f32 max_fract = 0.99999994f;
	out->x = rg_minf(rg_fracf(v->x), max_fract);
	out->y = rg_minf(rg_fracf(v->y), max_fract);
	out->z = rg_minf(rg_fracf(v->z), max_fract);
	out->w = rg_minf(rg_fracf(v->w), max_fract);
}

RGINLINE void rg_vec4_floor(const rg_vec4* v, rg_vec4* out)
{
	out->x = rg_floorf(v->x);
	out->y = rg_floorf(v->y);
	out->z = rg_floorf(v->z);
	out->w = rg_floorf(v->w);
}

RGINLINE void rg_vec4_mods(const rg_vec4* v, f32 s, rg_vec4* out)
{
	out->x = rg_fmodf(v->x, s);
	out->y = rg_fmodf(v->y, s);
	out->z = rg_fmodf(v->z, s);
	out->w = rg_fmodf(v->w, s);
}

RGINLINE void rg_vec4_steps(f32 edge, const rg_vec4* v, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 cmp = _mm_cmplt_ps(RG_VEC4_LOAD(v), _mm_set1_ps(edge));
	RG_VEC4_STORE(out, _mm_andnot_ps(cmp, _mm_set1_ps(1.0f)));
#else
	out->x = (f32) !(v->x < edge);
	out->y = (f32) !(v->y < edge);
	out->z = (f32) !(v->z < edge);
	out->w = (f32) !(v->w < edge);
#endif
}

RGINLINE void rg_vec4_stepr(const rg_vec4* edge, f32 v, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 cmp = _mm_cmplt_ps(_mm_set1_ps(v), RG_VEC4_LOAD(edge));
	RG_VEC4_STORE(out, _mm_andnot_ps(cmp, _mm_set1_ps(1.0f)));
#else
	out->x = (f32) !(v < edge->x);
	out->y = (f32) !(v < edge->y);
	out->z = (f32) !(v < edge->z);
	out->w = (f32) !(v < edge->w);
#endif
}

RGINLINE f32 rg_vec4_hadd(const rg_vec4* v)
{
#ifdef RG_MATH_SSE
	__m128 v4 = RG_VEC4_LOAD(v);
	__m128 shuf = _mm_shuffle_ps(v4, v4, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(v4, shuf);
	shuf = _mm_movehl_ps(shuf, sums);
	sums = _mm_add_ss(sums, shuf);
	return _mm_cvtss_f32(sums);
#else
	return v->x + v->y + v->z + v->w;
#endif
}

RGINLINE void rg_vec4_sqrt(const rg_vec4* v, rg_vec4* out)
{
	out->x = rg_sqrtf(v->x);
	out->y = rg_sqrtf(v->y);
	out->z = rg_sqrtf(v->z);
	out->w = rg_sqrtf(v->w);
}

RGINLINE void rg_vec4_cubic(f32 s, rg_vec4* out)
{
	f32 ss = s * s;
	out->x = ss * s;
	out->y = ss;
	out->z = s;
	out->w = 1.0f;
}

RGINLINE void rg_vec4_copy3(const rg_vec4* v, rg_vec3* out)
{
	rg_vec3_set(out, v->x, v->y, v->z);
}

RGINLINE void rg_vec4_copy(const rg_vec4* v, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, RG_VEC4_LOAD(v));
#else
	out->x = v->x;
	out->y = v->y;
	out->z = v->z;
	out->w = v->w;
#endif
}

RGINLINE void rg_vec4_ucopy(const rg_vec4* v, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 tmp = _mm_loadu_ps(v->data);
	_mm_storeu_ps(out->data, tmp);
#else
	out->x = v->x;
	out->y = v->y;
	out->z = v->z;
	out->w = v->w;
#endif
}

RGINLINE void rg_vec4_zero(rg_vec4* v)
{
	rg_vec4_set(v, 0.0f, 0.0f, 0.0f, 0.0f);
}

RGINLINE void rg_vec4_one(rg_vec4* v)
{
	rg_vec4_set(v, 1.0f, 1.0f, 1.0f, 1.0f);
}

RGINLINE void rg_vec4_make(const f32* src, rg_vec4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC4_STORE(out, _mm_loadu_ps(src));
#else
	out->x = src[0];
	out->y = src[1];
	out->z = src[2];
	out->w = src[3];
#endif
}

RGINLINE void rg_vec4_adds(const rg_vec4* v, f32 s, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 scalar = _mm_set1_ps(s);
	RG_VEC4_STORE(out, _mm_add_ps(RG_VEC4_LOAD(v), scalar));
#else
	out->x = v->x + s;
	out->y = v->y + s;
	out->z = v->z + s;
	out->w = v->w + s;
#endif
}

RGINLINE void rg_vec4_subs(const rg_vec4* v, f32 s, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 scalar = _mm_set1_ps(s);
	RG_VEC4_STORE(out, _mm_sub_ps(RG_VEC4_LOAD(v), scalar));
#else
	out->x = v->x - s;
	out->y = v->y - s;
	out->z = v->z - s;
	out->w = v->w - s;
#endif
}

RGINLINE void rg_vec4_mul(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, _mm_mul_ps(RG_VEC4_LOAD(a), RG_VEC4_LOAD(b)));
#else
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
	out->w = a->w * b->w;
#endif
}

RGINLINE void rg_vec4_scale_as(const rg_vec4* v, f32 s, rg_vec4* out)
{
#if RG_MATH_MAX_PERF
	f32 len_sq = v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
	f32 scale = s * rg_rsqrtf(len_sq);
	out->x = v->x * scale;
	out->y = v->y * scale;
	out->z = v->z * scale;
	out->w = v->w * scale;
#else
	f32 len = rg_vec4_len(v);
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len < RG_EPSILON)
	{
		rg_vec4_zero(out);
		return;
	}
#endif
	rg_vec4_scale(v, s / len, out);
#endif
}

RGINLINE void rg_vec4_divv(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	RG_MATH_ASSERT(b->x != 0.0f && b->y != 0.0f && b->z != 0.0f && b->w != 0.0f);
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, _mm_div_ps(RG_VEC4_LOAD(a), RG_VEC4_LOAD(b)));
#else
	out->x = a->x / b->x;
	out->y = a->y / b->y;
	out->z = a->z / b->z;
	out->w = a->w / b->w;
#endif
}

RGINLINE void rg_vec4_divs(const rg_vec4* v, f32 s, rg_vec4* out)
{
	rg_vec4_div(v, s, out);
}

RGINLINE void rg_vec4_addadd(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	out->x += a->x + b->x;
	out->y += a->y + b->y;
	out->z += a->z + b->z;
	out->w += a->w + b->w;
}

RGINLINE void rg_vec4_subadd(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	out->x += a->x - b->x;
	out->y += a->y - b->y;
	out->z += a->z - b->z;
	out->w += a->w - b->w;
}

RGINLINE void rg_vec4_muladd(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	out->x += a->x * b->x;
	out->y += a->y * b->y;
	out->z += a->z * b->z;
	out->w += a->w * b->w;
}

RGINLINE void rg_vec4_muladds(const rg_vec4* a, f32 s, rg_vec4* out)
{
	out->x += a->x * s;
	out->y += a->y * s;
	out->z += a->z * s;
	out->w += a->w * s;
}

RGINLINE void rg_vec4_maxadd(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	out->x += rg_maxf(a->x, b->x);
	out->y += rg_maxf(a->y, b->y);
	out->z += rg_maxf(a->z, b->z);
	out->w += rg_maxf(a->w, b->w);
}

RGINLINE void rg_vec4_minadd(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	out->x += rg_minf(a->x, b->x);
	out->y += rg_minf(a->y, b->y);
	out->z += rg_minf(a->z, b->z);
	out->w += rg_minf(a->w, b->w);
}

RGINLINE void rg_vec4_subsub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	out->x -= a->x - b->x;
	out->y -= a->y - b->y;
	out->z -= a->z - b->z;
	out->w -= a->w - b->w;
}

RGINLINE void rg_vec4_addsub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	out->x -= a->x + b->x;
	out->y -= a->y + b->y;
	out->z -= a->z + b->z;
	out->w -= a->w + b->w;
}

RGINLINE void rg_vec4_mulsub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	out->x -= a->x * b->x;
	out->y -= a->y * b->y;
	out->z -= a->z * b->z;
	out->w -= a->w * b->w;
}

RGINLINE void rg_vec4_mulsubs(const rg_vec4* a, f32 s, rg_vec4* out)
{
	out->x -= a->x * s;
	out->y -= a->y * s;
	out->z -= a->z * s;
	out->w -= a->w * s;
}

RGINLINE void rg_vec4_maxsub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	out->x -= rg_maxf(a->x, b->x);
	out->y -= rg_maxf(a->y, b->y);
	out->z -= rg_maxf(a->z, b->z);
	out->w -= rg_maxf(a->w, b->w);
}

RGINLINE void rg_vec4_minsub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	out->x -= rg_minf(a->x, b->x);
	out->y -= rg_minf(a->y, b->y);
	out->z -= rg_minf(a->z, b->z);
	out->w -= rg_minf(a->w, b->w);
}

RGINLINE void rg_vec4_negate_to(const rg_vec4* v, rg_vec4* out)
{
	out->x = -v->x;
	out->y = -v->y;
	out->z = -v->z;
	out->w = -v->w;
}

RGINLINE void rg_vec4_negate(rg_vec4* v)
{
	rg_vec4_negate_to(v, v);
}

RGINLINE void rg_vec4_inv(rg_vec4* v)
{
	rg_vec4_negate_to(v, v);
}

RGINLINE void rg_vec4_inv_to(const rg_vec4* v, rg_vec4* out)
{
	rg_vec4_negate_to(v, out);
}

RGINLINE void rg_vec4_normalize_to(const rg_vec4* v, rg_vec4* out)
{
	rg_vec4_normalize(v, out);
}

RGINLINE f32 rg_vec4_norm2(const rg_vec4* v)
{
	return rg_vec4_dot(v, v);
}

RGINLINE f32 rg_vec4_norm(const rg_vec4* v)
{
	return rg_vec4_len(v);
}

RGINLINE f32 rg_vec4_norm_one(const rg_vec4* v)
{
	return rg_absf(v->x) + rg_absf(v->y) + rg_absf(v->z) + rg_absf(v->w);
}

RGINLINE f32 rg_vec4_norm_inf(const rg_vec4* v)
{
	f32 ax = rg_absf(v->x);
	f32 ay = rg_absf(v->y);
	f32 az = rg_absf(v->z);
	f32 aw = rg_absf(v->w);
	return rg_maxf(rg_maxf(ax, ay), rg_maxf(az, aw));
}

RGINLINE void rg_vec4_maxv(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	rg_vec4_max(a, b, out);
}

RGINLINE void rg_vec4_minv(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
{
	rg_vec4_min(a, b, out);
}

RGINLINE void rg_vec4_lerpc(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out)
{
	rg_vec4_lerp(a, b, rg_clampf(t, 0.0f, 1.0f), out);
}

RGINLINE void rg_vec4_mix(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out)
{
	rg_vec4_lerp(a, b, t, out);
}

RGINLINE void rg_vec4_mixc(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out)
{
	rg_vec4_lerpc(a, b, t, out);
}

RGINLINE void rg_vec4_step(const rg_vec4* edge, const rg_vec4* v, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 cmp = _mm_cmplt_ps(RG_VEC4_LOAD(v), RG_VEC4_LOAD(edge));
	RG_VEC4_STORE(out, _mm_andnot_ps(cmp, _mm_set1_ps(1.0f)));
#else
	out->x = (f32) !(v->x < edge->x);
	out->y = (f32) !(v->y < edge->y);
	out->z = (f32) !(v->z < edge->z);
	out->w = (f32) !(v->w < edge->w);
#endif
}

RGINLINE void rg_vec4_smoothstep_uni(f32 edge0, f32 edge1, const rg_vec4* v, rg_vec4* out)
{
	out->x = rg_smoothstepf(edge0, edge1, v->x);
	out->y = rg_smoothstepf(edge0, edge1, v->y);
	out->z = rg_smoothstepf(edge0, edge1, v->z);
	out->w = rg_smoothstepf(edge0, edge1, v->w);
}

RGINLINE void rg_vec4_smoothstep(const rg_vec4* edge0, const rg_vec4* edge1, const rg_vec4* v, rg_vec4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 edge0v = RG_VEC4_LOAD(edge0);
	__m128 edge1v = RG_VEC4_LOAD(edge1);
	__m128 vv = RG_VEC4_LOAD(v);
	__m128 zero = _mm_setzero_ps();
	__m128 one = _mm_set1_ps(1.0f);
	__m128 t = _mm_div_ps(_mm_sub_ps(vv, edge0v), _mm_sub_ps(edge1v, edge0v));
	t = _mm_min_ps(_mm_max_ps(t, zero), one);
	__m128 smooth = _mm_mul_ps(_mm_mul_ps(t, t), _mm_sub_ps(_mm_set1_ps(3.0f), _mm_add_ps(t, t)));
	__m128 degenerate = _mm_cmple_ps(edge1v, edge0v);
	__m128 step = _mm_andnot_ps(_mm_cmplt_ps(vv, edge0v), one);
	RG_VEC4_STORE(out, _mm_or_ps(_mm_and_ps(degenerate, step), _mm_andnot_ps(degenerate, smooth)));
#else
	out->x = rg_smoothstepf(edge0->x, edge1->x, v->x);
	out->y = rg_smoothstepf(edge0->y, edge1->y, v->y);
	out->z = rg_smoothstepf(edge0->z, edge1->z, v->z);
	out->w = rg_smoothstepf(edge0->w, edge1->w, v->w);
#endif
}

RGINLINE void rg_vec4_smoothinterp(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out)
{
	f32 s = t * t * (3.0f - 2.0f * t);
	rg_vec4_lerp(a, b, s, out);
}

RGINLINE void rg_vec4_smoothinterpc(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out)
{
	rg_vec4_smoothinterp(a, b, rg_clampf(t, 0.0f, 1.0f), out);
}

RGINLINE void rg_vec4_swizzle(const rg_vec4* v, int mask, rg_vec4* out)
{
	rg_vec4 t;
	t.x = v->data[(mask >> 6) & 0x3];
	t.y = v->data[(mask >> 4) & 0x3];
	t.z = v->data[(mask >> 2) & 0x3];
	t.w = v->data[mask & 0x3];
	out->x = t.x;
	out->y = t.y;
	out->z = t.z;
	out->w = t.w;
}

// =============================================================================
// Integer Vector Implementation
// =============================================================================

RGINLINE void rg_vec2i_set(rg_vec2i* v, i32 x, i32 y)
{
	v->x = x;
	v->y = y;
}

RGINLINE void rg_vec2i_add(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x = a->x + b->x;
	out->y = a->y + b->y;
}

RGINLINE void rg_vec2i_sub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
}

RGINLINE void rg_vec2i_scale(const rg_vec2i* v, i32 s, rg_vec2i* out)
{
	out->x = v->x * s;
	out->y = v->y * s;
}

RGINLINE void rg_vec2i_copy(const rg_vec2i* a, rg_vec2i* out)
{
	out->x = a->x;
	out->y = a->y;
}

RGINLINE void rg_vec2i_zero(rg_vec2i* v)
{
	v->x = 0;
	v->y = 0;
}

RGINLINE void rg_vec2i_one(rg_vec2i* v)
{
	v->x = 1;
	v->y = 1;
}

RGINLINE void rg_vec2i_fill(rg_vec2i* v, i32 val)
{
	v->x = val;
	v->y = val;
}

RGINLINE i32 rg_vec2i_dot(const rg_vec2i* a, const rg_vec2i* b)
{
	return a->x * b->x + a->y * b->y;
}

RGINLINE i32 rg_vec2i_cross(const rg_vec2i* a, const rg_vec2i* b)
{
	return a->x * b->y - a->y * b->x;
}

RGINLINE void rg_vec2i_adds(const rg_vec2i* v, i32 s, rg_vec2i* out)
{
	out->x = v->x + s;
	out->y = v->y + s;
}

RGINLINE void rg_vec2i_subs(const rg_vec2i* v, i32 s, rg_vec2i* out)
{
	out->x = v->x - s;
	out->y = v->y - s;
}

RGINLINE void rg_vec2i_mul(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x = a->x * b->x;
	out->y = a->y * b->y;
}

RGINLINE void rg_vec2i_div(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	RG_MATH_ASSERT(b->x != 0 && b->y != 0);
	out->x = a->x / b->x;
	out->y = a->y / b->y;
}

RGINLINE void rg_vec2i_divs(const rg_vec2i* v, i32 s, rg_vec2i* out)
{
	RG_MATH_ASSERT(s != 0);
	out->x = v->x / s;
	out->y = v->y / s;
}

RGINLINE void rg_vec2i_mod(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	RG_MATH_ASSERT(b->x != 0 && b->y != 0);
	out->x = a->x % b->x;
	out->y = a->y % b->y;
}

RGINLINE void rg_vec2i_addadd(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x += a->x + b->x;
	out->y += a->y + b->y;
}

RGINLINE void rg_vec2i_addadds(const rg_vec2i* a, i32 s, rg_vec2i* out)
{
	out->x += a->x + s;
	out->y += a->y + s;
}

RGINLINE void rg_vec2i_subadd(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x += a->x - b->x;
	out->y += a->y - b->y;
}

RGINLINE void rg_vec2i_subadds(const rg_vec2i* a, i32 s, rg_vec2i* out)
{
	out->x += a->x - s;
	out->y += a->y - s;
}

RGINLINE void rg_vec2i_muladd(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x += a->x * b->x;
	out->y += a->y * b->y;
}

RGINLINE void rg_vec2i_muladds(const rg_vec2i* a, i32 s, rg_vec2i* out)
{
	out->x += a->x * s;
	out->y += a->y * s;
}

RGINLINE void rg_vec2i_maxadd(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x += a->x > b->x ? a->x : b->x;
	out->y += a->y > b->y ? a->y : b->y;
}

RGINLINE void rg_vec2i_minadd(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x += a->x < b->x ? a->x : b->x;
	out->y += a->y < b->y ? a->y : b->y;
}

RGINLINE void rg_vec2i_subsub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x -= a->x - b->x;
	out->y -= a->y - b->y;
}

RGINLINE void rg_vec2i_subsubs(const rg_vec2i* a, i32 s, rg_vec2i* out)
{
	out->x -= a->x - s;
	out->y -= a->y - s;
}

RGINLINE void rg_vec2i_addsub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x -= a->x + b->x;
	out->y -= a->y + b->y;
}

RGINLINE void rg_vec2i_addsubs(const rg_vec2i* a, i32 s, rg_vec2i* out)
{
	out->x -= a->x + s;
	out->y -= a->y + s;
}

RGINLINE void rg_vec2i_mulsub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x -= a->x * b->x;
	out->y -= a->y * b->y;
}

RGINLINE void rg_vec2i_mulsubs(const rg_vec2i* a, i32 s, rg_vec2i* out)
{
	out->x -= a->x * s;
	out->y -= a->y * s;
}

RGINLINE void rg_vec2i_maxsub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x -= a->x > b->x ? a->x : b->x;
	out->y -= a->y > b->y ? a->y : b->y;
}

RGINLINE void rg_vec2i_minsub(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x -= a->x < b->x ? a->x : b->x;
	out->y -= a->y < b->y ? a->y : b->y;
}

RGINLINE i32 rg_vec2i_distance2(const rg_vec2i* a, const rg_vec2i* b)
{
	i32 dx = a->x - b->x;
	i32 dy = a->y - b->y;
	return dx * dx + dy * dy;
}

RGINLINE f32 rg_vec2i_distance(const rg_vec2i* a, const rg_vec2i* b)
{
	return rg_sqrtf((f32)rg_vec2i_distance2(a, b));
}

RGINLINE int rg_vec2i_eq(const rg_vec2i* v, i32 val)
{
	return v->x == val && v->y == val;
}

RGINLINE int rg_vec2i_eqv(const rg_vec2i* a, const rg_vec2i* b)
{
	return a->x == b->x && a->y == b->y;
}

RGINLINE void rg_vec2i_maxv(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x = a->x > b->x ? a->x : b->x;
	out->y = a->y > b->y ? a->y : b->y;
}

RGINLINE void rg_vec2i_minv(const rg_vec2i* a, const rg_vec2i* b, rg_vec2i* out)
{
	out->x = a->x < b->x ? a->x : b->x;
	out->y = a->y < b->y ? a->y : b->y;
}

RGINLINE void rg_vec2i_clamp(rg_vec2i* v, i32 min_val, i32 max_val)
{
	if (v->x < min_val) v->x = min_val;
	else if (v->x > max_val) v->x = max_val;

	if (v->y < min_val) v->y = min_val;
	else if (v->y > max_val) v->y = max_val;
}

RGINLINE void rg_vec2i_abs(const rg_vec2i* v, rg_vec2i* out)
{
	out->x = v->x < 0 ? -v->x : v->x;
	out->y = v->y < 0 ? -v->y : v->y;
}

RGINLINE void rg_vec3i_set(rg_vec3i* v, i32 x, i32 y, i32 z)
{
#ifdef RG_MATH_SSE
	v->simd = _mm_setr_epi32(x, y, z, 0);
#else
	v->x = x;
	v->y = y;
	v->z = z;
	v->_pad = 0;
#endif
}

RGINLINE void rg_vec3i_add(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
#ifdef RG_MATH_SSE
	out->simd = _mm_add_epi32(a->simd, b->simd);
#else
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
	out->_pad = 0;
#endif
}

RGINLINE void rg_vec3i_sub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
#ifdef RG_MATH_SSE
	out->simd = _mm_sub_epi32(a->simd, b->simd);
#else
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
	out->_pad = 0;
#endif
}

RGINLINE void rg_vec3i_scale(const rg_vec3i* v, i32 s, rg_vec3i* out)
{
	// SSE2 doesn't have direct 32-bit integer multiply, use mullo from SSE4.1 or scalar
	out->x = v->x * s;
	out->y = v->y * s;
	out->z = v->z * s;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_copy(const rg_vec3i* a, rg_vec3i* out)
{
	out->x = a->x;
	out->y = a->y;
	out->z = a->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_zero(rg_vec3i* v)
{
	v->x = 0;
	v->y = 0;
	v->z = 0;
	v->_pad = 0;
}

RGINLINE void rg_vec3i_one(rg_vec3i* v)
{
	v->x = 1;
	v->y = 1;
	v->z = 1;
	v->_pad = 0;
}

RGINLINE void rg_vec3i_fill(rg_vec3i* v, i32 val)
{
	v->x = val;
	v->y = val;
	v->z = val;
	v->_pad = 0;
}

RGINLINE i32 rg_vec3i_dot(const rg_vec3i* a, const rg_vec3i* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

RGINLINE i32 rg_vec3i_norm2(const rg_vec3i* v)
{
	return rg_vec3i_dot(v, v);
}

RGINLINE i32 rg_vec3i_norm(const rg_vec3i* v)
{
	return (i32)rg_sqrtf((f32)rg_vec3i_norm2(v));
}

RGINLINE void rg_vec3i_adds(const rg_vec3i* v, i32 s, rg_vec3i* out)
{
	out->x = v->x + s;
	out->y = v->y + s;
	out->z = v->z + s;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_subs(const rg_vec3i* v, i32 s, rg_vec3i* out)
{
	out->x = v->x - s;
	out->y = v->y - s;
	out->z = v->z - s;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_mul(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_div(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	RG_MATH_ASSERT(b->x != 0 && b->y != 0 && b->z != 0);
	out->x = a->x / b->x;
	out->y = a->y / b->y;
	out->z = a->z / b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_divs(const rg_vec3i* v, i32 s, rg_vec3i* out)
{
	RG_MATH_ASSERT(s != 0);
	out->x = v->x / s;
	out->y = v->y / s;
	out->z = v->z / s;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_mod(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	RG_MATH_ASSERT(b->x != 0 && b->y != 0 && b->z != 0);
	out->x = a->x % b->x;
	out->y = a->y % b->y;
	out->z = a->z % b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_addadd(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x += a->x + b->x;
	out->y += a->y + b->y;
	out->z += a->z + b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_addadds(const rg_vec3i* a, i32 s, rg_vec3i* out)
{
	out->x += a->x + s;
	out->y += a->y + s;
	out->z += a->z + s;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_subadd(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x += a->x - b->x;
	out->y += a->y - b->y;
	out->z += a->z - b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_subadds(const rg_vec3i* a, i32 s, rg_vec3i* out)
{
	out->x += a->x - s;
	out->y += a->y - s;
	out->z += a->z - s;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_muladd(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x += a->x * b->x;
	out->y += a->y * b->y;
	out->z += a->z * b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_muladds(const rg_vec3i* a, i32 s, rg_vec3i* out)
{
	out->x += a->x * s;
	out->y += a->y * s;
	out->z += a->z * s;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_maxadd(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x += a->x > b->x ? a->x : b->x;
	out->y += a->y > b->y ? a->y : b->y;
	out->z += a->z > b->z ? a->z : b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_minadd(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x += a->x < b->x ? a->x : b->x;
	out->y += a->y < b->y ? a->y : b->y;
	out->z += a->z < b->z ? a->z : b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_subsub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x -= a->x - b->x;
	out->y -= a->y - b->y;
	out->z -= a->z - b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_subsubs(const rg_vec3i* a, i32 s, rg_vec3i* out)
{
	out->x -= a->x - s;
	out->y -= a->y - s;
	out->z -= a->z - s;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_addsub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x -= a->x + b->x;
	out->y -= a->y + b->y;
	out->z -= a->z + b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_addsubs(const rg_vec3i* a, i32 s, rg_vec3i* out)
{
	out->x -= a->x + s;
	out->y -= a->y + s;
	out->z -= a->z + s;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_mulsub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x -= a->x * b->x;
	out->y -= a->y * b->y;
	out->z -= a->z * b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_mulsubs(const rg_vec3i* a, i32 s, rg_vec3i* out)
{
	out->x -= a->x * s;
	out->y -= a->y * s;
	out->z -= a->z * s;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_maxsub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x -= a->x > b->x ? a->x : b->x;
	out->y -= a->y > b->y ? a->y : b->y;
	out->z -= a->z > b->z ? a->z : b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_minsub(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x -= a->x < b->x ? a->x : b->x;
	out->y -= a->y < b->y ? a->y : b->y;
	out->z -= a->z < b->z ? a->z : b->z;
	out->_pad = 0;
}

RGINLINE i32 rg_vec3i_distance2(const rg_vec3i* a, const rg_vec3i* b)
{
	i32 dx = a->x - b->x;
	i32 dy = a->y - b->y;
	i32 dz = a->z - b->z;
	return dx * dx + dy * dy + dz * dz;
}

RGINLINE f32 rg_vec3i_distance(const rg_vec3i* a, const rg_vec3i* b)
{
	return rg_sqrtf((f32)rg_vec3i_distance2(a, b));
}

RGINLINE int rg_vec3i_eq(const rg_vec3i* v, i32 val)
{
	return v->x == val && v->y == val && v->z == val;
}

RGINLINE int rg_vec3i_eqv(const rg_vec3i* a, const rg_vec3i* b)
{
	return a->x == b->x && a->y == b->y && a->z == b->z;
}

RGINLINE void rg_vec3i_maxv(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x = a->x > b->x ? a->x : b->x;
	out->y = a->y > b->y ? a->y : b->y;
	out->z = a->z > b->z ? a->z : b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_minv(const rg_vec3i* a, const rg_vec3i* b, rg_vec3i* out)
{
	out->x = a->x < b->x ? a->x : b->x;
	out->y = a->y < b->y ? a->y : b->y;
	out->z = a->z < b->z ? a->z : b->z;
	out->_pad = 0;
}

RGINLINE void rg_vec3i_clamp(rg_vec3i* v, i32 min_val, i32 max_val)
{
	if (v->x < min_val) v->x = min_val;
	else if (v->x > max_val) v->x = max_val;

	if (v->y < min_val) v->y = min_val;
	else if (v->y > max_val) v->y = max_val;

	if (v->z < min_val) v->z = min_val;
	else if (v->z > max_val) v->z = max_val;

	v->_pad = 0;
}

RGINLINE void rg_vec3i_abs(const rg_vec3i* v, rg_vec3i* out)
{
	out->x = v->x < 0 ? -v->x : v->x;
	out->y = v->y < 0 ? -v->y : v->y;
	out->z = v->z < 0 ? -v->z : v->z;
	out->_pad = 0;
}

RGINLINE void rg_vec4i_set(rg_vec4i* v, i32 x, i32 y, i32 z, i32 w)
{
#ifdef RG_MATH_SSE
	v->simd = _mm_setr_epi32(x, y, z, w);
#else
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
#endif
}

RGINLINE void rg_vec4i_add(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
#ifdef RG_MATH_SSE
	out->simd = _mm_add_epi32(a->simd, b->simd);
#else
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
	out->w = a->w + b->w;
#endif
}

RGINLINE void rg_vec4i_sub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
#ifdef RG_MATH_SSE
	out->simd = _mm_sub_epi32(a->simd, b->simd);
#else
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
	out->w = a->w - b->w;
#endif
}

RGINLINE void rg_vec4i_scale(const rg_vec4i* v, i32 s, rg_vec4i* out)
{
	out->x = v->x * s;
	out->y = v->y * s;
	out->z = v->z * s;
	out->w = v->w * s;
}

RGINLINE void rg_vec4i_copy(const rg_vec4i* a, rg_vec4i* out)
{
	out->x = a->x;
	out->y = a->y;
	out->z = a->z;
	out->w = a->w;
}

RGINLINE void rg_vec4i_zero(rg_vec4i* v)
{
	v->x = 0;
	v->y = 0;
	v->z = 0;
	v->w = 0;
}

RGINLINE void rg_vec4i_one(rg_vec4i* v)
{
	v->x = 1;
	v->y = 1;
	v->z = 1;
	v->w = 1;
}

RGINLINE void rg_vec4i_fill(rg_vec4i* v, i32 val)
{
	v->x = val;
	v->y = val;
	v->z = val;
	v->w = val;
}

RGINLINE i32 rg_vec4i_dot(const rg_vec4i* a, const rg_vec4i* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

RGINLINE void rg_vec4i_adds(const rg_vec4i* v, i32 s, rg_vec4i* out)
{
	out->x = v->x + s;
	out->y = v->y + s;
	out->z = v->z + s;
	out->w = v->w + s;
}

RGINLINE void rg_vec4i_subs(const rg_vec4i* v, i32 s, rg_vec4i* out)
{
	out->x = v->x - s;
	out->y = v->y - s;
	out->z = v->z - s;
	out->w = v->w - s;
}

RGINLINE void rg_vec4i_mul(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
	out->w = a->w * b->w;
}

RGINLINE void rg_vec4i_div(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	RG_MATH_ASSERT(b->x != 0 && b->y != 0 && b->z != 0 && b->w != 0);
	out->x = a->x / b->x;
	out->y = a->y / b->y;
	out->z = a->z / b->z;
	out->w = a->w / b->w;
}

RGINLINE void rg_vec4i_divs(const rg_vec4i* v, i32 s, rg_vec4i* out)
{
	RG_MATH_ASSERT(s != 0);
	out->x = v->x / s;
	out->y = v->y / s;
	out->z = v->z / s;
	out->w = v->w / s;
}

RGINLINE void rg_vec4i_mod(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	RG_MATH_ASSERT(b->x != 0 && b->y != 0 && b->z != 0 && b->w != 0);
	out->x = a->x % b->x;
	out->y = a->y % b->y;
	out->z = a->z % b->z;
	out->w = a->w % b->w;
}

RGINLINE void rg_vec4i_addadd(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x += a->x + b->x;
	out->y += a->y + b->y;
	out->z += a->z + b->z;
	out->w += a->w + b->w;
}

RGINLINE void rg_vec4i_addadds(const rg_vec4i* a, i32 s, rg_vec4i* out)
{
	out->x += a->x + s;
	out->y += a->y + s;
	out->z += a->z + s;
	out->w += a->w + s;
}

RGINLINE void rg_vec4i_subadd(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x += a->x - b->x;
	out->y += a->y - b->y;
	out->z += a->z - b->z;
	out->w += a->w - b->w;
}

RGINLINE void rg_vec4i_subadds(const rg_vec4i* a, i32 s, rg_vec4i* out)
{
	out->x += a->x - s;
	out->y += a->y - s;
	out->z += a->z - s;
	out->w += a->w - s;
}

RGINLINE void rg_vec4i_muladd(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x += a->x * b->x;
	out->y += a->y * b->y;
	out->z += a->z * b->z;
	out->w += a->w * b->w;
}

RGINLINE void rg_vec4i_muladds(const rg_vec4i* a, i32 s, rg_vec4i* out)
{
	out->x += a->x * s;
	out->y += a->y * s;
	out->z += a->z * s;
	out->w += a->w * s;
}

RGINLINE void rg_vec4i_maxadd(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x += a->x > b->x ? a->x : b->x;
	out->y += a->y > b->y ? a->y : b->y;
	out->z += a->z > b->z ? a->z : b->z;
	out->w += a->w > b->w ? a->w : b->w;
}

RGINLINE void rg_vec4i_minadd(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x += a->x < b->x ? a->x : b->x;
	out->y += a->y < b->y ? a->y : b->y;
	out->z += a->z < b->z ? a->z : b->z;
	out->w += a->w < b->w ? a->w : b->w;
}

RGINLINE void rg_vec4i_subsub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x -= a->x - b->x;
	out->y -= a->y - b->y;
	out->z -= a->z - b->z;
	out->w -= a->w - b->w;
}

RGINLINE void rg_vec4i_subsubs(const rg_vec4i* a, i32 s, rg_vec4i* out)
{
	out->x -= a->x - s;
	out->y -= a->y - s;
	out->z -= a->z - s;
	out->w -= a->w - s;
}

RGINLINE void rg_vec4i_addsub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x -= a->x + b->x;
	out->y -= a->y + b->y;
	out->z -= a->z + b->z;
	out->w -= a->w + b->w;
}

RGINLINE void rg_vec4i_addsubs(const rg_vec4i* a, i32 s, rg_vec4i* out)
{
	out->x -= a->x + s;
	out->y -= a->y + s;
	out->z -= a->z + s;
	out->w -= a->w + s;
}

RGINLINE void rg_vec4i_mulsub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x -= a->x * b->x;
	out->y -= a->y * b->y;
	out->z -= a->z * b->z;
	out->w -= a->w * b->w;
}

RGINLINE void rg_vec4i_mulsubs(const rg_vec4i* a, i32 s, rg_vec4i* out)
{
	out->x -= a->x * s;
	out->y -= a->y * s;
	out->z -= a->z * s;
	out->w -= a->w * s;
}

RGINLINE void rg_vec4i_maxsub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x -= a->x > b->x ? a->x : b->x;
	out->y -= a->y > b->y ? a->y : b->y;
	out->z -= a->z > b->z ? a->z : b->z;
	out->w -= a->w > b->w ? a->w : b->w;
}

RGINLINE void rg_vec4i_minsub(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x -= a->x < b->x ? a->x : b->x;
	out->y -= a->y < b->y ? a->y : b->y;
	out->z -= a->z < b->z ? a->z : b->z;
	out->w -= a->w < b->w ? a->w : b->w;
}

RGINLINE i32 rg_vec4i_distance2(const rg_vec4i* a, const rg_vec4i* b)
{
	i32 dx = a->x - b->x;
	i32 dy = a->y - b->y;
	i32 dz = a->z - b->z;
	i32 dw = a->w - b->w;
	return dx * dx + dy * dy + dz * dz + dw * dw;
}

RGINLINE f32 rg_vec4i_distance(const rg_vec4i* a, const rg_vec4i* b)
{
	return rg_sqrtf((f32)rg_vec4i_distance2(a, b));
}

RGINLINE int rg_vec4i_eq(const rg_vec4i* v, i32 val)
{
	return v->x == val && v->y == val && v->z == val && v->w == val;
}

RGINLINE int rg_vec4i_eqv(const rg_vec4i* a, const rg_vec4i* b)
{
	return a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w;
}

RGINLINE void rg_vec4i_maxv(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x = a->x > b->x ? a->x : b->x;
	out->y = a->y > b->y ? a->y : b->y;
	out->z = a->z > b->z ? a->z : b->z;
	out->w = a->w > b->w ? a->w : b->w;
}

RGINLINE void rg_vec4i_minv(const rg_vec4i* a, const rg_vec4i* b, rg_vec4i* out)
{
	out->x = a->x < b->x ? a->x : b->x;
	out->y = a->y < b->y ? a->y : b->y;
	out->z = a->z < b->z ? a->z : b->z;
	out->w = a->w < b->w ? a->w : b->w;
}

RGINLINE void rg_vec4i_clamp(rg_vec4i* v, i32 min_val, i32 max_val)
{
	if (v->x < min_val) v->x = min_val;
	else if (v->x > max_val) v->x = max_val;

	if (v->y < min_val) v->y = min_val;
	else if (v->y > max_val) v->y = max_val;

	if (v->z < min_val) v->z = min_val;
	else if (v->z > max_val) v->z = max_val;

	if (v->w < min_val) v->w = min_val;
	else if (v->w > max_val) v->w = max_val;
}

RGINLINE void rg_vec4i_abs(const rg_vec4i* v, rg_vec4i* out)
{
	out->x = v->x < 0 ? -v->x : v->x;
	out->y = v->y < 0 ? -v->y : v->y;
	out->z = v->z < 0 ? -v->z : v->z;
	out->w = v->w < 0 ? -v->w : v->w;
}

// =============================================================================
// Value-Based Constructors (convenience API)
// =============================================================================

RGINLINE rg_vec2 rg_make_vec2(f32 x, f32 y)
{
	rg_vec2 v;
	rg_vec2_set(&v, x, y);
	return v;
}

RGINLINE rg_vec3 rg_make_vec3(f32 x, f32 y, f32 z)
{
#if defined(RG_MATH_SSE) && !defined(RG_MATH_VEC3_PLAIN)
	rg_vec3 v;
	v.simd = _mm_setr_ps(x, y, z, 0.0f);
#else
	rg_vec3 v = {{x, y, z, 0.0f}};
#endif
	return v;
}

RGINLINE rg_vec4 rg_make_vec4(f32 x, f32 y, f32 z, f32 w)
{
#if defined(RG_MATH_SSE) && !defined(RG_MATH_VEC4_PLAIN)
	rg_vec4 v;
	v.simd = _mm_setr_ps(x, y, z, w);
#else
	rg_vec4 v = {{x, y, z, w}};
#endif
	return v;
}

RGINLINE rg_vec2i rg_make_vec2i(i32 x, i32 y)
{
	rg_vec2i v;
	rg_vec2i_set(&v, x, y);
	return v;
}

RGINLINE rg_vec3i rg_make_vec3i(i32 x, i32 y, i32 z)
{
	rg_vec3i v;
	rg_vec3i_set(&v, x, y, z);
	return v;
}

RGINLINE rg_vec4i rg_make_vec4i(i32 x, i32 y, i32 z, i32 w)
{
	rg_vec4i v;
	rg_vec4i_set(&v, x, y, z, w);
	return v;
}

// =============================================================================

#endif // RG_MATH_VEC_H
