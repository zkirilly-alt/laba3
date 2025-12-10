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

// Вспомогательные функции
void swap_nodes(Node* a, Node* b);
void print_deque(Deque* deque);
Node* get_node_at_index(Deque* deque, int index);
Deque* copy_deque(Deque* deque);  // НОВАЯ: копирование дека

#endif