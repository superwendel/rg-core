// rg_gpu - SDL3 GPU helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header C99 helper for SDL3 GPU device setup, uploads, and shaders.
//
// USAGE:
//   #include "rg_gpu.h"
//
//   RgGpuDeviceDesc desc = {0};
//   desc.shader_formats = RG_GPU_DEFAULT_SHADER_FORMATS;
//   desc.enable_debug = 1;
//
//   SDL_GPUDevice* device = rg_gpu_device_create(&desc);
//   rg_gpu_claim_window(device, window);
//
// OPTIONS:
//   #define RG_GPU_DEFAULT_SHADER_FORMATS  - Default shader formats mask
//   #define RG_GPU_ASSERT(x)               - Custom assert macro (default: assert)
//   #define RG_GPU_UPLOAD_RING_DEFAULT_ALIGN - Default alignment for upload ring allocations (default: 16)
//
// NOTES:
//   - Shader files live under <shader_root>/Compiled/<format>/<name>.<ext>.
//   - All functions have internal linkage and work in unity builds.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_GPU_H
#define RG_GPU_H

#include <SDL3/SDL.h>

#include "rg_defs.h"

// =============================================================================
// CONFIGURATION
// =============================================================================

#ifndef RG_GPU_DEFAULT_SHADER_FORMATS
#define RG_GPU_DEFAULT_SHADER_FORMATS (SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL)
#endif

#ifndef RG_GPU_ASSERT
#include <assert.h>
#define RG_GPU_ASSERT(x) assert(x)
#endif

#ifndef RG_GPU_UPLOAD_RING_DEFAULT_ALIGN
#define RG_GPU_UPLOAD_RING_DEFAULT_ALIGN 16u
#endif

#ifndef RG_GPU_SHADER_PATH_CAPACITY
#define RG_GPU_SHADER_PATH_CAPACITY 1024u
#endif

// =============================================================================
// TYPES
// =============================================================================

typedef struct RgGpuDeviceDesc
{
	u32 shader_formats;
	int enable_debug;
	const char* name;
} RgGpuDeviceDesc;

typedef struct RgGpuUploadRing
{
	SDL_GPUDevice* device;
	SDL_GPUTransferBuffer* buffer;
	void* mapped;
	u32 size;
	u32 offset;
} RgGpuUploadRing;

typedef struct RgGpuUploadSlice
{
	u32 offset;
	u32 size;
} RgGpuUploadSlice;

typedef struct RgGpuShaderDesc
{
	const char* name;
	SDL_GPUShaderStage stage;
	u32 sampler_count;
	u32 storage_texture_count;
	u32 storage_buffer_count;
	u32 uniform_buffer_count;
} RgGpuShaderDesc;

typedef struct RgGpuComputePipelineDesc
{
	const char* name;
	u32 sampler_count;
	u32 readonly_storage_texture_count;
	u32 readonly_storage_buffer_count;
	u32 readwrite_storage_texture_count;
	u32 readwrite_storage_buffer_count;
	u32 uniform_buffer_count;
	u32 threadcount_x;
	u32 threadcount_y;
	u32 threadcount_z;
} RgGpuComputePipelineDesc;

// =============================================================================
// PUBLIC API
// =============================================================================

/**
 * @brief Create an SDL GPU device
 * @param desc Device descriptor
 * @return SDL_GPUDevice* or NULL on failure
 */
RGINLINE SDL_GPUDevice* rg_gpu_device_create(const RgGpuDeviceDesc* desc);

/**
 * @brief Destroy an SDL GPU device
 * @param device GPU device
 */
RGINLINE void rg_gpu_device_destroy(SDL_GPUDevice* device);

/**
 * @brief Claim a window for GPU rendering
 * @param device GPU device
 * @param window SDL window
 * @return 1 on success, 0 on failure
 */
RGINLINE int rg_gpu_claim_window(SDL_GPUDevice* device, SDL_Window* window);

/**
 * @brief Wait for GPU idle
 * @param device GPU device
 */
RGINLINE void rg_gpu_wait_idle(SDL_GPUDevice* device);

/**
 * @brief Get swapchain texture format for a window
 * @param device GPU device
 * @param window SDL window
 * @return Swapchain texture format
 */
RGINLINE SDL_GPUTextureFormat rg_gpu_swapchain_format(SDL_GPUDevice* device, SDL_Window* window);

/**
 * @brief Initialize an upload ring buffer for transfer uploads
 * @param ring Upload ring to initialize
 * @param device GPU device
 * @param size Total size in bytes
 * @return 1 on success, 0 on failure
 */
RGINLINE int rg_gpu_upload_ring_init(RgGpuUploadRing* ring, SDL_GPUDevice* device, u32 size);

/**
 * @brief Destroy an upload ring buffer
 * @param ring Upload ring to destroy
 */
RGINLINE void rg_gpu_upload_ring_destroy(RgGpuUploadRing* ring);

/**
 * @brief Begin a frame of allocations (maps the transfer buffer)
 * @param ring Upload ring
 * @param cycle Whether to request cycling if the buffer is still in use
 */
RGINLINE void rg_gpu_upload_ring_begin(RgGpuUploadRing* ring, int cycle);

/**
 * @brief End a frame of allocations (unmaps the transfer buffer)
 * @param ring Upload ring
 */
RGINLINE void rg_gpu_upload_ring_end(RgGpuUploadRing* ring);

/**
 * @brief Allocate a slice from the upload ring
 * @param ring Upload ring (must be mapped)
 * @param size Allocation size in bytes
 * @param align Alignment in bytes (power of two); 0 uses default
 * @param out_slice Output slice
 * @return 1 on success, 0 on failure
 */
RGINLINE int rg_gpu_upload_ring_alloc(RgGpuUploadRing* ring, u32 size, u32 align, RgGpuUploadSlice* out_slice);

/**
 * @brief Get CPU pointer for a slice
 * @param ring Upload ring (must be mapped)
 * @param slice Slice to access
 * @return Pointer to slice memory
 */
RGINLINE void* rg_gpu_upload_ring_ptr(const RgGpuUploadRing* ring, const RgGpuUploadSlice* slice);

/**
 * @brief Load a graphics shader selected for the active GPU backend
 * @param device GPU device
 * @param shader_root Directory containing the Compiled directory
 * @param desc Shader name, stage, and resource counts
 * @return Shader or NULL on failure
 */
RGINLINE SDL_GPUShader* rg_gpu_shader_load(SDL_GPUDevice* device,
                                           const char* shader_root,
                                           const RgGpuShaderDesc* desc);

/**
 * @brief Load and create a compute pipeline selected for the active GPU backend
 * @param device GPU device
 * @param shader_root Directory containing the Compiled directory
 * @param desc Shader name, resource counts, and thread-group size
 * @return Compute pipeline or NULL on failure
 */
RGINLINE SDL_GPUComputePipeline* rg_gpu_compute_pipeline_load(SDL_GPUDevice* device,
                                                              const char* shader_root,
                                                              const RgGpuComputePipelineDesc* desc);

// =============================================================================
// IMPLEMENTATION
// =============================================================================

RGINLINE SDL_GPUDevice* rg_gpu_device_create(const RgGpuDeviceDesc* desc)
{
	RG_GPU_ASSERT(desc != NULL);

	u32 formats = desc->shader_formats != 0u ? desc->shader_formats : RG_GPU_DEFAULT_SHADER_FORMATS;
	return SDL_CreateGPUDevice(formats, desc->enable_debug != 0, desc->name);
}

RGINLINE void rg_gpu_device_destroy(SDL_GPUDevice* device)
{
	RG_GPU_ASSERT(device != NULL);
	SDL_DestroyGPUDevice(device);
}

RGINLINE int rg_gpu_claim_window(SDL_GPUDevice* device, SDL_Window* window)
{
	RG_GPU_ASSERT(device != NULL);
	RG_GPU_ASSERT(window != NULL);
	return SDL_ClaimWindowForGPUDevice(device, window) ? 1 : 0;
}

RGINLINE void rg_gpu_wait_idle(SDL_GPUDevice* device)
{
	RG_GPU_ASSERT(device != NULL);
	SDL_WaitForGPUIdle(device);
}

RGINLINE SDL_GPUTextureFormat rg_gpu_swapchain_format(SDL_GPUDevice* device, SDL_Window* window)
{
	RG_GPU_ASSERT(device != NULL);
	RG_GPU_ASSERT(window != NULL);
	return SDL_GetGPUSwapchainTextureFormat(device, window);
}

RGINLINE int rg_gpu_upload_ring_init(RgGpuUploadRing* ring, SDL_GPUDevice* device, u32 size)
{
	RG_GPU_ASSERT(ring != NULL);
	RG_GPU_ASSERT(device != NULL);
	RG_GPU_ASSERT(size > 0u);

	ring->device = device;
	ring->size = size;
	ring->offset = 0u;
	ring->mapped = NULL;

	SDL_GPUTransferBufferCreateInfo info = {0};
	info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	info.size = size;
	ring->buffer = SDL_CreateGPUTransferBuffer(device, &info);
	return ring->buffer != NULL ? 1 : 0;
}

RGINLINE void rg_gpu_upload_ring_destroy(RgGpuUploadRing* ring)
{
	if (!ring)
	{
		return;
	}

	if (ring->buffer)
	{
		if (ring->mapped)
		{
			SDL_UnmapGPUTransferBuffer(ring->device, ring->buffer);
			ring->mapped = NULL;
		}
		SDL_ReleaseGPUTransferBuffer(ring->device, ring->buffer);
	}

	ring->buffer = NULL;
	ring->device = NULL;
	ring->size = 0u;
	ring->offset = 0u;
}

RGINLINE void rg_gpu_upload_ring_begin(RgGpuUploadRing* ring, int cycle)
{
	RG_GPU_ASSERT(ring != NULL);
	RG_GPU_ASSERT(ring->device != NULL);
	RG_GPU_ASSERT(ring->buffer != NULL);

	ring->offset = 0u;
	ring->mapped = SDL_MapGPUTransferBuffer(ring->device, ring->buffer, cycle != 0);
}

RGINLINE void rg_gpu_upload_ring_end(RgGpuUploadRing* ring)
{
	RG_GPU_ASSERT(ring != NULL);
	RG_GPU_ASSERT(ring->device != NULL);
	RG_GPU_ASSERT(ring->buffer != NULL);

	if (ring->mapped)
	{
		SDL_UnmapGPUTransferBuffer(ring->device, ring->buffer);
		ring->mapped = NULL;
	}
}

RGINLINE int rg_gpu_upload_ring_alloc(RgGpuUploadRing* ring, u32 size, u32 align, RgGpuUploadSlice* out_slice)
{
	RG_GPU_ASSERT(ring != NULL);
	RG_GPU_ASSERT(out_slice != NULL);

	if (!ring->mapped || size == 0u)
	{
		out_slice->offset = 0u;
		out_slice->size = 0u;
		return 0;
	}

	if (align == 0u)
	{
		align = RG_GPU_UPLOAD_RING_DEFAULT_ALIGN;
	}

	RG_GPU_ASSERT(RG_IS_POWER_OF_2(align));

	size_t aligned = RG_ALIGN_UP((size_t)ring->offset, (size_t)align);
	size_t end = aligned + (size_t)size;
	if (end > ring->size)
	{
		out_slice->offset = 0u;
		out_slice->size = 0u;
		return 0;
	}

	ring->offset = (u32)end;
	out_slice->offset = (u32)aligned;
	out_slice->size = size;
	return 1;
}

RGINLINE void* rg_gpu_upload_ring_ptr(const RgGpuUploadRing* ring, const RgGpuUploadSlice* slice)
{
	RG_GPU_ASSERT(ring != NULL);
	RG_GPU_ASSERT(slice != NULL);
	RG_GPU_ASSERT(ring->mapped != NULL);
	return (u8*)ring->mapped + slice->offset;
}

RGINLINE int rg_gpu_shader_file_info(SDL_GPUDevice* device,
                                     SDL_GPUShaderFormat* format,
                                     const char** folder,
                                     const char** extension,
                                     const char** entrypoint)
{
	SDL_GPUShaderFormat formats = SDL_GetGPUShaderFormats(device);
	if (formats & SDL_GPU_SHADERFORMAT_SPIRV)
	{
		*format = SDL_GPU_SHADERFORMAT_SPIRV;
		*folder = "SPIRV";
		*extension = "spv";
		*entrypoint = "main";
		return 1;
	}
	if (formats & SDL_GPU_SHADERFORMAT_DXIL)
	{
		*format = SDL_GPU_SHADERFORMAT_DXIL;
		*folder = "DXIL";
		*extension = "dxil";
		*entrypoint = "main";
		return 1;
	}
	if (formats & SDL_GPU_SHADERFORMAT_MSL)
	{
		*format = SDL_GPU_SHADERFORMAT_MSL;
		*folder = "MSL";
		*extension = "msl";
		*entrypoint = "main0";
		return 1;
	}

	return 0;
}

RGINLINE void* rg_gpu_shader_code_load(SDL_GPUDevice* device,
                                       const char* shader_root,
                                       const char* name,
                                       size_t* code_size,
                                       SDL_GPUShaderFormat* format,
                                       const char** entrypoint)
{
	const char* folder = NULL;
	const char* extension = NULL;
	if (!rg_gpu_shader_file_info(device, format, &folder, &extension, entrypoint))
	{
		return NULL;
	}

	char path[RG_GPU_SHADER_PATH_CAPACITY];
	int path_length = SDL_snprintf(path, sizeof(path), "%s/Compiled/%s/%s.%s",
	                               shader_root, folder, name, extension);
	if (path_length < 0 || (size_t)path_length >= sizeof(path))
	{
		SDL_SetError("GPU shader path exceeds RG_GPU_SHADER_PATH_CAPACITY");
		return NULL;
	}

	return SDL_LoadFile(path, code_size);
}

RGINLINE SDL_GPUShader* rg_gpu_shader_load(SDL_GPUDevice* device,
                                           const char* shader_root,
                                           const RgGpuShaderDesc* desc)
{
	RG_GPU_ASSERT(device != NULL);
	RG_GPU_ASSERT(shader_root != NULL);
	RG_GPU_ASSERT(desc != NULL);
	RG_GPU_ASSERT(desc->name != NULL);

	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char* entrypoint = NULL;
	size_t code_size = 0u;
	void* code = rg_gpu_shader_code_load(device, shader_root, desc->name,
	                                     &code_size, &format, &entrypoint);
	if (!code)
	{
		return NULL;
	}

	SDL_GPUShaderCreateInfo info = {0};
	info.code_size = code_size;
	info.code = (const u8*)code;
	info.entrypoint = entrypoint;
	info.format = format;
	info.stage = desc->stage;
	info.num_samplers = desc->sampler_count;
	info.num_storage_textures = desc->storage_texture_count;
	info.num_storage_buffers = desc->storage_buffer_count;
	info.num_uniform_buffers = desc->uniform_buffer_count;

	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &info);
	SDL_free(code);
	return shader;
}

RGINLINE SDL_GPUComputePipeline* rg_gpu_compute_pipeline_load(SDL_GPUDevice* device,
                                                              const char* shader_root,
                                                              const RgGpuComputePipelineDesc* desc)
{
	RG_GPU_ASSERT(device != NULL);
	RG_GPU_ASSERT(shader_root != NULL);
	RG_GPU_ASSERT(desc != NULL);
	RG_GPU_ASSERT(desc->name != NULL);

	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char* entrypoint = NULL;
	size_t code_size = 0u;
	void* code = rg_gpu_shader_code_load(device, shader_root, desc->name,
	                                     &code_size, &format, &entrypoint);
	if (!code)
	{
		return NULL;
	}

	SDL_GPUComputePipelineCreateInfo info = {0};
	info.code_size = code_size;
	info.code = (const u8*)code;
	info.entrypoint = entrypoint;
	info.format = format;
	info.num_samplers = desc->sampler_count;
	info.num_readonly_storage_textures = desc->readonly_storage_texture_count;
	info.num_readonly_storage_buffers = desc->readonly_storage_buffer_count;
	info.num_readwrite_storage_textures = desc->readwrite_storage_texture_count;
	info.num_readwrite_storage_buffers = desc->readwrite_storage_buffer_count;
	info.num_uniform_buffers = desc->uniform_buffer_count;
	info.threadcount_x = desc->threadcount_x;
	info.threadcount_y = desc->threadcount_y;
	info.threadcount_z = desc->threadcount_z;

	SDL_GPUComputePipeline* pipeline = SDL_CreateGPUComputePipeline(device, &info);
	SDL_free(code);
	return pipeline;
}

#endif // RG_GPU_H
