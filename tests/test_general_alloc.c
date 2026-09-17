#include <stdio.h>

#include "cx_allocator.h"

int main(void) {
    cx_allocator al = cx_general_allocator();

    size_t count = 16;
    size_t size = count * sizeof(int);

    // test basic allocation
    int* values = al.alloc(NULL, size, _Alignof(int));

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
            al.dealloc(NULL, values, size, _Alignof(int));
            return 1;
        }
    }

    al.dealloc(NULL, values, size, _Alignof(int));
    printf("PASS: general allocator\n");

    return 0;
}
