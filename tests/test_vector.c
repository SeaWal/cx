#include "cx_vector.h"

#include <stdio.h>

typedef struct test_person {
    int id;
    float score;
} test_person;

static int test_create(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_create: vector creation failed\n");
        return 1;
    }

    if (cx_vector_length(vector) != 0) {
        printf("test_create: expected length 0\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_capacity(vector) != 0) {
        printf("test_create: expected capacity 0\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_vector_is_empty(vector)) {
        printf("test_create: expected vector to be empty\n");
        cx_vector_destroy(vector);
        return 1;
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_push_and_get(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_push_and_get: vector creation failed\n");
        return 1;
    }

    for (int i = 0; i < 100; i++) {
        if (!cx_vector_push(vector, int, i)) {
            printf("test_push_and_get: push failed at %d\n", i);
            cx_vector_destroy(vector);
            return 1;
        }
    }

    if (cx_vector_length(vector) != 100) {
        printf("test_push_and_get: expected length 100\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_capacity(vector) < 100) {
        printf("test_push_and_get: capacity is too small\n");
        cx_vector_destroy(vector);
        return 1;
    }

    for (size_t i = 0; i < 100; i++) {
        int* value = cx_vector_get(vector, i);

        if (value == NULL) {
            printf("test_push_and_get: get failed at %zu\n", i);
            cx_vector_destroy(vector);
            return 1;
        }

        if (*value != (int)i) {
            fprintf(
                stderr,
                "test_push_and_get: expected %zu, got %d\n",
                i,
                *value
            );
            cx_vector_destroy(vector);
            return 1;
        }
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_push_ptr(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_push_ptr: vector creation failed\n");
        return 1;
    }

    int value = 42;

    if (!cx_vector_push_ptr(vector, &value)) {
        printf("test_push_ptr: push failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    const int* result = cx_vector_get_const(vector, 0);

    if (result == NULL || *result != 42) {
        printf("test_push_ptr: incorrect stored value\n");
        cx_vector_destroy(vector);
        return 1;
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_struct_elements(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        test_person
    );

    if (vector == NULL) {
        printf("test_struct_elements: vector creation failed\n");
        return 1;
    }

    test_person people[] = {
        { .id = 1, .score = 91.5f },
        { .id = 2, .score = 87.0f },
        { .id = 3, .score = 99.5f }
    };

    for (size_t i = 0; i < 3; i++) {
        if (!cx_vector_push_ptr(vector, &people[i])) {
            printf("test_struct_elements: push failed\n");
            cx_vector_destroy(vector);
            return 1;
        }
    }

    if (cx_vector_length(vector) != 3) {
        printf("test_struct_elements: expected length 3\n");
        cx_vector_destroy(vector);
        return 1;
    }

    for (size_t i = 0; i < 3; i++) {
        const test_person* person =
            cx_vector_get_const(vector, i);

        if (person == NULL) {
            printf("test_struct_elements: get failed\n");
            cx_vector_destroy(vector);
            return 1;
        }

        if (person->id != people[i].id ||
            person->score != people[i].score) {
            fprintf(
                stderr,
                "test_struct_elements: incorrect element at %zu\n",
                i
            );
            cx_vector_destroy(vector);
            return 1;
        }
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_reserve(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_reserve: vector creation failed\n");
        return 1;
    }

    if (!cx_vector_reserve(vector, 100)) {
        printf("test_reserve: reserve failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_capacity(vector) < 100) {
        printf("test_reserve: capacity is too small\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_length(vector) != 0) {
        printf("test_reserve: reserve changed length\n");
        cx_vector_destroy(vector);
        return 1;
    }

    for (int i = 0; i < 100; i++) {
        if (!cx_vector_push(vector, int, i)) {
            printf("test_reserve: push failed\n");
            cx_vector_destroy(vector);
            return 1;
        }
    }

    if (cx_vector_capacity(vector) < 100) {
        printf("test_reserve: capacity changed incorrectly\n");
        cx_vector_destroy(vector);
        return 1;
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_clear(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_clear: vector creation failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        if (!cx_vector_push(vector, int, i)) {
            printf("test_clear: push failed\n");
            cx_vector_destroy(vector);
            return 1;
        }
    }

    size_t capacity = cx_vector_capacity(vector);

    cx_vector_clear(vector);

    if (!cx_vector_is_empty(vector)) {
        printf("test_clear: vector is not empty\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_length(vector) != 0) {
        printf("test_clear: expected length 0\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_capacity(vector) != capacity) {
        printf("test_clear: clear changed capacity\n");
        cx_vector_destroy(vector);
        return 1;
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_shrink_to_fit(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_shrink_to_fit: vector creation failed\n");
        return 1;
    }

    if (!cx_vector_reserve(vector, 100)) {
        printf("test_shrink_to_fit: reserve failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        if (!cx_vector_push(vector, int, i)) {
            printf("test_shrink_to_fit: push failed\n");
            cx_vector_destroy(vector);
            return 1;
        }
    }

    if (!cx_vector_shrink_to_fit(vector)) {
        printf("test_shrink_to_fit: shrink failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_capacity(vector) != 10) {
        fprintf(
            stderr,
            "test_shrink_to_fit: expected capacity 10, got %zu\n",
            cx_vector_capacity(vector)
        );
        cx_vector_destroy(vector);
        return 1;
    }

    for (size_t i = 0; i < 10; i++) {
        const int* value = cx_vector_get_const(vector, i);

        if (value == NULL || *value != (int)i) {
            fprintf(
                stderr,
                "test_shrink_to_fit: element %zu was corrupted\n",
                i
            );
            cx_vector_destroy(vector);
            return 1;
        }
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_pop(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_pop: vector creation failed\n");
        return 1;
    }

    int value = 10;

    if (cx_vector_pop(vector, &value)) {
        printf("test_pop: pop succeeded on empty vector\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_vector_push(vector, int, 10) ||
        !cx_vector_push(vector, int, 20) ||
        !cx_vector_push(vector, int, 30)) {
        printf("test_pop: push failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_vector_pop(vector, &value)) {
        printf("test_pop: pop failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (value != 30) {
        printf("test_pop: expected 30, got %d\n", value);
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_length(vector) != 2) {
        printf("test_pop: expected length 2\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_vector_pop(vector, NULL)) {
        printf("test_pop: pop with NULL failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_length(vector) != 1) {
        printf("test_pop: expected length 1\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_vector_pop(vector, &value) || value != 10) {
        printf("test_pop: final pop failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_vector_is_empty(vector)) {
        printf("test_pop: vector should be empty\n");
        cx_vector_destroy(vector);
        return 1;
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_remove(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_remove: vector creation failed\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        if (!cx_vector_push(vector, int, i)) {
            printf("test_remove: push failed\n");
            cx_vector_destroy(vector);
            return 1;
        }
    }

    if (cx_vector_remove(vector, 2) == false) {
        printf("test_remove: middle remove failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    int expected[] = { 0, 1, 3, 4 };

    if (cx_vector_length(vector) != 4) {
        printf("test_remove: expected length 4\n");
        cx_vector_destroy(vector);
        return 1;
    }

    for (size_t i = 0; i < 4; i++) {
        const int* value = cx_vector_get_const(vector, i);

        if (value == NULL || *value != expected[i]) {
            fprintf(
                stderr,
                "test_remove: incorrect value at %zu\n",
                i
            );
            cx_vector_destroy(vector);
            return 1;
        }
    }

    if (!cx_vector_remove(vector, 0)) {
        printf("test_remove: first remove failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_vector_remove(vector, cx_vector_length(vector) - 1)) {
        printf("test_remove: last remove failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_remove(vector, 100)) {
        printf("test_remove: out-of-bounds remove succeeded\n");
        cx_vector_destroy(vector);
        return 1;
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_insert(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_insert: vector creation failed\n");
        return 1;
    }

    if (!cx_vector_insert(vector, int, 0, 20)) {
        printf("test_insert: insert into empty vector failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_vector_insert(vector, int, 0, 10)) {
        printf("test_insert: insert at beginning failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_vector_insert(vector, int, 2, 30)) {
        printf("test_insert: insert at end failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (!cx_vector_insert(vector, int, 1, 15)) {
        printf("test_insert: insert in middle failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    int expected[] = { 10, 15, 20, 30 };

    if (cx_vector_length(vector) != 4) {
        printf("test_insert: expected length 4\n");
        cx_vector_destroy(vector);
        return 1;
    }

    for (size_t i = 0; i < 4; i++) {
        const int* value = cx_vector_get_const(vector, i);

        if (value == NULL || *value != expected[i]) {
            fprintf(
                stderr,
                "test_insert: incorrect value at %zu\n",
                i
            );
            cx_vector_destroy(vector);
            return 1;
        }
    }

    if (cx_vector_insert(vector, int, 100, 42)) {
        printf("test_insert: out-of-bounds insert succeeded\n");
        cx_vector_destroy(vector);
        return 1;
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_out_of_bounds(void)
{
    cx_allocator allocator = cx_general_allocator();

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_out_of_bounds: vector creation failed\n");
        return 1;
    }

    if (cx_vector_get(vector, 0) != NULL) {
        printf("test_out_of_bounds: get returned a value\n");
        cx_vector_destroy(vector);
        return 1;
    }

    int value = 42;

    if (!cx_vector_push_ptr(vector, &value)) {
        printf("test_out_of_bounds: push failed\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_get(vector, 1) != NULL) {
        printf("test_out_of_bounds: get succeeded\n");
        cx_vector_destroy(vector);
        return 1;
    }

    if (cx_vector_get_const(vector, 1) != NULL) {
        printf("test_out_of_bounds: const get succeeded\n");
        cx_vector_destroy(vector);
        return 1;
    }

    cx_vector_destroy(vector);
    return 0;
}

static int test_arena(void)
{
    cx_arena arena;

    if (!cx_arena_init(&arena, 4096)) {
        printf("test_arena: arena initialization failed\n");
        return 1;
    }

    cx_allocator allocator = cx_arena_allocator(&arena);

    cx_vector* vector = cx_vector_create(
        &allocator,
        int
    );

    if (vector == NULL) {
        printf("test_arena: vector creation failed\n");
        cx_arena_destroy(&arena);
        return 1;
    }

    for (int i = 0; i < 50; i++) {
        if (!cx_vector_push(vector, int, i)) {
            printf("test_arena: push failed at %d\n", i);
            cx_arena_destroy(&arena);
            return 1;
        }
    }

    if (cx_vector_length(vector) != 50) {
        printf("test_arena: expected length 50\n");
        cx_arena_destroy(&arena);
        return 1;
    }

    for (size_t i = 0; i < 50; i++) {
        const int* value = cx_vector_get_const(vector, i);

        if (value == NULL || *value != (int)i) {
            fprintf(
                stderr,
                "test_arena: incorrect value at %zu\n",
                i
            );
            cx_arena_destroy(&arena);
            return 1;
        }
    }

    cx_arena_destroy(&arena);
    return 0;
}

int main(void)
{
    if (test_create() != 0) return 1;
    if (test_push_and_get() != 0) return 1;
    if (test_push_ptr() != 0) return 1;
    if (test_struct_elements() != 0) return 1;
    if (test_reserve() != 0) return 1;
    if (test_clear() != 0) return 1;
    if (test_shrink_to_fit() != 0) return 1;
    if (test_pop() != 0) return 1;
    if (test_remove() != 0) return 1;
    if (test_insert() != 0) return 1;
    if (test_out_of_bounds() != 0) return 1;
    if (test_arena() != 0) return 1;

    return 0;
}
