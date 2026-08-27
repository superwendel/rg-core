// rg_hash benchmark suite
//
// Usage:
//   build.bat bench_hash
//
// Set RG_BENCH_DEPS to enable the optional stb_ds comparison.

#include "../src/rg_hash.h"

#ifdef RG_BENCH_STB_DS
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>
#endif

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <unordered_map>

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

typedef double (*HashBenchFn)(const uint32_t* keys, size_t count);

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

static double benchmark_median(HashBenchFn fn, const uint32_t* keys, size_t count)
{
    std::array<double, SAMPLE_COUNT> samples;

    fn(keys, count);
    for (size_t i = 0; i < samples.size(); i++)
    {
        samples[i] = fn(keys, count);
    }

    std::sort(samples.begin(), samples.end());
    return samples[SAMPLE_COUNT / 2];
}

RG_HASH_MAP_DEFINE(uint32_t, uint32_t, U32Map, rg_hash_u32, rg_hash_eq_u32);

#ifdef RG_BENCH_STB_DS
typedef struct StbU32Pair
{
    uint32_t key;
    uint32_t value;
} StbU32Pair;
#endif

static RgArena g_arena;

static uint32_t rng_next(uint32_t* state)
{
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static void fill_keys(std::vector<uint32_t>& keys)
{
    for (size_t i = 0; i < keys.size(); i++)
    {
        keys[i] = static_cast<uint32_t>(i * 2654435761u);
    }
}

static void shuffle_keys(std::vector<uint32_t>& keys, uint32_t seed)
{
    if (keys.empty())
    {
        return;
    }

    for (size_t i = keys.size() - 1; i > 0; i--)
    {
        seed = rng_next(&seed);
        size_t j = seed % (i + 1);
        uint32_t tmp = keys[i];
        keys[i] = keys[j];
        keys[j] = tmp;
    }
}

// =============================================================================
// rg_hash map benchmarks
// =============================================================================

static double bench_rg_map_insert(const uint32_t* keys, size_t count)
{
    rg_arena_reset(&g_arena);

    U32Map map;
    rg_hash_map_init(U32Map, &map, &g_arena);
    rg_hash_map_reserve(U32Map, &map, count);

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        rg_hash_map_put(U32Map, &map, keys[i], keys[i] ^ 0x9e3779b9u, NULL);
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(rg_hash_map_count(&map));
    return end - start;
}

static double bench_rg_map_find(const uint32_t* keys, size_t count)
{
    rg_arena_reset(&g_arena);

    U32Map map;
    rg_hash_map_init(U32Map, &map, &g_arena);
    rg_hash_map_reserve(U32Map, &map, count);

    for (size_t i = 0; i < count; i++)
    {
        rg_hash_map_put(U32Map, &map, keys[i], keys[i] ^ 0x9e3779b9u, NULL);
    }

    double start = get_time_ms();
    uint64_t acc = 0;
    for (size_t i = 0; i < count; i++)
    {
        uint32_t* value = rg_hash_map_get_ptr(U32Map, &map, keys[i]);
        if (value != NULL)
        {
            acc += *value;
        }
    }
    double end = get_time_ms();

    g_sink += acc;
    return end - start;
}

static double bench_rg_map_remove(const uint32_t* keys, size_t count)
{
    rg_arena_reset(&g_arena);

    U32Map map;
    rg_hash_map_init(U32Map, &map, &g_arena);
    rg_hash_map_reserve(U32Map, &map, count);

    for (size_t i = 0; i < count; i++)
    {
        rg_hash_map_put(U32Map, &map, keys[i], keys[i] ^ 0x9e3779b9u, NULL);
    }

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        rg_hash_map_remove(U32Map, &map, keys[i], NULL);
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(rg_hash_map_count(&map));
    return end - start;
}

// =============================================================================
// std::unordered_map benchmarks
// =============================================================================

static double bench_std_map_insert(const uint32_t* keys, size_t count)
{
    std::unordered_map<uint32_t, uint32_t> map;
    map.max_load_factor(0.7f);
    map.reserve(count);

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        map[keys[i]] = keys[i] ^ 0x9e3779b9u;
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(map.size());
    return end - start;
}

static double bench_std_map_find(const uint32_t* keys, size_t count)
{
    std::unordered_map<uint32_t, uint32_t> map;
    map.max_load_factor(0.7f);
    map.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        map[keys[i]] = keys[i] ^ 0x9e3779b9u;
    }

    double start = get_time_ms();
    uint64_t acc = 0;
    for (size_t i = 0; i < count; i++)
    {
        std::unordered_map<uint32_t, uint32_t>::const_iterator it = map.find(keys[i]);
        if (it != map.end())
        {
            acc += it->second;
        }
    }
    double end = get_time_ms();

    g_sink += acc;
    return end - start;
}

static double bench_std_map_remove(const uint32_t* keys, size_t count)
{
    std::unordered_map<uint32_t, uint32_t> map;
    map.max_load_factor(0.7f);
    map.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        map[keys[i]] = keys[i] ^ 0x9e3779b9u;
    }

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        map.erase(keys[i]);
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(map.size());
    return end - start;
}

// =============================================================================
// stb_ds benchmarks
// =============================================================================

#ifdef RG_BENCH_STB_DS
static double bench_stb_map_insert(const uint32_t* keys, size_t count)
{
    StbU32Pair* map = NULL;

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        hmput(map, keys[i], keys[i] ^ 0x9e3779b9u);
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(hmlen(map));
    hmfree(map);
    return end - start;
}

static double bench_stb_map_find(const uint32_t* keys, size_t count)
{
    StbU32Pair* map = NULL;
    for (size_t i = 0; i < count; i++)
    {
        hmput(map, keys[i], keys[i] ^ 0x9e3779b9u);
    }

    double start = get_time_ms();
    uint64_t acc = 0;
    for (size_t i = 0; i < count; i++)
    {
        int idx = hmgeti(map, keys[i]);
        if (idx >= 0)
        {
            acc += map[idx].value;
        }
    }
    double end = get_time_ms();

    g_sink += acc;
    hmfree(map);
    return end - start;
}

static double bench_stb_map_remove(const uint32_t* keys, size_t count)
{
    StbU32Pair* map = NULL;
    for (size_t i = 0; i < count; i++)
    {
        hmput(map, keys[i], keys[i] ^ 0x9e3779b9u);
    }

    double start = get_time_ms();
    for (size_t i = 0; i < count; i++)
    {
        hmdel(map, keys[i]);
    }
    double end = get_time_ms();

    g_sink += static_cast<uint64_t>(hmlen(map));
    hmfree(map);
    return end - start;
}
#endif

static void bench_hash_map(void)
{
    const size_t count = 500000;

    std::vector<uint32_t> keys(count);
    fill_keys(keys);
    shuffle_keys(keys, 0x31415926u);

    double rg_insert = benchmark_median(bench_rg_map_insert, keys.data(), count);
    double std_insert = benchmark_median(bench_std_map_insert, keys.data(), count);
#ifdef RG_BENCH_STB_DS
    double stb_insert = benchmark_median(bench_stb_map_insert, keys.data(), count);
#endif

    double rg_find = benchmark_median(bench_rg_map_find, keys.data(), count);
    double std_find = benchmark_median(bench_std_map_find, keys.data(), count);
#ifdef RG_BENCH_STB_DS
    double stb_find = benchmark_median(bench_stb_map_find, keys.data(), count);
#endif

    double rg_remove = benchmark_median(bench_rg_map_remove, keys.data(), count);
    double std_remove = benchmark_median(bench_std_map_remove, keys.data(), count);
#ifdef RG_BENCH_STB_DS
    double stb_remove = benchmark_median(bench_stb_map_remove, keys.data(), count);
#endif

    printf("Hash map (%zu entries):\n", count);
#ifdef RG_BENCH_STB_DS
    printf("  insert: rg %.2f ms | std %.2f ms | stb_ds %.2f ms\n",
           rg_insert, std_insert, stb_insert);
    printf("  find:   rg %.2f ms | std %.2f ms | stb_ds %.2f ms\n",
           rg_find, std_find, stb_find);
    printf("  remove: rg %.2f ms | std %.2f ms | stb_ds %.2f ms\n",
           rg_remove, std_remove, stb_remove);
#else
    printf("  insert: rg %.2f ms | std %.2f ms\n", rg_insert, std_insert);
    printf("  find:   rg %.2f ms | std %.2f ms\n", rg_find, std_find);
    printf("  remove: rg %.2f ms | std %.2f ms\n", rg_remove, std_remove);
#endif
}

// =============================================================================
// Main
// =============================================================================

int main(void)
{
    int processor_index = pin_benchmark_thread();

    printf("rg_hash benchmarks\n");
    printf("==================\n\n");
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

    bench_hash_map();

    rg_arena_free(&g_arena);
    rg_free();

    return 0;
}
