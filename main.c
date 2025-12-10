// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inout.h"

int main(int argc, char* argv[])
{
    // Проверяем аргументы командной строки
    if (argc == 3 && strcmp(argv[1], "--file") == 0) {
        // Режим работы с файлом
        process_file_mode(argv[2]);
        return 0;
    } 
    else if (argc == 1) {
        // Интерактивный режим (без аргументов)
        allinout();
        return 0;
    }
    else {
        // Неправильное использование
        printf("Использование:\n");
        printf("  %s                       - интерактивный режим\n", argv[0]);
        printf("  %s --file <имя_файла>    - работа с файлом\n", argv[0]);
        printf("\nПример:\n");
        printf("  %s --file data.txt\n", argv[0]);
        return 1;
    }
}