#include <stdlib.h>
#include <string.h>

#include "cx_allocator.h"
#include "cx_core.h"
#include "cx_iter.h"
#include "cx_slice.h"
#include "cx_types.h"
#include "cx_vector.h"
#include "utils.h"

#define CX_VECTOR_INITIAL_CAPACITY 8
#define CX_VECTOR_GROWTH_FACTOR 2

struct cx_vector {
    cx_allocator* allocator;
    u8* data;
    size_t length;
    size_t capacity;
    size_t elem_size;
    size_t elem_align;
};

/*
 * Returns a suitable larger capacity for a vector.
 *
 * The capacity is doubled until it can hold the required number of
 * elements. If doubling would overflow, the required capacity is used.
 *
 * @param vector Vector whose current capacity is used as the starting point.
 * @param required Minimum required capacity.
 * @return New capacity.
 */
static size_t cx_vector_grow_capacity(const cx_vector* vector, size_t required) {
    size_t cap = vector->capacity;

    if(cap == 0) {
        cap = CX_VECTOR_INITIAL_CAPACITY;
    }

    while(cap < required) {
        if(cap > SIZE_MAX / CX_VECTOR_GROWTH_FACTOR) {
            return required;
        }

        cap *= CX_VECTOR_GROWTH_FACTOR;
    }

    return cap;
}

/*
 * Resizes the vector's backing storage.
 *
 * @param vector Vector to resize.
 * @param new_capacity New capacity in elements.
 * @return true on success, false on allocation failure or size overflow.
 */
static bool cx_vector_resize_storage(cx_vector* vector, size_t new_capacity) {
    size_t old_size = vector->capacity * vector->elem_size;
    
    size_t new_size;
    if (!cx_checked_mul(new_capacity, vector->elem_size, &new_size)) {
        return false;
    }

    u8* data = cx_realloc(
        vector->allocator,
        vector->data,
        old_size,
        new_size,
        vector->elem_align
    );

    if (data == NULL) {
        return false;
    }

    vector->data = data;
    vector->capacity = new_capacity;

    return true;
}

cx_vector* cx_vector_create_raw(cx_allocator* allocator, size_t element_size, size_t element_align) {
    if(allocator == NULL || element_size == 0 || element_align == 0) {
        return NULL;
    }

    cx_vector* vector = cx_alloc(allocator, sizeof(cx_vector), _Alignof(cx_vector));

    if(vector == NULL) {
        return NULL;
    }

    *vector = (cx_vector) {
        .allocator = allocator,
        .data = NULL,
        .length = 0,
        .capacity = 0,
        .elem_size = element_size,
        .elem_align = element_align,
    };

    return vector;
}

void cx_vector_destroy(cx_vector* vector) {
    if(vector == NULL) {
        return;
    }

    if(vector->data != NULL) {
        size_t data_size = vector->capacity * vector->elem_size;
        cx_dealloc(vector->allocator, vector->data, data_size, vector->elem_align);
    }

    cx_dealloc(vector->allocator, vector, sizeof(cx_vector), _Alignof(cx_vector));
}

size_t cx_vector_length(const cx_vector* vector) {
    if(vector == NULL) {
        return 0;
    }

    return vector->length;
}

size_t cx_vector_capacity(const cx_vector* vector) {
    if(vector == NULL) {
        return 0;
    }

    return vector->capacity;
}

bool cx_vector_is_empty(const cx_vector* vector) {
    return vector == NULL || vector->length == 0;
}

bool cx_vector_push_ptr(cx_vector* vector, const void* value) {
    if (vector == NULL || value == NULL) {
        return false;
    }

    if(vector->length == vector->capacity) {
        if(vector->length == SIZE_MAX) {
            return false;
        }

        size_t required = vector->length + 1;
        size_t new_capacity = cx_vector_grow_capacity(vector, required);

        if(!cx_vector_resize_storage(vector, new_capacity)) {
            return false;
        }
    }

    size_t offset;
    if(!cx_checked_mul(vector->length, vector->elem_size, &offset)) {
        return false;
    }

    memcpy(vector->data + offset, value, vector->elem_size);
    vector->length++;

    return true;
}

void* cx_vector_get(cx_vector* vector, size_t index) {
    if(vector == NULL || index >= vector->length) {
        return NULL;
    }

    size_t offset;
    if(!cx_checked_mul(index, vector->elem_size, &offset)) {
        return NULL;
    }

    return vector->data + offset;
}

const void* cx_vector_get_const(const cx_vector* vector, size_t index) {
    if (vector == NULL || index >= vector->length) {
        return NULL;
    }

    size_t offset;

    if (!cx_checked_mul(index, vector->elem_size, &offset)) {
        return NULL;
    }

    return vector->data + offset;
}

void cx_vector_clear(cx_vector* vector) {
    if(vector == NULL) {
        return;
    }

    vector->length = 0;
}

bool cx_vector_reserve(cx_vector* vector, size_t capacity) {
    if(vector == NULL) {
        return false;
    }

    // vector already has enough capacity
    if(capacity <= vector->capacity) {
        return true;
    }

    return cx_vector_resize_storage(vector, capacity);
}

bool cx_vector_shrink_to_fit(cx_vector* vector) {
    if(vector == NULL) {
        return false;
    }

    // vector already small enough
    if(vector->length == vector->capacity) {
        return true;
    }

    return cx_vector_resize_storage(vector, vector->length);
}

bool cx_vector_pop(cx_vector* vector, void* out_value) {
    if(vector == NULL || vector->length == 0) {
        return false;
    }

    size_t index = vector->length - 1;
    size_t offset;
    if(!cx_checked_mul(index, vector->elem_size, &offset)) {
        return false;
    }

    if(out_value != NULL) {
        memcpy(out_value, vector->data + offset, vector->elem_size);
    }

    vector->length--;
    return true;
}

bool cx_vector_remove(cx_vector* vector, size_t index) {
    if (vector == NULL || index >= vector->length) {
        return false;
    }

    size_t offset;
    if (!cx_checked_mul(index, vector->elem_size, &offset)) {
        return false;
    }

    size_t elements_after = vector->length - index - 1;

    if (elements_after > 0) {
        size_t bytes_after;

        if (!cx_checked_mul(elements_after, vector->elem_size, &bytes_after)) {
            return false;
        }

        memmove(
            vector->data + offset,
            vector->data + offset + vector->elem_size,
            bytes_after
        );
    }

    vector->length--;

    return true;
}

bool cx_vector_insert_ptr(cx_vector* vector, size_t index, const void* value) {
    if (vector == NULL || value == NULL || index > vector->length) {
        return false;
    }

    if (vector->length == SIZE_MAX) {
        return false;
    }

    size_t required = vector->length + 1;

    if (required > vector->capacity) {
        size_t new_capacity = cx_vector_grow_capacity(vector, required);

        if (!cx_vector_resize_storage(vector, new_capacity)) {
            return false;
        }
    }

    size_t offset;
    if (!cx_checked_mul(index, vector->elem_size, &offset)) {
        return false;
    }

    size_t elements_after = vector->length - index;

    if (elements_after > 0) {
        size_t bytes_after;

        if (!cx_checked_mul(elements_after, vector->elem_size, &bytes_after)) {
            return false;
        }

        memmove(
            vector->data + offset + vector->elem_size,
            vector->data + offset,
            bytes_after
        );
    }

    memcpy(vector->data + offset, value, vector->elem_size);

    vector->length++;

    return true;
}

cx_slice cx_vector_as_slice(const cx_vector* vector) {
    if (vector == NULL) {
        return (cx_slice) { 0 };
    }

    return (cx_slice) {
        .data = vector->data,
        .length = vector->length,
        .elem_size = vector->elem_size,
    };
}

typedef struct {
    const cx_vector* vector;
    size_t index;
    bool started;
} cx_vector_iter_ctx;

static bool cx_vector_iter_next(cx_iter* iter) {
    cx_vector_iter_ctx* ctx = iter->ctx;

    if (ctx == NULL || ctx->vector == NULL) {
        return false;
    }

    if (!ctx->started) {
        ctx->started = true;
        return ctx->index < ctx->vector->length;
    }

    ctx->index++;

    return ctx->index < ctx->vector->length;
}

static const void* cx_vector_iter_get(const cx_iter* iter) {
    const cx_vector_iter_ctx* ctx = iter->ctx;

    if (ctx == NULL ||
        ctx->vector == NULL ||
        ctx->index >= ctx->vector->length) {
        return NULL;
    }

    size_t offset;

    if (!cx_checked_mul(ctx->index, ctx->vector->elem_size, &offset)) {
        return NULL;
    }

    return ctx->vector->data + offset;
}

static void cx_vector_iter_destroy(cx_iter* iter) {
    cx_vector_iter_ctx* ctx = iter->ctx;

    if (ctx == NULL || ctx->vector == NULL) {
        return;
    }

    cx_dealloc(ctx->vector->allocator, ctx, sizeof(cx_vector_iter_ctx), _Alignof(cx_vector_iter_ctx));

    iter->ctx = NULL;
}

cx_iter cx_vector_iter(const cx_vector* vector) {
    if (vector == NULL) {
        return (cx_iter){0};
    }

    cx_vector_iter_ctx* ctx = cx_alloc(vector->allocator, sizeof(cx_vector_iter_ctx), _Alignof(cx_vector_iter_ctx));

    if (ctx == NULL) {
        return (cx_iter){0};
    }

    *ctx = (cx_vector_iter_ctx) {
        .vector = vector,
        .index = 0,
        .started = false,
    };

    return (cx_iter) {
        .ctx = ctx,
        .next = cx_vector_iter_next,
        .get = cx_vector_iter_get,
        .destroy = cx_vector_iter_destroy,
    };
}