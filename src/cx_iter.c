#include "cx_iter.h"

bool cx_iter_next(cx_iter* iter) {
    if (iter == NULL || iter->next == NULL) {
        return false;
    }

    return iter->next(iter);
}

const void* cx_iter_get(const cx_iter* iter) {
    if (iter == NULL || iter->get == NULL) {
        return NULL;
    }

    return iter->get(iter);
}