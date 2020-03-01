#ifndef _DOUBLE_ENDED_STACK_ALLOCATOR_
#define _DOUBLE_ENDED_STACK_ALLOCATOR_
#include <stddef.h>						// size_t

typedef struct
{
	char *current_front;
	char *current_back;
	char *start;
	char *end;
} DoubleEndedStackAllocator;			// 8 bytes

extern void desa_init			(DoubleEndedStackAllocator *allocator, const size_t total_size);
extern void desa_terminate		(DoubleEndedStackAllocator *allocator);

// FOR BOTH OF STACKS
extern void desa_reset			(DoubleEndedStackAllocator *allocator);

// LOWER STACK
extern void *desa_front_alloc_aligned	(DoubleEndedStackAllocator *allocator, const size_t size, const size_t alignment);
extern void *desa_front_alloc		(DoubleEndedStackAllocator *allocator, const size_t size);
extern void desa_front_free		(DoubleEndedStackAllocator *allocator, void *ptr);
extern void desa_front_reset		(DoubleEndedStackAllocator *allocator);

// UPPER STACK
extern void *desa_back_alloc_aligned	(DoubleEndedStackAllocator *allocator, const size_t size, const size_t alignment);
extern void *desa_back_alloc		(DoubleEndedStackAllocator *allocator, const size_t size);
extern void desa_back_free		(DoubleEndedStackAllocator *allocator, void *ptr);
extern void desa_back_reset		(DoubleEndedStackAllocator *allocator);

// FOR DEBUBGGING
extern const size_t desa_used_space		(DoubleEndedStackAllocator *allocator);
extern const size_t desa_remaining_space	(DoubleEndedStackAllocator *allocator);
extern void desa_show_memory			(DoubleEndedStackAllocator *allocator);

#endif	// _DOUBLE_ENDED_STACK_ALLOCATOR_
