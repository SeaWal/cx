#include "cx_iter.h"
#include "cx_vector.h"

/* VECTOR ITERATOR */
static int test_vector_iterator(void) {
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(&allocator, int);

    if (vector == NULL) {
        return 1;
    }

    cx_vector_push(vector, int, 10);
    cx_vector_push(vector, int, 20);
    cx_vector_push(vector, int, 30);

    cx_iter iter = cx_vector_iter(vector);

    if (!cx_iter_next(&iter)) {
        cx_iter_destroy(&iter);
        cx_vector_destroy(vector);
        return 1;
    }

    const int* value = cx_iter_get(&iter);

    if (value == NULL || *value != 10) {
        cx_iter_destroy(&iter);
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_iter_next(&iter)) {
        cx_iter_destroy(&iter);
        cx_vector_destroy(vector);
        return 1;
    }

    value = cx_iter_get(&iter);

    if (value == NULL || *value != 20) {
        cx_iter_destroy(&iter);
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_iter_next(&iter)) {
        cx_iter_destroy(&iter);
        cx_vector_destroy(vector);
        return 1;
    }

    value = cx_iter_get(&iter);

    if (value == NULL || *value != 30) {
        cx_iter_destroy(&iter);
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_iter_next(&iter)) {
        cx_iter_destroy(&iter);
        cx_vector_destroy(vector);
        return 1;
    }

    cx_iter_destroy(&iter);
    cx_vector_destroy(vector);

    return 0;
}

static int test_empty_vector_iterator(void) {
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(&allocator, int);

    if (vector == NULL) {
        return 1;
    }

    cx_iter iter = cx_vector_iter(vector);

    if (cx_iter_next(&iter)) {
        cx_iter_destroy(&iter);
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_iter_get(&iter) != NULL) {
        cx_iter_destroy(&iter);
        cx_vector_destroy(vector);
        return 1;
    }

    cx_iter_destroy(&iter);
    cx_vector_destroy(vector);

    return 0;
}

static int test_null_iterator(void) {
    if (cx_iter_next(NULL)) {
        return 1;
    }

    if (cx_iter_get(NULL) != NULL) {
        return 1;
    }

    cx_iter_destroy(NULL);

    return 0;
}

static int test_null_vector_iterator(void) {
    cx_iter iter = cx_vector_iter(NULL);

    if (cx_iter_next(&iter)) {
        cx_iter_destroy(&iter);
        return 1;
    }

    if (cx_iter_get(&iter) != NULL) {
        cx_iter_destroy(&iter);
        return 1;
    }

    cx_iter_destroy(&iter);

    return 0;
}

/* SLICE ITERATOR */
static int test_slice_iterator(void) {
    cx_allocator allocator = cx_general_allocator();

    int values[] = {10, 20, 30};
    cx_slice slice = cx_slice_create(values, 3, sizeof(int));

    cx_iter iter = cx_slice_iter(&slice, &allocator);

    const int expected[] = {10, 20, 30};

    for (size_t i = 0; i < 3; i++) {
        if (!cx_iter_next(&iter)) {
            cx_iter_destroy(&iter);
            return 1;
        }

        const int* value = cx_iter_get(&iter);

        if (value == NULL || *value != expected[i]) {
            cx_iter_destroy(&iter);
            return 1;
        }
    }

    if (cx_iter_next(&iter)) {
        cx_iter_destroy(&iter);
        return 1;
    }

    if (cx_iter_get(&iter) != NULL) {
        cx_iter_destroy(&iter);
        return 1;
    }

    cx_iter_destroy(&iter);

    return 0;
}

static int test_empty_slice_iterator(void) {
    cx_allocator allocator = cx_general_allocator();

    cx_slice slice = cx_slice_create(NULL, 0, sizeof(int));

    cx_iter iter = cx_slice_iter(&slice, &allocator);

    if (cx_iter_next(&iter)) {
        cx_iter_destroy(&iter);
        return 1;
    }

    if (cx_iter_get(&iter) != NULL) {
        cx_iter_destroy(&iter);
        return 1;
    }

    cx_iter_destroy(&iter);

    return 0;
}

int main(void) {
    if(test_vector_iterator() != 0) return 1;
    if(test_empty_vector_iterator() != 0) return 1;
    if(test_null_iterator() != 0) return 1;
    if(test_null_vector_iterator() != 0) return 1;
    if(test_slice_iterator() != 0) return 1;
    if(test_empty_slice_iterator() != 0) return 1;
}
