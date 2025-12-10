#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "deque.h"
#include "sorting.h"
#include "file_io.h"

#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"
#define HEAP_OUTPUT_FILE "heap_output.txt"

void print_menu() {
    printf("\n=== Программа сортировки дека ===\n");
    printf("1. Ввести числа с клавиатуры\n");
    printf("2. Отсортировать дек пузырьком\n");
    printf("3. Отсортировать дек пирамидально\n");
    printf("4. Сравнить методы сортировки (на деке)\n");
    printf("5. Загрузить дек из файла\n");
    printf("6. Вывести содержимое дека\n");
    printf("7. Тестировать на всех тестовых данных\n");
    printf("8. Выйти\n");
    printf("Выберите опцию: ");
}

void process_command_line(int argc, char* argv[]) {
    if (argc == 3 && strcmp(argv[1], "--file") == 0) {
        printf("Загрузка данных из файла: %s\n", argv[2]);
        
        Deque* deque = load_deque_from_file(argv[2]);
        if (deque) {
            printf("Дек загружен, размер: %d\n", get_size(deque));
            printf("Исходный дек: ");
            print_deque(deque);
            
            Deque* bubble_copy = copy_deque(deque);
            bubble_sort_deque(bubble_copy);
            printf("Отсортирован пузырьком: ");
            print_deque(bubble_copy);
            save_deque_to_file(OUTPUT_FILE, bubble_copy);
            delete_deque(bubble_copy);
            
            printf("Результат сохранен в %s\n", OUTPUT_FILE);
            delete_deque(deque);
        } else {
            printf("Не удалось загрузить файл\n");
        }
        exit(0);
    }
}

// Функция для безопасного чтения строки
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

// Создание дека из строки ввода
Deque* create_deque_from_input(const char* input) {
    Deque* deque = create_deque();
    char* input_copy = strdup(input);
    char* token = strtok(input_copy, " ");
    
    while (token != NULL) {
        push_rear(deque, atoi(token));
        token = strtok(NULL, " ");
    }
    
    free(input_copy);
    return deque;
}

// Сравнение методов сортировки
void compare_sorting_methods() {
    printf("\n=== Сравнение методов сортировки ===\n");
    
    // Размеры для тестирования
    int test_sizes[] = {100, 500, 1000, 5000, 10000, 50000};
    int test_count = sizeof(test_sizes) / sizeof(test_sizes[0]);
    
    printf("\nРазмер | Пузырек (сек) | Пирамидальная (сек) | Ускорение\n");
    printf("-----------------------------------------------------------\n");
    
    for (int i = 0; i < test_count; i++) {
        Deque* deque = create_deque();
        srand(time(NULL) + i);
        
        for (int j = 0; j < test_sizes[i]; j++) {
            push_rear(deque, rand() % 10000);
        }
        
        double bubble_time = measure_bubble_sort_time(deque);
        double heap_time = measure_heap_sort_time(deque);
        
        printf("%6d | %13.6f | %18.6f | %.2fx\n", 
               test_sizes[i], bubble_time, heap_time, 
               bubble_time / heap_time);
        
        delete_deque(deque);
    }
    
    printf("\nВывод: Пирамидальная сортировка значительно быстрее на больших размерах дека.\n");
}

// Тестирование на файлах
void test_all_files() {
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

// Создание директории tests
void create_tests_directory(void) {
    int result;
#ifdef _WIN32
    result = system("mkdir tests 2>nul");
#else
    result = system("mkdir -p tests 2>/dev/null");
#endif
    (void)result;
}

int main(int argc, char* argv[]) {
    process_command_line(argc, argv);
    
    srand(time(NULL));
    Deque* deque = NULL;
    
    int choice;
    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода!\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        
        switch(choice) {
            case 1: {
                printf("Введите числа через пробел: ");
                char input[1024];
                if (!safe_fgets(input, sizeof(input), stdin)) {
                    printf("Ошибка чтения ввода!\n");
                    break;
                }
                
                if (deque) delete_deque(deque);
                deque = create_deque_from_input(input);
                
                save_deque_to_file(INPUT_FILE, deque);
                printf("Введено %d чисел, дек создан, сохранено в %s\n", 
                       get_size(deque), INPUT_FILE);
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
                    
                    save_deque_to_file(OUTPUT_FILE, deque);
                    printf("Результат сохранен в %s\n", OUTPUT_FILE);
                } else {
                    printf("Дек пуст! Сначала введите числа.\n");
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
                    
                    save_deque_to_file(HEAP_OUTPUT_FILE, deque);
                    printf("Результат сохранен в %s\n", HEAP_OUTPUT_FILE);
                } else {
                    printf("Дек пуст! Сначала введите числа.\n");
                }
                break;
                
            case 4:
                compare_sorting_methods();
                break;
                
            case 5: {
                printf("Введите имя файла для загрузки: ");
                char filename[256];
                if (scanf("%255s", filename) != 1) {
                    printf("Ошибка ввода имени файла!\n");
                    while (getchar() != '\n');
                    break;
                }
                while (getchar() != '\n');
                
                if (deque) delete_deque(deque);
                deque = load_deque_from_file(filename);
                
                if (deque) {
                    printf("Загружен дек из файла %s\n", filename);
                    printf("Размер дека: %d\n", get_size(deque));
                } else {
                    printf("Ошибка загрузки файла\n");
                }
                break;
            }
                
            case 6:
                if (deque && !is_empty(deque)) {
                    printf("Содержимое дека (размер: %d):\n", get_size(deque));
                    print_deque(deque);
                } else {
                    printf("Дек пуст!\n");
                }
                break;
                
            case 7:
                printf("Проверка тестовых файлов...\n");
                create_tests_directory();
                
                if (system("ls tests/*.txt 2>/dev/null | head -1") != 0) {
                    printf("Генерация тестовых файлов...\n");
                    generate_multiple_test_files(7);
                }
                
                test_all_files();
                break;
                
            case 8:
                printf("Выход...\n");
                break;
                
            default:
                printf("Неверный выбор!\n");
        }
    } while (choice != 8);
    
    if (deque) delete_deque(deque);
    
    return 0;
}