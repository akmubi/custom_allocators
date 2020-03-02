#include "debug/debug.h"

#include "include/linear_allocator.h"
#include "include/stack_allocator.h"
#include "include/pool_allocator.h"
#include "include/double_buffered_allocator.h"
#include "include/double_ended_stack_allocator.h"

// Uncomment to run particular test
#define LINEAR_TEST
#define STACK_TEST
#define POOL_TEST
#define DOUBLE_BUFFERED_TEST
#define DOUBLE_ENDED_TEST

int main(void)
{
	//
	// LINEAR ALLOCATOR
	//
#ifdef LINEAR_TEST
	{
		LinearAllocator allocator;
		const size_t total_size = sizeof(int) * 10;

		PRINT("[LINEAR ALLOCATOR INITIALIZED]");
		la_init(&allocator, total_size);
		la_show_all_info(&allocator);

		// First Allocation
		PRINT("[FIRST ALLOCATION]");
		const size_t n = 3;
		int *array = la_alloc(&allocator, n * sizeof(int));
		if (array != NULL)
		{
			for (int i = 0; i < n; i++)
				array[i] = 3;
			la_show_all_info(&allocator);
		}
		

		// Second Allocation
		PRINT("[SECOND ALLOCATION]");
		const size_t m = 6;
		int *array2 = la_alloc(&allocator, m * sizeof(int));
		
		if (array2 != NULL)
		{
			for (int i = 0; i < m; i++)
			array2[i] = 7;
			la_show_all_info(&allocator);
		}

		// Third Allocation
		PRINT("[THIRD ALLOCATION]");

		// Won't be available enough memory
		const size_t l = 2;
		int *array3 = la_alloc(&allocator, l * sizeof(int));

		PRINT("[RESETTING]");
		la_reset(&allocator);
		la_show_all_info(&allocator);

		la_terminate(&allocator);
		PRINT("[LINEAR ALLOCATOR TERMINATED]");
	}
#endif	// LINEAR_TEST

	//
	// STACK ALLOCATOR
	//
#ifdef STACK_TEST
	{
		StackAllocator allocator;
		const size_t total_size = sizeof(int) * 10;

		PRINT("[STACK ALLOCATOR INITIALIZED]");
		sa_init(&allocator, total_size);
		sa_show_all_info(&allocator);

		// First allocation
		PRINT("[FIRST ALLOCATION]");
		const size_t n = 4;
		int *array = sa_alloc(&allocator, n * sizeof(*array));
		if (array != NULL)
		{
			for (int i = 0; i < n; i++)
				array[i] = 0x3;
			sa_show_all_info(&allocator);
		}

		// Second allocation
		PRINT("[SECOND ALLOCATION]");
		const size_t m = 12;
		char *array2 = sa_alloc(&allocator, m * sizeof(*array2));
		if (array2 != NULL)
		{
			for (int i = 0; i < m; i++)
				array2[i] = 0x7;
			sa_show_all_info(&allocator);
		}


		// Third allocation
		// no space available
		PRINT("[THIRD ALLOCATION]");
		const size_t l = 4;
		float *array3 = sa_alloc(&allocator, l * sizeof(*array3));

		sa_show_all_info(&allocator);

		// fine if IGNORE_NULL turned on
		PRINT("[FIRST DEALLOCATION (array3)]");
		sa_free(&allocator, array3);

		sa_space_info(&allocator);

		// // error
		// PRINT("[DEALLOCATION of array]");
		// sa_free(&allocator, array);

		// error
		PRINT("[SECOND DEALLOCATION (array2)]");
		sa_free(&allocator, array2);
		sa_space_info(&allocator);

		PRINT("[RESETTING]");
		sa_reset(&allocator);
		sa_show_all_info(&allocator);

		sa_terminate(&allocator);
		PRINT("[STACK ALLOCATOR TERMINATED]");
	}
#endif	// STACK_TEST

	//
	// POOL ALLOCATOR
	//
#ifdef POOL_TEST
	{
		PoolAllocator allocator;
		
		const size_t num_of_elements = 5;
		
		struct SomeObject
		{
			int data1;
			short data2;
			char data3;
		} *array_of_objects[num_of_elements];

		const size_t element_size = sizeof(struct SomeObject);

		PRINT("[POOL ALLOCATOR INITIALIZED]");
		pa_init(&allocator, num_of_elements, element_size);
		pa_show_all_info(&allocator);

		PRINT("[ALLOCAIONS]");
		for (int i = 0; i < num_of_elements; i++)
		{
			array_of_objects[i] = (struct SomeObject *)pa_alloc(&allocator);
			if (array_of_objects[i] != NULL)
			{
				array_of_objects[i]->data1 = 0x0 + i;
				array_of_objects[i]->data2 = 0x9 + i;
				array_of_objects[i]->data3 = 'A' + i;
			}
		}
		pa_show_all_info(&allocator);

		PRINT("[DEALLOCATIONS]");
		pa_free(&allocator, array_of_objects[0]);
		pa_free(&allocator, array_of_objects[3]);
		pa_free(&allocator, array_of_objects[4]);
		pa_free(&allocator, array_of_objects[1]);
		pa_free(&allocator, array_of_objects[2]);

		pa_show_all_info(&allocator);

		PRINT("[RESETTING]");
		pa_reset(&allocator);
		pa_show_all_info(&allocator);

		pa_terminate(&allocator);
		PRINT("[POOL ALLOCATOR TERMINATED]");
	}
#endif	// POOL_TEST

	//
	// DOUBLE-BUFFERED ALLOCATOR
	//
#ifdef DOUBLE_BUFFERED_TEST
	{
		DoubleBufferedAllocator allocator;
		const size_t total_size = 10 * sizeof(int);

		PRINT("[DOUBLE-BUFFERED ALLOCATOR INITIALIZED]");
		dba_init(&allocator, total_size);
		dba_show_all_info(&allocator);
		
		// FIRST BUFFER
		PRINT("FIRST BUFFER\n");

		// first allocation
		PRINT("first allocation\n");

		const size_t n = 3;
		int *array = dba_alloc(&allocator, n * sizeof(*array));
		if (array != NULL)
		{
			for (int i = 0; i < n; i++)
			{
				array[i] = 0xFFFFFFFF;
			}
			dba_show_all_info(&allocator);
		}

		// second allocation
		PRINT("second allocation\n");
		const size_t m = 5;
		int *array2 = dba_alloc(&allocator, m * sizeof(*array2));
		if (array2 != NULL)
		{
			for (int i = 0; i < m; i++)
			{
				array2[i] = 0xAAAAAAAA;
			}
			dba_show_all_info(&allocator);
		}

		PRINT("[BUFFERS SWAPED]");
		dba_swap_buffers(&allocator);

		// SECOND BUFFER
		PRINT("SECOND BUFFER\n");

		// first allocation
		PRINT("first allocation\n");
		const size_t k = 4;
		long long int *array3 = dba_alloc(&allocator, k * sizeof(*array3));
		if (array3 != NULL)
		{
			for (int i = 0; i < k; i++)
			{
				array3[i] = 0xDDDDDDDD;
			}
			dba_show_all_info(&allocator);
		}

		// error will occur if IGNORE_NULL turned off
		const size_t l = 3;
		float *array4 = dba_alloc(&allocator, l * sizeof(*array4));

		PRINT("[RESETTING #1]");
		dba_reset(&allocator);
		dba_show_all_info(&allocator);

		PRINT("[BUFFERS SWAPED]");
		dba_swap_buffers(&allocator);

		PRINT("[RESETTING #2]");
		dba_reset(&allocator);
		dba_show_all_info(&allocator);

		dba_terminate(&allocator);
		PRINT("[DOUBLE-BUFFERED ALLOCATOR TERMINATED]");
	}
#endif	// DOUBLE_BUFFERED_TEST

	//
	// DOUBLE-ENDED STACK ALLOCATOR
	//
#ifdef DOUBLE_ENDED_TEST
	{
		DoubleEndedStackAllocator allocator;
		const size_t total_size = 128;

		PRINT("[DOUBLE-ENDED STACK ALLOCATOR INITIALIZED]");
		desa_init(&allocator, 128);
		desa_show_all_info(&allocator);

		const size_t l_n = 10, l_m = 15, u_n = 15, u_m = 5;
		int *array, *array4;
		char *array2, *array3;

		// First front allocation
		PRINT("FIRST FRONT ALLOCATION");

		array = desa_front_alloc(&allocator, l_n * sizeof(*array));
		if (array != NULL)
		{
			for (int i = 0; i < l_n; i++)
				array[i] = 0xAAAAAAAA;
			desa_show_all_info(&allocator);
		}

		// Second front allocation
		PRINT("SECOND FRONT ALLOCATION");
		array2 = desa_front_alloc(&allocator, l_m * sizeof(*array2));
		if (array2 != NULL)
		{
			for (int i = 0; i < l_m; i++)
				array2[i] = 0xBB;
			desa_show_all_info(&allocator);
		}

		// First back allocation
		PRINT("FIRST BACK ALLOCATION");
		array3 = desa_back_alloc(&allocator, u_n * sizeof(*array3));
		if (array3 != NULL)
		{
			for (int i = 0; i < u_n; i++)
				array3[i] = 0xCC;
			desa_show_all_info(&allocator);
		}

		// Second back allocation
		PRINT("SECOND BACK ALLOCATION");
		array4 = desa_back_alloc(&allocator, u_m * sizeof(*array4));
		if (array4 != NULL)
		{
			for (int i = 0; i < u_m; i++)
				array4[i] = 0xDDDDDDDD;
			desa_show_all_info(&allocator);
		}

		PRINT("BACK DEALLOCATION");
		desa_back_free(&allocator, array4);
		desa_space_info(&allocator);

		PRINT("FRONT DEALLOCATION");
		desa_front_free(&allocator, array2);
		desa_space_info(&allocator);

		// Also you can rest both of stacks with desa_reset funtion
		PRINT("RESETTING UPPER STACK");
		desa_back_reset(&allocator);
		desa_space_info(&allocator);

		PRINT("RESETTING LOWER STACK");
		desa_front_reset(&allocator);
		desa_space_info(&allocator);

		desa_terminate(&allocator);
		PRINT("[DOUBLE-ENDED STACK ALLOCATOR TERMINATED]");
	}
#endif	// DOUBLE_ENDED_TEST
	return 0;
}
