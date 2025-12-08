#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deque.h"
#include "file_io.h"
#include "sorting.h"  // ДОБАВЛЕНО!

int main() {
    printf("Генератор тестовых данных для дека\n");
    srand(time(NULL));
    
    int choice;
    do {
        printf("\n1. Сгенерировать один тестовый файл\n");
        printf("2. Сгенерировать серию файлов для сравнения\n");
        printf("3. Сгенерировать дек и сразу протестировать\n");
        printf("4. Выйти\n");
        printf("Выберите опцию: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода!\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); // Очистка буфера
        
        if (choice == 1) {
            int size, min, max;
            char filename[256];
            
            printf("Введите имя файла: ");
            if (scanf("%255s", filename) != 1) {
                printf("Ошибка ввода имени файла!\n");
                continue;
            }
            
            printf("Введите количество чисел: ");
            if (scanf("%d", &size) != 1) {
                printf("Ошибка ввода размера!\n");
                continue;
            }
            
            printf("Введите минимальное значение: ");
            if (scanf("%d", &min) != 1) {
                printf("Ошибка ввода минимального значения!\n");
                continue;
            }
            
            printf("Введите максимальное значение: ");
            if (scanf("%d", &max) != 1) {
                printf("Ошибка ввода максимального значения!\n");
                continue;
            }
            while (getchar() != '\n'); // Очистка буфера
            
            // Создаем дек
            Deque* deque = create_deque();
            for (int i = 0; i < size; i++) {
                push_rear(deque, min + rand() % (max - min + 1));
            }
            
            // Сохраняем дек в файл
            if (save_deque_to_file(filename, deque)) {
                printf("Создан файл: %s с %d элементами\n", filename, size);
            } else {
                printf("Ошибка сохранения файла!\n");
            }
            
            // Показываем первые 10 элементов
            printf("Первые 10 элементов: ");
            Node* current = deque->front;
            for (int i = 0; i < 10 && current; i++) {
                printf("%d ", current->data);
                current = current->next;
            }
            printf("\n");
            
            delete_deque(deque);
            
        } else if (choice == 2) {
            printf("Создание серии тестовых файлов...\n");
            
            // Создаем папку tests
            int mkdir_result;
#ifdef _WIN32
            mkdir_result = system("mkdir tests 2>nul");
#else
            mkdir_result = system("mkdir -p tests 2>/dev/null");
#endif
            (void)mkdir_result; // Игнорируем возвращаемое значение
            
            int sizes[] = {100, 500, 1000, 5000, 10000, 50000};
            int count = sizeof(sizes) / sizeof(sizes[0]);
            
            for (int i = 0; i < count; i++) {
                char filename[256];
                sprintf(filename, "tests/test_%d.txt", sizes[i]);
                
                Deque* deque = create_deque();
                for (int j = 0; j < sizes[i]; j++) {
                    push_rear(deque, rand() % 10000);
                }
                
                if (save_deque_to_file(filename, deque)) {
                    printf("Создан: %s (размер: %d)\n", filename, sizes[i]);
                } else {
                    printf("Ошибка создания: %s\n", filename);
                }
                
                delete_deque(deque);
            }
            
            printf("Создано %d тестовых файлов в папке tests/\n", count);
            
        } else if (choice == 3) {
            int size;
            printf("Введите размер дека для теста: ");
            if (scanf("%d", &size) != 1) {
                printf("Ошибка ввода размера!\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n'); // Очистка буфера
            
            // Создаем дек
            Deque* deque = create_deque();
            for (int i = 0; i < size; i++) {
                push_rear(deque, rand() % 10000);
            }
            
            printf("Создан дек размером %d\n", size);
            printf("Первые 10 элементов: ");
            
            Node* current = deque->front;
            for (int i = 0; i < 10 && current; i++) {
                printf("%d ", current->data);
                current = current->next;
            }
            printf("\n");
            
            // Тестируем сортировки
            printf("\nТестирование сортировок:\n");
            
            // Создаем копии для тестирования
            Deque* bubble_copy = copy_deque(deque);
            Deque* heap_copy = copy_deque(deque);
            
            if (!bubble_copy || !heap_copy) {
                printf("Ошибка копирования дека!\n");
                delete_deque(deque);
                if (bubble_copy) delete_deque(bubble_copy);
                if (heap_copy) delete_deque(heap_copy);
                continue;
            }
            
            // Сортировка пузырьком
            printf("Сортировка пузырьком... ");
            fflush(stdout);
            clock_t start = clock();
            bubble_sort_deque(bubble_copy);
            clock_t end = clock();
            printf("выполнена за %.6f сек\n", ((double)(end - start)) / CLOCKS_PER_SEC);
            
            // Пирамидальная сортировка
            printf("Пирамидальная сортировка... ");
            fflush(stdout);
            start = clock();
            heap_sort_deque(heap_copy);
            end = clock();
            printf("выполнена за %.6f сек\n", ((double)(end - start)) / CLOCKS_PER_SEC);
            
            // Проверяем, что оба отсортированы правильно
            printf("Обе сортировки дали одинаковый результат: ");
            Node* b_node = bubble_copy->front;
            Node* h_node = heap_copy->front;
            int sorted_correctly = 1;
            
            while (b_node && h_node) {
                if (b_node->data != h_node->data) {
                    sorted_correctly = 0;
                    break;
                }
                b_node = b_node->next;
                h_node = h_node->next;
            }
            
            if (sorted_correctly) {
                printf("ДА\n");
            } else {
                printf("НЕТ\n");
            }
            
            // Очистка
            delete_deque(deque);
            delete_deque(bubble_copy);
            delete_deque(heap_copy);
        }
        
    } while (choice != 4);
    
    return 0;
}