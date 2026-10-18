
#include "cx_algorithm.h"
#include "cx_allocator.h"
#include "cx_slice.h"

static bool equals_int(const void* element, const void* target) {
    return *(const int*)element == *(const int*)target;
}

static bool is_even(const void* element, void* context) {
    (void)context;
    return *(const int*)element % 2 == 0;
}

static bool greater_than(const void* element, void* context) {
    const int* value = element;
    const int* threshold = context;

    return *value > *threshold;
}

/* CX_FIND TESTS*/

static int test_find_existing_value(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {10, 20, 30, 40};
    int target = 30;

    cx_slice slice = cx_slice_create(values, 4, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    const int* found = cx_find(&iter, equals_int, &target);

    int result = found != NULL && *found == 30 ? 0 : 1;
    cx_iter_destroy(&iter);

    return result;
}

static int test_find_first_value(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {10, 20, 30};
    int target = 10;

    cx_slice slice = cx_slice_create(values, 3, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    const int* found = cx_find(&iter, equals_int, &target);

    int result = found != NULL && *found == 10 ? 0 : 1;
    cx_iter_destroy(&iter);

    return result;
}

static int test_find_last_value(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {10, 20, 30};
    int target = 30;

    cx_slice slice = cx_slice_create(values, 3, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    const int* found = cx_find(&iter, equals_int, &target);

    int result = found != NULL && *found == 30 ? 0 : 1;
    cx_iter_destroy(&iter);

    return result;
}

static int test_find_missing_value(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {10, 20, 30};
    int target = 99;

    cx_slice slice = cx_slice_create(values, 3, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    const void* found = cx_find(&iter, equals_int, &target);

    cx_iter_destroy(&iter);

    return found == NULL ? 0 : 1;
}

static int test_find_empty_slice(void) {
    cx_allocator allocator = cx_general_allocator();
    cx_slice slice = cx_slice_create(NULL, 0, sizeof(int));
    int target = 10;

    cx_iter iter = cx_slice_iter(&slice, &allocator);
    const void* found = cx_find(&iter, equals_int, &target);

    cx_iter_destroy(&iter);

    return found == NULL ? 0 : 1;
}

static int test_find_null_callback(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {10, 20};

    cx_slice slice = cx_slice_create(values, 2, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    const void* found = cx_find(&iter, NULL, NULL);

    cx_iter_destroy(&iter);

    return found == NULL ? 0 : 1;
}

/* CX_FIND_IF TESTS */

static int test_find_if_predicate(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {1, 3, 4, 6};
    cx_slice slice = cx_slice_create(values, 4, sizeof(int));

    cx_iter iter = cx_slice_iter(&slice, &allocator);
    const int* found = cx_find_if(&iter, is_even, NULL);

    int result = found != NULL && *found == 4 ? 0 : 1;
    cx_iter_destroy(&iter);

    return result;
}

static int test_find_if_with_context(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {10, 20, 30, 40};
    int threshold = 25;

    cx_slice slice = cx_slice_create(values, 4, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    const int* found = cx_find_if(
        &iter,
        greater_than,
        &threshold
    );

    int result = found != NULL && *found == 30 ? 0 : 1;
    cx_iter_destroy(&iter);

    return result;
}

static int test_find_if_missing(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {1, 3, 5};
    cx_slice slice = cx_slice_create(values, 3, sizeof(int));

    cx_iter iter = cx_slice_iter(&slice, &allocator);
    const void* found = cx_find_if(&iter, is_even, NULL);

    cx_iter_destroy(&iter);

    return found == NULL ? 0 : 1;
}

/* TEST COUNT */
static int test_count_values(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {10, 20, 10, 30, 10};
    int target = 10;

    cx_slice slice = cx_slice_create(values, 5, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    size_t count = cx_count(&iter, equals_int, &target);

    cx_iter_destroy(&iter);

    return count == 3 ? 0 : 1;
}

/* TEST COUNT_IF */
static int test_count_if_values(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {1, 2, 3, 4, 5, 6};

    cx_slice slice = cx_slice_create(values, 6, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    size_t count = cx_count_if(&iter, is_even, NULL);

    cx_iter_destroy(&iter);

    return count == 3 ? 0 : 1;
}

static int test_count_if_with_context(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {10, 20, 30, 40};
    int threshold = 25;

    cx_slice slice = cx_slice_create(values, 4, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    size_t count = cx_count_if(
        &iter,
        greater_than,
        &threshold
    );

    cx_iter_destroy(&iter);

    return count == 2 ? 0 : 1;
}

/* TEST ANY */
static int test_any_of_match(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {1, 3, 4, 5};

    cx_slice slice = cx_slice_create(values, 4, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    bool result = cx_any(&iter, is_even, NULL);

    cx_iter_destroy(&iter);

    return result ? 0 : 1;
}

static int test_any_of_no_match(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {1, 3, 5};

    cx_slice slice = cx_slice_create(values, 3, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    bool result = cx_any(&iter, is_even, NULL);

    cx_iter_destroy(&iter);

    return result ? 1 : 0;
}

/* TEST ALL */
static int test_all_of_match(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {2, 4, 6};

    cx_slice slice = cx_slice_create(values, 3, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    bool result = cx_all(&iter, is_even, NULL);

    cx_iter_destroy(&iter);

    return result ? 0 : 1;
}

static int test_all_of_no_match(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {2, 3, 6};

    cx_slice slice = cx_slice_create(values, 3, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    bool result = cx_all(&iter, is_even, NULL);

    cx_iter_destroy(&iter);

    return result ? 1 : 0;
}

/* TEST FOR_EACH */
static void sum_int(const void* element, void* context) {
    const int* value = element;
    int* sum = context;

    *sum += *value;
}

static int test_for_each(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {1, 2, 3, 4};
    int sum = 0;

    cx_slice slice = cx_slice_create(values, 4, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    cx_for_each(&iter, sum_int, &sum);

    cx_iter_destroy(&iter);

    return sum == 10 ? 0 : 1;
}

/* TEST ACCUMULATE */
static void sum_accumulate(
    void* initial,
    const void* element,
    void* context
) {
    (void)context;

    int* sum = initial;
    const int* value = element;

    *sum += *value;
}

static int test_accumulate_sum(void) {
    cx_allocator allocator = cx_general_allocator();
    int values[] = {1, 2, 3, 4};
    int sum = 0;

    cx_slice slice = cx_slice_create(values, 4, sizeof(int));
    cx_iter iter = cx_slice_iter(&slice, &allocator);

    cx_accumulate(&iter, &sum, sum_accumulate, NULL);

    cx_iter_destroy(&iter);

    return sum == 10 ? 0 : 1;
}

int main(void) {
    if (test_find_existing_value() != 0) return 1;
    if (test_find_first_value() != 0) return 1;
    if (test_find_last_value() != 0) return 1;
    if (test_find_missing_value() != 0) return 1;
    if (test_find_empty_slice() != 0) return 1;
    if (test_find_null_callback() != 0) return 1;

    if (test_find_if_predicate() != 0) return 1;
    if (test_find_if_with_context() != 0) return 1;
    if (test_find_if_missing() != 0) return 1;

    if(test_count_values() != 0) return 1;

    if(test_count_if_values() !=0) return 1;
    if(test_count_if_with_context() != 0) return 1;

    if(test_any_of_match() != 0) return 1;
    if(test_any_of_no_match() != 0) return 1;

    if(test_all_of_match() != 0) return 1;
    if(test_all_of_no_match() !=0) return 1;

    if(test_for_each() != 0) return 1;

    if(test_accumulate_sum() != 0) return 1;
    
    return 0;
}
