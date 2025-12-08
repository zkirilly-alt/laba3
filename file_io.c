#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file_io.h"

int save_array_to_file(const char* filename, int* arr, int size) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Ошибка открытия файла для записи");
        return 0;
    }
    
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d ", arr[i]);
    }
    
    fclose(file);
    return 1;
}

int load_array_from_file(const char* filename, int** arr, int* size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла для чтения");
        return 0;
    }
    
    *size = 0;
    int temp;
    while (fscanf(file, "%d", &temp) == 1) {
        (*size)++;
    }
    
    rewind(file);
    
    *arr = (int*)malloc((*size) * sizeof(int));
    if (!*arr) {
        fclose(file);
        return 0;
    }
    
    for (int i = 0; i < *size; i++) {
        if (fscanf(file, "%d", &(*arr)[i]) != 1) {
            free(*arr);
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    return 1;
}

int save_deque_to_file(const char* filename, Deque* deque) {
    if (!deque) return 0;
    
    int* arr = deque_to_array(deque);
    if (!arr) return 0;
    
    int result = save_array_to_file(filename, arr, get_size(deque));
    free(arr);
    
    return result;
}

Deque* load_deque_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла для чтения");
        return NULL;
    }
    
    Deque* deque = create_deque();
    if (!deque) {
        fclose(file);
        return NULL;
    }
    
    int value;
    while (fscanf(file, "%d", &value) == 1) {
        push_rear(deque, value);
    }
    
    fclose(file);
    
    if (is_empty(deque)) {
        delete_deque(deque);
        return NULL;
    }
    
    return deque;
}

void generate_test_file(const char* filename, int size, int min_val, int max_val) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Ошибка создания тестового файла");
        return;
    }
    
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        int num = min_val + rand() % (max_val - min_val + 1);
        fprintf(file, "%d ", num);
    }
    
    fclose(file);
    printf("Создан тестовый файл: %s (размер: %d)\n", filename, size);
}

// Функция для безопасного создания директории
static void safe_mkdir_tests(void) {
    int result;
    result = system("mkdir -p tests 2>/dev/null");

    (void)result; // Игнорируем возвращаемое значение
}

void generate_multiple_test_files(int base_count) {
    // Создаем папку tests
    safe_mkdir_tests();
    
    int sizes[] = {100, 500, 1000, 5000, 10000, 50000, 100000};
    int count = sizeof(sizes) / sizeof(sizes[0]);
    
    if (base_count < count) count = base_count;
    
    for (int i = 0; i < count; i++) {
        char filename[256];
        sprintf(filename, "tests/test_%d.txt", sizes[i]);
        generate_test_file(filename, sizes[i], -10000, 10000);
    }
}