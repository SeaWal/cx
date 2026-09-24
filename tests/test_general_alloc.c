#include <stdio.h>

#include "cx_allocator.h"
#include "cx_types.h"

static int test_general_alloc(void) {
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

    return 0;
}

static int test_realloc(void) {
    cx_allocator allocator = cx_general_allocator();

    u8* data = cx_realloc(&allocator, NULL, 0, 16, _Alignof(u8));

    if (data == NULL) {
        fprintf(stderr, "test_realloc: initial allocation failed\n");
        return 1;
    }

    for (size_t i = 0; i < 16; i++) {
        data[i] = (u8)i;
    }

    u8* resized = cx_realloc(&allocator, data, 16, 32, _Alignof(u8));

    if (resized == NULL) {
        fprintf(stderr, "test_realloc: grow failed\n");
        cx_dealloc(&allocator, data, 16, _Alignof(u8));
        return 1;
    }

    for (size_t i = 0; i < 16; i++) {
        if (resized[i] != (u8)i) {
            fprintf(
                stderr,
                "test_realloc: data corrupted after grow at %zu\n",
                i
            );

            cx_dealloc(&allocator, resized, 32, _Alignof(u8));
            return 1;
        }
    }

    for (size_t i = 16; i < 32; i++) {
        resized[i] = (u8)i;
    }

    u8* shrunk = cx_realloc(&allocator, resized, 32, 8, _Alignof(u8));

    if (shrunk == NULL) {
        fprintf(stderr, "test_realloc: shrink failed\n");
        return 1;
    }

    for (size_t i = 0; i < 8; i++) {
        if (shrunk[i] != (u8)i) {
            fprintf(
                stderr,
                "test_realloc: data corrupted after shrink at %zu\n",
                i
            );
            cx_dealloc(&allocator, shrunk, 8, _Alignof(u8));
            return 1;
        }
    }

    void* result = cx_realloc(&allocator, shrunk, 8, 0, _Alignof(u8));

    if (result != NULL) {
        fprintf(
            stderr,
            "test_realloc: zero-size realloc returned non-NULL\n"
        );
        return 1;
    }

    return 0;
}

static int test_realloc_null(void) {
    cx_allocator allocator = cx_general_allocator();

    int* value = cx_realloc(&allocator, NULL, 0, sizeof(int), _Alignof(int));

    if (value == NULL) {
        fprintf(
            stderr,
            "test_realloc_null: allocation failed\n"
        );
        return 1;
    }

    *value = 42;

    if (*value != 42) {
        fprintf(
            stderr,
            "test_realloc_null: value was corrupted\n"
        );
        cx_dealloc(&allocator, value, sizeof(int), _Alignof(int));
        return 1;
    }

    cx_dealloc(&allocator, value, sizeof(int), _Alignof(int));

    return 0;
}

int main(void) {
    if(test_general_alloc() != 0) return 1;
    if(test_realloc() != 0) return 1;
    if(test_realloc_null() !=0) return 1;

    printf("PASS: general allocator\n");
}
