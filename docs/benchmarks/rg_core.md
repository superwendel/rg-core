# rg_core benchmark results

These benchmarks exercise the public `rg_algo`, `rg_hash`, and
`rg_containers` headers. They were adapted from the original `rg-core-lab`
harnesses and rerun against the source in this repository.

Values are the median of three process runs. Within each process, the
benchmark thread is pinned to one logical CPU, each case receives one warmup,
and the reported value is the median of seven samples. Lower elapsed time is
better.

All implementations in a benchmark are built into the same C++17 executable
with MSVC 19.44.35217 for x64 using `/O2 /Ob3 /Oi /Ot /Oy /GL /arch:AVX2
/fp:fast /GS- /DNDEBUG`. Measurements were taken on an AMD Ryzen 9
4900HS on Windows build 26200.9168 on August 27, 2026.

## rg_algo

Allocation, input copying, and result validation are outside the timed region.
Radix and stable sorts receive preallocated caller scratch. The optional
quadsort and crumsort comparisons use their `int32` caller-scratch entry points
with the same full-size scratch allocation; their default allocating entry
points are not measured. Allocation inside `std::stable_sort` remains timed.
Each row sorts 1,048,576 values.

| Workload | rg sort | rg radix | `std::sort` | `qsort` | quadsort | crumsort |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Random `int32` | 74.63 ms | **15.38 ms** | 89.63 ms | 142.05 ms | 37.95 ms | 26.91 ms |
| 32-byte records, 16-bit keys | 77.42 ms | **21.05 ms** | 81.67 ms | 213.34 ms | n/a | n/a |
| 32-byte records, 64-bit keys | **81.84 ms** | 115.80 ms | 94.21 ms | 273.88 ms | n/a | n/a |

For random integers, the comparison-to-radix elapsed-time ratios were 5.8x for
`std::sort` and 9.2x for `qsort`; the radix path used 17% and 11% of their
elapsed time, respectively. For the 16-bit record workload, the
`std::sort`-to-radix ratio was 3.9x. Full-range 64-bit record keys favor the
comparison sort with the current global eight-bit radix configuration.

The comparison sort's monotonic fast paths took 0.56 ms for already sorted
input and 0.98 ms for reverse-ordered input. `std::sort` took 18.66 ms and
17.44 ms for the same cases.

At one million random integers, `rg_algo_nth_element` took 7.59 ms versus 9.19
ms for `std::nth_element`. With 256 distinct values it took 5.93 ms versus 8.95
ms.

The optional comparison build used quadsort and crumsort 1.2.1.3. Only their
`int32` paths are reported; the benchmarked interfaces do not provide the same
arbitrary-record, allocation-free contract as `RG_ALGO_DEFINE`.

### Comparison-sort context

Quadsort and crumsort are strong specialized comparison sorts, but they are not
interchangeable with every `rg_algo` path. Quadsort is stable and merge-based.
Crumsort is an adaptive partitioning sort that uses quadsort for suitable
subranges. Their default entry points use auxiliary storage and obtain it
dynamically on MSVC; this benchmark instead gives their `int32` caller-scratch
entry points preallocated memory.

On random `int32` input, crumsort and quadsort took 26.91 ms and 37.95 ms,
respectively, versus 74.63 ms for the allocation-free `rg_algo_sort` path. The
integer-key radix path remained fastest at 15.38 ms. The built-in comparison
sort led on already sorted and nearly sorted input, and it accepts arbitrary C
object types without allocation or caller scratch. The pinned external
implementations' primary generic dispatch supports a limited set of element
widths; quadsort provides a separate arbitrary-size path that performs
additional allocations.

The comparison libraries are therefore useful references and may be good
project-specific choices when their storage and type constraints fit. They are
not wholesale replacements for the general comparison, stable comparison, and
integer-key radix contracts exposed by `rg_algo`.

## rg_hash

The map benchmark inserts, finds, or removes 500,000 shuffled, unique `uint32`
keys. Map construction is outside find and remove timings. `rg_hash` and
`std::unordered_map` reserve for the full input before insert timing;
`stb_ds` 0.67 grows through its public map API. The standard map uses a 0.7
maximum load factor to match `rg_hash`.

| Operation | rg_hash | `std::unordered_map` | `stb_ds` 0.67 |
| --- | ---: | ---: | ---: |
| Insert | **25.81 ms** | 79.91 ms | 53.83 ms |
| Find | **16.08 ms** | 24.71 ms | 38.04 ms |
| Remove | **25.97 ms** | 78.30 ms | 79.11 ms |

The `std::unordered_map`-to-`rg_hash` elapsed-time ratios were 3.1x for
insertion and 3.0x for removal. The `stb_ds`-to-`rg_hash` ratios were 2.1x for
insertion and 3.0x for removal.

## rg_containers

Setup and explicit reserve calls are outside the timed region. The growing
array case starts each implementation empty. Because arena growth leaves old
blocks until reset while the standard containers release superseded storage,
the growing cases compare the intended game-frame workload rather than
identical allocation semantics.

| Workload | rg_containers | Comparison |
| --- | ---: | ---: |
| One million array pushes, reserved | **0.69 ms** | `std::vector`: 1.48 ms; `stb_ds`: 1.32 ms |
| One million array pushes, growing | **1.06 ms** | `std::vector`: 3.56 ms; `stb_ds`: 2.11 ms |
| Two million ring push/pop pairs | **4.44 ms** | `std::deque`: 6.05 ms |
| 200,000 sparse-set inserts | **0.38 ms** | EnTT 3.13.2: 2.01 ms |
| 200,000 sparse-set removals | **0.20 ms** | EnTT 3.13.2: 1.70 ms |

The `std::vector`-to-`rg_array` elapsed-time ratio was 2.1x for the reserved
case. The EnTT-to-`rg_sparse_set` ratios were 5.3x for insertion and 8.5x for
removal.

The harness also measures the inline-storage advantage of `rg_smallvec`: eight
pushes across 200,000 fresh containers took 1.82 ms versus 67.86 ms for an
unreserved `std::vector`. This case intentionally measures inline storage
against repeated heap allocation and is not used as a general container
speedup claim.

## Reproducing the benchmarks

From a Visual Studio Developer Command Prompt, reproduce the documented
three-process outer medians with:

```bat
build.bat bench_median
```

`build.bat bench` builds the same executables and runs each once for a quicker
diagnostic pass. The median runner builds once, launches three fresh processes
per benchmark, and aggregates the per-process medians into Markdown output.

The default build compares against the C and C++ standard libraries and needs
no additional source. Optional comparison libraries can be enabled by setting
`RG_BENCH_DEPS` to a directory containing:

- `quadsort.h` and `crumsort.h`;
- `stb_ds.h`;
- `entt/single_include/entt/entt.hpp`.

Then run the median command:

```bat
set RG_BENCH_DEPS=C:\path\to\benchmark-dependencies
build.bat bench_median
```

Individual one-process targets are `bench_algo`, `bench_hash`, and
`bench_containers`. The runner can also be invoked directly as
`tools\bench_core_median.ps1 -Runs 3`; pass `-OutputPath path.md` to save its
Markdown output.

Results are machine- and workload-specific and should not be treated as a
performance guarantee.
