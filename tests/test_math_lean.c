// rg_math lean umbrella compile smoke test

#define RG_MATH_NO_CAM
#define RG_MATH_NO_QUAT
#define RG_MATH_NO_EULER
#define RG_MATH_NO_COLOR
#define RG_MATH_NO_CURVE
#define RG_MATH_NO_GEOM
#define RG_MATH_NO_IO
#define RG_MATH_NO_NOISE
#include "../src/rg_math.h"
#include "../src/rg_math.h"

#include <stdio.h>

#if defined(RG_MATH_QUAT_H)
#error RG_MATH_NO_QUAT should omit rg_math_quat.h from rg_math.h
#endif

#if defined(RG_MATH_GEOM_H)
#error RG_MATH_NO_GEOM should omit rg_math_geom.h from rg_math.h
#endif

#if defined(RG_MATH_NOISE_H)
#error RG_MATH_NO_NOISE should omit rg_math_noise.h from rg_math.h
#endif

int main(void)
{
	rg_vec3 a;
	rg_vec3 b;
	rg_mat4 matrix;

	rg_vec3_set(&a, 1.0f, 2.0f, 3.0f);
	rg_vec3_scale(&a, 2.0f, &b);
	rg_mat4_identity(&matrix);

	f32 sum = b.x + b.y + b.z + matrix.m[0];
	if (sum != 13.0f)
	{
		printf("lean math smoke failed: %f\n", sum);
		return 1;
	}

	printf("lean math smoke passed\n");
	return 0;
}
