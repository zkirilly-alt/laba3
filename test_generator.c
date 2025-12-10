#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deque.h"
#include "file_io.h"
#include "sorting.h"

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
        while (getchar() != '\n');
        
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
            while (getchar() != '\n');
            
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
            
            delete_deque(deque);
            
        } else if (choice == 2) {
            printf("Создание серии тестовых файлов...\n");
            generate_multiple_test_files(7);
            
        } else if (choice == 3) {
            int size;
            printf("Введите размер дека для теста: ");
            if (scanf("%d", &size) != 1) {
                printf("Ошибка ввода размера!\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');
            
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
            clock_t start = clock();
            bubble_sort_deque(bubble_copy);
            clock_t end = clock();
            printf("выполнена за %.6f сек\n", ((double)(end - start)) / CLOCKS_PER_SEC);
            
            // Пирамидальная сортировка
            printf("Пирамидальная сортировка... ");
            start = clock();
            heap_sort_deque(heap_copy);
            end = clock();
            printf("выполнена за %.6f сек\n", ((double)(end - start)) / CLOCKS_PER_SEC);
            
            delete_deque(deque);
            delete_deque(bubble_copy);
            delete_deque(heap_copy);
        }
        
    } while (choice != 4);
    
    return 0;
}