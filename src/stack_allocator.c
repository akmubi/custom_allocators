#include "../include/stack_allocator.h"
#include "../debug/debug.h"				// M_ASSERT, ASSERT, PRINT, show_memory
#include "../include/memory.h"				// ALIGNED_SIZE, DEFAULT_ALIGNMENT
#include <stdlib.h>					// malloc, free

#define SIZE_OF_ALLOCATION_BLOCK_SIZE sizeof(size_t)  

void sa_init(StackAllocator *allocator, const size_t total_size)
{
	M_ASSERT(allocator != NULL, "Stack Allocator is NULL");
	allocator->start	= (char *)malloc(total_size);
	allocator->end		= allocator->start + total_size;
	allocator->current	= allocator->start;
	MEMSET_ZERO(allocator->start, total_size);
}

void sa_terminate(StackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Stack Allocator is NULL");
	free(allocator->start);
	allocator->current = allocator->start = allocator->end = NULL;
}

void *sa_alloc_aligned(StackAllocator *allocator, const size_t size, const size_t alignment)
{
	M_ASSERT(allocator != NULL, "Stack Allocator is NULL");
	M_ASSERT((alignment & (alignment - 1)) == 0, "Incorrect alignment");
	
	const size_t aligned_size = ALIGNED_SIZE(SIZE_OF_ALLOCATION_BLOCK_SIZE + size, alignment);

	if (allocator->current + aligned_size > allocator->end)
	{
		PRINT("There is no available space");
		return NULL;
	}
	
	char *ptr = (char *)allocator->current;

	// cast to size_t for storing allocation block size in first 4 bytes
	*((size_t *)(ptr)) = aligned_size;
	ptr += SIZE_OF_ALLOCATION_BLOCK_SIZE;

	// update current pointer
	allocator->current += aligned_size;
	return (void *)ptr;
}

void *sa_alloc(StackAllocator *allocator, const size_t size)
{
	return sa_alloc_aligned(allocator, size, DEFAULT_ALIGNMENT);
}

void sa_free(StackAllocator *allocator, void *ptr)
{
	M_ASSERT(allocator != NULL, "Stack Allocator is NULL");
#ifdef IGNORE_NULL
	if (ptr == NULL)
		return;
#else
	ASSERT(ptr != NULL);
#endif
	ASSERT(((char *)ptr > allocator->start) && ((char *)ptr < allocator->current));

	char *temp_ptr = (char *)ptr;

	// get size of allocated block
	temp_ptr -= SIZE_OF_ALLOCATION_BLOCK_SIZE;
	const size_t allocation_size = *((size_t*)temp_ptr);

	M_ASSERT(allocation_size == (size_t)(allocator->current - temp_ptr), "Attempt to free non-top block of stack");

	// update current pointer
	allocator->current -= allocation_size;
}

void sa_reset(StackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Stack Allocator is NULL");
	allocator->current = allocator->start;
	MEMSET_ZERO(allocator->start, (const size_t)(allocator->end - allocator->start));
}

const size_t sa_used_space(StackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Stack Allocator is NULL");
	return (const size_t)(allocator->current - allocator->start);
}

const size_t sa_remaining_space(StackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Stack Allocator is NULL");
	return (const size_t)(allocator->end - allocator->current);
}

void sa_show_memory(StackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Stack Allocator is NULL");
	PRINT("Memory occupied by StackAllocator:");
	show_memory(allocator->start, (const size_t)(allocator->end - allocator->start));
}
