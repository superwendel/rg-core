// rg_math_curve - curve, bezier, and easing helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides Bezier, spline, interpolation, and easing helpers.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_CURVE_H
#define RG_MATH_CURVE_H

#include "rg_math_mat.h"

RG_MATH_EXTERN_C_BEGIN

// =============================================================================
// CURVE CONSTANTS
// =============================================================================

#define RG_BEZIER_MAT_INIT {-1.0f, 3.0f, -3.0f, 1.0f, \
	                        3.0f, -6.0f, 3.0f, 0.0f,  \
	                        -3.0f, 3.0f, 0.0f, 0.0f,  \
	                        1.0f, 0.0f, 0.0f, 0.0f}
#define RG_HERMITE_MAT_INIT {2.0f, -3.0f, 0.0f, 1.0f, \
	                         -2.0f, 3.0f, 0.0f, 0.0f, \
	                         1.0f, -2.0f, 1.0f, 0.0f, \
	                         1.0f, -1.0f, 0.0f, 0.0f}

#define RG_BEZIER_MAT ((rg_mat4){RG_BEZIER_MAT_INIT})
#define RG_HERMITE_MAT ((rg_mat4){RG_HERMITE_MAT_INIT})

#define RG_DECASTEL_EPS 1e-9f
#define RG_DECASTEL_FAST_EPS 1e-6f
#define RG_DECASTEL_MAX 1000
#define RG_DECASTEL_SMALL 1e-20f

// =============================================================================
// CURVE HELPERS
// =============================================================================

RGINLINE f32 rg_smc(f32 s, const rg_mat4* m, const rg_vec4* c);
RGINLINE f32 rg_bezier(f32 s, f32 p0, f32 c0, f32 c1, f32 p1);
RGINLINE f32 rg_hermite(f32 s, f32 p0, f32 t0, f32 t1, f32 p1);
RGINLINE f32 rg_decasteljau(f32 prm, f32 p0, f32 c0, f32 c1, f32 p1);

// =============================================================================
// EASING HELPERS
// =============================================================================

RGINLINE f32 rg_ease_linear(f32 t);
RGINLINE f32 rg_ease_sine_in(f32 t);
RGINLINE f32 rg_ease_sine_out(f32 t);
RGINLINE f32 rg_ease_sine_inout(f32 t);
RGINLINE f32 rg_ease_sine_in_fast(f32 t);
RGINLINE f32 rg_ease_sine_out_fast(f32 t);
RGINLINE f32 rg_ease_sine_inout_fast(f32 t);
RGINLINE f32 rg_ease_quad_in(f32 t);
RGINLINE f32 rg_ease_quad_out(f32 t);
RGINLINE f32 rg_ease_quad_inout(f32 t);
RGINLINE f32 rg_ease_cubic_in(f32 t);
RGINLINE f32 rg_ease_cubic_out(f32 t);
RGINLINE f32 rg_ease_cubic_inout(f32 t);
RGINLINE f32 rg_ease_quart_in(f32 t);
RGINLINE f32 rg_ease_quart_out(f32 t);
RGINLINE f32 rg_ease_quart_inout(f32 t);
RGINLINE f32 rg_ease_quint_in(f32 t);
RGINLINE f32 rg_ease_quint_out(f32 t);
RGINLINE f32 rg_ease_quint_inout(f32 t);
RGINLINE f32 rg_ease_exp_in(f32 t);
RGINLINE f32 rg_ease_exp_out(f32 t);
RGINLINE f32 rg_ease_exp_inout(f32 t);
RGINLINE f32 rg_ease_circ_in(f32 t);
RGINLINE f32 rg_ease_circ_out(f32 t);
RGINLINE f32 rg_ease_circ_inout(f32 t);
RGINLINE f32 rg_ease_back_in(f32 t);
RGINLINE f32 rg_ease_back_out(f32 t);
RGINLINE f32 rg_ease_back_inout(f32 t);
RGINLINE f32 rg_ease_elast_in(f32 t);
RGINLINE f32 rg_ease_elast_out(f32 t);
RGINLINE f32 rg_ease_elast_inout(f32 t);
RGINLINE f32 rg_ease_bounce_in(f32 t);
RGINLINE f32 rg_ease_bounce_out(f32 t);
RGINLINE f32 rg_ease_bounce_inout(f32 t);

RG_MATH_EXTERN_C_END

// =============================================================================
// IMPLEMENTATION
// =============================================================================

RGINLINE f32 rg_smc(f32 s, const rg_mat4* m, const rg_vec4* c)
{
	f32 c0 = c->x;
	f32 c1 = c->y;
	f32 c2 = c->z;
	f32 c3 = c->w;

	f32 a = m->m[0] * c0 + m->m[4] * c1 + m->m[8] * c2 + m->m[12] * c3;
	f32 b = m->m[1] * c0 + m->m[5] * c1 + m->m[9] * c2 + m->m[13] * c3;
	f32 d = m->m[2] * c0 + m->m[6] * c1 + m->m[10] * c2 + m->m[14] * c3;
	f32 e = m->m[3] * c0 + m->m[7] * c1 + m->m[11] * c2 + m->m[15] * c3;

	return ((a * s + b) * s + d) * s + e;
}

RGINLINE f32 rg_bezier(f32 s, f32 p0, f32 c0, f32 c1, f32 p1)
{
	f32 x = 1.0f - s;
	f32 xx = x * x;
	f32 ss = s * s;
	f32 xs3 = (s - ss) * 3.0f;
	f32 a = p0 * xx + c0 * xs3;
	return a + s * (c1 * xs3 + p1 * ss - a);
}

RGINLINE f32 rg_hermite(f32 s, f32 p0, f32 t0, f32 t1, f32 p1)
{
	f32 ss = s * s;
	f32 a = ss + ss;
	f32 c = a + ss;
	f32 b = a * s;
	f32 d = s * ss;
	f32 f = d - ss;
	f32 e = b - c;

	return p0 * (e + 1.0f) + t0 * (f - ss + s) + t1 * f - p1 * e;
}

RGINLINE f32 rg_decasteljau(f32 prm, f32 p0, f32 c0, f32 c1, f32 p1)
{
	f32 u = 0.0f;
	f32 v = 1.0f;

	if (prm - p0 < RG_DECASTEL_SMALL)
	{
		return 0.0f;
	}

	if (p1 - prm < RG_DECASTEL_SMALL)
	{
		return 1.0f;
	}

#if RG_MATH_MAX_PERF
	if (p0 <= c0 && c0 <= c1 && c1 <= p1)
	{
		f32 lo = 0.0f;
		f32 hi = 1.0f;
		f32 span = p1 - p0;
		if (span > RG_DECASTEL_SMALL)
		{
			f32 one_third = span * (1.0f / 3.0f);
			if (c0 == p0 + one_third && c1 == p1 - one_third)
			{
				return rg_clampf((prm - p0) / span, 0.0f, 1.0f);
			}
		}

		f32 t = (span > RG_DECASTEL_SMALL) ? ((prm - p0) / span) : 0.5f;
		t = rg_clampf(t, 0.0f, 1.0f);

		for (int i = 0; i < 10; i++)
		{
			f32 f = rg_bezier(t, p0, c0, c1, p1) - prm;

			if (rg_absf(f) < RG_DECASTEL_FAST_EPS)
			{
				return t;
			}

			if (f < 0.0f)
			{
				lo = t;
			}
			else
			{
				hi = t;
			}

			f32 omt = 1.0f - t;
			f32 d = 3.0f * ((c0 - p0) * omt * omt + 2.0f * (c1 - c0) * omt * t + (p1 - c1) * t * t);
			f32 next = (lo + hi) * 0.5f;
			if (d > RG_DECASTEL_SMALL)
			{
				f32 nt = t - f / d;
				if (lo < nt && nt < hi)
				{
					next = nt;
				}
			}
			t = next;
		}

		for (int i = 0; i < 8; i++)
		{
			t = (lo + hi) * 0.5f;
			f32 f = rg_bezier(t, p0, c0, c1, p1);

			if (rg_absf(f - prm) < RG_DECASTEL_FAST_EPS)
			{
				return t;
			}

			if (f < prm)
			{
				lo = t;
			}
			else
			{
				hi = t;
			}
		}

		return (lo + hi) * 0.5f;
	}
#endif

	for (int i = 0; i < RG_DECASTEL_MAX; i++)
	{
		f32 a = (p0 + c0) * 0.5f;
		f32 b = (c0 + c1) * 0.5f;
		f32 c = (c1 + p1) * 0.5f;
		f32 d = (a + b) * 0.5f;
		f32 e = (b + c) * 0.5f;
		f32 f = (d + e) * 0.5f;

		if (rg_absf(f - prm) < RG_DECASTEL_EPS)
		{
			return rg_clampf((u + v) * 0.5f, 0.0f, 1.0f);
		}

		if (f < prm)
		{
			p0 = f;
			c0 = e;
			c1 = c;
			u = (u + v) * 0.5f;
		}
		else
		{
			c0 = a;
			c1 = d;
			p1 = f;
			v = (u + v) * 0.5f;
		}
	}

	return rg_clampf((u + v) * 0.5f, 0.0f, 1.0f);
}

RGINLINE f32 rg_ease_linear(f32 t)
{
	return t;
}

RGINLINE f32 rg_ease_sine_in(f32 t)
{
	return 1.0f - rg_cosf(t * RG_HALF_PI);
}

RGINLINE f32 rg_ease_sine_out(f32 t)
{
	return rg_sinf(t * RG_HALF_PI);
}

RGINLINE f32 rg_ease_sine_inout(f32 t)
{
	return 0.5f * (1.0f - rg_cosf(t * RG_PI));
}

RGINLINE f32 rg_ease_sine_in_fast(f32 t)
{
	return 1.0f - rg_cosf_fast(t * RG_HALF_PI);
}

RGINLINE f32 rg_ease_sine_out_fast(f32 t)
{
	return rg_sinf_fast(t * RG_HALF_PI);
}

RGINLINE f32 rg_ease_sine_inout_fast(f32 t)
{
	return 0.5f * (1.0f - rg_cosf_fast(t * RG_PI));
}

RGINLINE f32 rg_ease_quad_in(f32 t)
{
	return t * t;
}

RGINLINE f32 rg_ease_quad_out(f32 t)
{
	return t * (2.0f - t);
}

RGINLINE f32 rg_ease_quad_inout(f32 t)
{
	f32 tt = t * t;
	if (t < 0.5f)
	{
		return 2.0f * tt;
	}
	return (-2.0f * tt) + (4.0f * t) - 1.0f;
}

RGINLINE f32 rg_ease_cubic_in(f32 t)
{
	return t * t * t;
}

RGINLINE f32 rg_ease_cubic_out(f32 t)
{
	return t * (t * (t - 3.0f) + 3.0f);
}

RGINLINE f32 rg_ease_cubic_inout(f32 t)
{
	f32 f;

	if (t < 0.5f)
	{
		return 4.0f * t * t * t;
	}

	f = 2.0f * t - 2.0f;

	return 0.5f * f * f * f + 1.0f;
}

RGINLINE f32 rg_ease_quart_in(f32 t)
{
	f32 f = t * t;
	return f * f;
}

RGINLINE f32 rg_ease_quart_out(f32 t)
{
	f32 f = 1.0f - t;
	f32 ff = f * f;
	return 1.0f - ff * ff;
}

RGINLINE f32 rg_ease_quart_inout(f32 t)
{
	f32 f;
	f32 g;

	if (t < 0.5f)
	{
		f = t * t;
		return 8.0f * f * f;
	}

	f = t - 1.0f;
	g = f * f;

	return -8.0f * g * g + 1.0f;
}

RGINLINE f32 rg_ease_quint_in(f32 t)
{
	f32 f = t * t;
	return f * f * t;
}

RGINLINE f32 rg_ease_quint_out(f32 t)
{
	f32 f;
	f32 g;

	f = t - 1.0f;
	g = f * f;

	return g * g * f + 1.0f;
}

RGINLINE f32 rg_ease_quint_inout(f32 t)
{
	f32 f;
	f32 g;

	if (t < 0.5f)
	{
		f = t * t;
		return 16.0f * f * f * t;
	}

	f = 2.0f * t - 2.0f;
	g = f * f;

	return 0.5f * g * g * f + 1.0f;
}

RGINLINE f32 rg_ease_exp_in(f32 t)
{
	if (t == 0.0f)
	{
		return t;
	}

	return exp2f(10.0f * (t - 1.0f));
}

RGINLINE f32 rg_ease_exp_out(f32 t)
{
	if (t == 1.0f)
	{
		return t;
	}

	return 1.0f - exp2f(-10.0f * t);
}

RGINLINE f32 rg_ease_exp_inout(f32 t)
{
	if (t == 0.0f || t == 1.0f)
	{
		return t;
	}

	if (t < 0.5f)
	{
		return 0.5f * exp2f((20.0f * t) - 10.0f);
	}

	return -0.5f * exp2f((-20.0f * t) + 10.0f) + 1.0f;
}

RGINLINE f32 rg_ease_circ_in(f32 t)
{
	return 1.0f - rg_sqrtf(1.0f - (t * t));
}

RGINLINE f32 rg_ease_circ_out(f32 t)
{
	return rg_sqrtf((2.0f - t) * t);
}

RGINLINE f32 rg_ease_circ_inout(f32 t)
{
	if (t < 0.5f)
	{
		return 0.5f * (1.0f - rg_sqrtf(1.0f - 4.0f * (t * t)));
	}

	return 0.5f * (rg_sqrtf(-((2.0f * t) - 3.0f) * ((2.0f * t) - 1.0f)) + 1.0f);
}

RGINLINE f32 rg_ease_back_in(f32 t)
{
	f32 o = 1.70158f;
	f32 z = ((o + 1.0f) * t) - o;
	return t * t * z;
}

RGINLINE f32 rg_ease_back_out(f32 t)
{
	return t * (t * (2.70158f * t - 6.40316f) + 4.70158f);
}

RGINLINE f32 rg_ease_back_inout(f32 t)
{
	f32 o = 1.70158f;
	f32 s = o * 1.525f;
	f32 x = 0.5f;
	f32 n = t / 0.5f;

	if (n < 1.0f)
	{
		f32 z = (s + 1.0f) * n - s;
		f32 m = n * n * z;
		return x * m;
	}

	n -= 2.0f;
	f32 z = (s + 1.0f) * n + s;
	f32 m = (n * n * z) + 2.0f;
	return x * m;
}

RGINLINE f32 rg_ease_elast_in(f32 t)
{
	return rg_sinf(13.0f * RG_HALF_PI * t) * exp2f(10.0f * (t - 1.0f));
}

RGINLINE f32 rg_ease_elast_out(f32 t)
{
	return rg_sinf(-13.0f * RG_HALF_PI * (t + 1.0f)) * exp2f(-10.0f * t) + 1.0f;
}

RGINLINE f32 rg_ease_elast_inout(f32 t)
{
	f32 a = 2.0f * t;

	if (t < 0.5f)
	{
		return 0.5f * rg_sinf(13.0f * RG_HALF_PI * a) * exp2f(10.0f * (a - 1.0f));
	}

	return 0.5f * (rg_sinf(-13.0f * RG_HALF_PI * a) * exp2f(-10.0f * (a - 1.0f)) + 2.0f);
}

RGINLINE f32 rg_ease_bounce_out(f32 t)
{
	f32 tt = t * t;

	if (t < (4.0f / 11.0f))
	{
		return (121.0f * tt) / 16.0f;
	}

	if (t < 8.0f / 11.0f)
	{
		return ((363.0f / 40.0f) * tt) - ((99.0f / 10.0f) * t) + (17.0f / 5.0f);
	}

	if (t < (9.0f / 10.0f))
	{
		return (4356.0f / 361.0f) * tt - (35442.0f / 1805.0f) * t + (16061.0f / 1805.0f);
	}

	return ((54.0f / 5.0f) * tt) - ((513.0f / 25.0f) * t) + (268.0f / 25.0f);
}

RGINLINE f32 rg_ease_bounce_in(f32 t)
{
	f32 u = 1.0f - t;
	f32 uu = u * u;
	f32 b;

	if (u < (4.0f / 11.0f))
	{
		b = (121.0f * uu) / 16.0f;
	}
	else if (u < 8.0f / 11.0f)
	{
		b = ((363.0f / 40.0f) * uu) - ((99.0f / 10.0f) * u) + (17.0f / 5.0f);
	}
	else if (u < (9.0f / 10.0f))
	{
		b = (4356.0f / 361.0f) * uu - (35442.0f / 1805.0f) * u + (16061.0f / 1805.0f);
	}
	else
	{
		b = ((54.0f / 5.0f) * uu) - ((513.0f / 25.0f) * u) + (268.0f / 25.0f);
	}

	return 1.0f - b;
}

RGINLINE f32 rg_ease_bounce_inout(f32 t)
{
	if (t < 0.5f)
	{
		return 0.5f * (1.0f - rg_ease_bounce_out(t * 2.0f));
	}

	return 0.5f * rg_ease_bounce_out(t * 2.0f - 1.0f) + 0.5f;
}

#endif // RG_MATH_CURVE_H
