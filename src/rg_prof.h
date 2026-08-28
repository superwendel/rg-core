// rg_prof - Low-overhead per-thread CPU profiling
//
// Part of the Reverse Gravity (rg_) core libraries.
// Single-header C99 library for recording timed scopes, instant events, and
// frame markers into fixed per-thread buffers.
//
// USAGE:
//   #define RG_PROF_ENABLED 1
//   #include "rg_prof.h"
//
//   RgProf prof;
//   rg_prof_init(&prof, &arena, 4, 16384);
//   RgProfThread* main_thread = rg_prof_register_thread(&prof, "main");
//   rg_prof_set_thread(main_thread);
//
//   rg_prof_thread_reset(main_thread);
//   RG_PROF_FRAME_BEGIN("Frame");
//   RG_PROF_SCOPE("Update")
//   {
//       // work...
//   }
//   RG_PROF_FRAME_END("Frame");
//
// OPTIONS:
//   #define RG_PROF_ENABLED 1                 - Enable recording (default: 0)
//   #define RG_PROF_ASSERT(x)                 - Custom assertion macro
//   #define RG_PROF_DEFAULT_MAX_THREADS       - Default thread count (8)
//   #define RG_PROF_DEFAULT_EVENTS_PER_THREAD - Default events per thread (16384)
//
// NOTES:
//   - Register threads during setup, then give each OS thread its own handle.
//   - Recording is lock-free; reset and consume a thread only while it is idle.
//   - Event and thread names are borrowed pointers and must remain alive.
//   - break/continue close RG_PROF_SCOPE but apply to its macro loop, not an
//     enclosing application loop. return/goto require an explicit scope.
//   - All functions have internal linkage and work in unity builds.
//
// Author: Steven Wendel (superwendel)

#ifndef RG_PROF_H
#define RG_PROF_H

#ifndef RG_PROF_ENABLED
#define RG_PROF_ENABLED 0
#endif

#include "rg_mem.h"

#if RG_PROF_ENABLED
#include "rg_time.h"
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef RG_PROF_ASSERT
#include <assert.h>
#define RG_PROF_ASSERT(condition) assert(condition)
#endif

#ifndef RG_PROF_DEFAULT_MAX_THREADS
#define RG_PROF_DEFAULT_MAX_THREADS 8u
#endif

#ifndef RG_PROF_DEFAULT_EVENTS_PER_THREAD
#define RG_PROF_DEFAULT_EVENTS_PER_THREAD 16384u
#endif

#if RG_CACHE_LINE_SIZE < 32 || (RG_CACHE_LINE_SIZE & (RG_CACHE_LINE_SIZE - 1)) != 0
#error RG_CACHE_LINE_SIZE must be a power of two of at least 32 bytes
#endif

typedef struct RgProfThread RgProfThread;
typedef struct RgProfThreadSlot RgProfThreadSlot;

typedef enum RgProfEventType
{
	RG_PROF_EVENT_SCOPE = 0,
	RG_PROF_EVENT_INSTANT,
	RG_PROF_EVENT_FRAME_BEGIN,
	RG_PROF_EVENT_FRAME_END
} RgProfEventType;

typedef struct RgProfEvent
{
	const char* name;
	u64 start;
	u64 end;
	u32 thread_id;
	u16 depth;
	u8 type;
	u8 reserved;
} RgProfEvent;

typedef struct RgProf
{
	RgArena* arena;
	RgProfThreadSlot* thread_slots;
	u64 ticks_per_second;
	u32 max_threads;
	u32 thread_count;
	u32 events_per_thread;
} RgProf;

struct RgProfThread
{
	RgProf* prof;
	RgProfEvent* events;
	const char* name;
	u32 capacity;
	u32 count;
	u32 dropped;
	u32 id;
	u16 depth;
	u8 frame_open;
	u8 reserved;
	u64 completed_frames;
};

struct RgProfThreadSlot
{
	RgProfThread thread;
	u8 padding[RG_CACHE_LINE_SIZE - sizeof(RgProfThread)];
};

typedef char RgProfEventMustFit32Bytes[(sizeof(RgProfEvent) <= 32u) ? 1 : -1];
typedef char RgProfThreadMustFitCacheLine[(sizeof(RgProfThread) <= RG_CACHE_LINE_SIZE) ? 1 : -1];
typedef char RgProfThreadSlotMustBeCacheLine[(sizeof(RgProfThreadSlot) == RG_CACHE_LINE_SIZE) ? 1 : -1];

typedef struct RgProfScope
{
	RgProfThread* thread;
	RgProfEvent* event;
	i32 active;
	i32 depth_incremented;
} RgProfScope;

typedef struct RgProfFrameSample
{
	u64 frame_index;
	f32 frame_ms;
} RgProfFrameSample;

typedef struct RgProfHistory
{
	RgProfFrameSample* frames;
	f32* section_ms;
	const char* const* section_names;
	const RgProfThread* source_thread;
	u64 last_completed_frame;
	u32 capacity;
	u32 section_count;
	u32 count;
	u32 write_index;
} RgProfHistory;

typedef void (*RgProfEventFn)(const RgProfEvent* event, void* user);

RGINLINE int rg_prof_init(RgProf* prof, RgArena* arena, u32 max_threads, u32 events_per_thread);
RGINLINE int rg_prof_init_default(RgProf* prof, RgArena* arena);
RGINLINE RgProfThread* rg_prof_register_thread(RgProf* prof, const char* name);
RGINLINE void rg_prof_set_thread(RgProfThread* thread);
RGINLINE RgProfThread* rg_prof_get_thread(void);
RGINLINE void rg_prof_thread_reset(RgProfThread* thread);

RGINLINE u64 rg_prof_ticks_now(const RgProf* prof);
RGINLINE f64 rg_prof_ticks_to_us(const RgProf* prof, u64 ticks);
RGINLINE f64 rg_prof_ticks_to_ms(const RgProf* prof, u64 ticks);

RGINLINE RgProfScope rg_prof_scope_begin_on(RgProfThread* thread, const char* name);
RGINLINE RgProfScope rg_prof_scope_begin(const char* name);
RGINLINE void rg_prof_scope_end(RgProfScope* scope);
RGINLINE void rg_prof_event_on(RgProfThread* thread, const char* name);
RGINLINE void rg_prof_event(const char* name);
RGINLINE void rg_prof_frame_begin_on(RgProfThread* thread, const char* name);
RGINLINE void rg_prof_frame_begin(const char* name);
RGINLINE void rg_prof_frame_end_on(RgProfThread* thread, const char* name);
RGINLINE void rg_prof_frame_end(const char* name);
RGINLINE void rg_prof_iterate_events(const RgProf* prof, RgProfEventFn fn, void* user);

RGINLINE int rg_prof_history_init(RgProfHistory* history, RgArena* arena,
                                  u32 frame_capacity, const char* const* section_names,
                                  u32 section_count);
RGINLINE int rg_prof_history_capture(RgProfHistory* history, const RgProfThread* thread);
RGINLINE const RgProfFrameSample* rg_prof_history_latest(const RgProfHistory* history);
RGINLINE f32 rg_prof_history_latest_section_ms(const RgProfHistory* history, u32 section_index);
RGINLINE f32 rg_prof_history_average_frame_ms(const RgProfHistory* history, u32 sample_count);
RGINLINE f32 rg_prof_history_average_section_ms(const RgProfHistory* history,
                                                u32 section_index, u32 sample_count);

#define RG_PROF_CONCAT_INNER(a, b) a##b
#define RG_PROF_CONCAT(a, b) RG_PROF_CONCAT_INNER(a, b)
#define RG_PROF_IGNORE(value) ((void)sizeof(value))

#if defined(__COUNTER__)
#define RG_PROF_UNIQUE_ID __COUNTER__
#else
#define RG_PROF_UNIQUE_ID __LINE__
#endif

#if RG_PROF_ENABLED
#define RG_PROF_SCOPE(name) RG_PROF_SCOPE_IMPL(name, RG_PROF_UNIQUE_ID)
#define RG_PROF_SCOPE_ON(thread, name) RG_PROF_SCOPE_ON_IMPL(thread, name, RG_PROF_UNIQUE_ID)
#define RG_PROF_SCOPE_IMPL(name, id) RG_PROF_SCOPE_ON_IMPL(rg_prof_get_thread(), name, id)
#define RG_PROF_SCOPE_ON_IMPL(thread, name, id)                                                      \
	for (RgProfScope RG_PROF_CONCAT(_rg_prof_scope_, id) = rg_prof_scope_begin_on((thread), (name)); \
	     RG_PROF_CONCAT(_rg_prof_scope_, id).active;                                                 \
	     rg_prof_scope_end(&RG_PROF_CONCAT(_rg_prof_scope_, id)))                                    \
		for (i32 RG_PROF_CONCAT(_rg_prof_once_, id) = 1;                                             \
		     RG_PROF_CONCAT(_rg_prof_once_, id);                                                     \
		     RG_PROF_CONCAT(_rg_prof_once_, id) = 0)
#define RG_PROF_EVENT(name) rg_prof_event(name)
#define RG_PROF_EVENT_ON(thread, name) rg_prof_event_on((thread), (name))
#define RG_PROF_FRAME_BEGIN(name) rg_prof_frame_begin(name)
#define RG_PROF_FRAME_BEGIN_ON(thread, name) rg_prof_frame_begin_on((thread), (name))
#define RG_PROF_FRAME_END(name) rg_prof_frame_end(name)
#define RG_PROF_FRAME_END_ON(thread, name) rg_prof_frame_end_on((thread), (name))
#else
#define RG_PROF_SCOPE(name) RG_PROF_SCOPE_DISABLED_IMPL(name, RG_PROF_UNIQUE_ID)
#define RG_PROF_SCOPE_ON(thread, name) RG_PROF_SCOPE_ON_DISABLED_IMPL(thread, name, RG_PROF_UNIQUE_ID)
#define RG_PROF_SCOPE_DISABLED_IMPL(name, id) RG_PROF_SCOPE_ON_DISABLED_IMPL(NULL, name, id)
#define RG_PROF_SCOPE_ON_DISABLED_IMPL(thread, name, id)                                              \
	for (i32 RG_PROF_CONCAT(_rg_prof_scope_, id) = (RG_PROF_IGNORE(thread), RG_PROF_IGNORE(name), 1); \
	     RG_PROF_CONCAT(_rg_prof_scope_, id);                                                         \
	     RG_PROF_CONCAT(_rg_prof_scope_, id) = 0)                                                     \
		for (i32 RG_PROF_CONCAT(_rg_prof_once_, id) = 1;                                              \
		     RG_PROF_CONCAT(_rg_prof_once_, id);                                                      \
		     RG_PROF_CONCAT(_rg_prof_once_, id) = 0)
#define RG_PROF_EVENT(name) RG_PROF_IGNORE(name)
#define RG_PROF_EVENT_ON(thread, name) (RG_PROF_IGNORE(thread), RG_PROF_IGNORE(name))
#define RG_PROF_FRAME_BEGIN(name) RG_PROF_IGNORE(name)
#define RG_PROF_FRAME_BEGIN_ON(thread, name) (RG_PROF_IGNORE(thread), RG_PROF_IGNORE(name))
#define RG_PROF_FRAME_END(name) RG_PROF_IGNORE(name)
#define RG_PROF_FRAME_END_ON(thread, name) (RG_PROF_IGNORE(thread), RG_PROF_IGNORE(name))
#endif

#if RG_PROF_ENABLED

static RG_THREAD_LOCAL RgProfThread* rg_prof_tls_thread;

RGINLINE u64 rg_prof_ticks_now(const RgProf* prof)
{
	RG_UNUSED(prof);
	return rg_time_ticks();
}

RGINLINE f64 rg_prof_ticks_to_us(const RgProf* prof, u64 ticks)
{
	RG_PROF_ASSERT(prof != NULL);
	if (prof == NULL || prof->ticks_per_second == 0u) return 0.0;
	return (f64)ticks * 1000000.0 / (f64)prof->ticks_per_second;
}

RGINLINE f64 rg_prof_ticks_to_ms(const RgProf* prof, u64 ticks)
{
	RG_PROF_ASSERT(prof != NULL);
	if (prof == NULL || prof->ticks_per_second == 0u) return 0.0;
	return (f64)ticks * 1000.0 / (f64)prof->ticks_per_second;
}

RGINLINE int rg_prof_init(RgProf* prof, RgArena* arena, u32 max_threads, u32 events_per_thread)
{
	RG_PROF_ASSERT(prof != NULL);
	RG_PROF_ASSERT(arena != NULL);
	if (prof == NULL || arena == NULL || arena->memory == NULL ||
	    max_threads == 0u || events_per_thread == 0u)
	{
		return 0;
	}

	size_t arena_mark = arena->used;
	memset(prof, 0, sizeof(*prof));
	prof->arena = arena;
	prof->max_threads = max_threads;
	prof->events_per_thread = events_per_thread;

	if ((size_t)max_threads > SIZE_MAX / sizeof(RgProfThreadSlot))
	{
		memset(prof, 0, sizeof(*prof));
		return 0;
	}
	size_t thread_slot_bytes = (size_t)max_threads * sizeof(RgProfThreadSlot);
	prof->thread_slots = (RgProfThreadSlot*)rg_arena_alloc_aligned(
	    arena, thread_slot_bytes, RG_CACHE_LINE_SIZE);
	if (prof->thread_slots == NULL)
	{
		arena->used = arena_mark;
		memset(prof, 0, sizeof(*prof));
		return 0;
	}

#ifdef RG_TIME_H
	rg_time_init();
#endif
	prof->ticks_per_second = rg_time_ticks_per_second();
	if (prof->ticks_per_second == 0u)
	{
		arena->used = arena_mark;
		memset(prof, 0, sizeof(*prof));
		return 0;
	}

	return 1;
}

RGINLINE int rg_prof_init_default(RgProf* prof, RgArena* arena)
{
	return rg_prof_init(prof, arena, RG_PROF_DEFAULT_MAX_THREADS,
	                    RG_PROF_DEFAULT_EVENTS_PER_THREAD);
}

RGINLINE RgProfThread* rg_prof_register_thread(RgProf* prof, const char* name)
{
	RG_PROF_ASSERT(prof != NULL);
	if (prof == NULL || prof->thread_slots == NULL || prof->arena == NULL) return NULL;

	u32 index = prof->thread_count;
	if (index >= prof->max_threads) return NULL;

	RgProfThread* thread = &prof->thread_slots[index].thread;
	memset(thread, 0, sizeof(*thread));
	thread->prof = prof;
	thread->name = name;
	thread->capacity = prof->events_per_thread;
	thread->id = index;

	if ((size_t)thread->capacity > SIZE_MAX / sizeof(RgProfEvent))
	{
		memset(thread, 0, sizeof(*thread));
		return NULL;
	}
	size_t event_bytes = (size_t)thread->capacity * sizeof(RgProfEvent);
	size_t arena_mark = prof->arena->used;
	thread->events = (RgProfEvent*)rg_arena_alloc_aligned(
	    prof->arena, event_bytes, RG_ALIGNOF(RgProfEvent));
	if (thread->events == NULL)
	{
		prof->arena->used = arena_mark;
		memset(thread, 0, sizeof(*thread));
		return NULL;
	}
	memset(thread->events, 0, event_bytes);

	prof->thread_count = index + 1u;
	return thread;
}

RGINLINE void rg_prof_set_thread(RgProfThread* thread)
{
	rg_prof_tls_thread = thread;
}

RGINLINE RgProfThread* rg_prof_get_thread(void)
{
	return rg_prof_tls_thread;
}

RGINLINE void rg_prof_thread_reset(RgProfThread* thread)
{
	if (thread == NULL) return;
	thread->count = 0u;
	thread->dropped = 0u;
	thread->depth = 0u;
	thread->frame_open = 0u;
}

RGINLINE RgProfEvent* rg_prof_thread_push_event(RgProfThread* thread)
{
	u32 index = thread->count;
	if (RG_UNLIKELY(index >= thread->capacity))
	{
		thread->dropped++;
		return NULL;
	}
	thread->count = index + 1u;
	return &thread->events[index];
}

RGINLINE int rg_prof_emit_event_on(RgProfThread* thread, const char* name, RgProfEventType type)
{
	if (thread == NULL) return 0;

	RgProfEvent* event = rg_prof_thread_push_event(thread);
	if (event == NULL) return 0;

	u64 now = rg_prof_ticks_now(thread->prof);
	event->name = name;
	event->start = now;
	event->end = now;
	event->thread_id = thread->id;
	event->depth = thread->depth;
	event->type = (u8)type;
	event->reserved = 0u;
	return 1;
}

RGINLINE RgProfScope rg_prof_scope_begin_on(RgProfThread* thread, const char* name)
{
	RgProfScope scope = {thread, NULL, 1, 0};
	if (thread == NULL) return scope;

	RgProfEvent* event = rg_prof_thread_push_event(thread);
	if (event == NULL) return scope;

	event->name = name;
	event->start = rg_prof_ticks_now(thread->prof);
	event->end = event->start;
	event->thread_id = thread->id;
	event->depth = thread->depth;
	event->type = (u8)RG_PROF_EVENT_SCOPE;
	event->reserved = 0u;
	scope.event = event;

	if (thread->depth < UINT16_MAX)
	{
		thread->depth++;
		scope.depth_incremented = 1;
	}
	return scope;
}

RGINLINE RgProfScope rg_prof_scope_begin(const char* name)
{
	return rg_prof_scope_begin_on(rg_prof_tls_thread, name);
}

RGINLINE void rg_prof_scope_end(RgProfScope* scope)
{
	if (scope == NULL || !scope->active) return;
	if (scope->event != NULL) scope->event->end = rg_prof_ticks_now(scope->thread->prof);
	if (scope->depth_incremented && scope->thread->depth > 0u) scope->thread->depth--;
	scope->active = 0;
}

RGINLINE void rg_prof_event_on(RgProfThread* thread, const char* name)
{
	rg_prof_emit_event_on(thread, name, RG_PROF_EVENT_INSTANT);
}

RGINLINE void rg_prof_event(const char* name)
{
	rg_prof_event_on(rg_prof_tls_thread, name);
}

RGINLINE void rg_prof_frame_begin_on(RgProfThread* thread, const char* name)
{
	if (thread == NULL) return;
	thread->frame_open = (u8)rg_prof_emit_event_on(thread, name, RG_PROF_EVENT_FRAME_BEGIN);
}

RGINLINE void rg_prof_frame_begin(const char* name)
{
	rg_prof_frame_begin_on(rg_prof_tls_thread, name);
}

RGINLINE void rg_prof_frame_end_on(RgProfThread* thread, const char* name)
{
	if (thread == NULL) return;
	if (thread->frame_open && rg_prof_emit_event_on(thread, name, RG_PROF_EVENT_FRAME_END))
		thread->completed_frames++;
	thread->frame_open = 0u;
}

RGINLINE void rg_prof_frame_end(const char* name)
{
	rg_prof_frame_end_on(rg_prof_tls_thread, name);
}

RGINLINE void rg_prof_iterate_events(const RgProf* prof, RgProfEventFn fn, void* user)
{
	if (prof == NULL || fn == NULL) return;
	for (u32 i = 0; i < prof->thread_count; i++)
	{
		const RgProfThread* thread = &prof->thread_slots[i].thread;
		for (u32 j = 0; j < thread->count; j++) fn(&thread->events[j], user);
	}
}

RGINLINE int rg_prof_history_init(RgProfHistory* history, RgArena* arena,
                                  u32 frame_capacity, const char* const* section_names,
                                  u32 section_count)
{
	RG_PROF_ASSERT(history != NULL);
	RG_PROF_ASSERT(arena != NULL);
	if (history != NULL) memset(history, 0, sizeof(*history));
	if (history == NULL || arena == NULL || arena->memory == NULL || frame_capacity == 0u ||
	    (section_count > 0u && section_names == NULL))
	{
		return 0;
	}

	if ((size_t)frame_capacity > SIZE_MAX / sizeof(RgProfFrameSample)) return 0;
	if (section_count > 0u && (size_t)frame_capacity > SIZE_MAX / section_count) return 0;
	size_t section_cells = (size_t)frame_capacity * section_count;
	if (section_cells > SIZE_MAX / sizeof(f32)) return 0;

	size_t arena_mark = arena->used;
	size_t frame_bytes = (size_t)frame_capacity * sizeof(RgProfFrameSample);
	history->frames = (RgProfFrameSample*)rg_arena_alloc_aligned(
	    arena, frame_bytes, RG_ALIGNOF(RgProfFrameSample));
	if (history->frames == NULL) goto fail;

	if (section_cells > 0u)
	{
		history->section_ms = (f32*)rg_arena_alloc_aligned(
		    arena, section_cells * sizeof(f32), RG_ALIGNOF(f32));
		if (history->section_ms == NULL) goto fail;
	}

	history->section_names = section_names;
	history->capacity = frame_capacity;
	history->section_count = section_count;
	return 1;

fail:
	arena->used = arena_mark;
	memset(history, 0, sizeof(*history));
	return 0;
}

RGINLINE int rg_prof_names_match(const char* a, const char* b)
{
	if (a == b) return 1;
	if (a == NULL || b == NULL) return 0;
	return strcmp(a, b) == 0;
}

RGINLINE int rg_prof_history_capture(RgProfHistory* history, const RgProfThread* thread)
{
	if (history == NULL || history->frames == NULL || history->capacity == 0u ||
	    thread == NULL || thread->prof == NULL || thread->events == NULL || thread->count < 2u ||
	    thread->completed_frames == 0u ||
	    (history->source_thread == thread && history->last_completed_frame == thread->completed_frames))
	{
		return 0;
	}

	u32 end_index = thread->count;
	while (end_index > 0u)
	{
		end_index--;
		if (thread->events[end_index].type == RG_PROF_EVENT_FRAME_END) break;
		if (thread->events[end_index].type == RG_PROF_EVENT_FRAME_BEGIN) return 0;
	}
	if (thread->events[end_index].type != RG_PROF_EVENT_FRAME_END) return 0;

	u32 begin_index = end_index;
	while (begin_index > 0u)
	{
		begin_index--;
		if (thread->events[begin_index].type == RG_PROF_EVENT_FRAME_BEGIN) break;
	}
	if (thread->events[begin_index].type != RG_PROF_EVENT_FRAME_BEGIN) return 0;

	const RgProfEvent* begin = &thread->events[begin_index];
	const RgProfEvent* end = &thread->events[end_index];
	if (end->start < begin->start) return 0;

	u32 write_index = history->write_index;
	RgProfFrameSample* sample = &history->frames[write_index];
	sample->frame_index = thread->completed_frames - 1u;
	sample->frame_ms = (f32)rg_prof_ticks_to_ms(thread->prof, end->start - begin->start);

	f32* sections = history->section_count > 0u
	                    ? &history->section_ms[(size_t)write_index * history->section_count]
	                    : NULL;
	for (u32 section_index = 0; section_index < history->section_count; section_index++)
		sections[section_index] = 0.0f;

	for (u32 event_index = begin_index + 1u; event_index < end_index; event_index++)
	{
		const RgProfEvent* event = &thread->events[event_index];
		if (event->type != RG_PROF_EVENT_SCOPE || event->end < event->start) continue;

		for (u32 section_index = 0; section_index < history->section_count; section_index++)
		{
			if (rg_prof_names_match(event->name, history->section_names[section_index]))
			{
				sections[section_index] += (f32)rg_prof_ticks_to_ms(
				    thread->prof, event->end - event->start);
				break;
			}
		}
	}

	history->write_index = write_index + 1u;
	if (history->write_index == history->capacity) history->write_index = 0u;
	if (history->count < history->capacity) history->count++;
	history->source_thread = thread;
	history->last_completed_frame = thread->completed_frames;
	return 1;
}

RGINLINE const RgProfFrameSample* rg_prof_history_latest(const RgProfHistory* history)
{
	if (history == NULL || history->frames == NULL || history->count == 0u) return NULL;
	u32 index = history->write_index == 0u ? history->capacity - 1u : history->write_index - 1u;
	return &history->frames[index];
}

RGINLINE f32 rg_prof_history_latest_section_ms(const RgProfHistory* history, u32 section_index)
{
	if (history == NULL || history->section_ms == NULL || history->count == 0u ||
	    section_index >= history->section_count)
	{
		return 0.0f;
	}
	u32 index = history->write_index == 0u ? history->capacity - 1u : history->write_index - 1u;
	return history->section_ms[(size_t)index * history->section_count + section_index];
}

RGINLINE u32 rg_prof_history_sample_count(const RgProfHistory* history, u32 requested)
{
	if (history == NULL || history->count == 0u) return 0u;
	return requested == 0u || requested > history->count ? history->count : requested;
}

RGINLINE f32 rg_prof_history_average_frame_ms(const RgProfHistory* history, u32 sample_count)
{
	sample_count = rg_prof_history_sample_count(history, sample_count);
	if (sample_count == 0u) return 0.0f;

	f64 sum = 0.0;
	u32 index = history->write_index;
	for (u32 i = 0; i < sample_count; i++)
	{
		index = index == 0u ? history->capacity - 1u : index - 1u;
		sum += history->frames[index].frame_ms;
	}
	return (f32)(sum / sample_count);
}

RGINLINE f32 rg_prof_history_average_section_ms(const RgProfHistory* history,
                                                u32 section_index, u32 sample_count)
{
	if (history == NULL || history->section_ms == NULL || section_index >= history->section_count)
		return 0.0f;
	sample_count = rg_prof_history_sample_count(history, sample_count);
	if (sample_count == 0u) return 0.0f;

	f64 sum = 0.0;
	u32 index = history->write_index;
	for (u32 i = 0; i < sample_count; i++)
	{
		index = index == 0u ? history->capacity - 1u : index - 1u;
		sum += history->section_ms[(size_t)index * history->section_count + section_index];
	}
	return (f32)(sum / sample_count);
}

#else

static RgProfThread rg_prof_dummy_thread;

RGINLINE int rg_prof_init(RgProf* prof, RgArena* arena, u32 max_threads, u32 events_per_thread)
{
	RG_UNUSED(arena);
	RG_UNUSED(max_threads);
	RG_UNUSED(events_per_thread);
	if (prof != NULL) memset(prof, 0, sizeof(*prof));
	return 1;
}

RGINLINE int rg_prof_init_default(RgProf* prof, RgArena* arena)
{
	return rg_prof_init(prof, arena, RG_PROF_DEFAULT_MAX_THREADS,
	                    RG_PROF_DEFAULT_EVENTS_PER_THREAD);
}

RGINLINE RgProfThread* rg_prof_register_thread(RgProf* prof, const char* name)
{
	RG_UNUSED(prof);
	RG_UNUSED(name);
	return &rg_prof_dummy_thread;
}

RGINLINE void rg_prof_set_thread(RgProfThread* thread) { RG_UNUSED(thread); }
RGINLINE RgProfThread* rg_prof_get_thread(void) { return NULL; }
RGINLINE void rg_prof_thread_reset(RgProfThread* thread) { RG_UNUSED(thread); }
RGINLINE u64 rg_prof_ticks_now(const RgProf* prof)
{
	RG_UNUSED(prof);
	return 0u;
}
RGINLINE f64 rg_prof_ticks_to_us(const RgProf* prof, u64 ticks)
{
	RG_UNUSED(prof);
	RG_UNUSED(ticks);
	return 0.0;
}
RGINLINE f64 rg_prof_ticks_to_ms(const RgProf* prof, u64 ticks)
{
	RG_UNUSED(prof);
	RG_UNUSED(ticks);
	return 0.0;
}

RGINLINE RgProfScope rg_prof_scope_begin_on(RgProfThread* thread, const char* name)
{
	RG_UNUSED(thread);
	RG_UNUSED(name);
	RgProfScope scope = {0};
	return scope;
}

RGINLINE RgProfScope rg_prof_scope_begin(const char* name)
{
	return rg_prof_scope_begin_on(NULL, name);
}

RGINLINE void rg_prof_scope_end(RgProfScope* scope) { RG_UNUSED(scope); }
RGINLINE void rg_prof_event_on(RgProfThread* thread, const char* name)
{
	RG_UNUSED(thread);
	RG_UNUSED(name);
}
RGINLINE void rg_prof_event(const char* name) { RG_UNUSED(name); }
RGINLINE void rg_prof_frame_begin_on(RgProfThread* thread, const char* name)
{
	RG_UNUSED(thread);
	RG_UNUSED(name);
}
RGINLINE void rg_prof_frame_begin(const char* name) { RG_UNUSED(name); }
RGINLINE void rg_prof_frame_end_on(RgProfThread* thread, const char* name)
{
	RG_UNUSED(thread);
	RG_UNUSED(name);
}
RGINLINE void rg_prof_frame_end(const char* name) { RG_UNUSED(name); }

RGINLINE void rg_prof_iterate_events(const RgProf* prof, RgProfEventFn fn, void* user)
{
	RG_UNUSED(prof);
	RG_UNUSED(fn);
	RG_UNUSED(user);
}

RGINLINE int rg_prof_history_init(RgProfHistory* history, RgArena* arena,
                                  u32 frame_capacity, const char* const* section_names,
                                  u32 section_count)
{
	RG_UNUSED(arena);
	RG_UNUSED(frame_capacity);
	RG_UNUSED(section_names);
	RG_UNUSED(section_count);
	if (history != NULL) memset(history, 0, sizeof(*history));
	return 1;
}

RGINLINE int rg_prof_history_capture(RgProfHistory* history, const RgProfThread* thread)
{
	RG_UNUSED(history);
	RG_UNUSED(thread);
	return 0;
}

RGINLINE const RgProfFrameSample* rg_prof_history_latest(const RgProfHistory* history)
{
	RG_UNUSED(history);
	return NULL;
}

RGINLINE f32 rg_prof_history_latest_section_ms(const RgProfHistory* history, u32 section_index)
{
	RG_UNUSED(history);
	RG_UNUSED(section_index);
	return 0.0f;
}

RGINLINE f32 rg_prof_history_average_frame_ms(const RgProfHistory* history, u32 sample_count)
{
	RG_UNUSED(history);
	RG_UNUSED(sample_count);
	return 0.0f;
}

RGINLINE f32 rg_prof_history_average_section_ms(const RgProfHistory* history,
                                                u32 section_index, u32 sample_count)
{
	RG_UNUSED(history);
	RG_UNUSED(section_index);
	RG_UNUSED(sample_count);
	return 0.0f;
}

#endif

#endif // RG_PROF_H
