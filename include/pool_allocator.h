#ifndef _POOL_ALLOCATOR_H_
#define _POOL_ALLOCATOR_H_
#include <stddef.h>					// size_t

typedef struct
{
	struct FreeList
	{
		struct FreeList *next;
	};
	char *start;
	size_t num_of_elements;
	size_t element_size;
} PoolAllocator;					// 8 bytes

extern void pa_init			    (PoolAllocator *allocator, const size_t num_of_elements, const size_t element_size);
extern void pa_init_aligned	(PoolAllocator *allocator, const size_t num_of_elements, const size_t element_size, const size_t alignment);
extern void pa_free			    (PoolAllocator *allocator, void *ptr);
extern void *pa_alloc		    (PoolAllocator *allocator);
extern void pa_terminate	  (PoolAllocator *allocator);
extern void pa_reset		    (PoolAllocator *allocator);

// FOR DEBUGGING
extern void pa_show_memory	(PoolAllocator *allocator);
extern void *pa_get_header	(PoolAllocator *allocator);
#endif	// _POOL_ALLOCATOR_H_
