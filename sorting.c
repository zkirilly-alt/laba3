#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

static void heapify_deque(Deque *deque, int n, int start_i)
{
    if (!deque || n <= 0 || start_i < 0 || start_i >= n)
        return;

    int i = start_i;
    while (1)
    {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        Node *node_i = get_node_at_index(deque, i);
        if (!node_i)
            break;

        if (left < n)
        {
            Node *node_left = get_node_at_index(deque, left);
            if (node_left && node_left->data > node_i->data)
            {
                largest = left;
            }
        }

        if (right < n)
        {
            Node *node_right = get_node_at_index(deque, right);
            Node *node_largest = get_node_at_index(deque, largest);
            if (node_right && node_largest && node_right->data > node_largest->data)
            {
                largest = right;
            }
        }

        if (largest == i)
            break;

        Node *final_i = get_node_at_index(deque, i);
        Node *final_largest = get_node_at_index(deque, largest);
        if (final_i && final_largest)
        {
            swap_nodes_complete(deque, final_i, final_largest);
        }
        else
        {
            break;
        }

        i = largest;
    }
}

void bubble_sort_deque(Deque *deque)
{
    if (!deque || deque->size < 2)
        return;

    int n = deque->size;
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            Node *a = get_node_at_index(deque, j);
            Node *b = get_node_at_index(deque, j + 1);
            if (a && b && a->data > b->data) {
                swap_nodes_complete(deque, a, b);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

void heap_sort_deque(Deque *deque)
{
    if (!deque || deque->size < 2)
        return;

    int n = deque->size;

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify_deque(deque, n, i);
    }

    for (int i = n - 1; i > 0; i--)
    {
        Node *first = get_node_at_index(deque, 0);
        Node *last = get_node_at_index(deque, i);
        if (first && last)
        {
            swap_nodes_complete(deque, first, last);
            heapify_deque(deque, i, 0);
        }
    }
}

double measure_bubble_sort_time(Deque *deque)
{
    if (!deque || deque->size < 2)
        return 0.0;

    Deque *copy = copy_deque(deque);
    if (!copy)
        return 0.0;

    clock_t start = clock();
    bubble_sort_deque(copy);
    clock_t end = clock();

    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
    delete_deque(copy);

    return time_spent;
}

double measure_heap_sort_time(Deque *deque)
{
    if (!deque || deque->size < 2)
        return 0.0;

    Deque *copy = copy_deque(deque);
    if (!copy)
        return 0.0;

    clock_t start = clock();
    heap_sort_deque(copy);
    clock_t end = clock();

    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC;
    delete_deque(copy);

    return time_spent;
}