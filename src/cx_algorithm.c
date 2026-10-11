
#include "cx_algorithm.h"

const void* cx_find(
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