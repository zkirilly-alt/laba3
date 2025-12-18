#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file_io.h"

int save_deque_to_file(const char* filename, Deque* deque) {
    if (!deque) return 0;
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Ошибка открытия файла для записи");
        return 0;
    }
    
    Node* current = deque->front;
    while (current) {
        fprintf(file, "%d ", current->data);
        current = current->next;
    }
    
    fclose(file);
    return 1;
}

Deque* load_deque_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла для чтения");
        return NULL;
    }
    
    Deque* deque = create_deque();
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

static void create_tests_dir(void) {
    int result;
    result = system("mkdir -p tests 2>/dev/null");
    (void)result;
}

void generate_multiple_test_files(int base_count) {
    create_tests_dir();
    
    int test_sizes[] = {100, 500, 1000, 2000, 5000}; 
    int count = sizeof(test_sizes) / sizeof(test_sizes[0]);
    
    if (base_count < count) {
        count = base_count;
    }
    
    for (int i = 0; i < count; i++) {
        char filename[256];
        snprintf(filename, sizeof(filename), "tests/test_%d.txt", test_sizes[i]);
        generate_test_file(filename, test_sizes[i], -10000, 10000);
    }
}