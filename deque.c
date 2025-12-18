#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

static void swap_adjacent_nodes(Deque* deque, Node* first, Node* second);
static void swap_distant_nodes(Deque* deque, Node* a, Node* b);


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



static void swap_adjacent_nodes(Deque* deque, Node* first, Node* second) {
    if (!deque || !first || !second || first->next != second) return;
    
    Node* first_prev = first->prev;    
    Node* second_next = second->next;  
    

    

    if (first_prev) {
        first_prev->next = second;    
    } else {
        deque->front = second;        
    }
    
    if (second_next) {
        second_next->prev = first;    
    } else {
        deque->rear = first;         
    }
    

    

    second->prev = first_prev;         
    first->next = second_next;         
    
   
    first->prev = second;             
    second->next = first;              
}

static void swap_distant_nodes(Deque* deque, Node* a, Node* b) {
    if (!deque || !a || !b || a == b) return;
    
    Node* a_prev = a->prev;
    Node* a_next = a->next;
    Node* b_prev = b->prev;
    Node* b_next = b->next;
    
    
    if (a_prev) {
        a_prev->next = b;             
    } else {
        deque->front = b;             
    }
    
    if (a_next) {
        a_next->prev = b;           
    }
    
    
    if (b_prev) {
        b_prev->next = a;            
    } else {
        deque->front = a;            
    }
    
    if (b_next) {
        b_next->prev = a;             
    } else {
        deque->rear = a;           
    }
    
    a->prev = b_prev;                  
    a->next = b_next;                  
    
    b->prev = a_prev;                 
    b->next = a_next;               
    


    if (a->next == a) a->next = b_next;
    if (a->prev == a) a->prev = b_prev; 
    if (b->next == b) b->next = a_next; 
    if (b->prev == b) b->prev = a_prev; 
}

void swap_nodes_complete(Deque* deque, Node* a, Node* b) {
    if (!deque || !a || !b || a == b) {
        return; 
    }
    

    if (a->next == b) {
        swap_adjacent_nodes(deque, a, b);
    } else if (b->next == a) {
        swap_adjacent_nodes(deque, b, a);
    } else {
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