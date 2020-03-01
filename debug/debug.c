#include "debug.h"		// M_ASSERT

void show_memory(const void *start, const size_t size)
{
	M_ASSERT(start != NULL, "Start pointer is NULL");
	// buffer contains ASCII characters from this piece of memory
	unsigned char buffer[DEBUG_MEMORY_ROW_LENGTH + 1];
	unsigned char *pointer = (unsigned char *)start;
	for (int i = 0; i < size; i++)
	{
		// ADDRESSES
		if ((i % DEBUG_MEMORY_ROW_LENGTH) == 0)
			printf(" 0x%08X: ", (size_t)(pointer + i));

		// VALUES
		// 0F, ..., 01, 00
		printf("%02X ", pointer[i]);

		buffer[i % DEBUG_MEMORY_ROW_LENGTH] = pointer[i];

		if (((i + 1) % DEBUG_MEMORY_ALIGNMENT) == 0)
		 	printf(" ");
		if (((i + 1) % DEBUG_MEMORY_ROW_LENGTH) == 0)
		{
			printf(" | ");
			buffer[i + 1] = '\0';
			for (unsigned char *char_ptr = buffer; char_ptr != (buffer + DEBUG_MEMORY_ROW_LENGTH); char_ptr++)
			{
				// only printable characters
				if (*char_ptr < 0x20 || *char_ptr > 0x7F)
					printf(" ");
				else
					printf("%c", *char_ptr);
			}
			printf("\n");
		}
	}
	printf("\n"); // just in case
}
