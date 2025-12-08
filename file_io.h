#ifndef FILE_IO_H
#define FILE_IO_H

#include "deque.h"

// Работа с файлами
int save_array_to_file(const char* filename, int* arr, int size);
int load_array_from_file(const char* filename, int** arr, int* size);
int save_deque_to_file(const char* filename, Deque* deque);
Deque* load_deque_from_file(const char* filename);

// Генерация тестовых данных
void generate_test_file(const char* filename, int size, int min_val, int max_val);
void generate_multiple_test_files(int base_count);

#endif