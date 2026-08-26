// rg_math public API correctness tests

#include "../src/rg_math.h"
#include "../src/rg_math.h"

#include <math.h>
#include <stdio.h>

static int tests_run;
static int tests_failed;

static void check_condition(int condition, const char* expression, const char* file, int line)
{
	tests_run++;
	if (!condition)
	{
		printf("FAIL %s:%d: %s\n", file, line, expression);
		tests_failed++;
	}
}

#define CHECK(condition) check_condition(!!(condition), #condition, __FILE__, __LINE__)

#define CHECK_CLOSE(actual, expected, tolerance)               \
	do                                                         \
	{                                                          \
		f64 check_actual = (f64)(actual);                      \
		f64 check_expected = (f64)(expected);                  \
		f64 check_delta = fabs(check_actual - check_expected); \
		CHECK(check_delta <= (f64)(tolerance));                \
	} while (0)

static void check_vec3(const rg_vec3* value, f32 x, f32 y, f32 z, f32 tolerance)
{
	CHECK_CLOSE(value->x, x, tolerance);
	CHECK_CLOSE(value->y, y, tolerance);
	CHECK_CLOSE(value->z, z, tolerance);
}

static void test_layout(void)
{
	CHECK(sizeof(rg_vec2) == sizeof(f32) * 2);
	CHECK(sizeof(rg_vec3) == sizeof(f32) * 4);
	CHECK(sizeof(rg_vec4) == sizeof(f32) * 4);
	CHECK(sizeof(rg_mat4) == sizeof(f32) * 16);
	CHECK(sizeof(rg_quat) == sizeof(f32) * 4);
	CHECK(RG_ALIGNOF(rg_vec3) >= 16);
	CHECK(RG_ALIGNOF(rg_vec4) >= 16);
	CHECK(RG_ALIGNOF(rg_mat4) >= 16);
}

static void test_scalar(void)
{
	CHECK_CLOSE(rg_rad(180.0f), RG_PI, 1e-6f);
	CHECK_CLOSE(rg_deg(RG_PI), 180.0f, 1e-5f);
	CHECK_CLOSE(rg_clamp(2.0f, 0.0f, 1.0f), 1.0f, 1e-7f);
	CHECK_CLOSE(rg_lerp(-2.0f, 6.0f, 0.25f), 0.0f, 1e-7f);
	CHECK_CLOSE(rg_smoothstep(0.0f, 1.0f, 0.5f), 0.5f, 1e-7f);
	CHECK_CLOSE(rg_sqrtf(81.0f), 9.0f, 2e-3f);
	CHECK_CLOSE(rg_sinf(RG_HALF_PI), 1.0f, 1e-5f);
	CHECK_CLOSE(rg_cosf(RG_PI), -1.0f, 1e-5f);
	CHECK(rg_imin(-4, 3) == -4);
	CHECK(rg_imax(-4, 3) == 3);
}

static void test_vectors(void)
{
	rg_vec3 a;
	rg_vec3 b;
	rg_vec3 out;
	rg_vec3_set(&a, 1.0f, 2.0f, 3.0f);
	rg_vec3_set(&b, 4.0f, -5.0f, 6.0f);

	rg_vec3_add(&a, &b, &out);
	check_vec3(&out, 5.0f, -3.0f, 9.0f, 1e-6f);
	rg_vec3_sub(&a, &b, &out);
	check_vec3(&out, -3.0f, 7.0f, -3.0f, 1e-6f);
	rg_vec3_scale(&a, 2.0f, &out);
	check_vec3(&out, 2.0f, 4.0f, 6.0f, 1e-6f);
	CHECK_CLOSE(rg_vec3_dot(&a, &b), 12.0f, 1e-5f);

	rg_vec3_cross(&a, &b, &out);
	check_vec3(&out, 27.0f, 6.0f, -13.0f, 1e-5f);
	CHECK_CLOSE(rg_vec3_dot(&a, &out), 0.0f, 1e-5f);
	CHECK_CLOSE(rg_vec3_dot(&b, &out), 0.0f, 1e-5f);

	rg_vec3_set(&a, 3.0f, 4.0f, 0.0f);
	CHECK_CLOSE(rg_vec3_len(&a), 5.0f, 2e-3f);
	rg_vec3_normalize(&a, &out);
	check_vec3(&out, 0.6f, 0.8f, 0.0f, 2e-3f);
}

static void test_matrices(void)
{
	rg_mat4 identity;
	rg_mat4 translation;
	rg_mat4 inverse;
	rg_mat4 product;
	rg_vec3 offset;
	rg_vec3 point;
	rg_vec3 transformed;

	rg_mat4_identity(&identity);
	for (i32 column = 0; column < 4; column++)
	{
		for (i32 row = 0; row < 4; row++)
		{
			f32 expected = column == row ? 1.0f : 0.0f;
			CHECK_CLOSE(identity.m[column * 4 + row], expected, 1e-7f);
		}
	}

	rg_vec3_set(&offset, 5.0f, -2.0f, 3.0f);
	rg_mat4_translate_make(&translation, &offset);
	rg_vec3_set(&point, 1.0f, 2.0f, 3.0f);
	rg_mat4_mulv3(&translation, &point, 1.0f, &transformed);
	check_vec3(&transformed, 6.0f, 0.0f, 6.0f, 1e-5f);

	rg_mat4_inv(&translation, &inverse);
	rg_mat4_mul(&translation, &inverse, &product);
	for (i32 column = 0; column < 4; column++)
	{
		for (i32 row = 0; row < 4; row++)
		{
			f32 expected = column == row ? 1.0f : 0.0f;
			CHECK_CLOSE(product.m[column * 4 + row], expected, 2e-4f);
		}
	}
	CHECK_CLOSE(rg_mat4_det(&translation), 1.0f, 1e-5f);
}

static void test_quaternions_and_euler(void)
{
	rg_vec3 axis;
	rg_vec3 x_axis;
	rg_vec3 rotated;
	rg_vec3 angles;
	rg_quat rotation;
	rg_mat4 matrix;

	rg_vec3_set(&axis, 0.0f, 0.0f, 1.0f);
	rg_quat_from_axis_angle(RG_HALF_PI, &axis, &rotation);
	rg_vec3_set(&x_axis, 1.0f, 0.0f, 0.0f);
	rg_quat_rotatev(&rotation, &x_axis, &rotated);
	check_vec3(&rotated, 0.0f, 1.0f, 0.0f, 2e-3f);

	rg_vec3_set(&angles, 0.0f, 0.0f, 0.0f);
	rg_mat4_euler_xyz(&angles, &matrix);
	for (i32 i = 0; i < 16; i++)
	{
		f32 expected = (i % 5) == 0 ? 1.0f : 0.0f;
		CHECK_CLOSE(matrix.m[i], expected, 1e-6f);
	}
}

static void test_geometry(void)
{
	rg_vec3 origin;
	rg_vec3 direction;
	rg_vec3 center;
	rg_vec3 hit;
	rg_ray ray;
	rg_sphere sphere;
	f32 near_t = 0.0f;
	f32 far_t = 0.0f;

	rg_vec3_set(&origin, 0.0f, 0.0f, -5.0f);
	rg_vec3_set(&direction, 0.0f, 0.0f, 1.0f);
	rg_ray_set(&ray, &origin, &direction);
	rg_vec3_set(&center, 0.0f, 0.0f, 0.0f);
	rg_sphere_set(&sphere, &center, 1.0f);
	CHECK(rg_ray_sphere(&ray, &sphere, &near_t, &far_t));
	CHECK_CLOSE(near_t, 4.0f, 2e-3f);
	CHECK_CLOSE(far_t, 6.0f, 2e-3f);
	rg_ray_at(&ray, near_t, &hit);
	check_vec3(&hit, 0.0f, 0.0f, -1.0f, 2e-3f);
}

static void test_auxiliary_modules(void)
{
	rg_vec3 color;
	rg_vec2 noise_point;
	rg_mat4 projection;

	rg_vec3_set(&color, 1.0f, 1.0f, 1.0f);
	CHECK_CLOSE(rg_luminance(&color), 1.0f, 1e-6f);
	CHECK_CLOSE(rg_bezier(0.5f, 0.0f, 0.0f, 1.0f, 1.0f), 0.5f, 1e-6f);

	rg_vec2_set(&noise_point, 2.0f, -3.0f);
	CHECK_CLOSE(rg_perlin_vec2(&noise_point), 0.0f, 1e-6f);

	rg_mat4_perspective_default(16.0f / 9.0f, &projection);
	CHECK(projection.m[0] > 0.0f);
	CHECK(projection.m[5] > 0.0f);
	CHECK(projection.m[11] != 0.0f);
}

int main(void)
{
	test_layout();
	test_scalar();
	test_vectors();
	test_matrices();
	test_quaternions_and_euler();
	test_geometry();
	test_auxiliary_modules();

	printf("rg_math: %d checks, %d failures (%s)\n",
	       tests_run,
	       tests_failed,
#ifdef RG_MATH_SSE
	       "SIMD");
#else
	       "scalar");
#endif
	return tests_failed == 0 ? 0 : 1;
}
