// rg_containers benchmark suite
//
// Usage:
//   build.bat bench_containers
//
// Set RG_BENCH_DEPS to enable optional stb_ds and EnTT comparisons.

#include "../src/rg_containers.h"

#ifdef RG_BENCH_STB_DS
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#endif

#ifdef RG_BENCH_ENTT
#include <entt/entt.hpp>
#endif

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdint>
#include <deque>
#include <vector>

// Windows high-resolution timer
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static double get_time_ms(void)
{
    static LARGE_INTEGER freq = {0};
    if (freq.QuadPart == 0)
    {
        QueryPerformanceFrequency(&freq);
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return static_cast<double>(counter.QuadPart) * 1000.0 / static_cast<double>(freq.QuadPart);
}
#else
#include <time.h>
static double get_time_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}
#endif

static volatile uint64_t g_sink = 0;

enum
{
    SAMPLE_COUNT = 7
};

static int pin_benchmark_thread(void)
{
#ifdef _WIN32
    DWORD processor_count = GetActiveProcessorCount(0);
    DWORD processor_index = processor_count > 2u ? 2u : 0u;
    DWORD_PTR mask = (DWORD_PTR)1u << processor_index;
    if (SetThreadAffinityMask(GetCurrentThread(), mask) == 0u)
    {
        return -1;
    }
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    return (int)processor_index;
#else
    return -1;
#endif
}

template<typename Fn>
static double benchmark_median(Fn fn)
{
    std::array<double, SAMPLE_COUNT> samples;

    fn();
    for (size_t i = 0; i < samples.size(); i++)
    {
        samples[i] = fn();
    }

    std::sort(samples.begin(), samples.end());
    return samples[SAMPLE_COUNT / 2];
}

RG_ARRAY_DEFINE(int, IntArray);
RG_SMALLVEC_DEFINE(int, IntSmallVec, 8);
RG_RING_DEFINE(int, IntRing);
RG_SPARSE_SET_DEFINE(uint32_t, EntitySet);

static RgArena g_arena;

// =============================================================================
// Dynamic Array Benchmarks
// =============================================================================

static double bench_rg_array_push(size_t count, bool reserve)
{
    rg_arena_reset(&g_arena);

    IntArray arr;
    rg_array_init(&arr, &g_arena);
    if (reserve)
    {
        rg_array_reserve(int, &arr, count);
    }

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        rg_array_push(int, &arr, static_cast<int>(i));
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(arr.data[count - 1]);
    return end - start;
}

static double bench_std_vector_push(size_t count, bool reserve)
{
    std::vector<int> vec;
    if (reserve)
    {
        vec.reserve(count);
    }

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        vec.push_back(static_cast<int>(i));
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(vec.back());
    return end - start;
}

#ifdef RG_BENCH_STB_DS
static double bench_stb_ds_push(size_t count, bool reserve)
{
    int* arr = NULL;
    if (reserve)
    {
        arrsetcap(arr, count);
    }

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        arrpush(arr, static_cast<int>(i));
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(arr[count - 1]);
    arrfree(arr);
    return end - start;
}
#endif

static void bench_array_pushes(void)
{
    const size_t count = 1000000;

    double rg_reserve = benchmark_median([&]() { return bench_rg_array_push(count, true); });
    double std_reserve = benchmark_median([&]() { return bench_std_vector_push(count, true); });
#ifdef RG_BENCH_STB_DS
    double stb_reserve = benchmark_median([&]() { return bench_stb_ds_push(count, true); });
#endif

    double rg_grow = benchmark_median([&]() { return bench_rg_array_push(count, false); });
    double std_grow = benchmark_median([&]() { return bench_std_vector_push(count, false); });
#ifdef RG_BENCH_STB_DS
    double stb_grow = benchmark_median([&]() { return bench_stb_ds_push(count, false); });
#endif

    printf("Dynamic array push (%zu):\n", count);
#ifdef RG_BENCH_STB_DS
    printf("  reserve: rg %.2f ms | std %.2f ms | stb_ds %.2f ms\n",
           rg_reserve, std_reserve, stb_reserve);
    printf("  grow:    rg %.2f ms | std %.2f ms | stb_ds %.2f ms\n",
           rg_grow, std_grow, stb_grow);
#else
    printf("  reserve: rg %.2f ms | std %.2f ms\n", rg_reserve, std_reserve);
    printf("  grow:    rg %.2f ms | std %.2f ms\n", rg_grow, std_grow);
#endif
}

// =============================================================================
// Small Vector Benchmarks
// =============================================================================

static double bench_rg_smallvec_inline(size_t count, size_t repeat)
{
    rg_arena_reset(&g_arena);

    double start = get_time_ms();
    for (size_t r = 0; r < repeat; r++)
    {
        IntSmallVec vec;
        rg_smallvec_init(&vec, &g_arena);
        for (size_t i = 0; i < count; i++)
        {
            rg_smallvec_push(int, &vec, static_cast<int>(i));
        }
        g_sink += static_cast<uint64_t>(vec.data[count - 1]);
    }
    double end = get_time_ms();

    return end - start;
}

static double bench_std_vector_inline(size_t count, size_t repeat)
{
    double start = get_time_ms();
    for (size_t r = 0; r < repeat; r++)
    {
        std::vector<int> vec;
        for (size_t i = 0; i < count; i++)
        {
            vec.push_back(static_cast<int>(i));
        }
        g_sink += static_cast<uint64_t>(vec.back());
    }
    double end = get_time_ms();

    return end - start;
}

static void bench_smallvec_inline(void)
{
    const size_t count = 8;
    const size_t repeat = 200000;

    double rg_time = benchmark_median([&]() { return bench_rg_smallvec_inline(count, repeat); });
    double std_time = benchmark_median([&]() { return bench_std_vector_inline(count, repeat); });

    printf("Small-vector inline push (%zu x %zu):\n", count, repeat);
    printf("  rg_smallvec %.2f ms | std::vector %.2f ms\n", rg_time, std_time);
}

// =============================================================================
// Ring Buffer Benchmarks
// =============================================================================

static double bench_rg_ring_push_pop(size_t iters)
{
    rg_arena_reset(&g_arena);

    IntRing ring;
    rg_ring_init(int, &ring, &g_arena, 1024);

    double start = get_time_ms();
    for (size_t i = 0; i < iters; i++)
    {
        rg_ring_push(int, &ring, static_cast<int>(i));
        int out = 0;
        rg_ring_pop(int, &ring, &out);
        g_sink += static_cast<uint64_t>(out);
    }
    double end = get_time_ms();

    return end - start;
}

static double bench_std_deque_push_pop(size_t iters)
{
    std::deque<int> q;

    double start = get_time_ms();
    for (size_t i = 0; i < iters; i++)
    {
        q.push_back(static_cast<int>(i));
        g_sink += static_cast<uint64_t>(q.front());
        q.pop_front();
    }
    double end = get_time_ms();

    return end - start;
}

static void bench_ring_push_pop(void)
{
    const size_t iters = 2000000;

    double rg_time = benchmark_median([&]() { return bench_rg_ring_push_pop(iters); });
    double std_time = benchmark_median([&]() { return bench_std_deque_push_pop(iters); });

    printf("Ring push/pop (%zu):\n", iters);
    printf("  rg_ring %.2f ms | std::deque %.2f ms\n", rg_time, std_time);
}

// =============================================================================
// Sparse Set Benchmarks
// =============================================================================

static double bench_rg_sparse_insert(size_t count)
{
    rg_arena_reset(&g_arena);

    EntitySet set;
    rg_sparse_set_init(&set, &g_arena);
    rg_sparse_set_reserve_dense(uint32_t, &set, count);
    rg_sparse_set_reserve_sparse(&set, count);

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        rg_sparse_set_insert(uint32_t, &set, static_cast<uint32_t>(i));
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(rg_sparse_set_dense_count(&set));
    return end - start;
}

static double bench_rg_sparse_remove(size_t count)
{
    rg_arena_reset(&g_arena);

    EntitySet set;
    rg_sparse_set_init(&set, &g_arena);
    rg_sparse_set_reserve_dense(uint32_t, &set, count);
    rg_sparse_set_reserve_sparse(&set, count);

    for (size_t i = 0; i < count; i++)
    {
        rg_sparse_set_insert(uint32_t, &set, static_cast<uint32_t>(i));
    }

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        rg_sparse_set_remove(&set, static_cast<uint32_t>(i));
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(rg_sparse_set_dense_count(&set));
    return end - start;
}

#ifdef RG_BENCH_ENTT
static double bench_entt_sparse_insert(size_t count)
{
    entt::sparse_set set;
    set.reserve(count);

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        set.push(static_cast<entt::entity>(i));
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(set.size());
    return end - start;
}

static double bench_entt_sparse_remove(size_t count)
{
    entt::sparse_set set;
    set.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        set.push(static_cast<entt::entity>(i));
    }

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        set.remove(static_cast<entt::entity>(i));
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(set.size());
    return end - start;
}

static void bench_sparse_set(void)
{
    const size_t count = 200000;

    double rg_insert = benchmark_median([&]() { return bench_rg_sparse_insert(count); });
    double entt_insert = benchmark_median([&]() { return bench_entt_sparse_insert(count); });

    double rg_remove = benchmark_median([&]() { return bench_rg_sparse_remove(count); });
    double entt_remove = benchmark_median([&]() { return bench_entt_sparse_remove(count); });

    printf("Sparse set (%zu):\n", count);
    printf("  insert: rg %.2f ms | entt %.2f ms\n", rg_insert, entt_insert);
    printf("  remove: rg %.2f ms | entt %.2f ms\n", rg_remove, entt_remove);
}
#endif

// =============================================================================
// Main
// =============================================================================

int main(void)
{
    int processor_index = pin_benchmark_thread();

    printf("rg_containers benchmarks\n");
    printf("========================\n\n");
    printf("One warmup + median of %d samples; setup is outside the timed region.\n\n",
           SAMPLE_COUNT);
    if (processor_index >= 0)
    {
        printf("Benchmark thread pinned to logical processor %d.\n\n", processor_index);
    }

    if (rg_malloc(MB(128)) != 0)
    {
        printf("Failed to initialize rg_malloc pool.\n");
        return 1;
    }

    g_arena = rg_arena_create(MB(64));
    if (g_arena.memory == NULL)
    {
        printf("Failed to create benchmark arena.\n");
        rg_free();
        return 1;
    }

    bench_array_pushes();
    printf("\n");
    bench_smallvec_inline();
    printf("\n");
    bench_ring_push_pop();
#ifdef RG_BENCH_ENTT
    printf("\n");
    bench_sparse_set();
#endif

    rg_arena_free(&g_arena);
    rg_free();

    return 0;
}
