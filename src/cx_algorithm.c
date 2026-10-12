
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