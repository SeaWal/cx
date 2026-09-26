#include "cx_slice.h"
#include "utils.h"

cx_slice cx_slice_create(const void* data, size_t length, size_t elem_size) {
    if (elem_size == 0) {
        return (cx_slice) { 0 };
    }

    if (length > 0 && data == NULL) {
        return (cx_slice) { 0 };
    }

    return (cx_slice) {
        .data = (const u8*)data,
        .length = length,
        .elem_size = elem_size,
    };
}

bool cx_slice_is_empty(const cx_slice* slice) {
    return slice == NULL || slice->length == 0;
}

const void* cx_slice_get(const cx_slice* slice, size_t index) {
    if (slice == NULL ||
        index >= slice->length ||
        slice->data == NULL ||
        slice->elem_size == 0) {
        return NULL;
    }

    size_t offset;
    if (!cx_checked_mul(index, slice->elem_size, &offset)) {
        return NULL;
    }

    return slice->data + offset;
}

cx_slice cx_slice_subslice(const cx_slice* slice, size_t start, size_t length) {
    if (slice == NULL ||
        slice->elem_size == 0 ||
        start > slice->length ||
        length > slice->length - start) {
        return (cx_slice) { 0 };
    }

    size_t offset;
    if (!cx_checked_mul(start, slice->elem_size, &offset)) {
        return (cx_slice) { 0 };
    }

    return (cx_slice) {
        .data = slice->data != NULL
            ? slice->data + offset
            : NULL,
        .length = length,
        .elem_size = slice->elem_size,
    };
}