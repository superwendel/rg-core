# Benchmark reports

The benchmark suites measure public `rg-core` headers against established C
and C++ libraries in the same optimized executables. Results are specific to
the published hardware, compiler, configuration, and workloads; they are not
performance guarantees.

- [`rg_sprintf`](../rg_sprintf.md#performance) compares the portable and x64
  assembly formatters with `stb_sprintf` across integer, floating-point, string,
  and mixed game-style formats.
- [`rg_math`](../rg_math.md#performance) compares individual math operations
  with cglm and a weighted 3D-engine hot-path model across fully covered math
  libraries.
- [`rg_algo`, `rg_hash`, and `rg_containers`](rg-core.md) cover sorting,
  selection, integer-key hash maps, arrays, small vectors, rings, and sparse
  sets, including methodology and optional comparison-library versions.

The corresponding source harnesses are in [`benchmarks/`](../../benchmarks).
