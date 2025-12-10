#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

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

// Получение узла по индексу
Node* get_node_at_index(Deque* deque, int index) {
    if (!deque || index < 0 || index >= deque->size) {
        return NULL;
    }
    
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

// Обмен значений между узлами
void swap_nodes(Node* a, Node* b) {
    if (!a || !b || a == b) return;
    int temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// Копирование дека
Deque* copy_deque(Deque* deque) {
    if (!deque) return NULL;
    
    Deque* copy = create_deque();
    Node* current = deque->front;
    
    while (current) {
        push_rear(copy, current->data);
        current = current->next;
    }
    
    return copy;
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