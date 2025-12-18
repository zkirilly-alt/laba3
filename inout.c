#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "deque.h"
#include "sorting.h"
#include "file_io.h"
#include "inout.h"

#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"
#define HEAP_OUTPUT_FILE "heap_output.txt"

void process_file_mode(const char* filename) {
    printf("=== Режим работы с файлом ===\n");
    printf("Загрузка данных из файла: %s\n", filename);
    
    Deque* deque = load_deque_from_file(filename);
    if (!deque) {
        printf("Ошибка: не удалось загрузить файл '%s'\n", filename);
        printf("Убедитесь, что файл существует и содержит числа.\n");
        return;
    }
    
    printf("Успешно загружено. Размер дека: %d\n", get_size(deque));
    
    printf("\nИсходный ряд из файла:\n");
    print_deque(deque);
    
    Deque* copy_for_sorting = copy_deque(deque);
    if (!copy_for_sorting) {
        printf("Ошибка создания копии дека!\n");
        delete_deque(deque);
        return;
    }
    
    printf("\nСортировка пузырьком...\n");
    bubble_sort_deque(copy_for_sorting);
    
    printf("Отсортированный ряд:\n");
    print_deque(copy_for_sorting);
    
    
    if (save_deque_to_file(OUTPUT_FILE, copy_for_sorting)) {
        printf("\nРезультат сохранен в файл: %s\n", OUTPUT_FILE);
    }
    

    delete_deque(deque);
    delete_deque(copy_for_sorting);
    
    printf("=== Завершение работы с файлом ===\n");
}

void process_command_line(int argc, char* argv[]) {
    if (argc == 3 && strcmp(argv[1], "--file") == 0) {
        process_file_mode(argv[2]);
        exit(0);
    }
}


int safe_fgets(char* buffer, int size, FILE* stream) {
    if (fgets(buffer, size, stream) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

int is_valid_number(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (i == 0 && str[i] == '-') {
            continue;
        }
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    
    return 1;
}

Deque* create_deque_from_input(const char* input) {
    Deque* deque = create_deque();
    char* input_copy = strdup(input);
    
    if (input_copy == NULL) {
        delete_deque(deque);
        return NULL;
    }
    
    char* token = strtok(input_copy, " ");
    int numbers_added = 0;
    
    while (token != NULL) {
        if (!is_valid_number(token)) {
            printf("Ошибка: '%s' не является числом!\n", token);
            delete_deque(deque);
            free(input_copy);
            return NULL;
        }
        
        int num = atoi(token);
        push_rear(deque, num);
        numbers_added++;
        
        token = strtok(NULL, " ");
    }
    
    free(input_copy);
    
    if (numbers_added == 0) {
        printf("Ошибка: не введено ни одного числа!\n");
        delete_deque(deque);
        return NULL;
    }
    
    return deque;
}

void test_all_files(void) {
    printf("\n=== Тестирование на всех файлах из папки tests ===\n");
    
    int test_sizes[] = {100, 500, 1000, 5000, 10000, 50000, 100000};
    int test_count = sizeof(test_sizes) / sizeof(test_sizes[0]);
    
    printf("\nФайл           | Размер | Пузырек (сек) | Пирамид. (сек) | Ускорение\n");
    printf("---------------------------------------------------------------------\n");
    
    for (int i = 0; i < test_count; i++) {
        char filename[256];
        snprintf(filename, sizeof(filename), "tests/test_%d.txt", test_sizes[i]);
        
        Deque* deque = load_deque_from_file(filename);
        if (deque) {
            double bubble_time = measure_bubble_sort_time(deque);
            double heap_time = measure_heap_sort_time(deque);
            
            printf("%-15s | %6d | %13.6f | %13.6f | %.2fx\n", 
                   filename, test_sizes[i], bubble_time, heap_time, 
                   bubble_time / heap_time);
            
            delete_deque(deque);
        } else {
            printf("%-15s | %6d | Файл не найден\n", filename, test_sizes[i]);
        }
    }
}

void create_tests_directory(void) {
    int result;
#ifdef _WIN32
    result = system("mkdir tests 2>nul");
#else
    result = system("mkdir -p tests 2>/dev/null");
#endif
    (void)result;
}

void print_menu(void) {
    printf("\n=== Программа сортировки дека ===\n");
    printf("1. Ввести числа с клавиатуры\n");
    printf("2. Отсортировать дек пузырьком\n");
    printf("3. Отсортировать дек пирамидально\n");
    printf("4. Загрузить дек из файла\n");
    printf("5. Вывести содержимое дека\n");
    printf("6. Тестировать на всех тестовых данных\n");
    printf("7. Выйти\n");
    printf("Выберите опцию: ");
}

void allinout(void) {
    srand(time(NULL));
    Deque* deque = NULL;
    
    int choice;
    do {
        print_menu();
        
        char choice_str[10];
        if (!safe_fgets(choice_str, sizeof(choice_str), stdin)) {
            printf("Ошибка чтения ввода!\n");
            continue;
        }
        
        if (!is_valid_number(choice_str)) {
            printf("Ошибка: введите число от 1 до 7!\n");
            continue;
        }
        
        choice = atoi(choice_str);
        
        switch(choice) {
            case 1: {
                printf("Введите числа через пробел: ");
                char input[1024];
                if (!safe_fgets(input, sizeof(input), stdin)) {
                    printf("Ошибка чтения ввода!\n");
                    break;
                }
                
                Deque* new_deque = create_deque_from_input(input);
                
                if (new_deque) {
                    if (deque) delete_deque(deque);
                    deque = new_deque;
                    
                    if (save_deque_to_file(INPUT_FILE, deque)) {
                        printf("Введено %d чисел, дек создан, сохранено в %s\n", 
                               get_size(deque), INPUT_FILE);
                    } else {
                        printf("Ошибка сохранения в файл!\n");
                    }
                } else {
                    printf("Пожалуйста, вводите только числа!\n");
                }
                break;
            }
            
            case 2:
                if (deque && !is_empty(deque)) {
                    printf("Сортировка дека пузырьком...\n");
                    printf("Исходный дек (размер: %d):\n", get_size(deque));
                    print_deque(deque);
                    
                    clock_t start = clock();
                    bubble_sort_deque(deque);
                    clock_t end = clock();
                    
                    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
                    printf("Отсортированный дек: ");
                    print_deque(deque);
                    printf("Время сортировки: %.6f секунд\n", time_spent);
                    
                    if (save_deque_to_file(OUTPUT_FILE, deque)) {
                        printf("Результат сохранен в %s\n", OUTPUT_FILE);
                    } else {
                        printf("Ошибка сохранения результата!\n");
                    }
                } else {
                    printf("Дек пуст! Сначала введите числа (пункт 1).\n");
                }
                break;
                
            case 3:
                if (deque && !is_empty(deque)) {
                    printf("Пирамидальная сортировка дека...\n");
                    printf("Исходный дек (размер: %d):\n", get_size(deque));
                    print_deque(deque);
                    
                    clock_t start = clock();
                    heap_sort_deque(deque);
                    clock_t end = clock();
                    
                    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
                    printf("Отсортированный дек: ");
                    print_deque(deque);
                    printf("Время сортировки: %.6f секунд\n", time_spent);
                    
                    if (save_deque_to_file(HEAP_OUTPUT_FILE, deque)) {
                        printf("Результат сохранен в %s\n", HEAP_OUTPUT_FILE);
                    } else {
                        printf("Ошибка сохранения результата!\n");
                    }
                } else {
                    printf("Дек пуст! Сначала введите числа (пункт 1).\n");
                }
                break;
                
            case 4: {
                printf("Введите имя файла для загрузки: ");
                char filename[256];
                if (!safe_fgets(filename, sizeof(filename), stdin)) {
                    printf("Ошибка чтения имени файла!\n");
                    break;
                }
                
                size_t len = strlen(filename);
                if (len > 0 && filename[len-1] == '\n') {
                    filename[len-1] = '\0';
                }
                
                Deque* loaded_deque = load_deque_from_file(filename);
                if (loaded_deque) {
                    if (deque) delete_deque(deque);
                    deque = loaded_deque;
                    printf("Загружен дек из файла %s\n", filename);
                    printf("Размер дека: %d\n", get_size(deque));
                } else {
                    printf("Ошибка загрузки файла '%s'!\n", filename);
                }
                break;
            }
                
            case 5:
                if (deque && !is_empty(deque)) {
                    printf("Содержимое дека (размер: %d):\n", get_size(deque));
                    print_deque(deque);
                } else {
                    printf("Дек пуст!\n");
                }
                break;
                
            case 6:
                printf("Проверка тестовых файлов...\n");
                create_tests_directory();
                
                if (system("ls tests/*.txt 2>/dev/null | head -1") != 0) {
                    printf("Генерация тестовых файлов...\n");
                    generate_multiple_test_files(7);
                }
                
                test_all_files();
                break;
                
            case 7:
                printf("Выход...\n");
                break;
                
            default:
                printf("Неверный выбор! Введите число от 1 до 7.\n");
        }
    } while (choice != 7);
    
    if (deque) delete_deque(deque);
}