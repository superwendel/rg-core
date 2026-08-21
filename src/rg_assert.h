// rg_assert - Configurable assertion, ensure, and panic helpers
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header C99 assertions with optional rg_log integration.
//
// USAGE:
//   #include "rg_assert.h"
//
//   RG_ASSERT_MSG(ptr != NULL, "Allocation failed for %zu bytes", size);
//   if (!RG_ENSURE_MSG(count > 0, "Count must be positive"))
//       return;
//
// OPTIONS:
//   #define RG_ASSERT_ENABLED      - Enable assertions (default: !NDEBUG)
//   #define RG_ENSURE_ENABLED      - Enable ensure reports (default: RG_ASSERT_ENABLED)
//   #define RG_ASSERT_HANDLER      - Custom handler (default: rg_assert_default_handler)
//   #define RG_ASSERT_BREAK()      - Debug-break hook
//   #define RG_ASSERT_ABORT()      - Abort hook (default: abort())
//   #define RG_ASSERT_MESSAGE_BUFFER_SIZE - Message buffer size (default: 1024)
//   #define RG_ASSERT_USE_LOG      - Route reports through RG_CRIT
//
// NOTES:
//   - RG_ASSERT does not evaluate its condition when disabled.
//   - RG_ENSURE always evaluates its condition and returns 1 or 0.
//   - Include rg_log.h first to enable RG_CRIT integration automatically.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_ASSERT_H
#define RG_ASSERT_H

#include "rg_defs.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef RG_ASSERT_ENABLED
#if defined(NDEBUG)
#define RG_ASSERT_ENABLED 0
#else
#define RG_ASSERT_ENABLED 1
#endif
#endif

#ifndef RG_ENSURE_ENABLED
#define RG_ENSURE_ENABLED RG_ASSERT_ENABLED
#endif

#ifndef RG_ASSERT_MESSAGE_BUFFER_SIZE
#define RG_ASSERT_MESSAGE_BUFFER_SIZE 1024u
#endif

#if RG_ASSERT_MESSAGE_BUFFER_SIZE < 1
#error RG_ASSERT_MESSAGE_BUFFER_SIZE must be at least 1
#endif

#ifndef RG_ASSERT_USE_LOG
#if defined(RG_CRIT)
#define RG_ASSERT_USE_LOG 1
#else
#define RG_ASSERT_USE_LOG 0
#endif
#endif

#ifndef RG_ASSERT_BREAK
#if RG_COMPILER_MSVC
#define RG_ASSERT_BREAK() __debugbreak()
#elif RG_COMPILER_CLANG || RG_COMPILER_GCC
#define RG_ASSERT_BREAK() __builtin_trap()
#else
#define RG_ASSERT_BREAK() ((void)0)
#endif
#endif

#ifndef RG_ASSERT_ABORT
#define RG_ASSERT_ABORT() abort()
#endif

#ifndef RG_ASSERT_UNUSED
#define RG_ASSERT_UNUSED(value) (void)sizeof(value)
#endif

typedef void (*RgAssertHandler)(const char* expression, const char* file, int line, const char* message);

RGINLINE void rg_assert_default_handler(const char* expression, const char* file, int line, const char* message);

#ifndef RG_ASSERT_HANDLER
#define RG_ASSERT_HANDLER rg_assert_default_handler
#endif

RGINLINE void rg_assert_format_message(char* buffer, size_t buffer_size, const char* format, va_list args)
{
	if (buffer_size == 0)
		return;
	buffer[0] = '\0';
	if (format == NULL || format[0] == '\0')
		return;
	vsnprintf(buffer, buffer_size, format, args);
	buffer[buffer_size - 1] = '\0';
}

static void rg_assert_report(const char* expression, const char* file, int line, const char* format, ...)
{
	char buffer[RG_ASSERT_MESSAGE_BUFFER_SIZE];
	buffer[0] = '\0';
	if (format != NULL && format[0] != '\0')
	{
		va_list args;
		va_start(args, format);
		rg_assert_format_message(buffer, sizeof(buffer), format, args);
		va_end(args);
	}
	RG_ASSERT_HANDLER(expression, file, line, buffer);
}

static void rg_assert_fail(const char* expression, const char* file, int line, const char* format, ...)
{
	char buffer[RG_ASSERT_MESSAGE_BUFFER_SIZE];
	buffer[0] = '\0';
	if (format != NULL && format[0] != '\0')
	{
		va_list args;
		va_start(args, format);
		rg_assert_format_message(buffer, sizeof(buffer), format, args);
		va_end(args);
	}
	RG_ASSERT_HANDLER(expression, file, line, buffer);
	RG_ASSERT_BREAK();
	RG_ASSERT_ABORT();
}

RGINLINE void rg_assert_default_handler(const char* expression, const char* file, int line, const char* message)
{
	if (message == NULL)
		message = "";

#if RG_ASSERT_USE_LOG
	if (message[0] != '\0')
		RG_CRIT("ASSERT FAIL: (%s) - %s (%s:%d)", expression, message, file, line);
	else
		RG_CRIT("ASSERT FAIL: (%s) (%s:%d)", expression, file, line);
#else
	if (message[0] != '\0')
		fprintf(stderr, "ASSERT FAIL: (%s) - %s (%s:%d)\n", expression, message, file, line);
	else
		fprintf(stderr, "ASSERT FAIL: (%s) (%s:%d)\n", expression, file, line);
#endif
}

#if RG_ASSERT_ENABLED
#define RG_ASSERT_NO_MSG(condition)                                        \
	do                                                                      \
	{                                                                       \
		if (!(condition))                                                     \
			rg_assert_fail(#condition, __FILE__, __LINE__, NULL);               \
	} while (0)
#define RG_ASSERT_MSG(condition, ...)                                      \
	do                                                                      \
	{                                                                       \
		if (!(condition))                                                     \
			rg_assert_fail(#condition, __FILE__, __LINE__, __VA_ARGS__);        \
	} while (0)
#else
#define RG_ASSERT_NO_MSG(condition) RG_ASSERT_UNUSED(condition)
#define RG_ASSERT_MSG(condition, ...) RG_ASSERT_UNUSED(condition)
#endif

#if RG_ENSURE_ENABLED
#define RG_ENSURE_NO_MSG(condition) ((condition) ? 1 : (rg_assert_report(#condition, __FILE__, __LINE__, NULL), 0))
#define RG_ENSURE_MSG(condition, ...) ((condition) ? 1 : (rg_assert_report(#condition, __FILE__, __LINE__, __VA_ARGS__), 0))
#else
#define RG_ENSURE_NO_MSG(condition) ((condition) ? 1 : 0)
#define RG_ENSURE_MSG(condition, ...) ((condition) ? 1 : 0)
#endif

#define RG_PANIC_NO_MSG()                         \
	do                                              \
	{                                               \
		rg_assert_fail("PANIC", __FILE__, __LINE__, NULL); \
	} while (0)
#define RG_PANIC_MSG(...)                                \
	do                                                     \
	{                                                      \
		rg_assert_fail("PANIC", __FILE__, __LINE__, __VA_ARGS__); \
	} while (0)

#ifdef RG_ASSERT
#undef RG_ASSERT
#endif
#ifdef RG_ENSURE
#undef RG_ENSURE
#endif
#ifdef RG_PANIC
#undef RG_PANIC
#endif

#define RG_ASSERT(condition) RG_ASSERT_NO_MSG(condition)
#define RG_ENSURE(condition) RG_ENSURE_NO_MSG(condition)
#define RG_PANIC() RG_PANIC_NO_MSG()

#endif // RG_ASSERT_H
