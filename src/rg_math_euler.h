// rg_math_euler - Euler angle helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides ordered Euler-angle conversion for matrices and quaternions.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_EULER_H
#define RG_MATH_EULER_H

#include "rg_math_mat.h"
#include "rg_math_quat.h"

RG_MATH_EXTERN_C_BEGIN

// =============================================================================
// EULER ANGLES
// =============================================================================

typedef enum rg_euler_seq
{
	RG_EULER_XYZ = (0 << 0) | (1 << 2) | (2 << 4),
	RG_EULER_XZY = (0 << 0) | (2 << 2) | (1 << 4),
	RG_EULER_YZX = (1 << 0) | (2 << 2) | (0 << 4),
	RG_EULER_YXZ = (1 << 0) | (0 << 2) | (2 << 4),
	RG_EULER_ZXY = (2 << 0) | (0 << 2) | (1 << 4),
	RG_EULER_ZYX = (2 << 0) | (1 << 2) | (0 << 4)
} rg_euler_seq;

RGINLINE rg_euler_seq rg_euler_order(const int order[3]);
RGINLINE void rg_mat4_euler_angles(const rg_mat4* m, rg_vec3* out);
RGINLINE void rg_mat4_euler_angles_fast(const rg_mat4* m, rg_vec3* out);
RGINLINE void rg_mat4_euler(const rg_vec3* angles, rg_mat4* out);
RGINLINE void rg_mat4_euler_fast(const rg_vec3* angles, rg_mat4* out);
RGINLINE void rg_mat4_euler_xyz(const rg_vec3* angles, rg_mat4* out);
RGINLINE void rg_mat4_euler_xyz_fast(const rg_vec3* angles, rg_mat4* out);
RGINLINE void rg_mat4_euler_xzy(const rg_vec3* angles, rg_mat4* out);
RGINLINE void rg_mat4_euler_yxz(const rg_vec3* angles, rg_mat4* out);
RGINLINE void rg_mat4_euler_yzx(const rg_vec3* angles, rg_mat4* out);
RGINLINE void rg_mat4_euler_zxy(const rg_vec3* angles, rg_mat4* out);
RGINLINE void rg_mat4_euler_zyx(const rg_vec3* angles, rg_mat4* out);
RGINLINE void rg_mat4_euler_by_order(const rg_vec3* angles, rg_euler_seq order, rg_mat4* out);

RGINLINE void rg_quat_from_euler(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_xyz(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_xzy(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_yxz(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_yzx(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_zxy(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_zyx(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_lh(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_xyz_lh(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_xzy_lh(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_yxz_lh(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_yzx_lh(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_zxy_lh(const rg_vec3* angles, rg_quat* out);
RGINLINE void rg_quat_from_euler_zyx_lh(const rg_vec3* angles, rg_quat* out);

RG_MATH_EXTERN_C_END

// =============================================================================
// Euler Implementation
// =============================================================================

RGINLINE rg_euler_seq rg_euler_order(const int order[3])
{
	return (rg_euler_seq)(order[0] << 0 | order[1] << 2 | order[2] << 4);
}

RGINLINE void rg_mat4_euler_angles(const rg_mat4* m, rg_vec3* out)
{
	f32 m00 = m->m[0];
	f32 m10 = m->m[4];
	f32 m20 = m->m[8];
	f32 m01 = m->m[1];
	f32 m11 = m->m[5];
	f32 m21 = m->m[9];
	f32 m22 = m->m[10];

	f32 theta_x, theta_y, theta_z;

	if (m20 < 1.0f)
	{
		if (m20 > -1.0f)
		{
			theta_y = rg_asinf(m20);
			theta_x = rg_atan2f(-m21, m22);
			theta_z = rg_atan2f(-m10, m00);
		}
		else
		{
			theta_y = -RG_PI * 0.5f;
			theta_x = -rg_atan2f(m01, m11);
			theta_z = 0.0f;
		}
	}
	else
	{
		theta_y = RG_PI * 0.5f;
		theta_x = rg_atan2f(m01, m11);
		theta_z = 0.0f;
	}

	out->x = theta_x;
	out->y = theta_y;
	out->z = theta_z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_mat4_euler_angles_fast(const rg_mat4* m, rg_vec3* out)
{
	f32 m00 = m->m[0];
	f32 m10 = m->m[4];
	f32 m20 = m->m[8];
	f32 m01 = m->m[1];
	f32 m11 = m->m[5];
	f32 m21 = m->m[9];
	f32 m22 = m->m[10];

	f32 theta_x, theta_y, theta_z;

	if (m20 < 1.0f)
	{
		if (m20 > -1.0f)
		{
			theta_y = rg_asinf_fast(m20);
			theta_x = rg_atan2f_fast(-m21, m22);
			theta_z = rg_atan2f_fast(-m10, m00);
		}
		else
		{
			theta_y = -RG_PI * 0.5f;
			theta_x = -rg_atan2f_fast(m01, m11);
			theta_z = 0.0f;
		}
	}
	else
	{
		theta_y = RG_PI * 0.5f;
		theta_x = rg_atan2f_fast(m01, m11);
		theta_z = 0.0f;
	}

	out->x = theta_x;
	out->y = theta_y;
	out->z = theta_z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
}

RGINLINE void rg_mat4_euler(const rg_vec3* angles, rg_mat4* out)
{
	rg_mat4_euler_xyz(angles, out);
}

#if RG_MATH_MAX_PERF && defined(RG_MATH_SSE)
#define RG__EULER_SINCOS(angles, sx, cx, sy, cy, sz, cz)           \
	do {                                                           \
		__m128 rg__angles = _mm_setr_ps((angles)->x, (angles)->y,  \
		                                (angles)->z, 0.0f);        \
		__m128 rg__sin_angles;                                     \
		__m128 rg__cos_angles;                                     \
		rg_sincosf4(rg__angles, &rg__sin_angles, &rg__cos_angles); \
		RG_ALIGN16 f32 rg__s[4];                                   \
		RG_ALIGN16 f32 rg__c[4];                                   \
		_mm_store_ps(rg__s, rg__sin_angles);                       \
		_mm_store_ps(rg__c, rg__cos_angles);                       \
		(sx) = rg__s[0];                                           \
		(sy) = rg__s[1];                                           \
		(sz) = rg__s[2];                                           \
		(cx) = rg__c[0];                                           \
		(cy) = rg__c[1];                                           \
		(cz) = rg__c[2];                                           \
	} while (0)
#else
#define RG__EULER_SINCOS(angles, sx, cx, sy, cy, sz, cz) \
	do {                                                 \
		rg_sincosf((angles)->x, &(sx), &(cx));           \
		rg_sincosf((angles)->y, &(sy), &(cy));           \
		rg_sincosf((angles)->z, &(sz), &(cz));           \
	} while (0)
#endif

RGINLINE void rgi_mat4_euler_xyz_sincos(rg_mat4* out, f32 sx, f32 cx, f32 sy, f32 cy, f32 sz, f32 cz)
{
	f32 czsx = cz * sx;
	f32 cxcz = cx * cz;
	f32 sysz = sy * sz;

	out->m[0] = cy * cz;
	out->m[1] = czsx * sy + cx * sz;
	out->m[2] = -cxcz * sy + sx * sz;
	out->m[4] = -cy * sz;
	out->m[5] = cxcz - sx * sysz;
	out->m[6] = czsx + cx * sysz;
	out->m[8] = sy;
	out->m[9] = -cy * sx;
	out->m[10] = cx * cy;
	out->m[3] = 0.0f;
	out->m[7] = 0.0f;
	out->m[11] = 0.0f;
	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = 0.0f;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_euler_xyz(const rg_vec3* angles, rg_mat4* out)
{
#if RG_MATH_MAX_PERF && defined(RG_MATH_SSE)
	f32 sx = 0.0f;
	f32 cx = 0.0f;
	f32 sy = 0.0f;
	f32 cy = 0.0f;
	f32 sz = 0.0f;
	f32 cz = 0.0f;
	RG__EULER_SINCOS(angles, sx, cx, sy, cy, sz, cz);
	rgi_mat4_euler_xyz_sincos(out, sx, cx, sy, cy, sz, cz);
#else
	f32 sx = sinf(angles->x);
	f32 cx = cosf(angles->x);
	f32 sy = sinf(angles->y);
	f32 cy = cosf(angles->y);
	f32 sz = sinf(angles->z);
	f32 cz = cosf(angles->z);

	f32 czsx = cz * sx;
	f32 cxcz = cx * cz;
	f32 sysz = sy * sz;

	out->m[0] = cy * cz;
	out->m[1] = czsx * sy + cx * sz;
	out->m[2] = -cxcz * sy + sx * sz;
	out->m[4] = -cy * sz;
	out->m[5] = cxcz - sx * sysz;
	out->m[6] = czsx + cx * sysz;
	out->m[8] = sy;
	out->m[9] = -cy * sx;
	out->m[10] = cx * cy;
	out->m[3] = 0.0f;
	out->m[7] = 0.0f;
	out->m[11] = 0.0f;
	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = 0.0f;
	out->m[15] = 1.0f;
#endif
}

RGINLINE void rg_mat4_euler_fast(const rg_vec3* angles, rg_mat4* out)
{
	rg_mat4_euler_xyz_fast(angles, out);
}

RGINLINE void rg_mat4_euler_xyz_fast(const rg_vec3* angles, rg_mat4* out)
{
	f32 sx = 0.0f;
	f32 cx = 0.0f;
	f32 sy = 0.0f;
	f32 cy = 0.0f;
	f32 sz = 0.0f;
	f32 cz = 0.0f;
	RG__EULER_SINCOS(angles, sx, cx, sy, cy, sz, cz);
	rgi_mat4_euler_xyz_sincos(out, sx, cx, sy, cy, sz, cz);
}

RGINLINE void rg_mat4_euler_xzy(const rg_vec3* angles, rg_mat4* out)
{
	f32 sx = 0.0f;
	f32 cx = 0.0f;
	f32 sy = 0.0f;
	f32 cy = 0.0f;
	f32 sz = 0.0f;
	f32 cz = 0.0f;
	RG__EULER_SINCOS(angles, sx, cx, sy, cy, sz, cz);

	f32 sxsy = sx * sy;
	f32 cysx = cy * sx;
	f32 cxsy = cx * sy;
	f32 cxcy = cx * cy;

	out->m[0] = cy * cz;
	out->m[1] = sxsy + cxcy * sz;
	out->m[2] = -cxsy + cysx * sz;
	out->m[3] = 0.0f;

	out->m[4] = -sz;
	out->m[5] = cx * cz;
	out->m[6] = cz * sx;
	out->m[7] = 0.0f;

	out->m[8] = cz * sy;
	out->m[9] = -cysx + cxsy * sz;
	out->m[10] = cxcy + sxsy * sz;
	out->m[11] = 0.0f;

	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = 0.0f;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_euler_yxz(const rg_vec3* angles, rg_mat4* out)
{
	f32 sx = 0.0f;
	f32 cx = 0.0f;
	f32 sy = 0.0f;
	f32 cy = 0.0f;
	f32 sz = 0.0f;
	f32 cz = 0.0f;
	RG__EULER_SINCOS(angles, sx, cx, sy, cy, sz, cz);

	f32 cycz = cy * cz;
	f32 sysz = sy * sz;
	f32 czsy = cz * sy;
	f32 cysz = cy * sz;

	out->m[0] = cycz + sx * sysz;
	out->m[1] = cx * sz;
	out->m[2] = -czsy + cysz * sx;
	out->m[3] = 0.0f;

	out->m[4] = -cysz + czsy * sx;
	out->m[5] = cx * cz;
	out->m[6] = cycz * sx + sysz;
	out->m[7] = 0.0f;

	out->m[8] = cx * sy;
	out->m[9] = -sx;
	out->m[10] = cx * cy;
	out->m[11] = 0.0f;

	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = 0.0f;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_euler_yzx(const rg_vec3* angles, rg_mat4* out)
{
	f32 sx = 0.0f;
	f32 cx = 0.0f;
	f32 sy = 0.0f;
	f32 cy = 0.0f;
	f32 sz = 0.0f;
	f32 cz = 0.0f;
	RG__EULER_SINCOS(angles, sx, cx, sy, cy, sz, cz);

	f32 sxsy = sx * sy;
	f32 cxcy = cx * cy;
	f32 cysx = cy * sx;
	f32 cxsy = cx * sy;

	out->m[0] = cy * cz;
	out->m[1] = sz;
	out->m[2] = -cz * sy;
	out->m[3] = 0.0f;

	out->m[4] = sxsy - cxcy * sz;
	out->m[5] = cx * cz;
	out->m[6] = cysx + cxsy * sz;
	out->m[7] = 0.0f;

	out->m[8] = cxsy + cysx * sz;
	out->m[9] = -cz * sx;
	out->m[10] = cxcy - sxsy * sz;
	out->m[11] = 0.0f;

	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = 0.0f;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_euler_zxy(const rg_vec3* angles, rg_mat4* out)
{
	f32 sx = 0.0f;
	f32 cx = 0.0f;
	f32 sy = 0.0f;
	f32 cy = 0.0f;
	f32 sz = 0.0f;
	f32 cz = 0.0f;
	RG__EULER_SINCOS(angles, sx, cx, sy, cy, sz, cz);

	f32 cycz = cy * cz;
	f32 sxsy = sx * sy;
	f32 cysz = cy * sz;

	out->m[0] = cycz - sxsy * sz;
	out->m[1] = cz * sxsy + cysz;
	out->m[2] = -cx * sy;
	out->m[3] = 0.0f;

	out->m[4] = -cx * sz;
	out->m[5] = cx * cz;
	out->m[6] = sx;
	out->m[7] = 0.0f;

	out->m[8] = cz * sy + cysz * sx;
	out->m[9] = -cycz * sx + sy * sz;
	out->m[10] = cx * cy;
	out->m[11] = 0.0f;

	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = 0.0f;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_euler_zyx(const rg_vec3* angles, rg_mat4* out)
{
	f32 sx = 0.0f;
	f32 cx = 0.0f;
	f32 sy = 0.0f;
	f32 cy = 0.0f;
	f32 sz = 0.0f;
	f32 cz = 0.0f;
	RG__EULER_SINCOS(angles, sx, cx, sy, cy, sz, cz);

	f32 czsx = cz * sx;
	f32 cxcz = cx * cz;
	f32 sysz = sy * sz;

	out->m[0] = cy * cz;
	out->m[1] = cy * sz;
	out->m[2] = -sy;
	out->m[3] = 0.0f;

	out->m[4] = czsx * sy - cx * sz;
	out->m[5] = cxcz + sx * sysz;
	out->m[6] = cy * sx;
	out->m[7] = 0.0f;

	out->m[8] = cxcz * sy + sx * sz;
	out->m[9] = -czsx + cx * sysz;
	out->m[10] = cx * cy;
	out->m[11] = 0.0f;

	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = 0.0f;
	out->m[15] = 1.0f;
}

RGINLINE void rg_mat4_euler_by_order(const rg_vec3* angles, rg_euler_seq order, rg_mat4* out)
{
	f32 sx = 0.0f;
	f32 cx = 0.0f;
	f32 sy = 0.0f;
	f32 cy = 0.0f;
	f32 sz = 0.0f;
	f32 cz = 0.0f;
	RG__EULER_SINCOS(angles, sx, cx, sy, cy, sz, cz);

	f32 cycz = cy * cz;
	f32 cysz = cy * sz;
	f32 cysx = cy * sx;
	f32 cxcy = cx * cy;
	f32 czsy = cz * sy;
	f32 cxcz = cx * cz;
	f32 czsx = cz * sx;
	f32 cxsz = cx * sz;
	f32 sysz = sy * sz;

	switch (order)
	{
		case RG_EULER_XZY:
			out->m[0] = cycz;
			out->m[1] = sx * sy + cx * cysz;
			out->m[2] = -cx * sy + cysx * sz;
			out->m[4] = -sz;
			out->m[5] = cxcz;
			out->m[6] = czsx;
			out->m[8] = czsy;
			out->m[9] = -cysx + cx * sysz;
			out->m[10] = cxcy + sx * sysz;
			break;
		case RG_EULER_XYZ:
			out->m[0] = cycz;
			out->m[1] = czsx * sy + cxsz;
			out->m[2] = -cx * czsy + sx * sz;
			out->m[4] = -cysz;
			out->m[5] = cxcz - sx * sysz;
			out->m[6] = czsx + cx * sysz;
			out->m[8] = sy;
			out->m[9] = -cysx;
			out->m[10] = cxcy;
			break;
		case RG_EULER_YXZ:
			out->m[0] = cycz + sx * sysz;
			out->m[1] = cxsz;
			out->m[2] = -czsy + cysx * sz;
			out->m[4] = czsx * sy - cysz;
			out->m[5] = cxcz;
			out->m[6] = cycz * sx + sysz;
			out->m[8] = cx * sy;
			out->m[9] = -sx;
			out->m[10] = cxcy;
			break;
		case RG_EULER_YZX:
			out->m[0] = cycz;
			out->m[1] = sz;
			out->m[2] = -czsy;
			out->m[4] = sx * sy - cx * cysz;
			out->m[5] = cxcz;
			out->m[6] = cysx + cx * sysz;
			out->m[8] = cx * sy + cysx * sz;
			out->m[9] = -czsx;
			out->m[10] = cxcy - sx * sysz;
			break;
		case RG_EULER_ZXY:
			out->m[0] = cycz - sx * sysz;
			out->m[1] = czsx * sy + cysz;
			out->m[2] = -cx * sy;
			out->m[4] = -cxsz;
			out->m[5] = cxcz;
			out->m[6] = sx;
			out->m[8] = czsy + cysx * sz;
			out->m[9] = -cycz * sx + sysz;
			out->m[10] = cxcy;
			break;
		case RG_EULER_ZYX:
			out->m[0] = cycz;
			out->m[1] = cysz;
			out->m[2] = -sy;
			out->m[4] = czsx * sy - cxsz;
			out->m[5] = cxcz + sx * sysz;
			out->m[6] = cysx;
			out->m[8] = cx * czsy + sx * sz;
			out->m[9] = -czsx + cx * sysz;
			out->m[10] = cxcy;
			break;
	}

	out->m[3] = 0.0f;
	out->m[7] = 0.0f;
	out->m[11] = 0.0f;
	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = 0.0f;
	out->m[15] = 1.0f;
}

#undef RG__EULER_SINCOS

RGINLINE void rg_quat_from_euler(const rg_vec3* angles, rg_quat* out)
{
	rg_quat_from_euler_xyz(angles, out);
}

#if RG_MATH_MAX_PERF && defined(RG_MATH_SSE)
#define RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc)           \
	do {                                                                \
		__m128 rg__half_angles = _mm_setr_ps((angles)->x * 0.5f,        \
		                                     (angles)->y * 0.5f,        \
		                                     (angles)->z * 0.5f, 0.0f); \
		__m128 rg__sin_angles;                                          \
		__m128 rg__cos_angles;                                          \
		rg_sincosf4(rg__half_angles, &rg__sin_angles, &rg__cos_angles); \
		RG_ALIGN16 f32 rg__s[4];                                        \
		RG_ALIGN16 f32 rg__c[4];                                        \
		_mm_store_ps(rg__s, rg__sin_angles);                            \
		_mm_store_ps(rg__c, rg__cos_angles);                            \
		(xs) = rg__s[0];                                                \
		(ys) = rg__s[1];                                                \
		(zs) = rg__s[2];                                                \
		(xc) = rg__c[0];                                                \
		(yc) = rg__c[1];                                                \
		(zc) = rg__c[2];                                                \
	} while (0)
#else
#define RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc) \
	do {                                                      \
		rg_sincosf((angles)->x * 0.5f, &(xs), &(xc));         \
		rg_sincosf((angles)->y * 0.5f, &(ys), &(yc));         \
		rg_sincosf((angles)->z * 0.5f, &(zs), &(zc));         \
	} while (0)
#endif

RGINLINE void rg_quat_from_euler_xyz(const rg_vec3* angles, rg_quat* out)
{
#if RG_MATH_MAX_PERF && defined(RG_MATH_SSE)
	__m128 half_angles = _mm_setr_ps(angles->x * 0.5f, angles->y * 0.5f, angles->z * 0.5f, 0.0f);
	__m128 sin_angles;
	__m128 cos_angles;
	rg_sincosf4(half_angles, &sin_angles, &cos_angles);

	RG_ALIGN16 f32 s[4];
	RG_ALIGN16 f32 c[4];
	_mm_store_ps(s, sin_angles);
	_mm_store_ps(c, cos_angles);

	f32 xcys = c[0] * s[1];
	f32 xsyc = s[0] * c[1];
	f32 xcyc = c[0] * c[1];
	f32 xsys = s[0] * s[1];

	out->x = xcys * s[2] + xsyc * c[2];
	out->y = xcys * c[2] - xsyc * s[2];
	out->z = xcyc * s[2] + xsys * c[2];
	out->w = xcyc * c[2] - xsys * s[2];
#else
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	rg_sincosf(angles->x * 0.5f, &xs, &xc);
	rg_sincosf(angles->y * 0.5f, &ys, &yc);
	rg_sincosf(angles->z * 0.5f, &zs, &zc);

	f32 xcys = xc * ys;
	f32 xsyc = xs * yc;
	f32 xcyc = xc * yc;
	f32 xsys = xs * ys;

	out->x = xcys * zs + xsyc * zc;
	out->y = xcys * zc - xsyc * zs;
	out->z = xcyc * zs + xsys * zc;
	out->w = xcyc * zc - xsys * zs;
#endif
}

RGINLINE void rg_quat_from_euler_xzy(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	out->x = -xc * zs * ys + xs * zc * yc;
	out->y = xc * zc * ys - xs * zs * yc;
	out->z = xc * zs * yc + xs * zc * ys;
	out->w = xc * zc * yc + xs * zs * ys;
}

RGINLINE void rg_quat_from_euler_yxz(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	out->x = yc * xs * zc + ys * xc * zs;
	out->y = -yc * xs * zs + ys * xc * zc;
	out->z = yc * xc * zs - ys * xs * zc;
	out->w = yc * xc * zc + ys * xs * zs;
}

RGINLINE void rg_quat_from_euler_yzx(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	out->x = yc * zc * xs + ys * zs * xc;
	out->y = yc * zs * xs + ys * zc * xc;
	out->z = yc * zs * xc - ys * zc * xs;
	out->w = yc * zc * xc - ys * zs * xs;
}

RGINLINE void rg_quat_from_euler_zxy(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	out->x = zc * xs * yc - zs * xc * ys;
	out->y = zc * xc * ys + zs * xs * yc;
	out->z = zc * xs * ys + zs * xc * yc;
	out->w = zc * xc * yc - zs * xs * ys;
}

RGINLINE void rg_quat_from_euler_zyx(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	out->x = zc * yc * xs - zs * ys * xc;
	out->y = zc * ys * xc + zs * yc * xs;
	out->z = -zc * ys * xs + zs * yc * xc;
	out->w = zc * yc * xc + zs * ys * xs;
}

RGINLINE void rg_quat_from_euler_lh(const rg_vec3* angles, rg_quat* out)
{
	rg_quat_from_euler_xyz_lh(angles, out);
}

RGINLINE void rg_quat_from_euler_xyz_lh(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	zs = -zs;

	f32 xcys = xc * ys;
	f32 xsyc = xs * yc;
	f32 xcyc = xc * yc;
	f32 xsys = xs * ys;

	out->x = xcys * zs + xsyc * zc;
	out->y = xcys * zc - xsyc * zs;
	out->z = xcyc * zs + xsys * zc;
	out->w = xcyc * zc - xsys * zs;
}

RGINLINE void rg_quat_from_euler_xzy_lh(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	zs = -zs;

	out->x = -xc * zs * ys + xs * zc * yc;
	out->y = xc * zc * ys - xs * zs * yc;
	out->z = xc * zs * yc + xs * zc * ys;
	out->w = xc * zc * yc + xs * zs * ys;
}

RGINLINE void rg_quat_from_euler_yxz_lh(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	zs = -zs;

	out->x = yc * xs * zc + ys * xc * zs;
	out->y = -yc * xs * zs + ys * xc * zc;
	out->z = yc * xc * zs - ys * xs * zc;
	out->w = yc * xc * zc + ys * xs * zs;
}

RGINLINE void rg_quat_from_euler_yzx_lh(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	zs = -zs;

	out->x = yc * zc * xs + ys * zs * xc;
	out->y = yc * zs * xs + ys * zc * xc;
	out->z = yc * zs * xc - ys * zc * xs;
	out->w = yc * zc * xc - ys * zs * xs;
}

RGINLINE void rg_quat_from_euler_zxy_lh(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	zs = -zs;

	out->x = zc * xs * yc - zs * xc * ys;
	out->y = zc * xc * ys + zs * xs * yc;
	out->z = zc * xs * ys + zs * xc * yc;
	out->w = zc * xc * yc - zs * xs * ys;
}

RGINLINE void rg_quat_from_euler_zyx_lh(const rg_vec3* angles, rg_quat* out)
{
	f32 xs = 0.0f;
	f32 xc = 0.0f;
	f32 ys = 0.0f;
	f32 yc = 0.0f;
	f32 zs = 0.0f;
	f32 zc = 0.0f;
	RG__EULER_HALF_SINCOS(angles, xs, xc, ys, yc, zs, zc);

	zs = -zs;

	out->x = zc * yc * xs - zs * ys * xc;
	out->y = zc * ys * xc + zs * yc * xs;
	out->z = -zc * ys * xs + zs * yc * xc;
	out->w = zc * yc * xc + zs * ys * xs;
}

#undef RG__EULER_HALF_SINCOS

#endif // RG_MATH_EULER_H
