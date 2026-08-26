// rg_math_quat - quaternion operations
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides quaternion construction, conversion, interpolation, and transform helpers.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_QUAT_H
#define RG_MATH_QUAT_H

#include "rg_math_vec.h"
#include "rg_math_mat.h"

RG_MATH_EXTERN_C_BEGIN

// =============================================================================
// QUATERNION OPERATIONS - POINTER-BASED API
// =============================================================================

RGINLINE void rg_quat_set(rg_quat* q, f32 x, f32 y, f32 z, f32 w);
RGINLINE void rg_quat_init(rg_quat* q, f32 x, f32 y, f32 z, f32 w);
RGINLINE void rg_quat_copy(const rg_quat* q, rg_quat* out);
RGINLINE void rg_quat_identity(rg_quat* q);
RGINLINE void rg_quat_identity_array(rg_quat* q, size_t count);
RGINLINE void rg_quat_make(const f32* src, rg_quat* out);
RGINLINE void rg_quat_from_axis_angle(f32 angle_radians, const rg_vec3* axis, rg_quat* out);
RGINLINE void rg_quat_from_vecs(const rg_vec3* a, const rg_vec3* b, rg_quat* out);
RGINLINE void rg_quat_from_norm_pair_fast(const rg_vec3* a, const rg_vec3* b, rg_quat* out);
RGINLINE f32 rg_quat_dot(const rg_quat* a, const rg_quat* b);
RGINLINE f32 rg_quat_len(const rg_quat* q);
RGINLINE f32 rg_quat_norm(const rg_quat* q);
RGINLINE f32 rg_quat_real(const rg_quat* q);
RGINLINE void rg_quat_imag(const rg_quat* q, rg_vec3* out);
RGINLINE void rg_quat_imagn(const rg_quat* q, rg_vec3* out);
RGINLINE f32 rg_quat_imaglen(const rg_quat* q);
RGINLINE f32 rg_quat_angle(const rg_quat* q);
RGINLINE void rg_quat_axis(const rg_quat* q, rg_vec3* out);
RGINLINE void rg_quat_conjugate(const rg_quat* q, rg_quat* out);
RGINLINE void rg_quat_inv(const rg_quat* q, rg_quat* out);
RGINLINE void rg_quat_add(const rg_quat* a, const rg_quat* b, rg_quat* out);
RGINLINE void rg_quat_sub(const rg_quat* a, const rg_quat* b, rg_quat* out);
RGINLINE void rg_quat_mul(const rg_quat* a, const rg_quat* b, rg_quat* out);
RGINLINE void rg_quat_normalize(const rg_quat* q, rg_quat* out);
RGINLINE void rg_quat_normalize_to(const rg_quat* q, rg_quat* out);
RGINLINE void rg_quat_lerp(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out);
RGINLINE void rg_quat_lerpc(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out);
RGINLINE void rg_quat_nlerp(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out);
RGINLINE void rg_quat_slerp(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out);
RGINLINE void rg_quat_slerp_fast(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out);
RGINLINE void rg_quat_slerp_longest(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out);
RGINLINE void rg_quat_mat4(const rg_quat* q, rg_mat4* out);
RGINLINE void rg_quat_mat4t(const rg_quat* q, rg_mat4* out);
RGINLINE void rg_quat_mat3(const rg_quat* q, rg_mat3* out);
RGINLINE void rg_quat_mat3t(const rg_quat* q, rg_mat3* out);
RGINLINE void rg_quat_look(const rg_vec3* eye, const rg_quat* ori, rg_mat4* out);
RGINLINE void rg_quat_for(const rg_vec3* dir, const rg_vec3* up, rg_quat* out);
RGINLINE void rg_quat_forp(const rg_vec3* from, const rg_vec3* to, const rg_vec3* up, rg_quat* out);
RGINLINE void rg_quat_rotatev(const rg_quat* q, const rg_vec3* v, rg_vec3* out);
RGINLINE void rg_quat_rotate(const rg_mat4* m, const rg_quat* q, rg_mat4* out);
RGINLINE void rg_quat_rotate_at(const rg_mat4* m, const rg_quat* q, const rg_vec3* pivot, rg_mat4* out);
RGINLINE void rg_quat_rotate_atm(rg_mat4* out, const rg_quat* q, const rg_vec3* pivot);
RGINLINE void rg_quat_rotatev3(const rg_quat* q, const rg_vec3* v, rg_vec3* out);

RG_MATH_EXTERN_C_END

// =============================================================================
// Quaternion Implementation
// =============================================================================

#if defined(RG_MATH_SSE)
RGINLINE __m128 rg_math_quat_dot4_ps(__m128 a, __m128 b)
{
	__m128 mul = _mm_mul_ps(a, b);
	__m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
	__m128 sums = _mm_add_ps(mul, shuf);
	shuf = _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(0, 1, 2, 3));
	sums = _mm_add_ss(sums, shuf);
	return _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(0, 0, 0, 0));
}
#endif

RGINLINE void rg_quat_set(rg_quat* q, f32 x, f32 y, f32 z, f32 w)
{
	q->data[0] = x;
	q->data[1] = y;
	q->data[2] = z;
	q->data[3] = w;
}

RGINLINE void rg_quat_init(rg_quat* q, f32 x, f32 y, f32 z, f32 w)
{
	rg_quat_set(q, x, y, z, w);
}

RGINLINE void rg_quat_copy(const rg_quat* q, rg_quat* out)
{
#ifdef RG_MATH_SSE
	out->simd = q->simd;
#else
	out->x = q->x;
	out->y = q->y;
	out->z = q->z;
	out->w = q->w;
#endif
}

RGINLINE void rg_quat_identity(rg_quat* q)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	q->simd = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
#else
	q->x = 0.0f;
	q->y = 0.0f;
	q->z = 0.0f;
	q->w = 1.0f;
#endif
}

RGINLINE void rg_quat_identity_array(rg_quat* q, size_t count)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 identity = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
	if (count == 4)
	{
		q[0].simd = identity;
		q[1].simd = identity;
		q[2].simd = identity;
		q[3].simd = identity;
		return;
	}
	for (size_t i = 0; i < count; ++i)
	{
		q[i].simd = identity;
	}
#else
	for (size_t i = 0; i < count; ++i)
	{
		rg_quat_identity(&q[i]);
	}
#endif
}

RGINLINE void rg_quat_make(const f32* src, rg_quat* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	out->simd = _mm_loadu_ps(src);
#else
	out->x = src[0];
	out->y = src[1];
	out->z = src[2];
	out->w = src[3];
#endif
}

RGINLINE void rg_quat_from_axis_angle(f32 angle_radians, const rg_vec3* axis, rg_quat* out)
{
	f32 half = angle_radians * 0.5f;
	f32 s = rg_sinf(half);
	f32 c = rg_cosf(half);
	f32 ax = axis->x;
	f32 ay = axis->y;
	f32 az = axis->z;
#if !RG_MATH_MAX_PERF
	f32 len_sq = ax * ax + ay * ay + az * az;
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		rg_quat_identity(out);
		return;
	}
	f32 inv_len = 1.0f / rg_sqrtf(len_sq);
	ax *= inv_len;
	ay *= inv_len;
	az *= inv_len;
#endif
	out->x = ax * s;
	out->y = ay * s;
	out->z = az * s;
	out->w = c;
}

RGINLINE void rg_quat_from_vecs(const rg_vec3* a, const rg_vec3* b, rg_quat* out)
{
	f32 ax = a->x;
	f32 ay = a->y;
	f32 az = a->z;
	f32 bx = b->x;
	f32 by = b->y;
	f32 bz = b->z;
	f32 cos_theta = ax * bx + ay * by + az * bz;

	if (cos_theta < 1.0f - RG_EPSILON)
	{
		if (cos_theta <= -1.0f + RG_EPSILON)
		{
			rg_vec3 axis;
			rg_vec3_ortho(a, &axis);
			f32 len_sq = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
#ifndef RG_MATH_UNSAFE_NORMALIZE
			if (len_sq < RG_EPSILON * RG_EPSILON)
			{
				rg_quat_identity(out);
				return;
			}
#endif
#ifdef RG_MATH_FAST_NORMALIZE
			f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
			f32 inv_len = rg_rsqrtf(len_sq);
#endif
			out->x = axis.x * inv_len;
			out->y = axis.y * inv_len;
			out->z = axis.z * inv_len;
			out->w = 0.0f;
			return;
		}

		f32 x = ay * bz - az * by;
		f32 y = az * bx - ax * bz;
		f32 z = ax * by - ay * bx;
#if RG_MATH_MAX_PERF
		f32 w = 1.0f + cos_theta;
		f32 inv_s = rg_rsqrtf(w * 2.0f);
		out->x = x * inv_s;
		out->y = y * inv_s;
		out->z = z * inv_s;
		out->w = w * inv_s;
#else
		f32 w = 1.0f + cos_theta;
		f32 len_sq = x * x + y * y + z * z + w * w;
#ifndef RG_MATH_UNSAFE_NORMALIZE
		if (len_sq < RG_EPSILON * RG_EPSILON)
		{
			rg_quat_identity(out);
			return;
		}
#endif
#ifdef RG_MATH_FAST_NORMALIZE
		f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
		f32 inv_len = rg_rsqrtf(len_sq);
#endif
		out->x = x * inv_len;
		out->y = y * inv_len;
		out->z = z * inv_len;
		out->w = w * inv_len;
#endif
		return;
	}

	rg_quat_identity(out);
}

RGINLINE void rg_quat_from_norm_pair_fast(const rg_vec3* a, const rg_vec3* b, rg_quat* out)
{
	f32 ax = a->x;
	f32 ay = a->y;
	f32 az = a->z;
	f32 bx = b->x;
	f32 by = b->y;
	f32 bz = b->z;
	f32 x = ay * bz - az * by;
	f32 y = az * bx - ax * bz;
	f32 z = ax * by - ay * bx;
	f32 w = 1.0f + (ax * bx + ay * by + az * bz);
	f32 inv_s = rg_rsqrtf(w * 2.0f);
	out->x = x * inv_s;
	out->y = y * inv_s;
	out->z = z * inv_s;
	out->w = w * inv_s;
}

RGINLINE f32 rg_quat_dot(const rg_quat* a, const rg_quat* b)
{
#if defined(RG_MATH_SSE)
	return _mm_cvtss_f32(rg_math_quat_dot4_ps(a->simd, b->simd));
#else
	return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
#endif
}

RGINLINE f32 rg_quat_len(const rg_quat* q)
{
	f32 len_sq = rg_quat_dot(q, q);
	return rg_sqrtf(len_sq);
}

RGINLINE f32 rg_quat_norm(const rg_quat* q)
{
	return rg_quat_len(q);
}

RGINLINE f32 rg_quat_real(const rg_quat* q)
{
	return q->w;
}

RGINLINE void rg_quat_imag(const rg_quat* q, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	RG_VEC3_STORE(out, q->simd);
#else
	out->x = q->x;
	out->y = q->y;
	out->z = q->z;
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

RGINLINE void rg_quat_imagn(const rg_quat* q, rg_vec3* out)
{
#if defined(RG_MATH_SSE41) && defined(RG_MATH_FAST_NORMALIZE)
	__m128 v = q->simd;
	__m128 len_sq = _mm_dp_ps(v, v, 0x7F);
	__m128 inv_len = _mm_rsqrt_ps(len_sq);
	RG_VEC3_STORE(out, _mm_mul_ps(v, inv_len));
#else
	f32 len_sq = q->x * q->x + q->y * q->y + q->z * q->z;
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
	out->x = q->x * inv_len;
	out->y = q->y * inv_len;
	out->z = q->z * inv_len;
#endif
}

RGINLINE f32 rg_quat_imaglen(const rg_quat* q)
{
	f32 len_sq = q->x * q->x + q->y * q->y + q->z * q->z;
	return rg_sqrtf(len_sq);
}

RGINLINE f32 rg_quat_angle(const rg_quat* q)
{
	f32 imag_len_sq = q->x * q->x + q->y * q->y + q->z * q->z;
	return 2.0f * rg_atan2f(rg_sqrtf(imag_len_sq), q->w);
}

RGINLINE void rg_quat_axis(const rg_quat* q, rg_vec3* out)
{
	rg_quat_imagn(q, out);
}

RGINLINE void rg_quat_conjugate(const rg_quat* q, rg_quat* out)
{
#ifdef RG_MATH_SSE
	__m128 sign = _mm_castsi128_ps(_mm_set_epi32(0, (int)0x80000000u, (int)0x80000000u, (int)0x80000000u));
	out->simd = _mm_xor_ps(q->simd, sign);
#else
	out->x = -q->x;
	out->y = -q->y;
	out->z = -q->z;
	out->w = q->w;
#endif
}

RGINLINE void rg_quat_inv(const rg_quat* q, rg_quat* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 v = q->simd;
	__m128 len_sq = rg_math_quat_dot4_ps(v, v);
	__m128 sign = _mm_castsi128_ps(_mm_set_epi32(0, (int)0x80000000u, (int)0x80000000u, (int)0x80000000u));
	__m128 conj = _mm_xor_ps(v, sign);
	__m128 inv = _mm_rcp_ss(len_sq);
	inv = _mm_mul_ss(inv, _mm_sub_ss(_mm_set_ss(2.0f), _mm_mul_ss(len_sq, inv)));
	inv = _mm_shuffle_ps(inv, inv, _MM_SHUFFLE(0, 0, 0, 0));
	out->simd = _mm_mul_ps(conj, inv);
#else
	f32 len_sq = rg_quat_dot(q, q);
#if !RG_MATH_MAX_PERF
	RG_MATH_ASSERT(len_sq != 0.0f);
#endif
	f32 inv = 1.0f / len_sq;
#ifdef RG_MATH_SSE
	__m128 sign = _mm_castsi128_ps(_mm_set_epi32(0, (int)0x80000000u, (int)0x80000000u, (int)0x80000000u));
	__m128 conj = _mm_xor_ps(q->simd, sign);
	__m128 scale = _mm_set1_ps(inv);
	out->simd = _mm_mul_ps(conj, scale);
#else
	out->x = -q->x * inv;
	out->y = -q->y * inv;
	out->z = -q->z * inv;
	out->w = q->w * inv;
#endif
#endif
}

RGINLINE void rg_quat_add(const rg_quat* a, const rg_quat* b, rg_quat* out)
{
#ifdef RG_MATH_SSE
	out->simd = _mm_add_ps(a->simd, b->simd);
#else
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
	out->w = a->w + b->w;
#endif
}

RGINLINE void rg_quat_sub(const rg_quat* a, const rg_quat* b, rg_quat* out)
{
#ifdef RG_MATH_SSE
	out->simd = _mm_sub_ps(a->simd, b->simd);
#else
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
	out->w = a->w - b->w;
#endif
}

RGINLINE void rg_quat_mul(const rg_quat* a, const rg_quat* b, rg_quat* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = a->simd;
	__m128 bv = b->simd;
	__m128 sign_x = _mm_castsi128_ps(_mm_set_epi32((int)0x80000000u, 0, (int)0x80000000u, 0));
	__m128 sign_y = _mm_castsi128_ps(_mm_set_epi32((int)0x80000000u, (int)0x80000000u, 0, 0));
	__m128 sign_z = _mm_castsi128_ps(_mm_set_epi32((int)0x80000000u, 0, 0, (int)0x80000000u));

	__m128 x = _mm_xor_ps(_mm_shuffle_ps(av, av, _MM_SHUFFLE(0, 0, 0, 0)), sign_x);
	__m128 y = _mm_xor_ps(_mm_shuffle_ps(av, av, _MM_SHUFFLE(1, 1, 1, 1)), sign_y);
	__m128 z = _mm_xor_ps(_mm_shuffle_ps(av, av, _MM_SHUFFLE(2, 2, 2, 2)), sign_z);
	__m128 w = _mm_shuffle_ps(av, av, _MM_SHUFFLE(3, 3, 3, 3));

	__m128 r = _mm_mul_ps(w, _mm_shuffle_ps(bv, bv, _MM_SHUFFLE(3, 2, 1, 0)));
	r = RG_MATH_FMADD_PS(x, _mm_shuffle_ps(bv, bv, _MM_SHUFFLE(0, 1, 2, 3)), r);
	r = RG_MATH_FMADD_PS(y, _mm_shuffle_ps(bv, bv, _MM_SHUFFLE(1, 0, 3, 2)), r);
	r = RG_MATH_FMADD_PS(z, _mm_shuffle_ps(bv, bv, _MM_SHUFFLE(2, 3, 0, 1)), r);
	out->simd = r;
#else
	f32 ax = a->x;
	f32 ay = a->y;
	f32 az = a->z;
	f32 aw = a->w;
	f32 bx = b->x;
	f32 by = b->y;
	f32 bz = b->z;
	f32 bw = b->w;

	out->x = aw * bx + ax * bw + ay * bz - az * by;
	out->y = aw * by - ax * bz + ay * bw + az * bx;
	out->z = aw * bz + ax * by - ay * bx + az * bw;
	out->w = aw * bw - ax * bx - ay * by - az * bz;
#endif
}

RGINLINE void rg_quat_normalize(const rg_quat* q, rg_quat* out)
{
#ifdef RG_MATH_SSE
#if RG_MATH_MAX_PERF
	{
		__m128 v = q->simd;
		__m128 dot = rg_math_quat_dot4_ps(v, v);
#ifndef RG_MATH_UNSAFE_NORMALIZE
		f32 len_sq = _mm_cvtss_f32(dot);
		if (len_sq < RG_EPSILON * RG_EPSILON)
		{
			rg_quat_identity(out);
			return;
		}
#endif
#ifdef RG_MATH_FAST_NORMALIZE
		__m128 inv = _mm_rsqrt_ps(dot);
#else
		__m128 inv = _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(dot));
#endif
		out->simd = _mm_mul_ps(v, inv);
		return;
	}
#endif
#endif
	f32 len_sq = rg_quat_dot(q, q);
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		rg_quat_identity(out);
		return;
	}
#endif
#ifdef RG_MATH_FAST_NORMALIZE
	f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
	f32 inv_len = rg_rsqrtf(len_sq);
#endif
#ifdef RG_MATH_SSE
	__m128 inv = _mm_set1_ps(inv_len);
	out->simd = _mm_mul_ps(q->simd, inv);
#else
	out->x = q->x * inv_len;
	out->y = q->y * inv_len;
	out->z = q->z * inv_len;
	out->w = q->w * inv_len;
#endif
}

RGINLINE void rg_quat_normalize_to(const rg_quat* q, rg_quat* out)
{
	rg_quat_normalize(q, out);
}

RGINLINE void rg_quat_lerp(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out)
{
#ifdef RG_MATH_SSE
	__m128 av = a->simd;
	__m128 bv = b->simd;
	__m128 tv = _mm_set1_ps(t);
	out->simd = RG_MATH_FMADD_PS(_mm_sub_ps(bv, av), tv, av);
#else
	out->x = a->x + (b->x - a->x) * t;
	out->y = a->y + (b->y - a->y) * t;
	out->z = a->z + (b->z - a->z) * t;
	out->w = a->w + (b->w - a->w) * t;
#endif
}

RGINLINE void rg_quat_lerpc(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out)
{
	rg_quat_lerp(a, b, rg_clampf(t, 0.0f, 1.0f), out);
}

RGINLINE void rg_quat_nlerp(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = a->simd;
	__m128 bv = b->simd;
	__m128 dot = rg_math_quat_dot4_ps(av, bv);
	__m128 sign_all = _mm_castsi128_ps(_mm_set1_epi32((int)0x80000000u));
	__m128 sign = _mm_and_ps(_mm_cmplt_ps(dot, _mm_setzero_ps()), sign_all);
	bv = _mm_xor_ps(bv, sign);

	__m128 tv = _mm_set1_ps(t);
	__m128 r = RG_MATH_FMADD_PS(_mm_sub_ps(bv, av), tv, av);
	__m128 len_sq = rg_math_quat_dot4_ps(r, r);
	__m128 inv = _mm_rsqrt_ss(len_sq);
	inv = _mm_shuffle_ps(inv, inv, _MM_SHUFFLE(0, 0, 0, 0));
	out->simd = _mm_mul_ps(r, inv);
#else
	f32 ax = a->x;
	f32 ay = a->y;
	f32 az = a->z;
	f32 aw = a->w;
	f32 bx = b->x;
	f32 by = b->y;
	f32 bz = b->z;
	f32 bw = b->w;

	f32 dot = ax * bx + ay * by + az * bz + aw * bw;
	if (dot < 0.0f)
	{
		bx = -bx;
		by = -by;
		bz = -bz;
		bw = -bw;
	}

	f32 x = ax + (bx - ax) * t;
	f32 y = ay + (by - ay) * t;
	f32 z = az + (bz - az) * t;
	f32 w = aw + (bw - aw) * t;
	f32 len_sq = x * x + y * y + z * z + w * w;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		rg_quat_identity(out);
		return;
	}
#endif
#ifdef RG_MATH_SSE
	__m128 len = _mm_set_ss(len_sq);
	__m128 inv = _mm_rsqrt_ss(len);
#if !RG_MATH_MAX_PERF
	__m128 half = _mm_set_ss(0.5f);
	__m128 three = _mm_set_ss(3.0f);
	inv = _mm_mul_ss(_mm_mul_ss(half, inv), _mm_sub_ss(three, _mm_mul_ss(_mm_mul_ss(len, inv), inv)));
#endif
	f32 inv_len = _mm_cvtss_f32(inv);
#else
	f32 inv_len = rg_rsqrtf(len_sq);
#endif
	out->x = x * inv_len;
	out->y = y * inv_len;
	out->z = z * inv_len;
	out->w = w * inv_len;
#endif
}

RGINLINE void rg_quat_slerp(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = a->simd;
	__m128 bv = b->simd;
	__m128 dotv = rg_math_quat_dot4_ps(av, bv);
	f32 cos_theta = _mm_cvtss_f32(dotv);
	__m128 signed_bv = bv;

	if (cos_theta < 0.0f)
	{
		cos_theta = -cos_theta;
		__m128 sign = _mm_castsi128_ps(_mm_set1_epi32((int)0x80000000u));
		signed_bv = _mm_xor_ps(bv, sign);
	}

	if (cos_theta > 0.9995f)
	{
		__m128 tv = _mm_set1_ps(t);
		__m128 r = RG_MATH_FMADD_PS(_mm_sub_ps(signed_bv, av), tv, av);
		__m128 lenv = rg_math_quat_dot4_ps(r, r);
#ifndef RG_MATH_UNSAFE_NORMALIZE
		f32 len_sq = _mm_cvtss_f32(lenv);
		if (len_sq < RG_EPSILON * RG_EPSILON)
		{
			rg_quat_identity(out);
			return;
		}
#endif
#ifdef RG_MATH_FAST_NORMALIZE
		__m128 inv_len = _mm_rsqrt_ps(lenv);
#else
		__m128 inv_len = _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(lenv));
#endif
		out->simd = _mm_mul_ps(r, inv_len);
		return;
	}

	f32 sin_theta_sq = 1.0f - cos_theta * cos_theta;
	f32 inv_sin_theta = 1.0f / rg_sqrtf(sin_theta_sq);
	f32 angle = rg_acosf(cos_theta);
	f32 sin_t;
	f32 cos_t;
	rg_sincosf(t * angle, &sin_t, &cos_t);
	f32 scale1 = sin_t * inv_sin_theta;
	f32 scale0 = cos_t - cos_theta * scale1;
	out->simd = RG_MATH_FMADD_PS(signed_bv, _mm_set1_ps(scale1), _mm_mul_ps(av, _mm_set1_ps(scale0)));
#else
	f32 ax = a->x;
	f32 ay = a->y;
	f32 az = a->z;
	f32 aw = a->w;
	f32 bx = b->x;
	f32 by = b->y;
	f32 bz = b->z;
	f32 bw = b->w;
	f32 cos_theta = ax * bx + ay * by + az * bz + aw * bw;

	if (cos_theta < 0.0f)
	{
		cos_theta = -cos_theta;
		bx = -bx;
		by = -by;
		bz = -bz;
		bw = -bw;
	}

	if (cos_theta > 0.9995f)
	{
		f32 x = ax + (bx - ax) * t;
		f32 y = ay + (by - ay) * t;
		f32 z = az + (bz - az) * t;
		f32 w = aw + (bw - aw) * t;
		f32 len_sq = x * x + y * y + z * z + w * w;
#ifndef RG_MATH_UNSAFE_NORMALIZE
		if (len_sq < RG_EPSILON * RG_EPSILON)
		{
			rg_quat_identity(out);
			return;
		}
#endif
#ifdef RG_MATH_FAST_NORMALIZE
		f32 inv_len = rg_rsqrtf_fast(len_sq);
#else
		f32 inv_len = rg_rsqrtf(len_sq);
#endif
		out->x = x * inv_len;
		out->y = y * inv_len;
		out->z = z * inv_len;
		out->w = w * inv_len;
		return;
	}

#if !RG_MATH_MAX_PERF
	cos_theta = rg_clampf(cos_theta, -1.0f, 1.0f);
#endif
#if RG_MATH_MAX_PERF
	f32 sin_theta_sq = 1.0f - cos_theta * cos_theta;
	f32 inv_sin_theta = 1.0f / rg_sqrtf(sin_theta_sq);
	f32 angle = rg_acosf(cos_theta);
	f32 sin_t;
	f32 cos_t;
	rg_sincosf(t * angle, &sin_t, &cos_t);
	f32 scale1 = sin_t * inv_sin_theta;
	f32 scale0 = cos_t - cos_theta * scale1;
	out->x = ax * scale0 + bx * scale1;
	out->y = ay * scale0 + by * scale1;
	out->z = az * scale0 + bz * scale1;
	out->w = aw * scale0 + bw * scale1;
#else
	f32 angle = rg_acosf(cos_theta);
	f32 w0 = rg_sinf((1.0f - t) * angle);
	f32 w1 = rg_sinf(t * angle);
	f32 x = ax * w0 + bx * w1;
	f32 y = ay * w0 + by * w1;
	f32 z = az * w0 + bz * w1;
	f32 w = aw * w0 + bw * w1;
	f32 len_sq = x * x + y * y + z * z + w * w;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		rg_quat_identity(out);
		return;
	}
#endif
	f32 inv_len = rg_rsqrtf(len_sq);
	out->x = x * inv_len;
	out->y = y * inv_len;
	out->z = z * inv_len;
	out->w = w * inv_len;
#endif
#endif
}

RGINLINE void rg_quat_slerp_fast(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = a->simd;
	__m128 bv = b->simd;
	__m128 dotv = rg_math_quat_dot4_ps(av, bv);
	f32 cos_theta = _mm_cvtss_f32(dotv);
	__m128 signed_bv = bv;

	if (cos_theta < 0.0f)
	{
		cos_theta = -cos_theta;
		__m128 sign = _mm_castsi128_ps(_mm_set1_epi32((int)0x80000000u));
		signed_bv = _mm_xor_ps(bv, sign);
	}

	if (cos_theta > 0.9995f)
	{
		__m128 tv = _mm_set1_ps(t);
		__m128 r = RG_MATH_FMADD_PS(_mm_sub_ps(signed_bv, av), tv, av);
		__m128 lenv = rg_math_quat_dot4_ps(r, r);
		out->simd = _mm_mul_ps(r, _mm_rsqrt_ps(lenv));
		return;
	}

	f32 inv_sin_theta = rg_rsqrtf_fast(1.0f - cos_theta * cos_theta);
	f32 angle = rg_acosf_fast(cos_theta);
	f32 sin_t;
	f32 cos_t;
#if RG_MATH_MAX_PERF
	rg_sincosf(t * angle, &sin_t, &cos_t);
#else
	f32 t_angle = t * angle;
	sin_t = rg_sinf_fast(t_angle);
	cos_t = rg_cosf_fast(t_angle);
#endif
	f32 scale1 = sin_t * inv_sin_theta;
	f32 scale0 = cos_t - cos_theta * scale1;
	out->simd = RG_MATH_FMADD_PS(signed_bv, _mm_set1_ps(scale1), _mm_mul_ps(av, _mm_set1_ps(scale0)));
#else
	f32 ax = a->x;
	f32 ay = a->y;
	f32 az = a->z;
	f32 aw = a->w;
	f32 bx = b->x;
	f32 by = b->y;
	f32 bz = b->z;
	f32 bw = b->w;
	f32 cos_theta = ax * bx + ay * by + az * bz + aw * bw;

	if (cos_theta < 0.0f)
	{
		cos_theta = -cos_theta;
		bx = -bx;
		by = -by;
		bz = -bz;
		bw = -bw;
	}

	if (cos_theta > 0.9995f)
	{
		f32 x = ax + (bx - ax) * t;
		f32 y = ay + (by - ay) * t;
		f32 z = az + (bz - az) * t;
		f32 w = aw + (bw - aw) * t;
		f32 inv_len = rg_rsqrtf_fast(x * x + y * y + z * z + w * w);
		out->x = x * inv_len;
		out->y = y * inv_len;
		out->z = z * inv_len;
		out->w = w * inv_len;
		return;
	}

	f32 inv_sin_theta = rg_rsqrtf_fast(1.0f - cos_theta * cos_theta);
	f32 angle = rg_acosf_fast(cos_theta);
	f32 sin_t;
	f32 cos_t;
#if RG_MATH_MAX_PERF
	rg_sincosf(t * angle, &sin_t, &cos_t);
#else
	f32 t_angle = t * angle;
	sin_t = rg_sinf_fast(t_angle);
	cos_t = rg_cosf_fast(t_angle);
#endif
	f32 scale1 = sin_t * inv_sin_theta;
	f32 scale0 = cos_t - cos_theta * scale1;
	out->x = ax * scale0 + bx * scale1;
	out->y = ay * scale0 + by * scale1;
	out->z = az * scale0 + bz * scale1;
	out->w = aw * scale0 + bw * scale1;
#endif
}

RGINLINE void rg_quat_slerp_longest(const rg_quat* a, const rg_quat* b, f32 t, rg_quat* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 av = a->simd;
	__m128 bv = b->simd;
	f32 cos_theta = _mm_cvtss_f32(rg_math_quat_dot4_ps(av, bv));

	if (rg_absf(cos_theta) >= 1.0f)
	{
		out->simd = av;
		return;
	}

	__m128 path_av = av;
	if (!(cos_theta < 0.0f))
	{
		cos_theta = -cos_theta;
		__m128 sign = _mm_castsi128_ps(_mm_set1_epi32((int)0x80000000u));
		path_av = _mm_xor_ps(av, sign);
	}

	f32 sin_theta_sq = 1.0f - cos_theta * cos_theta;
	if (sin_theta_sq < 0.000001f)
	{
		rg_quat_lerp(a, b, t, out);
		return;
	}

	f32 angle = rg_acosf(cos_theta);
	__m128 sin_sqv = _mm_set_ss(sin_theta_sq);
	__m128 inv_sinv = _mm_rsqrt_ss(sin_sqv);
	inv_sinv = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), inv_sinv),
	                      _mm_sub_ss(_mm_set_ss(3.0f), _mm_mul_ss(_mm_mul_ss(sin_sqv, inv_sinv), inv_sinv)));
	f32 inv_sin_theta = _mm_cvtss_f32(inv_sinv);
	f32 t_angle = t * angle;
	f32 sin_t;
	f32 cos_t;
	rg_sincosf(t_angle, &sin_t, &cos_t);
	f32 scale1 = sin_t * inv_sin_theta;
	f32 scale0 = cos_t - cos_theta * scale1;
	out->simd = RG_MATH_FMADD_PS(bv, _mm_set1_ps(scale1), _mm_mul_ps(path_av, _mm_set1_ps(scale0)));
#else
	f32 ax = a->x;
	f32 ay = a->y;
	f32 az = a->z;
	f32 aw = a->w;
	f32 bx = b->x;
	f32 by = b->y;
	f32 bz = b->z;
	f32 bw = b->w;
	f32 cos_theta = ax * bx + ay * by + az * bz + aw * bw;

	if (rg_absf(cos_theta) >= 1.0f)
	{
		out->x = ax;
		out->y = ay;
		out->z = az;
		out->w = aw;
		return;
	}

	if (!(cos_theta < 0.0f))
	{
		cos_theta = -cos_theta;
		ax = -ax;
		ay = -ay;
		az = -az;
		aw = -aw;
	}

	f32 sin_theta_sq = 1.0f - cos_theta * cos_theta;
	if (sin_theta_sq < 0.000001f)
	{
		rg_quat_lerp(a, b, t, out);
		return;
	}

	f32 angle = rg_acosf(cos_theta);
#if RG_MATH_MAX_PERF
	f32 inv_sin_theta = rg_rsqrtf(sin_theta_sq);
	inv_sin_theta *= 1.5f - 0.5f * sin_theta_sq * inv_sin_theta * inv_sin_theta;
	f32 t_angle = t * angle;
	f32 sin_t;
	f32 cos_t;
	rg_sincosf(t_angle, &sin_t, &cos_t);
	f32 scale1 = sin_t * inv_sin_theta;
	f32 scale0 = cos_t - cos_theta * scale1;
	out->x = ax * scale0 + bx * scale1;
	out->y = ay * scale0 + by * scale1;
	out->z = az * scale0 + bz * scale1;
	out->w = aw * scale0 + bw * scale1;
#else
	f32 w0 = rg_sinf((1.0f - t) * angle);
	f32 w1 = rg_sinf(t * angle);
	f32 x = ax * w0 + bx * w1;
	f32 y = ay * w0 + by * w1;
	f32 z = az * w0 + bz * w1;
	f32 w = aw * w0 + bw * w1;
	f32 len_sq = x * x + y * y + z * z + w * w;
#ifndef RG_MATH_UNSAFE_NORMALIZE
	if (len_sq < RG_EPSILON * RG_EPSILON)
	{
		rg_quat_identity(out);
		return;
	}
#endif
	f32 inv_len = rg_rsqrtf(len_sq);
	out->x = x * inv_len;
	out->y = y * inv_len;
	out->z = z * inv_len;
	out->w = w * inv_len;
#endif
#endif
}

RGINLINE void rg_quat_mat4(const rg_quat* q, rg_mat4* out)
{
	rg_mat4_from_quat(out, q);
}

RGINLINE void rg_quat_mat4t(const rg_quat* q, rg_mat4* out)
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
		rg_mat4_identity(out);
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
	out->m[1] = xy - wz;
	out->m[2] = xz + wy;
	out->m[3] = 0.0f;

	out->m[4] = xy + wz;
	out->m[5] = 1.0f - xx - zz;
	out->m[6] = yz - wx;
	out->m[7] = 0.0f;

	out->m[8] = xz - wy;
	out->m[9] = yz + wx;
	out->m[10] = 1.0f - xx - yy;
	out->m[11] = 0.0f;

	out->m[12] = 0.0f;
	out->m[13] = 0.0f;
	out->m[14] = 0.0f;
	out->m[15] = 1.0f;
}

RGINLINE void rg_quat_mat3(const rg_quat* q, rg_mat3* out)
{
	rg_mat3_from_quat(q, out);
}

RGINLINE void rg_quat_mat3t(const rg_quat* q, rg_mat3* out)
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
	out->m[1] = xy - wz;
	out->m[2] = xz + wy;
	out->m[3] = 0.0f;

	out->m[4] = xy + wz;
	out->m[5] = 1.0f - xx - zz;
	out->m[6] = yz - wx;
	out->m[7] = 0.0f;

	out->m[8] = xz - wy;
	out->m[9] = yz + wx;
	out->m[10] = 1.0f - xx - yy;
	out->m[11] = 0.0f;
}

RGINLINE void rg_quat_look(const rg_vec3* eye, const rg_quat* ori, rg_mat4* out)
{
	rg_quat_mat4t(ori, out);

	f32 ex = eye->x;
	f32 ey = eye->y;
	f32 ez = eye->z;
	out->m[12] = -(out->m[0] * ex + out->m[4] * ey + out->m[8] * ez);
	out->m[13] = -(out->m[1] * ex + out->m[5] * ey + out->m[9] * ez);
	out->m[14] = -(out->m[2] * ex + out->m[6] * ey + out->m[10] * ez);
	out->m[15] = 1.0f;
}

RGINLINE void rg_quat_for(const rg_vec3* dir, const rg_vec3* up, rg_quat* out)
{
	rg_mat3 m;
	rg_vec3 forward;
	rg_vec3 right;
	rg_vec3 new_up;

	rg_vec3_normalize(dir, &forward);
	rg_vec3_negate(&forward);

	rg_vec3_crossn(up, &forward, &right);
	rg_vec3_cross(&forward, &right, &new_up);

	m.m[0] = right.x;
	m.m[1] = right.y;
	m.m[2] = right.z;
	m.m[3] = 0.0f;

	m.m[4] = new_up.x;
	m.m[5] = new_up.y;
	m.m[6] = new_up.z;
	m.m[7] = 0.0f;

	m.m[8] = forward.x;
	m.m[9] = forward.y;
	m.m[10] = forward.z;
	m.m[11] = 0.0f;

	rg_mat3_quat(&m, out);
}

RGINLINE void rg_quat_forp(const rg_vec3* from, const rg_vec3* to, const rg_vec3* up, rg_quat* out)
{
	rg_vec3 dir;
	rg_vec3_sub(to, from, &dir);
	rg_quat_for(&dir, up, out);
}

RGINLINE void rg_quat_rotatev(const rg_quat* q, const rg_vec3* v, rg_vec3* out)
{
#if RG_MATH_MAX_PERF
	rg_quat_rotatev3(q, v, out);
#else
	rg_quat qn;
	rg_quat_normalize(q, &qn);
	rg_quat_rotatev3(&qn, v, out);
#endif
}

RGINLINE void rg_quat_rotate(const rg_mat4* m, const rg_quat* q, rg_mat4* out)
{
	rg_mat4 rot;
	rg_quat_mat4(q, &rot);
	rg_mat4_mul_rot(m, &rot, out);
}

RGINLINE void rg_quat_rotate_at(const rg_mat4* m, const rg_quat* q, const rg_vec3* pivot, rg_mat4* out)
{
	rg_mat4 rot;
	rg_quat_rotate_atm(&rot, q, pivot);
	rg_mat4_mul_affine(m, &rot, out);
}

RGINLINE void rg_quat_rotate_atm(rg_mat4* out, const rg_quat* q, const rg_vec3* pivot)
{
	rg_quat_mat4(q, out);

	f32 px = pivot->x;
	f32 py = pivot->y;
	f32 pz = pivot->z;
	out->m[12] = px - (out->m[0] * px + out->m[4] * py + out->m[8] * pz);
	out->m[13] = py - (out->m[1] * px + out->m[5] * py + out->m[9] * pz);
	out->m[14] = pz - (out->m[2] * px + out->m[6] * py + out->m[10] * pz);
	out->m[15] = 1.0f;
}

RGINLINE void rg_quat_rotatev3(const rg_quat* q, const rg_vec3* v, rg_vec3* out)
{
#if defined(RG_MATH_SSE) && RG_MATH_MAX_PERF
	__m128 qv = q->simd;
	__m128 vv = RG_VEC3_LOAD(v);
	__m128 q_yzx = _mm_shuffle_ps(qv, qv, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 q_zxy = _mm_shuffle_ps(qv, qv, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 v_yzx = _mm_shuffle_ps(vv, vv, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 v_zxy = _mm_shuffle_ps(vv, vv, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 t = _mm_sub_ps(_mm_mul_ps(q_yzx, v_zxy), _mm_mul_ps(q_zxy, v_yzx));
	t = _mm_add_ps(t, t);
	__m128 t_yzx = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 t_zxy = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 c = _mm_sub_ps(_mm_mul_ps(q_yzx, t_zxy), _mm_mul_ps(q_zxy, t_yzx));
	__m128 qw = _mm_shuffle_ps(qv, qv, _MM_SHUFFLE(3, 3, 3, 3));
	RG_VEC3_STORE(out, _mm_add_ps(RG_MATH_FMADD_PS(qw, t, vv), c));
#else
	f32 qx = q->x;
	f32 qy = q->y;
	f32 qz = q->z;
	f32 qw = q->w;

	f32 vx = v->x;
	f32 vy = v->y;
	f32 vz = v->z;

	f32 tx = 2.0f * (qy * vz - qz * vy);
	f32 ty = 2.0f * (qz * vx - qx * vz);
	f32 tz = 2.0f * (qx * vy - qy * vx);

	out->x = vx + qw * tx + (qy * tz - qz * ty);
	out->y = vy + qw * ty + (qz * tx - qx * tz);
	out->z = vz + qw * tz + (qx * ty - qy * tx);
#if !RG_MATH_MAX_PERF
	out->_pad = 0.0f;
#endif
#endif
}

// =============================================================================

#endif // RG_MATH_QUAT_H
