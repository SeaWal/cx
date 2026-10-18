
#include "cx_algorithm.h"

const void* cx_find(
    cx_iter* iter,
    bool (*equals)(const void* element, const void* target),
    const void* target
)
{
    if (iter == NULL || equals == NULL) {
        return NULL;
    }

    while (cx_iter_next(iter)) {
        const void* element = cx_iter_get(iter);

        if (element != NULL && equals(element, target)) {
            return element;
        }
    }

    return NULL;
}

const void* cx_find_if(
    cx_iter* iter,
    bool (*predicate)(const void* element, void* context),
    void* context
)
{
    if (iter == NULL || predicate == NULL) {
        return NULL;
    }

    while (cx_iter_next(iter)) {
        const void* element = cx_iter_get(iter);

        if (element != NULL && predicate(element, context)) {
            return element;
        }
    }

    return NULL;
}


size_t cx_count(
    cx_iter* iter,
    bool (*equals)(const void* element, const void* target),
    const void* target
) {
    if (iter == NULL || equals == NULL) {
        return 0;
    }

    size_t count = 0;

    while (cx_iter_next(iter)) {
        const void* element = cx_iter_get(iter);

        if (element != NULL && equals(element, target)) {
            count++;
        }
    }

    return count;
}

size_t cx_count_if(
    cx_iter* iter,
    bool (*predicate)(const void* element, void* context),
    void* context
) {
    if (iter == NULL || predicate == NULL) {
        return 0;
    }

    size_t count = 0;

    while (cx_iter_next(iter)) {
        const void* element = cx_iter_get(iter);

        if (element != NULL && predicate(element, context)) {
            count++;
        }
    }

    return count;
}

bool cx_any(
    cx_iter* iter,
    bool (*predicate)(const void* element, void* context),
    void* context
) {
    if (iter == NULL || predicate == NULL) {
        return false;
    }

    while (cx_iter_next(iter)) {
        const void* element = cx_iter_get(iter);

        if (element != NULL && predicate(element, context)) {
            return true;
        }
    }

    return false;
}

bool cx_all(
    cx_iter* iter,
    bool (*predicate)(const void* element, void* context),
    void* context
) {
    if (iter == NULL || predicate == NULL) {
        return false;
    }

    while (cx_iter_next(iter)) {
        const void* element = cx_iter_get(iter);

        if (element == NULL || !predicate(element, context)) {
            return false;
        }
    }

    return true;
}

void cx_for_each(
    cx_iter* iter,
    void (*func)(const void* element, void* context),
    void* context
) {
    if (iter == NULL || func == NULL) {
        return;
    }

    while (cx_iter_next(iter)) {
        const void* element = cx_iter_get(iter);

        if (element != NULL) {
            func(element, context);
        }
    }
}

void cx_accumulate(
    cx_iter* iter,
    void* accumulator,
    void (*accumulate)(
        void* accumulator,
        const void* element,
        void* context
    ),
    void* context
) {
    if (iter == NULL || accumulator == NULL || accumulate == NULL) {
        return;
    }

    while (cx_iter_next(iter)) {
        const void* element = cx_iter_get(iter);

        if (element != NULL) {
            accumulate(accumulator, element, context);
        }
    }
}
