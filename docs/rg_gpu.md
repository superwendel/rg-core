# rg_gpu

`rg_gpu.h` contains the SDL3 GPU conventions shared by `rg_text`, `rg_gui`,
and Reverse Gravity applications. It covers device and window setup, a mapped
upload ring, and fixed compiled-shader loading.

## Device and window setup

Initialize SDL video, create a window and GPU device, then claim the window for
that device. Device creation and window claiming can fail; SDL's error string
describes the reason.

```c
#include "rg_gpu.h"
#include "rg_sdl.h"
#include "rg_window.h"

int main(void)
{
	if (!rg_sdl_init(SDL_INIT_VIDEO))
		return 1;

	RgWindowDesc window_desc = {0};
	window_desc.title = "GPU application";
	window_desc.width = 1280;
	window_desc.height = 720;
	window_desc.flags = SDL_WINDOW_RESIZABLE;

	SDL_Window* window = rg_window_create(&window_desc);
	if (!window)
	{
		rg_sdl_quit();
		return 1;
	}

	RgGpuDeviceDesc device_desc = {0};
	device_desc.shader_formats = RG_GPU_DEFAULT_SHADER_FORMATS;
	device_desc.enable_debug = 1;

	SDL_GPUDevice* device = rg_gpu_device_create(&device_desc);
	if (!device || !rg_gpu_claim_window(device, window))
	{
		if (device)
			rg_gpu_device_destroy(device);
		rg_window_destroy(window);
		rg_sdl_quit();
		return 1;
	}

	// Create resources and run the render loop.

	rg_gpu_wait_idle(device);
	SDL_ReleaseWindowFromGPUDevice(device, window);
	rg_gpu_device_destroy(device);
	rg_window_destroy(window);
	rg_sdl_quit();
	return 0;
}
```

A zero `shader_formats` field also selects `RG_GPU_DEFAULT_SHADER_FORMATS`.
`name` is forwarded to SDL as the requested GPU driver name; leave it null for
SDL's default selection. Query a claimed window's swapchain format with
`rg_gpu_swapchain_format`.

`rg_gpu_wait_idle` is a synchronization tool, not a per-frame requirement. Use
it before releasing resources that may still be referenced by submitted GPU
work. The wrapper does not expose SDL's success value, so call SDL directly if
the application must handle a wait failure. Release a claimed window with
`SDL_ReleaseWindowFromGPUDevice` before destroying the device or window.

## Upload ring

`RgGpuUploadRing` creates one SDL upload transfer buffer and suballocates
aligned slices from its mapped storage. Initialize it once, then begin and end
one allocation batch at a time:

```c
RgGpuUploadRing uploads = {0};
if (!rg_gpu_upload_ring_init(&uploads, device, 4u * 1024u * 1024u))
	return 1;

rg_gpu_upload_ring_begin(&uploads, 1);

RgGpuUploadSlice slice;
if (rg_gpu_upload_ring_alloc(&uploads, sizeof(u32), 0, &slice))
{
	u32* value = (u32*)rg_gpu_upload_ring_ptr(&uploads, &slice);
	*value = 42u;

	// Record a copy from uploads.buffer at slice.offset.
}

rg_gpu_upload_ring_end(&uploads);

// Submit recorded copy commands before reusing uploaded data.

rg_gpu_wait_idle(device);
rg_gpu_upload_ring_destroy(&uploads);
```

`rg_gpu_upload_ring_begin` resets the offset to zero and maps the transfer
buffer. Its `cycle` argument is forwarded to SDL. Allocation returns zero when
the buffer is not mapped, the size is zero, or the remaining capacity is too
small; the output slice is cleared in those cases. A zero alignment selects
`RG_GPU_UPLOAD_RING_DEFAULT_ALIGN`; nonzero alignment must be a power of two.

The ring only manages mapped CPU storage. The caller still records and submits
SDL copy commands, observes GPU synchronization, and avoids overwriting data
that remains in use. Destroy the ring before its GPU device.

## Compiled shader layout

`rg_gpu_shader_load` and `rg_gpu_compute_pipeline_load` choose the format
supported by the active SDL GPU backend. A shader root has this layout:

```text
shaders/
  Compiled/
    DXIL/name.dxil
    SPIRV/name.spv
    MSL/name.msl
```

Create the format directories before compiling. With the
[`shadercross`](https://github.com/libsdl-org/SDL_shadercross) command-line
tool, source names containing `.vert`, `.frag`, or `.comp` let the tool infer
the stage, while the output extension selects the target format:

```sh
shadercross shaders/sprite.vert.hlsl -o shaders/Compiled/DXIL/sprite.vert.dxil
shadercross shaders/sprite.vert.hlsl -o shaders/Compiled/SPIRV/sprite.vert.spv
shadercross shaders/sprite.vert.hlsl -o shaders/Compiled/MSL/sprite.vert.msl
```

Pass `--stage vertex`, `--stage fragment`, or `--stage compute` when the source
name does not encode the stage. The CLI can also reflect SPIR-V into JSON, which
is useful for checking the resource counts supplied in the descriptor:

```sh
shadercross shaders/Compiled/SPIRV/sprite.vert.spv \
  -o shaders/Compiled/sprite.vert.json
```

Pass `shaders` as the root and `name` in the descriptor. Shader resource
counts are explicit because SDL requires them when creating a shader or
compute pipeline.

```c
#include "rg_gpu.h"

RgGpuShaderDesc desc = {0};
desc.name = "sprite.vert";
desc.stage = SDL_GPU_SHADERSTAGE_VERTEX;
desc.uniform_buffer_count = 1u;

SDL_GPUShader* shader = rg_gpu_shader_load(device, "shaders", &desc);
```

Format selection follows this priority among the formats reported by the
device: SPIR-V, DXIL, then MSL. SPIR-V and DXIL use the entry point `main`; MSL
uses `main0`. The descriptor's sampler, storage, and uniform-buffer counts must
match the compiled shader. A load or SDL creation failure returns null and can
be diagnosed through `SDL_GetError` or `rg_sdl_error`.

The compute loader follows the same path and format rules. Its descriptor also
specifies read-only/read-write resources and the shader's thread-group size.

Generated files under `Compiled` are build output and should not be committed.
Keep the shader source in version control and rebuild the backend files with
SDL_shadercross. Its runtime dependencies vary by enabled output format; follow
the tool's release documentation when installing it or distributing a build
pipeline.

## API summary

| Area | Functions |
| --- | --- |
| Device | `rg_gpu_device_create`, `rg_gpu_device_destroy`, `rg_gpu_wait_idle` |
| Window | `rg_gpu_claim_window`, `rg_gpu_swapchain_format` |
| Uploads | `rg_gpu_upload_ring_init`, `rg_gpu_upload_ring_begin`, `rg_gpu_upload_ring_alloc`, `rg_gpu_upload_ring_ptr`, `rg_gpu_upload_ring_end`, `rg_gpu_upload_ring_destroy` |
| Shaders | `rg_gpu_shader_load`, `rg_gpu_compute_pipeline_load` |

Shader and pipeline objects returned by the loaders are caller-owned; release
them with `SDL_ReleaseGPUShader` and `SDL_ReleaseGPUComputePipeline`. The
wrapper does not create render pipelines, command buffers, copy passes, or
render passes.

## Configuration and thread use

Define options before including the header:

```c
#define RG_GPU_DEFAULT_SHADER_FORMATS SDL_GPU_SHADERFORMAT_SPIRV
#define RG_GPU_UPLOAD_RING_DEFAULT_ALIGN 256u
#define RG_GPU_SHADER_PATH_CAPACITY 2048u
#define RG_GPU_ASSERT(condition) my_assert(condition)
#include "rg_gpu.h"
```

Shader paths that do not fit `RG_GPU_SHADER_PATH_CAPACITY` fail with an SDL
error. The header has normal include guards and all functions have internal
linkage. Follow SDL's GPU synchronization and threading requirements; in
particular, claim a window from the thread that created it.
