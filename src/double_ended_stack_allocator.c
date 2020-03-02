#include "../include/double_ended_stack_allocator.h"
#include "../include/memory.h"				// ALIGNED_SIZE, DEFAULT_ALIGNMENT
#include "../debug/debug.h"				// M_ASSERT, ASSERT, PRINT, show_memory
#include <stdlib.h>					// malloc, free

#define SIZE_OF_ALLOCATION_BLOCK_SIZE		sizeof(size_t)

void desa_init(DoubleEndedStackAllocator *allocator, const size_t total_size)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
	allocator->start 			= (char *)malloc(total_size);
	allocator->end 				= allocator->start + total_size;
	allocator->current_front	= allocator->start;
	allocator->current_back		= allocator->end;
	MEMSET_ZERO(allocator->start, total_size);
}

void desa_terminate(DoubleEndedStackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
	free(allocator->start);
	allocator->start = allocator->end = allocator->current_front = allocator->current_back = NULL;
}

void desa_reset(DoubleEndedStackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
	allocator->current_front	= allocator->start;
	allocator->current_back		= allocator->end;
	MEMSET_ZERO(allocator->start, (const size_t)(allocator->end - allocator->start));
}

//
// LOWER STACK
//

void *desa_front_alloc_aligned(DoubleEndedStackAllocator *allocator, const size_t size, const size_t alignment)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
	M_ASSERT((alignment & (alignment - 1)) == 0, "Incorrect alignment");

	const size_t aligned_size = ALIGNED_SIZE(SIZE_OF_ALLOCATION_BLOCK_SIZE + size, alignment);

	if (allocator->current_front + aligned_size > allocator->current_back)
	{
		PRINT("There is no available space for lower stack");
		return NULL;
	}

	char *ptr = (char *)allocator->current_front;

	*((size_t *)ptr) = aligned_size;
	ptr += SIZE_OF_ALLOCATION_BLOCK_SIZE;

	allocator->current_front += aligned_size;
	return (void *)ptr;
}

void *desa_front_alloc(DoubleEndedStackAllocator *allocator, const size_t size)
{
	return desa_front_alloc_aligned(allocator, size, DEFAULT_ALIGNMENT);
}

void desa_front_free(DoubleEndedStackAllocator *allocator, void *ptr)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
#ifdef IGNORE_NULL
	if (ptr == NULL)
		return;
#else
	ASSERT(ptr != NULL);
#endif
	M_ASSERT(((char *)ptr > allocator->start) && ((char *)ptr < allocator->current_front), "Pointer is out of the stack borders");

	char *temp_ptr = (char *)ptr;

	temp_ptr -= SIZE_OF_ALLOCATION_BLOCK_SIZE;
	const size_t allocation_size = *((size_t *)temp_ptr);

	M_ASSERT(allocation_size == (const size_t)(allocator->current_front - temp_ptr), "Attempt to free non-top block of lower stack");

	allocator->current_front -= allocation_size;
}

void desa_front_reset(DoubleEndedStackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
	allocator->current_front 	= allocator->start;
}

//
// UPPER STACK
//

void *desa_back_alloc_aligned(DoubleEndedStackAllocator *allocator, const size_t size, const size_t alignment)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
	M_ASSERT((alignment & (alignment - 1)) == 0, "Incorrect alignment");

	const size_t aligned_size = ALIGNED_SIZE(SIZE_OF_ALLOCATION_BLOCK_SIZE + size, alignment);

	if (allocator->current_back - aligned_size < allocator->current_front)
	{
		PRINT("There is no available space for upper stack");
		return NULL;
	}

	// move current pointer left to aligned_size bytes
	allocator->current_back -= aligned_size;

	char *ptr = (char *)allocator->current_back;

	// store current address os current_back pointer 
	*((size_t *)ptr) = aligned_size;
	
	// move pointer to 4 bytes right
	ptr += SIZE_OF_ALLOCATION_BLOCK_SIZE;

	return (void *)ptr;
}

void *desa_back_alloc(DoubleEndedStackAllocator *allocator, const size_t size)
{
	return desa_back_alloc_aligned(allocator, size, DEFAULT_ALIGNMENT);
}

void desa_back_free(DoubleEndedStackAllocator *allocator, void *ptr)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
#ifdef IGNORE_NULL
	if (ptr == NULL)
		return;
#else
	ASSERT(ptr != NULL);
#endif
	M_ASSERT(((char *)ptr > allocator->current_back) && ((char *)ptr < allocator->end), "Pointer is out of the stack borders");

	char *temp_ptr = (char *)ptr;

	temp_ptr -= SIZE_OF_ALLOCATION_BLOCK_SIZE;
	const size_t allocation_size = *((size_t *)temp_ptr);

	M_ASSERT(temp_ptr == (char *)(allocator->current_back), "Attempt to free non-top block of upper stack");

	allocator->current_back += allocation_size;
}

void desa_back_reset(DoubleEndedStackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
	allocator->current_back 	= allocator->end;
}

const size_t desa_used_space(DoubleEndedStackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
	const size_t lower_stack_used_space = (const size_t)(allocator->current_front - allocator->start);
	const size_t upper_stack_used_space = (const size_t)(allocator->end - allocator->current_back);
	return lower_stack_used_space + upper_stack_used_space;
}

const size_t desa_remaining_space(DoubleEndedStackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
	return (const size_t)(allocator->current_back - allocator->current_front);
}

void desa_show_memory(DoubleEndedStackAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Double-Ended Stack Allocator is NULL");
	PRINT("Memory occupied by DoubleEndedStackAllocator:");
	show_memory(allocator->start, (const size_t)(allocator->end - allocator->start));
}

void desa_space_info(DoubleEndedStackAllocator *allocator)
{
	PRINT_UINT(desa_used_space(allocator));
	PRINT_UINT(desa_remaining_space(allocator));
}

void desa_show_all_info(DoubleEndedStackAllocator *allocator)
{
	desa_space_info(allocator);
	desa_show_memory(allocator);
}
