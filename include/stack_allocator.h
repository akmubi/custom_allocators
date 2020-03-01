#ifndef _STACK_ALLOCATOR_H_
#define _STACK_ALLOCATOR_H_
#include <stddef.h>					// size_t

typedef struct
{
	char *current;
	char *start;
	char *end;
} StackAllocator;					// 6 bytes

extern void sa_init		(StackAllocator *allocator, const size_t total_size);
extern void *sa_alloc_aligned	(StackAllocator *allocator, const size_t size, const size_t alignment);
extern void *sa_alloc		(StackAllocator *allocator, const size_t size);
extern void sa_free		(StackAllocator *allocator, void *ptr);
extern void sa_terminate	(StackAllocator *allocator);
extern void sa_reset		(StackAllocator *allocator);

// FOR DEBUGGING
extern const size_t sa_used_space	(StackAllocator *allocator);
extern const size_t sa_remaining_space	(StackAllocator *allocator);
extern void sa_show_memory		(StackAllocator *allocator);
#endif	// _STACK_ALLOCATOR_H_
