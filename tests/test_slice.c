#include "cx_slice.h"

#include <stdio.h>

static int test_create(void) {
    int values[] = { 10, 20, 30 };

    cx_slice slice = cx_slice_create(values, 3, sizeof(int));

    if (slice.data != (const u8*)values) {
        return 1;
    }

    if (slice.length != 3) {
        return 1;
    }

    if (slice.elem_size != sizeof(int)) {
        return 1;
    }

    return 0;
}

static int test_empty(void) {
    int values[] = { 1, 2, 3 };

    cx_slice empty = cx_slice_create(values, 0, sizeof(int));

    cx_slice non_empty = cx_slice_create(values, 3, sizeof(int));

    if (!cx_slice_is_empty(&empty)) {
        return 1;
    }

    if (cx_slice_is_empty(&non_empty)) {
        return 1;
    }

    return 0;
}

static int test_get(void) {
    int values[] = { 10, 20, 30 };

    cx_slice slice = cx_slice_create(values, 3, sizeof(int));

    const int* first = cx_slice_get(&slice, 0);
    const int* second = cx_slice_get(&slice, 1);
    const int* third = cx_slice_get(&slice, 2);

    if (first == NULL || *first != 10) {
        return 1;
    }

    if (second == NULL || *second != 20) {
        return 1;
    }

    if (third == NULL || *third != 30) {
        return 1;
    }

    if (cx_slice_get(&slice, 3) != NULL) {
        return 1;
    }

    return 0;
}

static int test_subslice(void) {
    int values[] = { 10, 20, 30, 40, 50 };

    cx_slice slice = cx_slice_create(values, 5, sizeof(int));

    cx_slice sub = cx_slice_subslice( &slice, 1, 3);

    if (sub.data != (const u8*)&values[1]) {
        return 1;
    }

    if (sub.length != 3) {
        return 1;
    }

    if (sub.elem_size != sizeof(int)) {
        return 1;
    }

    const int* first = cx_slice_get(&sub, 0);
    const int* last = cx_slice_get(&sub, 2);

    if (first == NULL || *first != 20) {
        return 1;
    }

    if (last == NULL || *last != 40) {
        return 1;
    }

    return 0;
}

static int test_subslice_at_end(void) {
    int values[] = { 10, 20, 30 };

    cx_slice slice = cx_slice_create(values, 3, sizeof(int));

    cx_slice sub = cx_slice_subslice(&slice, 3, 0);

    if (!cx_slice_is_empty(&sub)) {
        return 1;
    }

    if (sub.elem_size != sizeof(int)) {
        return 1;
    }

    return 0;
}

static int test_subslice_out_of_bounds(void) {
    int values[] = { 10, 20, 30 };

    cx_slice slice = cx_slice_create(values, 3, sizeof(int));

    cx_slice sub = cx_slice_subslice(&slice, 2, 2);

    if (!cx_slice_is_empty(&sub)) {
        return 1;
    }

    return 0;
}

static int test_invalid_create(void) {
    int values[] = { 1, 2, 3 };

    cx_slice invalid_size = cx_slice_create(values, 3, 0);

    if (!cx_slice_is_empty(&invalid_size)) {
        return 1;
    }

    cx_slice invalid_data = cx_slice_create(NULL, 3, sizeof(int)
    );

    if (!cx_slice_is_empty(&invalid_data)) {
        return 1;
    }

    return 0;
}

int main(void) {
    if (test_create() != 0) {
        printf("test_create failed\n");
        return 1;
    }

    if (test_empty() != 0) {
        printf("test_empty failed\n");
        return 1;
    }

    if (test_get() != 0) {
        printf("test_get failed\n");
        return 1;
    }

    if (test_subslice() != 0) {
        printf("test_subslice failed\n");
        return 1;
    }

    if (test_subslice_at_end() != 0) {
        printf("test_subslice_at_end failed\n");
        return 1;
    }

    if (test_subslice_out_of_bounds() != 0) {
        printf("test_subslice_out_of_bounds failed\n");
        return 1;
    }

    if (test_invalid_create() != 0) {
        printf("test_invalid_create failed\n");
        return 1;
    }

    return 0;
}
