// rg_algo benchmark suite
//
// Usage:
//   build.bat bench_algo
//
// Set RG_BENCH_DEPS to enable optional quadsort and crumsort comparisons.

// Reports the median of seven samples after one warmup. Input preparation,
// allocation, and correctness validation are outside the timed region.

#include "../src/rg_algo.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

extern "C" {
#ifdef RG_BENCH_ALGO_EXTRAS
typedef int QuadsortCmp(const void* a, const void* b);
void quadsort_swap_int32(void* array, void* swap, size_t swap_size, size_t nmemb, QuadsortCmp* cmp);

typedef int CrumsortCmp(const void* a, const void* b);
void crumsort_swap_int32(void* array, void* swap, size_t swap_size, size_t nmemb, CrumsortCmp* cmp);
#endif
}

enum
{
    SAMPLE_COUNT = 7
};

static volatile uint64_t g_sink = 0;

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

static uint32_t rng_next(uint32_t* state)
{
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static int qsort_int_cmp(const void* a, const void* b)
{
    int lhs = *(const int*)a;
    int rhs = *(const int*)b;
    return (lhs > rhs) - (lhs < rhs);
}

#define RG_ALGO_INT_LESS(a, b) (*(a) < *(b))
#define RG_ALGO_INT_KEY(a) ((uint32_t)(*(a)) ^ 0x80000000u)
RG_ALGO_DEFINE(int, IntAlgo, RG_ALGO_INT_LESS);
RG_ALGO_RADIX_U32_DEFINE(int, IntRadix, RG_ALGO_INT_KEY);

typedef struct Record32
{
    uint32_t key;
    uint32_t id;
    uint64_t payload[3];
} Record32;

typedef struct Record64
{
    uint64_t key;
    uint32_t id;
    uint32_t flags;
    uint64_t payload[2];
} Record64;

static_assert(sizeof(Record32) == 32, "Record32 must remain a 32-byte benchmark record");
static_assert(sizeof(Record64) == 32, "Record64 must remain a 32-byte benchmark record");

#define RG_ALGO_RECORD_LESS(a, b) ((a)->key < (b)->key)
#define RG_ALGO_RECORD_KEY(a) ((a)->key)
RG_ALGO_DEFINE(Record32, RecordAlgo, RG_ALGO_RECORD_LESS);
RG_ALGO_RADIX_U32_DEFINE(Record32, RecordRadix, RG_ALGO_RECORD_KEY);

#define RG_ALGO_RECORD64_LESS(a, b) ((a)->key < (b)->key)
#define RG_ALGO_RECORD64_KEY(a) ((a)->key)
RG_ALGO_DEFINE(Record64, Record64Algo, RG_ALGO_RECORD64_LESS);
RG_ALGO_RADIX_U64_DEFINE(Record64, Record64Radix, RG_ALGO_RECORD64_KEY);

static int qsort_record_cmp(const void* a, const void* b)
{
    uint32_t lhs = ((const Record32*)a)->key;
    uint32_t rhs = ((const Record32*)b)->key;
    return (lhs > rhs) - (lhs < rhs);
}

static int qsort_record64_cmp(const void* a, const void* b)
{
    uint64_t lhs = ((const Record64*)a)->key;
    uint64_t rhs = ((const Record64*)b)->key;
    return (lhs > rhs) - (lhs < rhs);
}

template<typename T>
using SortFn = void (*)(T* data, size_t count, T* scratch, size_t scratch_count);

template<typename T>
struct SortImpl
{
    const char* name;
    SortFn<T> fn;
};

struct BenchResult
{
    const char* name;
    std::array<double, SAMPLE_COUNT> samples;
};

static double median(std::array<double, SAMPLE_COUNT> values)
{
    std::sort(values.begin(), values.end());
    return values[SAMPLE_COUNT / 2];
}

static size_t repetitions_for(size_t count, size_t target_items)
{
    size_t repetitions = target_items / count;
    return repetitions > 0 ? repetitions : 1;
}

template<typename T, typename ValidateFn>
static double run_sort_sample(const SortImpl<T>& impl, const std::vector<T>& base,
                              size_t repetitions, ValidateFn validate)
{
    const size_t count = base.size();
    std::vector<T> work(count * repetitions);
    std::vector<T> scratch(count);

    for (size_t run = 0; run < repetitions; run++)
    {
        std::copy(base.begin(), base.end(), work.begin() + run * count);
    }

    auto start = std::chrono::steady_clock::now();
    for (size_t run = 0; run < repetitions; run++)
    {
        impl.fn(work.data() + run * count, count, scratch.data(), scratch.size());
    }
    auto end = std::chrono::steady_clock::now();

    for (size_t run = 0; run < repetitions; run++)
    {
        const T* output = work.data() + run * count;
        if (!validate(output, count))
        {
            std::fprintf(stderr, "Correctness failure: %s (N=%zu)\n", impl.name, count);
            std::exit(2);
        }
    }

    double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
    return elapsed_ms / (double)repetitions;
}

template<typename T, typename ValidateFn>
static std::vector<BenchResult> benchmark_case(const std::vector<SortImpl<T>>& implementations,
                                                const std::vector<T>& base, size_t target_items,
                                                ValidateFn validate)
{
    const size_t repetitions = repetitions_for(base.size(), target_items);
    std::vector<BenchResult> results(implementations.size());

    for (size_t i = 0; i < implementations.size(); i++)
    {
        results[i].name = implementations[i].name;
        (void)run_sort_sample(implementations[i], base, repetitions, validate);
    }

    // Rotate the first implementation each sample to reduce fixed-order bias.
    for (size_t sample = 0; sample < SAMPLE_COUNT; sample++)
    {
        for (size_t step = 0; step < implementations.size(); step++)
        {
            size_t index = (step + sample) % implementations.size();
            results[index].samples[sample] =
                run_sort_sample(implementations[index], base, repetitions, validate);
        }
    }

    return results;
}

static void sort_rg_int(int* data, size_t count, int*, size_t)
{
    rg_algo_sort_IntAlgo(data, count);
}

static void sort_rg_radix_int(int* data, size_t count, int* scratch, size_t scratch_count)
{
    rg_algo_radix_sort_IntRadix(data, count, scratch, scratch_count);
}

static void sort_std_int(int* data, size_t count, int*, size_t)
{
    std::sort(data, data + count);
}

static void sort_qsort_int(int* data, size_t count, int*, size_t)
{
    qsort(data, count, sizeof(int), qsort_int_cmp);
}

#ifdef RG_BENCH_ALGO_EXTRAS
static void sort_quadsort_int(int* data, size_t count, int* scratch, size_t scratch_count)
{
    quadsort_swap_int32(data, scratch, scratch_count, count, qsort_int_cmp);
}

static void sort_crumsort_int(int* data, size_t count, int* scratch, size_t scratch_count)
{
    crumsort_swap_int32(data, scratch, scratch_count, count, qsort_int_cmp);
}
#endif

static void sort_rg_stable_int(int* data, size_t count, int* scratch, size_t scratch_count)
{
    rg_algo_stable_sort_IntAlgo(data, count, scratch, scratch_count);
}

static void sort_std_stable_int(int* data, size_t count, int*, size_t)
{
    std::stable_sort(data, data + count);
}

static std::vector<int> make_int_pattern(size_t count, const char* pattern)
{
    std::vector<int> data(count);
    uint32_t state = 0x12345678u ^ (uint32_t)count;

    for (size_t i = 0; i < count; i++)
    {
        data[i] = (int32_t)rng_next(&state);
    }

    if (std::strcmp(pattern, "sorted") == 0 ||
        std::strcmp(pattern, "reverse") == 0 ||
        std::strcmp(pattern, "nearly") == 0)
    {
        std::sort(data.begin(), data.end());
    }

    if (std::strcmp(pattern, "reverse") == 0)
    {
        std::reverse(data.begin(), data.end());
    }
    else if (std::strcmp(pattern, "nearly") == 0)
    {
        size_t swaps = std::max<size_t>(1, count / 100);
        for (size_t i = 0; i < swaps; i++)
        {
            size_t a = rng_next(&state) % count;
            size_t b = rng_next(&state) % count;
            std::swap(data[a], data[b]);
        }
    }
    else if (std::strcmp(pattern, "dupes256") == 0)
    {
        for (size_t i = 0; i < count; i++)
        {
            data[i] = (int)(rng_next(&state) & 255u) - 128;
        }
    }
    else if (std::strcmp(pattern, "all_equal") == 0)
    {
        std::fill(data.begin(), data.end(), 7);
    }

    return data;
}

static void print_int_results(size_t count, const char* pattern,
                              const std::vector<BenchResult>& results)
{
    std::printf("%-9zu %-11s", count, pattern);
    for (const BenchResult& result : results)
    {
        std::printf(" %12.3f", median(result.samples) * 1000.0);
    }
    std::printf("\n");
}

using NthFn = void (*)(int* data, size_t count, size_t nth);

struct NthImpl
{
    const char* name;
    NthFn fn;
};

static void nth_rg(int* data, size_t count, size_t nth)
{
    rg_algo_nth_element_IntAlgo(data, count, nth);
}

static void nth_std(int* data, size_t count, size_t nth)
{
    std::nth_element(data, data + nth, data + count);
}

static bool validate_nth(const int* data, size_t count, size_t nth, int expected)
{
    int pivot = data[nth];
    if (pivot != expected)
    {
        return false;
    }
    for (size_t i = 0; i < nth; i++)
    {
        if (pivot < data[i])
        {
            return false;
        }
    }
    for (size_t i = nth + 1; i < count; i++)
    {
        if (data[i] < pivot)
        {
            return false;
        }
    }
    g_sink += (uint32_t)pivot;
    return true;
}

static double run_nth_sample(const NthImpl& impl, const std::vector<int>& base,
                             size_t repetitions, size_t nth, int expected)
{
    const size_t count = base.size();
    std::vector<int> work(count * repetitions);
    for (size_t run = 0; run < repetitions; run++)
    {
        std::copy(base.begin(), base.end(), work.begin() + run * count);
    }

    auto start = std::chrono::steady_clock::now();
    for (size_t run = 0; run < repetitions; run++)
    {
        impl.fn(work.data() + run * count, count, nth);
    }
    auto end = std::chrono::steady_clock::now();

    for (size_t run = 0; run < repetitions; run++)
    {
        if (!validate_nth(work.data() + run * count, count, nth, expected))
        {
            std::fprintf(stderr, "Correctness failure: %s (N=%zu, nth=%zu)\n",
                         impl.name, count, nth);
            std::exit(2);
        }
    }

    double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
    return elapsed_ms / (double)repetitions;
}

static std::vector<BenchResult> benchmark_nth_case(const std::vector<NthImpl>& implementations,
                                                    const std::vector<int>& base, int expected)
{
    const size_t repetitions = repetitions_for(base.size(), 1u << 18u);
    const size_t nth = base.size() / 2;
    std::vector<BenchResult> results(implementations.size());

    for (size_t i = 0; i < implementations.size(); i++)
    {
        results[i].name = implementations[i].name;
        (void)run_nth_sample(implementations[i], base, repetitions, nth, expected);
    }

    for (size_t sample = 0; sample < SAMPLE_COUNT; sample++)
    {
        for (size_t step = 0; step < implementations.size(); step++)
        {
            size_t index = (step + sample) % implementations.size();
            results[index].samples[sample] =
                run_nth_sample(implementations[index], base, repetitions, nth, expected);
        }
    }
    return results;
}

static void print_nth_results(size_t count, const char* pattern,
                              const std::vector<BenchResult>& results)
{
    std::printf("%-9zu %-11s", count, pattern);
    for (const BenchResult& result : results)
    {
        std::printf(" %12.3f", median(result.samples) * 1000.0);
    }
    std::printf("\n");
}

static uint64_t mix64(uint64_t value)
{
    value ^= value >> 30;
    value *= UINT64_C(0xbf58476d1ce4e5b9);
    value ^= value >> 27;
    value *= UINT64_C(0x94d049bb133111eb);
    return value ^ (value >> 31);
}

struct RecordDigest
{
    uint64_t sum;
    uint64_t xor_value;
};

static RecordDigest digest_records(const Record32* data, size_t count)
{
    RecordDigest digest = {0, 0};
    for (size_t i = 0; i < count; i++)
    {
        uint64_t hash = mix64(((uint64_t)data[i].key << 32) | data[i].id);
        hash ^= mix64(data[i].payload[0]);
        hash ^= mix64(data[i].payload[1]);
        hash ^= mix64(data[i].payload[2]);
        digest.sum += hash;
        digest.xor_value ^= hash;
    }
    return digest;
}

static RecordDigest digest_records(const Record64* data, size_t count)
{
    RecordDigest digest = {0, 0};
    for (size_t i = 0; i < count; i++)
    {
        uint64_t hash = mix64(data[i].key) ^ mix64(((uint64_t)data[i].id << 32) | data[i].flags);
        hash ^= mix64(data[i].payload[0]);
        hash ^= mix64(data[i].payload[1]);
        digest.sum += hash;
        digest.xor_value ^= hash;
    }
    return digest;
}

static void sort_rg_record(Record32* data, size_t count, Record32*, size_t)
{
    rg_algo_sort_RecordAlgo(data, count);
}

static void sort_rg_radix_record(Record32* data, size_t count, Record32* scratch, size_t scratch_count)
{
    rg_algo_radix_sort_RecordRadix(data, count, scratch, scratch_count);
}

static void sort_std_record(Record32* data, size_t count, Record32*, size_t)
{
    std::sort(data, data + count, [](const Record32& a, const Record32& b) { return a.key < b.key; });
}

static void sort_qsort_record(Record32* data, size_t count, Record32*, size_t)
{
    qsort(data, count, sizeof(Record32), qsort_record_cmp);
}

static void sort_rg_stable_record(Record32* data, size_t count, Record32* scratch, size_t scratch_count)
{
    rg_algo_stable_sort_RecordAlgo(data, count, scratch, scratch_count);
}

static void sort_std_stable_record(Record32* data, size_t count, Record32*, size_t)
{
    std::stable_sort(data, data + count,
                     [](const Record32& a, const Record32& b) { return a.key < b.key; });
}

static std::vector<Record32> make_records(size_t count)
{
    std::vector<Record32> data(count);
    uint32_t state = 0x9e3779b9u ^ (uint32_t)count;
    for (size_t i = 0; i < count; i++)
    {
        data[i].key = rng_next(&state) & 0xffffu;
        data[i].id = (uint32_t)i;
        data[i].payload[0] = ((uint64_t)rng_next(&state) << 32) | rng_next(&state);
        data[i].payload[1] = ((uint64_t)rng_next(&state) << 32) | rng_next(&state);
        data[i].payload[2] = ((uint64_t)rng_next(&state) << 32) | rng_next(&state);
    }
    return data;
}

static void sort_rg_record64(Record64* data, size_t count, Record64*, size_t)
{
    rg_algo_sort_Record64Algo(data, count);
}

static void sort_rg_radix_record64(Record64* data, size_t count, Record64* scratch, size_t scratch_count)
{
    rg_algo_radix_sort_Record64Radix(data, count, scratch, scratch_count);
}

static void sort_std_record64(Record64* data, size_t count, Record64*, size_t)
{
    std::sort(data, data + count, [](const Record64& a, const Record64& b) { return a.key < b.key; });
}

static void sort_qsort_record64(Record64* data, size_t count, Record64*, size_t)
{
    qsort(data, count, sizeof(Record64), qsort_record64_cmp);
}

static void sort_rg_stable_record64(Record64* data, size_t count, Record64* scratch, size_t scratch_count)
{
    rg_algo_stable_sort_Record64Algo(data, count, scratch, scratch_count);
}

static void sort_std_stable_record64(Record64* data, size_t count, Record64*, size_t)
{
    std::stable_sort(data, data + count,
                     [](const Record64& a, const Record64& b) { return a.key < b.key; });
}

static std::vector<Record64> make_records64(size_t count)
{
    std::vector<Record64> data(count);
    uint32_t state = 0x243f6a88u ^ (uint32_t)count;
    for (size_t i = 0; i < count; i++)
    {
        data[i].key = ((uint64_t)rng_next(&state) << 32) | rng_next(&state);
        data[i].id = (uint32_t)i;
        data[i].flags = rng_next(&state);
        data[i].payload[0] = ((uint64_t)rng_next(&state) << 32) | rng_next(&state);
        data[i].payload[1] = ((uint64_t)rng_next(&state) << 32) | rng_next(&state);
    }
    return data;
}

static void print_record_results(size_t count, const std::vector<BenchResult>& results)
{
    std::printf("%-9zu", count);
    for (const BenchResult& result : results)
    {
        std::printf(" %12.3f", median(result.samples) * 1000.0);
    }
    std::printf("\n");
}

int main(void)
{
    int processor_index = pin_benchmark_thread();
    const size_t sizes[] = {64, 4096, 65536, 1u << 20u};
    const char* patterns[] = {"random", "sorted", "reverse", "nearly", "dupes256", "all_equal"};

    const std::vector<SortImpl<int>> int_implementations = {
        {"rg_sort", sort_rg_int},
        {"rg_radix", sort_rg_radix_int},
        {"std_sort", sort_std_int},
        {"qsort", sort_qsort_int},
#ifdef RG_BENCH_ALGO_EXTRAS
        {"quadsort", sort_quadsort_int},
        {"crumsort", sort_crumsort_int},
#endif
        {"rg_stable", sort_rg_stable_int},
        {"std_stable", sort_std_stable_int}
    };

    std::printf("rg_algo benchmarks\n");
    std::printf("==================\n");
    std::printf("One warmup + median of %d samples; allocation, input copy, and validation excluded.\n", SAMPLE_COUNT);
    std::printf("Caller scratch is preallocated; std::stable_sort internal allocation is timed.\n\n");
    if (processor_index >= 0)
    {
        std::printf("Benchmark thread pinned to logical processor %d.\n\n", processor_index);
    }

    std::printf("int32 sort median (microseconds per sort; lower is better)\n");
    std::printf("%-9s %-11s", "N", "pattern");
    for (const SortImpl<int>& impl : int_implementations)
    {
        std::printf(" %12s", impl.name);
    }
    std::printf("\n");

    for (size_t count : sizes)
    {
        for (const char* pattern : patterns)
        {
            std::vector<int> base = make_int_pattern(count, pattern);
            std::vector<int> expected = base;
            std::sort(expected.begin(), expected.end());

            auto validate = [&expected](const int* output, size_t output_count) {
                g_sink += (uint32_t)output[output_count / 2];
                return std::equal(output, output + output_count, expected.begin());
            };

            std::vector<BenchResult> results =
                benchmark_case(int_implementations, base, 1u << 18u, validate);
            print_int_results(count, pattern, results);
        }
    }

    const std::vector<NthImpl> nth_implementations = {
        {"rg_nth", nth_rg},
        {"std_nth", nth_std}
    };
    const char* nth_patterns[] = {"random", "dupes256"};

    std::printf("\nint32 nth_element median (microseconds per selection; nth=N/2)\n");
    std::printf("%-9s %-11s", "N", "pattern");
    for (const NthImpl& impl : nth_implementations)
    {
        std::printf(" %12s", impl.name);
    }
    std::printf("\n");

    for (size_t count : sizes)
    {
        for (const char* pattern : nth_patterns)
        {
            std::vector<int> base = make_int_pattern(count, pattern);
            std::vector<int> expected = base;
            std::sort(expected.begin(), expected.end());
            std::vector<BenchResult> results =
                benchmark_nth_case(nth_implementations, base, expected[count / 2]);
            print_nth_results(count, pattern, results);
        }
    }

    const std::vector<SortImpl<Record32>> record_implementations = {
        {"rg_sort", sort_rg_record},
        {"rg_radix", sort_rg_radix_record},
        {"std_sort", sort_std_record},
        {"qsort", sort_qsort_record},
        {"rg_stable", sort_rg_stable_record},
        {"std_stable", sort_std_stable_record}
    };

    std::printf("\n32-byte record sort, 16-bit game-style keys (microseconds per sort)\n");
    std::printf("%-9s", "N");
    for (const SortImpl<Record32>& impl : record_implementations)
    {
        std::printf(" %12s", impl.name);
    }
    std::printf("\n");

    for (size_t count : sizes)
    {
        std::vector<Record32> base = make_records(count);
        RecordDigest expected_digest = digest_records(base.data(), base.size());
        auto validate = [expected_digest](const Record32* output, size_t output_count) {
            for (size_t i = 1; i < output_count; i++)
            {
                if (output[i].key < output[i - 1].key)
                {
                    return false;
                }
            }
            RecordDigest actual = digest_records(output, output_count);
            g_sink += actual.sum;
            return actual.sum == expected_digest.sum && actual.xor_value == expected_digest.xor_value;
        };

        std::vector<BenchResult> results =
            benchmark_case(record_implementations, base, 1u << 16u, validate);
        print_record_results(count, results);
    }

    const std::vector<SortImpl<Record64>> record64_implementations = {
        {"rg_sort", sort_rg_record64},
        {"rg_radix", sort_rg_radix_record64},
        {"std_sort", sort_std_record64},
        {"qsort", sort_qsort_record64},
        {"rg_stable", sort_rg_stable_record64},
        {"std_stable", sort_std_stable_record64}
    };

    std::printf("\n32-byte record sort, full-range 64-bit hash keys (microseconds per sort)\n");
    std::printf("%-9s", "N");
    for (const SortImpl<Record64>& impl : record64_implementations)
    {
        std::printf(" %12s", impl.name);
    }
    std::printf("\n");

    for (size_t count : sizes)
    {
        std::vector<Record64> base = make_records64(count);
        RecordDigest expected_digest = digest_records(base.data(), base.size());
        auto validate = [expected_digest](const Record64* output, size_t output_count) {
            for (size_t i = 1; i < output_count; i++)
            {
                if (output[i].key < output[i - 1].key)
                {
                    return false;
                }
            }
            RecordDigest actual = digest_records(output, output_count);
            g_sink += actual.sum;
            return actual.sum == expected_digest.sum && actual.xor_value == expected_digest.xor_value;
        };

        std::vector<BenchResult> results =
            benchmark_case(record64_implementations, base, 1u << 16u, validate);
        print_record_results(count, results);
    }

    std::printf("\nchecksum: %llu\n", (unsigned long long)g_sink);
    return 0;
}
