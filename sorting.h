#ifndef SORTING_H
#define SORTING_H

#include "deque.h"

// Сортировка пузырьком на деке
void bubble_sort_deque(Deque* deque);

// Пирамидальная сортировка на деке
void heap_sort_deque(Deque* deque);

// Измерение времени сортировки
double measure_bubble_sort_time(Deque* deque);
double measure_heap_sort_time(Deque* deque);

#endif