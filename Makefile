TARGET = start
TEST_GEN = test_generator
OBJ = main.o deque.o sorting.o file_io.o inout.o
TEST_OBJ = test_generator.o deque.o sorting.o file_io.o inout.o

all: $(TARGET) $(TEST_GEN)

$(TARGET): $(OBJ)
	gcc -o $(TARGET) $(OBJ)

$(TEST_GEN): $(TEST_OBJ)
	gcc -o $(TEST_GEN) $(TEST_OBJ)

main.o: main.c deque.h sorting.h file_io.h inout.h
	gcc -c main.c

# Убрал main.c из зависимостей inout.o
inout.o: inout.c deque.h sorting.h file_io.h inout.h
	gcc -c inout.c

deque.o: deque.c deque.h
	gcc -c deque.c

sorting.o: sorting.c sorting.h deque.h
	gcc -c sorting.c

file_io.o: file_io.c file_io.h deque.h
	gcc -c file_io.c

test_generator.o: test_generator.c file_io.h deque.h sorting.h inout.h
	gcc -c test_generator.c

clean:
	rm -f *.o $(TARGET) $(TEST_GEN) input.txt output.txt heap_output.txt tests/*.txt sorting_comparison.png

.PHONY: all clean