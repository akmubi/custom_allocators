#include "../include/pool_allocator.h"
#include "../debug/debug.h"			// M_ASSERT, PRINT, show_memory
#include "../include/memory.h"			// DEFAULT_ALIGNMENT
#include <stdlib.h>				// malloc, free

void pa_init_aligned(PoolAllocator *allocator, const size_t num_of_elements, const size_t element_size, const size_t alignment)
{
	M_ASSERT(allocator != NULL, "Pool Allocator is NULL");
	M_ASSERT((alignment & (alignment - 1)) == 0, "Incorrect alignment");
	
	// if alignment will be less than default alignment
	// then we can't fit address into an element block
	ASSERT(alignment >= DEFAULT_ALIGNMENT);
	
	// calculate each element aligned size and total aligned size for allocations
	const size_t aligned_element_size = ALIGNED_SIZE(element_size, alignment);
	size_t aligned_size = num_of_elements * aligned_element_size; 
	
	// save sizes for reset function
	allocator->num_of_elements	= num_of_elements;
	allocator->element_size		= aligned_element_size;

	// allocate memory
	char *ptr = (char *)malloc(aligned_size);
	
	MEMSET_ZERO(ptr, aligned_size);

	// save pointer to head of allocated block
	allocator->start = ptr;

	// starting in head
	allocator->freelist.next = (struct FreeList *)ptr;

	// create iterator that starts in head too
	struct FreeList *iterator = allocator->freelist.next;

	for (int i = 0; i < num_of_elements; i++)
	{
		// store address of next element and go there
		iterator = iterator->next = (struct FreeList *)ptr;

		// address of next element
		ptr += aligned_element_size;
	}

	// last element doesn't point to anything
	iterator->next = NULL;
}

void pa_init(PoolAllocator *allocator, const size_t num_of_elements, const size_t element_size)
{
	pa_init_aligned(allocator, num_of_elements, element_size, DEFAULT_ALIGNMENT);
}

void pa_terminate(PoolAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Pool Allocator is NULL");
	free(allocator->start);
	allocator->start = NULL;
}

void *pa_alloc(PoolAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Pool Allocator is NULL");
	if (allocator->freelist.next == NULL)
	{
		PRINT("There is no available space");
		return NULL;
	}
	
	// get address of current head element of free list
	struct FreeList *head = allocator->freelist.next;

	// make next element address a new entry point
	allocator->freelist.next = head->next;

	// return first element
	return head;
}

void pa_free(PoolAllocator *allocator, void *ptr)
{
	M_ASSERT(allocator != NULL, "Pool Allocator is NULL");
#ifdef IGNORE_NULL
	if (ptr == NULL)
		return;
#else
	ASSERT(ptr != NULL);
#endif
	struct FreeList *head = (struct FreeList *)ptr;

	// make returned chunk point to head of free list
	head->next = allocator->freelist.next;

	// make returned chunk head of free list
	allocator->freelist.next = head;
}

void pa_reset(PoolAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Pool Allocator is NULL");
	MEMSET_ZERO(allocator->start, allocator->num_of_elements * allocator->element_size);

	struct FreeList *iterator = (struct FreeList *)allocator->start;
	char *ptr = allocator->start;
	for (int i = 0; i < allocator->num_of_elements; i++)
	{
		iterator = iterator->next = (struct FreeList *)ptr;
		ptr += allocator->element_size;
	}
	iterator->next = NULL;

	// return next pointer to head of allocated block
	allocator->freelist.next = (struct FreeList *)allocator->start;
}

void pa_show_memory(PoolAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Pool Allocator is NULL");
	PRINT("Memory occupied by PoolAllocator:");
	show_memory(allocator->start, allocator->element_size * allocator->num_of_elements);
}

void *pa_get_header(PoolAllocator *allocator)
{
	M_ASSERT(allocator != NULL, "Pool Allocator is NULL");
	return (void *)allocator->freelist.next;
}

void pa_show_all_info(PoolAllocator *allocator)
{
	PRINT_HEX(pa_get_header(allocator));
	pa_show_memory(allocator);
}