#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

// ================= ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ =================

static void heapify_deque(Deque* deque, int n, int i) {
    if (!deque || n <= 0 || i < 0 || i >= n) return;
    
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    Node* largest_node = get_node_at_index(deque, largest);
    if (!largest_node) return;
    
    // Сравниваем с левым потомком
    if (left < n) {
        Node* left_node = get_node_at_index(deque, left);
        if (left_node && left_node->data > largest_node->data) {
            largest = left;
            largest_node = left_node;
        }
    }
    
    largest_node = get_node_at_index(deque, largest);
    if (!largest_node) return;
    
    // Сравниваем с правым потомком
    if (right < n) {
        Node* right_node = get_node_at_index(deque, right);
        if (right_node && right_node->data > largest_node->data) {
            largest = right;
        }
    }
    
    // Если наибольший не корень
    if (largest != i) {
        Node* i_node = get_node_at_index(deque, i);
        if (i_node) {
            // ИСПОЛЬЗУЕМ ОБМЕН УЗЛАМИ вместо обмена значений
            swap_nodes_complete(deque, i_node, get_node_at_index(deque, largest));
            heapify_deque(deque, n, largest);
        }
    }
}

// ================= СОРТИРОВКА ПУЗЫРЬКОМ С ОБМЕНОМ УЗЛОВ =================

void bubble_sort_deque(Deque* deque) {
    if (!deque || deque->size < 2) return;
    
    int swapped;
    int n = deque->size;
    
    do {
        swapped = 0;
        Node* current = deque->front;
        
        for (int i = 0; i < n - 1; i++) {
            if (!current || !current->next) break;
            
            if (current->data > current->next->data) {
                // ИСПОЛЬЗУЕМ ОБМЕН УЗЛАМИ вместо обмена значений
                swap_nodes_complete(deque, current, current->next);
                swapped = 1;
                
                // После обмена current и next поменялись местами
                // current теперь указывает на узел, который был next
                // Но нам нужно продолжать с того же места
                // Правильнее будет получить узел заново
                current = get_node_at_index(deque, i);
                if (!current) break;
            }
            
            current = current->next;
            if (!current) break;
        }
        n--;
        
    } while (swapped);
}

// ================= ПИРАМИДАЛЬНАЯ СОРТИРОВКА С ОБМЕНОМ УЗЛОВ =================

void heap_sort_deque(Deque* deque) {
    if (!deque || deque->size < 2) return;
    
    int n = deque->size;
    
    // Построение max-heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify_deque(deque, n, i);
    }
    
    // Извлечение элементов из кучи
    for (int i = n - 1; i > 0; i--) {
        Node* first = get_node_at_index(deque, 0);
        Node* last = get_node_at_index(deque, i);
        
        if (first && last) {
            // ИСПОЛЬЗУЕМ ОБМЕН УЗЛАМИ вместо обмена значений
            swap_nodes_complete(deque, first, last);
            heapify_deque(deque, i, 0);
        }
    }
}

// ================= ИЗМЕРЕНИЕ ВРЕМЕНИ =================

double measure_bubble_sort_time(Deque* deque) {
    if (!deque || deque->size < 2) return 0.0;
    
    Deque* copy = copy_deque(deque);
    if (!copy) return 0.0;
    
    clock_t start = clock();
    bubble_sort_deque(copy);
    clock_t end = clock();
    
    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
    delete_deque(copy);
    
    return time_spent;
}

double measure_heap_sort_time(Deque* deque) {
    if (!deque || deque->size < 2) return 0.0;
    
    Deque* copy = copy_deque(deque);
    if (!copy) return 0.0;
    
    clock_t start = clock();
    heap_sort_deque(copy);
    clock_t end = clock();
    
    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
    delete_deque(copy);
    
    return time_spent;
}