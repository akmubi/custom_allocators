#include "../include/linear_allocator.h"
#include "../debug/debug.h"				// M_ASSERT, PRINT
#include "../include/memory.h"				// ALIGNED_SIZE, DEFAULT_ALIGNMENT
#include <stdlib.h>					// malloc, free

void la_init(LinearAllocator *allocator, const size_t total_size)
{
	M_ASSERT(allocator != NULL, "Linear Allocator is NULL");
	allocator->start	= (char *)malloc(total_size);
	allocator->end		= allocator->start + total_size;
	allocator->current	= allocator->start;
	MEMSET_ZERO(allocator->start, total_size);
}

void la_terminate(LinearAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Linear Allocator is NULL");
	free(allocator->start);
	allocator->current = allocator->start = allocator->end = NULL;
}

void *la_alloc(LinearAllocator *allocator, const size_t size)
{
	return la_alloc_aligned(allocator, size, DEFAULT_ALIGNMENT);
}

void *la_alloc_aligned(LinearAllocator *allocator, const size_t size, const size_t alignment)
{
	M_ASSERT(allocator != NULL, "Linear Allocator is NULL");
	M_ASSERT((alignment & (alignment - 1)) == 0, "Incorrect alignment");
	
	const size_t aligned_size = ALIGNED_SIZE(size, alignment);
	
	void *ptr = allocator->current;
	allocator->current += aligned_size;

	if (allocator->current >= allocator->end)
	{
		PRINT("There is no available space");
		return NULL;
	}

	return ptr;
}

void la_reset(LinearAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Linear Allocator is NULL");
	allocator->current = allocator->start;
	MEMSET_ZERO(allocator->start, (const size_t)(allocator->end - allocator->start));
}

const size_t la_used_space(LinearAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Linear Allocator is NULL");
	return (const size_t)(allocator->current - allocator->start);
}

const size_t la_remaining_space(LinearAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Linear Allocator is NULL");
	return (const size_t)(allocator->end - allocator->current);
}

void la_show_memory(LinearAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Linear Allocator is NULL");
	PRINT("Memory occupied by LinearAllocator:");
	show_memory(allocator->start, (const size_t)(allocator->end - allocator->start));
}
