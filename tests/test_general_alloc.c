#include <stdio.h>

#include "cx_allocator.h"

int main(void) {
    cx_allocator al = cx_general_allocator();

    size_t count = 16;
    size_t size = count * sizeof(int);

    // test basic allocation
    int* values = cx_alloc(&al, size, _Alignof(int));

    if(values == NULL) {
        printf("FAIL: allocation returned NULL\n");
        return 1;
    }

    // test allocated memory is actually useable
    for (size_t i = 0; i < count; ++i) {
        values[i] = (int)i;
    }

    for (size_t i = 0; i < count; ++i) {
        if (values[i] != (int)i) {
            printf("FAIL: memory value mismatch at index %zu\n", i);
            cx_dealloc(&al, values, size, _Alignof(int));
            return 1;
        }
    }

    cx_dealloc(&al, values, size, _Alignof(int));

    // test zero size allocation
    void* zero = cx_alloc(&al, 0, _Alignof(int));

    if(zero != NULL) {
        printf("FAIL: zero size allocation did not return NULL\n");
        return 1;
    }

    // test multiple allocations
    int* first = cx_alloc(&al, sizeof(int), _Alignof(int));

    int* second = cx_alloc(&al, sizeof(int), _Alignof(int) );

    if (first == NULL || second == NULL) {
        printf("FAIL: multiple allocations failed\n");

        if (first != NULL) {
            cx_dealloc(&al, first, sizeof(int), _Alignof(int));
        }

        if (second != NULL) {
            cx_dealloc(&al, second, sizeof(int), _Alignof(int));
        }

        return 1;
    }

    if (first == second) {
        printf("FAIL: separate allocations returned the same pointer\n");
        cx_dealloc(&al, first, sizeof(int), _Alignof(int));
        cx_dealloc(&al, second, sizeof(int), _Alignof(int));
        return 1;
    }

    cx_dealloc(&al, first, sizeof(int), _Alignof(int));
    cx_dealloc(&al, second, sizeof(int), _Alignof(int));

    printf("PASS: general allocator\n");

    return 0;
}
