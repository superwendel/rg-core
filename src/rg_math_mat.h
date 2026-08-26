// rg_math_mat - matrix operations
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides column-major matrix operations, transforms, projection, and decomposition.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_MAT_H
#define RG_MATH_MAT_H

#include "rg_math_vec.h"

RG_MATH_EXTERN_C_BEGIN

// =============================================================================
// MATRIX OPERATIONS - POINTER-BASED API
// =============================================================================

// --- Mat2 operations ---

RGINLINE void rg_mat2_identity(rg_mat2* m);
RGINLINE void rg_mat2_identity_array(rg_mat2* m, size_t count);
RGINLINE void rg_mat2_zero(rg_mat2* m);
RGINLINE void rg_mat2_copy(const rg_mat2* m, rg_mat2* out);
RGINLINE void rg_mat2_transpose(const rg_mat2* m, rg_mat2* out);
RGINLINE void rg_mat2_mul(const rg_mat2* a, const rg_mat2* b, rg_mat2* out);
RGINLINE void rg_mat2_mulv2(const rg_mat2* m, const rg_vec2* v, rg_vec2* out);
RGINLINE f32 rg_mat2_trace(const rg_mat2* m);
RGINLINE void rg_mat2_scale(const rg_mat2* m, f32 s, rg_mat2* out);
RGINLINE f32 rg_mat2_det(const rg_mat2* m);
RGINLINE void rg_mat2_inv(const rg_mat2* m, rg_mat2* out);
RGINLINE void rg_mat2_swap_col(rg_mat2* m, int col1, int col2);
RGINLINE void rg_mat2_swap_row(rg_mat2* m, int row1, int row2);
RGINLINE f32 rg_mat2_rmc(const rg_vec2* r, const rg_mat2* m, const rg_vec2* c);
RGINLINE void rg_mat2_make(const f32* src, rg_mat2* out);

// --- Mat3 operations ---

RGINLINE void rg_mat3_identity(rg_mat3* m);
RGINLINE void rg_mat3_identity_array(rg_mat3* m, size_t count);
RGINLINE void rg_mat3_zero(rg_mat3* m);
RGINLINE void rg_mat3_copy(const rg_mat3* m, rg_mat3* out);
RGINLINE void rg_mat3_transpose(const rg_mat3* m, rg_mat3* out);
RGINLINE void rg_mat3_mul(const rg_mat3* a, const rg_mat3* b, rg_mat3* out);
RGINLINE void rg_mat3_mulv3(const rg_mat3* m, const rg_vec3* v, rg_vec3* out);
RGINLINE f32 rg_mat3_trace(const rg_mat3* m);
RGINLINE void rg_mat3_scale(const rg_mat3* m, f32 s, rg_mat3* out);
RGINLINE f32 rg_mat3_det(const rg_mat3* m);
RGINLINE void rg_mat3_inv(const rg_mat3* m, rg_mat3* out);
RGINLINE void rg_mat3_from_mat4(const rg_mat4* m, rg_mat3* out);
RGINLINE void rg_mat3_from_quat(const rg_quat* q, rg_mat3* out);
RGINLINE void rg_quat_from_mat3(const rg_mat3* m, rg_quat* out);
RGINLINE void rg_mat3_quat(const rg_mat3* m, rg_quat* out);
RGINLINE void rg_mat3_swap_col(rg_mat3* m, int col1, int col2);
RGINLINE void rg_mat3_swap_row(rg_mat3* m, int row1, int row2);
RGINLINE f32 rg_mat3_rmc(const rg_vec3* r, const rg_mat3* m, const rg_vec3* c);
RGINLINE void rg_mat3_make(const f32* src, rg_mat3* out);
RGINLINE void rg_mat3_normal_from_mat3(const rg_mat3* m, rg_mat3* out);
RGINLINE void rg_mat3_normal_from_mat4(const rg_mat4* m, rg_mat3* out);
RGINLINE void rg_mat3_translate2d(const rg_mat3* m, const rg_vec2* v, rg_mat3* out);
RGINLINE void rg_mat3_translate2d_x(const rg_mat3* m, f32 x, rg_mat3* out);
RGINLINE void rg_mat3_translate2d_y(const rg_mat3* m, f32 y, rg_mat3* out);
RGINLINE void rg_mat3_translate2d_make(rg_mat3* m, const rg_vec2* v);
RGINLINE void rg_mat3_scale2d(const rg_mat3* m, const rg_vec2* v, rg_mat3* out);
RGINLINE void rg_mat3_scale2d_make(rg_mat3* m, const rg_vec2* v);
RGINLINE void rg_mat3_scale2d_uni(const rg_mat3* m, f32 s, rg_mat3* out);
RGINLINE void rg_mat3_rotate2d_make(rg_mat3* m, f32 angle);
RGINLINE void rg_mat3_rotate2d(const rg_mat3* m, f32 angle, rg_mat3* out);

// --- Non-square matrix operations ---

RGINLINE void rg_mat2x3_copy(const rg_mat2x3* m, rg_mat2x3* out);
RGINLINE void rg_mat2x3_zero(rg_mat2x3* m);
RGINLINE void rg_mat2x3_make(const f32* src, rg_mat2x3* out);
RGINLINE void rg_mat2x3_mul(const rg_mat2x3* a, const rg_mat3x2* b, rg_mat3* out);
RGINLINE void rg_mat2x3_mulv(const rg_mat2x3* m, const rg_vec2* v, rg_vec3* out);
RGINLINE void rg_mat2x3_transpose(const rg_mat2x3* m, rg_mat3x2* out);
RGINLINE void rg_mat2x3_scale(const rg_mat2x3* m, f32 s, rg_mat2x3* out);

RGINLINE void rg_mat2x4_copy(const rg_mat2x4* m, rg_mat2x4* out);
RGINLINE void rg_mat2x4_zero(rg_mat2x4* m);
RGINLINE void rg_mat2x4_make(const f32* src, rg_mat2x4* out);
RGINLINE void rg_mat2x4_mul(const rg_mat2x4* a, const rg_mat4x2* b, rg_mat4* out);
RGINLINE void rg_mat2x4_mulv(const rg_mat2x4* m, const rg_vec2* v, rg_vec4* out);
RGINLINE void rg_mat2x4_transpose(const rg_mat2x4* m, rg_mat4x2* out);
RGINLINE void rg_mat2x4_scale(const rg_mat2x4* m, f32 s, rg_mat2x4* out);

RGINLINE void rg_mat3x2_copy(const rg_mat3x2* m, rg_mat3x2* out);
RGINLINE void rg_mat3x2_zero(rg_mat3x2* m);
RGINLINE void rg_mat3x2_make(const f32* src, rg_mat3x2* out);
RGINLINE void rg_mat3x2_mul(const rg_mat3x2* a, const rg_mat2x3* b, rg_mat2* out);
RGINLINE void rg_mat3x2_mulv(const rg_mat3x2* m, const rg_vec3* v, rg_vec2* out);
RGINLINE void rg_mat3x2_transpose(const rg_mat3x2* m, rg_mat2x3* out);
RGINLINE void rg_mat3x2_scale(const rg_mat3x2* m, f32 s, rg_mat3x2* out);

RGINLINE void rg_mat3x4_copy(const rg_mat3x4* m, rg_mat3x4* out);
RGINLINE void rg_mat3x4_zero(rg_mat3x4* m);
RGINLINE void rg_mat3x4_make(const f32* src, rg_mat3x4* out);
RGINLINE void rg_mat3x4_mul(const rg_mat3x4* a, const rg_mat4x3* b, rg_mat4* out);
RGINLINE void rg_mat3x4_mulv(const rg_mat3x4* m, const rg_vec3* v, rg_vec4* out);
RGINLINE void rg_mat3x4_transpose(const rg_mat3x4* m, rg_mat4x3* out);
RGINLINE void rg_mat3x4_scale(const rg_mat3x4* m, f32 s, rg_mat3x4* out);

RGINLINE void rg_mat4x2_copy(const rg_mat4x2* m, rg_mat4x2* out);
RGINLINE void rg_mat4x2_zero(rg_mat4x2* m);
RGINLINE void rg_mat4x2_make(const f32* src, rg_mat4x2* out);
RGINLINE void rg_mat4x2_mul(const rg_mat4x2* a, const rg_mat2x4* b, rg_mat2* out);
RGINLINE void rg_mat4x2_mulv(const rg_mat4x2* m, const rg_vec4* v, rg_vec2* out);
RGINLINE void rg_mat4x2_transpose(const rg_mat4x2* m, rg_mat2x4* out);
RGINLINE void rg_mat4x2_scale(const rg_mat4x2* m, f32 s, rg_mat4x2* out);

RGINLINE void rg_mat4x3_copy(const rg_mat4x3* m, rg_mat4x3* out);
RGINLINE void rg_mat4x3_zero(rg_mat4x3* m);
RGINLINE void rg_mat4x3_make(const f32* src, rg_mat4x3* out);
RGINLINE void rg_mat4x3_mul(const rg_mat4x3* a, const rg_mat3x4* b, rg_mat3* out);
RGINLINE void rg_mat4x3_mulv(const rg_mat4x3* m, const rg_vec4* v, rg_vec3* out);
RGINLINE void rg_mat4x3_transpose(const rg_mat4x3* m, rg_mat3x4* out);
RGINLINE void rg_mat4x3_scale(const rg_mat4x3* m, f32 s, rg_mat4x3* out);

// --- Mat4 operations ---

RGINLINE void rg_mat4_identity(rg_mat4* m);
RGINLINE void rg_mat4_identity_array(rg_mat4* m, size_t count);
RGINLINE void rg_mat4_zero(rg_mat4* m);
RGINLINE void rg_mat4_copy(const rg_mat4* m, rg_mat4* out);
RGINLINE void rg_mat4_ucopy(const rg_mat4* m, rg_mat4* out);
RGINLINE void rg_mat4_transpose(const rg_mat4* m, rg_mat4* out);
RGINLINE void rg_mat4_pick3(const rg_mat4* m, rg_mat3* out);
RGINLINE void rg_mat4_pick3t(const rg_mat4* m, rg_mat3* out);
RGINLINE void rg_mat4_ins3(const rg_mat3* m, rg_mat4* out);
RGINLINE f32 rg_mat4_trace(const rg_mat4* m);
RGINLINE f32 rg_mat4_trace3(const rg_mat4* m);
RGINLINE void rg_mat4_scale_p(rg_mat4* m, f32 s);
RGINLINE void rg_mat4_scale_scalar(const rg_mat4* m, f32 s, rg_mat4* out);
RGINLINE void rg_mat4_swap_col(rg_mat4* m, int col1, int col2);
RGINLINE void rg_mat4_swap_row(rg_mat4* m, int row1, int row2);
RGINLINE void rg_mat4_make(const f32* src, rg_mat4* out);
RGINLINE void rg_mat4_muln(const rg_mat4* const* matrices, int len, rg_mat4* out);
RGINLINE void rg_mat4_mul4(const rg_mat4* a, const rg_mat4* b, const rg_mat4* c, const rg_mat4* d, rg_mat4* out);
RGINLINE void rg_mat4_mul4_fast(const rg_mat4* a, const rg_mat4* b, const rg_mat4* c, const rg_mat4* d, rg_mat4* out);
RGINLINE void rg_mat4_translate_make(rg_mat4* m, const rg_vec3* v);
RGINLINE void rg_mat4_scale_make(rg_mat4* m, const rg_vec3* v);
RGINLINE void rg_mat4_rotate_make(rg_mat4* m, f32 angle, const rg_vec3* axis);
RGINLINE void rg_mat4_rotate_make_fast(rg_mat4* m, f32 angle, const rg_vec3* axis);
RGINLINE void rg_mat4_from_quat(rg_mat4* m, const rg_quat* q);
RGINLINE void rg_mat4_trs_make(rg_mat4* m, const rg_vec3* t, const rg_quat* r, const rg_vec3* s);
RGINLINE void rg_mat4_translate(const rg_mat4* m, const rg_vec3* v, rg_mat4* out);
RGINLINE void rg_mat4_translate_x(const rg_mat4* m, f32 x, rg_mat4* out);
RGINLINE void rg_mat4_translate_y(const rg_mat4* m, f32 y, rg_mat4* out);
RGINLINE void rg_mat4_translate_z(const rg_mat4* m, f32 z, rg_mat4* out);
RGINLINE void rg_mat4_scale(const rg_mat4* m, const rg_vec3* v, rg_mat4* out);
RGINLINE void rg_mat4_scale_uni(const rg_mat4* m, f32 s, rg_mat4* out);
RGINLINE void rg_mat4_rotate(const rg_mat4* m, f32 angle, const rg_vec3* axis, rg_mat4* out);
RGINLINE void rg_mat4_rotate_fast(const rg_mat4* m, f32 angle, const rg_vec3* axis, rg_mat4* out);
RGINLINE void rg_mat4_rotate_x(const rg_mat4* m, f32 angle, rg_mat4* out);
RGINLINE void rg_mat4_rotate_y(const rg_mat4* m, f32 angle, rg_mat4* out);
RGINLINE void rg_mat4_rotate_z(const rg_mat4* m, f32 angle, rg_mat4* out);
RGINLINE void rg_mat4_rotate_at(const rg_mat4* m, const rg_vec3* pivot, f32 angle, const rg_vec3* axis, rg_mat4* out);
RGINLINE void rg_mat4_rotate_atm(rg_mat4* out, const rg_vec3* pivot, f32 angle, const rg_vec3* axis);
RGINLINE void rg_mat4_translated(rg_mat4* m, const rg_vec3* v);
RGINLINE void rg_mat4_translated_to(const rg_mat4* m, const rg_vec3* v, rg_mat4* out);
RGINLINE void rg_mat4_translated_x(rg_mat4* m, f32 x);
RGINLINE void rg_mat4_translated_y(rg_mat4* m, f32 y);
RGINLINE void rg_mat4_translated_z(rg_mat4* m, f32 z);
RGINLINE void rg_mat4_rotated_x(const rg_mat4* m, f32 angle, rg_mat4* out);
RGINLINE void rg_mat4_rotated_y(const rg_mat4* m, f32 angle, rg_mat4* out);
RGINLINE void rg_mat4_rotated_z(const rg_mat4* m, f32 angle, rg_mat4* out);
RGINLINE void rg_mat4_rotated(rg_mat4* m, f32 angle, const rg_vec3* axis);
RGINLINE void rg_mat4_rotated_at(rg_mat4* m, const rg_vec3* pivot, f32 angle, const rg_vec3* axis);
RGINLINE void rg_mat4_spinned(rg_mat4* m, f32 angle, const rg_vec3* axis);
RGINLINE void rg_mat4_decompose_scale(const rg_mat4* m, rg_vec3* s);
RGINLINE void rg_mat4_decompose_scale_fast(const rg_mat4* m, rg_vec3* s);
RGINLINE int rg_mat4_uniscaled(const rg_mat4* m);
RGINLINE void rg_mat4_decompose_rs(const rg_mat4* m, rg_mat4* r, rg_vec3* s);
RGINLINE void rg_mat4_decompose_trs(const rg_mat4* m, rg_vec3* t, rg_mat4* r, rg_vec3* s);
RGINLINE void rg_mat4_decompose(const rg_mat4* m, rg_vec3* t, rg_quat* r, rg_vec3* s);
RGINLINE void rg_mat4_decompose_fast(const rg_mat4* m, rg_vec3* t, rg_quat* r, rg_vec3* s);
RGINLINE void rg_quat_from_mat4(const rg_mat4* m, rg_quat* out);
RGINLINE void rg_mat4_quat(const rg_mat4* m, rg_quat* out);
RGINLINE void rg_mat4_mul(const rg_mat4* a, const rg_mat4* b, rg_mat4* out);
RGINLINE void rg_mat4_mul_affine(const rg_mat4* a, const rg_mat4* b, rg_mat4* out);
RGINLINE void rg_mat4_mul_rot(const rg_mat4* a, const rg_mat4* b, rg_mat4* out);
RGINLINE void rg_mat4_mulv4(const rg_mat4* m, const rg_vec4* v, rg_vec4* out);
RGINLINE void rg_mat4_mulv3(const rg_mat4* m, const rg_vec3* v, f32 w, rg_vec3* out);
RGINLINE f32 rg_mat4_rmc(const rg_vec4* r, const rg_mat4* m, const rg_vec4* c);
RGINLINE void rg_mat4_project(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out);
RGINLINE void rg_mat4_project_no(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out);
RGINLINE void rg_mat4_project_zo(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out);
RGINLINE f32 rg_mat4_project_z(const rg_mat4* m, const rg_vec3* v);
RGINLINE f32 rg_mat4_project_z_no(const rg_mat4* m, const rg_vec3* v);
RGINLINE f32 rg_mat4_project_z_zo(const rg_mat4* m, const rg_vec3* v);
RGINLINE void rg_mat4_pickmatrix(const rg_vec2* center, const rg_vec2* size, const rg_vec4* vp, rg_mat4* out);
RGINLINE void rg_mat4_unprojecti(const rg_mat4* inv, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out);
RGINLINE void rg_mat4_unprojecti_no(const rg_mat4* inv, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out);
RGINLINE void rg_mat4_unprojecti_zo(const rg_mat4* inv, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out);
RGINLINE void rg_mat4_unproject(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out);
RGINLINE void rg_mat4_unproject_no(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out);
RGINLINE void rg_mat4_unproject_zo(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out);
RGINLINE f32 rg_mat4_det(const rg_mat4* m);
RGINLINE void rg_mat4_inv(const rg_mat4* m, rg_mat4* out);
RGINLINE void rg_mat4_inv_affine(const rg_mat4* m, rg_mat4* out);
RGINLINE void rg_mat4_inv_affine_fast(const rg_mat4* m, rg_mat4* out);
RGINLINE void rg_mat4_inv_tr(const rg_mat4* m, rg_mat4* out);
RGINLINE void rg_mat4_perspective(f32 fovy_radians, f32 aspect, f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_perspective_rh_no(f32 fovy_radians, f32 aspect, f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_perspective_rh_zo(f32 fovy_radians, f32 aspect, f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_perspective_lh_no(f32 fovy_radians, f32 aspect, f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_perspective_lh_zo(f32 fovy_radians, f32 aspect, f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_ortho_rh_no(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_ortho_rh_zo(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_ortho_lh_no(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_ortho_lh_zo(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_look_at(const rg_vec3* eye, const rg_vec3* center, const rg_vec3* up, rg_mat4* out);
RGINLINE void rg_mat4_look_at_rh(const rg_vec3* eye, const rg_vec3* center, const rg_vec3* up, rg_mat4* out);
RGINLINE void rg_mat4_look_at_lh(const rg_vec3* eye, const rg_vec3* center, const rg_vec3* up, rg_mat4* out);

RG_MATH_EXTERN_C_END

// =============================================================================
// Mat2 & Mat3 Implementation
// =============================================================================

RGINLINE void rg_mat2_identity(rg_mat2* m)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_storeu_ps(m->m, _mm_setr_ps(1.0f, 0.0f, 0.0f, 1.0f));
#else
	m->m[0] = 1.0f;
	m->m[1] = 0.0f;
	m->m[2] = 0.0f;
	m->m[3] = 1.0f;
#endif
}

RGINLINE void rg_mat2_identity_array(rg_mat2* m, size_t count)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 identity = _mm_setr_ps(1.0f, 0.0f, 0.0f, 1.0f);
	size_t i = 0;
	for (; i + 3 < count; i += 4)
	{
		_mm_storeu_ps(m[i + 0].m, identity);
		_mm_storeu_ps(m[i + 1].m, identity);
		_mm_storeu_ps(m[i + 2].m, identity);
		_mm_storeu_ps(m[i + 3].m, identity);
	}
	for (; i < count; i++)
	{
		_mm_storeu_ps(m[i].m, identity);
	}
#else
	for (size_t i = 0; i < count; i++)
	{
		rg_mat2_identity(&m[i]);
	}
#endif
}

RGINLINE void rg_mat2_zero(rg_mat2* m)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_storeu_ps(m->m, _mm_setzero_ps());
#else
	m->m[0] = 0.0f;
	m->m[1] = 0.0f;
	m->m[2] = 0.0f;
	m->m[3] = 0.0f;
#endif
}

RGINLINE void rg_mat2_copy(const rg_mat2* m, rg_mat2* out)
{
	*out = *m;
}

RGINLINE void rg_mat2_transpose(const rg_mat2* m, rg_mat2* out)
{
	if (out == m)
	{
		f32 tmp = out->m[1];
		out->m[1] = out->m[2];
		out->m[2] = tmp;
		return;
	}

	out->m[0] = m->m[0];
	out->m[1] = m->m[2];
	out->m[2] = m->m[1];
	out->m[3] = m->m[3];
}

RGINLINE void rg_mat2_mul(const rg_mat2* a, const rg_mat2* b, rg_mat2* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = _mm_loadu_ps(a->m);
	__m128 bv = _mm_loadu_ps(b->m);
	__m128 a0 = _mm_shuffle_ps(av, av, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 a1 = _mm_shuffle_ps(av, av, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 b0 = _mm_shuffle_ps(bv, bv, _MM_SHUFFLE(2, 2, 0, 0));
	__m128 b1 = _mm_shuffle_ps(bv, bv, _MM_SHUFFLE(3, 3, 1, 1));
	_mm_storeu_ps(out->m, RG_MATH_FMADD_PS(a1, b1, _mm_mul_ps(a0, b0)));
#else
	f32 a0 = a->m[0];
	f32 a1 = a->m[1];
	f32 a2 = a->m[2];
	f32 a3 = a->m[3];

	f32 b0 = b->m[0];
	f32 b1 = b->m[1];
	f32 b2 = b->m[2];
	f32 b3 = b->m[3];

	out->m[0] = a0 * b0 + a2 * b1;
	out->m[1] = a1 * b0 + a3 * b1;
	out->m[2] = a0 * b2 + a2 * b3;
	out->m[3] = a1 * b2 + a3 * b3;
#endif
}

RGINLINE void rg_mat2_mulv2(const rg_mat2* m, const rg_vec2* v, rg_vec2* out)
{
	out->x = m->m[0] * v->x + m->m[2] * v->y;
	out->y = m->m[1] * v->x + m->m[3] * v->y;
}

RGINLINE f32 rg_mat2_trace(const rg_mat2* m)
{
	return m->m[0] + m->m[3];
}

RGINLINE void rg_mat2_scale(const rg_mat2* m, f32 s, rg_mat2* out)
{
	out->m[0] = m->m[0] * s;
	out->m[1] = m->m[1] * s;
	out->m[2] = m->m[2] * s;
	out->m[3] = m->m[3] * s;
}

RGINLINE f32 rg_mat2_det(const rg_mat2* m)
{
	return m->m[0] * m->m[3] - m->m[2] * m->m[1];
}

RGINLINE void rg_mat2_inv(const rg_mat2* m, rg_mat2* out)
{
	f32 det = rg_mat2_det(m);
	RG_MATH_ASSERT(det != 0.0f);
	f32 inv_det = 1.0f / det;

	f32 a0 = m->m[0];
	f32 a1 = m->m[1];
	f32 a2 = m->m[2];
	f32 a3 = m->m[3];

	out->m[0] = a3 * inv_det;
	out->m[1] = -a1 * inv_det;
	out->m[2] = -a2 * inv_det;
	out->m[3] = a0 * inv_det;
}

RGINLINE void rg_mat2_swap_col(rg_mat2* m, int col1, int col2)
{
	int c1 = col1 * 2;
	int c2 = col2 * 2;
	f32 t0 = m->m[c1];
	f32 t1 = m->m[c1 + 1];
	m->m[c1] = m->m[c2];
	m->m[c1 + 1] = m->m[c2 + 1];
	m->m[c2] = t0;
	m->m[c2 + 1] = t1;
}

RGINLINE void rg_mat2_swap_row(rg_mat2* m, int row1, int row2)
{
	f32 t0 = m->m[row1];
	f32 t1 = m->m[row1 + 2];
	m->m[row1] = m->m[row2];
	m->m[row1 + 2] = m->m[row2 + 2];
	m->m[row2] = t0;
	m->m[row2 + 2] = t1;
}

RGINLINE f32 rg_mat2_rmc(const rg_vec2* r, const rg_mat2* m, const rg_vec2* c)
{
	rg_vec2 tmp;
	rg_mat2_mulv2(m, c, &tmp);
	return rg_vec2_dot(r, &tmp);
}

RGINLINE void rg_mat2_make(const f32* src, rg_mat2* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_storeu_ps(out->m, _mm_loadu_ps(src));
#else
	out->m[0] = src[0];
	out->m[1] = src[1];
	out->m[2] = src[2];
	out->m[3] = src[3];
#endif
}

RGINLINE void rg_mat3_identity(rg_mat3* m)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_storeu_ps(&m->m[0], _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f));
	_mm_storeu_ps(&m->m[4], _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f));
	_mm_storeu_ps(&m->m[8], _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f));
#else
	m->m[0] = 1.0f;
	m->m[1] = 0.0f;
	m->m[2] = 0.0f;
	m->m[3] = 0.0f;
	m->m[4] = 0.0f;
	m->m[5] = 1.0f;
	m->m[6] = 0.0f;
	m->m[7] = 0.0f;
	m->m[8] = 0.0f;
	m->m[9] = 0.0f;
	m->m[10] = 1.0f;
	m->m[11] = 0.0f;
#endif
}

RGINLINE void rg_mat3_identity_array(rg_mat3* m, size_t count)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 c0 = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
	__m128 c1 = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
	__m128 c2 = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
	size_t i = 0;
	for (; i + 3 < count; i += 4)
	{
		_mm_storeu_ps(&m[i + 0].m[0], c0);
		_mm_storeu_ps(&m[i + 0].m[4], c1);
		_mm_storeu_ps(&m[i + 0].m[8], c2);
		_mm_storeu_ps(&m[i + 1].m[0], c0);
		_mm_storeu_ps(&m[i + 1].m[4], c1);
		_mm_storeu_ps(&m[i + 1].m[8], c2);
		_mm_storeu_ps(&m[i + 2].m[0], c0);
		_mm_storeu_ps(&m[i + 2].m[4], c1);
		_mm_storeu_ps(&m[i + 2].m[8], c2);
		_mm_storeu_ps(&m[i + 3].m[0], c0);
		_mm_storeu_ps(&m[i + 3].m[4], c1);
		_mm_storeu_ps(&m[i + 3].m[8], c2);
	}
	for (; i < count; i++)
	{
		_mm_storeu_ps(&m[i].m[0], c0);
		_mm_storeu_ps(&m[i].m[4], c1);
		_mm_storeu_ps(&m[i].m[8], c2);
	}
#else
	for (size_t i = 0; i < count; i++)
	{
		rg_mat3_identity(&m[i]);
	}
#endif
}

RGINLINE void rg_mat3_zero(rg_mat3* m)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 z = _mm_setzero_ps();
	_mm_storeu_ps(&m->m[0], z);
	_mm_storeu_ps(&m->m[4], z);
	_mm_storeu_ps(&m->m[8], z);
#else
	for (int i = 0; i < 12; i++)
	{
		m->m[i] = 0.0f;
	}
#endif
}

RGINLINE void rg_mat3_copy(const rg_mat3* m, rg_mat3* out)
{
	*out = *m;
}

RGINLINE void rg_mat3_transpose(const rg_mat3* m, rg_mat3* out)
{
	if (out == m)
	{
		f32 t = out->m[1];
		out->m[1] = out->m[4];
		out->m[4] = t;

		t = out->m[2];
		out->m[2] = out->m[8];
		out->m[8] = t;

		t = out->m[6];
		out->m[6] = out->m[9];
		out->m[9] = t;
		return;
	}

	out->m[0] = m->m[0];
	out->m[1] = m->m[4];
	out->m[2] = m->m[8];
	out->m[3] = m->m[3];

	out->m[4] = m->m[1];
	out->m[5] = m->m[5];
	out->m[6] = m->m[9];
	out->m[7] = m->m[7];

	out->m[8] = m->m[2];
	out->m[9] = m->m[6];
	out->m[10] = m->m[10];
	out->m[11] = m->m[11];
}

RGINLINE void rg_mat3_mul(const rg_mat3* a, const rg_mat3* b, rg_mat3* out)
{
	f32 a0 = a->m[0];
	f32 a1 = a->m[1];
	f32 a2 = a->m[2];
	f32 a4 = a->m[4];
	f32 a5 = a->m[5];
	f32 a6 = a->m[6];
	f32 a8 = a->m[8];
	f32 a9 = a->m[9];
	f32 a10 = a->m[10];

	f32 r[12];

	for (int col = 0; col < 3; col++)
	{
		f32 b0 = b->m[col * 4 + 0];
		f32 b1 = b->m[col * 4 + 1];
		f32 b2 = b->m[col * 4 + 2];

		r[col * 4 + 0] = a0 * b0 + a4 * b1 + a8 * b2;
		r[col * 4 + 1] = a1 * b0 + a5 * b1 + a9 * b2;
		r[col * 4 + 2] = a2 * b0 + a6 * b1 + a10 * b2;
		r[col * 4 + 3] = 0.0f;
	}

	for (int i = 0; i < 12; i++)
	{
		out->m[i] = r[i];
	}
}

RGINLINE void rg_mat3_mulv3(const rg_mat3* m, const rg_vec3* v, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 vv = RG_VEC3_LOAD(v);
	__m128 x = _mm_shuffle_ps(vv, vv, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 y = _mm_shuffle_ps(vv, vv, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 z = _mm_shuffle_ps(vv, vv, _MM_SHUFFLE(2, 2, 2, 2));

	__m128 r = _mm_mul_ps(_mm_loadu_ps(&m->m[0]), x);
	r = RG_MATH_FMADD_PS(_mm_loadu_ps(&m->m[4]), y, r);
	r = RG_MATH_FMADD_PS(_mm_loadu_ps(&m->m[8]), z, r);
	RG_VEC3_STORE(out, r);
#else
	f32 x = v->x;
	f32 y = v->y;
	f32 z = v->z;

	f32 rx = m->m[0] * x + m->m[4] * y + m->m[8] * z;
	f32 ry = m->m[1] * x + m->m[5] * y + m->m[9] * z;
	f32 rz = m->m[2] * x + m->m[6] * y + m->m[10] * z;

	out->x = rx;
	out->y = ry;
	out->z = rz;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE f32 rg_mat3_trace(const rg_mat3* m)
{
	return m->m[0] + m->m[5] + m->m[10];
}

RGINLINE void rg_mat3_scale(const rg_mat3* m, f32 s, rg_mat3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 scale = _mm_set1_ps(s);
	_mm_storeu_ps(&out->m[0], _mm_mul_ps(_mm_loadu_ps(&m->m[0]), scale));
	_mm_storeu_ps(&out->m[4], _mm_mul_ps(_mm_loadu_ps(&m->m[4]), scale));
	_mm_storeu_ps(&out->m[8], _mm_mul_ps(_mm_loadu_ps(&m->m[8]), scale));
	out->m[3] = 0.0f;
	out->m[7] = 0.0f;
	out->m[11] = 0.0f;
#else
	out->m[0] = m->m[0] * s;
	out->m[1] = m->m[1] * s;
	out->m[2] = m->m[2] * s;
	out->m[3] = 0.0f;

	out->m[4] = m->m[4] * s;
	out->m[5] = m->m[5] * s;
	out->m[6] = m->m[6] * s;
	out->m[7] = 0.0f;

	out->m[8] = m->m[8] * s;
	out->m[9] = m->m[9] * s;
	out->m[10] = m->m[10] * s;
	out->m[11] = 0.0f;
#endif
}

RGINLINE f32 rg_mat3_det(const rg_mat3* m)
{
	f32 a00 = m->m[0];
	f32 a01 = m->m[4];
	f32 a02 = m->m[8];
	f32 a10 = m->m[1];
	f32 a11 = m->m[5];
	f32 a12 = m->m[9];
	f32 a20 = m->m[2];
	f32 a21 = m->m[6];
	f32 a22 = m->m[10];

	return a00 * (a11 * a22 - a12 * a21) - a01 * (a10 * a22 - a12 * a20) + a02 * (a10 * a21 - a11 * a20);
}

RGINLINE void rg_mat3_inv(const rg_mat3* m, rg_mat3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 a = _mm_loadu_ps(&m->m[0]);
	__m128 b = _mm_loadu_ps(&m->m[4]);
	__m128 c = _mm_loadu_ps(&m->m[8]);

	__m128 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 b_zxy = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 c_yzx = _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 c_zxy = _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 a_zxy = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2));

	__m128 i0 = _mm_sub_ps(_mm_mul_ps(b_yzx, c_zxy), _mm_mul_ps(b_zxy, c_yzx));
	__m128 i1 = _mm_sub_ps(_mm_mul_ps(c_yzx, a_zxy), _mm_mul_ps(c_zxy, a_yzx));
	__m128 i2 = _mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), _mm_mul_ps(a_zxy, b_yzx));
	__m128 detv = rg_math_vec_dot3_ps(a, i0);
#ifndef NDEBUG
	RG_MATH_ASSERT(_mm_cvtss_f32(detv) != 0.0f);
#endif
	__m128 scale = _mm_div_ss(_mm_set_ss(1.0f), detv);
	scale = _mm_shuffle_ps(scale, scale, _MM_SHUFFLE(0, 0, 0, 0));

	__m128 z = _mm_setzero_ps();
	_MM_TRANSPOSE4_PS(i0, i1, i2, z);
	_mm_storeu_ps(&out->m[0], _mm_mul_ps(i0, scale));
	_mm_storeu_ps(&out->m[4], _mm_mul_ps(i1, scale));
	_mm_storeu_ps(&out->m[8], _mm_mul_ps(i2, scale));
#else
	f32 a = m->m[0];
	f32 b = m->m[1];
	f32 c = m->m[2];
	f32 d = m->m[4];
	f32 e = m->m[5];
	f32 f = m->m[6];
	f32 g = m->m[8];
	f32 h = m->m[9];
	f32 i = m->m[10];

	f32 c00 = e * i - f * h;
	f32 c01 = h * c - b * i;
	f32 c02 = b * f - e * c;
	f32 c10 = g * f - d * i;
	f32 c11 = a * i - g * c;
	f32 c12 = d * c - a * f;
	f32 c20 = d * h - g * e;
	f32 c21 = g * b - a * h;
	f32 c22 = a * e - d * b;
#ifndef NDEBUG
	f32 det = a * c00 + b * c10 + c * c20;
	RG_MATH_ASSERT(det != 0.0f);
#endif
	f32 idt = 1.0f / (a * c00 + b * c10 + c * c20);

	out->m[0] = idt * c00;
	out->m[1] = idt * c01;
	out->m[2] = idt * c02;
#if !RG_MATH_MAX_PERF
	out->m[3] = 0.0f;
#endif

	out->m[4] = idt * c10;
	out->m[5] = idt * c11;
	out->m[6] = idt * c12;
#if !RG_MATH_MAX_PERF
	out->m[7] = 0.0f;
#endif

	out->m[8] = idt * c20;
	out->m[9] = idt * c21;
	out->m[10] = idt * c22;
#if !RG_MATH_MAX_PERF
	out->m[11] = 0.0f;
#endif
#endif
}

RGINLINE void rg_mat3_from_mat4(const rg_mat4* m, rg_mat3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 mask = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1));
	_mm_store_ps(&out->m[0], _mm_and_ps(_mm_load_ps(&m->m[0]), mask));
	_mm_store_ps(&out->m[4], _mm_and_ps(_mm_load_ps(&m->m[4]), mask));
	_mm_store_ps(&out->m[8], _mm_and_ps(_mm_load_ps(&m->m[8]), mask));
#else
	out->m[0] = m->m[0];
	out->m[1] = m->m[1];
	out->m[2] = m->m[2];
	out->m[3] = 0.0f;

	out->m[4] = m->m[4];
	out->m[5] = m->m[5];
	out->m[6] = m->m[6];
	out->m[7] = 0.0f;

	out->m[8] = m->m[8];
	out->m[9] = m->m[9];
	out->m[10] = m->m[10];
	out->m[11] = 0.0f;
#endif
}

RGINLINE void rg_mat3_from_quat(const rg_quat* q, rg_mat3* out)
{
	f32 x = q->x;
	f32 y = q->y;
	f32 z = q->z;
	f32 w = q->w;

#if RG_MATH_MAX_PERF
	f32 k = 2.0f;
#else
	f32 norm = x * x + y * y + z * z + w * w;
	if (norm < RG_EPSILON * RG_EPSILON)
	{
		rg_mat3_identity(out);
		return;
	}
	f32 k = 2.0f / norm;
#endif

	f32 xk = x * k;
	f32 yk = y * k;
	f32 zk = z * k;
	f32 xx = x * xk;
	f32 xy = x * yk;
	f32 xz = x * zk;
	f32 yy = y * yk;
	f32 yz = y * zk;
	f32 zz = z * zk;
	f32 wx = w * xk;
	f32 wy = w * yk;
	f32 wz = w * zk;

	out->m[0] = 1.0f - yy - zz;
	out->m[1] = xy + wz;
	out->m[2] = xz - wy;
	out->m[3] = 0.0f;

	out->m[4] = xy - wz;
	out->m[5] = 1.0f - xx - zz;
	out->m[6] = yz + wx;
	out->m[7] = 0.0f;

	out->m[8] = xz + wy;
	out->m[9] = yz - wx;
	out->m[10] = 1.0f - xx - yy;
	out->m[11] = 0.0f;
}

RGINLINE void rg_quat_from_mat3(const rg_mat3* m, rg_quat* out)
{
#if RG_MATH_MAX_PERF
	f32 fast_trace = m->m[0] + m->m[5] + m->m[10];
	if (fast_trace >= 0.0f)
	{
		f32 qt = 1.0f + fast_trace;
		f32 rinv = 0.5f / rg_sqrtf(qt);
		out->x = rinv * (m->m[6] - m->m[9]);
		out->y = rinv * (m->m[2] - m->m[8]);
		out->z = rinv * (m->m[1] - m->m[4]);
		out->w = qt * rinv;
		return;
	}
#endif

	f32 m00 = m->m[0];
	f32 m10 = m->m[1];
	f32 m20 = m->m[2];
	f32 m01 = m->m[4];
	f32 m11 = m->m[5];
	f32 m21 = m->m[6];
	f32 m02 = m->m[8];
	f32 m12 = m->m[9];
	f32 m22 = m->m[10];

#if !RG_MATH_MAX_PERF
	f32 sx = rg_sqrtf(m00 * m00 + m10 * m10 + m20 * m20);
	f32 sy = rg_sqrtf(m01 * m01 + m11 * m11 + m21 * m21);
	f32 sz = rg_sqrtf(m02 * m02 + m12 * m12 + m22 * m22);
	if (sx < RG_EPSILON || sy < RG_EPSILON || sz < RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->w = 1.0f;
		return;
	}
	f32 inv_sx = 1.0f / sx;
	f32 inv_sy = 1.0f / sy;
	f32 inv_sz = 1.0f / sz;
	m00 *= inv_sx;
	m10 *= inv_sx;
	m20 *= inv_sx;
	m01 *= inv_sy;
	m11 *= inv_sy;
	m21 *= inv_sy;
	m02 *= inv_sz;
	m12 *= inv_sz;
	m22 *= inv_sz;
#endif

	f32 trace = m00 + m11 + m22;
	if (trace >= 0.0f)
	{
		f32 rt = rg_sqrtf(1.0f + trace);
		f32 rinv = 0.5f / rt;
		out->x = rinv * (m21 - m12);
		out->y = rinv * (m20 - m02);
		out->z = rinv * (m10 - m01);
		out->w = rt * 0.5f;
	}
	else if (m00 >= m11 && m00 >= m22)
	{
		f32 rt = rg_sqrtf(1.0f - m11 - m22 + m00);
		f32 rinv = 0.5f / rt;
		out->x = rt * 0.5f;
		out->y = rinv * (m01 + m10);
		out->z = rinv * (m02 + m20);
		out->w = rinv * (m21 - m12);
	}
	else if (m11 >= m22)
	{
		f32 rt = rg_sqrtf(1.0f - m00 - m22 + m11);
		f32 rinv = 0.5f / rt;
		out->x = rinv * (m01 + m10);
		out->y = rt * 0.5f;
		out->z = rinv * (m12 + m21);
		out->w = rinv * (m20 - m02);
	}
	else
	{
		f32 rt = rg_sqrtf(1.0f - m00 - m11 + m22);
		f32 rinv = 0.5f / rt;
		out->x = rinv * (m02 + m20);
		out->y = rinv * (m12 + m21);
		out->z = rt * 0.5f;
		out->w = rinv * (m10 - m01);
	}
}

RGINLINE void rg_mat3_quat(const rg_mat3* m, rg_quat* out)
{
	rg_quat_from_mat3(m, out);
}

RGINLINE void rg_mat3_swap_col(rg_mat3* m, int col1, int col2)
{
	int c1 = col1 * 4;
	int c2 = col2 * 4;
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 a = _mm_loadu_ps(&m->m[c1]);
	__m128 b = _mm_loadu_ps(&m->m[c2]);
	_mm_storeu_ps(&m->m[c1], b);
	_mm_storeu_ps(&m->m[c2], a);
#else
	f32 t0 = m->m[c1];
	f32 t1 = m->m[c1 + 1];
	f32 t2 = m->m[c1 + 2];
	f32 t3 = m->m[c1 + 3];
	m->m[c1] = m->m[c2];
	m->m[c1 + 1] = m->m[c2 + 1];
	m->m[c1 + 2] = m->m[c2 + 2];
	m->m[c1 + 3] = m->m[c2 + 3];
	m->m[c2] = t0;
	m->m[c2 + 1] = t1;
	m->m[c2 + 2] = t2;
	m->m[c2 + 3] = t3;
#endif
}

RGINLINE void rg_mat3_swap_row(rg_mat3* m, int row1, int row2)
{
	f32 t0 = m->m[row1];
	f32 t1 = m->m[4 + row1];
	f32 t2 = m->m[8 + row1];
	m->m[row1] = m->m[row2];
	m->m[4 + row1] = m->m[4 + row2];
	m->m[8 + row1] = m->m[8 + row2];
	m->m[row2] = t0;
	m->m[4 + row2] = t1;
	m->m[8 + row2] = t2;
}

RGINLINE f32 rg_mat3_rmc(const rg_vec3* r, const rg_mat3* m, const rg_vec3* c)
{
	f32 x = c->x;
	f32 y = c->y;
	f32 z = c->z;
	f32 mx = m->m[0] * x + m->m[4] * y + m->m[8] * z;
	f32 my = m->m[1] * x + m->m[5] * y + m->m[9] * z;
	f32 mz = m->m[2] * x + m->m[6] * y + m->m[10] * z;
	return r->x * mx + r->y * my + r->z * mz;
}

RGINLINE void rg_mat3_make(const f32* src, rg_mat3* out)
{
#if defined(RG_MATH_SSE41) && RG_MATH_MAX_PERF
	__m128 zero = _mm_setzero_ps();
	_mm_storeu_ps(&out->m[0], _mm_blend_ps(_mm_loadu_ps(&src[0]), zero, 0x8));
	_mm_storeu_ps(&out->m[4], _mm_blend_ps(_mm_loadu_ps(&src[3]), zero, 0x8));
	_mm_storeu_ps(&out->m[8], _mm_setr_ps(src[6], src[7], src[8], 0.0f));
#else
	out->m[0] = src[0];
	out->m[1] = src[1];
	out->m[2] = src[2];
	out->m[3] = 0.0f;

	out->m[4] = src[3];
	out->m[5] = src[4];
	out->m[6] = src[5];
	out->m[7] = 0.0f;

	out->m[8] = src[6];
	out->m[9] = src[7];
	out->m[10] = src[8];
	out->m[11] = 0.0f;
#endif
}

RGINLINE void rg_mat3_normal_from_mat3(const rg_mat3* m, rg_mat3* out)
{
	f32 a = m->m[0];
	f32 b = m->m[1];
	f32 c = m->m[2];
	f32 d = m->m[4];
	f32 e = m->m[5];
	f32 f = m->m[6];
	f32 g = m->m[8];
	f32 h = m->m[9];
	f32 i = m->m[10];

	f32 c1 = e * i - f * h;
	f32 c2 = d * i - g * f;
	f32 c3 = d * h - g * e;
	f32 det = a * c1 - b * c2 + c * c3;
	RG_MATH_ASSERT(det != 0.0f);
	f32 idt = 1.0f / det;
	f32 ndt = -idt;

	out->m[0] = idt * c1;
	out->m[1] = ndt * c2;
	out->m[2] = idt * c3;
	out->m[3] = 0.0f;

	out->m[4] = ndt * (b * i - h * c);
	out->m[5] = idt * (a * i - g * c);
	out->m[6] = ndt * (a * h - g * b);
	out->m[7] = 0.0f;

	out->m[8] = idt * (b * f - e * c);
	out->m[9] = ndt * (a * f - d * c);
	out->m[10] = idt * (a * e - d * b);
	out->m[11] = 0.0f;
}

RGINLINE void rg_mat3_normal_from_mat4(const rg_mat4* m, rg_mat3* out)
{
	f32 a = m->m[0];
	f32 b = m->m[1];
	f32 c = m->m[2];
	f32 d = m->m[4];
	f32 e = m->m[5];
	f32 f = m->m[6];
	f32 g = m->m[8];
	f32 h = m->m[9];
	f32 i = m->m[10];

	f32 c1 = e * i - f * h;
	f32 c2 = d * i - g * f;
	f32 c3 = d * h - g * e;
	f32 det = a * c1 - b * c2 + c * c3;
	RG_MATH_ASSERT(det != 0.0f);
	f32 idt = 1.0f / det;
	f32 ndt = -idt;

	out->m[0] = idt * c1;
	out->m[1] = ndt * c2;
	out->m[2] = idt * c3;
	out->m[3] = 0.0f;

	out->m[4] = ndt * (b * i - h * c);
	out->m[5] = idt * (a * i - g * c);
	out->m[6] = ndt * (a * h - g * b);
	out->m[7] = 0.0f;

	out->m[8] = idt * (b * f - e * c);
	out->m[9] = ndt * (a * f - d * c);
	out->m[10] = idt * (a * e - d * b);
	out->m[11] = 0.0f;
}

// =============================================================================
// Mat3 2D Affine Helpers
// =============================================================================

RGINLINE void rg_mat3_translate2d(const rg_mat3* m, const rg_vec2* v, rg_mat3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 c0 = _mm_loadu_ps(&m->m[0]);
	__m128 c1 = _mm_loadu_ps(&m->m[4]);
	__m128 c2 = _mm_loadu_ps(&m->m[8]);

	if (out != m)
	{
		_mm_storeu_ps(&out->m[0], c0);
		_mm_storeu_ps(&out->m[4], c1);
	}

	__m128 t = _mm_add_ps(c2, _mm_add_ps(_mm_mul_ps(c0, _mm_set1_ps(v->x)), _mm_mul_ps(c1, _mm_set1_ps(v->y))));
	__m128 keep_w = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0));
	t = _mm_or_ps(_mm_andnot_ps(keep_w, t), _mm_and_ps(keep_w, c2));
	_mm_storeu_ps(&out->m[8], t);
#else
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];

	if (out != m)
	{
		out->m[0] = m0;
		out->m[1] = m1;
		out->m[2] = m2;
		out->m[3] = m->m[3];
		out->m[4] = m4;
		out->m[5] = m5;
		out->m[6] = m6;
		out->m[7] = m->m[7];
		out->m[11] = m->m[11];
	}

	out->m[8] = m0 * v->x + m4 * v->y + m8;
	out->m[9] = m1 * v->x + m5 * v->y + m9;
	out->m[10] = m2 * v->x + m6 * v->y + m10;
#endif
}

RGINLINE void rg_mat3_translate2d_x(const rg_mat3* m, f32 x, rg_mat3* out)
{
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];

	if (out != m)
	{
		out->m[0] = m0;
		out->m[1] = m1;
		out->m[2] = m2;
		out->m[3] = m->m[3];
		out->m[4] = m->m[4];
		out->m[5] = m->m[5];
		out->m[6] = m->m[6];
		out->m[7] = m->m[7];
		out->m[11] = m->m[11];
	}

	out->m[8] = m0 * x + m8;
	out->m[9] = m1 * x + m9;
	out->m[10] = m2 * x + m10;
}

RGINLINE void rg_mat3_translate2d_y(const rg_mat3* m, f32 y, rg_mat3* out)
{
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];

	if (out != m)
	{
		out->m[0] = m->m[0];
		out->m[1] = m->m[1];
		out->m[2] = m->m[2];
		out->m[3] = m->m[3];
		out->m[4] = m4;
		out->m[5] = m5;
		out->m[6] = m6;
		out->m[7] = m->m[7];
		out->m[11] = m->m[11];
	}

	out->m[8] = m4 * y + m8;
	out->m[9] = m5 * y + m9;
	out->m[10] = m6 * y + m10;
}

RGINLINE void rg_mat3_translate2d_make(rg_mat3* m, const rg_vec2* v)
{
	rg_mat3_identity(m);
	m->m[8] = v->x;
	m->m[9] = v->y;
}

RGINLINE void rg_mat3_scale2d(const rg_mat3* m, const rg_vec2* v, rg_mat3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 keep_w = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0));
	__m128 c0_src = _mm_loadu_ps(&m->m[0]);
	__m128 c1_src = _mm_loadu_ps(&m->m[4]);
	__m128 c0 = _mm_mul_ps(c0_src, _mm_set1_ps(v->x));
	__m128 c1 = _mm_mul_ps(c1_src, _mm_set1_ps(v->y));
	__m128 c2 = _mm_loadu_ps(&m->m[8]);

	c0 = _mm_or_ps(_mm_andnot_ps(keep_w, c0), _mm_and_ps(keep_w, c0_src));
	c1 = _mm_or_ps(_mm_andnot_ps(keep_w, c1), _mm_and_ps(keep_w, c1_src));
	_mm_storeu_ps(&out->m[0], c0);
	_mm_storeu_ps(&out->m[4], c1);
	_mm_storeu_ps(&out->m[8], c2);
#else
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];

	out->m[0] = m0 * v->x;
	out->m[1] = m1 * v->x;
	out->m[2] = m2 * v->x;
	out->m[3] = m->m[3];

	out->m[4] = m4 * v->y;
	out->m[5] = m5 * v->y;
	out->m[6] = m6 * v->y;
	out->m[7] = m->m[7];

	out->m[8] = m->m[8];
	out->m[9] = m->m[9];
	out->m[10] = m->m[10];
	out->m[11] = m->m[11];
#endif
}

RGINLINE void rg_mat3_scale2d_make(rg_mat3* m, const rg_vec2* v)
{
	rg_mat3_identity(m);
	m->m[0] = v->x;
	m->m[5] = v->y;
}

RGINLINE void rg_mat3_scale2d_uni(const rg_mat3* m, f32 s, rg_mat3* out)
{
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];

	out->m[0] = m0 * s;
	out->m[1] = m1 * s;
	out->m[2] = m2 * s;
	out->m[3] = m->m[3];

	out->m[4] = m4 * s;
	out->m[5] = m5 * s;
	out->m[6] = m6 * s;
	out->m[7] = m->m[7];

	out->m[8] = m->m[8];
	out->m[9] = m->m[9];
	out->m[10] = m->m[10];
	out->m[11] = m->m[11];
}

RGINLINE void rg_mat3_rotate2d_make(rg_mat3* m, f32 angle)
{
	f32 s = 0.0f;
	f32 c = 0.0f;
	rg_sincosf(angle, &s, &c);
	m->m[0] = c;
	m->m[1] = s;
	m->m[2] = 0.0f;
	m->m[3] = 0.0f;

	m->m[4] = -s;
	m->m[5] = c;
	m->m[6] = 0.0f;
	m->m[7] = 0.0f;

	m->m[8] = 0.0f;
	m->m[9] = 0.0f;
	m->m[10] = 1.0f;
	m->m[11] = 0.0f;
}

RGINLINE void rg_mat3_rotate2d(const rg_mat3* m, f32 angle, rg_mat3* out)
{
	f32 s = 0.0f;
	f32 c = 0.0f;
	rg_sincosf(angle, &s, &c);
	f32 m00 = m->m[0];
	f32 m01 = m->m[1];
	f32 m02 = m->m[2];
	f32 m10 = m->m[4];
	f32 m11 = m->m[5];
	f32 m12 = m->m[6];

	out->m[0] = m00 * c + m10 * s;
	out->m[1] = m01 * c + m11 * s;
	out->m[2] = m02 * c + m12 * s;
	out->m[3] = m->m[3];

	out->m[4] = m00 * -s + m10 * c;
	out->m[5] = m01 * -s + m11 * c;
	out->m[6] = m02 * -s + m12 * c;
	out->m[7] = m->m[7];

	out->m[8] = m->m[8];
	out->m[9] = m->m[9];
	out->m[10] = m->m[10];
	out->m[11] = m->m[11];
}

// =============================================================================
// Non-square Matrix Implementation
// =============================================================================

RGINLINE void rg_mat2x3_copy(const rg_mat2x3* m, rg_mat2x3* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[1];
	out->m[2] = m->m[2];
	out->m[3] = m->m[3];
	out->m[4] = m->m[4];
	out->m[5] = m->m[5];
}

RGINLINE void rg_mat2x3_zero(rg_mat2x3* m)
{
	m->m[0] = 0.0f;
	m->m[1] = 0.0f;
	m->m[2] = 0.0f;
	m->m[3] = 0.0f;
	m->m[4] = 0.0f;
	m->m[5] = 0.0f;
}

RGINLINE void rg_mat2x3_make(const f32* src, rg_mat2x3* out)
{
	out->m[0] = src[0];
	out->m[1] = src[1];
	out->m[2] = src[2];
	out->m[3] = src[3];
	out->m[4] = src[4];
	out->m[5] = src[5];
}

RGINLINE void rg_mat2x3_mul(const rg_mat2x3* a, const rg_mat3x2* b, rg_mat3* out)
{
	f32 a00 = a->m[0];
	f32 a01 = a->m[1];
	f32 a02 = a->m[2];
	f32 a10 = a->m[3];
	f32 a11 = a->m[4];
	f32 a12 = a->m[5];

	f32 b00 = b->m[0];
	f32 b01 = b->m[1];
	f32 b10 = b->m[2];
	f32 b11 = b->m[3];
	f32 b20 = b->m[4];
	f32 b21 = b->m[5];

	out->m[0] = a00 * b00 + a10 * b01;
	out->m[1] = a01 * b00 + a11 * b01;
	out->m[2] = a02 * b00 + a12 * b01;
	out->m[3] = 0.0f;

	out->m[4] = a00 * b10 + a10 * b11;
	out->m[5] = a01 * b10 + a11 * b11;
	out->m[6] = a02 * b10 + a12 * b11;
	out->m[7] = 0.0f;

	out->m[8] = a00 * b20 + a10 * b21;
	out->m[9] = a01 * b20 + a11 * b21;
	out->m[10] = a02 * b20 + a12 * b21;
	out->m[11] = 0.0f;
}

RGINLINE void rg_mat2x3_mulv(const rg_mat2x3* m, const rg_vec2* v, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 c0 = _mm_loadu_ps(&m->m[0]);
	__m128 c1 = _mm_set_ps(0.0f, m->m[5], m->m[4], m->m[3]);
	__m128 r = _mm_add_ps(_mm_mul_ps(c0, _mm_set1_ps(v->x)), _mm_mul_ps(c1, _mm_set1_ps(v->y)));
	RG_VEC3_STORE(out, r);
#else
	f32 v0 = v->x;
	f32 v1 = v->y;

	out->x = m->m[0] * v0 + m->m[3] * v1;
	out->y = m->m[1] * v0 + m->m[4] * v1;
	out->z = m->m[2] * v0 + m->m[5] * v1;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_mat2x3_transpose(const rg_mat2x3* m, rg_mat3x2* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[3];
	out->m[2] = m->m[1];
	out->m[3] = m->m[4];
	out->m[4] = m->m[2];
	out->m[5] = m->m[5];
}

RGINLINE void rg_mat2x3_scale(const rg_mat2x3* m, f32 s, rg_mat2x3* out)
{
	out->m[0] = m->m[0] * s;
	out->m[1] = m->m[1] * s;
	out->m[2] = m->m[2] * s;
	out->m[3] = m->m[3] * s;
	out->m[4] = m->m[4] * s;
	out->m[5] = m->m[5] * s;
}

RGINLINE void rg_mat2x4_copy(const rg_mat2x4* m, rg_mat2x4* out)
{
	for (int i = 0; i < 8; i++)
	{
		out->m[i] = m->m[i];
	}
}

RGINLINE void rg_mat2x4_zero(rg_mat2x4* m)
{
	for (int i = 0; i < 8; i++)
	{
		m->m[i] = 0.0f;
	}
}

RGINLINE void rg_mat2x4_make(const f32* src, rg_mat2x4* out)
{
	for (int i = 0; i < 8; i++)
	{
		out->m[i] = src[i];
	}
}

RGINLINE void rg_mat2x4_mul(const rg_mat2x4* a, const rg_mat4x2* b, rg_mat4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 c0 = _mm_loadu_ps(&a->m[0]);
	__m128 c1 = _mm_loadu_ps(&a->m[4]);

#if defined(RG_MATH_FMA)
	_mm_storeu_ps(&out->m[0], _mm_fmadd_ps(c1, _mm_set1_ps(b->m[1]), _mm_mul_ps(c0, _mm_set1_ps(b->m[0]))));
	_mm_storeu_ps(&out->m[4], _mm_fmadd_ps(c1, _mm_set1_ps(b->m[3]), _mm_mul_ps(c0, _mm_set1_ps(b->m[2]))));
	_mm_storeu_ps(&out->m[8], _mm_fmadd_ps(c1, _mm_set1_ps(b->m[5]), _mm_mul_ps(c0, _mm_set1_ps(b->m[4]))));
	_mm_storeu_ps(&out->m[12], _mm_fmadd_ps(c1, _mm_set1_ps(b->m[7]), _mm_mul_ps(c0, _mm_set1_ps(b->m[6]))));
#else
	_mm_storeu_ps(&out->m[0], _mm_add_ps(_mm_mul_ps(c0, _mm_set1_ps(b->m[0])), _mm_mul_ps(c1, _mm_set1_ps(b->m[1]))));
	_mm_storeu_ps(&out->m[4], _mm_add_ps(_mm_mul_ps(c0, _mm_set1_ps(b->m[2])), _mm_mul_ps(c1, _mm_set1_ps(b->m[3]))));
	_mm_storeu_ps(&out->m[8], _mm_add_ps(_mm_mul_ps(c0, _mm_set1_ps(b->m[4])), _mm_mul_ps(c1, _mm_set1_ps(b->m[5]))));
	_mm_storeu_ps(&out->m[12], _mm_add_ps(_mm_mul_ps(c0, _mm_set1_ps(b->m[6])), _mm_mul_ps(c1, _mm_set1_ps(b->m[7]))));
#endif
#else
	f32 a00 = a->m[0];
	f32 a01 = a->m[1];
	f32 a02 = a->m[2];
	f32 a03 = a->m[3];
	f32 a10 = a->m[4];
	f32 a11 = a->m[5];
	f32 a12 = a->m[6];
	f32 a13 = a->m[7];

	f32 b00 = b->m[0];
	f32 b01 = b->m[1];
	f32 b10 = b->m[2];
	f32 b11 = b->m[3];
	f32 b20 = b->m[4];
	f32 b21 = b->m[5];
	f32 b30 = b->m[6];
	f32 b31 = b->m[7];

	out->m[0] = a00 * b00 + a10 * b01;
	out->m[1] = a01 * b00 + a11 * b01;
	out->m[2] = a02 * b00 + a12 * b01;
	out->m[3] = a03 * b00 + a13 * b01;

	out->m[4] = a00 * b10 + a10 * b11;
	out->m[5] = a01 * b10 + a11 * b11;
	out->m[6] = a02 * b10 + a12 * b11;
	out->m[7] = a03 * b10 + a13 * b11;

	out->m[8] = a00 * b20 + a10 * b21;
	out->m[9] = a01 * b20 + a11 * b21;
	out->m[10] = a02 * b20 + a12 * b21;
	out->m[11] = a03 * b20 + a13 * b21;

	out->m[12] = a00 * b30 + a10 * b31;
	out->m[13] = a01 * b30 + a11 * b31;
	out->m[14] = a02 * b30 + a12 * b31;
	out->m[15] = a03 * b30 + a13 * b31;
#endif
}

RGINLINE void rg_mat2x4_mulv(const rg_mat2x4* m, const rg_vec2* v, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 c0 = _mm_loadu_ps(&m->m[0]);
	__m128 c1 = _mm_loadu_ps(&m->m[4]);
	__m128 r = RG_MATH_FMADD_PS(c1, _mm_set1_ps(v->y), _mm_mul_ps(c0, _mm_set1_ps(v->x)));
	_mm_storeu_ps(&out->x, r);
#else
	f32 v0 = v->x;
	f32 v1 = v->y;

	out->x = m->m[0] * v0 + m->m[4] * v1;
	out->y = m->m[1] * v0 + m->m[5] * v1;
	out->z = m->m[2] * v0 + m->m[6] * v1;
	out->w = m->m[3] * v0 + m->m[7] * v1;
#endif
}

RGINLINE void rg_mat2x4_transpose(const rg_mat2x4* m, rg_mat4x2* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[4];
	out->m[2] = m->m[1];
	out->m[3] = m->m[5];
	out->m[4] = m->m[2];
	out->m[5] = m->m[6];
	out->m[6] = m->m[3];
	out->m[7] = m->m[7];
}

RGINLINE void rg_mat2x4_scale(const rg_mat2x4* m, f32 s, rg_mat2x4* out)
{
	for (int i = 0; i < 8; i++)
	{
		out->m[i] = m->m[i] * s;
	}
}

RGINLINE void rg_mat3x2_copy(const rg_mat3x2* m, rg_mat3x2* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[1];
	out->m[2] = m->m[2];
	out->m[3] = m->m[3];
	out->m[4] = m->m[4];
	out->m[5] = m->m[5];
}

RGINLINE void rg_mat3x2_zero(rg_mat3x2* m)
{
	for (int i = 0; i < 6; i++)
	{
		m->m[i] = 0.0f;
	}
}

RGINLINE void rg_mat3x2_make(const f32* src, rg_mat3x2* out)
{
	for (int i = 0; i < 6; i++)
	{
		out->m[i] = src[i];
	}
}

RGINLINE void rg_mat3x2_mul(const rg_mat3x2* a, const rg_mat2x3* b, rg_mat2* out)
{
	f32 a00 = a->m[0];
	f32 a01 = a->m[1];
	f32 a10 = a->m[2];
	f32 a11 = a->m[3];
	f32 a20 = a->m[4];
	f32 a21 = a->m[5];

	f32 b00 = b->m[0];
	f32 b01 = b->m[1];
	f32 b02 = b->m[2];
	f32 b10 = b->m[3];
	f32 b11 = b->m[4];
	f32 b12 = b->m[5];

	out->m[0] = a00 * b00 + a10 * b01 + a20 * b02;
	out->m[1] = a01 * b00 + a11 * b01 + a21 * b02;
	out->m[2] = a00 * b10 + a10 * b11 + a20 * b12;
	out->m[3] = a01 * b10 + a11 * b11 + a21 * b12;
}

RGINLINE void rg_mat3x2_mulv(const rg_mat3x2* m, const rg_vec3* v, rg_vec2* out)
{
	f32 v0 = v->x;
	f32 v1 = v->y;
	f32 v2 = v->z;

	out->x = m->m[0] * v0 + m->m[2] * v1 + m->m[4] * v2;
	out->y = m->m[1] * v0 + m->m[3] * v1 + m->m[5] * v2;
}

RGINLINE void rg_mat3x2_transpose(const rg_mat3x2* m, rg_mat2x3* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[2];
	out->m[2] = m->m[4];
	out->m[3] = m->m[1];
	out->m[4] = m->m[3];
	out->m[5] = m->m[5];
}

RGINLINE void rg_mat3x2_scale(const rg_mat3x2* m, f32 s, rg_mat3x2* out)
{
	for (int i = 0; i < 6; i++)
	{
		out->m[i] = m->m[i] * s;
	}
}

RGINLINE void rg_mat3x4_copy(const rg_mat3x4* m, rg_mat3x4* out)
{
	for (int i = 0; i < 12; i++)
	{
		out->m[i] = m->m[i];
	}
}

RGINLINE void rg_mat3x4_zero(rg_mat3x4* m)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 z = _mm_setzero_ps();
	_mm_storeu_ps(&m->m[0], z);
	_mm_storeu_ps(&m->m[4], z);
	_mm_storeu_ps(&m->m[8], z);
#else
	for (int i = 0; i < 12; i++)
	{
		m->m[i] = 0.0f;
	}
#endif
}

RGINLINE void rg_mat3x4_make(const f32* src, rg_mat3x4* out)
{
	for (int i = 0; i < 12; i++)
	{
		out->m[i] = src[i];
	}
}

RGINLINE void rg_mat3x4_mul(const rg_mat3x4* a, const rg_mat4x3* b, rg_mat4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 a0 = _mm_loadu_ps(&a->m[0]);
	__m128 a1 = _mm_loadu_ps(&a->m[4]);
	__m128 a2 = _mm_loadu_ps(&a->m[8]);

	__m128 r0 = _mm_mul_ps(a0, _mm_set1_ps(b->m[0]));
	r0 = RG_MATH_FMADD_PS(a1, _mm_set1_ps(b->m[1]), r0);
	r0 = RG_MATH_FMADD_PS(a2, _mm_set1_ps(b->m[2]), r0);

	__m128 r1 = _mm_mul_ps(a0, _mm_set1_ps(b->m[3]));
	r1 = RG_MATH_FMADD_PS(a1, _mm_set1_ps(b->m[4]), r1);
	r1 = RG_MATH_FMADD_PS(a2, _mm_set1_ps(b->m[5]), r1);

	__m128 r2 = _mm_mul_ps(a0, _mm_set1_ps(b->m[6]));
	r2 = RG_MATH_FMADD_PS(a1, _mm_set1_ps(b->m[7]), r2);
	r2 = RG_MATH_FMADD_PS(a2, _mm_set1_ps(b->m[8]), r2);

	__m128 r3 = _mm_mul_ps(a0, _mm_set1_ps(b->m[9]));
	r3 = RG_MATH_FMADD_PS(a1, _mm_set1_ps(b->m[10]), r3);
	r3 = RG_MATH_FMADD_PS(a2, _mm_set1_ps(b->m[11]), r3);

	_mm_storeu_ps(&out->m[0], r0);
	_mm_storeu_ps(&out->m[4], r1);
	_mm_storeu_ps(&out->m[8], r2);
	_mm_storeu_ps(&out->m[12], r3);
#else
	f32 a00 = a->m[0];
	f32 a01 = a->m[1];
	f32 a02 = a->m[2];
	f32 a03 = a->m[3];
	f32 a10 = a->m[4];
	f32 a11 = a->m[5];
	f32 a12 = a->m[6];
	f32 a13 = a->m[7];
	f32 a20 = a->m[8];
	f32 a21 = a->m[9];
	f32 a22 = a->m[10];
	f32 a23 = a->m[11];

	f32 b00 = b->m[0];
	f32 b01 = b->m[1];
	f32 b02 = b->m[2];
	f32 b10 = b->m[3];
	f32 b11 = b->m[4];
	f32 b12 = b->m[5];
	f32 b20 = b->m[6];
	f32 b21 = b->m[7];
	f32 b22 = b->m[8];
	f32 b30 = b->m[9];
	f32 b31 = b->m[10];
	f32 b32 = b->m[11];

	out->m[0] = a00 * b00 + a10 * b01 + a20 * b02;
	out->m[1] = a01 * b00 + a11 * b01 + a21 * b02;
	out->m[2] = a02 * b00 + a12 * b01 + a22 * b02;
	out->m[3] = a03 * b00 + a13 * b01 + a23 * b02;

	out->m[4] = a00 * b10 + a10 * b11 + a20 * b12;
	out->m[5] = a01 * b10 + a11 * b11 + a21 * b12;
	out->m[6] = a02 * b10 + a12 * b11 + a22 * b12;
	out->m[7] = a03 * b10 + a13 * b11 + a23 * b12;

	out->m[8] = a00 * b20 + a10 * b21 + a20 * b22;
	out->m[9] = a01 * b20 + a11 * b21 + a21 * b22;
	out->m[10] = a02 * b20 + a12 * b21 + a22 * b22;
	out->m[11] = a03 * b20 + a13 * b21 + a23 * b22;

	out->m[12] = a00 * b30 + a10 * b31 + a20 * b32;
	out->m[13] = a01 * b30 + a11 * b31 + a21 * b32;
	out->m[14] = a02 * b30 + a12 * b31 + a22 * b32;
	out->m[15] = a03 * b30 + a13 * b31 + a23 * b32;
#endif
}

RGINLINE void rg_mat3x4_mulv(const rg_mat3x4* m, const rg_vec3* v, rg_vec4* out)
{
	f32 v0 = v->x;
	f32 v1 = v->y;
	f32 v2 = v->z;

	out->x = m->m[0] * v0 + m->m[4] * v1 + m->m[8] * v2;
	out->y = m->m[1] * v0 + m->m[5] * v1 + m->m[9] * v2;
	out->z = m->m[2] * v0 + m->m[6] * v1 + m->m[10] * v2;
	out->w = m->m[3] * v0 + m->m[7] * v1 + m->m[11] * v2;
}

RGINLINE void rg_mat3x4_transpose(const rg_mat3x4* m, rg_mat4x3* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[4];
	out->m[2] = m->m[8];
	out->m[3] = m->m[1];
	out->m[4] = m->m[5];
	out->m[5] = m->m[9];
	out->m[6] = m->m[2];
	out->m[7] = m->m[6];
	out->m[8] = m->m[10];
	out->m[9] = m->m[3];
	out->m[10] = m->m[7];
	out->m[11] = m->m[11];
}

RGINLINE void rg_mat3x4_scale(const rg_mat3x4* m, f32 s, rg_mat3x4* out)
{
	for (int i = 0; i < 12; i++)
	{
		out->m[i] = m->m[i] * s;
	}
}

RGINLINE void rg_mat4x2_copy(const rg_mat4x2* m, rg_mat4x2* out)
{
	for (int i = 0; i < 8; i++)
	{
		out->m[i] = m->m[i];
	}
}

RGINLINE void rg_mat4x2_zero(rg_mat4x2* m)
{
	for (int i = 0; i < 8; i++)
	{
		m->m[i] = 0.0f;
	}
}

RGINLINE void rg_mat4x2_make(const f32* src, rg_mat4x2* out)
{
	for (int i = 0; i < 8; i++)
	{
		out->m[i] = src[i];
	}
}

RGINLINE void rg_mat4x2_mul(const rg_mat4x2* a, const rg_mat2x4* b, rg_mat2* out)
{
	f32 a00 = a->m[0];
	f32 a01 = a->m[1];
	f32 a10 = a->m[2];
	f32 a11 = a->m[3];
	f32 a20 = a->m[4];
	f32 a21 = a->m[5];
	f32 a30 = a->m[6];
	f32 a31 = a->m[7];

	f32 b00 = b->m[0];
	f32 b01 = b->m[1];
	f32 b02 = b->m[2];
	f32 b03 = b->m[3];
	f32 b10 = b->m[4];
	f32 b11 = b->m[5];
	f32 b12 = b->m[6];
	f32 b13 = b->m[7];

	out->m[0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
	out->m[1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
	out->m[2] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
	out->m[3] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
}

RGINLINE void rg_mat4x2_mulv(const rg_mat4x2* m, const rg_vec4* v, rg_vec2* out)
{
	f32 v0 = v->x;
	f32 v1 = v->y;
	f32 v2 = v->z;
	f32 v3 = v->w;

	out->x = m->m[0] * v0 + m->m[2] * v1 + m->m[4] * v2 + m->m[6] * v3;
	out->y = m->m[1] * v0 + m->m[3] * v1 + m->m[5] * v2 + m->m[7] * v3;
}

RGINLINE void rg_mat4x2_transpose(const rg_mat4x2* m, rg_mat2x4* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[2];
	out->m[2] = m->m[4];
	out->m[3] = m->m[6];
	out->m[4] = m->m[1];
	out->m[5] = m->m[3];
	out->m[6] = m->m[5];
	out->m[7] = m->m[7];
}

RGINLINE void rg_mat4x2_scale(const rg_mat4x2* m, f32 s, rg_mat4x2* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 sv = _mm_set1_ps(s);
	_mm_storeu_ps(&out->m[0], _mm_mul_ps(_mm_loadu_ps(&m->m[0]), sv));
	_mm_storeu_ps(&out->m[4], _mm_mul_ps(_mm_loadu_ps(&m->m[4]), sv));
#else
	out->m[0] = m->m[0] * s;
	out->m[1] = m->m[1] * s;
	out->m[2] = m->m[2] * s;
	out->m[3] = m->m[3] * s;
	out->m[4] = m->m[4] * s;
	out->m[5] = m->m[5] * s;
	out->m[6] = m->m[6] * s;
	out->m[7] = m->m[7] * s;
#endif
}

RGINLINE void rg_mat4x3_copy(const rg_mat4x3* m, rg_mat4x3* out)
{
	for (int i = 0; i < 12; i++)
	{
		out->m[i] = m->m[i];
	}
}

RGINLINE void rg_mat4x3_zero(rg_mat4x3* m)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 z = _mm_setzero_ps();
	_mm_storeu_ps(&m->m[0], z);
	_mm_storeu_ps(&m->m[4], z);
	_mm_storeu_ps(&m->m[8], z);
#else
	for (int i = 0; i < 12; i++)
	{
		m->m[i] = 0.0f;
	}
#endif
}

RGINLINE void rg_mat4x3_make(const f32* src, rg_mat4x3* out)
{
	for (int i = 0; i < 12; i++)
	{
		out->m[i] = src[i];
	}
}

RGINLINE void rg_mat4x3_mul(const rg_mat4x3* a, const rg_mat3x4* b, rg_mat3* out)
{
	f32 a00 = a->m[0];
	f32 a01 = a->m[1];
	f32 a02 = a->m[2];
	f32 a10 = a->m[3];
	f32 a11 = a->m[4];
	f32 a12 = a->m[5];
	f32 a20 = a->m[6];
	f32 a21 = a->m[7];
	f32 a22 = a->m[8];
	f32 a30 = a->m[9];
	f32 a31 = a->m[10];
	f32 a32 = a->m[11];

	f32 b00 = b->m[0];
	f32 b01 = b->m[1];
	f32 b02 = b->m[2];
	f32 b03 = b->m[3];
	f32 b10 = b->m[4];
	f32 b11 = b->m[5];
	f32 b12 = b->m[6];
	f32 b13 = b->m[7];
	f32 b20 = b->m[8];
	f32 b21 = b->m[9];
	f32 b22 = b->m[10];
	f32 b23 = b->m[11];

	out->m[0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
	out->m[1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
	out->m[2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
	out->m[3] = 0.0f;

	out->m[4] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
	out->m[5] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
	out->m[6] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
	out->m[7] = 0.0f;

	out->m[8] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
	out->m[9] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
	out->m[10] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
	out->m[11] = 0.0f;
}

RGINLINE void rg_mat4x3_mulv(const rg_mat4x3* m, const rg_vec4* v, rg_vec3* out)
{
	f32 v0 = v->x;
	f32 v1 = v->y;
	f32 v2 = v->z;
	f32 v3 = v->w;

	out->x = m->m[0] * v0 + m->m[3] * v1 + m->m[6] * v2 + m->m[9] * v3;
	out->y = m->m[1] * v0 + m->m[4] * v1 + m->m[7] * v2 + m->m[10] * v3;
	out->z = m->m[2] * v0 + m->m[5] * v1 + m->m[8] * v2 + m->m[11] * v3;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_mat4x3_transpose(const rg_mat4x3* m, rg_mat3x4* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[3];
	out->m[2] = m->m[6];
	out->m[3] = m->m[9];
	out->m[4] = m->m[1];
	out->m[5] = m->m[4];
	out->m[6] = m->m[7];
	out->m[7] = m->m[10];
	out->m[8] = m->m[2];
	out->m[9] = m->m[5];
	out->m[10] = m->m[8];
	out->m[11] = m->m[11];
}

RGINLINE void rg_mat4x3_scale(const rg_mat4x3* m, f32 s, rg_mat4x3* out)
{
	for (int i = 0; i < 12; i++)
	{
		out->m[i] = m->m[i] * s;
	}
}

// =============================================================================
// Mat4 Implementation
// =============================================================================

RGINLINE void rg_mat4_identity(rg_mat4* m)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_store_ps(&m->m[0], _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&m->m[4], _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f));
	_mm_store_ps(&m->m[8], _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f));
	_mm_store_ps(&m->m[12], _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));
#else
	m->m[0] = 1.0f;
	m->m[1] = 0.0f;
	m->m[2] = 0.0f;
	m->m[3] = 0.0f;
	m->m[4] = 0.0f;
	m->m[5] = 1.0f;
	m->m[6] = 0.0f;
	m->m[7] = 0.0f;
	m->m[8] = 0.0f;
	m->m[9] = 0.0f;
	m->m[10] = 1.0f;
	m->m[11] = 0.0f;
	m->m[12] = 0.0f;
	m->m[13] = 0.0f;
	m->m[14] = 0.0f;
	m->m[15] = 1.0f;
#endif
}

RGINLINE void rg_mat4_identity_array(rg_mat4* m, size_t count)
{
#if defined(RG_MATH_AVX) && RG_MATH_MAX_PERF
	__m256 c01 = _mm256_setr_ps(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	__m256 c23 = _mm256_setr_ps(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	for (size_t i = 0; i < count; i++)
	{
		_mm256_storeu_ps(&m[i].m[0], c01);
		_mm256_storeu_ps(&m[i].m[8], c23);
	}
#elif defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 c0 = _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f);
	__m128 c1 = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
	__m128 c2 = _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f);
	__m128 c3 = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
	for (size_t i = 0; i < count; i++)
	{
		_mm_store_ps(&m[i].m[0], c0);
		_mm_store_ps(&m[i].m[4], c1);
		_mm_store_ps(&m[i].m[8], c2);
		_mm_store_ps(&m[i].m[12], c3);
	}
#else
	for (size_t i = 0; i < count; i++)
	{
		rg_mat4_identity(&m[i]);
	}
#endif
}

RGINLINE void rg_mat4_zero(rg_mat4* m)
{
#if defined(RG_MATH_AVX)
	__m256 z = _mm256_setzero_ps();
	_mm256_storeu_ps(&m->m[0], z);
	_mm256_storeu_ps(&m->m[8], z);
#elif defined(RG_MATH_SSE)
	__m128 z = _mm_setzero_ps();
	_mm_store_ps(&m->m[0], z);
	_mm_store_ps(&m->m[4], z);
	_mm_store_ps(&m->m[8], z);
	_mm_store_ps(&m->m[12], z);
#else
	for (int i = 0; i < 16; i++)
	{
		m->m[i] = 0.0f;
	}
#endif
}

RGINLINE void rg_mat4_copy(const rg_mat4* m, rg_mat4* out)
{
#if defined(RG_MATH_SSE) && !RG_MATH_MAX_PERF
	__m128 v0 = _mm_load_ps(&m->m[0]);
	__m128 v1 = _mm_load_ps(&m->m[4]);
	__m128 v2 = _mm_load_ps(&m->m[8]);
	__m128 v3 = _mm_load_ps(&m->m[12]);
	_mm_store_ps(&out->m[0], v0);
	_mm_store_ps(&out->m[4], v1);
	_mm_store_ps(&out->m[8], v2);
	_mm_store_ps(&out->m[12], v3);
#else
	*out = *m;
#endif
}

RGINLINE void rg_mat4_ucopy(const rg_mat4* m, rg_mat4* out)
{
#if defined(RG_MATH_AVX) && RG_MATH_MAX_PERF
	__m256 v0 = _mm256_loadu_ps(&m->m[0]);
	__m256 v1 = _mm256_loadu_ps(&m->m[8]);
	_mm256_storeu_ps(&out->m[0], v0);
	_mm256_storeu_ps(&out->m[8], v1);
#elif defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 v0 = _mm_loadu_ps(&m->m[0]);
	__m128 v1 = _mm_loadu_ps(&m->m[4]);
	__m128 v2 = _mm_loadu_ps(&m->m[8]);
	__m128 v3 = _mm_loadu_ps(&m->m[12]);
	_mm_storeu_ps(&out->m[0], v0);
	_mm_storeu_ps(&out->m[4], v1);
	_mm_storeu_ps(&out->m[8], v2);
	_mm_storeu_ps(&out->m[12], v3);
#else
	for (int i = 0; i < 16; i++)
	{
		out->m[i] = m->m[i];
	}
#endif
}

RGINLINE void rg_mat4_transpose(const rg_mat4* m, rg_mat4* out)
{
#ifdef RG_MATH_SSE
	__m128 c0 = _mm_load_ps(&m->m[0]);
	__m128 c1 = _mm_load_ps(&m->m[4]);
	__m128 c2 = _mm_load_ps(&m->m[8]);
	__m128 c3 = _mm_load_ps(&m->m[12]);
	_MM_TRANSPOSE4_PS(c0, c1, c2, c3);
	_mm_store_ps(&out->m[0], c0);
	_mm_store_ps(&out->m[4], c1);
	_mm_store_ps(&out->m[8], c2);
	_mm_store_ps(&out->m[12], c3);
#else
	out->m[0] = m->m[0];
	out->m[1] = m->m[4];
	out->m[2] = m->m[8];
	out->m[3] = m->m[12];
	out->m[4] = m->m[1];
	out->m[5] = m->m[5];
	out->m[6] = m->m[9];
	out->m[7] = m->m[13];
	out->m[8] = m->m[2];
	out->m[9] = m->m[6];
	out->m[10] = m->m[10];
	out->m[11] = m->m[14];
	out->m[12] = m->m[3];
	out->m[13] = m->m[7];
	out->m[14] = m->m[11];
	out->m[15] = m->m[15];
#endif
}

RGINLINE void rg_mat4_pick3(const rg_mat4* m, rg_mat3* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[1];
	out->m[2] = m->m[2];
	out->m[3] = 0.0f;

	out->m[4] = m->m[4];
	out->m[5] = m->m[5];
	out->m[6] = m->m[6];
	out->m[7] = 0.0f;

	out->m[8] = m->m[8];
	out->m[9] = m->m[9];
	out->m[10] = m->m[10];
	out->m[11] = 0.0f;
}

RGINLINE void rg_mat4_pick3t(const rg_mat4* m, rg_mat3* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[4];
	out->m[2] = m->m[8];
	out->m[3] = 0.0f;

	out->m[4] = m->m[1];
	out->m[5] = m->m[5];
	out->m[6] = m->m[9];
	out->m[7] = 0.0f;

	out->m[8] = m->m[2];
	out->m[9] = m->m[6];
	out->m[10] = m->m[10];
	out->m[11] = 0.0f;
}

RGINLINE void rg_mat4_ins3(const rg_mat3* m, rg_mat4* out)
{
	out->m[0] = m->m[0];
	out->m[1] = m->m[1];
	out->m[2] = m->m[2];

	out->m[4] = m->m[4];
	out->m[5] = m->m[5];
	out->m[6] = m->m[6];

	out->m[8] = m->m[8];
	out->m[9] = m->m[9];
	out->m[10] = m->m[10];
}

RGINLINE f32 rg_mat4_trace(const rg_mat4* m)
{
	return m->m[0] + m->m[5] + m->m[10] + m->m[15];
}

RGINLINE f32 rg_mat4_trace3(const rg_mat4* m)
{
	return m->m[0] + m->m[5] + m->m[10];
}

RGINLINE void rg_mat4_scale_p(rg_mat4* m, f32 s)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 sv = _mm_set1_ps(s);
	_mm_store_ps(&m->m[0], _mm_mul_ps(_mm_load_ps(&m->m[0]), sv));
	_mm_store_ps(&m->m[4], _mm_mul_ps(_mm_load_ps(&m->m[4]), sv));
	_mm_store_ps(&m->m[8], _mm_mul_ps(_mm_load_ps(&m->m[8]), sv));
	_mm_store_ps(&m->m[12], _mm_mul_ps(_mm_load_ps(&m->m[12]), sv));
#else
	m->m[0] *= s;
	m->m[1] *= s;
	m->m[2] *= s;
	m->m[3] *= s;
	m->m[4] *= s;
	m->m[5] *= s;
	m->m[6] *= s;
	m->m[7] *= s;
	m->m[8] *= s;
	m->m[9] *= s;
	m->m[10] *= s;
	m->m[11] *= s;
	m->m[12] *= s;
	m->m[13] *= s;
	m->m[14] *= s;
	m->m[15] *= s;
#endif
}

RGINLINE void rg_mat4_scale_scalar(const rg_mat4* m, f32 s, rg_mat4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 sv = _mm_set1_ps(s);
	_mm_store_ps(&out->m[0], _mm_mul_ps(_mm_load_ps(&m->m[0]), sv));
	_mm_store_ps(&out->m[4], _mm_mul_ps(_mm_load_ps(&m->m[4]), sv));
	_mm_store_ps(&out->m[8], _mm_mul_ps(_mm_load_ps(&m->m[8]), sv));
	_mm_store_ps(&out->m[12], _mm_mul_ps(_mm_load_ps(&m->m[12]), sv));
#else
	out->m[0] = m->m[0] * s;
	out->m[1] = m->m[1] * s;
	out->m[2] = m->m[2] * s;
	out->m[3] = m->m[3] * s;
	out->m[4] = m->m[4] * s;
	out->m[5] = m->m[5] * s;
	out->m[6] = m->m[6] * s;
	out->m[7] = m->m[7] * s;
	out->m[8] = m->m[8] * s;
	out->m[9] = m->m[9] * s;
	out->m[10] = m->m[10] * s;
	out->m[11] = m->m[11] * s;
	out->m[12] = m->m[12] * s;
	out->m[13] = m->m[13] * s;
	out->m[14] = m->m[14] * s;
	out->m[15] = m->m[15] * s;
#endif
}

RGINLINE void rg_mat4_swap_col(rg_mat4* m, int col1, int col2)
{
	if (col1 == col2)
	{
		return;
	}

	int c1 = col1 * 4;
	int c2 = col2 * 4;
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 a = _mm_load_ps(&m->m[c1]);
	__m128 b = _mm_load_ps(&m->m[c2]);
	_mm_store_ps(&m->m[c1], b);
	_mm_store_ps(&m->m[c2], a);
#else
	f32 t0 = m->m[c1];
	f32 t1 = m->m[c1 + 1];
	f32 t2 = m->m[c1 + 2];
	f32 t3 = m->m[c1 + 3];
	m->m[c1] = m->m[c2];
	m->m[c1 + 1] = m->m[c2 + 1];
	m->m[c1 + 2] = m->m[c2 + 2];
	m->m[c1 + 3] = m->m[c2 + 3];
	m->m[c2] = t0;
	m->m[c2 + 1] = t1;
	m->m[c2 + 2] = t2;
	m->m[c2 + 3] = t3;
#endif
}

RGINLINE void rg_mat4_swap_row(rg_mat4* m, int row1, int row2)
{
	if (row1 == row2)
	{
		return;
	}

	f32 t0 = m->m[row1];
	f32 t1 = m->m[4 + row1];
	f32 t2 = m->m[8 + row1];
	f32 t3 = m->m[12 + row1];

	m->m[row1] = m->m[row2];
	m->m[4 + row1] = m->m[4 + row2];
	m->m[8 + row1] = m->m[8 + row2];
	m->m[12 + row1] = m->m[12 + row2];

	m->m[row2] = t0;
	m->m[4 + row2] = t1;
	m->m[8 + row2] = t2;
	m->m[12 + row2] = t3;
}

RGINLINE void rg_mat4_make(const f32* src, rg_mat4* out)
{
#ifdef RG_MATH_SSE
	__m128 c0 = _mm_loadu_ps(&src[0]);
	__m128 c1 = _mm_loadu_ps(&src[4]);
	__m128 c2 = _mm_loadu_ps(&src[8]);
	__m128 c3 = _mm_loadu_ps(&src[12]);
	_mm_store_ps(&out->m[0], c0);
	_mm_store_ps(&out->m[4], c1);
	_mm_store_ps(&out->m[8], c2);
	_mm_store_ps(&out->m[12], c3);
#else
	out->m[0] = src[0];
	out->m[1] = src[1];
	out->m[2] = src[2];
	out->m[3] = src[3];
	out->m[4] = src[4];
	out->m[5] = src[5];
	out->m[6] = src[6];
	out->m[7] = src[7];
	out->m[8] = src[8];
	out->m[9] = src[9];
	out->m[10] = src[10];
	out->m[11] = src[11];
	out->m[12] = src[12];
	out->m[13] = src[13];
	out->m[14] = src[14];
	out->m[15] = src[15];
#endif
}

RGINLINE void rg_mat4_translate_make(rg_mat4* m, const rg_vec3* v)
{
#if defined(RG_MATH_SSE) && !RG_MATH_MAX_PERF
	_mm_store_ps(&m->m[0], _mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&m->m[4], _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f));
	_mm_store_ps(&m->m[8], _mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f));
	_mm_store_ps(&m->m[12], RG_VEC3_LOAD(v));
	m->m[15] = 1.0f;
#else
	m->m[0] = 1.0f;
	m->m[1] = 0.0f;
	m->m[2] = 0.0f;
	m->m[3] = 0.0f;
	m->m[4] = 0.0f;
	m->m[5] = 1.0f;
	m->m[6] = 0.0f;
	m->m[7] = 0.0f;
	m->m[8] = 0.0f;
	m->m[9] = 0.0f;
	m->m[10] = 1.0f;
	m->m[11] = 0.0f;
	m->m[12] = v->x;
	m->m[13] = v->y;
	m->m[14] = v->z;
	m->m[15] = 1.0f;
#endif
}

RGINLINE void rg_mat4_scale_make(rg_mat4* m, const rg_vec3* v)
{
#if defined(RG_MATH_SSE) && !RG_MATH_MAX_PERF
	_mm_store_ps(&m->m[0], _mm_setr_ps(v->x, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&m->m[4], _mm_setr_ps(0.0f, v->y, 0.0f, 0.0f));
	_mm_store_ps(&m->m[8], _mm_setr_ps(0.0f, 0.0f, v->z, 0.0f));
	_mm_store_ps(&m->m[12], _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f));
#else
	m->m[0] = v->x;
	m->m[1] = 0.0f;
	m->m[2] = 0.0f;
	m->m[3] = 0.0f;
	m->m[4] = 0.0f;
	m->m[5] = v->y;
	m->m[6] = 0.0f;
	m->m[7] = 0.0f;
	m->m[8] = 0.0f;
	m->m[9] = 0.0f;
	m->m[10] = v->z;
	m->m[11] = 0.0f;
	m->m[12] = 0.0f;
	m->m[13] = 0.0f;
	m->m[14] = 0.0f;
	m->m[15] = 1.0f;
#endif
}

RGINLINE int rg__mat4_rotate_make_cardinal(rg_mat4* m, f32 s, f32 c, f32 x, f32 y, f32 z)
{
	if (y == 0.0f && z == 0.0f && (x == 1.0f || x == -1.0f))
	{
		if (x < 0.0f) s = -s;
		m->m[0] = 1.0f;
		m->m[1] = 0.0f;
		m->m[2] = 0.0f;
		m->m[3] = 0.0f;
		m->m[4] = 0.0f;
		m->m[5] = c;
		m->m[6] = s;
		m->m[7] = 0.0f;
		m->m[8] = 0.0f;
		m->m[9] = -s;
		m->m[10] = c;
		m->m[11] = 0.0f;
		m->m[12] = 0.0f;
		m->m[13] = 0.0f;
		m->m[14] = 0.0f;
		m->m[15] = 1.0f;
		return 1;
	}
	if (x == 0.0f && z == 0.0f && (y == 1.0f || y == -1.0f))
	{
		if (y < 0.0f) s = -s;
		m->m[0] = c;
		m->m[1] = 0.0f;
		m->m[2] = -s;
		m->m[3] = 0.0f;
		m->m[4] = 0.0f;
		m->m[5] = 1.0f;
		m->m[6] = 0.0f;
		m->m[7] = 0.0f;
		m->m[8] = s;
		m->m[9] = 0.0f;
		m->m[10] = c;
		m->m[11] = 0.0f;
		m->m[12] = 0.0f;
		m->m[13] = 0.0f;
		m->m[14] = 0.0f;
		m->m[15] = 1.0f;
		return 1;
	}
	if (x == 0.0f && y == 0.0f && (z == 1.0f || z == -1.0f))
	{
		if (z < 0.0f) s = -s;
		m->m[0] = c;
		m->m[1] = s;
		m->m[2] = 0.0f;
		m->m[3] = 0.0f;
		m->m[4] = -s;
		m->m[5] = c;
		m->m[6] = 0.0f;
		m->m[7] = 0.0f;
		m->m[8] = 0.0f;
		m->m[9] = 0.0f;
		m->m[10] = 1.0f;
		m->m[11] = 0.0f;
		m->m[12] = 0.0f;
		m->m[13] = 0.0f;
		m->m[14] = 0.0f;
		m->m[15] = 1.0f;
		return 1;
	}

	return 0;
}

RGINLINE void rg_mat4_rotate_make(rg_mat4* m, f32 angle, const rg_vec3* axis)
{
#if RG_MATH_MAX_PERF
	f32 x = axis->x;
	f32 y = axis->y;
	f32 z = axis->z;
#else
	rg_vec3 n = *axis;
	rg_vec3_normalize(axis, &n);
	f32 x = n.x;
	f32 y = n.y;
	f32 z = n.z;
#endif
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);
	if (rg__mat4_rotate_make_cardinal(m, s, c, x, y, z))
	{
		return;
	}

	f32 t = 1.0f - c;
	f32 tx = t * x;
	f32 ty = t * y;
	f32 tz = t * z;
	f32 txy = tx * y;
	f32 txz = tx * z;
	f32 tyz = ty * z;
	f32 sx = s * x;
	f32 sy = s * y;
	f32 sz = s * z;

	m->m[0] = tx * x + c;
	m->m[1] = txy + sz;
	m->m[2] = txz - sy;
	m->m[3] = 0.0f;

	m->m[4] = txy - sz;
	m->m[5] = ty * y + c;
	m->m[6] = tyz + sx;
	m->m[7] = 0.0f;

	m->m[8] = txz + sy;
	m->m[9] = tyz - sx;
	m->m[10] = tz * z + c;
	m->m[11] = 0.0f;

	m->m[12] = 0.0f;
	m->m[13] = 0.0f;
	m->m[14] = 0.0f;
	m->m[15] = 1.0f;
}

RGINLINE void rg_mat4_rotate_make_fast(rg_mat4* m, f32 angle, const rg_vec3* axis)
{
#if RG_MATH_MAX_PERF
	f32 x = axis->x;
	f32 y = axis->y;
	f32 z = axis->z;
#else
	rg_vec3 n = *axis;
	rg_vec3_normalize(axis, &n);
	f32 x = n.x;
	f32 y = n.y;
	f32 z = n.z;
#endif
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);
	if (rg__mat4_rotate_make_cardinal(m, s, c, x, y, z))
	{
		return;
	}

	f32 t = 1.0f - c;
	f32 tx = t * x;
	f32 ty = t * y;
	f32 tz = t * z;
	f32 txy = tx * y;
	f32 txz = tx * z;
	f32 tyz = ty * z;
	f32 sx = s * x;
	f32 sy = s * y;
	f32 sz = s * z;

	m->m[0] = tx * x + c;
	m->m[1] = txy + sz;
	m->m[2] = txz - sy;
	m->m[3] = 0.0f;

	m->m[4] = txy - sz;
	m->m[5] = ty * y + c;
	m->m[6] = tyz + sx;
	m->m[7] = 0.0f;

	m->m[8] = txz + sy;
	m->m[9] = tyz - sx;
	m->m[10] = tz * z + c;
	m->m[11] = 0.0f;

	m->m[12] = 0.0f;
	m->m[13] = 0.0f;
	m->m[14] = 0.0f;
	m->m[15] = 1.0f;
}

RGINLINE void rg_mat4_from_quat(rg_mat4* m, const rg_quat* q)
{
	f32 x = q->x;
	f32 y = q->y;
	f32 z = q->z;
	f32 w = q->w;

#if RG_MATH_MAX_PERF
	f32 k = 2.0f;
#else
	f32 norm = x * x + y * y + z * z + w * w;
	if (norm < RG_EPSILON * RG_EPSILON)
	{
		rg_mat4_identity(m);
		return;
	}
	f32 k = 2.0f / norm;
#endif

	f32 xk = x * k;
	f32 yk = y * k;
	f32 zk = z * k;
	f32 xx = x * xk;
	f32 xy = x * yk;
	f32 xz = x * zk;
	f32 yy = y * yk;
	f32 yz = y * zk;
	f32 zz = z * zk;
	f32 wx = w * xk;
	f32 wy = w * yk;
	f32 wz = w * zk;

	m->m[0] = 1.0f - yy - zz;
	m->m[1] = xy + wz;
	m->m[2] = xz - wy;
	m->m[3] = 0.0f;

	m->m[4] = xy - wz;
	m->m[5] = 1.0f - xx - zz;
	m->m[6] = yz + wx;
	m->m[7] = 0.0f;

	m->m[8] = xz + wy;
	m->m[9] = yz - wx;
	m->m[10] = 1.0f - xx - yy;
	m->m[11] = 0.0f;

	m->m[12] = 0.0f;
	m->m[13] = 0.0f;
	m->m[14] = 0.0f;
	m->m[15] = 1.0f;
}

RGINLINE void rg_mat4_trs_make(rg_mat4* m, const rg_vec3* t, const rg_quat* r, const rg_vec3* s)
{
	f32 x = r->x;
	f32 y = r->y;
	f32 z = r->z;
	f32 w = r->w;

#if RG_MATH_MAX_PERF
	f32 k = 2.0f;
#else
	f32 norm = x * x + y * y + z * z + w * w;
	if (norm < RG_EPSILON * RG_EPSILON)
	{
		rg_mat4_identity(m);
		m->m[12] = t->x;
		m->m[13] = t->y;
		m->m[14] = t->z;
		return;
	}
	f32 k = 2.0f / norm;
#endif

	f32 xx = x * x * k;
	f32 xy = x * y * k;
	f32 xz = x * z * k;
	f32 yy = y * y * k;
	f32 yz = y * z * k;
	f32 zz = z * z * k;
	f32 wx = w * x * k;
	f32 wy = w * y * k;
	f32 wz = w * z * k;

	f32 sx = s->x;
	f32 sy = s->y;
	f32 sz = s->z;

	m->m[0] = (1.0f - yy - zz) * sx;
	m->m[1] = (xy + wz) * sx;
	m->m[2] = (xz - wy) * sx;
	m->m[3] = 0.0f;

	m->m[4] = (xy - wz) * sy;
	m->m[5] = (1.0f - xx - zz) * sy;
	m->m[6] = (yz + wx) * sy;
	m->m[7] = 0.0f;

	m->m[8] = (xz + wy) * sz;
	m->m[9] = (yz - wx) * sz;
	m->m[10] = (1.0f - xx - yy) * sz;
	m->m[11] = 0.0f;

	m->m[12] = t->x;
	m->m[13] = t->y;
	m->m[14] = t->z;
	m->m[15] = 1.0f;
}

RGINLINE void rg_mat4_translate(const rg_mat4* m, const rg_vec3* v, rg_mat4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 c0 = _mm_loadu_ps(&m->m[0]);
	__m128 c1 = _mm_loadu_ps(&m->m[4]);
	__m128 c2 = _mm_loadu_ps(&m->m[8]);
	__m128 c3 = _mm_loadu_ps(&m->m[12]);

	if (out != m)
	{
		_mm_storeu_ps(&out->m[0], c0);
		_mm_storeu_ps(&out->m[4], c1);
		_mm_storeu_ps(&out->m[8], c2);
	}

	__m128 r = _mm_add_ps(c3, _mm_add_ps(_mm_mul_ps(c0, _mm_set1_ps(v->x)),
	                                     _mm_add_ps(_mm_mul_ps(c1, _mm_set1_ps(v->y)),
	                                                _mm_mul_ps(c2, _mm_set1_ps(v->z)))));
	_mm_storeu_ps(&out->m[12], r);
#else
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m3 = m->m[3];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m7 = m->m[7];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m11 = m->m[11];
	f32 m12 = m->m[12];
	f32 m13 = m->m[13];
	f32 m14 = m->m[14];
	f32 m15 = m->m[15];

	if (out != m)
	{
		out->m[0] = m0;
		out->m[1] = m1;
		out->m[2] = m2;
		out->m[3] = m3;
		out->m[4] = m4;
		out->m[5] = m5;
		out->m[6] = m6;
		out->m[7] = m7;
		out->m[8] = m8;
		out->m[9] = m9;
		out->m[10] = m10;
		out->m[11] = m11;
	}

	out->m[12] = m0 * v->x + m4 * v->y + m8 * v->z + m12;
	out->m[13] = m1 * v->x + m5 * v->y + m9 * v->z + m13;
	out->m[14] = m2 * v->x + m6 * v->y + m10 * v->z + m14;
	out->m[15] = m3 * v->x + m7 * v->y + m11 * v->z + m15;
#endif
}

RGINLINE void rg_mat4_translate_x(const rg_mat4* m, f32 x, rg_mat4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	if (out != m)
	{
		_mm_storeu_ps(&out->m[0], _mm_loadu_ps(&m->m[0]));
		_mm_storeu_ps(&out->m[4], _mm_loadu_ps(&m->m[4]));
		_mm_storeu_ps(&out->m[8], _mm_loadu_ps(&m->m[8]));
	}

	__m128 col0 = _mm_loadu_ps(&m->m[0]);
	__m128 col3 = _mm_loadu_ps(&m->m[12]);
	_mm_storeu_ps(&out->m[12], _mm_add_ps(col3, _mm_mul_ps(col0, _mm_set1_ps(x))));
#else
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m3 = m->m[3];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m7 = m->m[7];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m11 = m->m[11];
	f32 m12 = m->m[12];
	f32 m13 = m->m[13];
	f32 m14 = m->m[14];
	f32 m15 = m->m[15];

	if (out != m)
	{
		out->m[0] = m0;
		out->m[1] = m1;
		out->m[2] = m2;
		out->m[3] = m3;
		out->m[4] = m4;
		out->m[5] = m5;
		out->m[6] = m6;
		out->m[7] = m7;
		out->m[8] = m8;
		out->m[9] = m9;
		out->m[10] = m10;
		out->m[11] = m11;
	}

	out->m[12] = m0 * x + m12;
	out->m[13] = m1 * x + m13;
	out->m[14] = m2 * x + m14;
	out->m[15] = m3 * x + m15;
#endif
}

RGINLINE void rg_mat4_translate_y(const rg_mat4* m, f32 y, rg_mat4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	if (out != m)
	{
		_mm_storeu_ps(&out->m[0], _mm_loadu_ps(&m->m[0]));
		_mm_storeu_ps(&out->m[4], _mm_loadu_ps(&m->m[4]));
		_mm_storeu_ps(&out->m[8], _mm_loadu_ps(&m->m[8]));
	}

	__m128 col1 = _mm_loadu_ps(&m->m[4]);
	__m128 col3 = _mm_loadu_ps(&m->m[12]);
	_mm_storeu_ps(&out->m[12], _mm_add_ps(col3, _mm_mul_ps(col1, _mm_set1_ps(y))));
#else
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m3 = m->m[3];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m7 = m->m[7];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m11 = m->m[11];
	f32 m12 = m->m[12];
	f32 m13 = m->m[13];
	f32 m14 = m->m[14];
	f32 m15 = m->m[15];

	if (out != m)
	{
		out->m[0] = m0;
		out->m[1] = m1;
		out->m[2] = m2;
		out->m[3] = m3;
		out->m[4] = m4;
		out->m[5] = m5;
		out->m[6] = m6;
		out->m[7] = m7;
		out->m[8] = m8;
		out->m[9] = m9;
		out->m[10] = m10;
		out->m[11] = m11;
	}

	out->m[12] = m4 * y + m12;
	out->m[13] = m5 * y + m13;
	out->m[14] = m6 * y + m14;
	out->m[15] = m7 * y + m15;
#endif
}

RGINLINE void rg_mat4_translate_z(const rg_mat4* m, f32 z, rg_mat4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	if (out != m)
	{
		_mm_storeu_ps(&out->m[0], _mm_loadu_ps(&m->m[0]));
		_mm_storeu_ps(&out->m[4], _mm_loadu_ps(&m->m[4]));
		_mm_storeu_ps(&out->m[8], _mm_loadu_ps(&m->m[8]));
	}

	__m128 col2 = _mm_loadu_ps(&m->m[8]);
	__m128 col3 = _mm_loadu_ps(&m->m[12]);
	_mm_storeu_ps(&out->m[12], _mm_add_ps(col3, _mm_mul_ps(col2, _mm_set1_ps(z))));
#else
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m3 = m->m[3];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m7 = m->m[7];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m11 = m->m[11];
	f32 m12 = m->m[12];
	f32 m13 = m->m[13];
	f32 m14 = m->m[14];
	f32 m15 = m->m[15];

	if (out != m)
	{
		out->m[0] = m0;
		out->m[1] = m1;
		out->m[2] = m2;
		out->m[3] = m3;
		out->m[4] = m4;
		out->m[5] = m5;
		out->m[6] = m6;
		out->m[7] = m7;
		out->m[8] = m8;
		out->m[9] = m9;
		out->m[10] = m10;
		out->m[11] = m11;
	}

	out->m[12] = m8 * z + m12;
	out->m[13] = m9 * z + m13;
	out->m[14] = m10 * z + m14;
	out->m[15] = m11 * z + m15;
#endif
}

RGINLINE void rg_mat4_scale(const rg_mat4* m, const rg_vec3* v, rg_mat4* out)
{
#ifdef RG_MATH_SSE
	_mm_storeu_ps(&out->m[0], _mm_mul_ps(_mm_loadu_ps(&m->m[0]), _mm_set1_ps(v->x)));
	_mm_storeu_ps(&out->m[4], _mm_mul_ps(_mm_loadu_ps(&m->m[4]), _mm_set1_ps(v->y)));
	_mm_storeu_ps(&out->m[8], _mm_mul_ps(_mm_loadu_ps(&m->m[8]), _mm_set1_ps(v->z)));
	if (out != m)
	{
		_mm_storeu_ps(&out->m[12], _mm_loadu_ps(&m->m[12]));
	}
#else
	out->m[0] = m->m[0] * v->x;
	out->m[1] = m->m[1] * v->x;
	out->m[2] = m->m[2] * v->x;
	out->m[3] = m->m[3] * v->x;

	out->m[4] = m->m[4] * v->y;
	out->m[5] = m->m[5] * v->y;
	out->m[6] = m->m[6] * v->y;
	out->m[7] = m->m[7] * v->y;

	out->m[8] = m->m[8] * v->z;
	out->m[9] = m->m[9] * v->z;
	out->m[10] = m->m[10] * v->z;
	out->m[11] = m->m[11] * v->z;

	out->m[12] = m->m[12];
	out->m[13] = m->m[13];
	out->m[14] = m->m[14];
	out->m[15] = m->m[15];
#endif
}

RGINLINE void rg_mat4_scale_uni(const rg_mat4* m, f32 s, rg_mat4* out)
{
#ifdef RG_MATH_SSE
	__m128 sv = _mm_set1_ps(s);
	_mm_storeu_ps(&out->m[0], _mm_mul_ps(_mm_loadu_ps(&m->m[0]), sv));
	_mm_storeu_ps(&out->m[4], _mm_mul_ps(_mm_loadu_ps(&m->m[4]), sv));
	_mm_storeu_ps(&out->m[8], _mm_mul_ps(_mm_loadu_ps(&m->m[8]), sv));
	if (out != m)
	{
		_mm_storeu_ps(&out->m[12], _mm_loadu_ps(&m->m[12]));
	}
#else
	out->m[0] = m->m[0] * s;
	out->m[1] = m->m[1] * s;
	out->m[2] = m->m[2] * s;
	out->m[3] = m->m[3] * s;

	out->m[4] = m->m[4] * s;
	out->m[5] = m->m[5] * s;
	out->m[6] = m->m[6] * s;
	out->m[7] = m->m[7] * s;

	out->m[8] = m->m[8] * s;
	out->m[9] = m->m[9] * s;
	out->m[10] = m->m[10] * s;
	out->m[11] = m->m[11] * s;

	out->m[12] = m->m[12];
	out->m[13] = m->m[13];
	out->m[14] = m->m[14];
	out->m[15] = m->m[15];
#endif
}

RGINLINE void rg_mat4_rotate(const rg_mat4* m, f32 angle, const rg_vec3* axis, rg_mat4* out)
{
	rg_mat4_rotate_fast(m, angle, axis, out);
}

RGINLINE void rg_mat4_rotate_fast(const rg_mat4* m, f32 angle, const rg_vec3* axis, rg_mat4* out)
{
#if RG_MATH_MAX_PERF
	f32 x = axis->x;
	f32 y = axis->y;
	f32 z = axis->z;
#else
	rg_vec3 n = *axis;
	rg_vec3_normalize(axis, &n);
	f32 x = n.x;
	f32 y = n.y;
	f32 z = n.z;
#endif
	if (y == 0.0f && z == 0.0f && (x == 1.0f || x == -1.0f))
	{
		rg_mat4_rotate_x(m, x < 0.0f ? -angle : angle, out);
		return;
	}
	if (x == 0.0f && z == 0.0f && (y == 1.0f || y == -1.0f))
	{
		rg_mat4_rotate_y(m, y < 0.0f ? -angle : angle, out);
		return;
	}
	if (x == 0.0f && y == 0.0f && (z == 1.0f || z == -1.0f))
	{
		rg_mat4_rotate_z(m, z < 0.0f ? -angle : angle, out);
		return;
	}

	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);
	f32 t = 1.0f - c;
	f32 tx = t * x;
	f32 ty = t * y;
	f32 tz = t * z;
	f32 r00 = tx * x + c;
	f32 r01 = tx * y + s * z;
	f32 r02 = tx * z - s * y;
	f32 r10 = ty * x - s * z;
	f32 r11 = ty * y + c;
	f32 r12 = ty * z + s * x;
	f32 r20 = tz * x + s * y;
	f32 r21 = tz * y - s * x;
	f32 r22 = tz * z + c;

#ifdef RG_MATH_SSE
	__m128 c0 = _mm_load_ps(&m->m[0]);
	__m128 c1 = _mm_load_ps(&m->m[4]);
	__m128 c2 = _mm_load_ps(&m->m[8]);
	__m128 c3 = _mm_load_ps(&m->m[12]);

	__m128 o0 = _mm_mul_ps(c0, _mm_set1_ps(r00));
	o0 = RG_MATH_FMADD_PS(c1, _mm_set1_ps(r01), o0);
	o0 = RG_MATH_FMADD_PS(c2, _mm_set1_ps(r02), o0);

	__m128 o1 = _mm_mul_ps(c0, _mm_set1_ps(r10));
	o1 = RG_MATH_FMADD_PS(c1, _mm_set1_ps(r11), o1);
	o1 = RG_MATH_FMADD_PS(c2, _mm_set1_ps(r12), o1);

	__m128 o2 = _mm_mul_ps(c0, _mm_set1_ps(r20));
	o2 = RG_MATH_FMADD_PS(c1, _mm_set1_ps(r21), o2);
	o2 = RG_MATH_FMADD_PS(c2, _mm_set1_ps(r22), o2);

	_mm_store_ps(&out->m[0], o0);
	_mm_store_ps(&out->m[4], o1);
	_mm_store_ps(&out->m[8], o2);
	_mm_store_ps(&out->m[12], c3);
#else
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m3 = m->m[3];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m7 = m->m[7];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m11 = m->m[11];

	out->m[0] = m0 * r00 + m4 * r01 + m8 * r02;
	out->m[1] = m1 * r00 + m5 * r01 + m9 * r02;
	out->m[2] = m2 * r00 + m6 * r01 + m10 * r02;
	out->m[3] = m3 * r00 + m7 * r01 + m11 * r02;

	out->m[4] = m0 * r10 + m4 * r11 + m8 * r12;
	out->m[5] = m1 * r10 + m5 * r11 + m9 * r12;
	out->m[6] = m2 * r10 + m6 * r11 + m10 * r12;
	out->m[7] = m3 * r10 + m7 * r11 + m11 * r12;

	out->m[8] = m0 * r20 + m4 * r21 + m8 * r22;
	out->m[9] = m1 * r20 + m5 * r21 + m9 * r22;
	out->m[10] = m2 * r20 + m6 * r21 + m10 * r22;
	out->m[11] = m3 * r20 + m7 * r21 + m11 * r22;

	out->m[12] = m->m[12];
	out->m[13] = m->m[13];
	out->m[14] = m->m[14];
	out->m[15] = m->m[15];
#endif
}

RGINLINE void rg_mat4_rotate_x(const rg_mat4* m, f32 angle, rg_mat4* out)
{
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);

#ifdef RG_MATH_SSE
	__m128 c1 = _mm_loadu_ps(&m->m[4]);
	__m128 c2 = _mm_loadu_ps(&m->m[8]);
	__m128 cs = _mm_set1_ps(c);
	__m128 ss = _mm_set1_ps(s);
	__m128 r1 = _mm_add_ps(_mm_mul_ps(c1, cs), _mm_mul_ps(c2, ss));
	__m128 r2 = _mm_sub_ps(_mm_mul_ps(c2, cs), _mm_mul_ps(c1, ss));

	if (out != m)
	{
		_mm_storeu_ps(&out->m[0], _mm_loadu_ps(&m->m[0]));
		_mm_storeu_ps(&out->m[12], _mm_loadu_ps(&m->m[12]));
	}

	_mm_storeu_ps(&out->m[4], r1);
	_mm_storeu_ps(&out->m[8], r2);
#else
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m7 = m->m[7];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m11 = m->m[11];

	out->m[4] = m4 * c + m8 * s;
	out->m[5] = m5 * c + m9 * s;
	out->m[6] = m6 * c + m10 * s;
	out->m[7] = m7 * c + m11 * s;
	out->m[8] = m8 * c - m4 * s;
	out->m[9] = m9 * c - m5 * s;
	out->m[10] = m10 * c - m6 * s;
	out->m[11] = m11 * c - m7 * s;

	if (out != m)
	{
		out->m[0] = m->m[0];
		out->m[1] = m->m[1];
		out->m[2] = m->m[2];
		out->m[3] = m->m[3];
		out->m[12] = m->m[12];
		out->m[13] = m->m[13];
		out->m[14] = m->m[14];
		out->m[15] = m->m[15];
	}
#endif
}

RGINLINE void rg_mat4_rotate_y(const rg_mat4* m, f32 angle, rg_mat4* out)
{
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);

#ifdef RG_MATH_SSE
	__m128 c0 = _mm_loadu_ps(&m->m[0]);
	__m128 c2 = _mm_loadu_ps(&m->m[8]);
	__m128 cs = _mm_set1_ps(c);
	__m128 ss = _mm_set1_ps(s);
	__m128 r0 = _mm_sub_ps(_mm_mul_ps(c0, cs), _mm_mul_ps(c2, ss));
	__m128 r2 = _mm_add_ps(_mm_mul_ps(c0, ss), _mm_mul_ps(c2, cs));

	if (out != m)
	{
		_mm_storeu_ps(&out->m[4], _mm_loadu_ps(&m->m[4]));
		_mm_storeu_ps(&out->m[12], _mm_loadu_ps(&m->m[12]));
	}

	_mm_storeu_ps(&out->m[0], r0);
	_mm_storeu_ps(&out->m[8], r2);
#else
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m3 = m->m[3];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m11 = m->m[11];

	out->m[0] = m0 * c - m8 * s;
	out->m[1] = m1 * c - m9 * s;
	out->m[2] = m2 * c - m10 * s;
	out->m[3] = m3 * c - m11 * s;
	out->m[8] = m0 * s + m8 * c;
	out->m[9] = m1 * s + m9 * c;
	out->m[10] = m2 * s + m10 * c;
	out->m[11] = m3 * s + m11 * c;

	if (out != m)
	{
		out->m[4] = m->m[4];
		out->m[5] = m->m[5];
		out->m[6] = m->m[6];
		out->m[7] = m->m[7];
		out->m[12] = m->m[12];
		out->m[13] = m->m[13];
		out->m[14] = m->m[14];
		out->m[15] = m->m[15];
	}
#endif
}

RGINLINE void rg_mat4_rotate_z(const rg_mat4* m, f32 angle, rg_mat4* out)
{
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);

#ifdef RG_MATH_SSE
	__m128 c0 = _mm_loadu_ps(&m->m[0]);
	__m128 c1 = _mm_loadu_ps(&m->m[4]);
	__m128 cs = _mm_set1_ps(c);
	__m128 ss = _mm_set1_ps(s);
	__m128 r0 = _mm_add_ps(_mm_mul_ps(c0, cs), _mm_mul_ps(c1, ss));
	__m128 r1 = _mm_sub_ps(_mm_mul_ps(c1, cs), _mm_mul_ps(c0, ss));

	if (out != m)
	{
		_mm_storeu_ps(&out->m[8], _mm_loadu_ps(&m->m[8]));
		_mm_storeu_ps(&out->m[12], _mm_loadu_ps(&m->m[12]));
	}

	_mm_storeu_ps(&out->m[0], r0);
	_mm_storeu_ps(&out->m[4], r1);
#else
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m3 = m->m[3];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m7 = m->m[7];

	out->m[0] = m0 * c + m4 * s;
	out->m[1] = m1 * c + m5 * s;
	out->m[2] = m2 * c + m6 * s;
	out->m[3] = m3 * c + m7 * s;
	out->m[4] = m4 * c - m0 * s;
	out->m[5] = m5 * c - m1 * s;
	out->m[6] = m6 * c - m2 * s;
	out->m[7] = m7 * c - m3 * s;

	if (out != m)
	{
		out->m[8] = m->m[8];
		out->m[9] = m->m[9];
		out->m[10] = m->m[10];
		out->m[11] = m->m[11];
		out->m[12] = m->m[12];
		out->m[13] = m->m[13];
		out->m[14] = m->m[14];
		out->m[15] = m->m[15];
	}
#endif
}

RGINLINE void rg_mat4_rotate_at(const rg_mat4* m, const rg_vec3* pivot, f32 angle, const rg_vec3* axis, rg_mat4* out)
{
#if RG_MATH_MAX_PERF
	f32 x = axis->x;
	f32 y = axis->y;
	f32 z = axis->z;
#else
	rg_vec3 n = *axis;
	rg_vec3_normalize(axis, &n);
	f32 x = n.x;
	f32 y = n.y;
	f32 z = n.z;
#endif
	f32 c = rg_cosf(angle);
	f32 s = rg_sinf(angle);
	f32 t = 1.0f - c;
	f32 tx = t * x;
	f32 ty = t * y;
	f32 tz = t * z;
	f32 r00 = tx * x + c;
	f32 r01 = tx * y + s * z;
	f32 r02 = tx * z - s * y;
	f32 r10 = tx * y - s * z;
	f32 r11 = ty * y + c;
	f32 r12 = ty * z + s * x;
	f32 r20 = tx * z + s * y;
	f32 r21 = ty * z - s * x;
	f32 r22 = tz * z + c;
	f32 px = pivot->x;
	f32 py = pivot->y;
	f32 pz = pivot->z;
	f32 r30 = px - (r00 * px + r10 * py + r20 * pz);
	f32 r31 = py - (r01 * px + r11 * py + r21 * pz);
	f32 r32 = pz - (r02 * px + r12 * py + r22 * pz);

#ifdef RG_MATH_SSE
	__m128 a0 = _mm_load_ps(&m->m[0]);
	__m128 a1 = _mm_load_ps(&m->m[4]);
	__m128 a2 = _mm_load_ps(&m->m[8]);
	__m128 a3 = _mm_load_ps(&m->m[12]);

	__m128 b0 = _mm_set_ps(0.0f, r02, r01, r00);
	__m128 b1 = _mm_set_ps(0.0f, r12, r11, r10);
	__m128 b2 = _mm_set_ps(0.0f, r22, r21, r20);
	__m128 b3 = _mm_set_ps(1.0f, r32, r31, r30);

	__m128 r0 = _mm_mul_ps(a0, _mm_shuffle_ps(b0, b0, _MM_SHUFFLE(0, 0, 0, 0)));
	r0 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b0, b0, _MM_SHUFFLE(1, 1, 1, 1)), r0);
	r0 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b0, b0, _MM_SHUFFLE(2, 2, 2, 2)), r0);

	__m128 r1 = _mm_mul_ps(a0, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(0, 0, 0, 0)));
	r1 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(1, 1, 1, 1)), r1);
	r1 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(2, 2, 2, 2)), r1);

	__m128 r2 = _mm_mul_ps(a0, _mm_shuffle_ps(b2, b2, _MM_SHUFFLE(0, 0, 0, 0)));
	r2 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b2, b2, _MM_SHUFFLE(1, 1, 1, 1)), r2);
	r2 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b2, b2, _MM_SHUFFLE(2, 2, 2, 2)), r2);

	__m128 r3 = _mm_mul_ps(a0, _mm_shuffle_ps(b3, b3, _MM_SHUFFLE(0, 0, 0, 0)));
	r3 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b3, b3, _MM_SHUFFLE(1, 1, 1, 1)), r3);
	r3 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b3, b3, _MM_SHUFFLE(2, 2, 2, 2)), r3);
	r3 = _mm_add_ps(r3, a3);

	_mm_store_ps(&out->m[0], r0);
	_mm_store_ps(&out->m[4], r1);
	_mm_store_ps(&out->m[8], r2);
	_mm_store_ps(&out->m[12], r3);
#else
	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m3 = m->m[3];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m7 = m->m[7];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m11 = m->m[11];
	f32 m12 = m->m[12];
	f32 m13 = m->m[13];
	f32 m14 = m->m[14];
	f32 m15 = m->m[15];

	out->m[0] = m0 * r00 + m4 * r01 + m8 * r02;
	out->m[1] = m1 * r00 + m5 * r01 + m9 * r02;
	out->m[2] = m2 * r00 + m6 * r01 + m10 * r02;
	out->m[3] = m3 * r00 + m7 * r01 + m11 * r02;
	out->m[4] = m0 * r10 + m4 * r11 + m8 * r12;
	out->m[5] = m1 * r10 + m5 * r11 + m9 * r12;
	out->m[6] = m2 * r10 + m6 * r11 + m10 * r12;
	out->m[7] = m3 * r10 + m7 * r11 + m11 * r12;
	out->m[8] = m0 * r20 + m4 * r21 + m8 * r22;
	out->m[9] = m1 * r20 + m5 * r21 + m9 * r22;
	out->m[10] = m2 * r20 + m6 * r21 + m10 * r22;
	out->m[11] = m3 * r20 + m7 * r21 + m11 * r22;
	out->m[12] = m0 * r30 + m4 * r31 + m8 * r32 + m12;
	out->m[13] = m1 * r30 + m5 * r31 + m9 * r32 + m13;
	out->m[14] = m2 * r30 + m6 * r31 + m10 * r32 + m14;
	out->m[15] = m3 * r30 + m7 * r31 + m11 * r32 + m15;
#endif
}

RGINLINE void rg_mat4_rotate_atm(rg_mat4* out, const rg_vec3* pivot, f32 angle, const rg_vec3* axis)
{
	rg_mat4_rotate_make(out, angle, axis);

	f32 px = pivot->x;
	f32 py = pivot->y;
	f32 pz = pivot->z;
	out->m[12] = px - (out->m[0] * px + out->m[4] * py + out->m[8] * pz);
	out->m[13] = py - (out->m[1] * px + out->m[5] * py + out->m[9] * pz);
	out->m[14] = pz - (out->m[2] * px + out->m[6] * py + out->m[10] * pz);
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_translated(rg_mat4* m, const rg_vec3* v)
{
	m->m[12] += v->x;
	m->m[13] += v->y;
	m->m[14] += v->z;
}

RGINLINE void rg_mat4_translated_to(const rg_mat4* m, const rg_vec3* v, rg_mat4* out)
{
	f32 x = v->x;
	f32 y = v->y;
	f32 z = v->z;

	out->m[0] = m->m[0];
	out->m[1] = m->m[1];
	out->m[2] = m->m[2];
	out->m[3] = m->m[3];
	out->m[4] = m->m[4];
	out->m[5] = m->m[5];
	out->m[6] = m->m[6];
	out->m[7] = m->m[7];
	out->m[8] = m->m[8];
	out->m[9] = m->m[9];
	out->m[10] = m->m[10];
	out->m[11] = m->m[11];
	out->m[12] = m->m[12] + x;
	out->m[13] = m->m[13] + y;
	out->m[14] = m->m[14] + z;
	out->m[15] = m->m[15];
}

RGINLINE void rg_mat4_translated_x(rg_mat4* m, f32 x)
{
	m->m[12] += x;
}

RGINLINE void rg_mat4_translated_y(rg_mat4* m, f32 y)
{
	m->m[13] += y;
}

RGINLINE void rg_mat4_translated_z(rg_mat4* m, f32 z)
{
	m->m[14] += z;
}

RGINLINE void rg_mat4_rotated_x(const rg_mat4* m, f32 angle, rg_mat4* out)
{
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);

	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m13 = m->m[13];
	f32 m14 = m->m[14];

	if (out != m)
	{
		out->m[0] = m->m[0];
		out->m[4] = m->m[4];
		out->m[8] = m->m[8];
		out->m[12] = m->m[12];
	}

	out->m[1] = m1 * c - m2 * s;
	out->m[2] = m1 * s + m2 * c;
	out->m[3] = 0.0f;
	out->m[5] = m5 * c - m6 * s;
	out->m[6] = m5 * s + m6 * c;
	out->m[7] = 0.0f;
	out->m[9] = m9 * c - m10 * s;
	out->m[10] = m9 * s + m10 * c;
	out->m[11] = 0.0f;
	out->m[13] = m13 * c - m14 * s;
	out->m[14] = m13 * s + m14 * c;
	out->m[15] = m->m[15];
}

RGINLINE void rg_mat4_rotated_y(const rg_mat4* m, f32 angle, rg_mat4* out)
{
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);

	f32 m0 = m->m[0];
	f32 m2 = m->m[2];
	f32 m4 = m->m[4];
	f32 m6 = m->m[6];
	f32 m8 = m->m[8];
	f32 m10 = m->m[10];
	f32 m12 = m->m[12];
	f32 m14 = m->m[14];

	if (out != m)
	{
		out->m[1] = m->m[1];
		out->m[5] = m->m[5];
		out->m[9] = m->m[9];
		out->m[13] = m->m[13];
	}

	out->m[0] = m0 * c + m2 * s;
	out->m[2] = m2 * c - m0 * s;
	out->m[3] = 0.0f;
	out->m[4] = m4 * c + m6 * s;
	out->m[6] = m6 * c - m4 * s;
	out->m[7] = 0.0f;
	out->m[8] = m8 * c + m10 * s;
	out->m[10] = m10 * c - m8 * s;
	out->m[11] = 0.0f;
	out->m[12] = m12 * c + m14 * s;
	out->m[14] = m14 * c - m12 * s;
	out->m[15] = m->m[15];
}

RGINLINE void rg_mat4_rotated_z(const rg_mat4* m, f32 angle, rg_mat4* out)
{
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);

	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m12 = m->m[12];
	f32 m13 = m->m[13];

	if (out != m)
	{
		out->m[2] = m->m[2];
		out->m[6] = m->m[6];
		out->m[10] = m->m[10];
		out->m[14] = m->m[14];
	}

	out->m[0] = m0 * c - m1 * s;
	out->m[1] = m0 * s + m1 * c;
	out->m[3] = 0.0f;
	out->m[4] = m4 * c - m5 * s;
	out->m[5] = m4 * s + m5 * c;
	out->m[7] = 0.0f;
	out->m[8] = m8 * c - m9 * s;
	out->m[9] = m8 * s + m9 * c;
	out->m[11] = 0.0f;
	out->m[12] = m12 * c - m13 * s;
	out->m[13] = m12 * s + m13 * c;
	out->m[15] = m->m[15];
}

RGINLINE void rg_mat4_rotated(rg_mat4* m, f32 angle, const rg_vec3* axis)
{
#if RG_MATH_MAX_PERF
	f32 x = axis->x;
	f32 y = axis->y;
	f32 z = axis->z;
#else
	rg_vec3 n = *axis;
	rg_vec3_normalize(axis, &n);
	f32 x = n.x;
	f32 y = n.y;
	f32 z = n.z;
#endif

	if (y == 0.0f && z == 0.0f && (x == 1.0f || x == -1.0f))
	{
		rg_mat4_rotated_x(m, x < 0.0f ? -angle : angle, m);
		return;
	}
	if (x == 0.0f && z == 0.0f && (y == 1.0f || y == -1.0f))
	{
		rg_mat4_rotated_y(m, y < 0.0f ? -angle : angle, m);
		return;
	}
	if (x == 0.0f && y == 0.0f && (z == 1.0f || z == -1.0f))
	{
		rg_mat4_rotated_z(m, z < 0.0f ? -angle : angle, m);
		return;
	}

	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);

	f32 t = 1.0f - c;
	f32 tx = t * x;
	f32 ty = t * y;
	f32 tz = t * z;
	f32 txy = tx * y;
	f32 txz = tx * z;
	f32 tyz = ty * z;
	f32 sx = s * x;
	f32 sy = s * y;
	f32 sz = s * z;

	f32 r00 = tx * x + c;
	f32 r01 = txy + sz;
	f32 r02 = txz - sy;
	f32 r10 = txy - sz;
	f32 r11 = ty * y + c;
	f32 r12 = tyz + sx;
	f32 r20 = txz + sy;
	f32 r21 = tyz - sx;
	f32 r22 = tz * z + c;

	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m12 = m->m[12];
	f32 m13 = m->m[13];
	f32 m14 = m->m[14];
	f32 m15 = m->m[15];

	m->m[0] = r00 * m0 + r10 * m1 + r20 * m2;
	m->m[1] = r01 * m0 + r11 * m1 + r21 * m2;
	m->m[2] = r02 * m0 + r12 * m1 + r22 * m2;
	m->m[3] = 0.0f;

	m->m[4] = r00 * m4 + r10 * m5 + r20 * m6;
	m->m[5] = r01 * m4 + r11 * m5 + r21 * m6;
	m->m[6] = r02 * m4 + r12 * m5 + r22 * m6;
	m->m[7] = 0.0f;

	m->m[8] = r00 * m8 + r10 * m9 + r20 * m10;
	m->m[9] = r01 * m8 + r11 * m9 + r21 * m10;
	m->m[10] = r02 * m8 + r12 * m9 + r22 * m10;
	m->m[11] = 0.0f;

	m->m[12] = r00 * m12 + r10 * m13 + r20 * m14;
	m->m[13] = r01 * m12 + r11 * m13 + r21 * m14;
	m->m[14] = r02 * m12 + r12 * m13 + r22 * m14;
	m->m[15] = m15;
}

RGINLINE void rg_mat4_rotated_at(rg_mat4* m, const rg_vec3* pivot, f32 angle, const rg_vec3* axis)
{
#if RG_MATH_MAX_PERF
	f32 x = axis->x;
	f32 y = axis->y;
	f32 z = axis->z;
#else
	rg_vec3 n = *axis;
	rg_vec3_normalize(axis, &n);
	f32 x = n.x;
	f32 y = n.y;
	f32 z = n.z;
#endif
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);

	f32 t = 1.0f - c;
	f32 tx = t * x;
	f32 ty = t * y;
	f32 tz = t * z;
	f32 txy = tx * y;
	f32 txz = tx * z;
	f32 tyz = ty * z;
	f32 sx = s * x;
	f32 sy = s * y;
	f32 sz = s * z;

	f32 r00 = tx * x + c;
	f32 r01 = txy + sz;
	f32 r02 = txz - sy;
	f32 r10 = txy - sz;
	f32 r11 = ty * y + c;
	f32 r12 = tyz + sx;
	f32 r20 = txz + sy;
	f32 r21 = tyz - sx;
	f32 r22 = tz * z + c;

	f32 px = pivot->x;
	f32 py = pivot->y;
	f32 pz = pivot->z;
	f32 r30 = px - (r00 * px + r10 * py + r20 * pz);
	f32 r31 = py - (r01 * px + r11 * py + r21 * pz);
	f32 r32 = pz - (r02 * px + r12 * py + r22 * pz);

	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m12 = m->m[12];
	f32 m13 = m->m[13];
	f32 m14 = m->m[14];
	f32 m15 = m->m[15];

	m->m[0] = r00 * m0 + r10 * m1 + r20 * m2;
	m->m[1] = r01 * m0 + r11 * m1 + r21 * m2;
	m->m[2] = r02 * m0 + r12 * m1 + r22 * m2;
	m->m[3] = 0.0f;

	m->m[4] = r00 * m4 + r10 * m5 + r20 * m6;
	m->m[5] = r01 * m4 + r11 * m5 + r21 * m6;
	m->m[6] = r02 * m4 + r12 * m5 + r22 * m6;
	m->m[7] = 0.0f;

	m->m[8] = r00 * m8 + r10 * m9 + r20 * m10;
	m->m[9] = r01 * m8 + r11 * m9 + r21 * m10;
	m->m[10] = r02 * m8 + r12 * m9 + r22 * m10;
	m->m[11] = 0.0f;

	m->m[12] = r00 * m12 + r10 * m13 + r20 * m14 + r30 * m15;
	m->m[13] = r01 * m12 + r11 * m13 + r21 * m14 + r31 * m15;
	m->m[14] = r02 * m12 + r12 * m13 + r22 * m14 + r32 * m15;
	m->m[15] = m15;
}

RGINLINE void rg_mat4_spinned(rg_mat4* m, f32 angle, const rg_vec3* axis)
{
#if RG_MATH_MAX_PERF
	f32 x = axis->x;
	f32 y = axis->y;
	f32 z = axis->z;
#else
	rg_vec3 n = *axis;
	rg_vec3_normalize(axis, &n);
	f32 x = n.x;
	f32 y = n.y;
	f32 z = n.z;
#endif
	f32 s;
	f32 c;
	rg_sincosf(angle, &s, &c);

	f32 t = 1.0f - c;
	f32 tx = t * x;
	f32 ty = t * y;
	f32 tz = t * z;
	f32 txy = tx * y;
	f32 txz = tx * z;
	f32 tyz = ty * z;
	f32 sx = s * x;
	f32 sy = s * y;
	f32 sz = s * z;

	f32 r00 = tx * x + c;
	f32 r01 = txy + sz;
	f32 r02 = txz - sy;
	f32 r10 = txy - sz;
	f32 r11 = ty * y + c;
	f32 r12 = tyz + sx;
	f32 r20 = txz + sy;
	f32 r21 = tyz - sx;
	f32 r22 = tz * z + c;

	f32 px = m->m[12];
	f32 py = m->m[13];
	f32 pz = m->m[14];
	f32 r30 = px - (r00 * px + r10 * py + r20 * pz);
	f32 r31 = py - (r01 * px + r11 * py + r21 * pz);
	f32 r32 = pz - (r02 * px + r12 * py + r22 * pz);

	f32 m0 = m->m[0];
	f32 m1 = m->m[1];
	f32 m2 = m->m[2];
	f32 m4 = m->m[4];
	f32 m5 = m->m[5];
	f32 m6 = m->m[6];
	f32 m8 = m->m[8];
	f32 m9 = m->m[9];
	f32 m10 = m->m[10];
	f32 m12 = m->m[12];
	f32 m13 = m->m[13];
	f32 m14 = m->m[14];
	f32 m15 = m->m[15];

	m->m[0] = r00 * m0 + r10 * m1 + r20 * m2;
	m->m[1] = r01 * m0 + r11 * m1 + r21 * m2;
	m->m[2] = r02 * m0 + r12 * m1 + r22 * m2;
	m->m[3] = 0.0f;

	m->m[4] = r00 * m4 + r10 * m5 + r20 * m6;
	m->m[5] = r01 * m4 + r11 * m5 + r21 * m6;
	m->m[6] = r02 * m4 + r12 * m5 + r22 * m6;
	m->m[7] = 0.0f;

	m->m[8] = r00 * m8 + r10 * m9 + r20 * m10;
	m->m[9] = r01 * m8 + r11 * m9 + r21 * m10;
	m->m[10] = r02 * m8 + r12 * m9 + r22 * m10;
	m->m[11] = 0.0f;

	m->m[12] = r00 * m12 + r10 * m13 + r20 * m14 + r30 * m15;
	m->m[13] = r01 * m12 + r11 * m13 + r21 * m14 + r31 * m15;
	m->m[14] = r02 * m12 + r12 * m13 + r22 * m14 + r32 * m15;
	m->m[15] = m15;
}

RGINLINE void rg_mat4_decompose_scale(const rg_mat4* m, rg_vec3* s)
{
	f32 c0x = m->m[0];
	f32 c0y = m->m[1];
	f32 c0z = m->m[2];
	f32 c1x = m->m[4];
	f32 c1y = m->m[5];
	f32 c1z = m->m[6];
	f32 c2x = m->m[8];
	f32 c2y = m->m[9];
	f32 c2z = m->m[10];

	f32 sx2 = c0x * c0x + c0y * c0y + c0z * c0z;
	f32 sy2 = c1x * c1x + c1y * c1y + c1z * c1z;
	f32 sz2 = c2x * c2x + c2y * c2y + c2z * c2z;
#ifdef RG_MATH_SSE
	RG_VEC3_STORE(s, _mm_sqrt_ps(_mm_setr_ps(sx2, sy2, sz2, 0.0f)));
#else
	s->x = rg_sqrtf(sx2);
	s->y = rg_sqrtf(sy2);
	s->z = rg_sqrtf(sz2);
#if !RG_MATH_MAX_PERF
	s->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_mat4_decompose_scale_fast(const rg_mat4* m, rg_vec3* s)
{
	f32 c0x = m->m[0];
	f32 c0y = m->m[1];
	f32 c0z = m->m[2];
	f32 c1x = m->m[4];
	f32 c1y = m->m[5];
	f32 c1z = m->m[6];
	f32 c2x = m->m[8];
	f32 c2y = m->m[9];
	f32 c2z = m->m[10];

	f32 sx2 = c0x * c0x + c0y * c0y + c0z * c0z;
	f32 sy2 = c1x * c1x + c1y * c1y + c1z * c1z;
	f32 sz2 = c2x * c2x + c2y * c2y + c2z * c2z;

	s->x = sx2 * rg_rsqrtf(sx2);
	s->y = sy2 * rg_rsqrtf(sy2);
	s->z = sz2 * rg_rsqrtf(sz2);
#if !RG_MATH_MAX_PERF
	s->_pad = 0.0f;
#endif
}

RGINLINE int rg_mat4_uniscaled(const rg_mat4* m)
{
	f32 c0x = m->m[0];
	f32 c0y = m->m[1];
	f32 c0z = m->m[2];
	f32 c1x = m->m[4];
	f32 c1y = m->m[5];
	f32 c1z = m->m[6];
	f32 c2x = m->m[8];
	f32 c2y = m->m[9];
	f32 c2z = m->m[10];

	f32 sx2 = c0x * c0x + c0y * c0y + c0z * c0z;
	f32 sy2 = c1x * c1x + c1y * c1y + c1z * c1z;
	f32 sz2 = c2x * c2x + c2y * c2y + c2z * c2z;
#if RG_MATH_MAX_PERF
	if (sy2 == sx2 && sz2 == sx2)
	{
		return 1;
	}

	f32 sx = rg_sqrtf(sx2);
	f32 sy = rg_sqrtf(sy2);
	f32 sz = rg_sqrtf(sz2);
	return rg_absf(sy - sx) <= RG_EPSILON && rg_absf(sz - sx) <= RG_EPSILON;
#else
	if (sy2 == sx2 && sz2 == sx2)
	{
		return 1;
	}

	f32 sx = rg_sqrtf(sx2);
	f32 sy = rg_sqrtf(sy2);
	f32 sz = rg_sqrtf(sz2);
	return rg_absf(sy - sx) <= RG_EPSILON && rg_absf(sz - sx) <= RG_EPSILON;
#endif
}

RGINLINE void rg_mat4_decompose_rs(const rg_mat4* m, rg_mat4* r, rg_vec3* s)
{
	f32 c0x = m->m[0];
	f32 c0y = m->m[1];
	f32 c0z = m->m[2];
	f32 c1x = m->m[4];
	f32 c1y = m->m[5];
	f32 c1z = m->m[6];
	f32 c2x = m->m[8];
	f32 c2y = m->m[9];
	f32 c2z = m->m[10];

	f32 sx2 = c0x * c0x + c0y * c0y + c0z * c0z;
	f32 sy2 = c1x * c1x + c1y * c1y + c1z * c1z;
	f32 sz2 = c2x * c2x + c2y * c2y + c2z * c2z;
#if defined(RG_MATH_SSE) && !RG_MATH_MAX_PERF
	__m128 scales = _mm_sqrt_ps(_mm_setr_ps(sx2, sy2, sz2, 1.0f));
	f32 sx = _mm_cvtss_f32(scales);
	f32 sy = _mm_cvtss_f32(_mm_shuffle_ps(scales, scales, _MM_SHUFFLE(1, 1, 1, 1)));
	f32 sz = _mm_cvtss_f32(_mm_shuffle_ps(scales, scales, _MM_SHUFFLE(2, 2, 2, 2)));
#else
	f32 sx = rg_sqrtf(sx2);
	f32 sy = rg_sqrtf(sy2);
	f32 sz = rg_sqrtf(sz2);
#endif

	f32 cx = c0y * c1z - c0z * c1y;
	f32 cy = c0z * c1x - c0x * c1z;
	f32 cz = c0x * c1y - c0y * c1x;
	f32 det = cx * c2x + cy * c2y + cz * c2z;

	if (det < 0.0f)
	{
		sx = -sx;
		sy = -sy;
		sz = -sz;
	}

	s->x = sx;
	s->y = sy;
	s->z = sz;
#if !RG_MATH_MAX_PERF
	s->_pad = 0.0f;
	if (rg_absf(sx) < RG_EPSILON || rg_absf(sy) < RG_EPSILON || rg_absf(sz) < RG_EPSILON)
	{
		rg_mat4_identity(r);
		return;
	}
#endif

	f32 inv_sx = 1.0f / sx;
	f32 inv_sy = 1.0f / sy;
	f32 inv_sz = 1.0f / sz;

	r->m[0] = c0x * inv_sx;
	r->m[1] = c0y * inv_sx;
	r->m[2] = c0z * inv_sx;
	r->m[3] = 0.0f;

	r->m[4] = c1x * inv_sy;
	r->m[5] = c1y * inv_sy;
	r->m[6] = c1z * inv_sy;
	r->m[7] = 0.0f;

	r->m[8] = c2x * inv_sz;
	r->m[9] = c2y * inv_sz;
	r->m[10] = c2z * inv_sz;
	r->m[11] = 0.0f;

	r->m[12] = 0.0f;
	r->m[13] = 0.0f;
	r->m[14] = 0.0f;
	r->m[15] = 1.0f;
}

RGINLINE void rg_mat4_decompose_trs(const rg_mat4* m, rg_vec3* t, rg_mat4* r, rg_vec3* s)
{
	t->x = m->m[12];
	t->y = m->m[13];
	t->z = m->m[14];
#if !RG_MATH_MAX_PERF
	t->_pad = 0.0f;
#endif

	rg_mat4_decompose_rs(m, r, s);
}

RGINLINE void rg_mat4_decompose(const rg_mat4* m, rg_vec3* t, rg_quat* r, rg_vec3* s)
{
	f32 c0x = m->m[0];
	f32 c0y = m->m[1];
	f32 c0z = m->m[2];
	f32 c1x = m->m[4];
	f32 c1y = m->m[5];
	f32 c1z = m->m[6];
	f32 c2x = m->m[8];
	f32 c2y = m->m[9];
	f32 c2z = m->m[10];

	t->x = m->m[12];
	t->y = m->m[13];
	t->z = m->m[14];
#if !RG_MATH_MAX_PERF
	t->_pad = 0.0f;
#endif

	f32 sx2 = c0x * c0x + c0y * c0y + c0z * c0z;
	f32 sy2 = c1x * c1x + c1y * c1y + c1z * c1z;
	f32 sz2 = c2x * c2x + c2y * c2y + c2z * c2z;
#if defined(RG_MATH_SSE) && !RG_MATH_MAX_PERF
	__m128 scales = _mm_sqrt_ps(_mm_setr_ps(sx2, sy2, sz2, 1.0f));
	f32 sx = _mm_cvtss_f32(scales);
	f32 sy = _mm_cvtss_f32(_mm_shuffle_ps(scales, scales, _MM_SHUFFLE(1, 1, 1, 1)));
	f32 sz = _mm_cvtss_f32(_mm_shuffle_ps(scales, scales, _MM_SHUFFLE(2, 2, 2, 2)));
#else
	f32 sx = rg_sqrtf(sx2);
	f32 sy = rg_sqrtf(sy2);
	f32 sz = rg_sqrtf(sz2);
#endif

	f32 cx = c0y * c1z - c0z * c1y;
	f32 cy = c0z * c1x - c0x * c1z;
	f32 cz = c0x * c1y - c0y * c1x;
	f32 det = cx * c2x + cy * c2y + cz * c2z;

	if (det < 0.0f)
	{
		sx = -sx;
		sy = -sy;
		sz = -sz;
	}

#if !RG_MATH_MAX_PERF
	s->x = sx;
	s->y = sy;
	s->z = sz;
	if (rg_absf(sx) < RG_EPSILON || rg_absf(sy) < RG_EPSILON || rg_absf(sz) < RG_EPSILON)
	{
		r->x = 0.0f;
		r->y = 0.0f;
		r->z = 0.0f;
		r->w = 1.0f;
		return;
	}
#endif

	f32 inv_sx = 1.0f / sx;
	f32 inv_sy = 1.0f / sy;
	f32 inv_sz = 1.0f / sz;

	f32 m00 = c0x * inv_sx;
	f32 m10 = c0y * inv_sx;
	f32 m20 = c0z * inv_sx;
	f32 m01 = c1x * inv_sy;
	f32 m11 = c1y * inv_sy;
	f32 m21 = c1z * inv_sy;
	f32 m02 = c2x * inv_sz;
	f32 m12 = c2y * inv_sz;
	f32 m22 = c2z * inv_sz;

	f32 qt;
	f32 qx;
	f32 qy;
	f32 qz;
	f32 qw;
	if (m22 < 0.0f)
	{
		if (m00 > m11)
		{
			qt = 1.0f + m00 - m11 - m22;
			qx = qt;
			qy = m01 + m10;
			qz = m02 + m20;
			qw = m21 - m12;
		}
		else
		{
			qt = 1.0f - m00 + m11 - m22;
			qx = m01 + m10;
			qy = qt;
			qz = m12 + m21;
			qw = m20 - m02;
		}
	}
	else
	{
		if (m00 < -m11)
		{
			qt = 1.0f - m00 - m11 + m22;
			qx = m02 + m20;
			qy = m12 + m21;
			qz = qt;
			qw = m10 - m01;
		}
		else
		{
			qt = 1.0f + m00 + m11 + m22;
			qx = m21 - m12;
			qy = m20 - m02;
			qz = m10 - m01;
			qw = qt;
		}
	}
	f32 rinv = 0.5f / rg_sqrtf(qt);
	r->x = qx * rinv;
	r->y = qy * rinv;
	r->z = qz * rinv;
	r->w = qw * rinv;

	s->x = sx;
	s->y = sy;
	s->z = sz;
}

RGINLINE void rg_mat4_decompose_fast(const rg_mat4* m, rg_vec3* t, rg_quat* r, rg_vec3* s)
{
	f32 c0x = m->m[0];
	f32 c0y = m->m[1];
	f32 c0z = m->m[2];
	f32 c1x = m->m[4];
	f32 c1y = m->m[5];
	f32 c1z = m->m[6];
	f32 c2x = m->m[8];
	f32 c2y = m->m[9];
	f32 c2z = m->m[10];

	t->x = m->m[12];
	t->y = m->m[13];
	t->z = m->m[14];
#if !RG_MATH_MAX_PERF
	t->_pad = 0.0f;
#endif

	f32 sx2 = c0x * c0x + c0y * c0y + c0z * c0z;
	f32 sy2 = c1x * c1x + c1y * c1y + c1z * c1z;
	f32 sz2 = c2x * c2x + c2y * c2y + c2z * c2z;
	f32 inv_sx = rg_rsqrtf(sx2);
	f32 inv_sy = rg_rsqrtf(sy2);
	f32 inv_sz = rg_rsqrtf(sz2);
	f32 sx = sx2 * inv_sx;
	f32 sy = sy2 * inv_sy;
	f32 sz = sz2 * inv_sz;

	f32 cx = c0y * c1z - c0z * c1y;
	f32 cy = c0z * c1x - c0x * c1z;
	f32 cz = c0x * c1y - c0y * c1x;
	f32 det = cx * c2x + cy * c2y + cz * c2z;

	if (det < 0.0f)
	{
		sx = -sx;
		sy = -sy;
		sz = -sz;
		inv_sx = -inv_sx;
		inv_sy = -inv_sy;
		inv_sz = -inv_sz;
	}

	f32 m00 = c0x * inv_sx;
	f32 m10 = c0y * inv_sx;
	f32 m20 = c0z * inv_sx;
	f32 m01 = c1x * inv_sy;
	f32 m11 = c1y * inv_sy;
	f32 m21 = c1z * inv_sy;
	f32 m02 = c2x * inv_sz;
	f32 m12 = c2y * inv_sz;
	f32 m22 = c2z * inv_sz;

	f32 qt;
	f32 qx;
	f32 qy;
	f32 qz;
	f32 qw;
	if (m22 < 0.0f)
	{
		if (m00 > m11)
		{
			qt = 1.0f + m00 - m11 - m22;
			qx = qt;
			qy = m01 + m10;
			qz = m02 + m20;
			qw = m21 - m12;
		}
		else
		{
			qt = 1.0f - m00 + m11 - m22;
			qx = m01 + m10;
			qy = qt;
			qz = m12 + m21;
			qw = m20 - m02;
		}
	}
	else
	{
		if (m00 < -m11)
		{
			qt = 1.0f - m00 - m11 + m22;
			qx = m02 + m20;
			qy = m12 + m21;
			qz = qt;
			qw = m10 - m01;
		}
		else
		{
			qt = 1.0f + m00 + m11 + m22;
			qx = m21 - m12;
			qy = m20 - m02;
			qz = m10 - m01;
			qw = qt;
		}
	}
	f32 rinv = 0.5f * rg_rsqrtf(qt);
	r->x = qx * rinv;
	r->y = qy * rinv;
	r->z = qz * rinv;
	r->w = qw * rinv;

	s->x = sx;
	s->y = sy;
	s->z = sz;
#if !RG_MATH_MAX_PERF
	s->_pad = 0.0f;
#endif
}

RGINLINE void rg_quat_from_mat4(const rg_mat4* m, rg_quat* out)
{
#if RG_MATH_MAX_PERF
	f32 fast_trace = m->m[0] + m->m[5] + m->m[10];
	if (fast_trace >= 0.0f)
	{
		f32 qt = 1.0f + fast_trace;
		f32 rinv = 0.5f / rg_sqrtf(qt);
		out->x = rinv * (m->m[6] - m->m[9]);
		out->y = rinv * (m->m[2] - m->m[8]);
		out->z = rinv * (m->m[1] - m->m[4]);
		out->w = qt * rinv;
		return;
	}
#endif

	f32 m00 = m->m[0];
	f32 m10 = m->m[1];
	f32 m20 = m->m[2];
	f32 m01 = m->m[4];
	f32 m11 = m->m[5];
	f32 m21 = m->m[6];
	f32 m02 = m->m[8];
	f32 m12 = m->m[9];
	f32 m22 = m->m[10];

#if !RG_MATH_MAX_PERF
	f32 sx = rg_sqrtf(m00 * m00 + m10 * m10 + m20 * m20);
	f32 sy = rg_sqrtf(m01 * m01 + m11 * m11 + m21 * m21);
	f32 sz = rg_sqrtf(m02 * m02 + m12 * m12 + m22 * m22);
	if (sx < RG_EPSILON || sy < RG_EPSILON || sz < RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->w = 1.0f;
		return;
	}
	f32 inv_sx = 1.0f / sx;
	f32 inv_sy = 1.0f / sy;
	f32 inv_sz = 1.0f / sz;
	m00 *= inv_sx;
	m10 *= inv_sx;
	m20 *= inv_sx;
	m01 *= inv_sy;
	m11 *= inv_sy;
	m21 *= inv_sy;
	m02 *= inv_sz;
	m12 *= inv_sz;
	m22 *= inv_sz;
#endif

	f32 trace = m00 + m11 + m22;
	if (trace >= 0.0f)
	{
#if RG_MATH_MAX_PERF
		f32 qt = 1.0f + trace;
		f32 rinv = 0.5f / rg_sqrtf(qt);
#else
		f32 rt = rg_sqrtf(1.0f + trace);
		f32 rinv = 0.5f / rt;
#endif
		out->x = rinv * (m21 - m12);
		out->y = rinv * (m20 - m02);
		out->z = rinv * (m10 - m01);
#if RG_MATH_MAX_PERF
		out->w = qt * rinv;
#else
		out->w = rt * 0.5f;
#endif
	}
	else if (m00 >= m11 && m00 >= m22)
	{
#if RG_MATH_MAX_PERF
		f32 qt = 1.0f - m11 - m22 + m00;
		f32 rinv = 0.5f / rg_sqrtf(qt);
#else
		f32 rt = rg_sqrtf(1.0f - m11 - m22 + m00);
		f32 rinv = 0.5f / rt;
#endif
#if RG_MATH_MAX_PERF
		out->x = qt * rinv;
#else
		out->x = rt * 0.5f;
#endif
		out->y = rinv * (m01 + m10);
		out->z = rinv * (m02 + m20);
		out->w = rinv * (m21 - m12);
	}
	else if (m11 >= m22)
	{
#if RG_MATH_MAX_PERF
		f32 qt = 1.0f - m00 - m22 + m11;
		f32 rinv = 0.5f / rg_sqrtf(qt);
#else
		f32 rt = rg_sqrtf(1.0f - m00 - m22 + m11);
		f32 rinv = 0.5f / rt;
#endif
		out->x = rinv * (m01 + m10);
#if RG_MATH_MAX_PERF
		out->y = qt * rinv;
#else
		out->y = rt * 0.5f;
#endif
		out->z = rinv * (m12 + m21);
		out->w = rinv * (m20 - m02);
	}
	else
	{
#if RG_MATH_MAX_PERF
		f32 qt = 1.0f - m00 - m11 + m22;
		f32 rinv = 0.5f / rg_sqrtf(qt);
#else
		f32 rt = rg_sqrtf(1.0f - m00 - m11 + m22);
		f32 rinv = 0.5f / rt;
#endif
		out->x = rinv * (m02 + m20);
		out->y = rinv * (m12 + m21);
#if RG_MATH_MAX_PERF
		out->z = qt * rinv;
#else
		out->z = rt * 0.5f;
#endif
		out->w = rinv * (m10 - m01);
	}
}

RGINLINE void rg_mat4_quat(const rg_mat4* m, rg_quat* out)
{
	rg_quat_from_mat4(m, out);
}

#if defined(RG_MATH_AVX) && RG_MATH_MAX_PERF
RGINLINE void rg__mat4_mul_avx_raw(const f32* a, const f32* b, f32* out, int a_aligned, int b_aligned, int out_aligned)
{
	__m256 b01 = b_aligned ? _mm256_load_ps(&b[0]) : _mm256_loadu_ps(&b[0]);
	__m256 b23 = b_aligned ? _mm256_load_ps(&b[8]) : _mm256_loadu_ps(&b[8]);
	__m256 a01 = a_aligned ? _mm256_load_ps(&a[0]) : _mm256_loadu_ps(&a[0]);
	__m256 a23 = a_aligned ? _mm256_load_ps(&a[8]) : _mm256_loadu_ps(&a[8]);

	__m256 a10 = _mm256_permute2f128_ps(a01, a01, 0x03);
	__m256 a32 = _mm256_permute2f128_ps(a23, a23, 0x03);

	__m256i ix0 = _mm256_set_epi32(1, 1, 1, 1, 0, 0, 0, 0);
	__m256i ix1 = _mm256_set_epi32(3, 3, 3, 3, 2, 2, 2, 2);
	__m256i ix2 = _mm256_set_epi32(0, 0, 0, 0, 1, 1, 1, 1);
	__m256i ix3 = _mm256_set_epi32(2, 2, 2, 2, 3, 3, 3, 3);

	__m256 b01_x = _mm256_permutevar_ps(b01, ix0);
	__m256 b01_y = _mm256_permutevar_ps(b01, ix1);
	__m256 b01_z = _mm256_permutevar_ps(b01, ix2);
	__m256 b01_w = _mm256_permutevar_ps(b01, ix3);
	__m256 b23_x = _mm256_permutevar_ps(b23, ix0);
	__m256 b23_y = _mm256_permutevar_ps(b23, ix1);
	__m256 b23_z = _mm256_permutevar_ps(b23, ix2);
	__m256 b23_w = _mm256_permutevar_ps(b23, ix3);

	__m256 r01 = _mm256_mul_ps(a01, b01_x);
	__m256 r23 = _mm256_mul_ps(a01, b23_x);
#if defined(RG_MATH_FMA)
	r01 = _mm256_fmadd_ps(a23, b01_y, r01);
	r23 = _mm256_fmadd_ps(a23, b23_y, r23);
	r01 = _mm256_fmadd_ps(a10, b01_z, r01);
	r23 = _mm256_fmadd_ps(a10, b23_z, r23);
	r01 = _mm256_fmadd_ps(a32, b01_w, r01);
	r23 = _mm256_fmadd_ps(a32, b23_w, r23);
#else
	r01 = _mm256_add_ps(_mm256_mul_ps(a23, b01_y), r01);
	r23 = _mm256_add_ps(_mm256_mul_ps(a23, b23_y), r23);
	r01 = _mm256_add_ps(_mm256_mul_ps(a10, b01_z), r01);
	r23 = _mm256_add_ps(_mm256_mul_ps(a10, b23_z), r23);
	r01 = _mm256_add_ps(_mm256_mul_ps(a32, b01_w), r01);
	r23 = _mm256_add_ps(_mm256_mul_ps(a32, b23_w), r23);
#endif

	if (out_aligned)
	{
		_mm256_store_ps(&out[0], r01);
		_mm256_store_ps(&out[8], r23);
	}
	else
	{
		_mm256_storeu_ps(&out[0], r01);
		_mm256_storeu_ps(&out[8], r23);
	}
}

RGINLINE void rg__mat4_mul_avx_rawu(const f32* a, const f32* b, f32* out)
{
	__m256 b01 = _mm256_loadu_ps(&b[0]);
	__m256 b23 = _mm256_loadu_ps(&b[8]);
	__m256 a01 = _mm256_loadu_ps(&a[0]);
	__m256 a23 = _mm256_loadu_ps(&a[8]);

	__m256 a10 = _mm256_permute2f128_ps(a01, a01, 0x03);
	__m256 a32 = _mm256_permute2f128_ps(a23, a23, 0x03);

	__m256i ix0 = _mm256_set_epi32(1, 1, 1, 1, 0, 0, 0, 0);
	__m256i ix1 = _mm256_set_epi32(3, 3, 3, 3, 2, 2, 2, 2);
	__m256i ix2 = _mm256_set_epi32(0, 0, 0, 0, 1, 1, 1, 1);
	__m256i ix3 = _mm256_set_epi32(2, 2, 2, 2, 3, 3, 3, 3);

	__m256 b01_x = _mm256_permutevar_ps(b01, ix0);
	__m256 b01_y = _mm256_permutevar_ps(b01, ix1);
	__m256 b01_z = _mm256_permutevar_ps(b01, ix2);
	__m256 b01_w = _mm256_permutevar_ps(b01, ix3);
	__m256 b23_x = _mm256_permutevar_ps(b23, ix0);
	__m256 b23_y = _mm256_permutevar_ps(b23, ix1);
	__m256 b23_z = _mm256_permutevar_ps(b23, ix2);
	__m256 b23_w = _mm256_permutevar_ps(b23, ix3);

	__m256 r01 = _mm256_mul_ps(a01, b01_x);
	__m256 r23 = _mm256_mul_ps(a01, b23_x);
#if defined(RG_MATH_FMA)
	r01 = _mm256_fmadd_ps(a23, b01_y, r01);
	r23 = _mm256_fmadd_ps(a23, b23_y, r23);
	r01 = _mm256_fmadd_ps(a10, b01_z, r01);
	r23 = _mm256_fmadd_ps(a10, b23_z, r23);
	r01 = _mm256_fmadd_ps(a32, b01_w, r01);
	r23 = _mm256_fmadd_ps(a32, b23_w, r23);
#else
	r01 = _mm256_add_ps(_mm256_mul_ps(a23, b01_y), r01);
	r23 = _mm256_add_ps(_mm256_mul_ps(a23, b23_y), r23);
	r01 = _mm256_add_ps(_mm256_mul_ps(a10, b01_z), r01);
	r23 = _mm256_add_ps(_mm256_mul_ps(a10, b23_z), r23);
	r01 = _mm256_add_ps(_mm256_mul_ps(a32, b01_w), r01);
	r23 = _mm256_add_ps(_mm256_mul_ps(a32, b23_w), r23);
#endif

	_mm256_storeu_ps(&out[0], r01);
	_mm256_storeu_ps(&out[8], r23);
}
#endif

RGINLINE void rg_mat4_mul(const rg_mat4* a, const rg_mat4* b, rg_mat4* out)
{
#if defined(RG_MATH_AVX) && RG_MATH_MAX_PERF && !defined(_MSC_VER)
	__m256 b01 = _mm256_loadu_ps(&b->m[0]);
	__m256 b23 = _mm256_loadu_ps(&b->m[8]);
	__m256 a01 = _mm256_loadu_ps(&a->m[0]);
	__m256 a23 = _mm256_loadu_ps(&a->m[8]);

	__m256 a10 = _mm256_permute2f128_ps(a01, a01, 0x03);
	__m256 a32 = _mm256_permute2f128_ps(a23, a23, 0x03);

	__m256i ix0 = _mm256_set_epi32(1, 1, 1, 1, 0, 0, 0, 0);
	__m256i ix1 = _mm256_set_epi32(3, 3, 3, 3, 2, 2, 2, 2);
	__m256i ix2 = _mm256_set_epi32(0, 0, 0, 0, 1, 1, 1, 1);
	__m256i ix3 = _mm256_set_epi32(2, 2, 2, 2, 3, 3, 3, 3);

	__m256 b01_x = _mm256_permutevar_ps(b01, ix0);
	__m256 b01_y = _mm256_permutevar_ps(b01, ix1);
	__m256 b01_z = _mm256_permutevar_ps(b01, ix2);
	__m256 b01_w = _mm256_permutevar_ps(b01, ix3);
	__m256 b23_x = _mm256_permutevar_ps(b23, ix0);
	__m256 b23_y = _mm256_permutevar_ps(b23, ix1);
	__m256 b23_z = _mm256_permutevar_ps(b23, ix2);
	__m256 b23_w = _mm256_permutevar_ps(b23, ix3);

	__m256 r01 = _mm256_mul_ps(a01, b01_x);
	__m256 r23 = _mm256_mul_ps(a01, b23_x);
#if defined(RG_MATH_FMA)
	r01 = _mm256_fmadd_ps(a23, b01_y, r01);
	r23 = _mm256_fmadd_ps(a23, b23_y, r23);
	r01 = _mm256_fmadd_ps(a10, b01_z, r01);
	r23 = _mm256_fmadd_ps(a10, b23_z, r23);
	r01 = _mm256_fmadd_ps(a32, b01_w, r01);
	r23 = _mm256_fmadd_ps(a32, b23_w, r23);
#else
	r01 = _mm256_add_ps(_mm256_mul_ps(a23, b01_y), r01);
	r23 = _mm256_add_ps(_mm256_mul_ps(a23, b23_y), r23);
	r01 = _mm256_add_ps(_mm256_mul_ps(a10, b01_z), r01);
	r23 = _mm256_add_ps(_mm256_mul_ps(a10, b23_z), r23);
	r01 = _mm256_add_ps(_mm256_mul_ps(a32, b01_w), r01);
	r23 = _mm256_add_ps(_mm256_mul_ps(a32, b23_w), r23);
#endif

	_mm256_storeu_ps(&out->m[0], r01);
	_mm256_storeu_ps(&out->m[8], r23);
#elif defined(RG_MATH_SSE)
	__m128 a0 = _mm_load_ps(&a->m[0]);
	__m128 a1 = _mm_load_ps(&a->m[4]);
	__m128 a2 = _mm_load_ps(&a->m[8]);
	__m128 a3 = _mm_load_ps(&a->m[12]);

	__m128 b0 = _mm_load_ps(&b->m[0]);
	__m128 b1 = _mm_load_ps(&b->m[4]);
	__m128 b2 = _mm_load_ps(&b->m[8]);
	__m128 b3 = _mm_load_ps(&b->m[12]);

	__m128 r0 = _mm_mul_ps(a0, _mm_shuffle_ps(b0, b0, _MM_SHUFFLE(0, 0, 0, 0)));
	r0 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b0, b0, _MM_SHUFFLE(1, 1, 1, 1)), r0);
	r0 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b0, b0, _MM_SHUFFLE(2, 2, 2, 2)), r0);
	r0 = RG_MATH_FMADD_PS(a3, _mm_shuffle_ps(b0, b0, _MM_SHUFFLE(3, 3, 3, 3)), r0);

	__m128 r1 = _mm_mul_ps(a0, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(0, 0, 0, 0)));
	r1 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(1, 1, 1, 1)), r1);
	r1 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(2, 2, 2, 2)), r1);
	r1 = RG_MATH_FMADD_PS(a3, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(3, 3, 3, 3)), r1);

	__m128 r2 = _mm_mul_ps(a0, _mm_shuffle_ps(b2, b2, _MM_SHUFFLE(0, 0, 0, 0)));
	r2 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b2, b2, _MM_SHUFFLE(1, 1, 1, 1)), r2);
	r2 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b2, b2, _MM_SHUFFLE(2, 2, 2, 2)), r2);
	r2 = RG_MATH_FMADD_PS(a3, _mm_shuffle_ps(b2, b2, _MM_SHUFFLE(3, 3, 3, 3)), r2);

	__m128 r3 = _mm_mul_ps(a0, _mm_shuffle_ps(b3, b3, _MM_SHUFFLE(0, 0, 0, 0)));
	r3 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b3, b3, _MM_SHUFFLE(1, 1, 1, 1)), r3);
	r3 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b3, b3, _MM_SHUFFLE(2, 2, 2, 2)), r3);
	r3 = RG_MATH_FMADD_PS(a3, _mm_shuffle_ps(b3, b3, _MM_SHUFFLE(3, 3, 3, 3)), r3);

	_mm_store_ps(&out->m[0], r0);
	_mm_store_ps(&out->m[4], r1);
	_mm_store_ps(&out->m[8], r2);
	_mm_store_ps(&out->m[12], r3);
#else
	f32 r[16];
	const f32* am = a->m;
	const f32* bm = b->m;

	for (int col = 0; col < 4; col++)
	{
		f32 b0 = bm[col * 4 + 0];
		f32 b1 = bm[col * 4 + 1];
		f32 b2 = bm[col * 4 + 2];
		f32 b3 = bm[col * 4 + 3];

		r[col * 4 + 0] = am[0] * b0 + am[4] * b1 + am[8] * b2 + am[12] * b3;
		r[col * 4 + 1] = am[1] * b0 + am[5] * b1 + am[9] * b2 + am[13] * b3;
		r[col * 4 + 2] = am[2] * b0 + am[6] * b1 + am[10] * b2 + am[14] * b3;
		r[col * 4 + 3] = am[3] * b0 + am[7] * b1 + am[11] * b2 + am[15] * b3;
	}

	for (int i = 0; i < 16; i++)
	{
		out->m[i] = r[i];
	}
#endif
}

RGINLINE void rg_mat4_muln(const rg_mat4* const* matrices, int len, rg_mat4* out)
{
	RG_MATH_ASSERT(len > 1);
	const rg_mat4* m0 = matrices[0];
	const rg_mat4* m1 = matrices[1];
	if (len == 4 &&
	    out != m0 &&
	    out != m1 &&
	    out != matrices[2] &&
	    out != matrices[3])
	{
		rg_mat4_mul4(m0, m1, matrices[2], matrices[3], out);
		return;
	}
	rg_mat4_mul(m0, m1, out);
	for (int i = 2; i < len; i++)
	{
		rg_mat4_mul(out, matrices[i], out);
	}
}

RGINLINE void rg_mat4_mul4(const rg_mat4* a, const rg_mat4* b, const rg_mat4* c, const rg_mat4* d, rg_mat4* out)
{
#if defined(RG_MATH_AVX) && RG_MATH_MAX_PERF
	RG_ALIGN32 f32 tmp0[16];
	RG_ALIGN32 f32 tmp1[16];
	rg__mat4_mul_avx_raw(a->m, b->m, tmp0, 0, 0, 1);
	rg__mat4_mul_avx_raw(tmp0, c->m, tmp1, 1, 0, 1);
	rg__mat4_mul_avx_raw(tmp1, d->m, out->m, 1, 0, 0);
#else
	rg_mat4 tmp0;
	rg_mat4 tmp1;
	rg_mat4_mul(a, b, &tmp0);
	rg_mat4_mul(&tmp0, c, &tmp1);
	rg_mat4_mul(&tmp1, d, out);
#endif
}

RGINLINE void rg_mat4_mul4_fast(const rg_mat4* a, const rg_mat4* b, const rg_mat4* c, const rg_mat4* d, rg_mat4* out)
{
#if defined(RG_MATH_AVX) && RG_MATH_MAX_PERF
	RG_MATH_ASSERT((((uintptr_t)a->m) & 31u) == 0u);
	RG_MATH_ASSERT((((uintptr_t)b->m) & 31u) == 0u);
	RG_MATH_ASSERT((((uintptr_t)c->m) & 31u) == 0u);
	RG_MATH_ASSERT((((uintptr_t)d->m) & 31u) == 0u);
	RG_MATH_ASSERT((((uintptr_t)out->m) & 31u) == 0u);
	RG_ALIGN32 f32 tmp0[16];
	RG_ALIGN32 f32 tmp1[16];
	rg__mat4_mul_avx_raw(a->m, b->m, tmp0, 1, 1, 1);
	rg__mat4_mul_avx_raw(tmp0, c->m, tmp1, 1, 1, 1);
	rg__mat4_mul_avx_raw(tmp1, d->m, out->m, 1, 1, 1);
#else
	rg_mat4_mul4(a, b, c, d, out);
#endif
}

RGINLINE void rg_mat4_mul_affine(const rg_mat4* a, const rg_mat4* b, rg_mat4* out)
{
	f32 a00 = a->m[0];
	f32 a01 = a->m[1];
	f32 a02 = a->m[2];
	f32 a03 = a->m[3];
	f32 a10 = a->m[4];
	f32 a11 = a->m[5];
	f32 a12 = a->m[6];
	f32 a13 = a->m[7];
	f32 a20 = a->m[8];
	f32 a21 = a->m[9];
	f32 a22 = a->m[10];
	f32 a23 = a->m[11];
	f32 a30 = a->m[12];
	f32 a31 = a->m[13];
	f32 a32 = a->m[14];
	f32 a33 = a->m[15];

	f32 b00 = b->m[0];
	f32 b01 = b->m[1];
	f32 b02 = b->m[2];
	f32 b10 = b->m[4];
	f32 b11 = b->m[5];
	f32 b12 = b->m[6];
	f32 b20 = b->m[8];
	f32 b21 = b->m[9];
	f32 b22 = b->m[10];
	f32 b30 = b->m[12];
	f32 b31 = b->m[13];
	f32 b32 = b->m[14];
	f32 b33 = b->m[15];

	f32 r[16];
	r[0] = a00 * b00 + a10 * b01 + a20 * b02;
	r[1] = a01 * b00 + a11 * b01 + a21 * b02;
	r[2] = a02 * b00 + a12 * b01 + a22 * b02;
	r[3] = a03 * b00 + a13 * b01 + a23 * b02;

	r[4] = a00 * b10 + a10 * b11 + a20 * b12;
	r[5] = a01 * b10 + a11 * b11 + a21 * b12;
	r[6] = a02 * b10 + a12 * b11 + a22 * b12;
	r[7] = a03 * b10 + a13 * b11 + a23 * b12;

	r[8] = a00 * b20 + a10 * b21 + a20 * b22;
	r[9] = a01 * b20 + a11 * b21 + a21 * b22;
	r[10] = a02 * b20 + a12 * b21 + a22 * b22;
	r[11] = a03 * b20 + a13 * b21 + a23 * b22;

	r[12] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
	r[13] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
	r[14] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
	r[15] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;

	for (int i = 0; i < 16; i++)
	{
		out->m[i] = r[i];
	}
}

RGINLINE void rg_mat4_mul_rot(const rg_mat4* a, const rg_mat4* b, rg_mat4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 a0 = _mm_load_ps(&a->m[0]);
	__m128 a1 = _mm_load_ps(&a->m[4]);
	__m128 a2 = _mm_load_ps(&a->m[8]);
	__m128 a3 = _mm_load_ps(&a->m[12]);
	__m128 b0 = _mm_load_ps(&b->m[0]);
	__m128 b1 = _mm_load_ps(&b->m[4]);
	__m128 b2 = _mm_load_ps(&b->m[8]);

	__m128 r0 = _mm_mul_ps(a0, _mm_shuffle_ps(b0, b0, _MM_SHUFFLE(0, 0, 0, 0)));
	r0 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b0, b0, _MM_SHUFFLE(1, 1, 1, 1)), r0);
	r0 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b0, b0, _MM_SHUFFLE(2, 2, 2, 2)), r0);

	__m128 r1 = _mm_mul_ps(a0, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(0, 0, 0, 0)));
	r1 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(1, 1, 1, 1)), r1);
	r1 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(2, 2, 2, 2)), r1);

	__m128 r2 = _mm_mul_ps(a0, _mm_shuffle_ps(b2, b2, _MM_SHUFFLE(0, 0, 0, 0)));
	r2 = RG_MATH_FMADD_PS(a1, _mm_shuffle_ps(b2, b2, _MM_SHUFFLE(1, 1, 1, 1)), r2);
	r2 = RG_MATH_FMADD_PS(a2, _mm_shuffle_ps(b2, b2, _MM_SHUFFLE(2, 2, 2, 2)), r2);

	_mm_store_ps(&out->m[0], r0);
	_mm_store_ps(&out->m[4], r1);
	_mm_store_ps(&out->m[8], r2);
	_mm_store_ps(&out->m[12], a3);
#else
	f32 a00 = a->m[0];
	f32 a01 = a->m[1];
	f32 a02 = a->m[2];
	f32 a03 = a->m[3];
	f32 a10 = a->m[4];
	f32 a11 = a->m[5];
	f32 a12 = a->m[6];
	f32 a13 = a->m[7];
	f32 a20 = a->m[8];
	f32 a21 = a->m[9];
	f32 a22 = a->m[10];
	f32 a23 = a->m[11];
	f32 a30 = a->m[12];
	f32 a31 = a->m[13];
	f32 a32 = a->m[14];
	f32 a33 = a->m[15];

	f32 b00 = b->m[0];
	f32 b01 = b->m[1];
	f32 b02 = b->m[2];
	f32 b10 = b->m[4];
	f32 b11 = b->m[5];
	f32 b12 = b->m[6];
	f32 b20 = b->m[8];
	f32 b21 = b->m[9];
	f32 b22 = b->m[10];

	f32 r[16];
	r[0] = a00 * b00 + a10 * b01 + a20 * b02;
	r[1] = a01 * b00 + a11 * b01 + a21 * b02;
	r[2] = a02 * b00 + a12 * b01 + a22 * b02;
	r[3] = a03 * b00 + a13 * b01 + a23 * b02;

	r[4] = a00 * b10 + a10 * b11 + a20 * b12;
	r[5] = a01 * b10 + a11 * b11 + a21 * b12;
	r[6] = a02 * b10 + a12 * b11 + a22 * b12;
	r[7] = a03 * b10 + a13 * b11 + a23 * b12;

	r[8] = a00 * b20 + a10 * b21 + a20 * b22;
	r[9] = a01 * b20 + a11 * b21 + a21 * b22;
	r[10] = a02 * b20 + a12 * b21 + a22 * b22;
	r[11] = a03 * b20 + a13 * b21 + a23 * b22;

	r[12] = a30;
	r[13] = a31;
	r[14] = a32;
	r[15] = a33;

	for (int i = 0; i < 16; i++)
	{
		out->m[i] = r[i];
	}
#endif
}

RGINLINE void rg_mat4_mulv4(const rg_mat4* m, const rg_vec4* v, rg_vec4* out)
{
#if defined(RG_MATH_SSE)
	__m128 c0 = _mm_load_ps(&m->m[0]);
	__m128 c1 = _mm_load_ps(&m->m[4]);
	__m128 c2 = _mm_load_ps(&m->m[8]);
	__m128 c3 = _mm_load_ps(&m->m[12]);
	__m128 v4 = RG_VEC4_LOAD(v);

	__m128 x = _mm_shuffle_ps(v4, v4, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 y = _mm_shuffle_ps(v4, v4, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 z = _mm_shuffle_ps(v4, v4, _MM_SHUFFLE(2, 2, 2, 2));
	__m128 w = _mm_shuffle_ps(v4, v4, _MM_SHUFFLE(3, 3, 3, 3));

	__m128 res = _mm_mul_ps(c3, w);
	res = RG_MATH_FMADD_PS(c2, z, res);
	res = RG_MATH_FMADD_PS(c1, y, res);
	res = RG_MATH_FMADD_PS(c0, x, res);
	RG_VEC4_STORE(out, res);
#else
	f32 x = v->x;
	f32 y = v->y;
	f32 z = v->z;
	f32 w = v->w;

	out->x = m->m[0] * x + m->m[4] * y + m->m[8] * z + m->m[12] * w;
	out->y = m->m[1] * x + m->m[5] * y + m->m[9] * z + m->m[13] * w;
	out->z = m->m[2] * x + m->m[6] * y + m->m[10] * z + m->m[14] * w;
	out->w = m->m[3] * x + m->m[7] * y + m->m[11] * z + m->m[15] * w;
#endif
}

RGINLINE void rg_mat4_mulv3(const rg_mat4* m, const rg_vec3* v, f32 w, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 vv = RG_VEC3_LOAD(v);
	__m128 x = _mm_shuffle_ps(vv, vv, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 y = _mm_shuffle_ps(vv, vv, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 z = _mm_shuffle_ps(vv, vv, _MM_SHUFFLE(2, 2, 2, 2));
	__m128 ww = _mm_set1_ps(w);

	__m128 r = _mm_mul_ps(_mm_load_ps(&m->m[12]), ww);
	r = RG_MATH_FMADD_PS(_mm_load_ps(&m->m[8]), z, r);
	r = RG_MATH_FMADD_PS(_mm_load_ps(&m->m[4]), y, r);
	r = RG_MATH_FMADD_PS(_mm_load_ps(&m->m[0]), x, r);
	RG_VEC3_STORE(out, r);
#else
	f32 x = v->x;
	f32 y = v->y;
	f32 z = v->z;

	out->x = m->m[0] * x + m->m[4] * y + m->m[8] * z + m->m[12] * w;
	out->y = m->m[1] * x + m->m[5] * y + m->m[9] * z + m->m[13] * w;
	out->z = m->m[2] * x + m->m[6] * y + m->m[10] * z + m->m[14] * w;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE f32 rg_mat4_rmc(const rg_vec4* r, const rg_mat4* m, const rg_vec4* c)
{
	rg_vec4 tmp;
	rg_mat4_mulv4(m, c, &tmp);
	return rg_vec4_dot(r, &tmp);
}

RGINLINE void rg_mat4_project(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out)
{
#if (RG_MATH_CLIP_CONTROL & RG_MATH_CLIP_CONTROL_ZO_BIT)
	rg_mat4_project_zo(m, v, vp, out);
#else
	rg_mat4_project_no(m, v, vp, out);
#endif
}

RGINLINE void rg_mat4_project_no(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out)
{
	f32 vx = v->x;
	f32 vy = v->y;
	f32 vz = v->z;
	f32 x = m->m[0] * vx + m->m[4] * vy + m->m[8] * vz + m->m[12];
	f32 y = m->m[1] * vx + m->m[5] * vy + m->m[9] * vz + m->m[13];
	f32 z = m->m[2] * vx + m->m[6] * vy + m->m[10] * vz + m->m[14];
	f32 w = m->m[3] * vx + m->m[7] * vy + m->m[11] * vz + m->m[15];
#if !RG_MATH_MAX_PERF
	if (rg_absf(w) < RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->_pad = 0.0f;
		return;
	}
#endif
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 wv = _mm_set_ss(w);
	__m128 inv = _mm_rcp_ss(wv);
	inv = _mm_mul_ss(inv, _mm_sub_ss(_mm_set_ss(2.0f), _mm_mul_ss(wv, inv)));
	f32 inv_w = _mm_cvtss_f32(inv);
#else
	f32 inv_w = 1.0f / w;
#endif
	f32 sx = vp->z * 0.5f;
	f32 sy = vp->w * 0.5f;
	f32 sw = inv_w * 0.5f;
	out->x = x * (inv_w * sx) + sx + vp->x;
	out->y = y * (inv_w * sy) + sy + vp->y;
	out->z = z * sw + 0.5f;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_mat4_project_zo(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out)
{
	f32 vx = v->x;
	f32 vy = v->y;
	f32 vz = v->z;
	f32 x = m->m[0] * vx + m->m[4] * vy + m->m[8] * vz + m->m[12];
	f32 y = m->m[1] * vx + m->m[5] * vy + m->m[9] * vz + m->m[13];
	f32 z = m->m[2] * vx + m->m[6] * vy + m->m[10] * vz + m->m[14];
	f32 w = m->m[3] * vx + m->m[7] * vy + m->m[11] * vz + m->m[15];
#if !RG_MATH_MAX_PERF
	if (rg_absf(w) < RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->_pad = 0.0f;
		return;
	}
#endif
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 wv = _mm_set_ss(w);
	__m128 inv = _mm_rcp_ss(wv);
	inv = _mm_mul_ss(inv, _mm_sub_ss(_mm_set_ss(2.0f), _mm_mul_ss(wv, inv)));
	f32 inv_w = _mm_cvtss_f32(inv);
#else
	f32 inv_w = 1.0f / w;
#endif
	f32 sx = vp->z * 0.5f;
	f32 sy = vp->w * 0.5f;
	out->x = x * (inv_w * sx) + sx + vp->x;
	out->y = y * (inv_w * sy) + sy + vp->y;
	out->z = z * inv_w;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE f32 rg_mat4_project_z(const rg_mat4* m, const rg_vec3* v)
{
#if (RG_MATH_CLIP_CONTROL & RG_MATH_CLIP_CONTROL_ZO_BIT)
	return rg_mat4_project_z_zo(m, v);
#else
	return rg_mat4_project_z_no(m, v);
#endif
}

RGINLINE f32 rg_mat4_project_z_no(const rg_mat4* m, const rg_vec3* v)
{
	f32 vx = v->x;
	f32 vy = v->y;
	f32 vz = v->z;
	f32 z = m->m[2] * vx + m->m[6] * vy + m->m[10] * vz + m->m[14];
	f32 w = m->m[3] * vx + m->m[7] * vy + m->m[11] * vz + m->m[15];
#if !RG_MATH_MAX_PERF
	if (rg_absf(w) < RG_EPSILON)
	{
		return 0.0f;
	}
#endif
	return 0.5f * (z / w) + 0.5f;
}

RGINLINE f32 rg_mat4_project_z_zo(const rg_mat4* m, const rg_vec3* v)
{
	f32 vx = v->x;
	f32 vy = v->y;
	f32 vz = v->z;
	f32 z = m->m[2] * vx + m->m[6] * vy + m->m[10] * vz + m->m[14];
	f32 w = m->m[3] * vx + m->m[7] * vy + m->m[11] * vz + m->m[15];
#if !RG_MATH_MAX_PERF
	if (rg_absf(w) < RG_EPSILON)
	{
		return 0.0f;
	}
#endif
	return z / w;
}

RGINLINE void rg_mat4_pickmatrix(const rg_vec2* center, const rg_vec2* size, const rg_vec4* vp, rg_mat4* out)
{
	RG_MATH_ASSERT(size->x != 0.0f && size->y != 0.0f);
	f32 tx = (vp->z - 2.0f * (center->x - vp->x)) / size->x;
	f32 ty = (vp->w - 2.0f * (center->y - vp->y)) / size->y;
	f32 sx = vp->z / size->x;
	f32 sy = vp->w / size->y;

	rg_mat4_identity(out);
	out->m[0] = sx;
	out->m[5] = sy;
	out->m[12] = tx;
	out->m[13] = ty;
}

RGINLINE void rg_mat4_unprojecti(const rg_mat4* inv, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out)
{
#if (RG_MATH_CLIP_CONTROL & RG_MATH_CLIP_CONTROL_ZO_BIT)
	rg_mat4_unprojecti_zo(inv, v, vp, out);
#else
	rg_mat4_unprojecti_no(inv, v, vp, out);
#endif
}

RGINLINE void rg_mat4_unprojecti_no(const rg_mat4* inv, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out)
{
	f32 x = (v->x - vp->x) * 2.0f / vp->z - 1.0f;
	f32 y = (v->y - vp->y) * 2.0f / vp->w - 1.0f;
	f32 z = v->z * 2.0f - 1.0f;
	f32 ox = inv->m[0] * x + inv->m[4] * y + inv->m[8] * z + inv->m[12];
	f32 oy = inv->m[1] * x + inv->m[5] * y + inv->m[9] * z + inv->m[13];
	f32 oz = inv->m[2] * x + inv->m[6] * y + inv->m[10] * z + inv->m[14];
	f32 ow = inv->m[3] * x + inv->m[7] * y + inv->m[11] * z + inv->m[15];
#if !RG_MATH_MAX_PERF
	if (rg_absf(ow) < RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->_pad = 0.0f;
		return;
	}
#endif
	f32 inv_w = 1.0f / ow;
	out->x = ox * inv_w;
	out->y = oy * inv_w;
	out->z = oz * inv_w;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_mat4_unprojecti_zo(const rg_mat4* inv, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out)
{
	f32 x = (v->x - vp->x) * 2.0f / vp->z - 1.0f;
	f32 y = (v->y - vp->y) * 2.0f / vp->w - 1.0f;
	f32 z = v->z;
	f32 ox = inv->m[0] * x + inv->m[4] * y + inv->m[8] * z + inv->m[12];
	f32 oy = inv->m[1] * x + inv->m[5] * y + inv->m[9] * z + inv->m[13];
	f32 oz = inv->m[2] * x + inv->m[6] * y + inv->m[10] * z + inv->m[14];
	f32 ow = inv->m[3] * x + inv->m[7] * y + inv->m[11] * z + inv->m[15];
#if !RG_MATH_MAX_PERF
	if (rg_absf(ow) < RG_EPSILON)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		out->_pad = 0.0f;
		return;
	}
#endif
	f32 inv_w = 1.0f / ow;
	out->x = ox * inv_w;
	out->y = oy * inv_w;
	out->z = oz * inv_w;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg__mat4_unproject_adjoint(const rg_mat4* m, f32 x, f32 y, f32 z, rg_vec3* out)
{
	f32 a = m->m[0];
	f32 b = m->m[1];
	f32 c = m->m[2];
	f32 d = m->m[3];
	f32 e = m->m[4];
	f32 f = m->m[5];
	f32 g = m->m[6];
	f32 h = m->m[7];
	f32 i = m->m[8];
	f32 j = m->m[9];
	f32 k = m->m[10];
	f32 l = m->m[11];
	f32 m0 = m->m[12];
	f32 n = m->m[13];
	f32 o = m->m[14];
	f32 p = m->m[15];

	f32 c1 = k * p - l * o;
	f32 c2 = c * h - d * g;
	f32 c3 = i * p - l * m0;
	f32 c4 = a * h - d * e;
	f32 c5 = j * p - l * n;
	f32 c6 = b * h - d * f;
	f32 c7 = i * n - j * m0;
	f32 c8 = a * f - b * e;
	f32 c9 = j * o - k * n;
	f32 c10 = b * g - c * f;
	f32 c11 = i * o - k * m0;
	f32 c12 = a * g - c * e;

#ifndef NDEBUG
	f32 det = c8 * c1 + c4 * c9 + c10 * c3 + c2 * c7 - c12 * c5 - c6 * c11;
	RG_MATH_ASSERT(det != 0.0f);
#endif

	f32 ox = (f * c1 - g * c5 + h * c9) * x - (e * c1 - g * c3 + h * c11) * y + (e * c5 - f * c3 + h * c7) * z - (e * c9 - f * c11 + g * c7);
	f32 oy = -(b * c1 - c * c5 + d * c9) * x + (a * c1 - c * c3 + d * c11) * y - (a * c5 - b * c3 + d * c7) * z + (a * c9 - b * c11 + c * c7);
	f32 oz = (n * c2 - o * c6 + p * c10) * x - (m0 * c2 - o * c4 + p * c12) * y + (m0 * c6 - n * c4 + p * c8) * z - (m0 * c10 - n * c12 + o * c8);
	f32 ow = -(j * c2 - k * c6 + l * c10) * x + (i * c2 - k * c4 + l * c12) * y - (i * c6 - j * c4 + l * c8) * z + (i * c10 - j * c12 + k * c8);

	f32 inv_w = 1.0f / ow;
	out->x = ox * inv_w;
	out->y = oy * inv_w;
	out->z = oz * inv_w;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_mat4_unproject(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out)
{
#if (RG_MATH_CLIP_CONTROL & RG_MATH_CLIP_CONTROL_ZO_BIT)
	rg_mat4_unproject_zo(m, v, vp, out);
#else
	rg_mat4_unproject_no(m, v, vp, out);
#endif
}

RGINLINE void rg_mat4_unproject_no(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out)
{
#if RG_MATH_MAX_PERF
	f32 x = (v->x - vp->x) * 2.0f / vp->z - 1.0f;
	f32 y = (v->y - vp->y) * 2.0f / vp->w - 1.0f;
	f32 z = v->z * 2.0f - 1.0f;
	rg__mat4_unproject_adjoint(m, x, y, z, out);
#else
	rg_mat4 inv;
	rg_mat4_inv(m, &inv);
	rg_mat4_unprojecti_no(&inv, v, vp, out);
#endif
}

RGINLINE void rg_mat4_unproject_zo(const rg_mat4* m, const rg_vec3* v, const rg_vec4* vp, rg_vec3* out)
{
#if RG_MATH_MAX_PERF
	f32 x = (v->x - vp->x) * 2.0f / vp->z - 1.0f;
	f32 y = (v->y - vp->y) * 2.0f / vp->w - 1.0f;
	rg__mat4_unproject_adjoint(m, x, y, v->z, out);
#else
	rg_mat4 inv;
	rg_mat4_inv(m, &inv);
	rg_mat4_unprojecti_zo(&inv, v, vp, out);
#endif
}

RGINLINE f32 rg_mat4_det(const rg_mat4* m)
{
	f32 a = m->m[0];
	f32 b = m->m[1];
	f32 c = m->m[2];
	f32 d = m->m[3];
	f32 e = m->m[4];
	f32 f = m->m[5];
	f32 g = m->m[6];
	f32 h = m->m[7];
	f32 i = m->m[8];
	f32 j = m->m[9];
	f32 k = m->m[10];
	f32 l = m->m[11];
	f32 m0 = m->m[12];
	f32 n = m->m[13];
	f32 o = m->m[14];
	f32 p = m->m[15];

	f32 t0 = k * p - o * l;
	f32 t1 = j * p - n * l;
	f32 t2 = j * o - n * k;
	f32 t3 = i * p - m0 * l;
	f32 t4 = i * o - m0 * k;
	f32 t5 = i * n - m0 * j;

	return a * (f * t0 - g * t1 + h * t2) - b * (e * t0 - g * t3 + h * t4) + c * (e * t1 - f * t3 + h * t5) - d * (e * t2 - f * t4 + g * t5);
}

RGINLINE void rg_mat4_inv(const rg_mat4* m, rg_mat4* out)
{
	f32 a = m->m[0];
	f32 b = m->m[1];
	f32 c = m->m[2];
	f32 d = m->m[3];
	f32 e = m->m[4];
	f32 f = m->m[5];
	f32 g = m->m[6];
	f32 h = m->m[7];
	f32 i = m->m[8];
	f32 j = m->m[9];
	f32 k = m->m[10];
	f32 l = m->m[11];
	f32 m0 = m->m[12];
	f32 n = m->m[13];
	f32 o = m->m[14];
	f32 p = m->m[15];

	f32 c1 = k * p - l * o;
	f32 c2 = c * h - d * g;
	f32 c3 = i * p - l * m0;
	f32 c4 = a * h - d * e;
	f32 c5 = j * p - l * n;
	f32 c6 = b * h - d * f;
	f32 c7 = i * n - j * m0;
	f32 c8 = a * f - b * e;
	f32 c9 = j * o - k * n;
	f32 c10 = b * g - c * f;
	f32 c11 = i * o - k * m0;
	f32 c12 = a * g - c * e;

	f32 det = c8 * c1 + c4 * c9 + c10 * c3 + c2 * c7 - c12 * c5 - c6 * c11;
	RG_MATH_ASSERT(det != 0.0f);
	f32 idt = 1.0f / det;
	f32 ndt = -idt;

	out->m[0] = (f * c1 - g * c5 + h * c9) * idt;
	out->m[1] = (b * c1 - c * c5 + d * c9) * ndt;
	out->m[2] = (n * c2 - o * c6 + p * c10) * idt;
	out->m[3] = (j * c2 - k * c6 + l * c10) * ndt;

	out->m[4] = (e * c1 - g * c3 + h * c11) * ndt;
	out->m[5] = (a * c1 - c * c3 + d * c11) * idt;
	out->m[6] = (m0 * c2 - o * c4 + p * c12) * ndt;
	out->m[7] = (i * c2 - k * c4 + l * c12) * idt;

	out->m[8] = (e * c5 - f * c3 + h * c7) * idt;
	out->m[9] = (a * c5 - b * c3 + d * c7) * ndt;
	out->m[10] = (m0 * c6 - n * c4 + p * c8) * idt;
	out->m[11] = (i * c6 - j * c4 + l * c8) * ndt;

	out->m[12] = (e * c9 - f * c11 + g * c7) * ndt;
	out->m[13] = (a * c9 - b * c11 + c * c7) * idt;
	out->m[14] = (m0 * c10 - n * c12 + o * c8) * ndt;
	out->m[15] = (i * c10 - j * c12 + k * c8) * idt;
}

RGINLINE void rg_mat4_inv_affine(const rg_mat4* m, rg_mat4* out)
{
	f32 a00 = m->m[0];
	f32 a01 = m->m[4];
	f32 a02 = m->m[8];
	f32 a10 = m->m[1];
	f32 a11 = m->m[5];
	f32 a12 = m->m[9];
	f32 a20 = m->m[2];
	f32 a21 = m->m[6];
	f32 a22 = m->m[10];
	f32 tx = m->m[12];
	f32 ty = m->m[13];
	f32 tz = m->m[14];

	f32 c00 = a11 * a22 - a12 * a21;
	f32 c01 = a10 * a22 - a12 * a20;
	f32 c02 = a10 * a21 - a11 * a20;
	f32 c10 = a01 * a22 - a02 * a21;
	f32 c11 = a00 * a22 - a02 * a20;
	f32 c12 = a00 * a21 - a01 * a20;
	f32 c20 = a01 * a12 - a02 * a11;
	f32 c21 = a00 * a12 - a02 * a10;
	f32 c22 = a00 * a11 - a01 * a10;

	f32 det = a00 * c00 - a01 * c01 + a02 * c02;
	RG_MATH_ASSERT(det != 0.0f);
	f32 inv_det = 1.0f / det;

	f32 i00 = c00 * inv_det;
	f32 i01 = -c10 * inv_det;
	f32 i02 = c20 * inv_det;
	f32 i10 = -c01 * inv_det;
	f32 i11 = c11 * inv_det;
	f32 i12 = -c21 * inv_det;
	f32 i20 = c02 * inv_det;
	f32 i21 = -c12 * inv_det;
	f32 i22 = c22 * inv_det;

	out->m[0] = i00;
	out->m[1] = i10;
	out->m[2] = i20;
	out->m[3] = 0.0f;

	out->m[4] = i01;
	out->m[5] = i11;
	out->m[6] = i21;
	out->m[7] = 0.0f;

	out->m[8] = i02;
	out->m[9] = i12;
	out->m[10] = i22;
	out->m[11] = 0.0f;

	out->m[12] = -(i00 * tx + i01 * ty + i02 * tz);
	out->m[13] = -(i10 * tx + i11 * ty + i12 * tz);
	out->m[14] = -(i20 * tx + i21 * ty + i22 * tz);
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_inv_affine_fast(const rg_mat4* m, rg_mat4* out)
{
	f32 r00 = m->m[0];
	f32 r01 = m->m[4];
	f32 r02 = m->m[8];
	f32 r10 = m->m[1];
	f32 r11 = m->m[5];
	f32 r12 = m->m[9];
	f32 r20 = m->m[2];
	f32 r21 = m->m[6];
	f32 r22 = m->m[10];

	out->m[0] = r00;
	out->m[1] = r01;
	out->m[2] = r02;
	out->m[3] = 0.0f;

	out->m[4] = r10;
	out->m[5] = r11;
	out->m[6] = r12;
	out->m[7] = 0.0f;

	out->m[8] = r20;
	out->m[9] = r21;
	out->m[10] = r22;
	out->m[11] = 0.0f;

	f32 tx = m->m[12];
	f32 ty = m->m[13];
	f32 tz = m->m[14];

	out->m[12] = -(r00 * tx + r10 * ty + r20 * tz);
	out->m[13] = -(r01 * tx + r11 * ty + r21 * tz);
	out->m[14] = -(r02 * tx + r12 * ty + r22 * tz);
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_inv_tr(const rg_mat4* m, rg_mat4* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 r0 = _mm_load_ps(&m->m[0]);
	__m128 r1 = _mm_load_ps(&m->m[4]);
	__m128 r2 = _mm_load_ps(&m->m[8]);
	__m128 t = _mm_load_ps(&m->m[12]);
	__m128 affine_w = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

	_MM_TRANSPOSE4_PS(r0, r1, r2, affine_w);

	__m128 inv_t = _mm_mul_ps(r0, _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 0, 0, 0)));
	inv_t = RG_MATH_FMADD_PS(r1, _mm_shuffle_ps(t, t, _MM_SHUFFLE(1, 1, 1, 1)), inv_t);
	inv_t = RG_MATH_FMADD_PS(r2, _mm_shuffle_ps(t, t, _MM_SHUFFLE(2, 2, 2, 2)), inv_t);
	inv_t = _mm_xor_ps(inv_t, _mm_set1_ps(-0.0f));
	inv_t = _mm_add_ps(inv_t, _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f));

	_mm_store_ps(&out->m[0], r0);
	_mm_store_ps(&out->m[4], r1);
	_mm_store_ps(&out->m[8], r2);
	_mm_store_ps(&out->m[12], inv_t);
#else
	rg_mat4_inv_affine_fast(m, out);
#endif
}

RGINLINE void rg_mat4_perspective(f32 fovy_radians, f32 aspect, f32 near_z, f32 far_z, rg_mat4* out)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_perspective_lh_zo(fovy_radians, aspect, near_z, far_z, out);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_perspective_lh_no(fovy_radians, aspect, near_z, far_z, out);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_perspective_rh_zo(fovy_radians, aspect, near_z, far_z, out);
#else
	rg_mat4_perspective_rh_no(fovy_radians, aspect, near_z, far_z, out);
#endif
}

RGINLINE void rg_mat4_perspective_rh_no(f32 fovy_radians, f32 aspect, f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 f = 1.0f / rg_tanf(fovy_radians * 0.5f);
	f32 fn = 1.0f / (near_z - far_z);

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_store_ps(&out->m[0], _mm_setr_ps(f / aspect, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[4], _mm_setr_ps(0.0f, f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[8], _mm_setr_ps(0.0f, 0.0f, (near_z + far_z) * fn, -1.0f));
	_mm_store_ps(&out->m[12], _mm_setr_ps(0.0f, 0.0f, 2.0f * near_z * far_z * fn, 0.0f));
#else
	rg_mat4_zero(out);
	out->m[0] = f / aspect;
	out->m[5] = f;
	out->m[10] = (near_z + far_z) * fn;
	out->m[11] = -1.0f;
	out->m[14] = 2.0f * near_z * far_z * fn;
#endif
}

RGINLINE void rg_mat4_perspective_rh_zo(f32 fovy_radians, f32 aspect, f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 f = 1.0f / rg_tanf(fovy_radians * 0.5f);
	f32 fn = 1.0f / (near_z - far_z);

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_store_ps(&out->m[0], _mm_setr_ps(f / aspect, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[4], _mm_setr_ps(0.0f, f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[8], _mm_setr_ps(0.0f, 0.0f, far_z * fn, -1.0f));
	_mm_store_ps(&out->m[12], _mm_setr_ps(0.0f, 0.0f, near_z * far_z * fn, 0.0f));
#else
	rg_mat4_zero(out);
	out->m[0] = f / aspect;
	out->m[5] = f;
	out->m[10] = far_z * fn;
	out->m[11] = -1.0f;
	out->m[14] = near_z * far_z * fn;
#endif
}

RGINLINE void rg_mat4_perspective_lh_no(f32 fovy_radians, f32 aspect, f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 f = 1.0f / rg_tanf(fovy_radians * 0.5f);
	f32 fn = 1.0f / (near_z - far_z);

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_store_ps(&out->m[0], _mm_setr_ps(f / aspect, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[4], _mm_setr_ps(0.0f, f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[8], _mm_setr_ps(0.0f, 0.0f, -(near_z + far_z) * fn, 1.0f));
	_mm_store_ps(&out->m[12], _mm_setr_ps(0.0f, 0.0f, 2.0f * near_z * far_z * fn, 0.0f));
#else
	rg_mat4_zero(out);
	out->m[0] = f / aspect;
	out->m[5] = f;
	out->m[10] = -(near_z + far_z) * fn;
	out->m[11] = 1.0f;
	out->m[14] = 2.0f * near_z * far_z * fn;
#endif
}

RGINLINE void rg_mat4_perspective_lh_zo(f32 fovy_radians, f32 aspect, f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 f = 1.0f / rg_tanf(fovy_radians * 0.5f);
	f32 fn = 1.0f / (near_z - far_z);

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_store_ps(&out->m[0], _mm_setr_ps(f / aspect, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[4], _mm_setr_ps(0.0f, f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[8], _mm_setr_ps(0.0f, 0.0f, -far_z * fn, 1.0f));
	_mm_store_ps(&out->m[12], _mm_setr_ps(0.0f, 0.0f, near_z * far_z * fn, 0.0f));
#else
	rg_mat4_zero(out);
	out->m[0] = f / aspect;
	out->m[5] = f;
	out->m[10] = -far_z * fn;
	out->m[11] = 1.0f;
	out->m[14] = near_z * far_z * fn;
#endif
}

RGINLINE void rg_mat4_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z, rg_mat4* out)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_ortho_lh_zo(left, right, bottom, top, near_z, far_z, out);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_ortho_lh_no(left, right, bottom, top, near_z, far_z, out);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_ortho_rh_zo(left, right, bottom, top, near_z, far_z, out);
#else
	rg_mat4_ortho_rh_no(left, right, bottom, top, near_z, far_z, out);
#endif
}

RGINLINE void rg_mat4_ortho_rh_no(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 rl = 1.0f / (right - left);
	f32 tb = 1.0f / (top - bottom);
	f32 fn = -1.0f / (far_z - near_z);

	out->m[0] = 2.0f * rl;
	out->m[1] = 0.0f;
	out->m[2] = 0.0f;
	out->m[3] = 0.0f;

	out->m[4] = 0.0f;
	out->m[5] = 2.0f * tb;
	out->m[6] = 0.0f;
	out->m[7] = 0.0f;

	out->m[8] = 0.0f;
	out->m[9] = 0.0f;
	out->m[10] = 2.0f * fn;
	out->m[11] = 0.0f;

	out->m[12] = -(right + left) * rl;
	out->m[13] = -(top + bottom) * tb;
	out->m[14] = (far_z + near_z) * fn;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_ortho_rh_zo(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 rl = 1.0f / (right - left);
	f32 tb = 1.0f / (top - bottom);
	f32 fn = -1.0f / (far_z - near_z);

	out->m[0] = 2.0f * rl;
	out->m[1] = 0.0f;
	out->m[2] = 0.0f;
	out->m[3] = 0.0f;

	out->m[4] = 0.0f;
	out->m[5] = 2.0f * tb;
	out->m[6] = 0.0f;
	out->m[7] = 0.0f;

	out->m[8] = 0.0f;
	out->m[9] = 0.0f;
	out->m[10] = fn;
	out->m[11] = 0.0f;

	out->m[12] = -(right + left) * rl;
	out->m[13] = -(top + bottom) * tb;
	out->m[14] = near_z * fn;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_ortho_lh_no(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 rl = 1.0f / (right - left);
	f32 tb = 1.0f / (top - bottom);
	f32 fn = -1.0f / (far_z - near_z);

	out->m[0] = 2.0f * rl;
	out->m[1] = 0.0f;
	out->m[2] = 0.0f;
	out->m[3] = 0.0f;

	out->m[4] = 0.0f;
	out->m[5] = 2.0f * tb;
	out->m[6] = 0.0f;
	out->m[7] = 0.0f;

	out->m[8] = 0.0f;
	out->m[9] = 0.0f;
	out->m[10] = -2.0f * fn;
	out->m[11] = 0.0f;

	out->m[12] = -(right + left) * rl;
	out->m[13] = -(top + bottom) * tb;
	out->m[14] = (far_z + near_z) * fn;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_ortho_lh_zo(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 rl = 1.0f / (right - left);
	f32 tb = 1.0f / (top - bottom);
	f32 fn = -1.0f / (far_z - near_z);

	out->m[0] = 2.0f * rl;
	out->m[1] = 0.0f;
	out->m[2] = 0.0f;
	out->m[3] = 0.0f;

	out->m[4] = 0.0f;
	out->m[5] = 2.0f * tb;
	out->m[6] = 0.0f;
	out->m[7] = 0.0f;

	out->m[8] = 0.0f;
	out->m[9] = 0.0f;
	out->m[10] = -fn;
	out->m[11] = 0.0f;

	out->m[12] = -(right + left) * rl;
	out->m[13] = -(top + bottom) * tb;
	out->m[14] = near_z * fn;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_look_at(const rg_vec3* eye, const rg_vec3* center, const rg_vec3* up, rg_mat4* out)
{
#if (RG_MATH_CLIP_CONTROL & RG_MATH_CLIP_CONTROL_LH_BIT)
	rg_mat4_look_at_lh(eye, center, up, out);
#else
	rg_mat4_look_at_rh(eye, center, up, out);
#endif
}

RGINLINE void rg_mat4_look_at_rh(const rg_vec3* eye, const rg_vec3* center, const rg_vec3* up, rg_mat4* out)
{
	f32 fx = center->x - eye->x;
	f32 fy = center->y - eye->y;
	f32 fz = center->z - eye->z;
	f32 len_sq = fx * fx + fy * fy + fz * fz;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		fx = 0.0f;
		fy = 0.0f;
		fz = 0.0f;
	}
	else
#endif
	{
#ifdef RG_MATH_FAST_NORMALIZE
		f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
		f32 inv_len = rg_rsqrtf(len_sq);
#endif
		fx *= inv_len;
		fy *= inv_len;
		fz *= inv_len;
	}

	f32 sx;
	f32 sy;
	f32 sz;
	if (up->x == 0.0f && up->y == 1.0f && up->z == 0.0f)
	{
		sx = -fz;
		sy = 0.0f;
		sz = fx;
	}
	else
	{
		sx = fy * up->z - fz * up->y;
		sy = fz * up->x - fx * up->z;
		sz = fx * up->y - fy * up->x;
	}
	len_sq = sx * sx + sy * sy + sz * sz;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		sx = 0.0f;
		sy = 0.0f;
		sz = 0.0f;
	}
	else
#endif
	{
#ifdef RG_MATH_FAST_NORMALIZE
		f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
		f32 inv_len = rg_rsqrtf(len_sq);
#endif
		sx *= inv_len;
		sy *= inv_len;
		sz *= inv_len;
	}

	f32 ux = sy * fz - sz * fy;
	f32 uy = sz * fx - sx * fz;
	f32 uz = sx * fy - sy * fx;

	out->m[0] = sx;
	out->m[1] = sy;
	out->m[2] = sz;
	out->m[3] = 0.0f;

	out->m[4] = ux;
	out->m[5] = uy;
	out->m[6] = uz;
	out->m[7] = 0.0f;

	out->m[8] = -fx;
	out->m[9] = -fy;
	out->m[10] = -fz;
	out->m[11] = 0.0f;

	out->m[12] = -(sx * eye->x + sy * eye->y + sz * eye->z);
	out->m[13] = -(ux * eye->x + uy * eye->y + uz * eye->z);
	out->m[14] = fx * eye->x + fy * eye->y + fz * eye->z;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_look_at_lh(const rg_vec3* eye, const rg_vec3* center, const rg_vec3* up, rg_mat4* out)
{
	f32 fx = center->x - eye->x;
	f32 fy = center->y - eye->y;
	f32 fz = center->z - eye->z;
	f32 len_sq = fx * fx + fy * fy + fz * fz;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		fx = 0.0f;
		fy = 0.0f;
		fz = 0.0f;
	}
	else
#endif
	{
#ifdef RG_MATH_FAST_NORMALIZE
		f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
		f32 inv_len = rg_rsqrtf(len_sq);
#endif
		fx *= inv_len;
		fy *= inv_len;
		fz *= inv_len;
	}

	f32 sx;
	f32 sy;
	f32 sz;
	if (up->x == 0.0f && up->y == 1.0f && up->z == 0.0f)
	{
		sx = fz;
		sy = 0.0f;
		sz = -fx;
	}
	else
	{
		sx = up->y * fz - up->z * fy;
		sy = up->z * fx - up->x * fz;
		sz = up->x * fy - up->y * fx;
	}
	len_sq = sx * sx + sy * sy + sz * sz;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		sx = 0.0f;
		sy = 0.0f;
		sz = 0.0f;
	}
	else
#endif
	{
#ifdef RG_MATH_FAST_NORMALIZE
		f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
		f32 inv_len = rg_rsqrtf(len_sq);
#endif
		sx *= inv_len;
		sy *= inv_len;
		sz *= inv_len;
	}

	f32 ux = fy * sz - fz * sy;
	f32 uy = fz * sx - fx * sz;
	f32 uz = fx * sy - fy * sx;

	out->m[0] = sx;
	out->m[1] = sy;
	out->m[2] = sz;
	out->m[3] = 0.0f;

	out->m[4] = ux;
	out->m[5] = uy;
	out->m[6] = uz;
	out->m[7] = 0.0f;

	out->m[8] = fx;
	out->m[9] = fy;
	out->m[10] = fz;
	out->m[11] = 0.0f;

	out->m[12] = -(sx * eye->x + sy * eye->y + sz * eye->z);
	out->m[13] = -(ux * eye->x + uy * eye->y + uz * eye->z);
	out->m[14] = -(fx * eye->x + fy * eye->y + fz * eye->z);
	out->m[15] = 1.0f;
}

#endif // RG_MATH_MAT_H
