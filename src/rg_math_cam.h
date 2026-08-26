// rg_math_cam - camera and projection helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides camera transforms, projection helpers, and frustum extraction.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_CAM_H
#define RG_MATH_CAM_H

#include "rg_math_mat.h"

RG_MATH_EXTERN_C_BEGIN

// =============================================================================
// CAMERA / PROJECTION HELPERS
// =============================================================================

RGINLINE void rg_mat4_frustum(f32 left, f32 right, f32 bottom, f32 top,
                              f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_frustum_rh_no(f32 left, f32 right, f32 bottom, f32 top,
                                    f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_frustum_rh_zo(f32 left, f32 right, f32 bottom, f32 top,
                                    f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_frustum_lh_no(f32 left, f32 right, f32 bottom, f32 top,
                                    f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_frustum_lh_zo(f32 left, f32 right, f32 bottom, f32 top,
                                    f32 near_z, f32 far_z, rg_mat4* out);
RGINLINE void rg_mat4_ortho_aabb(const rg_vec3 box[2], rg_mat4* out);
RGINLINE void rg_mat4_ortho_aabb_p(const rg_vec3 box[2], f32 padding, rg_mat4* out);
RGINLINE void rg_mat4_ortho_aabb_pz(const rg_vec3 box[2], f32 padding, rg_mat4* out);
RGINLINE void rg_mat4_ortho_default(f32 aspect, rg_mat4* out);
RGINLINE void rg_mat4_ortho_default_s(f32 aspect, f32 size, rg_mat4* out);
RGINLINE void rg_mat4_perspective_default(f32 aspect, rg_mat4* out);
RGINLINE void rg_mat4_perspective_resize(f32 aspect, rg_mat4* proj);
RGINLINE void rg_mat4_look(const rg_vec3* eye, const rg_vec3* dir, const rg_vec3* up, rg_mat4* out);
RGINLINE void rg_mat4_look_rh(const rg_vec3* eye, const rg_vec3* dir, const rg_vec3* up, rg_mat4* out);
RGINLINE void rg_mat4_look_lh(const rg_vec3* eye, const rg_vec3* dir, const rg_vec3* up, rg_mat4* out);
RGINLINE void rg_mat4_look_anyup(const rg_vec3* eye, const rg_vec3* dir, rg_mat4* out);
RGINLINE void rg_mat4_look_anyup_rh(const rg_vec3* eye, const rg_vec3* dir, rg_mat4* out);
RGINLINE void rg_mat4_look_anyup_lh(const rg_vec3* eye, const rg_vec3* dir, rg_mat4* out);

RGINLINE void rg_mat4_persp_move_far(rg_mat4* proj, f32 delta_far);
RGINLINE void rg_mat4_persp_move_far_rh_no(rg_mat4* proj, f32 delta_far);
RGINLINE void rg_mat4_persp_move_far_rh_zo(rg_mat4* proj, f32 delta_far);
RGINLINE void rg_mat4_persp_move_far_lh_no(rg_mat4* proj, f32 delta_far);
RGINLINE void rg_mat4_persp_move_far_lh_zo(rg_mat4* proj, f32 delta_far);

RGINLINE void rg_mat4_persp_decomp(const rg_mat4* proj, f32* near_z, f32* far_z,
                                   f32* top, f32* bottom, f32* left, f32* right);
RGINLINE void rg_mat4_persp_decomp_rh_no(const rg_mat4* proj, f32* near_z, f32* far_z,
                                         f32* top, f32* bottom, f32* left, f32* right);
RGINLINE void rg_mat4_persp_decomp_rh_zo(const rg_mat4* proj, f32* near_z, f32* far_z,
                                         f32* top, f32* bottom, f32* left, f32* right);
RGINLINE void rg_mat4_persp_decomp_lh_no(const rg_mat4* proj, f32* near_z, f32* far_z,
                                         f32* top, f32* bottom, f32* left, f32* right);
RGINLINE void rg_mat4_persp_decomp_lh_zo(const rg_mat4* proj, f32* near_z, f32* far_z,
                                         f32* top, f32* bottom, f32* left, f32* right);
RGINLINE void rg_mat4_persp_decompv(const rg_mat4* proj, f32 dest[6]);
RGINLINE void rg_mat4_persp_decompv_rh_no(const rg_mat4* proj, f32 dest[6]);
RGINLINE void rg_mat4_persp_decompv_rh_zo(const rg_mat4* proj, f32 dest[6]);
RGINLINE void rg_mat4_persp_decompv_lh_no(const rg_mat4* proj, f32 dest[6]);
RGINLINE void rg_mat4_persp_decompv_lh_zo(const rg_mat4* proj, f32 dest[6]);
RGINLINE void rg_mat4_persp_decomp_x(const rg_mat4* proj, f32* left, f32* right);
RGINLINE void rg_mat4_persp_decomp_x_rh_no(const rg_mat4* proj, f32* left, f32* right);
RGINLINE void rg_mat4_persp_decomp_x_rh_zo(const rg_mat4* proj, f32* left, f32* right);
RGINLINE void rg_mat4_persp_decomp_x_lh_no(const rg_mat4* proj, f32* left, f32* right);
RGINLINE void rg_mat4_persp_decomp_x_lh_zo(const rg_mat4* proj, f32* left, f32* right);
RGINLINE void rg_mat4_persp_decomp_y(const rg_mat4* proj, f32* top, f32* bottom);
RGINLINE void rg_mat4_persp_decomp_y_rh_no(const rg_mat4* proj, f32* top, f32* bottom);
RGINLINE void rg_mat4_persp_decomp_y_rh_zo(const rg_mat4* proj, f32* top, f32* bottom);
RGINLINE void rg_mat4_persp_decomp_y_lh_no(const rg_mat4* proj, f32* top, f32* bottom);
RGINLINE void rg_mat4_persp_decomp_y_lh_zo(const rg_mat4* proj, f32* top, f32* bottom);
RGINLINE void rg_mat4_persp_decomp_z(const rg_mat4* proj, f32* near_z, f32* far_z);
RGINLINE void rg_mat4_persp_decomp_z_rh_no(const rg_mat4* proj, f32* near_z, f32* far_z);
RGINLINE void rg_mat4_persp_decomp_z_rh_zo(const rg_mat4* proj, f32* near_z, f32* far_z);
RGINLINE void rg_mat4_persp_decomp_z_lh_no(const rg_mat4* proj, f32* near_z, f32* far_z);
RGINLINE void rg_mat4_persp_decomp_z_lh_zo(const rg_mat4* proj, f32* near_z, f32* far_z);
RGINLINE void rg_mat4_persp_decomp_far(const rg_mat4* proj, f32* far_z);
RGINLINE void rg_mat4_persp_decomp_far_rh_no(const rg_mat4* proj, f32* far_z);
RGINLINE void rg_mat4_persp_decomp_far_rh_zo(const rg_mat4* proj, f32* far_z);
RGINLINE void rg_mat4_persp_decomp_far_lh_no(const rg_mat4* proj, f32* far_z);
RGINLINE void rg_mat4_persp_decomp_far_lh_zo(const rg_mat4* proj, f32* far_z);
RGINLINE void rg_mat4_persp_decomp_near(const rg_mat4* proj, f32* near_z);
RGINLINE void rg_mat4_persp_decomp_near_rh_no(const rg_mat4* proj, f32* near_z);
RGINLINE void rg_mat4_persp_decomp_near_rh_zo(const rg_mat4* proj, f32* near_z);
RGINLINE void rg_mat4_persp_decomp_near_lh_no(const rg_mat4* proj, f32* near_z);
RGINLINE void rg_mat4_persp_decomp_near_lh_zo(const rg_mat4* proj, f32* near_z);
RGINLINE f32 rg_mat4_persp_fovy(const rg_mat4* proj);
RGINLINE f32 rg_mat4_persp_aspect(const rg_mat4* proj);
RGINLINE void rg_mat4_persp_sizes(const rg_mat4* proj, f32 fovy, rg_vec4* out);
RGINLINE void rg_mat4_persp_sizes_rh_no(const rg_mat4* proj, f32 fovy, rg_vec4* out);
RGINLINE void rg_mat4_persp_sizes_rh_zo(const rg_mat4* proj, f32 fovy, rg_vec4* out);
RGINLINE void rg_mat4_persp_sizes_lh_no(const rg_mat4* proj, f32 fovy, rg_vec4* out);
RGINLINE void rg_mat4_persp_sizes_lh_zo(const rg_mat4* proj, f32 fovy, rg_vec4* out);

RG_MATH_EXTERN_C_END

// =============================================================================
// Camera / Projection Implementation
// =============================================================================

RGINLINE void rg_mat4_frustum(f32 left, f32 right, f32 bottom, f32 top,
                              f32 near_z, f32 far_z, rg_mat4* out)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_frustum_lh_zo(left, right, bottom, top, near_z, far_z, out);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_frustum_lh_no(left, right, bottom, top, near_z, far_z, out);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_frustum_rh_zo(left, right, bottom, top, near_z, far_z, out);
#else
	rg_mat4_frustum_rh_no(left, right, bottom, top, near_z, far_z, out);
#endif
}

RGINLINE void rg_mat4_frustum_rh_no(f32 left, f32 right, f32 bottom, f32 top,
                                    f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 rl = 1.0f / (right - left);
	f32 tb = 1.0f / (top - bottom);
	f32 fn = -1.0f / (far_z - near_z);
	f32 nv = 2.0f * near_z;

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_store_ps(&out->m[0], _mm_setr_ps(nv * rl, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[4], _mm_setr_ps(0.0f, nv * tb, 0.0f, 0.0f));
	_mm_store_ps(&out->m[8], _mm_setr_ps((right + left) * rl, (top + bottom) * tb,
	                                     (far_z + near_z) * fn, -1.0f));
	_mm_store_ps(&out->m[12], _mm_setr_ps(0.0f, 0.0f, far_z * nv * fn, 0.0f));
#else
	out->m[0] = nv * rl;
	out->m[1] = 0.0f;
	out->m[2] = 0.0f;
	out->m[3] = 0.0f;

	out->m[4] = 0.0f;
	out->m[5] = nv * tb;
	out->m[6] = 0.0f;
	out->m[7] = 0.0f;

	out->m[8] = (right + left) * rl;
	out->m[9] = (top + bottom) * tb;
	out->m[10] = (far_z + near_z) * fn;
	out->m[11] = -1.0f;

	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = far_z * nv * fn;
	out->m[15] = 0.0f;
#endif
}

RGINLINE void rg_mat4_frustum_rh_zo(f32 left, f32 right, f32 bottom, f32 top,
                                    f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 rl = 1.0f / (right - left);
	f32 tb = 1.0f / (top - bottom);
	f32 fn = -1.0f / (far_z - near_z);
	f32 nv = 2.0f * near_z;

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_store_ps(&out->m[0], _mm_setr_ps(nv * rl, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[4], _mm_setr_ps(0.0f, nv * tb, 0.0f, 0.0f));
	_mm_store_ps(&out->m[8], _mm_setr_ps((right + left) * rl, (top + bottom) * tb,
	                                     far_z * fn, -1.0f));
	_mm_store_ps(&out->m[12], _mm_setr_ps(0.0f, 0.0f, far_z * near_z * fn, 0.0f));
#else
	out->m[0] = nv * rl;
	out->m[1] = 0.0f;
	out->m[2] = 0.0f;
	out->m[3] = 0.0f;

	out->m[4] = 0.0f;
	out->m[5] = nv * tb;
	out->m[6] = 0.0f;
	out->m[7] = 0.0f;

	out->m[8] = (right + left) * rl;
	out->m[9] = (top + bottom) * tb;
	out->m[10] = far_z * fn;
	out->m[11] = -1.0f;

	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = far_z * near_z * fn;
	out->m[15] = 0.0f;
#endif
}

RGINLINE void rg_mat4_frustum_lh_no(f32 left, f32 right, f32 bottom, f32 top,
                                    f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 rl = 1.0f / (right - left);
	f32 tb = 1.0f / (top - bottom);
	f32 fn = -1.0f / (far_z - near_z);
	f32 nv = 2.0f * near_z;

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_store_ps(&out->m[0], _mm_setr_ps(nv * rl, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[4], _mm_setr_ps(0.0f, nv * tb, 0.0f, 0.0f));
	_mm_store_ps(&out->m[8], _mm_setr_ps((right + left) * rl, (top + bottom) * tb,
	                                     -(far_z + near_z) * fn, 1.0f));
	_mm_store_ps(&out->m[12], _mm_setr_ps(0.0f, 0.0f, far_z * nv * fn, 0.0f));
#else
	out->m[0] = nv * rl;
	out->m[1] = 0.0f;
	out->m[2] = 0.0f;
	out->m[3] = 0.0f;

	out->m[4] = 0.0f;
	out->m[5] = nv * tb;
	out->m[6] = 0.0f;
	out->m[7] = 0.0f;

	out->m[8] = (right + left) * rl;
	out->m[9] = (top + bottom) * tb;
	out->m[10] = -(far_z + near_z) * fn;
	out->m[11] = 1.0f;

	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = far_z * nv * fn;
	out->m[15] = 0.0f;
#endif
}

RGINLINE void rg_mat4_frustum_lh_zo(f32 left, f32 right, f32 bottom, f32 top,
                                    f32 near_z, f32 far_z, rg_mat4* out)
{
	f32 rl = 1.0f / (right - left);
	f32 tb = 1.0f / (top - bottom);
	f32 fn = -1.0f / (far_z - near_z);
	f32 nv = 2.0f * near_z;

#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	_mm_store_ps(&out->m[0], _mm_setr_ps(nv * rl, 0.0f, 0.0f, 0.0f));
	_mm_store_ps(&out->m[4], _mm_setr_ps(0.0f, nv * tb, 0.0f, 0.0f));
	_mm_store_ps(&out->m[8], _mm_setr_ps((right + left) * rl, (top + bottom) * tb,
	                                     -far_z * fn, 1.0f));
	_mm_store_ps(&out->m[12], _mm_setr_ps(0.0f, 0.0f, far_z * near_z * fn, 0.0f));
#else
	out->m[0] = nv * rl;
	out->m[1] = 0.0f;
	out->m[2] = 0.0f;
	out->m[3] = 0.0f;

	out->m[4] = 0.0f;
	out->m[5] = nv * tb;
	out->m[6] = 0.0f;
	out->m[7] = 0.0f;

	out->m[8] = (right + left) * rl;
	out->m[9] = (top + bottom) * tb;
	out->m[10] = -far_z * fn;
	out->m[11] = 1.0f;

	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = far_z * near_z * fn;
	out->m[15] = 0.0f;
#endif
}

RGINLINE void rg_mat4_ortho_aabb(const rg_vec3 box[2], rg_mat4* out)
{
	rg_mat4_ortho(box[0].x, box[1].x,
	              box[0].y, box[1].y,
	              -box[1].z, -box[0].z,
	              out);
}

RGINLINE void rg_mat4_ortho_aabb_p(const rg_vec3 box[2], f32 padding, rg_mat4* out)
{
	rg_mat4_ortho(box[0].x - padding, box[1].x + padding,
	              box[0].y - padding, box[1].y + padding,
	              -(box[1].z + padding), -(box[0].z - padding),
	              out);
}

RGINLINE void rg_mat4_ortho_aabb_pz(const rg_vec3 box[2], f32 padding, rg_mat4* out)
{
	rg_mat4_ortho(box[0].x, box[1].x,
	              box[0].y, box[1].y,
	              -(box[1].z + padding), -(box[0].z - padding),
	              out);
}

RGINLINE void rg_mat4_ortho_default(f32 aspect, rg_mat4* out)
{
	if (aspect >= 1.0f)
	{
		rg_mat4_ortho(-aspect, aspect, -1.0f, 1.0f, -100.0f, 100.0f, out);
		return;
	}

	aspect = 1.0f / aspect;
	rg_mat4_ortho(-1.0f, 1.0f, -aspect, aspect, -100.0f, 100.0f, out);
}

RGINLINE void rg_mat4_ortho_default_s(f32 aspect, f32 size, rg_mat4* out)
{
	if (aspect >= 1.0f)
	{
		rg_mat4_ortho(-size * aspect, size * aspect,
		              -size, size,
		              -size - 100.0f, size + 100.0f,
		              out);
		return;
	}

	rg_mat4_ortho(-size, size,
	              -size / aspect, size / aspect,
	              -size - 100.0f, size + 100.0f,
	              out);
}

RGINLINE void rg_mat4_perspective_default(f32 aspect, rg_mat4* out)
{
	rg_mat4_perspective(RG_PI * 0.25f, aspect, 0.01f, 100.0f, out);
}

RGINLINE void rg_mat4_perspective_resize(f32 aspect, rg_mat4* proj)
{
	if (proj->m[0] == 0.0f)
		return;

	proj->m[0] = proj->m[5] / aspect;
}

RGINLINE void rg_mat4_look(const rg_vec3* eye, const rg_vec3* dir, const rg_vec3* up, rg_mat4* out)
{
#if (RG_MATH_CLIP_CONTROL & RG_MATH_CLIP_CONTROL_LH_BIT)
	rg_mat4_look_lh(eye, dir, up, out);
#else
	rg_mat4_look_rh(eye, dir, up, out);
#endif
}

RGINLINE void rg_mat4_look_rh(const rg_vec3* eye, const rg_vec3* dir, const rg_vec3* up, rg_mat4* out)
{
	f32 fx = dir->x;
	f32 fy = dir->y;
	f32 fz = dir->z;
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

	f32 sx = fy * up->z - fz * up->y;
	f32 sy = fz * up->x - fx * up->z;
	f32 sz = fx * up->y - fy * up->x;
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

RGINLINE void rg_mat4_look_lh(const rg_vec3* eye, const rg_vec3* dir, const rg_vec3* up, rg_mat4* out)
{
	f32 fx = dir->x;
	f32 fy = dir->y;
	f32 fz = dir->z;
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

	f32 sx = up->y * fz - up->z * fy;
	f32 sy = up->z * fx - up->x * fz;
	f32 sz = up->x * fy - up->y * fx;
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

RGINLINE void rg_mat4_look_anyup(const rg_vec3* eye, const rg_vec3* dir, rg_mat4* out)
{
#if (RG_MATH_CLIP_CONTROL & RG_MATH_CLIP_CONTROL_LH_BIT)
	rg_mat4_look_anyup_lh(eye, dir, out);
#else
	rg_mat4_look_anyup_rh(eye, dir, out);
#endif
}

RGINLINE void rg_mat4_look_anyup_rh(const rg_vec3* eye, const rg_vec3* dir, rg_mat4* out)
{
	f32 fx = dir->x;
	f32 fy = dir->y;
	f32 fz = dir->z;
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
	if (rg_absf(dir->y) > 0.999f)
	{
		sx = 0.0f;
		sy = fz;
		sz = -fy;
		len_sq = sy * sy + sz * sz;
	}
	else
	{
		sx = -fz;
		sy = 0.0f;
		sz = fx;
		len_sq = sx * sx + sz * sz;
	}

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

RGINLINE void rg_mat4_look_anyup_lh(const rg_vec3* eye, const rg_vec3* dir, rg_mat4* out)
{
	f32 fx = dir->x;
	f32 fy = dir->y;
	f32 fz = dir->z;
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
	if (rg_absf(dir->y) > 0.999f)
	{
		sx = 0.0f;
		sy = -fz;
		sz = fy;
		len_sq = sy * sy + sz * sz;
	}
	else
	{
		sx = fz;
		sy = 0.0f;
		sz = -fx;
		len_sq = sx * sx + sz * sz;
	}

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

RGINLINE void rg_mat4_persp_move_far(rg_mat4* proj, f32 delta_far)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_persp_move_far_lh_zo(proj, delta_far);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_persp_move_far_lh_no(proj, delta_far);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_persp_move_far_rh_zo(proj, delta_far);
#else
	rg_mat4_persp_move_far_rh_no(proj, delta_far);
#endif
}

RGINLINE void rg_mat4_persp_move_far_rh_no(rg_mat4* proj, f32 delta_far)
{
	f32 p22 = proj->m[10];
	f32 p32 = proj->m[14];

	f32 near_z = p32 / (p22 - 1.0f);
	f32 far_z = p32 / (p22 + 1.0f) + delta_far;
	f32 fn = 1.0f / (near_z - far_z);

	proj->m[10] = (far_z + near_z) * fn;
	proj->m[14] = 2.0f * near_z * far_z * fn;
}

RGINLINE void rg_mat4_persp_move_far_rh_zo(rg_mat4* proj, f32 delta_far)
{
	f32 p22 = proj->m[10];
	f32 p32 = proj->m[14];

	f32 near_z = p32 / p22;
	f32 far_z = p32 / (p22 + 1.0f) + delta_far;
	f32 fn = 1.0f / (near_z - far_z);

	proj->m[10] = far_z * fn;
	proj->m[14] = near_z * far_z * fn;
}

RGINLINE void rg_mat4_persp_move_far_lh_no(rg_mat4* proj, f32 delta_far)
{
	f32 p22 = -proj->m[10];
	f32 p32 = proj->m[14];

	f32 near_z = p32 / (p22 - 1.0f);
	f32 far_z = p32 / (p22 + 1.0f) + delta_far;
	f32 fn = 1.0f / (near_z - far_z);

	proj->m[10] = -(far_z + near_z) * fn;
	proj->m[14] = 2.0f * near_z * far_z * fn;
}

RGINLINE void rg_mat4_persp_move_far_lh_zo(rg_mat4* proj, f32 delta_far)
{
	f32 p22 = -proj->m[10];
	f32 p32 = proj->m[14];

	f32 near_z = p32 / p22;
	f32 far_z = p32 / (p22 + 1.0f) + delta_far;
	f32 fn = 1.0f / (near_z - far_z);

	proj->m[10] = -far_z * fn;
	proj->m[14] = near_z * far_z * fn;
}

RGINLINE void rg_mat4_persp_decomp(const rg_mat4* proj, f32* near_z, f32* far_z,
                                   f32* top, f32* bottom, f32* left, f32* right)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_persp_decomp_lh_zo(proj, near_z, far_z, top, bottom, left, right);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_persp_decomp_lh_no(proj, near_z, far_z, top, bottom, left, right);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_persp_decomp_rh_zo(proj, near_z, far_z, top, bottom, left, right);
#else
	rg_mat4_persp_decomp_rh_no(proj, near_z, far_z, top, bottom, left, right);
#endif
}

RGINLINE void rg_mat4_persp_decomp_rh_no(const rg_mat4* proj, f32* near_z, f32* far_z,
                                         f32* top, f32* bottom, f32* left, f32* right)
{
	f32 m00 = proj->m[0];
	f32 m11 = proj->m[5];
	f32 m20 = proj->m[8];
	f32 m21 = proj->m[9];
	f32 m22 = proj->m[10];
	f32 m32 = proj->m[14];

	f32 n = m32 / (m22 - 1.0f);
	f32 f = m32 / (m22 + 1.0f);

	f32 n_m11 = n / m11;
	f32 n_m00 = n / m00;

	*near_z = n;
	*far_z = f;
	*bottom = n_m11 * (m21 - 1.0f);
	*top = n_m11 * (m21 + 1.0f);
	*left = n_m00 * (m20 - 1.0f);
	*right = n_m00 * (m20 + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_rh_zo(const rg_mat4* proj, f32* near_z, f32* far_z,
                                         f32* top, f32* bottom, f32* left, f32* right)
{
	f32 m00 = proj->m[0];
	f32 m11 = proj->m[5];
	f32 m20 = proj->m[8];
	f32 m21 = proj->m[9];
	f32 m22 = proj->m[10];
	f32 m32 = proj->m[14];

	f32 n = m32 / m22;
	f32 f = m32 / (m22 + 1.0f);

	f32 n_m11 = n / m11;
	f32 n_m00 = n / m00;

	*near_z = n;
	*far_z = f;
	*bottom = n_m11 * (m21 - 1.0f);
	*top = n_m11 * (m21 + 1.0f);
	*left = n_m00 * (m20 - 1.0f);
	*right = n_m00 * (m20 + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_lh_no(const rg_mat4* proj, f32* near_z, f32* far_z,
                                         f32* top, f32* bottom, f32* left, f32* right)
{
	f32 m00 = proj->m[0];
	f32 m11 = proj->m[5];
	f32 m20 = proj->m[8];
	f32 m21 = proj->m[9];
	f32 m22 = -proj->m[10];
	f32 m32 = proj->m[14];

	f32 n = m32 / (m22 - 1.0f);
	f32 f = m32 / (m22 + 1.0f);

	f32 n_m11 = n / m11;
	f32 n_m00 = n / m00;

	*near_z = n;
	*far_z = f;
	*bottom = n_m11 * (m21 - 1.0f);
	*top = n_m11 * (m21 + 1.0f);
	*left = n_m00 * (m20 - 1.0f);
	*right = n_m00 * (m20 + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_lh_zo(const rg_mat4* proj, f32* near_z, f32* far_z,
                                         f32* top, f32* bottom, f32* left, f32* right)
{
	f32 m00 = proj->m[0];
	f32 m11 = proj->m[5];
	f32 m20 = proj->m[8];
	f32 m21 = proj->m[9];
	f32 m22 = -proj->m[10];
	f32 m32 = proj->m[14];

	f32 n = m32 / m22;
	f32 f = m32 / (m22 + 1.0f);

	f32 n_m11 = n / m11;
	f32 n_m00 = n / m00;

	*near_z = n;
	*far_z = f;
	*bottom = n_m11 * (m21 - 1.0f);
	*top = n_m11 * (m21 + 1.0f);
	*left = n_m00 * (m20 - 1.0f);
	*right = n_m00 * (m20 + 1.0f);
}

RGINLINE void rg_mat4_persp_decompv(const rg_mat4* proj, f32 dest[6])
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_persp_decompv_lh_zo(proj, dest);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_persp_decompv_lh_no(proj, dest);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_persp_decompv_rh_zo(proj, dest);
#else
	rg_mat4_persp_decompv_rh_no(proj, dest);
#endif
}

RGINLINE void rg_mat4_persp_decompv_rh_no(const rg_mat4* proj, f32 dest[6])
{
	rg_mat4_persp_decomp_rh_no(proj, &dest[0], &dest[1], &dest[2], &dest[3], &dest[4], &dest[5]);
}

RGINLINE void rg_mat4_persp_decompv_rh_zo(const rg_mat4* proj, f32 dest[6])
{
	rg_mat4_persp_decomp_rh_zo(proj, &dest[0], &dest[1], &dest[2], &dest[3], &dest[4], &dest[5]);
}

RGINLINE void rg_mat4_persp_decompv_lh_no(const rg_mat4* proj, f32 dest[6])
{
	rg_mat4_persp_decomp_lh_no(proj, &dest[0], &dest[1], &dest[2], &dest[3], &dest[4], &dest[5]);
}

RGINLINE void rg_mat4_persp_decompv_lh_zo(const rg_mat4* proj, f32 dest[6])
{
	rg_mat4_persp_decomp_lh_zo(proj, &dest[0], &dest[1], &dest[2], &dest[3], &dest[4], &dest[5]);
}

RGINLINE void rg_mat4_persp_decomp_x(const rg_mat4* proj, f32* left, f32* right)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_persp_decomp_x_lh_zo(proj, left, right);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_persp_decomp_x_lh_no(proj, left, right);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_persp_decomp_x_rh_zo(proj, left, right);
#else
	rg_mat4_persp_decomp_x_rh_no(proj, left, right);
#endif
}

RGINLINE void rg_mat4_persp_decomp_x_rh_no(const rg_mat4* proj, f32* left, f32* right)
{
	f32 m00 = proj->m[0];
	f32 m20 = proj->m[8];
	f32 m22 = proj->m[10];
	f32 near_z = proj->m[14] / (m22 - 1.0f);
	*left = near_z * (m20 - 1.0f) / m00;
	*right = near_z * (m20 + 1.0f) / m00;
}

RGINLINE void rg_mat4_persp_decomp_x_rh_zo(const rg_mat4* proj, f32* left, f32* right)
{
	f32 m00 = proj->m[0];
	f32 m20 = proj->m[8];
	f32 m22 = proj->m[10];
	f32 near_z = proj->m[14] / m22;
	*left = near_z * (m20 - 1.0f) / m00;
	*right = near_z * (m20 + 1.0f) / m00;
}

RGINLINE void rg_mat4_persp_decomp_x_lh_no(const rg_mat4* proj, f32* left, f32* right)
{
	f32 m00 = proj->m[0];
	f32 m20 = proj->m[8];
	f32 m22 = -proj->m[10];
	f32 near_z = proj->m[14] / (m22 - 1.0f);
	*left = near_z * (m20 - 1.0f) / m00;
	*right = near_z * (m20 + 1.0f) / m00;
}

RGINLINE void rg_mat4_persp_decomp_x_lh_zo(const rg_mat4* proj, f32* left, f32* right)
{
	f32 m00 = proj->m[0];
	f32 m20 = proj->m[8];
	f32 m22 = -proj->m[10];
	f32 near_z = proj->m[14] / m22;
	*left = near_z * (m20 - 1.0f) / m00;
	*right = near_z * (m20 + 1.0f) / m00;
}

RGINLINE void rg_mat4_persp_decomp_y(const rg_mat4* proj, f32* top, f32* bottom)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_persp_decomp_y_lh_zo(proj, top, bottom);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_persp_decomp_y_lh_no(proj, top, bottom);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_persp_decomp_y_rh_zo(proj, top, bottom);
#else
	rg_mat4_persp_decomp_y_rh_no(proj, top, bottom);
#endif
}

RGINLINE void rg_mat4_persp_decomp_y_rh_no(const rg_mat4* proj, f32* top, f32* bottom)
{
	f32 m21 = proj->m[9];
	f32 m11 = proj->m[5];
	f32 m22 = proj->m[10];
	f32 near_z = proj->m[14] / (m22 - 1.0f);
	*bottom = near_z * (m21 - 1.0f) / m11;
	*top = near_z * (m21 + 1.0f) / m11;
}

RGINLINE void rg_mat4_persp_decomp_y_rh_zo(const rg_mat4* proj, f32* top, f32* bottom)
{
	f32 m21 = proj->m[9];
	f32 m11 = proj->m[5];
	f32 m22 = proj->m[10];
	f32 near_z = proj->m[14] / m22;
	*bottom = near_z * (m21 - 1.0f) / m11;
	*top = near_z * (m21 + 1.0f) / m11;
}

RGINLINE void rg_mat4_persp_decomp_y_lh_no(const rg_mat4* proj, f32* top, f32* bottom)
{
	f32 m21 = proj->m[9];
	f32 m11 = proj->m[5];
	f32 m22 = -proj->m[10];
	f32 near_z = proj->m[14] / (m22 - 1.0f);
	*bottom = near_z * (m21 - 1.0f) / m11;
	*top = near_z * (m21 + 1.0f) / m11;
}

RGINLINE void rg_mat4_persp_decomp_y_lh_zo(const rg_mat4* proj, f32* top, f32* bottom)
{
	f32 m21 = proj->m[9];
	f32 m11 = proj->m[5];
	f32 m22 = -proj->m[10];
	f32 near_z = proj->m[14] / m22;
	*bottom = near_z * (m21 - 1.0f) / m11;
	*top = near_z * (m21 + 1.0f) / m11;
}

RGINLINE void rg_mat4_persp_decomp_z(const rg_mat4* proj, f32* near_z, f32* far_z)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_persp_decomp_z_lh_zo(proj, near_z, far_z);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_persp_decomp_z_lh_no(proj, near_z, far_z);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_persp_decomp_z_rh_zo(proj, near_z, far_z);
#else
	rg_mat4_persp_decomp_z_rh_no(proj, near_z, far_z);
#endif
}

RGINLINE void rg_mat4_persp_decomp_z_rh_no(const rg_mat4* proj, f32* near_z, f32* far_z)
{
	f32 m32 = proj->m[14];
	f32 m22 = proj->m[10];
	*near_z = m32 / (m22 - 1.0f);
	*far_z = m32 / (m22 + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_z_rh_zo(const rg_mat4* proj, f32* near_z, f32* far_z)
{
	f32 m32 = proj->m[14];
	f32 m22 = proj->m[10];
	*near_z = m32 / m22;
	*far_z = m32 / (m22 + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_z_lh_no(const rg_mat4* proj, f32* near_z, f32* far_z)
{
	f32 m32 = proj->m[14];
	f32 m22 = -proj->m[10];
	*near_z = m32 / (m22 - 1.0f);
	*far_z = m32 / (m22 + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_z_lh_zo(const rg_mat4* proj, f32* near_z, f32* far_z)
{
	f32 m32 = proj->m[14];
	f32 m22 = -proj->m[10];
	*near_z = m32 / m22;
	*far_z = m32 / (m22 + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_far(const rg_mat4* proj, f32* far_z)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_persp_decomp_far_lh_zo(proj, far_z);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_persp_decomp_far_lh_no(proj, far_z);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_persp_decomp_far_rh_zo(proj, far_z);
#else
	rg_mat4_persp_decomp_far_rh_no(proj, far_z);
#endif
}

RGINLINE void rg_mat4_persp_decomp_far_rh_no(const rg_mat4* proj, f32* far_z)
{
	*far_z = proj->m[14] / (proj->m[10] + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_far_rh_zo(const rg_mat4* proj, f32* far_z)
{
	*far_z = proj->m[14] / (proj->m[10] + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_far_lh_no(const rg_mat4* proj, f32* far_z)
{
	*far_z = proj->m[14] / (-proj->m[10] + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_far_lh_zo(const rg_mat4* proj, f32* far_z)
{
	*far_z = proj->m[14] / (-proj->m[10] + 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_near(const rg_mat4* proj, f32* near_z)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_persp_decomp_near_lh_zo(proj, near_z);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_persp_decomp_near_lh_no(proj, near_z);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_persp_decomp_near_rh_zo(proj, near_z);
#else
	rg_mat4_persp_decomp_near_rh_no(proj, near_z);
#endif
}

RGINLINE void rg_mat4_persp_decomp_near_rh_no(const rg_mat4* proj, f32* near_z)
{
	*near_z = proj->m[14] / (proj->m[10] - 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_near_rh_zo(const rg_mat4* proj, f32* near_z)
{
	*near_z = proj->m[14] / proj->m[10];
}

RGINLINE void rg_mat4_persp_decomp_near_lh_no(const rg_mat4* proj, f32* near_z)
{
	*near_z = proj->m[14] / (-proj->m[10] - 1.0f);
}

RGINLINE void rg_mat4_persp_decomp_near_lh_zo(const rg_mat4* proj, f32* near_z)
{
	*near_z = proj->m[14] / -proj->m[10];
}

RGINLINE f32 rg_mat4_persp_fovy(const rg_mat4* proj)
{
	return 2.0f * rg_atanf(1.0f / proj->m[5]);
}

RGINLINE f32 rg_mat4_persp_aspect(const rg_mat4* proj)
{
	return proj->m[5] / proj->m[0];
}

RGINLINE void rg_mat4_persp_sizes(const rg_mat4* proj, f32 fovy, rg_vec4* out)
{
#if RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_ZO
	rg_mat4_persp_sizes_lh_zo(proj, fovy, out);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_LH_NO
	rg_mat4_persp_sizes_lh_no(proj, fovy, out);
#elif RG_MATH_CLIP_CONTROL == RG_MATH_CLIP_CONTROL_RH_ZO
	rg_mat4_persp_sizes_rh_zo(proj, fovy, out);
#else
	rg_mat4_persp_sizes_rh_no(proj, fovy, out);
#endif
}

RGINLINE void rg_mat4_persp_sizes_rh_no(const rg_mat4* proj, f32 fovy, rg_vec4* out)
{
	f32 t = 2.0f * rg_tanf(fovy * 0.5f);
	f32 a = rg_mat4_persp_aspect(proj);
	f32 near_z, far_z;
	rg_mat4_persp_decomp_z_rh_no(proj, &near_z, &far_z);

	out->y = t * near_z;
	out->w = t * far_z;
	out->x = a * out->y;
	out->z = a * out->w;
}

RGINLINE void rg_mat4_persp_sizes_rh_zo(const rg_mat4* proj, f32 fovy, rg_vec4* out)
{
	f32 t = 2.0f * rg_tanf(fovy * 0.5f);
	f32 a = rg_mat4_persp_aspect(proj);
	f32 near_z, far_z;
	rg_mat4_persp_decomp_z_rh_zo(proj, &near_z, &far_z);

	out->y = t * near_z;
	out->w = t * far_z;
	out->x = a * out->y;
	out->z = a * out->w;
}

RGINLINE void rg_mat4_persp_sizes_lh_no(const rg_mat4* proj, f32 fovy, rg_vec4* out)
{
	f32 t = 2.0f * rg_tanf(fovy * 0.5f);
	f32 a = rg_mat4_persp_aspect(proj);
	f32 near_z, far_z;
	rg_mat4_persp_decomp_z_lh_no(proj, &near_z, &far_z);

	out->y = t * near_z;
	out->w = t * far_z;
	out->x = a * out->y;
	out->z = a * out->w;
}

RGINLINE void rg_mat4_persp_sizes_lh_zo(const rg_mat4* proj, f32 fovy, rg_vec4* out)
{
	f32 t = 2.0f * rg_tanf(fovy * 0.5f);
	f32 a = rg_mat4_persp_aspect(proj);
	f32 near_z, far_z;
	rg_mat4_persp_decomp_z_lh_zo(proj, &near_z, &far_z);

	out->y = t * near_z;
	out->w = t * far_z;
	out->x = a * out->y;
	out->z = a * out->w;
}

#endif // RG_MATH_CAM_H
