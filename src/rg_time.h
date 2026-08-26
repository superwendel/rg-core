// rg_time - Monotonic timing and thread sleep helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header C99 library for high-resolution monotonic timing, tick
// conversion, sleeping, and yielding.
//
// USAGE:
//   #include "rg_time.h"
//
//   rg_time_init(); // Before starting worker threads
//   u64 start = rg_time_ticks();
//   // work...
//   f64 elapsed_ms = rg_time_ticks_to_ms(rg_time_ticks() - start);
//
// OPTIONS:
//   #define RG_TIME_CUSTOM 1            - Use custom platform hooks
//   #define RG_TIME_PLATFORM_TICKS      - Custom tick function
//   #define RG_TIME_PLATFORM_FREQUENCY  - Custom frequency function
//   #define RG_TIME_PLATFORM_SLEEP_NS   - Custom nanosecond sleep function
//   #define RG_TIME_PLATFORM_YIELD      - Custom yield function
//
// NOTES:
//   - Time values are monotonic, not wall-clock timestamps.
//   - Call rg_time_init once per translation unit to cache the frequency.
//   - Custom hook macros name functions and are invoked by this header.
//   - All functions have internal linkage and work in unity builds.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_TIME_H
#define RG_TIME_H

#include "rg_defs.h"

#include <stdint.h>

#ifndef RG_TIME_CUSTOM
#define RG_TIME_CUSTOM 0
#endif

#if RG_TIME_CUSTOM
#ifndef RG_TIME_PLATFORM_TICKS
#error RG_TIME_CUSTOM requires RG_TIME_PLATFORM_TICKS
#endif
#ifndef RG_TIME_PLATFORM_FREQUENCY
#error RG_TIME_CUSTOM requires RG_TIME_PLATFORM_FREQUENCY
#endif
#ifndef RG_TIME_PLATFORM_SLEEP_NS
#error RG_TIME_CUSTOM requires RG_TIME_PLATFORM_SLEEP_NS
#endif
#ifndef RG_TIME_PLATFORM_YIELD
#error RG_TIME_CUSTOM requires RG_TIME_PLATFORM_YIELD
#endif
#else
#ifndef RG_TIME_PLATFORM_TICKS
#define RG_TIME_PLATFORM_TICKS rg_time_platform_ticks
#endif
#ifndef RG_TIME_PLATFORM_FREQUENCY
#define RG_TIME_PLATFORM_FREQUENCY rg_time_platform_frequency
#endif
#ifndef RG_TIME_PLATFORM_SLEEP_NS
#define RG_TIME_PLATFORM_SLEEP_NS rg_time_platform_sleep_ns
#endif
#ifndef RG_TIME_PLATFORM_YIELD
#define RG_TIME_PLATFORM_YIELD rg_time_platform_yield
#endif

#if RG_PLATFORM_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#elif RG_PLATFORM_LINUX
#include <errno.h>
#include <sched.h>
#include <time.h>
#elif RG_PLATFORM_MACOS
#include <errno.h>
#include <mach/mach_time.h>
#include <sched.h>
#include <time.h>
#else
#error rg_time requires RG_TIME_CUSTOM on unsupported platforms
#endif
#endif

// =============================================================================
// PUBLIC API
// =============================================================================

/**
 * @brief Cache the platform timer frequency for fast duration conversions
 * @note Call once per translation unit before starting threads that use the
 *       conversion or convenience functions. Repeated calls are inexpensive.
 */
RGINLINE void rg_time_init(void);

/**
 * @brief Return the current monotonic timer tick count
 * @return Monotonic timestamp in platform ticks
 */
RGINLINE u64 rg_time_ticks(void);

/**
 * @brief Return the monotonic timer frequency
 * @return Platform ticks per second, or zero if unavailable
 */
RGINLINE u64 rg_time_ticks_per_second(void);

/**
 * @brief Convert a tick duration to seconds
 * @param ticks Duration in platform ticks
 * @return Duration in seconds, or zero when frequency is unavailable
 */
RGINLINE f64 rg_time_ticks_to_seconds(u64 ticks);

/**
 * @brief Convert a tick duration to milliseconds
 * @param ticks Duration in platform ticks
 * @return Duration in milliseconds, or zero when frequency is unavailable
 */
RGINLINE f64 rg_time_ticks_to_ms(u64 ticks);

/**
 * @brief Convert a tick duration to microseconds
 * @param ticks Duration in platform ticks
 * @return Duration in microseconds, or zero when frequency is unavailable
 */
RGINLINE f64 rg_time_ticks_to_us(u64 ticks);

/**
 * @brief Convert seconds to platform ticks
 * @param seconds Duration in seconds
 * @return Duration in ticks, saturated to UINT64_MAX
 */
RGINLINE u64 rg_time_seconds_to_ticks(f64 seconds);

/**
 * @brief Convert milliseconds to platform ticks
 * @param ms Duration in milliseconds
 * @return Duration in ticks, saturated to UINT64_MAX
 */
RGINLINE u64 rg_time_ms_to_ticks(f64 ms);

/**
 * @brief Convert microseconds to platform ticks
 * @param us Duration in microseconds
 * @return Duration in ticks, saturated to UINT64_MAX
 */
RGINLINE u64 rg_time_us_to_ticks(f64 us);

/**
 * @brief Return monotonic time in seconds
 * @return Monotonic time in seconds
 */
RGINLINE f64 rg_time_seconds(void);

/**
 * @brief Return monotonic time in milliseconds
 * @return Monotonic time in milliseconds
 */
RGINLINE f64 rg_time_ms(void);

/**
 * @brief Return monotonic time in microseconds
 * @return Monotonic time in microseconds
 */
RGINLINE f64 rg_time_us(void);

/**
 * @brief Sleep for at least the requested number of nanoseconds
 * @param ns Requested duration in nanoseconds
 */
RGINLINE void rg_time_sleep_ns(u64 ns);

/**
 * @brief Sleep for at least the requested number of microseconds
 * @param us Requested duration in microseconds
 */
RGINLINE void rg_time_sleep_us(u64 us);

/**
 * @brief Sleep for at least the requested number of milliseconds
 * @param ms Requested duration in milliseconds
 */
RGINLINE void rg_time_sleep_ms(u32 ms);

/**
 * @brief Yield execution to another runnable thread
 */
RGINLINE void rg_time_yield(void);

// =============================================================================
// PLATFORM BACKENDS
// =============================================================================

#if !RG_TIME_CUSTOM
RGINLINE u64 rg_time_platform_ticks(void);
RGINLINE u64 rg_time_platform_frequency(void);
RGINLINE void rg_time_platform_sleep_ns(u64 ns);
RGINLINE void rg_time_platform_yield(void);

#if RG_PLATFORM_WINDOWS

RGINLINE u64 rg_time_platform_ticks(void)
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return (u64)counter.QuadPart;
}

RGINLINE u64 rg_time_platform_frequency(void)
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return (u64)frequency.QuadPart;
}

RGINLINE void rg_time_platform_sleep_ns(u64 ns)
{
	if (ns == 0u) return;

	u64 ms = ns / UINT64_C(1000000);
	if (ns % UINT64_C(1000000) != 0u) ms++;
	DWORD platform_ms = ms > UINT32_MAX ? UINT32_MAX : (DWORD)ms;
	Sleep(platform_ms);
}

RGINLINE void rg_time_platform_yield(void)
{
	SwitchToThread();
}

#elif RG_PLATFORM_LINUX

RGINLINE u64 rg_time_platform_ticks(void)
{
	struct timespec timestamp;
#ifdef CLOCK_MONOTONIC_RAW
	if (clock_gettime(CLOCK_MONOTONIC_RAW, &timestamp) != 0) return 0;
#else
	if (clock_gettime(CLOCK_MONOTONIC, &timestamp) != 0) return 0;
#endif
	return (u64)timestamp.tv_sec * UINT64_C(1000000000) +
	       (u64)timestamp.tv_nsec;
}

RGINLINE u64 rg_time_platform_frequency(void)
{
	return UINT64_C(1000000000);
}

RGINLINE void rg_time_platform_sleep_ns(u64 ns)
{
	if (ns == 0u) return;

	struct timespec request;
	request.tv_sec = (time_t)(ns / UINT64_C(1000000000));
	request.tv_nsec = (long)(ns % UINT64_C(1000000000));
	while (nanosleep(&request, &request) != 0 && errno == EINTR)
	{
	}
}

RGINLINE void rg_time_platform_yield(void)
{
	sched_yield();
}

#elif RG_PLATFORM_MACOS

RGINLINE u64 rg_time_platform_ticks(void)
{
	return (u64)mach_absolute_time();
}

RGINLINE u64 rg_time_platform_frequency(void)
{
	mach_timebase_info_data_t info;
	if (mach_timebase_info(&info) != KERN_SUCCESS || info.numer == 0u || info.denom == 0u)
	{
		return 0;
	}
	return (UINT64_C(1000000000) * (u64)info.denom) / (u64)info.numer;
}

RGINLINE void rg_time_platform_sleep_ns(u64 ns)
{
	if (ns == 0u) return;

	struct timespec request;
	request.tv_sec = (time_t)(ns / UINT64_C(1000000000));
	request.tv_nsec = (long)(ns % UINT64_C(1000000000));
	while (nanosleep(&request, &request) != 0 && errno == EINTR)
	{
	}
}

RGINLINE void rg_time_platform_yield(void)
{
	sched_yield();
}

#endif
#endif

// =============================================================================
// IMPLEMENTATION
// =============================================================================

static u64 rg_time_frequency_cache;

RGINLINE void rg_time_init(void)
{
	if (rg_time_frequency_cache != 0u) return;
	u64 frequency = (u64)RG_TIME_PLATFORM_FREQUENCY();
	if (frequency != 0u) rg_time_frequency_cache = frequency;
}

RGINLINE u64 rg_time_ticks(void)
{
	return (u64)RG_TIME_PLATFORM_TICKS();
}

RGINLINE u64 rg_time_ticks_per_second(void)
{
	u64 frequency = rg_time_frequency_cache;
	return frequency != 0u ? frequency : (u64)RG_TIME_PLATFORM_FREQUENCY();
}

RGINLINE f64 rg_time_ticks_to_seconds(u64 ticks)
{
	u64 frequency = rg_time_ticks_per_second();
	if (frequency == 0u) return 0.0;
	return (f64)ticks / (f64)frequency;
}

RGINLINE f64 rg_time_ticks_to_ms(u64 ticks)
{
	u64 frequency = rg_time_ticks_per_second();
	if (frequency == 0u) return 0.0;
	return (f64)ticks * 1000.0 / (f64)frequency;
}

RGINLINE f64 rg_time_ticks_to_us(u64 ticks)
{
	u64 frequency = rg_time_ticks_per_second();
	if (frequency == 0u) return 0.0;
	return (f64)ticks * 1000000.0 / (f64)frequency;
}

RGINLINE u64 rg_time_units_to_ticks(f64 value, f64 units_per_second)
{
	if (!(value > 0.0)) return 0;

	u64 frequency = rg_time_ticks_per_second();
	if (frequency == 0u) return 0;

	f64 ticks = value * (f64)frequency / units_per_second;
	if (ticks >= (f64)UINT64_MAX) return UINT64_MAX;
	return (u64)ticks;
}

RGINLINE u64 rg_time_seconds_to_ticks(f64 seconds)
{
	return rg_time_units_to_ticks(seconds, 1.0);
}

RGINLINE u64 rg_time_ms_to_ticks(f64 ms)
{
	return rg_time_units_to_ticks(ms, 1000.0);
}

RGINLINE u64 rg_time_us_to_ticks(f64 us)
{
	return rg_time_units_to_ticks(us, 1000000.0);
}

RGINLINE f64 rg_time_seconds(void)
{
	return rg_time_ticks_to_seconds(rg_time_ticks());
}

RGINLINE f64 rg_time_ms(void)
{
	return rg_time_ticks_to_ms(rg_time_ticks());
}

RGINLINE f64 rg_time_us(void)
{
	return rg_time_ticks_to_us(rg_time_ticks());
}

RGINLINE void rg_time_sleep_ns(u64 ns)
{
	if (ns == 0u) return;
	RG_TIME_PLATFORM_SLEEP_NS(ns);
}

RGINLINE void rg_time_sleep_us(u64 us)
{
	u64 ns = us > UINT64_MAX / UINT64_C(1000) ? UINT64_MAX : us * UINT64_C(1000);
	rg_time_sleep_ns(ns);
}

RGINLINE void rg_time_sleep_ms(u32 ms)
{
	rg_time_sleep_ns((u64)ms * UINT64_C(1000000));
}

RGINLINE void rg_time_yield(void)
{
	RG_TIME_PLATFORM_YIELD();
}

#endif // RG_TIME_H
