// rg_math_io - debug print helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Provides configurable debug output for math and geometry values.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_MATH_IO_H
#define RG_MATH_IO_H

#include "rg_math_geom.h"

#ifndef RG_MATH_PRINTS_ENABLED
#if !defined(NDEBUG) || defined(RG_MATH_DEFINE_PRINTS) || defined(RG_MATH_NO_PRINTS_NOOP)
#define RG_MATH_PRINTS_ENABLED 1
#else
#define RG_MATH_PRINTS_ENABLED 0
#endif
#endif

#if RG_MATH_PRINTS_ENABLED
#ifndef RG_MATH_IO_NO_SPRINTF_INCLUDE
#include "rg_sprintf_hybrid.h"
#endif
#include <stdio.h>
#endif

#if RG_MATH_PRINTS_ENABLED
#ifndef RG_MATH_PRINT_PRECISION
#define RG_MATH_PRINT_PRECISION 5
#endif

#ifndef RG_MATH_PRINT_MAX_TO_SHORT
#define RG_MATH_PRINT_MAX_TO_SHORT 1e5f
#endif

#ifdef RG_MATH_PRINT_NO_COLOR
#ifndef RG_MATH_PRINT_COLOR
#define RG_MATH_PRINT_COLOR ""
#endif
#ifndef RG_MATH_PRINT_COLOR_RESET
#define RG_MATH_PRINT_COLOR_RESET ""
#endif
#else
#ifndef RG_MATH_PRINT_COLOR
#define RG_MATH_PRINT_COLOR "\033[36m"
#endif
#ifndef RG_MATH_PRINT_COLOR_RESET
#define RG_MATH_PRINT_COLOR_RESET "\033[0m"
#endif
#endif
#endif

RG_MATH_EXTERN_C_BEGIN

#if RG_MATH_PRINTS_ENABLED
RGINLINE void rg_arch_print(FILE* ostream);
RGINLINE void rg_arch_print_name(FILE* ostream);
RGINLINE void rg_mat4_print(const rg_mat4* matrix, FILE* ostream);
RGINLINE void rg_mat3_print(const rg_mat3* matrix, FILE* ostream);
RGINLINE void rg_mat2_print(const rg_mat2* matrix, FILE* ostream);
RGINLINE void rg_vec4_print(const rg_vec4* vec, FILE* ostream);
RGINLINE void rg_vec3_print(const rg_vec3* vec, FILE* ostream);
RGINLINE void rg_vec2_print(const rg_vec2* vec, FILE* ostream);
RGINLINE void rg_vec4i_print(const rg_vec4i* vec, FILE* ostream);
RGINLINE void rg_vec3i_print(const rg_vec3i* vec, FILE* ostream);
RGINLINE void rg_vec2i_print(const rg_vec2i* vec, FILE* ostream);
RGINLINE void rg_quat_print(const rg_quat* quat, FILE* ostream);
RGINLINE void rg_aabb_print(const rg_aabb* aabb, const char* tag, FILE* ostream);
RGINLINE void rg_aabb2_print(const rg_aabb2* aabb, const char* tag, FILE* ostream);
RGINLINE void rg_sphere_print(const rg_sphere* sphere, FILE* ostream);
RGINLINE void rg_plane_print(const rg_plane* plane, FILE* ostream);
RGINLINE void rg_ray_print(const rg_ray* ray, FILE* ostream);
#endif

RG_MATH_EXTERN_C_END

#if RG_MATH_PRINTS_ENABLED
RGINLINE void rg_math_io_write_cb(const char* buf, void* user, int len)
{
	FILE* ostream = (FILE*)user;
	if (len > 0)
	{
		fwrite(buf, 1, (size_t)len, ostream);
	}
}

static inline void rg_math_io_printf(FILE* ostream, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	rg_vsprintf_cb(rg_math_io_write_cb, ostream, fmt, args);
	va_end(args);
}

RGINLINE void rg_arch_print(FILE* ostream)
{
	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR "arch: ");
#if defined(RG_MATH_SSE)
	rg_math_io_printf(ostream, "x86 SSE");
#if defined(RG_MATH_SSE41)
	rg_math_io_printf(ostream, " SSE4.1");
#endif
#if defined(RG_MATH_AVX)
	rg_math_io_printf(ostream, " AVX");
#endif
#else
	rg_math_io_printf(ostream, "scalar");
#endif
	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR_RESET);
}

RGINLINE void rg_arch_print_name(FILE* ostream)
{
	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR "\nrg_math ");
	rg_arch_print(ostream);
	rg_math_io_printf(ostream, "\n\n" RG_MATH_PRINT_COLOR_RESET);
}

RGINLINE void rg_mat4_print(const rg_mat4* matrix, FILE* ostream)
{
	char buff[16];
	int cw[4] = {0, 0, 0, 0};
	rg_math_io_printf(ostream, "Matrix (float4x4): " RG_MATH_PRINT_COLOR "\n");

	for (int c = 0; c < 4; ++c)
	{
		for (int r = 0; r < 4; ++r)
		{
			f32 val = matrix->m[c * 4 + r];
			int cwi;
			if (val < RG_MATH_PRINT_MAX_TO_SHORT)
			{
				cwi = rg_snprintf(buff, sizeof(buff), "% .*f", RG_MATH_PRINT_PRECISION, (f64)val);
			}
			else
			{
				cwi = rg_snprintf(buff, sizeof(buff), "% g", (f64)val);
			}
			cw[c] = RG_MAX(cw[c], cwi);
		}
	}

	for (int r = 0; r < 4; ++r)
	{
		rg_math_io_printf(ostream, "  |");
		for (int c = 0; c < 4; ++c)
		{
			f32 val = matrix->m[c * 4 + r];
			if (val < RG_MATH_PRINT_MAX_TO_SHORT)
			{
				rg_math_io_printf(ostream, " % *.*f", cw[c], RG_MATH_PRINT_PRECISION, (f64)val);
			}
			else
			{
				rg_math_io_printf(ostream, " % *g", cw[c], (f64)val);
			}
		}
		rg_math_io_printf(ostream, "  |\n");
	}

	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR_RESET "\n");
}

RGINLINE void rg_mat3_print(const rg_mat3* matrix, FILE* ostream)
{
	char buff[16];
	int cw[3] = {0, 0, 0};
	rg_math_io_printf(ostream, "Matrix (float3x3): " RG_MATH_PRINT_COLOR "\n");

	for (int c = 0; c < 3; ++c)
	{
		for (int r = 0; r < 3; ++r)
		{
			f32 val = matrix->m[c * 4 + r];
			int cwi;
			if (val < RG_MATH_PRINT_MAX_TO_SHORT)
			{
				cwi = rg_snprintf(buff, sizeof(buff), "% .*f", RG_MATH_PRINT_PRECISION, (f64)val);
			}
			else
			{
				cwi = rg_snprintf(buff, sizeof(buff), "% g", (f64)val);
			}
			cw[c] = RG_MAX(cw[c], cwi);
		}
	}

	for (int r = 0; r < 3; ++r)
	{
		rg_math_io_printf(ostream, "  |");
		for (int c = 0; c < 3; ++c)
		{
			f32 val = matrix->m[c * 4 + r];
			if (val < RG_MATH_PRINT_MAX_TO_SHORT)
			{
				rg_math_io_printf(ostream, " % *.*f", cw[c], RG_MATH_PRINT_PRECISION, (f64)val);
			}
			else
			{
				rg_math_io_printf(ostream, " % *g", cw[c], (f64)val);
			}
		}
		rg_math_io_printf(ostream, "  |\n");
	}

	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR_RESET "\n");
}

RGINLINE void rg_mat2_print(const rg_mat2* matrix, FILE* ostream)
{
	char buff[16];
	int cw[2] = {0, 0};
	rg_math_io_printf(ostream, "Matrix (float2x2): " RG_MATH_PRINT_COLOR "\n");

	for (int c = 0; c < 2; ++c)
	{
		for (int r = 0; r < 2; ++r)
		{
			f32 val = matrix->m[c * 2 + r];
			int cwi;
			if (val < RG_MATH_PRINT_MAX_TO_SHORT)
			{
				cwi = rg_snprintf(buff, sizeof(buff), "% .*f", RG_MATH_PRINT_PRECISION, (f64)val);
			}
			else
			{
				cwi = rg_snprintf(buff, sizeof(buff), "% g", (f64)val);
			}
			cw[c] = RG_MAX(cw[c], cwi);
		}
	}

	for (int r = 0; r < 2; ++r)
	{
		rg_math_io_printf(ostream, "  |");
		for (int c = 0; c < 2; ++c)
		{
			f32 val = matrix->m[c * 2 + r];
			if (val < RG_MATH_PRINT_MAX_TO_SHORT)
			{
				rg_math_io_printf(ostream, " % *.*f", cw[c], RG_MATH_PRINT_PRECISION, (f64)val);
			}
			else
			{
				rg_math_io_printf(ostream, " % *g", cw[c], (f64)val);
			}
		}
		rg_math_io_printf(ostream, "  |\n");
	}

	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR_RESET "\n");
}

RGINLINE void rg_vec4_print(const rg_vec4* vec, FILE* ostream)
{
	rg_math_io_printf(ostream, "Vector (float4): " RG_MATH_PRINT_COLOR "\n  (");
	for (int i = 0; i < 4; ++i)
	{
		f32 val = vec->data[i];
		if (val < RG_MATH_PRINT_MAX_TO_SHORT)
		{
			rg_math_io_printf(ostream, " % .*f", RG_MATH_PRINT_PRECISION, (f64)val);
		}
		else
		{
			rg_math_io_printf(ostream, " % g", (f64)val);
		}
	}
	rg_math_io_printf(ostream, "  )" RG_MATH_PRINT_COLOR_RESET "\n\n");
}

RGINLINE void rg_vec3_print(const rg_vec3* vec, FILE* ostream)
{
	rg_math_io_printf(ostream, "Vector (float3): " RG_MATH_PRINT_COLOR "\n  (");
	for (int i = 0; i < 3; ++i)
	{
		f32 val = vec->data[i];
		if (val < RG_MATH_PRINT_MAX_TO_SHORT)
		{
			rg_math_io_printf(ostream, " % .*f", RG_MATH_PRINT_PRECISION, (f64)val);
		}
		else
		{
			rg_math_io_printf(ostream, " % g", (f64)val);
		}
	}
	rg_math_io_printf(ostream, "  )" RG_MATH_PRINT_COLOR_RESET "\n\n");
}

RGINLINE void rg_vec2_print(const rg_vec2* vec, FILE* ostream)
{
	rg_math_io_printf(ostream, "Vector (float2): " RG_MATH_PRINT_COLOR "\n  (");
	for (int i = 0; i < 2; ++i)
	{
		f32 val = vec->data[i];
		if (val < RG_MATH_PRINT_MAX_TO_SHORT)
		{
			rg_math_io_printf(ostream, " % .*f", RG_MATH_PRINT_PRECISION, (f64)val);
		}
		else
		{
			rg_math_io_printf(ostream, " % g", (f64)val);
		}
	}
	rg_math_io_printf(ostream, "  )" RG_MATH_PRINT_COLOR_RESET "\n\n");
}

RGINLINE void rg_vec4i_print(const rg_vec4i* vec, FILE* ostream)
{
	rg_math_io_printf(ostream, "Vector (int4): " RG_MATH_PRINT_COLOR "\n  (");
	for (int i = 0; i < 4; ++i)
	{
		rg_math_io_printf(ostream, " % d", vec->data[i]);
	}
	rg_math_io_printf(ostream, "  )" RG_MATH_PRINT_COLOR_RESET "\n\n");
}

RGINLINE void rg_vec3i_print(const rg_vec3i* vec, FILE* ostream)
{
	rg_math_io_printf(ostream, "Vector (int3): " RG_MATH_PRINT_COLOR "\n  (");
	for (int i = 0; i < 3; ++i)
	{
		rg_math_io_printf(ostream, " % d", vec->data[i]);
	}
	rg_math_io_printf(ostream, "  )" RG_MATH_PRINT_COLOR_RESET "\n\n");
}

RGINLINE void rg_vec2i_print(const rg_vec2i* vec, FILE* ostream)
{
	rg_math_io_printf(ostream, "Vector (int2): " RG_MATH_PRINT_COLOR "\n  (");
	for (int i = 0; i < 2; ++i)
	{
		rg_math_io_printf(ostream, " % d", vec->data[i]);
	}
	rg_math_io_printf(ostream, "  )" RG_MATH_PRINT_COLOR_RESET "\n\n");
}

RGINLINE void rg_quat_print(const rg_quat* quat, FILE* ostream)
{
	rg_math_io_printf(ostream, "Quaternion (float4): " RG_MATH_PRINT_COLOR "\n  (");
	for (int i = 0; i < 4; ++i)
	{
		f32 val = quat->data[i];
		if (val < RG_MATH_PRINT_MAX_TO_SHORT)
		{
			rg_math_io_printf(ostream, " % .*f", RG_MATH_PRINT_PRECISION, (f64)val);
		}
		else
		{
			rg_math_io_printf(ostream, " % g", (f64)val);
		}
	}
	rg_math_io_printf(ostream, "  )" RG_MATH_PRINT_COLOR_RESET "\n\n");
}

RGINLINE void rg_aabb_print(const rg_aabb* aabb, const char* tag, FILE* ostream)
{
	rg_math_io_printf(ostream, "AABB (%s): " RG_MATH_PRINT_COLOR "\n", tag ? tag : "f32");

	const rg_vec3* corners[2] = {&aabb->min, &aabb->max};
	for (int i = 0; i < 2; ++i)
	{
		rg_math_io_printf(ostream, "  (");
		for (int j = 0; j < 3; ++j)
		{
			f32 val = corners[i]->data[j];
			if (val < RG_MATH_PRINT_MAX_TO_SHORT)
			{
				rg_math_io_printf(ostream, " % .*f", RG_MATH_PRINT_PRECISION, (f64)val);
			}
			else
			{
				rg_math_io_printf(ostream, " % g", (f64)val);
			}
		}
		rg_math_io_printf(ostream, "  )\n");
	}

	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR_RESET "\n");
}

RGINLINE void rg_aabb2_print(const rg_aabb2* aabb, const char* tag, FILE* ostream)
{
	rg_math_io_printf(ostream, "AABB2 (%s): " RG_MATH_PRINT_COLOR "\n", tag ? tag : "f32");

	const rg_vec2* corners[2] = {&aabb->min, &aabb->max};
	for (int i = 0; i < 2; ++i)
	{
		rg_math_io_printf(ostream, "  (");
		for (int j = 0; j < 2; ++j)
		{
			f32 val = corners[i]->data[j];
			if (val < RG_MATH_PRINT_MAX_TO_SHORT)
			{
				rg_math_io_printf(ostream, " % .*f", RG_MATH_PRINT_PRECISION, (f64)val);
			}
			else
			{
				rg_math_io_printf(ostream, " % g", (f64)val);
			}
		}
		rg_math_io_printf(ostream, "  )\n");
	}

	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR_RESET "\n");
}

RGINLINE void rg_sphere_print(const rg_sphere* sphere, FILE* ostream)
{
	rg_math_io_printf(ostream, "Sphere: " RG_MATH_PRINT_COLOR "\n");
	rg_math_io_printf(ostream, "  center: (");
	for (int i = 0; i < 3; ++i)
	{
		f32 val = sphere->center.data[i];
		if (val < RG_MATH_PRINT_MAX_TO_SHORT)
		{
			rg_math_io_printf(ostream, " % .*f", RG_MATH_PRINT_PRECISION, (f64)val);
		}
		else
		{
			rg_math_io_printf(ostream, " % g", (f64)val);
		}
	}
	rg_math_io_printf(ostream, "  )\n");

	if (sphere->radius < RG_MATH_PRINT_MAX_TO_SHORT)
	{
		rg_math_io_printf(ostream, "  radius: % .*f\n", RG_MATH_PRINT_PRECISION, (f64)sphere->radius);
	}
	else
	{
		rg_math_io_printf(ostream, "  radius: % g\n", (f64)sphere->radius);
	}

	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR_RESET "\n");
}

RGINLINE void rg_plane_print(const rg_plane* plane, FILE* ostream)
{
	rg_math_io_printf(ostream, "Plane: " RG_MATH_PRINT_COLOR "\n");
	rg_math_io_printf(ostream, "  normal: (");
	for (int i = 0; i < 3; ++i)
	{
		f32 val = plane->normal.data[i];
		if (val < RG_MATH_PRINT_MAX_TO_SHORT)
		{
			rg_math_io_printf(ostream, " % .*f", RG_MATH_PRINT_PRECISION, (f64)val);
		}
		else
		{
			rg_math_io_printf(ostream, " % g", (f64)val);
		}
	}
	rg_math_io_printf(ostream, "  )\n");

	if (plane->d < RG_MATH_PRINT_MAX_TO_SHORT)
	{
		rg_math_io_printf(ostream, "  d: % .*f\n", RG_MATH_PRINT_PRECISION, (f64)plane->d);
	}
	else
	{
		rg_math_io_printf(ostream, "  d: % g\n", (f64)plane->d);
	}

	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR_RESET "\n");
}

RGINLINE void rg_ray_print(const rg_ray* ray, FILE* ostream)
{
	rg_math_io_printf(ostream, "Ray: " RG_MATH_PRINT_COLOR "\n");
	rg_math_io_printf(ostream, "  origin: (");
	for (int i = 0; i < 3; ++i)
	{
		f32 val = ray->origin.data[i];
		if (val < RG_MATH_PRINT_MAX_TO_SHORT)
		{
			rg_math_io_printf(ostream, " % .*f", RG_MATH_PRINT_PRECISION, (f64)val);
		}
		else
		{
			rg_math_io_printf(ostream, " % g", (f64)val);
		}
	}
	rg_math_io_printf(ostream, "  )\n");

	rg_math_io_printf(ostream, "  dir:    (");
	for (int i = 0; i < 3; ++i)
	{
		f32 val = ray->dir.data[i];
		if (val < RG_MATH_PRINT_MAX_TO_SHORT)
		{
			rg_math_io_printf(ostream, " % .*f", RG_MATH_PRINT_PRECISION, (f64)val);
		}
		else
		{
			rg_math_io_printf(ostream, " % g", (f64)val);
		}
	}
	rg_math_io_printf(ostream, "  )\n");

	rg_math_io_printf(ostream, RG_MATH_PRINT_COLOR_RESET "\n");
}
#else
#define rg_arch_print(s) \
	do                   \
	{                    \
		(void)(s);       \
	} while (0)
#define rg_arch_print_name(s) \
	do                        \
	{                         \
		(void)(s);            \
	} while (0)
#define rg_mat4_print(v, s) \
	do                      \
	{                       \
		(void)(v);          \
		(void)(s);          \
	} while (0)
#define rg_mat3_print(v, s) \
	do                      \
	{                       \
		(void)(v);          \
		(void)(s);          \
	} while (0)
#define rg_mat2_print(v, s) \
	do                      \
	{                       \
		(void)(v);          \
		(void)(s);          \
	} while (0)
#define rg_vec4_print(v, s) \
	do                      \
	{                       \
		(void)(v);          \
		(void)(s);          \
	} while (0)
#define rg_vec3_print(v, s) \
	do                      \
	{                       \
		(void)(v);          \
		(void)(s);          \
	} while (0)
#define rg_vec2_print(v, s) \
	do                      \
	{                       \
		(void)(v);          \
		(void)(s);          \
	} while (0)
#define rg_vec4i_print(v, s) \
	do                       \
	{                        \
		(void)(v);           \
		(void)(s);           \
	} while (0)
#define rg_vec3i_print(v, s) \
	do                       \
	{                        \
		(void)(v);           \
		(void)(s);           \
	} while (0)
#define rg_vec2i_print(v, s) \
	do                       \
	{                        \
		(void)(v);           \
		(void)(s);           \
	} while (0)
#define rg_quat_print(v, s) \
	do                      \
	{                       \
		(void)(v);          \
		(void)(s);          \
	} while (0)
#define rg_aabb_print(v, t, s) \
	do                         \
	{                          \
		(void)(v);             \
		(void)(t);             \
		(void)(s);             \
	} while (0)
#define rg_aabb2_print(v, t, s) \
	do                          \
	{                           \
		(void)(v);              \
		(void)(t);              \
		(void)(s);              \
	} while (0)
#define rg_sphere_print(v, s) \
	do                        \
	{                         \
		(void)(v);            \
		(void)(s);            \
	} while (0)
#define rg_plane_print(v, s) \
	do                       \
	{                        \
		(void)(v);           \
		(void)(s);           \
	} while (0)
#define rg_ray_print(v, s) \
	do                     \
	{                      \
		(void)(v);         \
		(void)(s);         \
	} while (0)
#endif

#endif // RG_MATH_IO_H
