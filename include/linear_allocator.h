#ifndef _LINEAR_ALLOC_H_
#define _LINEAR_ALLOC_H_
#include <stddef.h>				// size_t

typedef struct
{
	char *current;
	char *start;
	char *end;
} LinearAllocator;				// 6 bytes

extern void la_init				(LinearAllocator *allocator, const size_t total_size);
extern void *la_alloc_aligned	(LinearAllocator *allocator, const size_t size, const size_t alignment);
extern void *la_alloc			(LinearAllocator *allocator, const size_t size);
extern void la_reset			(LinearAllocator *allocator);
extern void la_terminate		(LinearAllocator *allocator);

// FOR DEBUGGING
extern const size_t la_used_space		(LinearAllocator *allocator);
extern const size_t la_remaining_space	(LinearAllocator *allocator);
extern void la_show_memory				(LinearAllocator *allocator);
#endif // _LINEAR_ALLOC_H_
