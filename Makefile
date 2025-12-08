CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = start
TEST_GEN = test_generator
OBJ = main.o deque.o sorting.o file_io.o
TEST_OBJ = test_generator.o file_io.o deque.o sorting.o

all: $(TARGET) $(TEST_GEN)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

$(TEST_GEN): $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $(TEST_GEN) $(TEST_OBJ)

main.o: main.c deque.h sorting.h file_io.h
	$(CC) $(CFLAGS) -c main.c

deque.o: deque.c deque.h
	$(CC) $(CFLAGS) -c deque.c

sorting.o: sorting.c sorting.h deque.h
	$(CC) $(CFLAGS) -c sorting.c

file_io.o: file_io.c file_io.h deque.h
	$(CC) $(CFLAGS) -c file_io.c

test_generator.o: test_generator.c file_io.h
	$(CC) $(CFLAGS) -c test_generator.c

clean:
	rm -f *.o $(TARGET) $(TEST_GEN) input.txt output.txt heap_output.txt tests/*.txt sorting_comparison.png

test: all
	@echo "Тестирование программы..."
	@echo "5 3 8 1 9 2" > test_input.txt
	./$(TARGET) --file test_input.txt || true
	@rm -f test_input.txt

