#ifndef _DOUBLE_BUFFERED_ALLOCATOR_
#define _DOUBLE_BUFFERED_ALLOCATOR_
#include "stack_allocator.h"
#include <stddef.h>						// size_t

typedef struct
{
	unsigned int current_stack;
	StackAllocator stack[2];
} DoubleBufferedAllocator;				// 16 bytes

extern void dba_init			(DoubleBufferedAllocator *allocator, const size_t total_size);
extern void dba_terminate		(DoubleBufferedAllocator *allocator);
extern void dba_reset			(DoubleBufferedAllocator *allocator);
extern void *dba_alloc_aligned	(DoubleBufferedAllocator *allocator, const size_t size, const size_t alignment);
extern void *dba_alloc			(DoubleBufferedAllocator *allocator, const size_t size);
extern void dba_free			(DoubleBufferedAllocator *allocator, void *ptr);

// switches a stack to another one
extern void dba_swap_buffers	(DoubleBufferedAllocator *allocator);

//
// FOR DEBUGGING (CURRENT SELECTED STACK)
//

// returns pointer to allocated memory for both of stacks
extern void *dba_get_pointer			(DoubleBufferedAllocator *allocator);

// shows memory occupied by stack
extern void dba_show_memory				(DoubleBufferedAllocator *allocator);
extern const size_t dba_used_space		(DoubleBufferedAllocator *allocator);
extern const size_t dba_remaining_space	(DoubleBufferedAllocator *allocator);
extern void dba_space_info				(DoubleBufferedAllocator *allocator);
extern void dba_show_all_info			(DoubleBufferedAllocator *allocator);
#endif	// _DOUBLE_BUFFERED_ALLOCATOR_
