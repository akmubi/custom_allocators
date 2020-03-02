all: allocators clean

allocators: main.o debug.o linear.o stack.o pool.o double_buffered.o double_ended.o
	gcc main.o debug.o linear.o stack.o pool.o double_buffered.o double_ended.o -o main

main.o: main.c
	gcc -c main.c

debug.o: debug/debug.c debug/debug.h
	gcc -c debug/debug.c

stack.o: src/stack_allocator.c include/stack_allocator.h
	gcc -c src/stack_allocator.c -o stack.o

linear.o: src/linear_allocator.c include/linear_allocator.h
	gcc -c src/linear_allocator.c -o linear.o

pool.o: src/pool_allocator.c include/pool_allocator.h
	gcc -c src/pool_allocator.c -o pool.o

double_buffered.o: src/double_buffered_allocator.c include/double_buffered_allocator.h
	gcc -c src/double_buffered_allocator.c -o double_buffered.o

double_ended.o: src/double_ended_stack_allocator.c include/double_ended_stack_allocator.h
	gcc -c src/double_ended_stack_allocator.c -o double_ended.o

clean:
	rm -f *.o
