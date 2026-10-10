#include <stdint.h>

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

typedef struct {
    const cx_slice* slice;
    cx_allocator* allocator;
    size_t index;
    bool started;
} cx_slice_iter_ctx;

static bool cx_slice_iter_next(cx_iter* iter) {
    cx_slice_iter_ctx* ctx = iter->ctx;

    if (ctx == NULL || ctx->slice == NULL) {
        return false;
    }

    if (!ctx->started) {
        ctx->started = true;

        return ctx->index < ctx->slice->length;
    }

    // Avoid advancing past the final element or wrapping the index.
    if (ctx->index >= ctx->slice->length ||
        ctx->index == SIZE_MAX ||
        ctx->index + 1 >= ctx->slice->length) {
        ctx->index = ctx->slice->length;
        return false;
    }

    ctx->index++;

    return true;
}

static const void* cx_slice_iter_get(const cx_iter* iter) {
    const cx_slice_iter_ctx* ctx = iter->ctx;

    if (ctx == NULL ||
        ctx->slice == NULL ||
        ctx->index >= ctx->slice->length ||
        ctx->slice->data == NULL ||
        ctx->slice->elem_size == 0) {
        return NULL;
    }

    size_t offset;

    if (!cx_checked_mul(ctx->index, ctx->slice->elem_size, &offset)) {
        return NULL;
    }

    return ctx->slice->data + offset;
}

static void cx_slice_iter_destroy(cx_iter* iter) {
    cx_slice_iter_ctx* ctx = iter->ctx;

    if (ctx == NULL) {
        return;
    }

    cx_dealloc(ctx->allocator, ctx, sizeof(*ctx), _Alignof(cx_slice_iter_ctx));

    iter->ctx = NULL;
}

cx_iter cx_slice_iter(const cx_slice* slice, cx_allocator* allocator) {
    if (slice == NULL ||
        allocator == NULL ||
        slice->elem_size == 0 ||
        (slice->length > 0 && slice->data == NULL)) {
        return (cx_iter){0};
    }

    cx_slice_iter_ctx* ctx = cx_alloc(allocator, sizeof(*ctx), _Alignof(cx_slice_iter_ctx));

    if (ctx == NULL) {
        return (cx_iter){0};
    }

    *ctx = (cx_slice_iter_ctx) {
        .slice = slice,
        .allocator = allocator,
        .index = 0,
        .started = false,
    };

    return (cx_iter) {
        .ctx = ctx,
        .next = cx_slice_iter_next,
        .get = cx_slice_iter_get,
        .destroy = cx_slice_iter_destroy,
    };
}
