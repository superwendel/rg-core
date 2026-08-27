// rg_sdl, rg_window, and rg_gpu integration tests

#include "../src/rg_sdl.h"
#include "../src/rg_window.h"
#include "../src/rg_gpu.h"

#include <stdio.h>
#include <string.h>

static int test_descriptors(void)
{
	RgWindowDesc window = {0};
	window.title = "rg-core";
	window.width = 1280;
	window.height = 720;
	window.flags = SDL_WINDOW_RESIZABLE;

	RgGpuDeviceDesc device = {0};
	device.shader_formats = RG_GPU_DEFAULT_SHADER_FORMATS;
	device.enable_debug = 1;

	RgGpuShaderDesc shader = {0};
	shader.name = "example.vert";
	shader.stage = SDL_GPU_SHADERSTAGE_VERTEX;
	shader.uniform_buffer_count = 1u;

	RgGpuComputePipelineDesc compute = {0};
	compute.name = "example.comp";
	compute.threadcount_x = 64u;
	compute.threadcount_y = 1u;
	compute.threadcount_z = 1u;

	return window.width == 1280 &&
	       device.shader_formats != SDL_GPU_SHADERFORMAT_INVALID &&
	       shader.uniform_buffer_count == 1u &&
	       compute.threadcount_x == 64u;
}

int main(void)
{
	if (!test_descriptors())
	{
		fprintf(stderr, "descriptor test failed\n");
		return 1;
	}

	if (!rg_sdl_init(0u))
	{
		fprintf(stderr, "SDL initialization failed: %s\n", rg_sdl_error());
		return 1;
	}

	rg_sdl_clear_error();
	if (rg_sdl_error()[0] != '\0')
	{
		fprintf(stderr, "SDL error clear failed\n");
		rg_sdl_quit();
		return 1;
	}

	rg_sdl_quit();
	puts("rg_sdl integration tests passed");
	return 0;
}
