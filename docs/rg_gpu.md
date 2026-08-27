# rg_gpu

`rg_gpu.h` contains the SDL3 GPU conventions shared by `rg_text`, `rg_gui`,
and Reverse Gravity applications. It covers device and window setup, a mapped
upload ring, and fixed compiled-shader loading.

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

Pass `shaders` as the root and `name` in the descriptor. Shader resource
counts are explicit because SDL requires them when creating a shader or
compute pipeline.

```c
RgGpuShaderDesc desc = {0};
desc.name = "sprite.vert";
desc.stage = SDL_GPU_SHADERSTAGE_VERTEX;
desc.uniform_buffer_count = 1u;

SDL_GPUShader* shader = rg_gpu_shader_load(device, "shaders", &desc);
```

Generated files under `Compiled` are build output and should not be committed.
Keep the shader source in version control and rebuild the backend files with
SDL_shadercross.
