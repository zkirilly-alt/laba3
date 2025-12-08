#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

// ================= ОБЪЯВЛЕНИЯ ВСПОМОГАТЕЛЬНЫХ ФУНКЦИЙ =================

// Объявляем статические функции ДО их использования
static void swap_adjacent_nodes(Deque* deque, Node* first, Node* second);
static void swap_distant_nodes(Deque* deque, Node* a, Node* b);

// ================= ОСНОВНЫЕ ФУНКЦИИ =================

Deque* create_deque() {
    Deque* deque = (Deque*)malloc(sizeof(Deque));
    if (!deque) {
        fprintf(stderr, "Ошибка выделения памяти для дека\n");
        exit(1);
    }
    deque->front = deque->rear = NULL;
    deque->size = 0;
    return deque;
}

void delete_deque(Deque* deque) {
    if (!deque) return;
    
    Node* current = deque->front;
    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(deque);
}

void push_front(Deque* deque, int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Ошибка выделения памяти для узла\n");
        exit(1);
    }
    
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = deque->front;
    
    if (is_empty(deque)) {
        deque->rear = new_node;
    } else {
        deque->front->prev = new_node;
    }
    
    deque->front = new_node;
    deque->size++;
}

void push_rear(Deque* deque, int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Ошибка выделения памяти для узла\n");
        exit(1);
    }
    
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = deque->rear;
    
    if (is_empty(deque)) {
        deque->front = new_node;
    } else {
        deque->rear->next = new_node;
    }
    
    deque->rear = new_node;
    deque->size++;
}

int pop_front(Deque* deque) {
    if (is_empty(deque)) {
        fprintf(stderr, "Дек пуст!\n");
        exit(1);
    }
    
    Node* temp = deque->front;
    int data = temp->data;
    
    deque->front = deque->front->next;
    if (deque->front) {
        deque->front->prev = NULL;
    } else {
        deque->rear = NULL;
    }
    
    free(temp);
    deque->size--;
    return data;
}

int pop_rear(Deque* deque) {
    if (is_empty(deque)) {
        fprintf(stderr, "Дек пуст!\n");
        exit(1);
    }
    
    Node* temp = deque->rear;
    int data = temp->data;
    
    deque->rear = deque->rear->prev;
    if (deque->rear) {
        deque->rear->next = NULL;
    } else {
        deque->front = NULL;
    }
    
    free(temp);
    deque->size--;
    return data;
}

int is_empty(Deque* deque) {
    return deque->size == 0;
}

int get_size(Deque* deque) {
    return deque->size;
}

Deque* array_to_deque(int* arr, int size) {
    Deque* deque = create_deque();
    for (int i = 0; i < size; i++) {
        push_rear(deque, arr[i]);
    }
    return deque;
}

int* deque_to_array(Deque* deque) {
    if (!deque || is_empty(deque)) return NULL;
    
    int* arr = (int*)malloc(deque->size * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Ошибка выделения памяти для массива\n");
        exit(1);
    }
    
    Node* current = deque->front;
    for (int i = 0; i < deque->size; i++) {
        arr[i] = current->data;
        current = current->next;
    }
    
    return arr;
}

// ================= ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ =================

// Получение узла по индексу
Node* get_node_at_index(Deque* deque, int index) {
    if (!deque || index < 0 || index >= deque->size) {
        return NULL;
    }
    
    // Оптимизация: идем с начала или с конца
    if (index < deque->size / 2) {
        Node* current = deque->front;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current;
    } else {
        Node* current = deque->rear;
        for (int i = deque->size - 1; i > index; i--) {
            current = current->prev;
        }
        return current;
    }
}


// Обмен соседних узлов
static void swap_adjacent_nodes(Deque* deque, Node* first, Node* second) {
    // first -> second
    Node* first_prev = first->prev;
    Node* second_next = second->next;
    
    // Обновляем внешние связи
    if (first_prev) {
        first_prev->next = second;
    } else {
        deque->front = second; // first был front
    }
    
    if (second_next) {
        second_next->prev = first;
    } else {
        deque->rear = first; // second был rear
    }
    
    // Обновляем связи между узлами
    second->prev = first_prev;
    first->next = second_next;
    
    first->prev = second;
    second->next = first;
}

// Обмен удаленных узлов
static void swap_distant_nodes(Deque* deque, Node* a, Node* b) {
    Node* a_prev = a->prev;
    Node* a_next = a->next;
    Node* b_prev = b->prev;
    Node* b_next = b->next;
    
    // Обновляем связи для узла A
    if (a_prev) {
        a_prev->next = b;
    } else {
        deque->front = b; // a был front
    }
    
    if (a_next) {
        a_next->prev = b;
    }
    
    // Обновляем связи для узла B
    if (b_prev) {
        b_prev->next = a;
    } else {
        deque->front = a; // b был front
    }
    
    if (b_next) {
        b_next->prev = a;
    } else {
        deque->rear = a; // b был rear
    }
    
    // Обновляем связи самих узлов
    a->prev = b_prev;
    a->next = b_next;
    b->prev = a_prev;
    b->next = a_next;
}

// Полный обмен узлами
void swap_nodes_complete(Deque* deque, Node* a, Node* b) {
    if (!deque || !a || !b || a == b) {
        return;
    }
    
    // Если узлы соседние, обрабатываем особо
    if (a->next == b) {
        // a и b идут подряд: a -> b
        swap_adjacent_nodes(deque, a, b);
    } else if (b->next == a) {
        // b и a идут подряд: b -> a
        swap_adjacent_nodes(deque, b, a);
    } else {
        // Узлы не соседние
        swap_distant_nodes(deque, a, b);
    }
}

void print_deque(Deque* deque) {
    if (!deque || is_empty(deque)) {
        printf("Дек пуст\n");
        return;
    }
    
    Node* current = deque->front;
    int count = 0;
    while (current != NULL) {
        printf("%d ", current->data);
        if (++count % 10 == 0) printf("\n");
        current = current->next;
    }
    printf("\n");
}