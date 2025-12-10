TARGET = start
TEST_GEN = test_generator
OBJ = main.o deque.o sorting.o file_io.o
TEST_OBJ = test_generator.o file_io.o deque.o sorting.o

all: start test_generator

start: $(OBJ)
	gcc  -o start $(OBJ)

test_generator: $(TEST_OBJ)
	gcc  -o test_generator $(TEST_OBJ)

main.o: main.c deque.h sorting.h file_io.h
	gcc  -c main.c

deque.o: deque.c deque.h
	gcc  -c deque.c

sorting.o: sorting.c sorting.h deque.h
	gcc  -c sorting.c

file_io.o: file_io.c file_io.h deque.h
	gcc  -c file_io.c

test_generator.o: test_generator.c file_io.h
	gcc  -c test_generator.c

clean:
	rm -f *.o start test_generator input.txt output.txt heap_output.txt tests/*.txt sorting_comparison.png