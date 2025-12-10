#ifndef INOUT_H
#define INOUT_H

#include <stdio.h>
#include "deque.h"

// Главная функция программы
void allinout(void);

// Функции меню и интерфейса
void print_menu(void);
void process_command_line(int argc, char* argv[]);  // УЖЕ ЕСТЬ

// Функция для работы с файлом через командную строку
void process_file_mode(const char* filename);  // ДОБАВЛЕНО

// Функции ввода/вывода
int safe_fgets(char* buffer, int size, FILE* stream);
int is_valid_number(const char* str);
Deque* create_deque_from_input(const char* input);

// Функции тестирования и работы с файлами
void test_all_files(void);
void create_tests_directory(void);

#endif // INOUT_H