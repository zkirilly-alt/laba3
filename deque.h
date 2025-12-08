#ifndef DEQUE_H
#define DEQUE_H

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct Deque {
    Node* front;
    Node* rear;
    int size;
} Deque;

// Создание и удаление дека
Deque* create_deque();
void delete_deque(Deque* deque);

// Основные операции
void push_front(Deque* deque, int data);
void push_rear(Deque* deque, int data);
int pop_front(Deque* deque);
int pop_rear(Deque* deque);
int is_empty(Deque* deque);
int get_size(Deque* deque);

// Преобразования
Deque* array_to_deque(int* arr, int size);
int* deque_to_array(Deque* deque);

// Вспомогательные функции
void swap_nodes(Node* a, Node* b);          // Обмен значениями (старая)
void swap_nodes_complete(Deque* deque, Node* a, Node* b); // НОВАЯ: обмен узлами
void print_deque(Deque* deque);
Node* get_node_at_index(Deque* deque, int index); // Делаем публичной для тестов

#endif