// rg_defs - Shared compiler, platform, and utility definitions
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header containing compiler detection, platform macros, and common utilities.
//
// USAGE:
//   #include "rg_defs.h"
//   // Provides: RGINLINE, RG_LIKELY, RG_ALIGNOF, KB/MB/GB, etc.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_DEFS_H
#define RG_DEFS_H

#include <stddef.h>
#include <stdint.h>

// =============================================================================
// COMPILER DETECTION
// =============================================================================

#undef RG_COMPILER_MSVC
#undef RG_COMPILER_GCC
#undef RG_COMPILER_CLANG

#if defined(__clang__)
#define RG_COMPILER_CLANG 1
#define RG_COMPILER_GCC 0
#define RG_COMPILER_MSVC 0
#elif defined(__GNUC__)
#define RG_COMPILER_CLANG 0
#define RG_COMPILER_GCC 1
#define RG_COMPILER_MSVC 0
#elif defined(_MSC_VER)
#define RG_COMPILER_CLANG 0
#define RG_COMPILER_GCC 0
#define RG_COMPILER_MSVC 1
#else
#define RG_COMPILER_CLANG 0
#define RG_COMPILER_GCC 0
#define RG_COMPILER_MSVC 0
#endif

// =============================================================================
// PLATFORM DETECTION
// =============================================================================

#undef RG_PLATFORM_WINDOWS
#undef RG_PLATFORM_LINUX
#undef RG_PLATFORM_MACOS

#if defined(_WIN32) || defined(_WIN64)
#define RG_PLATFORM_WINDOWS 1
#define RG_PLATFORM_LINUX 0
#define RG_PLATFORM_MACOS 0
#elif defined(__linux__)
#define RG_PLATFORM_WINDOWS 0
#define RG_PLATFORM_LINUX 1
#define RG_PLATFORM_MACOS 0
#elif defined(__APPLE__) && defined(__MACH__)
#define RG_PLATFORM_WINDOWS 0
#define RG_PLATFORM_LINUX 0
#define RG_PLATFORM_MACOS 1
#else
#define RG_PLATFORM_WINDOWS 0
#define RG_PLATFORM_LINUX 0
#define RG_PLATFORM_MACOS 0
#endif

// =============================================================================
// ARCHITECTURE DETECTION
// =============================================================================

#undef RG_ARCH_X64
#undef RG_ARCH_X86
#undef RG_ARCH_ARM64

#if defined(_M_X64) || defined(__x86_64__)
#define RG_ARCH_X64 1
#define RG_ARCH_X86 0
#define RG_ARCH_ARM64 0
#elif defined(_M_IX86) || defined(__i386__)
#define RG_ARCH_X64 0
#define RG_ARCH_X86 1
#define RG_ARCH_ARM64 0
#elif defined(_M_ARM64) || defined(__aarch64__)
#define RG_ARCH_X64 0
#define RG_ARCH_X86 0
#define RG_ARCH_ARM64 1
#else
#define RG_ARCH_X64 0
#define RG_ARCH_X86 0
#define RG_ARCH_ARM64 0
#endif

// =============================================================================
// FORCE INLINE
// =============================================================================

#ifndef RGINLINE
#if RG_COMPILER_MSVC
#define RGINLINE static __forceinline
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
#define RGINLINE static inline __attribute__((always_inline))
#else
#define RGINLINE static inline
#endif
#endif

// =============================================================================
// BRANCH PREDICTION HINTS
// =============================================================================

#ifndef RG_LIKELY
#if RG_COMPILER_GCC || RG_COMPILER_CLANG
#define RG_LIKELY(x) __builtin_expect(!!(x), 1)
#define RG_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define RG_LIKELY(x) (x)
#define RG_UNLIKELY(x) (x)
#endif
#endif

// =============================================================================
// ALIGNMENT UTILITIES
// =============================================================================

#ifndef RG_ALIGNOF
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define RG_ALIGNOF(type) _Alignof(type)
#elif RG_COMPILER_MSVC
#define RG_ALIGNOF(type) __alignof(type)
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
#define RG_ALIGNOF(type) __alignof__(type)
#else
#define RG_ALIGNOF(type) sizeof(type)
#endif
#endif

#ifndef RG_ALIGN_UP
#define RG_ALIGN_UP(value, align) \
	(((size_t)(value) + ((size_t)(align) - 1)) & ~((size_t)(align) - 1))
#endif

#ifndef RG_ALIGN_DOWN
#define RG_ALIGN_DOWN(value, align) \
	((size_t)(value) & ~((size_t)(align) - 1))
#endif

#ifndef RG_IS_POWER_OF_2
#define RG_IS_POWER_OF_2(x) (((x) != 0) && (((x) & ((x) - 1)) == 0))
#endif

#ifndef RG_IS_ALIGNED
#define RG_IS_ALIGNED(ptr, align) (((uintptr_t)(ptr) & ((align) - 1)) == 0)
#endif

// =============================================================================
// SIZE HELPERS
// =============================================================================

#ifndef KB
#define KB(x) ((size_t)((x) * 1024ULL))
#endif

#ifndef MB
#define MB(x) ((size_t)((x) * 1024ULL * 1024ULL))
#endif

#ifndef GB
#define GB(x) ((size_t)((x) * 1024ULL * 1024ULL * 1024ULL))
#endif

// =============================================================================
// CACHE LINE SIZE
// =============================================================================

#ifndef RG_CACHE_LINE_SIZE
#define RG_CACHE_LINE_SIZE 64
#endif

// =============================================================================
// THREAD LOCAL STORAGE
// =============================================================================

#ifndef RG_THREAD_LOCAL
#if RG_COMPILER_MSVC
#define RG_THREAD_LOCAL __declspec(thread)
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
#define RG_THREAD_LOCAL __thread
#else
#define RG_THREAD_LOCAL
#endif
#endif

// =============================================================================
// RESTRICT KEYWORD
// =============================================================================

#ifndef RG_RESTRICT
#if RG_COMPILER_MSVC
#define RG_RESTRICT __restrict
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
#define RG_RESTRICT __restrict__
#else
#define RG_RESTRICT
#endif
#endif

// =============================================================================
// NO INLINE (for cold paths)
// =============================================================================

#ifndef RG_NOINLINE
#if RG_COMPILER_MSVC
#define RG_NOINLINE __declspec(noinline)
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
#define RG_NOINLINE __attribute__((noinline))
#else
#define RG_NOINLINE
#endif
#endif

// =============================================================================
// ARRAY COUNT
// =============================================================================

#ifndef RG_ARRAY_COUNT
#define RG_ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

// =============================================================================
// MIN / MAX / CLAMP (safe versions that evaluate arguments once)
// =============================================================================

#if RG_COMPILER_GCC || RG_COMPILER_CLANG
// GCC/Clang: Use statement expressions to evaluate args once
#ifndef RG_MIN
#define RG_MIN(a, b) ({     \
	__typeof__(a) _a = (a); \
	__typeof__(b) _b = (b); \
	_a < _b ? _a : _b;      \
})
#endif

#ifndef RG_MAX
#define RG_MAX(a, b) ({     \
	__typeof__(a) _a = (a); \
	__typeof__(b) _b = (b); \
	_a > _b ? _a : _b;      \
})
#endif

#ifndef RG_CLAMP
#define RG_CLAMP(val, lo, hi) ({                  \
	__typeof__(val) _val = (val);                 \
	__typeof__(lo) _lo = (lo);                    \
	__typeof__(hi) _hi = (hi);                    \
	_val < _lo ? _lo : (_val > _hi ? _hi : _val); \
})
#endif
#else
// MSVC: Simple macros (avoid side effects in arguments)
#ifndef RG_MIN
#define RG_MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef RG_MAX
#define RG_MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef RG_CLAMP
#define RG_CLAMP(val, lo, hi) ((val) < (lo) ? (lo) : ((val) > (hi) ? (hi) : (val)))
#endif
#endif

// =============================================================================
// UNUSED PARAMETER
// =============================================================================

#ifndef RG_UNUSED
#define RG_UNUSED(x) (void)(x)
#endif

// =============================================================================
// DEBUG BREAK
// =============================================================================

#ifndef RG_DEBUG_BREAK
#if RG_COMPILER_MSVC
#define RG_DEBUG_BREAK() __debugbreak()
#elif RG_COMPILER_GCC || RG_COMPILER_CLANG
#if RG_ARCH_X64 || RG_ARCH_X86
#define RG_DEBUG_BREAK() __asm__ volatile("int $0x03")
#elif RG_ARCH_ARM64
#define RG_DEBUG_BREAK() __builtin_trap()
#else
#define RG_DEBUG_BREAK() __builtin_trap()
#endif
#else
#define RG_DEBUG_BREAK() ((void)0)
#endif
#endif

#endif // RG_DEFS_H
