#ifndef INOUT_H
#define INOUT_H

#include <stdio.h>
#include "deque.h"

void allinout(void);

void print_menu(void);
void process_command_line(int argc, char* argv[]);  

void process_file_mode(const char* filename); 

int safe_fgets(char* buffer, int size, FILE* stream);
int is_valid_number(const char* str);
Deque* create_deque_from_input(const char* input);

void test_all_files(void);
void create_tests_directory(void);

#endif 