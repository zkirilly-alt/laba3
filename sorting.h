#ifndef SORTING_H
#define SORTING_H

#include "deque.h"

void bubble_sort_deque(Deque* deque);

void heap_sort_deque(Deque* deque);

double measure_bubble_sort_time(Deque* deque);
double measure_heap_sort_time(Deque* deque);

#endif