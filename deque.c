#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

// ================= ОБЪЯВЛЕНИЯ СТАТИЧЕСКИХ ФУНКЦИЙ =================
// Объявляем ДО их использования
static void swap_adjacent_nodes(Deque* deque, Node* first, Node* second);
static void swap_distant_nodes(Deque* deque, Node* a, Node* b);

// ================= ОСНОВНЫЕ ФУНКЦИИ ДЕКА =================

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

// ================= ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ =================

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


// ================= ФУНКЦИИ ОБМЕНА УЗЛАМИ =================

// Обмен соседних узлов (first находится непосредственно перед second)
static void swap_adjacent_nodes(Deque* deque, Node* first, Node* second) {
    if (!deque || !first || !second || first->next != second) return;
    
    // Сохраняем ссылки на соседей
    Node* first_prev = first->prev;    // Узел перед first (может быть NULL)
    Node* second_next = second->next;  // Узел после second (может быть NULL)
    
    // Обновляем внешние связи (связи с другими узлами дека)
    
    // Если first был первым узлом (front), то теперь second становится первым
    if (first_prev) {
        first_prev->next = second;     // Узел перед first теперь ссылается на second
    } else {
        deque->front = second;         // second становится новым front
    }
    
    // Если second был последним узлом (rear), то теперь first становится последним
    if (second_next) {
        second_next->prev = first;     // Узел после second теперь ссылается на first
    } else {
        deque->rear = first;           // first становится новым rear
    }
    
    // Обновляем связи между самими узлами
    
    // Связываем second с его новыми соседями
    second->prev = first_prev;         // Новый предыдущий для second
    first->next = second_next;         // Новый следующий для first
    
    // Меняем first и second местами
    first->prev = second;              // first теперь идет после second
    second->next = first;              // second теперь идет перед first
}

// Обмен удаленных узлов (узлы не являются соседями)
static void swap_distant_nodes(Deque* deque, Node* a, Node* b) {
    if (!deque || !a || !b || a == b) return;
    
    // Сохраняем все связи обоих узлов
    Node* a_prev = a->prev;
    Node* a_next = a->next;
    Node* b_prev = b->prev;
    Node* b_next = b->next;
    
    // Обновляем связи для узла A
    
    // Если у узла A был предыдущий узел, он теперь должен ссылаться на B
    if (a_prev) {
        a_prev->next = b;             // Старый сосед A слева теперь ссылается на B
    } else {
        deque->front = b;             // A был front, теперь B становится front
    }
    
    // Если у узла A был следующий узел, он теперь должен ссылаться на B
    if (a_next) {
        a_next->prev = b;             // Старый сосед A справа теперь ссылается на B
    }
    // Иначе: если A был rear, это обрабатывается ниже при обновлении B
    
    // Обновляем связи для узла B
    
    // Если у узла B был предыдущий узел, он теперь должен ссылаться на A
    if (b_prev) {
        b_prev->next = a;             // Старый сосед B слева теперь ссылается на A
    } else {
        deque->front = a;             // B был front, теперь A становится front
    }
    
    // Если у узла B был следующий узел, он теперь должен ссылаться на A
    if (b_next) {
        b_next->prev = a;             // Старый сосед B справа теперь ссылается на A
    } else {
        deque->rear = a;              // B был rear, теперь A становится rear
    }
    
    // Обновляем связи самих узлов A и B
    
    // Узел A получает связи узла B
    a->prev = b_prev;                  // A теперь имеет предыдущего соседа B
    a->next = b_next;                  // A теперь имеет следующего соседа B
    
    // Узел B получает связи узла A
    b->prev = a_prev;                  // B теперь имеет предыдущего соседа A
    b->next = a_next;                  // B теперь имеет следующего соседа A
    
    // Проверка и исправление циклических ссылок
    // (если A и B были соседями, но мы все равно вызвали эту функцию)
    if (a->next == a) a->next = b_next;  // Исправляем если A ссылается на себя
    if (a->prev == a) a->prev = b_prev;  // Исправляем если A ссылается на себя
    if (b->next == b) b->next = a_next;  // Исправляем если B ссылается на себя
    if (b->prev == b) b->prev = a_prev;  // Исправляем если B ссылается на себя
}

// Основная функция обмена узлами - определяет тип обмена и вызывает нужную функцию
void swap_nodes_complete(Deque* deque, Node* a, Node* b) {
    if (!deque || !a || !b || a == b) {
        return;  // Некорректные параметры или одинаковые узлы
    }
    
    // Проверяем, являются ли узлы соседями
    if (a->next == b) {
        // A находится непосредственно перед B: A -> B
        swap_adjacent_nodes(deque, a, b);
    } else if (b->next == a) {
        // B находится непосредственно перед A: B -> A
        swap_adjacent_nodes(deque, b, a);
    } else {
        // Узлы не являются соседями
        swap_distant_nodes(deque, a, b);
    }
}

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