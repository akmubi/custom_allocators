#ifndef _MEMORY_H
#define _MEMORY_H

#define DEFAULT_ALIGNMENT 4

// calculating aligned size
#define ALIGNED_SIZE(size, alignment) (((size) + (alignment - 1)) & ~(alignment - 1))

// calculating padding
#define PADDING(size, alignment) ((alignment - ((size) % alignment)) % alignment)

#endif	// _MEMORY_H_
