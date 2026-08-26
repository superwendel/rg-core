// rg_math_noise - perlin noise helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides periodic and non-periodic Perlin noise in two through four dimensions.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_NOISE_H
#define RG_MATH_NOISE_H

#include "rg_math_vec.h"

RG_MATH_EXTERN_C_BEGIN

// =============================================================================
// NOISE HELPERS
// =============================================================================

RGINLINE f32 rg_perlin_vec4(const rg_vec4* point);
RGINLINE f32 rg_perlin_vec3(const rg_vec3* point);
RGINLINE f32 rg_perlin_vec2(const rg_vec2* point);

RG_MATH_EXTERN_C_END

// =============================================================================
// IMPLEMENTATION
// =============================================================================

#define RG_NOISE_FRACT_MAX 0.999999940395355224609375f

RGINLINE f32 rg_noise_mod289(f32 x)
{
	return x - rg_floorf(x * (1.0f / 289.0f)) * 289.0f;
}

RGINLINE void rg_noise_vec2_add(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x = a->x + b->x;
	out->y = a->y + b->y;
}

RGINLINE void rg_noise_vec2_sub(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
}

RGINLINE void rg_noise_vec2_mul(const rg_vec2* a, const rg_vec2* b, rg_vec2* out)
{
	out->x = a->x * b->x;
	out->y = a->y * b->y;
}

RGINLINE void rg_noise_vec2_scale(const rg_vec2* v, f32 s, rg_vec2* out)
{
	out->x = v->x * s;
	out->y = v->y * s;
}

RGINLINE void rg_noise_vec2_adds(const rg_vec2* v, f32 s, rg_vec2* out)
{
	out->x = v->x + s;
	out->y = v->y + s;
}

RGINLINE void rg_noise_vec2_subs(const rg_vec2* v, f32 s, rg_vec2* out)
{
	out->x = v->x - s;
	out->y = v->y - s;
}

RGINLINE void rg_noise_vec2_mods(const rg_vec2* v, f32 s, rg_vec2* out)
{
	out->x = rg_fmodf(v->x, s);
	out->y = rg_fmodf(v->y, s);
}

RGINLINE void rg_noise_vec2_floor(const rg_vec2* v, rg_vec2* out)
{
	out->x = rg_floorf(v->x);
	out->y = rg_floorf(v->y);
}

RGINLINE void rg_noise_vec2_fract(const rg_vec2* v, rg_vec2* out)
{
	f32 fx = v->x - rg_floorf(v->x);
	f32 fy = v->y - rg_floorf(v->y);
	out->x = rg_minf(fx, RG_NOISE_FRACT_MAX);
	out->y = rg_minf(fy, RG_NOISE_FRACT_MAX);
}

RGINLINE void rg_noise_vec2_lerp(const rg_vec2* a, const rg_vec2* b, f32 t, rg_vec2* out)
{
	out->x = a->x + (b->x - a->x) * t;
	out->y = a->y + (b->y - a->y) * t;
}

RGINLINE f32 rg_noise_vec2_dot(const rg_vec2* a, const rg_vec2* b)
{
	return a->x * b->x + a->y * b->y;
}

RGINLINE void rg_noise_vec3_add(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
}

RGINLINE void rg_noise_vec3_sub(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
}

RGINLINE void rg_noise_vec3_mul(const rg_vec3* a, const rg_vec3* b, rg_vec3* out)
{
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
}

RGINLINE void rg_noise_vec3_scale(const rg_vec3* v, f32 s, rg_vec3* out)
{
	out->x = v->x * s;
	out->y = v->y * s;
	out->z = v->z * s;
}

RGINLINE void rg_noise_vec3_adds(const rg_vec3* v, f32 s, rg_vec3* out)
{
	out->x = v->x + s;
	out->y = v->y + s;
	out->z = v->z + s;
}

RGINLINE void rg_noise_vec3_subs(const rg_vec3* v, f32 s, rg_vec3* out)
{
	out->x = v->x - s;
	out->y = v->y - s;
	out->z = v->z - s;
}

RGINLINE void rg_noise_vec3_mods(const rg_vec3* v, f32 s, rg_vec3* out)
{
	out->x = rg_fmodf(v->x, s);
	out->y = rg_fmodf(v->y, s);
	out->z = rg_fmodf(v->z, s);
}

RGINLINE void rg_noise_vec3_floor(const rg_vec3* v, rg_vec3* out)
{
	out->x = rg_floorf(v->x);
	out->y = rg_floorf(v->y);
	out->z = rg_floorf(v->z);
}

RGINLINE void rg_noise_vec3_fract(const rg_vec3* v, rg_vec3* out)
{
	f32 fx = v->x - rg_floorf(v->x);
	f32 fy = v->y - rg_floorf(v->y);
	f32 fz = v->z - rg_floorf(v->z);
	out->x = rg_minf(fx, RG_NOISE_FRACT_MAX);
	out->y = rg_minf(fy, RG_NOISE_FRACT_MAX);
	out->z = rg_minf(fz, RG_NOISE_FRACT_MAX);
}

RGINLINE void rg_noise_vec3_lerp(const rg_vec3* a, const rg_vec3* b, f32 t, rg_vec3* out)
{
	out->x = a->x + (b->x - a->x) * t;
	out->y = a->y + (b->y - a->y) * t;
	out->z = a->z + (b->z - a->z) * t;
}

RGINLINE f32 rg_noise_vec3_dot(const rg_vec3* a, const rg_vec3* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

RGINLINE void rg_noise_vec4_add(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
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

RGINLINE void rg_noise_vec4_sub(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
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

RGINLINE void rg_noise_vec4_mul(const rg_vec4* a, const rg_vec4* b, rg_vec4* out)
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

RGINLINE void rg_noise_vec4_scale(const rg_vec4* v, f32 s, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, _mm_mul_ps(RG_VEC4_LOAD(v), _mm_set1_ps(s)));
#else
	out->x = v->x * s;
	out->y = v->y * s;
	out->z = v->z * s;
	out->w = v->w * s;
#endif
}

RGINLINE void rg_noise_vec4_divs(const rg_vec4* v, f32 s, rg_vec4* out)
{
	f32 inv = 1.0f / s;
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, _mm_mul_ps(RG_VEC4_LOAD(v), _mm_set1_ps(inv)));
#else
	out->x = v->x * inv;
	out->y = v->y * inv;
	out->z = v->z * inv;
	out->w = v->w * inv;
#endif
}

RGINLINE void rg_noise_vec4_adds(const rg_vec4* v, f32 s, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, _mm_add_ps(RG_VEC4_LOAD(v), _mm_set1_ps(s)));
#else
	out->x = v->x + s;
	out->y = v->y + s;
	out->z = v->z + s;
	out->w = v->w + s;
#endif
}

RGINLINE void rg_noise_vec4_subs(const rg_vec4* v, f32 s, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(out, _mm_sub_ps(RG_VEC4_LOAD(v), _mm_set1_ps(s)));
#else
	out->x = v->x - s;
	out->y = v->y - s;
	out->z = v->z - s;
	out->w = v->w - s;
#endif
}

RGINLINE void rg_noise_vec4_mods(const rg_vec4* v, f32 s, rg_vec4* out)
{
	out->x = rg_fmodf(v->x, s);
	out->y = rg_fmodf(v->y, s);
	out->z = rg_fmodf(v->z, s);
	out->w = rg_fmodf(v->w, s);
}

RGINLINE void rg_noise_vec4_floor(const rg_vec4* v, rg_vec4* out)
{
#ifdef RG_MATH_SSE41
	RG_VEC4_STORE(out, _mm_floor_ps(RG_VEC4_LOAD(v)));
#else
	out->x = rg_floorf(v->x);
	out->y = rg_floorf(v->y);
	out->z = rg_floorf(v->z);
	out->w = rg_floorf(v->w);
#endif
}

RGINLINE void rg_noise_vec4_fract(const rg_vec4* v, rg_vec4* out)
{
#ifdef RG_MATH_SSE41
	__m128 val = RG_VEC4_LOAD(v);
	__m128 fract = _mm_sub_ps(val, _mm_floor_ps(val));
	RG_VEC4_STORE(out, _mm_min_ps(fract, _mm_set1_ps(RG_NOISE_FRACT_MAX)));
#else
	f32 fx = v->x - rg_floorf(v->x);
	f32 fy = v->y - rg_floorf(v->y);
	f32 fz = v->z - rg_floorf(v->z);
	f32 fw = v->w - rg_floorf(v->w);
	out->x = rg_minf(fx, RG_NOISE_FRACT_MAX);
	out->y = rg_minf(fy, RG_NOISE_FRACT_MAX);
	out->z = rg_minf(fz, RG_NOISE_FRACT_MAX);
	out->w = rg_minf(fw, RG_NOISE_FRACT_MAX);
#endif
}

RGINLINE void rg_noise_vec4_abs(const rg_vec4* v, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 sign = _mm_set1_ps(-0.0f);
	RG_VEC4_STORE(out, _mm_andnot_ps(sign, RG_VEC4_LOAD(v)));
#else
	out->x = rg_absf(v->x);
	out->y = rg_absf(v->y);
	out->z = rg_absf(v->z);
	out->w = rg_absf(v->w);
#endif
}

RGINLINE void rg_noise_vec4_fill(rg_vec4* v, f32 s)
{
#ifdef RG_MATH_SSE
	RG_VEC4_STORE(v, _mm_set1_ps(s));
#else
	v->x = s;
	v->y = s;
	v->z = s;
	v->w = s;
#endif
}

RGINLINE void rg_noise_vec4_zero(rg_vec4* v)
{
	rg_noise_vec4_fill(v, 0.0f);
}

RGINLINE void rg_noise_vec4_step(const rg_vec4* edge, const rg_vec4* x, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 cmp = _mm_cmplt_ps(RG_VEC4_LOAD(x), RG_VEC4_LOAD(edge));
	RG_VEC4_STORE(out, _mm_andnot_ps(cmp, _mm_set1_ps(1.0f)));
#else
	out->x = (x->x < edge->x) ? 0.0f : 1.0f;
	out->y = (x->y < edge->y) ? 0.0f : 1.0f;
	out->z = (x->z < edge->z) ? 0.0f : 1.0f;
	out->w = (x->w < edge->w) ? 0.0f : 1.0f;
#endif
}

RGINLINE void rg_noise_vec4_stepr(const rg_vec4* edge, f32 x, rg_vec4* out)
{
#ifdef RG_MATH_SSE
	__m128 cmp = _mm_cmplt_ps(_mm_set1_ps(x), RG_VEC4_LOAD(edge));
	RG_VEC4_STORE(out, _mm_andnot_ps(cmp, _mm_set1_ps(1.0f)));
#else
	out->x = (x < edge->x) ? 0.0f : 1.0f;
	out->y = (x < edge->y) ? 0.0f : 1.0f;
	out->z = (x < edge->z) ? 0.0f : 1.0f;
	out->w = (x < edge->w) ? 0.0f : 1.0f;
#endif
}

RGINLINE void rg_noise_vec4_lerp(const rg_vec4* a, const rg_vec4* b, f32 t, rg_vec4* out)
{
	out->x = a->x + (b->x - a->x) * t;
	out->y = a->y + (b->y - a->y) * t;
	out->z = a->z + (b->z - a->z) * t;
	out->w = a->w + (b->w - a->w) * t;
}

RGINLINE f32 rg_noise_vec4_dot(const rg_vec4* a, const rg_vec4* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

RGINLINE void rg_noise_permute_vec4(const rg_vec4* x, rg_vec4* dest)
{
	dest->x = rg_noise_mod289((x->x * 34.0f + 1.0f) * x->x);
	dest->y = rg_noise_mod289((x->y * 34.0f + 1.0f) * x->y);
	dest->z = rg_noise_mod289((x->z * 34.0f + 1.0f) * x->z);
	dest->w = rg_noise_mod289((x->w * 34.0f + 1.0f) * x->w);
}

RGINLINE void rg_noise_fade_vec4(const rg_vec4* t, rg_vec4* dest)
{
	f32 x2 = t->x * t->x;
	f32 y2 = t->y * t->y;
	f32 z2 = t->z * t->z;
	f32 w2 = t->w * t->w;

	f32 x3 = x2 * t->x;
	f32 y3 = y2 * t->y;
	f32 z3 = z2 * t->z;
	f32 w3 = w2 * t->w;

	dest->x = x3 * (t->x * (t->x * 6.0f - 15.0f) + 10.0f);
	dest->y = y3 * (t->y * (t->y * 6.0f - 15.0f) + 10.0f);
	dest->z = z3 * (t->z * (t->z * 6.0f - 15.0f) + 10.0f);
	dest->w = w3 * (t->w * (t->w * 6.0f - 15.0f) + 10.0f);
}

RGINLINE void rg_noise_fade_vec3(const rg_vec3* t, rg_vec3* dest)
{
	f32 x2 = t->x * t->x;
	f32 y2 = t->y * t->y;
	f32 z2 = t->z * t->z;

	f32 x3 = x2 * t->x;
	f32 y3 = y2 * t->y;
	f32 z3 = z2 * t->z;

	dest->x = x3 * (t->x * (t->x * 6.0f - 15.0f) + 10.0f);
	dest->y = y3 * (t->y * (t->y * 6.0f - 15.0f) + 10.0f);
	dest->z = z3 * (t->z * (t->z * 6.0f - 15.0f) + 10.0f);
}

RGINLINE void rg_noise_fade_vec2(const rg_vec2* t, rg_vec2* dest)
{
	f32 x2 = t->x * t->x;
	f32 y2 = t->y * t->y;

	f32 x3 = x2 * t->x;
	f32 y3 = y2 * t->y;

	dest->x = x3 * (t->x * (t->x * 6.0f - 15.0f) + 10.0f);
	dest->y = y3 * (t->y * (t->y * 6.0f - 15.0f) + 10.0f);
}

RGINLINE void rg_noise_taylor_inv_sqrt(const rg_vec4* x, rg_vec4* dest)
{
	dest->x = 1.79284291400159f - 0.85373472095314f * x->x;
	dest->y = 1.79284291400159f - 0.85373472095314f * x->y;
	dest->z = 1.79284291400159f - 0.85373472095314f * x->z;
	dest->w = 1.79284291400159f - 0.85373472095314f * x->w;
}

RGINLINE void rg_noise_grad_norm_vec4(rg_vec4* g00, rg_vec4* g01, rg_vec4* g10, rg_vec4* g11)
{
	rg_vec4 norm;
	norm.x = rg_noise_vec4_dot(g00, g00);
	norm.y = rg_noise_vec4_dot(g01, g01);
	norm.z = rg_noise_vec4_dot(g10, g10);
	norm.w = rg_noise_vec4_dot(g11, g11);
	rg_noise_taylor_inv_sqrt(&norm, &norm);

	rg_noise_vec4_scale(g00, norm.x, g00);
	rg_noise_vec4_scale(g01, norm.y, g01);
	rg_noise_vec4_scale(g10, norm.z, g10);
	rg_noise_vec4_scale(g11, norm.w, g11);
}

RGINLINE void rg_noise_grad_norm_vec3(rg_vec3* g00, rg_vec3* g01, rg_vec3* g10, rg_vec3* g11)
{
	rg_vec4 norm;
	norm.x = rg_noise_vec3_dot(g00, g00);
	norm.y = rg_noise_vec3_dot(g01, g01);
	norm.z = rg_noise_vec3_dot(g10, g10);
	norm.w = rg_noise_vec3_dot(g11, g11);
	rg_noise_taylor_inv_sqrt(&norm, &norm);

	rg_noise_vec3_scale(g00, norm.x, g00);
	rg_noise_vec3_scale(g01, norm.y, g01);
	rg_noise_vec3_scale(g10, norm.z, g10);
	rg_noise_vec3_scale(g11, norm.w, g11);
}

RGINLINE void rg_noise_grad_norm_vec2(rg_vec2* g00, rg_vec2* g01, rg_vec2* g10, rg_vec2* g11)
{
	rg_vec4 norm;
	norm.x = rg_noise_vec2_dot(g00, g00);
	norm.y = rg_noise_vec2_dot(g01, g01);
	norm.z = rg_noise_vec2_dot(g10, g10);
	norm.w = rg_noise_vec2_dot(g11, g11);
	rg_noise_taylor_inv_sqrt(&norm, &norm);

	rg_noise_vec2_scale(g00, norm.x, g00);
	rg_noise_vec2_scale(g01, norm.y, g01);
	rg_noise_vec2_scale(g10, norm.z, g10);
	rg_noise_vec2_scale(g11, norm.w, g11);
}

RGINLINE void rg_noise_i2gxyzw(const rg_vec4* ixy, rg_vec4* gx, rg_vec4* gy, rg_vec4* gz, rg_vec4* gw)
{
	rg_noise_vec4_divs(ixy, 7.0f, gx);
	rg_noise_vec4_floor(gx, gy);
	rg_noise_vec4_divs(gy, 7.0f, gy);
	rg_noise_vec4_floor(gy, gz);
	rg_noise_vec4_divs(gz, 6.0f, gz);

	rg_noise_vec4_fract(gx, gx);
	rg_noise_vec4_subs(gx, 0.5f, gx);
	rg_noise_vec4_fract(gy, gy);
	rg_noise_vec4_subs(gy, 0.5f, gy);
	rg_noise_vec4_fract(gz, gz);
	rg_noise_vec4_subs(gz, 0.5f, gz);

	rg_vec4 gxa, gya, gza;
	rg_noise_vec4_abs(gx, &gxa);
	rg_noise_vec4_abs(gy, &gya);
	rg_noise_vec4_abs(gz, &gza);

	rg_noise_vec4_fill(gw, 0.75f);
	rg_noise_vec4_sub(gw, &gxa, gw);
	rg_noise_vec4_sub(gw, &gza, gw);
	rg_noise_vec4_sub(gw, &gya, gw);

	rg_vec4 sw;
	rg_noise_vec4_stepr(gw, 0.0f, &sw);

	rg_vec4 temp;
	rg_noise_vec4_zero(&temp);
	rg_noise_vec4_step(&temp, gx, &temp);
	rg_noise_vec4_subs(&temp, 0.5f, &temp);
	rg_noise_vec4_mul(&sw, &temp, &temp);
	rg_noise_vec4_sub(gx, &temp, gx);

	rg_noise_vec4_zero(&temp);
	rg_noise_vec4_step(&temp, gy, &temp);
	rg_noise_vec4_subs(&temp, 0.5f, &temp);
	rg_noise_vec4_mul(&sw, &temp, &temp);
	rg_noise_vec4_sub(gy, &temp, gy);
}

RGINLINE void rg_noise_i2gxyz(const rg_vec4* ixy, rg_vec4* gx, rg_vec4* gy, rg_vec4* gz)
{
	rg_noise_vec4_scale(ixy, 1.0f / 7.0f, gx);
	rg_noise_vec4_floor(gx, gy);
	rg_noise_vec4_scale(gy, 1.0f / 7.0f, gy);
	rg_noise_vec4_fract(gy, gy);
	rg_noise_vec4_subs(gy, 0.5f, gy);

	rg_noise_vec4_fract(gx, gx);

	rg_vec4 gxa, gya;
	rg_noise_vec4_abs(gx, &gxa);
	rg_noise_vec4_abs(gy, &gya);

	rg_noise_vec4_fill(gz, 0.5f);
	rg_noise_vec4_sub(gz, &gxa, gz);
	rg_noise_vec4_sub(gz, &gya, gz);

	rg_vec4 sz;
	rg_noise_vec4_stepr(gz, 0.0f, &sz);

	rg_vec4 temp;
	rg_noise_vec4_zero(&temp);
	rg_noise_vec4_step(&temp, gx, &temp);
	rg_noise_vec4_subs(&temp, 0.5f, &temp);
	rg_noise_vec4_mul(&sz, &temp, &temp);
	rg_noise_vec4_sub(gx, &temp, gx);

	rg_noise_vec4_zero(&temp);
	rg_noise_vec4_step(&temp, gy, &temp);
	rg_noise_vec4_subs(&temp, 0.5f, &temp);
	rg_noise_vec4_mul(&sz, &temp, &temp);
	rg_noise_vec4_sub(gy, &temp, gy);
}

RGINLINE void rg_noise_i2gxy(const rg_vec4* i, rg_vec4* gx, rg_vec4* gy)
{
	rg_noise_vec4_divs(i, 41.0f, gx);
	rg_noise_vec4_fract(gx, gx);
	rg_noise_vec4_scale(gx, 2.0f, gx);
	rg_noise_vec4_subs(gx, 1.0f, gx);

	rg_noise_vec4_abs(gx, gy);
	rg_noise_vec4_subs(gy, 0.5f, gy);

	rg_vec4 tx;
	rg_noise_vec4_adds(gx, 0.5f, &tx);
	rg_noise_vec4_floor(&tx, &tx);
	rg_noise_vec4_sub(gx, &tx, gx);
}

RGINLINE f32 rg_perlin_vec4(const rg_vec4* point)
{
	rg_vec4 pi0;
	rg_vec4 pi1;
	rg_vec4 pf0;
	rg_vec4 pf1;

	rg_noise_vec4_floor(point, &pi0);
	rg_noise_vec4_adds(&pi0, 1.0f, &pi1);

	rg_noise_vec4_mods(&pi0, 289.0f, &pi0);
	rg_noise_vec4_mods(&pi1, 289.0f, &pi1);

	rg_noise_vec4_fract(point, &pf0);
	rg_noise_vec4_subs(&pf0, 1.0f, &pf1);

	rg_vec4 ix;
	rg_vec4 iy;
	rg_vec4 iz0;
	rg_vec4 iz1;
	rg_vec4 iw0;
	rg_vec4 iw1;
	rg_vec4_set(&ix, pi0.x, pi1.x, pi0.x, pi1.x);
	rg_vec4_set(&iy, pi0.y, pi0.y, pi1.y, pi1.y);
	rg_vec4_set(&iz0, pi0.z, pi0.z, pi0.z, pi0.z);
	rg_vec4_set(&iz1, pi1.z, pi1.z, pi1.z, pi1.z);
	rg_vec4_set(&iw0, pi0.w, pi0.w, pi0.w, pi0.w);
	rg_vec4_set(&iw1, pi1.w, pi1.w, pi1.w, pi1.w);

	rg_vec4 ixy;
	rg_noise_permute_vec4(&ix, &ixy);
	rg_noise_vec4_add(&ixy, &iy, &ixy);
	rg_noise_permute_vec4(&ixy, &ixy);

	rg_vec4 ixy0;
	rg_noise_vec4_add(&ixy, &iz0, &ixy0);
	rg_noise_permute_vec4(&ixy0, &ixy0);

	rg_vec4 ixy1;
	rg_noise_vec4_add(&ixy, &iz1, &ixy1);
	rg_noise_permute_vec4(&ixy1, &ixy1);

	rg_vec4 ixy00;
	rg_noise_vec4_add(&ixy0, &iw0, &ixy00);
	rg_noise_permute_vec4(&ixy00, &ixy00);

	rg_vec4 ixy01;
	rg_noise_vec4_add(&ixy0, &iw1, &ixy01);
	rg_noise_permute_vec4(&ixy01, &ixy01);

	rg_vec4 ixy10;
	rg_noise_vec4_add(&ixy1, &iw0, &ixy10);
	rg_noise_permute_vec4(&ixy10, &ixy10);

	rg_vec4 ixy11;
	rg_noise_vec4_add(&ixy1, &iw1, &ixy11);
	rg_noise_permute_vec4(&ixy11, &ixy11);

	rg_vec4 gx00, gy00, gz00, gw00;
	rg_noise_i2gxyzw(&ixy00, &gx00, &gy00, &gz00, &gw00);

	rg_vec4 gx01, gy01, gz01, gw01;
	rg_noise_i2gxyzw(&ixy01, &gx01, &gy01, &gz01, &gw01);

	rg_vec4 gx10, gy10, gz10, gw10;
	rg_noise_i2gxyzw(&ixy10, &gx10, &gy10, &gz10, &gw10);

	rg_vec4 gx11, gy11, gz11, gw11;
	rg_noise_i2gxyzw(&ixy11, &gx11, &gy11, &gz11, &gw11);

	rg_vec4 g0000;
	rg_vec4_set(&g0000, gx00.x, gy00.x, gz00.x, gw00.x);
	rg_vec4 g0100;
	rg_vec4_set(&g0100, gx00.z, gy00.z, gz00.z, gw00.z);
	rg_vec4 g1000;
	rg_vec4_set(&g1000, gx00.y, gy00.y, gz00.y, gw00.y);
	rg_vec4 g1100;
	rg_vec4_set(&g1100, gx00.w, gy00.w, gz00.w, gw00.w);

	rg_vec4 g0001;
	rg_vec4_set(&g0001, gx01.x, gy01.x, gz01.x, gw01.x);
	rg_vec4 g0101;
	rg_vec4_set(&g0101, gx01.z, gy01.z, gz01.z, gw01.z);
	rg_vec4 g1001;
	rg_vec4_set(&g1001, gx01.y, gy01.y, gz01.y, gw01.y);
	rg_vec4 g1101;
	rg_vec4_set(&g1101, gx01.w, gy01.w, gz01.w, gw01.w);

	rg_vec4 g0010;
	rg_vec4_set(&g0010, gx10.x, gy10.x, gz10.x, gw10.x);
	rg_vec4 g0110;
	rg_vec4_set(&g0110, gx10.z, gy10.z, gz10.z, gw10.z);
	rg_vec4 g1010;
	rg_vec4_set(&g1010, gx10.y, gy10.y, gz10.y, gw10.y);
	rg_vec4 g1110;
	rg_vec4_set(&g1110, gx10.w, gy10.w, gz10.w, gw10.w);

	rg_vec4 g0011;
	rg_vec4_set(&g0011, gx11.x, gy11.x, gz11.x, gw11.x);
	rg_vec4 g0111;
	rg_vec4_set(&g0111, gx11.z, gy11.z, gz11.z, gw11.z);
	rg_vec4 g1011;
	rg_vec4_set(&g1011, gx11.y, gy11.y, gz11.y, gw11.y);
	rg_vec4 g1111;
	rg_vec4_set(&g1111, gx11.w, gy11.w, gz11.w, gw11.w);

	rg_noise_grad_norm_vec4(&g0000, &g0100, &g1000, &g1100);
	rg_noise_grad_norm_vec4(&g0001, &g0101, &g1001, &g1101);
	rg_noise_grad_norm_vec4(&g0010, &g0110, &g1010, &g1110);
	rg_noise_grad_norm_vec4(&g0011, &g0111, &g1011, &g1111);

	f32 n0000 = rg_noise_vec4_dot(&g0000, &pf0);

	rg_vec4 n1000d;
	rg_vec4_set(&n1000d, pf1.x, pf0.y, pf0.z, pf0.w);
	f32 n1000 = rg_noise_vec4_dot(&g1000, &n1000d);

	rg_vec4 n0100d;
	rg_vec4_set(&n0100d, pf0.x, pf1.y, pf0.z, pf0.w);
	f32 n0100 = rg_noise_vec4_dot(&g0100, &n0100d);

	rg_vec4 n1100d;
	rg_vec4_set(&n1100d, pf1.x, pf1.y, pf0.z, pf0.w);
	f32 n1100 = rg_noise_vec4_dot(&g1100, &n1100d);

	rg_vec4 n0010d;
	rg_vec4_set(&n0010d, pf0.x, pf0.y, pf1.z, pf0.w);
	f32 n0010 = rg_noise_vec4_dot(&g0010, &n0010d);

	rg_vec4 n1010d;
	rg_vec4_set(&n1010d, pf1.x, pf0.y, pf1.z, pf0.w);
	f32 n1010 = rg_noise_vec4_dot(&g1010, &n1010d);

	rg_vec4 n0110d;
	rg_vec4_set(&n0110d, pf0.x, pf1.y, pf1.z, pf0.w);
	f32 n0110 = rg_noise_vec4_dot(&g0110, &n0110d);

	rg_vec4 n1110d;
	rg_vec4_set(&n1110d, pf1.x, pf1.y, pf1.z, pf0.w);
	f32 n1110 = rg_noise_vec4_dot(&g1110, &n1110d);

	rg_vec4 n0001d;
	rg_vec4_set(&n0001d, pf0.x, pf0.y, pf0.z, pf1.w);
	f32 n0001 = rg_noise_vec4_dot(&g0001, &n0001d);

	rg_vec4 n1001d;
	rg_vec4_set(&n1001d, pf1.x, pf0.y, pf0.z, pf1.w);
	f32 n1001 = rg_noise_vec4_dot(&g1001, &n1001d);

	rg_vec4 n0101d;
	rg_vec4_set(&n0101d, pf0.x, pf1.y, pf0.z, pf1.w);
	f32 n0101 = rg_noise_vec4_dot(&g0101, &n0101d);

	rg_vec4 n1101d;
	rg_vec4_set(&n1101d, pf1.x, pf1.y, pf0.z, pf1.w);
	f32 n1101 = rg_noise_vec4_dot(&g1101, &n1101d);

	rg_vec4 n0011d;
	rg_vec4_set(&n0011d, pf0.x, pf0.y, pf1.z, pf1.w);
	f32 n0011 = rg_noise_vec4_dot(&g0011, &n0011d);

	rg_vec4 n1011d;
	rg_vec4_set(&n1011d, pf1.x, pf0.y, pf1.z, pf1.w);
	f32 n1011 = rg_noise_vec4_dot(&g1011, &n1011d);

	rg_vec4 n0111d;
	rg_vec4_set(&n0111d, pf0.x, pf1.y, pf1.z, pf1.w);
	f32 n0111 = rg_noise_vec4_dot(&g0111, &n0111d);

	f32 n1111 = rg_noise_vec4_dot(&g1111, &pf1);

	rg_vec4 fade_xyzw;
	rg_noise_fade_vec4(&pf0, &fade_xyzw);

	rg_vec4 n_0w1;
	rg_vec4_set(&n_0w1, n0000, n1000, n0100, n1100);
	rg_vec4 n_0w2;
	rg_vec4_set(&n_0w2, n0001, n1001, n0101, n1101);
	rg_vec4 n_0w;
	rg_noise_vec4_lerp(&n_0w1, &n_0w2, fade_xyzw.w, &n_0w);

	rg_vec4 n_1w1;
	rg_vec4_set(&n_1w1, n0010, n1010, n0110, n1110);
	rg_vec4 n_1w2;
	rg_vec4_set(&n_1w2, n0011, n1011, n0111, n1111);
	rg_vec4 n_1w;
	rg_noise_vec4_lerp(&n_1w1, &n_1w2, fade_xyzw.w, &n_1w);

	rg_vec4 n_zw;
	rg_noise_vec4_lerp(&n_0w, &n_1w, fade_xyzw.z, &n_zw);

	rg_vec2 n_yzw;
	rg_vec2 n_yzw1;
	rg_vec2 n_yzw2;
	rg_vec2_set(&n_yzw1, n_zw.x, n_zw.y);
	rg_vec2_set(&n_yzw2, n_zw.z, n_zw.w);
	rg_noise_vec2_lerp(&n_yzw1, &n_yzw2, fade_xyzw.y, &n_yzw);

	f32 n_xyzw = rg_lerpf(n_yzw.x, n_yzw.y, fade_xyzw.x);

	return n_xyzw * 2.2f;
}

RGINLINE f32 rg_perlin_vec3(const rg_vec3* point)
{
	rg_vec3 pi0;
	rg_vec3 pi1;
	rg_vec3 pf0;
	rg_vec3 pf1;

	rg_noise_vec3_floor(point, &pi0);
	rg_noise_vec3_adds(&pi0, 1.0f, &pi1);

	rg_noise_vec3_mods(&pi0, 289.0f, &pi0);
	rg_noise_vec3_mods(&pi1, 289.0f, &pi1);

	rg_noise_vec3_fract(point, &pf0);
	rg_noise_vec3_subs(&pf0, 1.0f, &pf1);

	rg_vec4 ix;
	rg_vec4_set(&ix, pi0.x, pi1.x, pi0.x, pi1.x);
	rg_vec4 iy;
	rg_vec4_set(&iy, pi0.y, pi0.y, pi1.y, pi1.y);
	rg_vec4 iz0;
	rg_vec4_set(&iz0, pi0.z, pi0.z, pi0.z, pi0.z);
	rg_vec4 iz1;
	rg_vec4_set(&iz1, pi1.z, pi1.z, pi1.z, pi1.z);

	rg_vec4 ixy;
	rg_noise_permute_vec4(&ix, &ixy);
	rg_noise_vec4_add(&ixy, &iy, &ixy);
	rg_noise_permute_vec4(&ixy, &ixy);

	rg_vec4 ixy0;
	rg_noise_vec4_add(&ixy, &iz0, &ixy0);
	rg_noise_permute_vec4(&ixy0, &ixy0);

	rg_vec4 ixy1;
	rg_noise_vec4_add(&ixy, &iz1, &ixy1);
	rg_noise_permute_vec4(&ixy1, &ixy1);

	rg_vec4 gx0, gy0, gz0;
	rg_noise_i2gxyz(&ixy0, &gx0, &gy0, &gz0);

	rg_vec4 gx1, gy1, gz1;
	rg_noise_i2gxyz(&ixy1, &gx1, &gy1, &gz1);

	rg_vec3 g000;
	rg_vec3_set(&g000, gx0.x, gy0.x, gz0.x);
	rg_vec3 g100;
	rg_vec3_set(&g100, gx0.y, gy0.y, gz0.y);
	rg_vec3 g010;
	rg_vec3_set(&g010, gx0.z, gy0.z, gz0.z);
	rg_vec3 g110;
	rg_vec3_set(&g110, gx0.w, gy0.w, gz0.w);

	rg_vec3 g001;
	rg_vec3_set(&g001, gx1.x, gy1.x, gz1.x);
	rg_vec3 g101;
	rg_vec3_set(&g101, gx1.y, gy1.y, gz1.y);
	rg_vec3 g011;
	rg_vec3_set(&g011, gx1.z, gy1.z, gz1.z);
	rg_vec3 g111;
	rg_vec3_set(&g111, gx1.w, gy1.w, gz1.w);

	rg_noise_grad_norm_vec3(&g000, &g010, &g100, &g110);
	rg_noise_grad_norm_vec3(&g001, &g011, &g101, &g111);

	f32 n000 = rg_noise_vec3_dot(&g000, &pf0);

	rg_vec3 n100d;
	rg_vec3_set(&n100d, pf1.x, pf0.y, pf0.z);
	f32 n100 = rg_noise_vec3_dot(&g100, &n100d);

	rg_vec3 n010d;
	rg_vec3_set(&n010d, pf0.x, pf1.y, pf0.z);
	f32 n010 = rg_noise_vec3_dot(&g010, &n010d);

	rg_vec3 n110d;
	rg_vec3_set(&n110d, pf1.x, pf1.y, pf0.z);
	f32 n110 = rg_noise_vec3_dot(&g110, &n110d);

	rg_vec3 n001d;
	rg_vec3_set(&n001d, pf0.x, pf0.y, pf1.z);
	f32 n001 = rg_noise_vec3_dot(&g001, &n001d);

	rg_vec3 n101d;
	rg_vec3_set(&n101d, pf1.x, pf0.y, pf1.z);
	f32 n101 = rg_noise_vec3_dot(&g101, &n101d);

	rg_vec3 n011d;
	rg_vec3_set(&n011d, pf0.x, pf1.y, pf1.z);
	f32 n011 = rg_noise_vec3_dot(&g011, &n011d);

	f32 n111 = rg_noise_vec3_dot(&g111, &pf1);

	rg_vec3 fade_xyz;
	rg_noise_fade_vec3(&pf0, &fade_xyz);

	rg_vec4 n_z1;
	rg_vec4_set(&n_z1, n000, n100, n010, n110);
	rg_vec4 n_z2;
	rg_vec4_set(&n_z2, n001, n101, n011, n111);
	rg_vec4 n_z;
	rg_noise_vec4_lerp(&n_z1, &n_z2, fade_xyz.z, &n_z);

	rg_vec2 n_yz;
	rg_vec2 n_yz1;
	rg_vec2 n_yz2;
	rg_vec2_set(&n_yz1, n_z.x, n_z.y);
	rg_vec2_set(&n_yz2, n_z.z, n_z.w);
	rg_noise_vec2_lerp(&n_yz1, &n_yz2, fade_xyz.y, &n_yz);

	f32 n_xyz = rg_lerpf(n_yz.x, n_yz.y, fade_xyz.x);

	return n_xyz * 2.2f;
}

RGINLINE f32 rg_perlin_vec2(const rg_vec2* point)
{
	rg_vec4 pi;
	rg_vec4 pf;

	rg_vec4_set(&pi, point->x, point->y, point->x, point->y);
	rg_noise_vec4_floor(&pi, &pi);
	pi.z += 1.0f;
	pi.w += 1.0f;

	rg_vec4_set(&pf, point->x, point->y, point->x, point->y);
	rg_noise_vec4_fract(&pf, &pf);
	pf.z -= 1.0f;
	pf.w -= 1.0f;

	rg_noise_vec4_mods(&pi, 289.0f, &pi);

	rg_vec4 ix;
	rg_vec4_set(&ix, pi.x, pi.z, pi.x, pi.z);
	rg_vec4 iy;
	rg_vec4_set(&iy, pi.y, pi.y, pi.w, pi.w);
	rg_vec4 fx;
	rg_vec4_set(&fx, pf.x, pf.z, pf.x, pf.z);
	rg_vec4 fy;
	rg_vec4_set(&fy, pf.y, pf.y, pf.w, pf.w);

	rg_vec4 i;
	rg_noise_permute_vec4(&ix, &i);
	rg_noise_vec4_add(&i, &iy, &i);
	rg_noise_permute_vec4(&i, &i);

	rg_vec4 gx, gy;
	rg_noise_i2gxy(&i, &gx, &gy);

	rg_vec2 g00;
	rg_vec2_set(&g00, gx.x, gy.x);
	rg_vec2 g10;
	rg_vec2_set(&g10, gx.y, gy.y);
	rg_vec2 g01;
	rg_vec2_set(&g01, gx.z, gy.z);
	rg_vec2 g11;
	rg_vec2_set(&g11, gx.w, gy.w);

	rg_noise_grad_norm_vec2(&g00, &g01, &g10, &g11);

	rg_vec2 n00d;
	rg_vec2_set(&n00d, fx.x, fy.x);
	f32 n00 = rg_noise_vec2_dot(&g00, &n00d);

	rg_vec2 n10d;
	rg_vec2_set(&n10d, fx.y, fy.y);
	f32 n10 = rg_noise_vec2_dot(&g10, &n10d);

	rg_vec2 n01d;
	rg_vec2_set(&n01d, fx.z, fy.z);
	f32 n01 = rg_noise_vec2_dot(&g01, &n01d);

	rg_vec2 n11d;
	rg_vec2_set(&n11d, fx.w, fy.w);
	f32 n11 = rg_noise_vec2_dot(&g11, &n11d);

	rg_vec2 fade_xy;
	rg_vec2 temp2;
	rg_vec2_set(&temp2, pf.x, pf.y);
	rg_noise_fade_vec2(&temp2, &fade_xy);

	rg_vec2 n_x1;
	rg_vec2_set(&n_x1, n00, n01);
	rg_vec2 n_x2;
	rg_vec2_set(&n_x2, n10, n11);
	rg_vec2 n_x;
	rg_noise_vec2_lerp(&n_x1, &n_x2, fade_xy.x, &n_x);

	f32 n_xy = rg_lerpf(n_x.x, n_x.y, fade_xy.y);

	return n_xy * 2.3f;
}

#undef RG_NOISE_FRACT_MAX

#endif // RG_MATH_NOISE_H
