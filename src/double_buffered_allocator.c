#include "../include/double_buffered_allocator.h"
#include "../debug/debug.h"				// M_ASSERT, PRINT, show_memory

void dba_init(DoubleBufferedAllocator *allocator, const size_t total_size)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
	sa_init(&allocator->stack[0], total_size);
	sa_init(&allocator->stack[1], total_size);
	allocator->current_stack = 0;
}

void *dba_alloc_aligned(DoubleBufferedAllocator *allocator, const size_t size, const size_t alignment)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
	return sa_alloc_aligned(&allocator->stack[allocator->current_stack], size, alignment);
}

void *dba_alloc(DoubleBufferedAllocator *allocator, const size_t size)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
	return sa_alloc(&allocator->stack[allocator->current_stack], size);
}

void dba_free(DoubleBufferedAllocator *allocator, void *ptr)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
#ifdef IGNORE_NULL
	if (ptr == NULL)
		return;
#else
	ASSERT(ptr != NULL);
#endif
	sa_free(&allocator->stack[allocator->current_stack], ptr);
}

void dba_swap_buffers(DoubleBufferedAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
	allocator->current_stack = !allocator->current_stack;
}

void dba_reset(DoubleBufferedAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
	sa_reset(&allocator->stack[allocator->current_stack]);
}

void dba_terminate(DoubleBufferedAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
	sa_terminate(&allocator->stack[0]);
	sa_terminate(&allocator->stack[1]);
}

// For debug purposes
void *dba_get_pointer(DoubleBufferedAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
	return (void *)allocator->stack[allocator->current_stack].start;
}

void dba_show_memory(DoubleBufferedAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
	PRINT("Memory occupied by DoubleBufferedAllocator:");
	if (allocator->current_stack == 0)
		PRINT("Stack #1");
	else
		PRINT("Stack #2");
	sa_show_memory(&allocator->stack[allocator->current_stack]);
}

const size_t dba_used_space(DoubleBufferedAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
	return sa_used_space(&allocator->stack[allocator->current_stack]);
}

const size_t dba_remaining_space(DoubleBufferedAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Buffered Allocator is NULL");
	return sa_remaining_space(&allocator->stack[allocator->current_stack]);
}
