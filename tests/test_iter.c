#include "cx_iter.h"
#include "cx_vector.h"

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

int main(void) {
    if(test_vector_iterator() != 0) return 1;
}
